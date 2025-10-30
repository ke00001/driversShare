#include <rtthread.h>
#include "st7920_rt.h"

static int st7920_example_app(void)
{
    if (st7920_init_from_config() != RT_EOK)
    {
        rt_kprintf("[st7920] init failed\n");
        return -1;
    }

    st7920_clear_text();
    st7920_demo_ascii();

    st7920_clear_gdram();
    st7920_demo_gdram_grid(0xAA, 0x55);

    rt_kprintf("[st7920] demo done\n");
    return 0;
}
INIT_APP_EXPORT(st7920_example_app);
