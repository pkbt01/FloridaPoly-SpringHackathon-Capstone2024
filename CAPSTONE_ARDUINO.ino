#define PIN 10
#define RATE 1000
#define WINDOW 0.5

bool *scans_done, *scans_live;
double deltaTime = 1.0/RATE;
int size = WINDOW * RATE;
int counter = 0;
int top = 0;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(PIN,INPUT);

    int memUsage = 2 * sizeof(bool)*size;
    Serial.print("Memory used for scans: ");
    Serial.print(memUsage);
    Serial.println();

  scans_done = (bool*) malloc(sizeof(bool)*size);
  scans_live = (bool*) malloc(sizeof(bool)*size);



    for(int i = size-1; i >= 0; i--)
    {
      scans_done[i] = 0;
      scans_live[i] = 0;
    }

    Serial.print("Effective deltatime (ms): ");
        Serial.print(deltaTime * 1000);
        Serial.println();
}

void AddReading(bool reading)
{
    scans_live[top] = reading;
    top++;

    if(top >= size)
    {
        for (int i = 0; i < size; i++)
        {
            scans_done[i] = scans_live[i];
        }
        
        top = 0;
    }
}

int SumReadings()
{
    int count = 0;
    for(int i = 0; i < size; i++)
    {
        if(scans_done[i] == true)
            count++;
    }

    return count;
}

int SumChanges()
{
    int count = 0;
    bool high = scans_done[0];
    for(int i = 1; i < size; i++)
    {
        if(scans_done[i] == true){
            if(!high)
              count++;
            
            high=true;
        }
        else
        {
          high = false;
        }

    }

    return count;
}

void loop() {
  // put your main code here, to run repeatedly:

    AddReading(digitalRead(PIN) == 0);
    counter++;
    if(counter >= size*4)
    {
        int num = SumChanges();
        Serial.print("RPM: ");
        Serial.print(60 * num/WINDOW);
        Serial.println();

        counter=0;
    }

    delay(deltaTime*1000.0);
 

}
