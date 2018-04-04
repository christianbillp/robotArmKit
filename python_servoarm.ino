#include <Servo.h> 

//Constants (robot dimensions in mm)
int baseHeight = 50;
int humerus = 50;
int ulna = 50;
int hand = 80;

#define BASENEUTRAL     1500
#define SHOULDERNEUTRAL 0
#define ELBOWEUTRAL     1944
#define WRISTNEUTRAL    1000
#define GRIPPERNEUTRAL  0

int baseValue,
    shoulderValue,
    elbowValue,
    wristValue,
    gripperValue;

//PINOUT
int servoPin[] = {3,5,6,9,10};

char serialBuffer[50];
char workBuffer[5];
int bufferIndex = 25;

//Servo object declaration using <Servo.h>
Servo myservo;  //A-1 (Base)      // 500 - 2500
Servo myservo2; //A-2 (Shoulder)  //
Servo myservo3; //A-3 (Elbow)     //
Servo myservo4; //A-4 (Wrist)     //
Servo myservo5; //A-5 (Gripper)   //

void setup(){
    Serial.begin(57600);
    myservo.attach(servoPin[0]);  //A-1 (Base)    
    myservo2.attach(servoPin[1]); //A-2 (Shoulder)
    myservo3.attach(servoPin[2]); //A-3 (Elbow)
    myservo4.attach(servoPin[3]); //A-4 (Wrist)
    myservo5.attach(servoPin[4]); //A-5 (Gripper)
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);  
    strcpy(serialBuffer, "a1500b1500c1500d1500e1500");
}

void loop() {
    
    while (Serial.available() > 0) {                // Captures commandstring into serialbuffer
      serialBuffer[bufferIndex++] = Serial.read();  // a[4]b[4]c[4]d[4]e[4]
                                                    // a0000b0010c0010d0010e0010 - a1500b1500c1500d1500e1500
    }
    
    //Serial.print("BufferIndex: ");
    //Serial.println(bufferIndex);
    /*
    Serial.print("Command String: ");
    for (int i = 0; i < bufferIndex; i++) {
      Serial.print(serialBuffer[i]);
    }
    Serial.println();
    */
    if (bufferIndex == 25) {
      workBuffer[0] = serialBuffer[1];
      workBuffer[1] = serialBuffer[2];
      workBuffer[2] = serialBuffer[3];
      workBuffer[3] = serialBuffer[4];
      baseValue = atoi(workBuffer);
      
      workBuffer[0] = serialBuffer[6];
      workBuffer[1] = serialBuffer[7];
      workBuffer[2] = serialBuffer[8];
      workBuffer[3] = serialBuffer[9];
      shoulderValue = atoi(workBuffer);
      
      workBuffer[0] = serialBuffer[11];
      workBuffer[1] = serialBuffer[12];
      workBuffer[2] = serialBuffer[13];
      workBuffer[3] = serialBuffer[14];
      elbowValue = atoi(workBuffer);
  
      workBuffer[0] = serialBuffer[16];
      workBuffer[1] = serialBuffer[17];
      workBuffer[2] = serialBuffer[18];
      workBuffer[3] = serialBuffer[19];
      wristValue = atoi(workBuffer);
  
      workBuffer[0] = serialBuffer[21];
      workBuffer[1] = serialBuffer[22];
      workBuffer[2] = serialBuffer[23];
      workBuffer[3] = serialBuffer[24];
      gripperValue  = atoi(workBuffer);
      bufferIndex = 0;
      
    }
    else {
      bufferIndex = 0;
      digitalWrite(13, HIGH);    
      //Serial.println("Hmmm");
    }
    shoulderValue = constrain(shoulderValue, 650, 2500);
    
    myservo.writeMicroseconds(baseValue);       //Base. Neutral position = 90
    myservo2.writeMicroseconds(shoulderValue);  //Shoulder. Vertical = 90.
    myservo3.writeMicroseconds(elbowValue);     //Elbow. Vertical up = 175.
    myservo4.writeMicroseconds(wristValue);     //Wrist. Vertical = 90.
    myservo5.writeMicroseconds(gripperValue);   //Less is more closed
    //serialDebug();
    delay(50);
}

