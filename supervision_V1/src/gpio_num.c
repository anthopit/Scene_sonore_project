#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"


#include "driver/gpio.h"

#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"

#include "gpio_num.h"

static const char *TAG = "NUM";


/*static void gpio_task_example(void* arg)
{

    while(1)
    {
         ESP_LOGI(TAG, "%d%d%d%d",
                 gpio_get_level(GPIO_BIT_3), gpio_get_level(GPIO_BIT_2), gpio_get_level(GPIO_BIT_1), gpio_get_level(GPIO_BIT_0));
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

}*/

void init_gpio_num(){
// Reset all pin use 
    gpio_reset_pin(GPIO_BIT_0);
    gpio_reset_pin(GPIO_BIT_1);
    gpio_reset_pin(GPIO_BIT_2);
    gpio_reset_pin(GPIO_BIT_3);

    //configuration of each gpiop

     gpio_config_t io_conf = {
         .pin_bit_mask = 0B000000000000011100000000000100000,
         .mode         = GPIO_MODE_INPUT,
         .pull_up_en   = 1,
         .pull_down_en = 0,
         .intr_type = GPIO_INTR_DISABLE
     };
     gpio_config(&io_conf);

    ESP_LOGI(TAG, "%d%d%d%d",
                 gpio_get_level(GPIO_BIT_3), gpio_get_level(GPIO_BIT_2), gpio_get_level(GPIO_BIT_1), gpio_get_level(GPIO_BIT_0));
     

    
    //xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);

}

void get_gpio_num(char *num_buffer){
    int bit0 = gpio_get_level(GPIO_BIT_0);
    int bit1 = gpio_get_level(GPIO_BIT_1);
    int bit2 = gpio_get_level(GPIO_BIT_2);
    int bit3 = gpio_get_level(GPIO_BIT_3);

    sprintf(num_buffer, "%d%d%d%d",bit3, bit2, bit1, bit0);
}