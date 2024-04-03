
#ifndef IRSensor_h
#define IRSensor_h

#include "Arduino.h"

class IRSensor{
    public:
    IRSensor(int pin, int rate, double window);
    void Begin();
    void Stop();
    int GetReading();
    double GetRPM();
    void Loop();

    private:
    void AddReading(bool reading);
    void UpdateLatest();
   
    int _latestReading;
    int _pin;
    int _rate;
    double _window;
    bool *_scans_done, *_scans_live;
    double _deltaTime;
    int _size;
    int _top;
    int *_avgReadings;
    int _avgTop;
    int _avgSize;
    double _avgRPM;
};

#endif