void serialDebug() {
    Serial.print(baseValue); //LEFT RIGHT(1024)
    Serial.print("\t");
    Serial.print(shoulderValue); //DOWN UP(1024)
    Serial.print("\t");
    Serial.print(elbowValue);//LEFT RIGHT(1024)
    Serial.print("\t");
    Serial.print(wristValue);
    Serial.print("\t");
    Serial.print(gripperValue);
    Serial.print("\n");
    delay(30);
}

/*
 * Functions, variables and commands for on-board math!
 *     // h[3]r[3]d[3]a[3] - h100r004d060a015 - h150r004d060a015
 * 
 * 
 * float shoulder;
float elbow;
float wristAngle;
int setHeight;
int setDepth;
int setAngle;

int rotationStartDepth; //Initial depth value before compensating for rotation
int setRotation;  //
int rotationDepth;  //Depth value after rotation
 * 
 * workBuffer[0] = serialBuffer[1];
    workBuffer[1] = serialBuffer[2];
    workBuffer[2] = serialBuffer[3];
    setHeight = atoi(workBuffer);
    
    workBuffer[0] = serialBuffer[5];
    workBuffer[1] = serialBuffer[6];
    workBuffer[2] = serialBuffer[7];
    setRotation = atoi(workBuffer);
    
    workBuffer[0] = serialBuffer[9];
    workBuffer[1] = serialBuffer[10];
    workBuffer[2] = serialBuffer[11];
    rotationStartDepth = atoi(workBuffer);

    workBuffer[0] = serialBuffer[13];
    workBuffer[1] = serialBuffer[14];
    workBuffer[2] = serialBuffer[15];
    setAngle = atoi(workBuffer);
    bufferIndex = 0;
    //serialDebug2();
    
    rotationDepth = constrain(rotationStartDepth/cos(setRotation*0.0174532925),10,200);
    findAngles(setHeight,rotationDepth,setAngle); //End effector angle locked to 0!
    shoulder = constrain(shoulder,10,170);
    if (shoulder != 0) {
        myservo.write(90+setRotation);  //Base.Neutral position = 100 100+setRotation
        myservo2.write(shoulder);     //Shoulder. Vertical = 90.
        myservo3.write(175+elbow);  //Elbow. Vertical up = 175.
        myservo4.write(90+wristAngle);  //Wrist. Vertical = 90.
        myservo5.write(30);  //Less is more closed
    }
 * 
void findAngles(float localHeight, float localDepth, float wristAngleToGround){
    float offsetsHeight = sin(radians(wristAngleToGround))*hand;
    float offsetsDepth = cos(radians(wristAngleToGround))*hand;
    float wristHeight = localHeight-offsetsHeight-baseHeight;
    float wristDepth = localDepth-offsetsDepth;
    float shoulderToWrist = sqrt((wristHeight*wristHeight)+(wristDepth*wristDepth));
    float angle1 = atan2(wristHeight,wristDepth);
    float angle2 = acos(((humerus*humerus)-(ulna*ulna)+(shoulderToWrist*shoulderToWrist))/((2*humerus)*shoulderToWrist));
    shoulder = int((angle1+angle2)*180/pi);
    elbow = int(-(180-(acos(((humerus*humerus)+(ulna*ulna)-(shoulderToWrist*shoulderToWrist))/((2*humerus)*ulna)))*180/pi));
    wristAngle = wristAngleToGround-elbow-shoulder;
}

void serialDebug() {
    Serial.print(setHeight); //LEFT RIGHT(1024)
    Serial.print("\t");
    Serial.print(setDepth); //DOWN UP(1024)
    Serial.print("\t");
    Serial.print(setAngle);//LEFT RIGHT(1024)
    Serial.print("\t");
    Serial.print(shoulder);
    Serial.print("\t");
    Serial.print(elbow);
    Serial.print("\t");
    Serial.print(setRotation);
    Serial.print("\t");
    Serial.print(rotationDepth);
    Serial.print("\n");
    delay(30);
}
*/

