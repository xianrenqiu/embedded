#ifndef __GPIO_H__
#define __GPIO_H__

#include "config.h"

#if defined(CONFIG_LED_ENABLE)

#include <stdint.h>

typedef enum 
{
  LED1 = 0,
} led_typedef;

void led_init(led_typedef led);
void led_deinit(led_typedef led);
void led_on(led_typedef led);
void led_off(led_typedef led);
void led_toggle(led_typedef led);

#endif

#endif /* __GPIO_H__ */
