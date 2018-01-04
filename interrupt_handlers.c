
/*
 * interrupt_handlers.c
 * Scooter final project's implementation file for interrupt handlers
 * Authors: Ryan ZumBrunnen, Guanxiong Fu and Arash Yousefdezah
 * Created on: October 26, 2017
 *
 */

#include "msp.h"
#include "configuration.h"
#include "interrupt_handlers.h"
#include "conversion.h"
#include "send_terminal.h"

extern volatile uint8_t flag_dist;
extern volatile uint8_t flag_vel;

extern volatile uint32_t count_pin;
extern volatile float distance;

extern volatile float distance_prev;
extern volatile float velocity;
extern volatile float velocity_prev;

extern volatile float Nadc;
extern volatile float Nadcx;
extern volatile float Nadcy;
extern volatile float Nadcz;

void PORT1_IRQHandler(void){
    if (P1->IFG & BIT1){ // Left button press
        flag_dist = 1;
        P1->IFG &= ~BIT1; // clear interrupt flag for Left button
        // NVIC_DisableIRQ(T32_INT1_IRQn);
    }
    if (P1->IFG & BIT4){ // Right button press
        ADC14->CTL0 |= ADC14_CTL0_SC; // start sampling and conversion
        P1->IFG &= ~BIT4;
    }
    if (P1->IFG & BIT6) { // input interrupt
        count_pin++; // count the number of time the interrupt is triggered
        P1->IFG &= ~(BIT6); // clear interrupt flag
    }
}


void T32_INT1_IRQHandler(void){
    distance = count_pin * 0.022;
    velocity_prev = velocity;
    velocity = distance - distance_prev;
    distance_prev = distance;

    flag_vel = 1; // goes to main because function calls inside ISRs is bad practice

    ADC14->CTL0 |= ADC14_CTL0_SC; // start sampling and conversion

    TIMER32_1->INTCLR = 0;
}


void ADC14_IRQHandler(void){
    if (ADC14->IFGR0 & ADC14_IFGR0_IFG0){
        Nadc = ADC14->MEM[0]; //Nadc value for temperature
        // don't need to clear flag because it automatically clears when data is read
    }
    if (ADC14->IFGR0 & (ADC14_IFGR0_IFG1 | ADC14_IFGR0_IFG2 | ADC14_IFGR0_IFG3)){
        Nadcx = ADC14->MEM[1]; // Nadc value for accelerometer in x direction
        Nadcy = ADC14->MEM[2]; // y direction
        Nadcz = ADC14->MEM[3]; // z direction
    }
}

