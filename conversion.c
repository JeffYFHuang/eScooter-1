/*
 * conversion.c
 * Scooter final project's implementation file for conversion functions
 * Authors: Ryan ZumBrunnen, Guanxiong Fu and Arash Yousefdezah
 * Created on: October 26, 2017
 *
 */

#include "msp.h"
#include "configuration.h"
#include "interrupt_handlers.h"
#include "conversion.h"
#include "send_terminal.h"

void reverse(char s[], int slength)
{
     int i, j;
     char c;

     for (i = 0, j = slength-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }

 //itoa taken from https://en.wikibooks.org/wiki/C_Programming/stdlib.h/itoa and cahnged slightly
int itoa(uint32_t n, char s[])
{
    int i, sign;
    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s, i); //MIGHT BE MESSING WITH NULL CHARACTER
    return i;
}

//ftoa taken from http://www.geeksforgeeks.org/convert-floating-point-number-string/
//and modified to fit our needs better
void ftoa(float n, char s[], int afterpoint){
    // Extract integer part
    int ipart = (int)n;

    // Extract floating part
    float fpart = n - (float)ipart;

    // convert integer part to string
    int position = itoa(ipart, s);

    // check for display option after point
    if (afterpoint != 0)
    {
        s[position] = '.';
        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter is needed
        // to handle cases like 233.007
        int tempnum = 1;
        int i=0;
        for (i = 0; i < afterpoint; i++)
            tempnum *= 10; //to get 10^afterpoint
        fpart = fpart * tempnum; //should make this a number without dec
        if (fpart<0)
            fpart=abs(fpart);
        tempnum/=10;
        while (fpart<tempnum){ //adds back zeros between decimal and trailing digits
            itoa(0,s+position+1);
            tempnum /= 10;
            position++;
        }
        itoa((uint32_t)fpart, s + position + 1);

    }
}

