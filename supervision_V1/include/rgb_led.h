#ifndef MAIN_RGB_LED_H_
#define MAIN_RGB_LED_H_

// RGB LED GPIOs
#define RGB_LED_RED_GPIO		21
#define RGB_LED_GREEN_GPIO		22
#define RGB_LED_BLUE_GPIO		23

// RGB LED color mix channels
#define RGB_LED_CHANNEL_NUM		3

// RGB LED configuration
typedef struct
{
	int channel;
	int gpio;
	int mode;
	int timer_index;
} ledc_info_t;
ledc_info_t ledc_ch[RGB_LED_CHANNEL_NUM];

/**
 * Set a custom color send by Gateway
 */
void custom_led(uint8_t red, uint8_t green, uint8_t blue);

/**
 * Color to indicate WiFi application has started.
 */
void rgb_led_wifi_app_started(void);

/**
 * Color to indicate that the ESP32 is connected to an access point.
 */
void rgb_led_wifi_connected(void);

/**
 * Color to indicate that the ESP32 is failed to connect to an acces point.
 */
void rgb_led_wifi_failed(void);


/**
 * Stop LEDs
 * 
 */
void rgb_led_stop(void);

#endif /* MAIN_RGB_LED_H_ */
