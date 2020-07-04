#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <stm32f4xx_hal.h>

typedef struct stm32_motor
{
  GPIO_TypeDef        *dir_port;
  uint16_t            pin_1;
  uint16_t            pin_2;
  TIM_HandleTypeDef   *tim;
  uint8_t             channel;
}stm32_motor;

typedef enum{
  MOTOR_STOP = 0,
  GO_FRONT, 
  GO_BACK,
  GO_RIGHT,
  GO_LEFT
}MOTOR_STATUS;

void motor_init();
void update_motors(MOTOR_STATUS *status);

#endif /* __MOTOR_H__ */
