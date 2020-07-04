#include "motor.h"

extern  TIM_HandleTypeDef htim5;

static stm32_motor motor_1={GPIOC,
							GPIO_PIN_1,
							GPIO_PIN_3,
							&htim5,
							TIM_CHANNEL_4};

static stm32_motor motor_2={GPIOE,
							GPIO_PIN_2,
							GPIO_PIN_4,
							&htim5,
							TIM_CHANNEL_1};

static stm32_motor motor_3={GPIOE,
							GPIO_PIN_3,
							GPIO_PIN_5,
							&htim5,
							TIM_CHANNEL_2};

static stm32_motor motor_4={GPIOC,
							GPIO_PIN_0,
							GPIO_PIN_2,
							&htim5,
							TIM_CHANNEL_3};

static uint16_t speed_1 = 0;
static uint16_t speed_2 = 0;
static uint16_t speed_3 = 0;
static uint16_t speed_4 = 0;


static void go_front();
static void go_right();
static void go_left();
static void go_back();
static void go_cw();
static void go_ucw();

static void stop();
static void spin(stm32_motor *motor, uint16_t speed, int8_t inv);

void update_motors(MOTOR_STATUS *status)
{
	if(status == NULL) return;
	switch(*status)
	{
		case MOTOR_STOP:
		{
			stop();
			break;
		}
		case GO_FRONT:
		{
			go_front();
			break;
		}
		case GO_BACK:
		{
			go_back();
			break;
		}
		case GO_RIGHT:
		{
			go_right();
			break;
		}
		case GO_LEFT:
		{
			go_left();
			break;
		}
                case GO_CW:
		{
			go_cw();
			break;
		}
                case GO_UCW:
		{
			go_ucw();
			break;
		}
		default: break;
	}
}

void motor_init()
{
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_4);
	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_1,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_SET);
	
	speed_1 = 70;
	speed_2 = 70;
	speed_3 = 70;
	speed_4 = 70;
}


static void spin(stm32_motor *motor, uint16_t speed, int8_t inv)
{
	if(motor == NULL)return;
	
	if(speed<=30)
	{
		HAL_GPIO_WritePin(motor->dir_port,motor->pin_1,GPIO_PIN_SET);
		HAL_GPIO_WritePin(motor->dir_port,motor->pin_2,GPIO_PIN_SET);
	}
	else if(inv == 1)
	{
		HAL_GPIO_WritePin(motor->dir_port,motor->pin_1,GPIO_PIN_SET);
		HAL_GPIO_WritePin(motor->dir_port,motor->pin_2,GPIO_PIN_RESET);
	}
	else if(inv == 0)
	{
		HAL_GPIO_WritePin(motor->dir_port,motor->pin_1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(motor->dir_port,motor->pin_2,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(motor->dir_port,motor->pin_1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(motor->dir_port,motor->pin_2,GPIO_PIN_RESET);
	}
	
	__HAL_TIM_SET_COMPARE(motor->tim,motor->channel,speed);
}						


/*
2  1

3  4
*/

static void go_front()
{
    spin(&motor_1,speed_1,1);
    spin(&motor_2,speed_2,1);
    spin(&motor_3,speed_3,1);
    spin(&motor_4,speed_4,1);
}

static void go_right()
{
    spin(&motor_1,speed_1,0);
    spin(&motor_2,speed_2,1);
    spin(&motor_3,speed_3,0);
    spin(&motor_4,speed_4,1);
}	
static void go_left()
{
    spin(&motor_1,speed_1,1);
    spin(&motor_2,speed_2,0);
    spin(&motor_3,speed_3,1);
    spin(&motor_4,speed_4,0);
}

static void go_back()
{
    spin(&motor_1,speed_1,0);
    spin(&motor_2,speed_2,0);
    spin(&motor_3,speed_3,0);
    spin(&motor_4,speed_4,0);
}	
static void stop()
{
    spin(&motor_1,0,0);
    spin(&motor_2,0,0);
    spin(&motor_3,0,0);
    spin(&motor_4,0,0);
}	

static void go_cw()
{
    spin(&motor_1,speed_1,0);
    spin(&motor_2,speed_2,1);
    spin(&motor_3,speed_3,1);
    spin(&motor_4,speed_4,0);
}
static void go_ucw()
{
    spin(&motor_1,speed_1,1);
    spin(&motor_2,speed_2,0);
    spin(&motor_3,speed_3,0);
    spin(&motor_4,speed_4,1);
}
