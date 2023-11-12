/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_timer.h"


#define BUTTON_PIN 17
#define DEBOUNCE_TIME 50

static int student_id_delay = 1000;
static int esp32_delay = 10;

enum BUTTON
{
    RELEASE = 0,
    PRESSED = 1
};

int BUTTON_STATE = 0;
int lastFlickerableState = 0;
unsigned long lastDebounceTime = 0;

TaskHandle_t xHandle = NULL;

void student_id_task(void *pvParameter)
{
    while (1)
    {
        printf("2011213\n");
        vTaskDelay(student_id_delay / portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}

void esp32_task(void *pvParameter)
{
    printf("Hello world!\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    uint32_t flash_size;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), %s%s%s%s, ",
           CONFIG_IDF_TARGET,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
           (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
           (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

    unsigned major_rev = chip_info.revision / 100;
    unsigned minor_rev = chip_info.revision % 100;
    printf("silicon revision v%d.%d, ", major_rev, minor_rev);
    if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
        printf("Get flash size failed");
        return;
    }

    printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());


    while (1)
    {
        if (gpio_get_level(BUTTON_PIN) != lastFlickerableState)
        {
            // reset the debouncing timer
            lastDebounceTime = esp_timer_get_time();
            // save the the last flickerable state
            lastFlickerableState = gpio_get_level(BUTTON_PIN);
        }

        if ((esp_timer_get_time() - lastDebounceTime) > DEBOUNCE_TIME)
        {
            switch (BUTTON_STATE)
            {
            case RELEASE:
                if (gpio_get_level(BUTTON_PIN) == 0)
                {
                    BUTTON_STATE = PRESSED;
                    printf("ESP32\n");
                }
                break;
            case PRESSED:
                if (gpio_get_level(BUTTON_PIN) == 1)
                {
                    BUTTON_STATE = RELEASE;
                }
                break;
            }
            vTaskDelay(esp32_delay / portTICK_PERIOD_MS);
        }
    }

    vTaskDelete(NULL);
}

void gpio_init()
{
    gpio_reset_pin(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);

}

void app_main(void)
{
    gpio_init();

    xTaskCreate(&student_id_task, "student_id_task", 2048, NULL, 3, NULL);
    xTaskCreate(&esp32_task, "esp32_task", 2048, NULL, 9, NULL);
}