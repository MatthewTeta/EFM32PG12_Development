/**
 * @file cmu.c
 * @author Matthew Teta
 * @date 09/18/2022
 * @brief Custom CMU config
 */

//***********************************************************************************
// Include files
//***********************************************************************************
#include "cmu.h"
#include "em_assert.h"

//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// Private variables
//***********************************************************************************


//***********************************************************************************
// Private functions
//***********************************************************************************


//***********************************************************************************
// Global functions
//***********************************************************************************

/**
 * @brief
 *   Configure all of the clocks to the peripherals
 *
 *  @details
 *    Configure the clock tree for the LETIMER and I2C
 *
 */
void cmu_open(void){
    // High Frequency Oscillator (I2C)
    CMU_ClockEnable(cmuClock_HFPER, true);

    // Low Frequency Oscillator
    // By default, Low Frequency Resistor Capacitor Oscillator, LFRCO, is enabled,
    // Disable the LFRCO oscillator
    CMU_OscillatorEnable(cmuOsc_LFRCO , false, false);  // What is the enumeration required for LFRCO?

    // Disable the Low Frequency Crystal Oscillator, LFXO
    CMU_OscillatorEnable(cmuOsc_LFXO, false, false);  // What is the enumeration required for LFXO?

    // No requirement to enable the ULFRCO oscillator.  It is always enabled in EM0-4H

    // Route LF clock to LETIMER0 clock tree
    CMU_ClockSelectSet(cmuClock_LFA , cmuSelect_ULFRCO); // What clock tree does the LETIMER0 reside on?
//      CMU_ClockSelectSet(cmuClock_LETIMER0 , cmuSelect_ULFRCO); // What clock tree does the LETIMER0 reside on?

    // Now, you must ensure that the global Low Frequency is enabled
//    CMU_ClockEnable(cmuClock_LETIMER0, true); //This enumeration is found in the Lab 2 assignment
    CMU_ClockEnable(cmuClock_HFLE, true); //This enumeration is found in the Lab 2 assignment
//    CMU_ClockEnable(cmuClock_LFA, true); //This enumeration is found in the Lab 2 assignment

}

