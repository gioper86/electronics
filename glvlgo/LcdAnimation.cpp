#include "DeliveryStatus.cpp"

class LcdAnimation {
  int xPosition;
  int screenWidth;
  int imageWidth;
  unsigned long millisMarker;
  DeliveryStatus deliveryStatus;
  
  public:
  LcdAnimation(int sw, int imageWidth) {
    xPosition = -imageWidth;
    screenWidth = sw;
    millisMarker = 0;
  }

  bool animate() {
    if (!isFinished() && (millis() - millisMarker) > 100) {
      xPosition = xPosition + 2;
      millisMarker = millis();
      // refreshDisplay(deliveryStatus, bmp);
      return true;
    }
    return false;
  }

  int getXPosition() {
    return xPosition;
  }

  void setDeliveryStatus(DeliveryStatus sp) {
    deliveryStatus = sp;
  }

  bool isFinished() {
    if(xPosition >= screenWidth) {
      return true;
    }
    return false;
  }

  void reset() {
    xPosition = -imageWidth;
  }
};
