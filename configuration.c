/*
 * configuration.c
 * Scooter final project's configuration file for timer, gpio, uart, etc.
 * Authors: Ryan ZumBrunnen, Guanxiong Fu and Arash Yousefdezah
 * Created on: October 26, 2017
 *
 */

#include "msp.h"
#include "configuration.h"
#include "interrupt_handlers.h"
#include "conversion.h"
#include <grlib.h>
#include "Crystalfontz128x128_ST7735.h"

extern Graphics_Context g_sContext;

void display_configure(void)
{
    /* Initializes display */
    Crystalfontz128x128_Init();

    /* Set default screen orientation */
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    /* Initializes graphics context */
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, "Velocity:", AUTO_STRING_LENGTH, 64, 30, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, "Distance:", AUTO_STRING_LENGTH, 64, 60, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, "Stopped", AUTO_STRING_LENGTH, 64, 90, OPAQUE_TEXT);
}

// configuration for GPIO ports
void GPIO_configure(void)
{
    /* Left button configure */
    P1->SEL0 &= ~(BIT1);
    P1->SEL1 &= ~(BIT1);
    P1->DIR &= ~(BIT1);
    P1->OUT |= BIT1;
    P1->REN |= BIT1;
    P1->IFG &= ~(BIT1);
    P1->IES |= BIT1;
    P1->IE |= BIT1;

    /* Right button configure */
    P1->SEL0 &= ~(BIT4);
    P1->SEL1 &= ~(BIT4);
    P1->DIR &= ~(BIT4);
    P1->OUT |= BIT4;
    P1->REN |= BIT4;
    P1->IFG &= ~(BIT4);
    P1->IES |= BIT4;
    P1->IE |= BIT4;

    //Configure pin for input interrupt
    P1->DIR &= ~(BIT6);
    P1->SEL0 &= ~(BIT6);
    P1->SEL1 &= ~(BIT6);
    P1->IE |= BIT6;
    P1->REN |= BIT6;
    P1->IFG &= ~(BIT6);

    /* P1.0 LED Output */
    P1->DIR |= BIT0;
    P1->SEL0 &= ~(BIT0);
    P1->SEL1 &= ~(BIT0);
    P1->OUT &= ~(BIT0);

    /* RGB LED Output */
    P2->DIR |= BIT0 | BIT1 | BIT2;
    P2->SEL0 &= ~(BIT0 | BIT1 | BIT2);
    P2->SEL1 &= ~(BIT0 | BIT1 | BIT2);
    P2->OUT = BIT0; //Start with red on

    P6->SEL0 |= BIT0;
    P6->SEL1 |= BIT0;

    NVIC_EnableIRQ(PORT1_IRQn);
}

// configuration for Timer32
void timer_configure(void){
    //enable prescaling, interrupt flag, and set to be periodically repeating
    TIMER32_1->CONTROL |= TIMER32_CONTROL_PRESCALE_1 | TIMER32_CONTROL_IE | TIMER32_CONTROL_SIZE | TIMER32_CONTROL_MODE | TIMER32_CONTROL_ENABLE;
    TIMER32_1->LOAD = 93749; //time to get 0.5sec
    NVIC_EnableIRQ(T32_INT1_IRQn);
}


// configuration for ADC14
void ADC_configure(void){
    // Initialize the shared reference module
    // By default, REFMSTR=1 => REFCTL is used to configure the internal reference
    while(REF_A->CTL0 & REF_A_CTL0_GENBUSY); // If ref generator busy, WAIT
    REF_A->CTL0 = REF_A_CTL0_VSEL_0 | REF_A_CTL0_ON; // Enable internal 1.2V ref
    REF_A->CTL0 &= ~REF_A_CTL0_TCOFF; // Turn on Temperature Sensor

    ADC14->CTL0 |= ADC14_CTL0_SHT0_5 | ADC14_CTL0_ON | ADC14_CTL0_SHP | ADC14_CTL0_CONSEQ_1; //multiple sampling (repeating sequence of channel)
    ADC14->CTL1 |= ADC14_CTL1_TCMAP | BIT(16) | ADC14_CTL1_RES__14BIT ; // Conf internal temp sensor channel, ADC conversion start address from 1, set resolution to 14 bit
    ADC14->MCTL[0]= ADC14_MCTLN_INCH_22 | ADC14_MCTLN_VRSEL_0; // Map Temp Analog channel to MEM0/MCTL0, set 3.3v ref
    ADC14->MCTL[1]= ADC14_MCTLN_INCH_14 | ADC14_MCTLN_VRSEL_0; // Map Analog channel for Accelerometer x direction to MEM1/MCTL1, set 3.3v ref Accelerometer X Direction
    ADC14->MCTL[2]= ADC14_MCTLN_INCH_13 | ADC14_MCTLN_VRSEL_0; // Y Direction to MEM2/MCTL2
    ADC14->MCTL[3]= ADC14_MCTLN_INCH_11 | ADC14_MCTLN_VRSEL_0 | ADC14_MCTLN_EOS; //Z direction to MEM3/MCTL3
    ADC14->IER0 = ADC14_IER0_IE0 | ADC14_IER0_IE1 | ADC14_IER0_IE2 | ADC14_IER0_IE3; // Enable MCTL1/MEM0 , MCTL1/MEM1 , MCTL2/MEM2 , MCTL3|MEM3 Interrupts

    while(!(REF_A->CTL0 & REF_A_CTL0_GENRDY)); // Wait for ref generator to settle
    ADC14->CTL0 |= ADC14_CTL0_ENC; // Enable Conversions
    NVIC_EnableIRQ(ADC14_IRQn); // Enable ADC int in NVIC module
}
