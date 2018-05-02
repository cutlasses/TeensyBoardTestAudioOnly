#include <Audio.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <ADC.h>

#include "core_pins.h"

// wrap in a struct to ensure initialisation order
struct IO
{
  ADC                         adc;
  AudioInputAnalog            audio_input;
  AudioOutputAnalog           audio_output;

  IO() :
    adc(),
    audio_input(A0),
    audio_output()
  {
  }
};

IO io;

AudioConnection       patchCord1(io.audio_input, 0, io.audio_output, 0);

void set_adc1_to_3v3()
{

  ADC1_SC3 = 0; // cancel calibration
  ADC1_SC2 = ADC_SC2_REFSEL(0); // vcc/ext ref 3.3v

  ADC1_SC3 = ADC_SC3_CAL;  // begin calibration

  uint16_t sum;

  //serial_print("wait_for_cal\n");

  while( (ADC1_SC3 & ADC_SC3_CAL))
  {
    // wait
  }

  __disable_irq();

    sum = ADC1_CLPS + ADC1_CLP4 + ADC1_CLP3 + ADC1_CLP2 + ADC1_CLP1 + ADC1_CLP0;
    sum = (sum / 2) | 0x8000;
    ADC1_PG = sum;
    sum = ADC1_CLMS + ADC1_CLM4 + ADC1_CLM3 + ADC1_CLM2 + ADC1_CLM1 + ADC1_CLM0;
    sum = (sum / 2) | 0x8000;
    ADC1_MG = sum;

  __enable_irq();
  
}

void setup()
{
  Serial.println("Setup BEGIN");
  
  AudioMemory(12);
  
  Serial.begin(9600);

  // THIS WILL CAUSE THE AUDIO LIBRARY TO STOP EMITTING SOUND
  // Audio library has overriden this, so need to reset the reference voltages
  //io.adc.setReference( ADC_REFERENCE::REF_1V2, ADC_1 ); // NOTE: ADC CODE CHECKS FOR SETTING SAME VALUE, SO SET IT TO SOMETHING ELSE FIRST
  //io.adc.setReference( ADC_REFERENCE::REF_3V3, ADC_1 );

  set_adc1_to_3v3();
  
  delay(1000);

  Serial.println("Setup END");
}

void loop()
{

}

