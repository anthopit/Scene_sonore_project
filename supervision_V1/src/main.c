#include "nvs_flash.h"
#include "wifi_app.h"
#include "udp_app.h"

#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"

#include "driver/gpio.h"

#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_netif.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#include "gpio_num.h"



void app_main(void)
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    init_gpio_num();

    
    wifi_init_sta();

    ESP_ERROR_CHECK(esp_netif_init());
    xTaskCreate(udp_server_task, "udp_server", 4096, (void*)AF_INET, 5, NULL);
    
    /*****************************************************************************************
    At this stage the application is able to :
        - initialize four outputs with a pull-up resistor to create an physically an id number by connecting cables to outputs 
        - initialize an wifi connection with a wifi station (raspberry Pi in our case) (! The application is not yet able to detect a disconnection)
        - manage an RGB led
        - receive and respond a UDP, parse the packet to adapt his respond

    Next to do :
        - improve the wifi system
        - improve the parsing of UDP packet 
        - implement SPIFFS files system to store configuration send from the gateway during the use

    *****************************************************************************************/
}