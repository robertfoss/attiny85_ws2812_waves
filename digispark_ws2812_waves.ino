//#include <DigiCDC.h>

// use the cRGB struct hsv method
#define USE_HSV
#include <WS2812.h>

#define f(x) ((float)(x))
#define u(x) ((unsigned int)(x))

#define DATA_PIN_0 0
#define DATA_PIN_1 1
#define BTN_PIN    2

#define NUM_LEDS 22
WS2812 LED_0(NUM_LEDS);
WS2812 LED_1(NUM_LEDS);
uint32_t btn_last_high;
uint32_t btn_last_low;

static byte brightness = 128;
static int wave_time_div = 16;
static int color_rotation_speed = 5;



void setup()
{
    pinMode(BTN_PIN, INPUT);
    LED_0.setOutput(DATA_PIN_0);
    LED_1.setOutput(DATA_PIN_1);    
/*
    SerialUSB.begin(); 
    if (SerialUSB.available()) {
        SerialUSB.write("setup()");
    }
*/
}

void loop()
{
    wave_fx();
    delay(20);
/*
    SerialUSB.println("CDC Test");
*/
}


void read_button(byte *brightness)
{
/*
    uint32_t now = millis();
    if (digitalRead(BTN_PIN) == LOW)
    {
        btn_last_low = millis();
    } else {
        btn_last_high = millis();
    }
    if (now - btn_last_low > 250 &&
        now - btn_last_high > 250)
    {

    }
*/
/*
    if (digitalRead(BTN_PIN) == LOW)
    {
        if (*brightness >= 255) {
            *brightness = 64;
        } 
        else if (*brightness + 64 > 255)
        {
            *brightness = 255;
        } else {
            *brightness += 64;
        }
    }
*/
}

void wave_fx()
{
//    read_button(&brightness);
    uint32_t time = millis();
    byte intensity = brightness;
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        uint8_t base_change = time / wave_time_div;
        uint8_t color_rotation_time = sin8(time / 12000);
        uint8_t color_rotation = i * color_rotation_speed * color_rotation_time / 255;
        uint8_t wave = sin8(base_change + color_rotation);

        cRGB value;
        value.SetHSV(wave, 255, intensity);
        LED_0.set_crgb_at(i, value);
        LED_1.set_crgb_at(i, value);
    }
    LED_0.sync();
    LED_1.sync();
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
