/*
 * main.c
 * Scooter final project's main source file
 * Authors: Ryan ZumBrunnen, Guanxiong Fu and Arash Yousefdezah
 * Created on: October 26, 2017
 *
 */

#include "msp.h"
#include "configuration.h"
#include "interrupt_handlers.h"
#include "conversion.h"
#include "send_terminal.h"
#include <stdint.h>
#include <grlib.h>
#include "Crystalfontz128x128_ST7735.h"

/* Graphic library context */
Graphics_Context g_sContext;

volatile uint8_t flag_dist =1;
volatile uint8_t flag_vel;
volatile uint8_t startflag;
volatile uint8_t delay = 0;

volatile uint32_t count_pin = 0;
volatile float distance;
volatile float velocity;
volatile float velocitydiff = 0;

volatile float distance_prev = 0;
volatile float velocity_prev = 0;

volatile float Nadc;
volatile float Nadcx;
volatile float Nadcy;
volatile float Nadcz;

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    ADC_configure();
    GPIO_configure();
    timer_configure();
    display_configure();

    while(1){
        if (flag_dist == 1){ // button press occurred
            print_distance();
            flag_dist = 0;
        }
        if (flag_vel == 1){
            if (delay==1) //delay since 0.5 sec printing is too fast
                delay=0;
            if (delay==0){
                print_velocity();
                delay=1;
            }
            flag_vel = 0;
        }
        velocitydiff = velocity - velocity_prev;
        if (velocity < 1 && startflag==0 && velocitydiff > 0.1){ //less than .5 meter/sec == 1 mph and some positive accel
            Graphics_drawStringCentered(&g_sContext, "Moving", AUTO_STRING_LENGTH, 64, 90, OPAQUE_TEXT);
            print_direction(); //changes the P2.0 LED to either green or blue based on direction
            startflag=1;
        }
        if (velocity < 0.1 && startflag==1 && velocitydiff < 0){ //small velocity and some buffer for negative accel
            Graphics_drawStringCentered(&g_sContext, "Stopped", AUTO_STRING_LENGTH, 64, 90, OPAQUE_TEXT);
            P2->OUT = BIT0; //red LED for stopped
            startflag=0;
        }
    }
}


