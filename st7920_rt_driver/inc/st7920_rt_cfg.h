#ifndef __ST7920_RT_CFG_H__
#define __ST7920_RT_CFG_H__

/* === User Configuration ===
 * LCD 端关键信号命名（参考项目 PDF：ST7920/SMR12864-15）
 *   RS(CS)  -> ST7920_PIN_CS
 *   R/W(SID)-> ST7920_PIN_SID
 *   E(SCLK) -> ST7920_PIN_SCK
 *   /RST    -> ST7920_PIN_RES
 *   PSB     -> ST7920_PIN_PSB (串口=接地；并口=接VDD；若已焊死串口可设为 -1)
 *
 * LCD 20P 常见排针对照：
 *   1 VSS, 2 VDD, 3 V0, 4 RS, 5 R/W, 6 E, 7~14 DB0~DB7, 15 PSB, 16 NC/VSS, 17 /RST, 18 VOUT, 19 BLA, 20 BLK
 */
 * Fill in the actual pins according to your hardware.
 * Use GET_PIN(Port, Pin) macro from RT-Thread to map GPIO pins, e.g. GET_PIN(B, 12).
 * If your module has PSB fixed for serial mode, you may set ST7920_PIN_PSB to -1.
 */

#ifndef GET_PIN
#include <board.h>  /* Make sure GET_PIN is available from your BSP */
#endif

/* Default example mapping — CHANGE to your real wiring */
#ifndef ST7920_PIN_CS
#define ST7920_PIN_CS   GET_PIN(B, 12)   /* RS(CS) */
#endif

#ifndef ST7920_PIN_SID
#define ST7920_PIN_SID  GET_PIN(B, 11)   /* R/W(SID) */
#endif

#ifndef ST7920_PIN_SCK
#define ST7920_PIN_SCK  GET_PIN(B, 10)   /* E(SCLK) */
#endif

#ifndef ST7920_PIN_RES
#define ST7920_PIN_RES  GET_PIN(A,  1)   /* /RST */
#endif

#ifndef ST7920_PIN_PSB
#define ST7920_PIN_PSB  GET_PIN(A,  4)   /* PSB; set to -1 if hard-wired */
#endif

#endif /* __ST7920_RT_CFG_H__ */
