#ifndef MAIN_WIFI_APP_H_
#define MAIN_WIFI_APP_H_

#include "esp_netif.h"

//#define EXAMPLE_ESP_WIFI_SSID      "SFR_3440"
//#define EXAMPLE_ESP_WIFI_PASS      "ruyp6yxk88jnqn2tbm27"
#define EXAMPLE_ESP_WIFI_SSID      "SceneSonoreGateway"
#define EXAMPLE_ESP_WIFI_PASS      "motdepasse"
#define EXAMPLE_ESP_MAXIMUM_RETRY  25

void wifi_init_sta(void);

#endif /* MAIN_WIFI_APP_H_ */