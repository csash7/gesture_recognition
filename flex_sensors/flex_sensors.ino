#include <HIDMouse.h>
#include <Arduino_LSM9DS1.h>

HIDMouse bleMouse;

unsigned long time1;
unsigned long time2;
bool left_pressed = false;
bool right_pressed = false; 

int flex1 = A0;
int flex2 = A1;
int flex_data1 = 0;
int flex_data2 = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

    if (!IMU.begin()) {
    while (1);
  }
  pinMode(flex1, INPUT);
  pinMode(flex2, INPUT);

  bleMouse.setDeviceName("Device");
  bleMouse.setManufacturerName("Manufacturer");
  bleMouse.setBatteryLevel(75);
  bleMouse.begin();

}

void loop() {

  float ax, ay, az;
  float prevxValue = 0, prevyValue = 0;
  int xAcc = 0, yAcc = 0, maxPosAcc = 5, maxNegAcc = -5;
  
   while(bleMouse.isConnected()) {

    if (IMU.accelerationAvailable  ()) {
    IMU.readAcceleration(ax, ay, az);

    if(ax > 0.3){
      if(prevxValue > 0.3){
        if(yAcc < maxPosAcc){
        yAcc += 1;
        }
      }
      else {
        prevxValue = ax;
        yAcc = 1;
        }
      bleMouse.move(0,yAcc); // y-Axis down
      }
    else if(ax < -0.3){
      if(prevxValue < -0.3){
        if(yAcc > maxNegAcc){
        yAcc -= 1;
        }
      }
      else {
        prevxValue = ax;
        yAcc = -1;
        }
      bleMouse.move(0,yAcc); // y-axis up
      }
    if(ay > 0.3){
      if(prevyValue > 0.3){
        if(xAcc > maxNegAcc){
        xAcc -= 1;
        }
      }
      else {
        prevyValue = ay;
        xAcc = -1;
        }
      bleMouse.move(xAcc,0); // x-axis right
      }
    else if(ay < -0.3){
      if(prevyValue < -0.3){
        if(xAcc < maxPosAcc){
        xAcc += 1;
        }
      }
      else {
        prevyValue = ay;
        xAcc = 1;
        }
      bleMouse.move(xAcc,0); // x-axis left
      }  
  }
      

  flex_data1 = analogRead(flex1);
  flex_data2 = analogRead(flex2);

  if(!left_pressed && flex_data2 < 160){
    left_pressed = true;
    bleMouse.press(MOUSE_BUTTON_RIGHT);
   Serial.println("LEFT CLICK Pressed");
    }
   if(!right_pressed && flex_data1 < 160){
    right_pressed = true;
    bleMouse.press(MOUSE_BUTTON_LEFT);
    Serial.println("RIGHT CLICK Pressed");
    }
    if(left_pressed && flex_data2 > 250){
      left_pressed = false;
      bleMouse.release(MOUSE_BUTTON_RIGHT);
      Serial.println("LEFT RELEASED");
      }
     if(right_pressed && flex_data1 > 250){
      right_pressed = false;
      bleMouse.release(MOUSE_BUTTON_LEFT);
      Serial.println("RIGHT RELEASED");
      }
  /*  
  Serial.print("flex1: ");
  Serial.print(flex_data1);
  Serial.print(", flex2: ");
  Serial.println(flex_data2);
  delay(1000);
  */
    }
}
