#include "st7920_rt.h"

#ifndef RT_USING_PIN
#error "RT_USING_PIN must be enabled in RT-Thread config"
#endif

static st7920_pins_t s_pins;

#define POUT(pin, v)   rt_pin_write((pin), (v))
#define PINH(pin)      POUT((pin), PIN_HIGH)
#define PINL(pin)      POUT((pin), PIN_LOW)

#define DELAY_US(us)   rt_hw_us_delay(us)
#define DELAY_MS(ms)   rt_thread_mdelay(ms)

static void _send8(rt_uint8_t v)
{
    for (int i = 0; i < 8; i++)
    {
        PINL(s_pins.pin_sck);
        DELAY_US(2);
        POUT(s_pins.pin_sid, (v & 0x80) ? PIN_HIGH : PIN_LOW);
        v <<= 1;
        PINH(s_pins.pin_sck);
        DELAY_US(2);
        PINL(s_pins.pin_sck);
        DELAY_US(2);
    }
}

static rt_uint8_t _recv8(void)
{
    rt_uint8_t val = 0;
    for (int i = 0; i < 8; i++)
    {
        val <<= 1;
        PINL(s_pins.pin_sck);
        DELAY_US(2);
        PINH(s_pins.pin_sck);
        DELAY_US(2);
        PINL(s_pins.pin_sck);
        DELAY_US(2);
        if (rt_pin_read(s_pins.pin_sid)) val |= 1;
    }
    return val;
}

static void _wait_ready(void)
{
    /* Busy flag read sequence: 0xFC then read two bytes; BF is MSB of first byte */
    do {
        _send8(0xFC);
    } while (_recv8() & 0x80);
}

static void _send_frame(rt_uint8_t start, rt_uint8_t byte)
{
    _send8(start);
    _send8(byte & 0xF0);
    _send8((byte << 4) & 0xF0);
}

void st7920_write_cmd(rt_uint8_t cmd)
{
    PINH(s_pins.pin_cs);
    _wait_ready();
    _send_frame(0xF8, cmd);
    PINL(s_pins.pin_cs);
}

void st7920_write_dat(rt_uint8_t dat)
{
    PINH(s_pins.pin_cs);
    _wait_ready();
    _send_frame(0xFA, dat);
    PINL(s_pins.pin_cs);
}

static void _hw_pins_config(const st7920_pins_t *pins)
{
    s_pins = *pins;
    rt_pin_mode(s_pins.pin_cs,  PIN_MODE_OUTPUT);
    rt_pin_mode(s_pins.pin_sid, PIN_MODE_OUTPUT);
    rt_pin_mode(s_pins.pin_sck, PIN_MODE_OUTPUT);
    rt_pin_mode(s_pins.pin_res, PIN_MODE_OUTPUT);
    if (s_pins.pin_psb >= 0) rt_pin_mode(s_pins.pin_psb, PIN_MODE_OUTPUT);

    PINL(s_pins.pin_cs);
    PINL(s_pins.pin_sck);
    PINL(s_pins.pin_sid);
    PINH(s_pins.pin_res);
    if (s_pins.pin_psb >= 0) PINL(s_pins.pin_psb); /* PSB=0 -> serial */
}

static void _st7920_boot_sequence(void)
{
    DELAY_MS(50);
    PINL(s_pins.pin_res); DELAY_MS(10);
    PINH(s_pins.pin_res); DELAY_MS(10);

    st7920_write_cmd(0x30); DELAY_MS(5);
    st7920_write_cmd(0x30); DELAY_MS(5);
    st7920_write_cmd(0x30); DELAY_MS(5);

    st7920_write_cmd(0x03); DELAY_MS(2);
    st7920_write_cmd(0x01); DELAY_MS(2);
    st7920_write_cmd(0x06); DELAY_MS(2);
    st7920_write_cmd(0x0C); DELAY_MS(2);
}

int st7920_init(const st7920_pins_t *pins)
{
    RT_ASSERT(pins);
    _hw_pins_config(pins);
    _st7920_boot_sequence();
    return RT_EOK;
}

int st7920_init_from_config(void)
{
    st7920_pins_t pins = {
        .pin_cs  = ST7920_PIN_CS,
        .pin_sid = ST7920_PIN_SID,
        .pin_sck = ST7920_PIN_SCK,
        .pin_res = ST7920_PIN_RES,
        .pin_psb = ST7920_PIN_PSB,
    };
    return st7920_init(&pins);
}

void st7920_clear_text(void)
{
    st7920_write_cmd(0x30);
    st7920_write_cmd(0x80);
    for (int i = 0; i < 64; i++) st7920_write_dat(0x20);
}

void st7920_clear_gdram(void)
{
    st7920_write_cmd(0x34);
    st7920_write_cmd(0x36);
    for (rt_uint8_t y = 0; y < 32; y++)
    {
        st7920_write_cmd(0x80 | y);
        st7920_write_cmd(0x80);
        for (rt_uint8_t i = 0; i < 32; i++) st7920_write_dat(0x00);
    }
}

void st7920_gdram_write_block(rt_uint8_t y, rt_uint8_t x8, rt_uint8_t hi, rt_uint8_t lo)
{
    rt_uint8_t y_sel = (y < 32) ? (0x80 | y) : (0x80 | (y - 32));
    rt_uint8_t x_sel = (y < 32) ? (0x80 | x8) : (0x88 | x8);

    st7920_write_cmd(0x34);
    st7920_write_cmd(y_sel);
    st7920_write_cmd(x_sel);
    st7920_write_cmd(0x30);
    st7920_write_dat(hi);
    st7920_write_dat(lo);
}

void st7920_put_bmp(const rt_uint8_t *bmp_1024)
{
    rt_uint16_t k = 0;
    st7920_write_cmd(0x34);
    st7920_write_cmd(0x36);
    for (rt_uint8_t y = 0; y < 32; y++)
    {
        st7920_write_cmd(0x80 | y);
        st7920_write_cmd(0x80);
        for (rt_uint8_t i = 0; i < 32; i++) st7920_write_dat(bmp_1024[k++]);
    }
}

void st7920_demo_ascii(void)
{
    st7920_write_cmd(0x80);
    for (rt_uint8_t c = 0x20; c < 0x7F; c++) st7920_write_dat(c);
}

void st7920_demo_gdram_grid(rt_uint8_t k, rt_uint8_t h)
{
    for (rt_uint8_t row = 0; row < 64; row++)
    {
        st7920_write_cmd(0x80 | (row & 31));
        st7920_write_cmd((row < 32) ? 0x80 : 0x88);
        for (rt_uint8_t i = 0; i < 16; i++)
        {
            st7920_write_dat(k);
            st7920_write_dat(h);
        }
    }
    st7920_write_cmd(0x34);
    DELAY_US(200);
    st7920_write_cmd(0x36);
    DELAY_US(200);
}
