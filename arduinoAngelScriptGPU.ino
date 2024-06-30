// Double-buffered 8-bit Adafruit_GFX-compatible framebuffer for PicoDVI.
// Animates without redraw flicker. Requires Adafruit_GFX >= 1.11.4

#include <PicoDVI.h>

// Here's how a 320x240 8-bit (color-paletted) framebuffer is declared.
// Second argument ('true' here) enables double-buffering for flicker-free
// animation. Third argument is a hardware configuration -- examples are
// written for Adafruit Feather RP2040 DVI, but that's easily switched out
// for boards like the Pimoroni Pico DV (use 'pimoroni_demo_hdmi_cfg') or
// Pico DVI Sock ('pico_sock_cfg').
DVIGFX8 display(DVI_RES_320x240p60, true, adafruit_feather_dvi_cfg);

const uint16_t g_colorCount = 256;
String g_inText;

void setup()
{
  Serial1.begin(9600);

  if (not display.begin() || not Serial1) // Blink LED if insufficient RAM
  {
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;) digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }

  for (uint16_t index = 0; index < g_colorCount; ++index)
  {
    display.setColor(index, index, index, index);
  }

  display.swap(false, true); // Duplicate same palette into front & back buffers
}

void loop()
{
  display.fillScreen(0);
  
  display.fillRect(5, 5, 25, 25, 255);
  display.fillRect(25, 25, 25, 25, 127);

  display.setTextColor(255);
  display.setCursor(35, 10);
  display.print("Hello Arduino DVI!");
  
  if (Serial1.available() > 0)
  {
    g_inText = Serial1.readStringUntil('\0');
  }

  display.setCursor(35, 55);
  display.print(g_inText);

  display.swap();
}
