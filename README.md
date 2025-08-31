# Scriabin_CV-DMX

A two channel CV-DMX converter based on an Arduino Pro Mini microcontroller.

<img src="https://github.com/clarionut/Scriabin_CV-DMX/blob/main/Scriabin_CV-DMX.jpg" alt="image">

Named for the famously synaesthetic composer Scriabin, this module was designed to allow RGB LED PAR cans to be voltage controlled by an analogue modular synthesiser.
Each of the two channels can be controlled in the RGB (red, green, blue) or HSV (hue, saturation, value) colourspace, giving three control inputs per channel.
Each individual input has an attenuator for the CV input and an initial value potentiometer, so all colours are accessible without using the CV inputs.
In RGB mode the inputs control the red, green and blue channels individually, simple to understand but harder to use. Consequently HSV is probably the more useful colourspace,
where setting the saturation and value initial values to maximum allows all hues to be accessed using just one CV input. This is also the reason for using HSV rather than
the HSL (hue, saturation, lightness) colourspace, where the fully saturated colours are found at 100% saturation, 50% lightness.

There is also a 'hidden' mode on each channel (hidden mainly because of lack of space on the panel!). With the mode switch in the centre-off position the corresponding
channel will enter 'Scriabin mode' which converts the incoming Red/Hue CV into quantised light colours according to the scheme used by Scriabin in his ['Clavier à lumières'](https://en.wikipedia.org/wiki/Clavier_%C3%A0_lumi%C3%A8res).
The colours are: C - red; C# - purple; D - yellow; D# - flesh; E - sky blue; F - deep red; F# - bright blue; G - orange; G# - lilac; A - green; A# - rose; B - blue.
In this mode the input CV range should be adjusted so that successive octaves give the same colour. The tracking isn't perfect but should be good over about 2 1/2 octaves.
The Green/Saturation control has no effect in this mode but the Blue/Value control can be used to fade the light in and out, though the colours are increasingly inaccurate at
low brightness.

The hardware is based on an Arduino Pro Mini driving a MAX485 RS-485 transceiver chip, as used in some of the commercial Arduino DMX shields.
The CV inputs have protection against out-of-range voltage inputs, but there is no isolation on the MAX485 (again like many of the commercial DMX shields).
Consequently the synthesiser and the light units must be powered from the same electrical circuit to avoid potentially dangerous voltages entering the synth.

The KiCad schematic and board layout use some custom symbols and footprints, so you will need to add my [kiCad_libraries](https://github.com/clarionut/kiCad_libraries)
before using these files.
