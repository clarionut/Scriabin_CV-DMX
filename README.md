# Scriabin_CV-DMX

A two channel CV-DMX converter based on an Arduino Pro Mini microcontroller.

Named for the famously synaesthetic composer Scriabin, this module was designed to allow RGB LED PAR cans to be voltage controlled by an analogue modular synthesiser.
Each of the two channels can be controlled in the RGB (red, green,blue) or HSV (hue, saturation, value) colourspace, giving three control inputs per channel.
Each individual input has an attenuator for the CV input and an initial value potentiometer, so all colours are accessible without using the CV inputs.
In RGB mode the inputs control the red, green and blue channels individually, simple to understand but harder to use. Consequently HSV is probably the more useful colourspace,
where setting the saturation and value initial values to maximum allows all hues to be accessed using just one CV input. This is also the reason for using HSV rather than
the HSL (hue, saturation, lightness) colourspace, where the fully saturated colours are found at 100% saturation, 50% lightness.

The hardware is based on an Arduino Pro Mini driving a MAX485 RS-485 transceiver chip, as used in some of the commercial Arduino DMX shields.
The CV inputs have protection against out-of-range voltage inputs, but there is no isolation on the MAX485 (again like many of the commercial DMX shields).
Consequently the synthesiser and the light units must be powered from the same electrical circuit to avoid potentially dangerous voltages entering the synth.
