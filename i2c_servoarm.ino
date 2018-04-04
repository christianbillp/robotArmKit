#include <Wire.h>
#include <Servo.h> 

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

int servo1Setpoint;
int servo2Setpoint;
int servo3Setpoint;
int servo4Setpoint;

int buffer[8]={};

void setup() 
{ 
    servo1.attach(5);
    servo2.attach(6);
    servo3.attach(10);
    servo4.attach(11);
    Serial.begin(115200);
    Wire.begin(9);
    Wire.onReceive(receiveEvent);
    Serial.println("System ready");
    setAngles(90,90,90,90);
} 

void analogTest(int potpin)  {
    int val = analogRead(potpin);
    Serial.println(val); 
    val = map(val, 0, 1023, 0, 179);
    servo1.write(val);
    delay(15);
}

void setAngles(int angle1,int angle2,int angle3,int angle4)  {
    servo1.write(angle1);
    servo2.write(angle2);
    servo3.write(angle3);
    servo4.write(angle4);
}


void loop(){
}

void servos(int byteCount) {
  //get commands from i2c
    int cmd[4];
    int x = 0;
    while(Wire.available()) {
        cmd[x] = Wire.read();
        x++;
    }
        Serial.println(cmd[0]);
        Serial.println(cmd[1]);
        Serial.println(cmd[2]);
        Serial.println(cmd[3]);
             
}


void receiveEvent(int howMany) {
  //get commands from i2c
    int commandBuffer[4];
    int x = 0;
    while(Wire.available()) {
        commandBuffer[x] = Wire.read();
        x++;
    }
    /*
        Serial.println(commandBuffer[0]);
        Serial.println(commandBuffer[1]);
        Serial.println(commandBuffer[2]);
        Serial.println(commandBuffer[3]);
        */
        setAngles(commandBuffer[0],commandBuffer[1],commandBuffer[2],commandBuffer[3]);
}

void requestEvent() {
}

