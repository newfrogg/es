#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_timer.h"

#define BUTTON_PIN 2
#define DEBOUNCE_TIME 50

static int student_id_delay = 1000;

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
        vTaskDelay(student_id_delay / portTICK_RATE_MS);
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
    printf("ESP32\n");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    xTaskCreate(&student_id_task, "student_id_task", 2048, NULL, 3, NULL);
}