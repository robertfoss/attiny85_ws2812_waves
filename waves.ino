// use the cRGB struct hsv method
#define USE_HSV
#include <WS2812.h>


#define DATA_PIN 0

#define NUM_LEDS 22
WS2812 LED(NUM_LEDS);

int wave_scale = 100;

void setup()
{
    LED.setOutput(DATA_PIN);
}

void loop()
{
    wave_fx();
    delay(20);
}


void wave_fx()
{

    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        static uint16_t counter = 0;
        //this is the function that creates the wave effect
        //it may take time to fully understand what is happening here
        uint8_t wave = sin8(millis() / wave_scale) * i / 16 + counter/4;
        counter++;

        //populate the leds[x] with the temp[x] array hue's
        cRGB value;
        value.SetHSV(wave, 255, 192);
        LED.set_crgb_at(i, value);

    }
    LED.sync();
}

const uint8_t b_m16_interleave[] = { 0, 49, 49, 41, 90, 27, 117, 10 };
uint8_t sin8( uint8_t theta)
{
    uint8_t offset = theta;
    if( theta & 0x40 ) {
        offset = (uint8_t)255 - offset;
    }
    offset &= 0x3F; // 0..63

    uint8_t secoffset  = offset & 0x0F; // 0..15
    if( theta & 0x40) secoffset++;

    uint8_t section = offset >> 4; // 0..3
    uint8_t s2 = section * 2;
    const uint8_t* p = b_m16_interleave;
    p += s2;
    uint8_t b   =  *p;
    p++;
    uint8_t m16 =  *p;

    uint8_t mx = (m16 * secoffset) >> 4;

    int8_t y = mx + b;
    if( theta & 0x80 ) y = -y;

    y += 128;

    return y;
}
