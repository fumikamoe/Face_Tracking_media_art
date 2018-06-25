#include "HMLesp8266.h"
#include <Servo.h>

//For WIFI SETTING
#define SSID        ""
#define PASSWORD    ""

SoftwareSerial mySerial(2, 3); /* RX:D2, TX:D3 */
HMLesp8266 wifi(mySerial);

//For UltraSensor
const int TriggerPin = 8; //Trig pin
const int EchoPin = 9; //Echo pin
long Duration = 0;

//For VISION
#define VISION   "10.10.1.101"
#define HOST_PORT   (8080)

//Set Servo Motor Parameter
Servo LeftEyeWidth;
Servo LeftEyeHeight;
Servo RightEyeWidth;
Servo RightEyeHeight;

int pinLeftEyeWidth = 10;
int pinLeftEyeHeight = 11;
int pinRightEyeWidth = 12;
int pinRightEyeHeight = 13;

//INIT variable
String input;
int V;
int H;
int Prev_V = 0;
int Prev_H = 0;

String captured = "false";

String temp;
int count = 0;
int V_Margin = 50;
int H_Margin = (int)(V_Margin * 4/3);
long Distance_mm = 0;
bool moved = false;
int MIN_Angle = 60;
int MAX_Angle = 150;

void setup() {
  //Serial.begin(9600);
  
  //set Servo
  LeftEyeWidth.attach(pinLeftEyeWidth);
  LeftEyeHeight.attach(pinLeftEyeHeight);
  RightEyeWidth.attach(pinRightEyeWidth);
  RightEyeHeight.attach(pinRightEyeHeight);
  
  //set Ult Sensor
  pinMode(TriggerPin, OUTPUT); // Trigger is an output pin
  pinMode(EchoPin, INPUT); // Echo is an input pin
  
  //Wifi Setup
  wifi.connectWifi(SSID, PASSWORD, 3);
  wifi.setTCPClient(1,NULL,VISION,HOST_PORT,NULL);
  //wifi.setMux(1);
  //wifi.setConnection(VISION, HOST_PORT);
  */
}

void loop()
{
    /*
    //wifi get
    wifi.sendData(1);
    input = wifi.receiveData();    
    H, V= Parsing(input);
    */
    // Ultrasound sensor Part
    moved = CaptureMovement();
    
    //Servo Part
    if(captured == "True")
    {
        if(V <= Prev_V -5 || V >= Prev_V + 5) // 비전 인식에서의 모터 진동을 감소
        {
        LeftEyeHeight.write(V);
        RightEyeHeight.write(V);
        Prev_V = V;
        } 
        if(V <= Prev_H -5 || V >= Prev_H + 5)
        {
        LeftEyeWidth.write(H);
        RightEyeWidth.write(H);
        Prev_H = H;
        }
        Serial.println("Controlled");
    }
    else //if not capture face
    {
      
        H = random(MIN_Angle, MAX_Angle);
        V = random(MIN_Angle, MAX_Angle);
        LeftEyeWidth.write(H);
        RightEyeWidth.write(H);
        LeftEyeHeight.write(V);
        RightEyeHeight.write(V);
        Serial.println("Random ACTION");
        if(moved == true)
        {
          delay(200);
        }
          delay(1000);
    }
}


//For Parsing Data from wifi
void Parsing(String input)
{
    for (int i = 0; i < input.length(); i++)
  {
    if (input.substring(i, i+1) == ",")
    {
      H = input.substring(0, i).toInt();
      temp = input.substring(i+1);
      break;
    }
  }
    for (int i = 0; i < temp.length(); i++)
  {
    if (temp.substring(i, i+1) == ",")
    {
      V = temp.substring(0, i).toInt();
      captured = temp.substring(i+1);
      break;
    }
  }
  H = map(H, 0 + H_Margin, 640 - H_Margin, MIN_Angle, MAX_Angle); // X Coordnate to Angle
  H = 180 - H;
  
  V = map(V, 0 + V_Margin, 480 - V_Margin, MIN_Angle, MAX_Angle);// Y Coordnate to Angle
  V = 180 - V;
  
  Serial.print("Horizontal Degree : ");
  Serial.println(H);
  
  Serial.print("Vertical Degree : ");
  Serial.println(V);
  
  Serial.print("Captured : ");
  Serial.println(captured);
  
}
// Parsing Part end


bool CaptureMovement(){
    bool moved = false;
    
    digitalWrite(TriggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(TriggerPin, HIGH);
    
    delayMicroseconds(10);
    digitalWrite(TriggerPin, LOW);
  
    Duration = pulseIn(EchoPin, HIGH);
    
    if(Distance(Duration) <= Distance_mm - 20 || Distance(Duration) >= Distance_mm + 20)
    {
      moved = true;
    }
    Distance_mm = Distance(Duration);
  
    Serial.print("Distance = ");
    Serial.print(Distance_mm);
    return moved;
}

long Distance(long time)
{
  long DistanceCalc;
  DistanceCalc = ((time / 2.9) / 2);
  return DistanceCalc;
}
