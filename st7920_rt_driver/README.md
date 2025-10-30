# ST7920 128x64 LCD Driver for RT-Thread Nano

A minimal, self-contained RT-Thread Nano component for ST7920/AIP31020-compatible 128x64 LCDs
(3-wire serial). It uses `rt_pin_*`, `rt_hw_us_delay`, and `rt_thread_mdelay`.

## Directory
```
st7920_rt_driver/
├── inc/
│   ├── st7920_rt.h
│   └── st7920_rt_cfg.h
├── src/
│   └── st7920_rt.c
├── examples/
│   └── app_lcd.c
└── SConscript
```

## Quick Start

1. Copy the whole `st7920_rt_driver` folder into your project (e.g. at project root).
2. Edit `inc/st7920_rt_cfg.h` to match your actual pins.
3. Include this component from your project's `SConscript` by adding:

```python
# In your project root SConscript:
# (Assumes this file is located next to your SConscript in 'st7920_rt_driver')
src_group = []
group = Glob('st7920_rt_driver/src/*.c')
CPPPATH = ['st7920_rt_driver/inc']
src_group += group
AddIncludePath(CPPPATH)
# (If your project uses groups:)
group_st7920 = DefineGroup('st7920_rt', group, depend = [], CPPPATH = CPPPATH)
```

> If your BSP uses a central component collection (e.g., `components` list), append
> `group_st7920` to that list or merge `group` and `CPPPATH` into your project’s
> build system as appropriate.

4. Build & flash. On boot you should see a basic text+GDRAM demo from `examples/app_lcd.c`.
   If you already have an `applications/` directory, either replace the template main
   or call `st7920_example_app()` via `INIT_APP_EXPORT` as shown.

## Notes
- PSB must be **LOW** for serial mode (or left unconnected if your module hard-wires it).
- Supply voltage: many modules support 3.3V logic; check your specific panel.
- Backlight usually runs at 5V; keep power domains in mind.

License: MIT


## LCD 端引脚速查（依据 ST7920/SMR12864-15 常见 20P 接口）

> 你的项目 PDF（如《ST7920中文说明》《SMR12864-15中文资料》《SMR12864-15A》）给出的命名通常与下表一致：
> RS/CS、R/W(SID)、E(SCLK)、PSB、/RST。PSB=0 为串口模式。

| # | 名称      | 说明                                   | 备注                          |
|---|-----------|----------------------------------------|-------------------------------|
| 1 | VSS       | 数字地                                 | 接 GND                        |
| 2 | VDD       | 逻辑电源                               | 3.3V/5V（看模块标称）         |
| 3 | V0        | 对比度输入（LCD 驱动）                 | 通常接电位器/模块 DC-DC 输出  |
| 4 | RS (CS)   | 寄存器选择/片选（串口为 **CS**）       | 本驱动的 `pin_cs`             |
| 5 | R/W (SID) | 读写/串行数据（串口为 **SID**）       | 本驱动的 `pin_sid`            |
| 6 | E (SCLK)  | 使能/串行时钟（串口为 **SCLK**）       | 本驱动的 `pin_sck`            |
| 7 | DB0       | 并口用                                 | 串口模式下不用                |
| 8 | DB1       | 并口用                                 | 串口模式下不用                |
| 9 | DB2       | 并口用                                 | 串口模式下不用                |
|10 | DB3       | 并口用                                 | 串口模式下不用                |
|11 | DB4       | 并口用                                 | 串口模式下不用                |
|12 | DB5       | 并口用                                 | 串口模式下不用                |
|13 | DB6       | 并口用                                 | 串口模式下不用                |
|14 | DB7       | 并口用                                 | 串口模式下不用                |
|15 | PSB       | 接地=串口；接 VDD=并口                 | 推荐 **接地**（串口）         |
|16 | NC/VSS    | 厂商不同，常为 NC 或地                 | 以模块丝印为准                |
|17 | /RST      | 低有效复位                             | 本驱动的 `pin_res`            |
|18 | VOUT      | 对比度升压输出（给 V0/电位器）        | 依模块典型应用                |
|19 | BLA       | 背光阳极                               | 一般 5V 通过限流/电阻         |
|20 | BLK       | 背光阴极                               | 接 GND                        |

### 串口 3 线 + 复位 + PSB 典型接线
- LCD **PSB → GND**
- LCD **RS(CS) → MCU GPIO（`ST7920_PIN_CS`）**
- LCD **R/W(SID) → MCU GPIO（`ST7920_PIN_SID`）**
- LCD **E(SCLK) → MCU GPIO（`ST7920_PIN_SCK`）**
- LCD **/RST → MCU GPIO（`ST7920_PIN_RES`）**
- VDD/VSS、BLA/BLK 按模块电源规范连接（注意背光多为 5V）

> 如果你的板卡把 PSB 已焊成串口模式，可将 `ST7920_PIN_PSB` 设为 `-1` 并无需接线。
