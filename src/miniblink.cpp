/**
 * \mainpage notitle
 * 
 * \section intro_sec Introduction
 * 
 * Introduction comments.
 */ 

// FreeRTOS includes
#include <FreeRTOS/FreeRTOS.h>
#include <FreeRTOS/task.h>
#include <FreeRTOS/queue.h>

// libopencm3 includes
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/cm3/nvic.h>

uint8_t buffer[32];

static void configureUSART1()
{
    rcc_periph_clock_enable(RCC_USART1);
    nvic_enable_irq(NVIC_USART1_IRQ);

    gpio_set_mode(GPIOA,
        GPIO_MODE_OUTPUT_50_MHZ,
        GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
        GPIO_USART1_TX);

    gpio_set_mode(GPIOA,
        GPIO_MODE_INPUT,
        GPIO_CNF_INPUT_FLOAT,
        GPIO_USART1_RX);

    usart_set_baudrate(USART1, 38400);
    usart_set_databits(USART1, 8);
    usart_set_stopbits(USART1, USART_STOPBITS_1);
    usart_set_mode(USART1, USART_MODE_TX_RX);
    usart_set_parity(USART1, USART_PARITY_NONE);
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
    USART_CR1(USART1) |= USART_CR1_RXNEIE;
    usart_enable(USART1);
}

static void setupHardware()
{
    
    /* Enable GPIOC clock. */
    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_GPIOA);

    /* Set GPIOC13 to output push-pull. */
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);

    configureUSART1();
}

void uart_putc(char ch)
{
    usart_send_blocking(USART1, ch);
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
        //toggleLED();
        //vTaskDelayUntil(&lastExecute, (5000 / portTICK_PERIOD_MS));
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

extern "C" void usartTask(void *pvParameters)
{
    uint16_t c = '0' - 1;

    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(200));
        c = usart_recv_blocking(USART1);
        toggleLED();
        usart_send_blocking(USART1, c);
        /*
        if (++c >= 'Z') {
            uart_putc('0');
            uart_putc('\r');
            uart_putc('\n');
            c = '0' - 1;
        } else {
            uart_putc('0');
        }
        */
    }
}

/**
 * EXAMPLE DOCS
 *
 * MORE EXAMPLE DOCS
 */
int main(void)
{
    setupHardware();
    toggleLED();
    //xTaskCreate(blinkTask, "Blink", configMINIMAL_STACK_SIZE, NULL, 10, NULL);
    //xTaskCreate(usartTask, "Usart", configMINIMAL_STACK_SIZE, NULL, 15, NULL);

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

extern "C" void usart1_isr(void)
{
    static uint8_t data = 'A';

    if (((USART_CR1(USART1) & USART_CR1_RXNEIE) != 0) &&
        ((USART_SR(USART1) & USART_SR_RXNE) != 0)) {
        
        gpio_toggle(GPIOC, GPIO13);
        data = usart_recv(USART1);

        USART_CR1(USART1) |= USART_CR1_TXEIE;
    }

    if (((USART_CR1(USART1) & USART_CR1_TXEIE) != 0) &&
        ((USART_SR(USART1) & USART_SR_TXE) != 0)) {
    
        usart_send(USART1, data);
        USART_CR1(USART1) &= ~USART_CR1_TXEIE;
    }
}
