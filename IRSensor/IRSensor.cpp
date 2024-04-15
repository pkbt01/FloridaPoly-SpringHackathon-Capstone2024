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
    _avgTop = 0;
    _avgSize = 3;
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
    _avgReadings = (int*) malloc(sizeof(int)*_avgSize);

    for(int i = _size-1; i >= 0; i--)
    {
      _scans_done[i] = 0;
      _scans_live[i] = 0;
    }
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

double IRSensor::GetRPM()
{
    return _avgRPM;
}


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

     _avgReadings[_avgTop] = 60.0 * _latestReading / _window;

    double sum = 0.0;

    for (int i = 0; i < _avgSize; i++)
    {
      sum += _avgReadings[i];
    }

    _avgRPM = sum / _avgSize;

    _avgTop++;
    if(_avgTop >= _avgSize)
      _avgTop = 0;
    

}

int IRSensor::GetReading()
{
   return _latestReading;
}

int IRSensor::GetRate()
{
  return _rate;
}

void IRSensor::Loop() {

    AddReading(digitalRead(_pin) == 0);
}