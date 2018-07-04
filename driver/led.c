#include "config.h"

#if defined(CONFIG_LED_ENABLE)

#include <stdint.h>

#include "led.h"

#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_gpio.h"

#define LEDN 1
#define LEDx_GPIO_CLK_ENABLE(__INDEX__)   do { if((__INDEX__) == 0) __HAL_RCC_GPIOC_CLK_ENABLE();} while(0)
#define LEDx_GPIO_CLK_DISABLE(__INDEX__)  (((__INDEX__) == 0) ? __HAL_RCC_GPIOC_CLK_DISABLE() : 0)

GPIO_TypeDef* LED_PORT[LEDN] = {GPIOC};
const uint16_t LED_PIN[LEDN] = {GPIO_PIN_13};

/**
  * @brief  Configures LED GPIO.
  * @param  led: led to be configured. 
  *          This parameter can be one of the following values:
  *     @arg LED2
  */
void led_init(led_typedef led)
{
  GPIO_InitTypeDef  gpioinitstruct;
  
  /* Enable the GPIO_LED Clock */
  LEDx_GPIO_CLK_ENABLE(led);

  /* Configure the GPIO_LED pin */
  gpioinitstruct.Pin    = LED_PIN[led];
  gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
  gpioinitstruct.Pull   = GPIO_NOPULL;
  gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
  
  HAL_GPIO_Init(LED_PORT[led], &gpioinitstruct);

  /* Reset PIN to switch off the LED */
  HAL_GPIO_WritePin(LED_PORT[led],LED_PIN[led], GPIO_PIN_RESET);
}

/**
  * @brief  deinit LEDs.
  * @param  led: LED to be de-init. 
  *   This parameter can be one of the following values:
  *     @arg  LED2
  * @note led deinit does not disable the GPIO clock nor disable the Mfx 
  */
void led_deinit(led_typedef led)
{
  GPIO_InitTypeDef  gpio_init_structure;

  /* Turn off LED */
  HAL_GPIO_WritePin(LED_PORT[led],LED_PIN[led], GPIO_PIN_RESET);
  /* deinit the GPIO_LED pin */
  gpio_init_structure.Pin = LED_PIN[led];
  HAL_GPIO_DeInit(LED_PORT[led], gpio_init_structure.Pin);
}

/**
  * @brief  Turns selected LED on.
  * @param  led: Specifies the led to be set on. 
  *   This parameter can be one of following parameters:
  *     @arg LED2
  */
void led_on(led_typedef led)
{
  HAL_GPIO_WritePin(LED_PORT[led], LED_PIN[led], GPIO_PIN_SET); 
}

/**
  * @brief  Turns selected LED off.
  * @param  led: Specifies the led to be set off. 
  *   This parameter can be one of following parameters:
  *     @arg LED2
  */
void led_off(led_typedef led)
{
  HAL_GPIO_WritePin(LED_PORT[led], LED_PIN[led], GPIO_PIN_RESET); 
}

/**
  * @brief  toggles the selected LED.
  * @param  led: Specifies the led to be toggled. 
  *   This parameter can be one of following parameters:
  *            @arg  LED2
  */
void led_toggle(led_typedef led)
{
  HAL_GPIO_TogglePin(LED_PORT[led], LED_PIN[led]);
}


#endif