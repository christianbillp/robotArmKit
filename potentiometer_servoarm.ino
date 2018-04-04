#include <Servo.h> 

//Constants (robot dimensions in mm)
int baseHeight = 50;
int humerus = 50;
int ulna = 50;
int hand = 80;
const float pi = 3.141593;

float shoulder;
float elbow;
float wristAngle;
int setHeight;
int setDepth;
int setAngle;

int rotationStartDepth; //Initial depth value before compensating for rotation
int setRotation;  //
int rotationDepth;  //Depth value after rotation

//PINOUT
int servoPin[] = {3,5,6,9,10};
int gripperButton = 12;

//Servo object declaration using <Servo.h>
Servo myservo;  //A-1 (Base)
Servo myservo2; //A-2 (Shoulder)
Servo myservo3; //A-3 (Elbow)
Servo myservo4; //A-4 (Wrist)
Servo myservo5; //A-5 (Gripper)

void setup(){
    Serial.begin(115200);
    myservo.attach(servoPin[0]);  //A-1 (Base) 
    myservo2.attach(servoPin[1]); //A-2 (Shoulder)
    myservo3.attach(servoPin[2]); //A-3 (Elbow)
    myservo4.attach(servoPin[3]); //A-4 (Wrist)
    myservo5.attach(servoPin[4]); //A-5 (Gripper)
    pinMode(gripperButton, INPUT_PULLUP);
}

void loop() {
    serialDebug();
    setHeight = map(analogRead(A0),0,1024,10,200);
    setRotation = map(analogRead(A1),0,1024,-80,80);
    rotationStartDepth = map(analogRead(A2),0,1024,10,200);
    setAngle = map(analogRead(A3),0,1024,-90,90);
    rotationDepth = constrain(rotationStartDepth/cos(setRotation*0.0174532925),10,200);
    findAngles(setHeight,rotationDepth,setAngle); //End effector angle locked to 0!
    if (shoulder != 0) {
        myservo.write(90+setRotation);  //Base.Neutral position = 100 100+setRotation
        myservo2.write(shoulder);     //Shoulder. Vertical = 90.
        myservo3.write(175+elbow);  //Elbow. Vertical up = 175.
        myservo4.write(90+wristAngle);  //Wrist. Vertical = 90.
        if (digitalRead(gripperButton) == 1) {
            myservo5.write(30);  //Less is more closed
        }
        else {
            myservo5.write(90);  //Less is more closed
        }
    }
    else Serial.println("OUTSIDE AREA OF OPERATION");    
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
