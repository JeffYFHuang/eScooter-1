/*
 * send_terminal.c
 * Scooter final project's implementation file for functions for sending things terminal
 * Authors: Ryan ZumBrunnen ,Guanxiong Fu and Arash Yousefdezah
 * Created on: October 26, 2017
 *
 */
#include "msp.h"
#include "configuration.h"
#include "interrupt_handlers.h"
#include "conversion.h"
#include "send_terminal.h"
#include <grlib.h>
#include "Crystalfontz128x128_ST7735.h"

extern Graphics_Context g_sContext;

extern volatile uint32_t count_pin;
extern volatile float distance;
extern volatile float velocity;
extern volatile float velocity_prev;

extern volatile float Nadcz;

void print_distance(void){
    distance = count_pin * 0.022; // use count_pin to find the distance of scooter
    char string[6]={};
    ftoa(distance, string, 2);
    Graphics_drawStringCentered(&g_sContext, (int8_t *)string, 8, 64, 70, OPAQUE_TEXT);
}

void print_velocity(void){
    char string[6]={};
    ftoa(velocity, string, 2);
    Graphics_drawStringCentered(&g_sContext, (int8_t *)string, 8, 64, 70, OPAQUE_TEXT);
}

void print_direction(void){
    float zaccel = (0.00020142*Nadcz)/0.66-2.5; //voltage divided by mV per g and subtracting 2.5 to get equilibrium
    if (zaccel > 0.25)      //forward
        P2->OUT = BIT1;     //Green
    else if (zaccel < 0.25) //backward
        P2->OUT = BIT2;     //Blue
}


