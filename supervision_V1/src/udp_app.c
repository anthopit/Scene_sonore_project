#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#include "wifi_app.h"
#include "udp_app.h"
#include "utilitaires.h"
#include "gpio_num.h"
#include "rgb_led.h"

static const char *TAG = "example";

void udp_server_task(void *pvParameters)
{
    char rx_buffer[128];
    char addr_str[128];
    int addr_family = (int)pvParameters;
    int ip_protocol = 0;
    struct sockaddr_in6 dest_addr;

    while (1)
    {

        if (addr_family == AF_INET)
        {
            struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *)&dest_addr;
            dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
            dest_addr_ip4->sin_family = AF_INET;
            dest_addr_ip4->sin_port = htons(PORT);
            ip_protocol = IPPROTO_IP;
        }
        else if (addr_family == AF_INET6)
        {
            bzero(&dest_addr.sin6_addr.un, sizeof(dest_addr.sin6_addr.un));
            dest_addr.sin6_family = AF_INET6;
            dest_addr.sin6_port = htons(PORT);
            ip_protocol = IPPROTO_IPV6;
        }

        int sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
        if (sock < 0)
        {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket created");

#if defined(CONFIG_EXAMPLE_IPV4) && defined(CONFIG_EXAMPLE_IPV6)
        if (addr_family == AF_INET6)
        {
            // Note that by default IPV6 binds to both protocols, it is must be disabled
            // if both protocols used at the same time (used in CI)
            int opt = 1;
            setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
            setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, &opt, sizeof(opt));
        }
#endif

        int err = bind(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (err < 0)
        {
            ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
        }
        ESP_LOGI(TAG, "Socket bound, port %d", PORT);

        while (1)
        {

            ESP_LOGI(TAG, "Waiting for data");
            struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
            socklen_t socklen = sizeof(source_addr);
            int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);

            // Error occurred during receiving
            if (len < 0)
            {
                ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
                break;
            }
            // Data received
            else
            {
                // Get the sender's ip address as string
                if (source_addr.ss_family == PF_INET)
                {
                    inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr, addr_str, sizeof(addr_str) - 1);
                }
                else if (source_addr.ss_family == PF_INET6)
                {
                    inet6_ntoa_r(((struct sockaddr_in6 *)&source_addr)->sin6_addr, addr_str, sizeof(addr_str) - 1);
                }

                /************************************Data processing received****************************************/

                rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string...
                ESP_LOGI(TAG, "Received %d bytes from %s:", len, addr_str);
                ESP_LOGI(TAG, "%s", rx_buffer);

                char **tokens;

                tokens = str_split(rx_buffer, ':');

                if (strcmp(*(tokens + 0), "INIT") == 0)
                {
                    char num_buffer[5];
                    get_gpio_num(num_buffer);
                    int err = sendto(sock, num_buffer, 5, 0, (struct sockaddr *)&source_addr, sizeof(source_addr));
                    if (err < 0)
                    {
                        ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                        break;
                    }
                }
                else if (strcmp(*(tokens + 0), "CONF") == 0)
                {
                    if (strcmp(*(tokens + 1), "0") == 0)
                    {

                        int err = sendto(sock, "Led éteinte", 12, 0, (struct sockaddr *)&source_addr, sizeof(source_addr));

                        if (err < 0)
                        {

                            ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                            break;
                        }
                    }
                    else
                    {
                        uint8_t red = 0;
                        uint8_t green = 0;
                        uint8_t blue = (uint8_t *)(tokens + 4);

                        ESP_LOGE(TAG, "%d:%d:%d", red, green, blue);

                        custom_led(red, green, blue);

                        int err = sendto(sock, *(tokens + 3), 3, 0, (struct sockaddr *)&source_addr, sizeof(source_addr));

                        if (err < 0)
                        {

                            ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                            break;
                        }
                    }
                }
                else if (strcmp(tokens[0], "CONF") != 0 && strcmp(tokens[0], "INIT") != 0)
                {
                    int err = sendto(sock, "ERREUR: Demande inconnue\n", 24, 0, (struct sockaddr *)&source_addr, sizeof(source_addr));
                    if (err < 0)
                    {
                        ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                        break;
                    }
                }

                if (tokens)
                {
                    int i;
                    for (i = 0; *(tokens + i); i++)
                    {
                        ESP_LOGI(TAG, "%s\n", *(tokens + i));
                        free(*(tokens + i));
                    }
                    printf("\n");
                    free(tokens);
                }
            }
        }

        if (sock != -1)
        {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
    vTaskDelete(NULL);
}