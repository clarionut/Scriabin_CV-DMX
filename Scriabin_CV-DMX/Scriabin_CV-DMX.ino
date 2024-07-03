/*
    Script to drive the Scriabin CV-DMX module

    The DMX_BASE and DMX_GAP defines below assume that the lamp(s) on the first channel
    start at DMX address 1 and those on the second channel start at address 9.
    BR_OFF is the offset of the overall brightness control from DMX_BASE, and
    R_OFF, G_OFF & B_OFF are the offsets from the base address of the individual
    red, green and blue controls in your lamps

    Please change these values if your DMX setup starts at a different address, and/or your
    lamps each require a larger range of, or different, addresses compared with mine.

    Analogue reads are performed twice to allow settling time in the Arduino's analogue
    multiplexer in order to reduce bleedthrough from one analogue input to the next.

    C G Earnshaw 16-11-23
*/

#include <DmxSimple.h>

#define DMX_BASE 1
#define DMX_GAP 8
#define BR_OFF 0
#define R_OFF 1
#define G_OFF 2
#define B_OFF 3

uint8_t chnl, anlg, addr;
long Red[2], Grn[2], Blu[2], H, S, V;
uint16_t oldRed[2] = {9999};
uint16_t oldGrn[2] = {9999};
uint16_t oldBlu[2] = {9999};
uint8_t scriabinClr[][3] = {
  { 0xFF, 0x00, 0x00 }, // C
  { 0xA0, 0x00, 0xFF }, // C#/Db
  { 0xFF, 0x72, 0x00 }, // D
  { 0xFF, 0x75, 0x6C }, // D#/Eb
  { 0x00, 0xFF, 0xF6 }, // E
  { 0x70, 0x00, 0x00 }, // F
  { 0x46, 0x00, 0xFF }, // F#/Gb
  { 0xFF, 0x56, 0x00 }, // G
  { 0xD7, 0x4B, 0xD8 }, // G#/Ab
  { 0x00, 0xFF, 0x00 }, // A
  { 0xFF, 0x57, 0x54 }, // A#/Bb
  { 0x00, 0x56, 0xFF }  // B/Cb
};


void setup () {
  DmxSimple.usePin(4);
  DmxSimple.maxChannel(DMX_GAP * 2 + DMX_BASE);

  // Pins for HSV/RGB switches
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);

  Serial.begin(115200);
}

void loop () {
  // Check Mode switch
  anlg = 0;
  for (uint8_t i = 0; i < 2; i++) {
    addr = DMX_BASE + i * DMX_GAP;
    if (0 == digitalRead(5 + i)) {
      // RGB mode - read RGB directly  DmxSimple.write(DMX_BASE + BR_OFF, 255);
      DmxSimple.write(addr + BR_OFF, 255);
      analogRead(anlg);
      Red[i] = analogRead(anlg) >> 2;
      analogRead(++anlg);
      Grn[i] = analogRead(anlg) >> 2;
      analogRead(++anlg);
      Blu[i] = analogRead(anlg) >> 2;

    } else if (0 == digitalRead(7 + i)) {
      // HSV mode - read H S & V & convert to RGB
      DmxSimple.write(addr + BR_OFF, 255);
      analogRead(anlg);
      H = analogRead(anlg);
      analogRead(++anlg);
      S = analogRead(anlg);
      analogRead(++anlg);
      V = analogRead(anlg);
      HSV_2_RGB(H, S, V, &Red[i], &Grn[i], &Blu[i]);

    } else {
      // 'hidden' Scriabin mode
      analogRead(anlg);
      int tmp = ((analogRead(anlg) % 205) / 17) % 12;
      anlg += 2;
      analogRead(anlg);
      Red[i] = scriabinClr[tmp][0];
      Grn[i] = scriabinClr[tmp][1];
      Blu[i] = scriabinClr[tmp][2];
      DmxSimple.write(addr + BR_OFF, analogRead(anlg) / 4);
    }

    if (Red[i] != oldRed[i]) {
      DmxSimple.write(addr + R_OFF, Red[i]);
      oldRed[i] = Red[i];
    }
    if (Grn[i] != oldGrn[i]) {
      DmxSimple.write(addr + G_OFF, Grn[i]);
      oldGrn[i] = Grn[i];
    } 
    if (Blu[i] != oldBlu[i]) {
      DmxSimple.write(addr + B_OFF, Blu[i]);
      oldBlu[i] = Blu[i];
    }
    anlg = 3;
  }
}

void HSV_2_RGB(long hue, long sat, long val, long *RP, long *GP, long *BP) {
  // Calculate RGB values
  long R, G, B;
  if (0 == val) {
    // All channels are 0
    R = G = B = 0;

  } else if (0 == sat) {
    // 0 saturation => all channels are equal to V
    R = G = B = val;

  } else {
  // 1 - convert Hue into fully saturated RGB values
  // 2 - apply conversion for Saturation
  // 3 - reduce values according to Value
    uint16_t Satn = (1023 - sat);
    if (hue < 170) {  // 0 -> 169
      // Pure Red -> Yellow
      R = 1023;
      G = 1023 * hue / 170;
      B = Satn;

      G += (1023 - G) * Satn / 1023;

    } else if (hue < 341) {  // 170 - 340
      // Pure Yellow -> Green
      R = 1023 - 1023 * (hue - 170) / 171;
      G = 1023;
      B = Satn;

      R += (1023 - R) * Satn / 1023;

    } else if (hue < 512) {  // 341 -> 511
      // Pure Green -> Cyan
      R = Satn;
      G = 1023;
      B = 1023 * (hue - 341) / 171;

      B += (1023 - B) * Satn / 1023;

    } else if (hue < 682) {  // 512 -> 681
      // Pure Cyan -> Blue
      R = Satn;
      G = 1023 - 1023 * (hue - 512) / 170;
      B = 1023;

      G += (1023 - G) * Satn / 1023;

    } else if (hue < 853) {  // 682 -> 852
      // Pure Blue -> Magenta
      R = 1023 * (hue - 682) / 171;
      G = Satn;
      B = 1023;

      R += (1023 - R) * Satn / 1023;

    } else {  // (hue < 1024): 853 -> 1023
      // Pure Magenta -> Red
      R = 1023;
      G = Satn;
      B = 1023 - 1023 * (hue - 853) / 171;

      B += (1023 - B) * Satn / 1023;
    }
  }
  *RP = R * val / 1023 / 4;
  *GP = G * val / 1023 / 4;
  *BP = B * val / 1023 / 4;
}
