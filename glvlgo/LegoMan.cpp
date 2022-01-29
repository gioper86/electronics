#include <Servo.h>
#include "DeliveryStatus.cpp"

class LegoMan {
  Servo servo;
  int servoPin;
  int angleStart;
  int angleFinish;
  int angleCurrent;
  unsigned long millisMarker;
  unsigned long pauseMillis;
  int maxRepetitions;
  int currentRepetitions;
  bool isPaused;
  DeliveryStatus deliveryStatus;
  
  public:
  LegoMan(int srvPin, int aStart, int aFinish) {
    angleStart = aStart;
    angleCurrent = aStart;
    angleFinish = aFinish;
    servo.attach(srvPin);
    servo.write(aStart);
    millisMarker = 0;
    maxRepetitions = 5;
    currentRepetitions = 0;
    pauseMillis = 0;
  }

  void animate() {
    if(isFinished()) {
      return;
    }
    
    if((millis() - pauseMillis > 800)) {
      isPaused = false;
    } else {
      return;
    }
    
    if ((millis() - millisMarker) > 20) {
      millisMarker = millis();
      servo.write(angleCurrent);
      angleCurrent += 2;
    }
    
    if(angleCurrent >= angleFinish) {
      angleCurrent = angleStart;
      isPaused = true;
      pauseMillis = millis();
      currentRepetitions++;
    }
    
  }

  bool isFinished() {
    if(deliveryStatus.orderStatus != "Delivering") {
      return true;
    }
    
    if(currentRepetitions >= maxRepetitions) {
      return true;
    }
    return false;
  }

  void setDeliveryStatus(DeliveryStatus sp) {
    deliveryStatus = sp;
  }

  void reset() {
    currentRepetitions = 0;
  }
};
