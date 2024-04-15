#include <IRSensor.h>

// Constants
const int SENSOR_PIN = A0;  // Input pin for measuring Vout
const int RS = 10;          // Shunt resistor value (in ohms)
const int VOLTAGE_REF = 5;  // Reference voltage for analog read

// Global Variables
float sensorValue;   // Variable to store value from analog read
float current;       // Calculated current value
float holdingCurrent; // Sum of current since last reading
int counter = 0;

IRSensor* ir;

void setup() {

  // Initialize serial monitor
  Serial.begin(9600);
  ir = new IRSensor(10, 500, 1.0);
  ir->Begin();

  holdingCurrent = 0;
}

void loop() {

  ir->Loop();
  // Read a value from the INA169 board
  sensorValue = analogRead(SENSOR_PIN);

  // Remap the ADC value into a voltage number (5V reference)
  sensorValue = (sensorValue * VOLTAGE_REF) / 1023;

  // Follow the equation given by the INA169 datasheet to
  // determine the current flowing through RS. Assume RL = 10k
  // Is = (Vout x 1k) / (RS x RL)
  current = sensorValue / (1000 * RS);
  holdingCurrent += current;

  // Output value (in amps) to the serial monitor to 3 decimal
  // places
  if(counter > ir->GetRate()){
  Serial.print(holdingCurrent, 3);
  Serial.print(",");
  Serial.print(ir->GetRPM()/1);
  Serial.println("");
  counter = 0;
  holdingCurrent = 0;
  }

  counter++;
  // Delay program for a few milliseconds
  delay(1000.0 / ir->GetRate());

}