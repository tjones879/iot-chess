// FreeRTOS includes
#include <FreeRTOS/FreeRTOS.h>
#include <FreeRTOS/task.h>
#include <FreeRTOS/queue.h>

// libopencm3 includes
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

static void setupHardware()
{
    /* Enable GPIOC clock. */
    rcc_periph_clock_enable(RCC_GPIOC);

    /* Set GPIOC13 to output push-pull. */
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
}

void toggleLED()
{
    gpio_toggle(GPIOC, GPIO13); /* LED on/off */
}

extern "C" void blinkTask(void *pvParameters)
{
    (void) pvParameters;
    auto lastExecute = xTaskGetTickCount();

    for ( ;; ) {
        toggleLED();
        //vTaskDelayUntil(&lastExecute, (5000 / portTICK_PERIOD_MS));
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

int main(void)
{
    int i;

    setupHardware();
    xTaskCreate(blinkTask, "Blink", configMINIMAL_STACK_SIZE, NULL, 10, NULL);

    vTaskStartScheduler();

    while (1) {
        /* Manually: */
        // GPIOC_BSRR = GPIO12;     /* LED off */
        // for (i = 0; i < 800000; i++) /* Wait a bit. */
        //  __asm__("nop");
        // GPIOC_BRR = GPIO12;      /* LED on */
        // for (i = 0; i < 800000; i++) /* Wait a bit. */
        //  __asm__("nop");

        /* Using API functions gpio_set()/gpio_clear(): */
        // gpio_set(GPIOC, GPIO12); /* LED off */
        // for (i = 0; i < 800000; i++) /* Wait a bit. */
        //  __asm__("nop");
        // gpio_clear(GPIOC, GPIO12);   /* LED on */
        // for (i = 0; i < 800000; i++) /* Wait a bit. */
        //  __asm__("nop");

        /* Using API function gpio_toggle(): */
        //gpio_toggle(GPIOC, GPIO13); /* LED on/off */
        //for (i = 0; i < 800000; i++)    /* Wait a bit. */
        //    __asm__("nop");
    }
}

extern "C" void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    (void) pxTask;
    (void) pcTaskName;
    while (1);
}
