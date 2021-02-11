#include "MKL25Z4.h"                    // Device header

#define RED_LED 18 // PORTB pin 18
#define GREEN_LED 19 // PORTB pin 19
#define BLUE_LED 1 // PORTD pin 1
#define MASK(x) (1 << (x))

enum color_t {RED, GREEN, BLUE};

static void delay(volatile uint32_t nof) {
    while (nof != 0) {
        __asm("NOP");
        nof--;
    }
}

void InitGPIO(void) {
    // Enable clock to PORTB and PORTD
    SIM->SCGC5 |= ((SIM_SCGC5_PORTB_MASK) | (SIM_SCGC5_PORTD_MASK));
    
    // Configure MUX, set to GPIO
    PORTB->PCR[RED_LED] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[RED_LED] |= PORT_PCR_MUX(1);
    
    PORTB->PCR[GREEN_LED] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[GREEN_LED] |= PORT_PCR_MUX(1);
    
    PORTD->PCR[BLUE_LED] &= ~PORT_PCR_MUX_MASK;
    PORTD->PCR[BLUE_LED] |= PORT_PCR_MUX(1);
    
    // Set pins as output
    PTB->PDDR |= (MASK(RED_LED) | MASK(GREEN_LED));
    PTD->PDDR |= MASK(BLUE_LED);
}

void led_control(enum color_t color) {
    switch (color) {
        case RED:
            PTB->PCOR = MASK(RED_LED);
            break;
        case GREEN:
            PTB->PCOR = MASK(GREEN_LED);
            break;
        case BLUE:
            PTD->PCOR = MASK(BLUE_LED);
            break;
    }
}

void offLED() {
    // LEDs are active low, so set output to 1 to turn off
    PTB->PSOR = MASK(RED_LED);
    PTB->PSOR = MASK(GREEN_LED);
    PTD->PSOR = MASK(BLUE_LED);
}

int main(void) {
    SystemCoreClockUpdate();
    InitGPIO();
    offLED();
    
    while (1) {
        led_control(RED);
        delay(0xFFFFF);
        offLED();
        
        led_control(GREEN);
        delay(0xFFFFF);
        offLED();
        
        led_control(BLUE);
        delay(0xFFFFF);
        offLED();
    }
}
