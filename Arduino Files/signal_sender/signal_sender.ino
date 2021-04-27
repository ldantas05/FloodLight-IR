#include "IRLibAll.h"
 
//Create a receiver object to listen on pin 2
 
//Create a decoder object 
IRdecode myDecoder;   
IRsend mySender;
void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT); //red
  pinMode(4, INPUT); //blue
  pinMode(5, INPUT); //green
  pinMode(6, INPUT); //orange
  pinMode(7, INPUT); //purple
  pinMode(8, INPUT); //aqua
  pinMode(12, INPUT); //pink
  pinMode(13, INPUT); //yellow
  delay(2000); while (!Serial); //delay for Leonardo
}
 
void loop() {
  //Continue looping until you get a complete signal received
  //red
  if(digitalRead(2)){
    Serial.println(F("Got signal from Beagle red"));
    mySender.send(NEC,0xFF1AE5, 32);
  }
  //blue
  if(digitalRead(4)){
    Serial.println(F("Got signal from Beagle blue"));
    mySender.send(NEC,0xFFA25D, 32);
  }
  //green
  if(digitalRead(5)){
    Serial.println(F("Got signal from Beagle green"));
    mySender.send(NEC,0xFF9A65, 32);
  }

  //purple
  if(digitalRead(6)){
    Serial.println(F("Got signal from Beagle purple"));
    mySender.send(NEC,0xFFB24D, 32);
  }
  //aqua
  if(digitalRead(7)){
    Serial.println(F("Got signal from Beagle aqua"));
    mySender.send(NEC,0xFF8A75, 32);
  }
  //pink
  if(digitalRead(8)){
    Serial.println(F("Got signal from Beagle pink"));
    mySender.send(NEC,0xFF58A7, 32);
  }
  //yellow
  if(digitalRead(12)){
    Serial.println(F("Got signal from Beagle yellow"));
    mySender.send(NEC,0xFF18E7, 32);
  }

  //orange
  if(digitalRead(13)){
    Serial.println(F("Got signal from Beagle orange"));
    mySender.send(NEC,0xFF2AD5, 32);
  }
}
