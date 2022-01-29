#ifndef StatusPage
#define StatusPage

#include <Arduino.h>

class DeliveryStatus {
  public:
    String eta;
    String message;
    String orderStatus;  
};

#endif
