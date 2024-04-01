#include "Arduino.h"
#include "IRSensor.h"


IRSensor::IRSensor(int pin, int rate, double window)
{
    _pin = pin;
    _rate = rate;
    _window = window;
    _deltaTime = 1.0 / _rate;
    _size = _window * _rate;
    _top = 0;
    _latestReading = 0;
}

void IRSensor::Begin() {
  // put your setup code here, to run once:
//Serial.begin(9600);
    pinMode(_pin,INPUT);

    int memUsage = 2 * sizeof(bool)*_size;
  //  Serial.print("Memory used for scans: ");
   // Serial.print(memUsage);
    //Serial.println();

    _scans_done = (bool*) malloc(sizeof(bool)*_size);
    _scans_live = (bool*) malloc(sizeof(bool)*_size);



    for(int i = _size-1; i >= 0; i--)
    {
      _scans_done[i] = 0;
      _scans_live[i] = 0;
    }

   // Serial.print("Effective deltatime (ms): ");
     //   Serial.print(deltaTime * 1000);
       // Serial.println();
}

void IRSensor::AddReading(bool reading)
{
    _scans_live[_top] = reading;
    _top++;

    if(_top >= _size)
    {
        for (int i = 0; i < _size; i++)
        {
            _scans_done[i] = _scans_live[i];
        }
        
        UpdateLatest();

        _top = 0;
    }
}

/*int SumReadings()
{
    int count = 0;
    for(int i = 0; i < _size; i++)
    {
        if(_scans_done[i] == true)
            count++;
    }

    return count;
}*/

void IRSensor::UpdateLatest()
{
    int count = 0;
    bool high = _scans_done[0];
    for(int i = 1; i < _size; i++)
    {
        if(_scans_done[i] == true){
            if(!high)
              count++;
            
            high=true;
        }
        else
        {
          high = false;
        }

    }

    _latestReading = count;
}

int IRSensor::GetReading()
{
   return _latestReading;
}

void IRSensor::Loop() {

    AddReading(digitalRead(_pin) == 0);

    delay(_deltaTime*1000.0);
}