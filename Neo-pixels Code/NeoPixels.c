
/*==============================================================================
 Project: NeoPixel-Starter
 Date:    June 8, 2022
 
 This program demonstrates basic NeoPixel data transmission & colour adjustment.
==============================================================================*/

#include    "xc.h"              // Microchip XC8 compiler include file
#include    "stdint.h"          // Include integer definitions
#include    "stdbool.h"         // Include Boolean (true/false) definitions

#include    "UBMP410.h"           // Include UBMP4 constants and functions

// TODO Set linker ROM ranges to 'default,-0-7FF' under "Memory model" pull-down.
// TODO Set linker code offset to '800' under "Additional options" pull-down.

// Program constant definitions
#define LEDS 96

// Program variable definitions
unsigned char red = 10;         // Starting colour or colour-adjusted output values, the maximum is 255.
unsigned char green = 10;
unsigned char blue = 10;
unsigned char redP[LEDS];
unsigned char greenP[LEDS];
unsigned char blueP[LEDS];

//Variables for waves
bool countUp = true;
bool patternUp = true;
unsigned char colourIndex = 0;
unsigned char colourIndexCopy;

unsigned char pix;

// Shift 8-bits of NeoPixel data
void neopixel_shift(unsigned char col)
{
    for(unsigned char bits = 8; bits != 0; bits --)
    {
        H8OUT = 1;
        if((col & 0b10000000) == 0)
        {
            H8OUT = 0;
        }
        NOP();
        H8OUT = 0;
        //NOP();
        col = col << 1;
    }
}

// Fill NeoPixel strip with raw colour from the red, green, and blue variables
void neopixel_fill(unsigned char leds)
{
    for(leds; leds != 0; leds--)
    {
        neopixel_shift(green);
        neopixel_shift(red);
        neopixel_shift(blue);

    }
}

void neopixel_array_fill(unsigned char leds)
{
    for(pix = 0; pix != leds; pix++)
    {
        neopixel_shift(greenP[pix]);
        neopixel_shift(redP[pix]);
        neopixel_shift(blueP[pix]);
    }
}

int main(void)
{
    OSC_config();               // Configure internal oscillator for 48 MHz
    UBMP4_config();             // Configure on-board UBMP4 I/O devices
    
    //green = 0b10101010;         // Timing test 
    
    //Makes a gradient thingy where red is on one side, blue is on the other and in the middle there is purple
    // for(unsigned char pix = 0; pix != LEDS; pix++)
    // {
    //     redP[pix] = pix * 2;
    //     greenP[pix] = 0;
    //     blueP[pix] = 192 - pix * 2;
    // }

    while(1)
    {
        neopixel_array_fill(LEDS);
        neopixel_fill(LEDS);
        __delay_ms(20);

        // //Showing some colours when the button is pressed. 
        // if (SW2 == 0) //Cyan
        // {
        //     green = 150;
        //     red = 0;
        //     blue = 150; 
        // }
        // if (SW3 == 0) // Red
        // {
        //     green = 0;
        //     red = 225;
        //     blue = 0;
        // }
        // if (SW4 == 0) // Yellow
        // {
        //     green = 255;
        //     red = 255;
        //     blue = 0;
        // }
        // if (SW5 == 0) //Grey
        // {
        //     green = 50;
        //     red = 50;
        //     blue = 50;
        // }

        colourIndexCopy = colourIndex;
        //Making a moving pattern of purple if SW3 is pressed 
        // if (SW3 == 0)
        // {
            //This makes the pattern light brighter
            for (pix = 0; pix != LEDS; pix++)
            {
                //redP[pix] = colourIndex;
                //blueP[pix] = colourIndex;
                greenP[pix] = colourIndex;
                if (countUp)
                {
                    colourIndex++;
                    if (colourIndex == 255)
                    {
                        countUp = false;
                    }
                }
                else 
                {
                    colourIndex--;
                    if (colourIndex == 0)
                    {
                        colourIndex = true;
                    }
                }
            //} //This one makes the code move up and down in one certain pattern

            //This makes the pattern dim
            if (patternUp)
            {
                colourIndex = colourIndexCopy + 1;
                if (colourIndex == 254)
                {
                    patternUp = false;
                }
            }
            else
            {
                colourIndex = colourIndexCopy - 1;
                if (colourIndex == 1)
                {
                    patternUp = true;
                }
            }
        } //This closing bracket makes the lights glow and dim

        //If SW2 is pressed, then light in purple
        // if (SW2 == 0)
        // {
        //     for(unsigned char pixel = 0; pixel != LEDS; pixel++)
        //     {
        //     redP[pixel] = pixel;
        //     greenP[pixel] = 0;
        //     blueP[pixel] = pixel;
        //     }
        // }

        // //Makes some sort of pattern (I randomly did this and it made a pattern that blinks red while SW2 is pressed)
        // for (unsigned int i = 0; i < 225; i++)
        // {
        //     green = 0;
        //     red += i;
        //     blue = 100;
        // }


        // Activate bootloader if SW1 is pressed.
        if(SW1 == 0)
        {
            RESET();
        }        
    }
}

/* Learn More - Program Analysis Activities
 * 
 * 1. Modify the UBMP4_config() function to allow output on the NeoPixel pin.
 *    Use the test pattern and an oscilloscope to measure the width of the 0 and
 *    1 data bits transmitted for each colour byte, as well as the total bit
 *    period duration.
 * 
 * 2. What do NOP(); operations do? There is also a _delay() function that will
 *    create a delay for the specified number of clock cycles. Replace the two
 *    NOP(); statements with: _delay(2);
 * NOP operations does nothing while the code is being executed. 
 *    Is the timing the same?
 * I'm not sure but it seems the same. 
 * 3. Adjust the amount of delay, and the location of the delay code in the
 *    neopixel_shift() function to adjust the bit width and bit period to most
 *    closely match the values in the NeoPixel data sheet.
 * 
 * 4. Determine the number of pulses required to create a start bit, a zero bit,
 *    and a one bit for the protocol that your remote control uses.
 * 
 * 5. Complete the transmit_Sony() function by adding the code to transmit the
 *    5-bit device address code after the command code.
 *
 * 6. Create the definitions and the functions required to transmit the protocol
 *    used by your IR device.
 */
