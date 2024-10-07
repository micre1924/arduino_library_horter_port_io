/*

TEST:

- I2HE (INPUTS) -> I2HA (OUTPUTS)

- I2HAE (4 CHANNELS) (ANALOG INPUTS) -> I2HAA (4 CHANNELS) (ANALOG OUTPUTS)

*/

#include "Arduino.h"
#include "Wire.h"
#include "horter.h"

#define INPUT_ADDRESS 0x38
#define OUTPUT_ADDRESS 0x20
#define ADC_ADDRESS 0x08
#define DAC_ADDRESS 0x58

uint8_t wireJobListPosition;

String  adcValuesBuffer, dacValuesBuffer;

horter::I2HE    testInputs(INPUT_ADDRESS);
horter::I2HA    testOutputs(OUTPUT_ADDRESS);
horter::I2HAE   testAnalogInputs(ADC_ADDRESS);
horter::I2HAA   testAnalogOutputs(DAC_ADDRESS);

void wireJobList();

void every1s(){
  Serial.println(adcValuesBuffer);
}

void setup() {
  delay(500);
  Serial.begin(9600);  // start serial for output
  Wire.begin();        // join i2c bus (address optional for master)
}

void loop() {
  wireJobList();
  Delay(5);
}

void wireJobList(){
  if(wireJobListFreq.tick()){
    wireJobListPosition++;
    switch (wireJobListPosition)
    {
    case 1:
      testAnalogInputs.read5();
      break;
    case 2:
      testAnalogOutputs.write4(testAnalogInputs.dataIn);
      break;
    case 3:
      testInputs.read8();
      break;
    case 4:
      testOutputs.write8(testInputs.dataIn);
      break;
    default:
      wireJobListPosition = 0;
      break;
    }
  }
}