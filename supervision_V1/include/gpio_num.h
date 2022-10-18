#ifndef MAIN_GPIO_NUM_H_
#define MAIN_GPIO_NUM_H_

#define GPIO_BIT_0  GPIO_NUM_17
#define GPIO_BIT_1  GPIO_NUM_5
#define GPIO_BIT_2  GPIO_NUM_18
#define GPIO_BIT_3  GPIO_NUM_19

#define ESP_INTR_FLAG_DEFAULT 0

void init_gpio_num(void);

void get_gpio_num(char *oui);

#endif