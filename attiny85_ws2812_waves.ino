#include <FastLED.h>


#define DATA_PIN_0 0
#define DATA_PIN_1 1
#define BTN_PIN    2

#define NUM_LEDS 22

static const int wave_time_div = 16;
static const int color_rotation_speed = 8;
static const byte levels[] = { 20, 48, 64, 128, 192, 255 };
static const uint8_t nbr_levels = sizeof(levels)/sizeof(levels[0]);

CRGB leds[NUM_LEDS];


bool is_btn_pressed()
{
    static uint32_t last_change = 0;
    if (digitalRead(BTN_PIN) == LOW &&
        millis() - last_change > 250)
    {
      last_change = millis();
      return true;
    }
    return false;
}

uint8_t get_brightness(bool is_btn_pressed)
{
    static uint8_t curr_level = 0;

    if (is_btn_pressed) {
      curr_level = (curr_level + 1) % nbr_levels;
    }

    return levels[curr_level];
}

void setup()
{
    FastLED.addLeds<WS2812B, DATA_PIN_0>(leds, NUM_LEDS);
    FastLED.addLeds<WS2812B, DATA_PIN_1>(leds, NUM_LEDS);

    pinMode(BTN_PIN, INPUT);
}

void loop()
{
    wave_fx();
    delay(30);
}

void wave_fx()
{
    uint8_t brightness = get_brightness(is_btn_pressed());

    uint32_t time = millis();
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        uint8_t base_change = time / wave_time_div;
        uint8_t color_rotation_time = sin8(time / 250);
        uint8_t color_rotation = i * color_rotation_speed * color_rotation_time / 255;
        uint8_t hue = sin8(base_change + color_rotation);

        leds[i] = CHSV(hue, 255, brightness);
    }
    FastLED.show();
}
