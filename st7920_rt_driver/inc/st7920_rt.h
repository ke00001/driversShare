#ifndef __ST7920_RT_H__
#define __ST7920_RT_H__

#include <rtthread.h>
#include <rtdevice.h>
#include "st7920_rt_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    rt_base_t pin_cs;   /* RS(CS) */
    rt_base_t pin_sid;  /* R/W(SID) */
    rt_base_t pin_sck;  /* E(SCLK) */
    rt_base_t pin_res;  /* /RST */
    rt_base_t pin_psb;  /* PSB (optional; -1 if not used) */
} st7920_pins_t;

/* Init using runtime pins */
int  st7920_init(const st7920_pins_t *pins);
/* Init using compile-time config from st7920_rt_cfg.h */
int  st7920_init_from_config(void);

/* Low-level API */
void st7920_write_cmd(rt_uint8_t cmd);
void st7920_write_dat(rt_uint8_t dat);

/* Helpers */
void st7920_clear_text(void);
void st7920_clear_gdram(void);
void st7920_put_bmp(const rt_uint8_t *bmp_1024);
void st7920_gdram_write_block(rt_uint8_t y, rt_uint8_t x8, rt_uint8_t hi, rt_uint8_t lo);

/* Demos */
void st7920_demo_ascii(void);
void st7920_demo_gdram_grid(rt_uint8_t k, rt_uint8_t h);

#ifdef __cplusplus
}
#endif
#endif /* __ST7920_RT_H__ */
