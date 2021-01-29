#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 9
#define DATA_PIN 13

typedef struct {
  unsigned long lastActivationTime;
} led;

constexpr int LED_COUNT= 8;

//Vibration sensors
constexpr int V_SENSOR_START = 2;
constexpr int V_SENSOR_END = 9;

// Motion sensors
constexpr int M_SENSOR_BOTTOM = 11;
constexpr int M_SENSOR_TOP= 10;
int lastMotionBotVal = 0;
int lastMotionTopVal = 0;



CRGB leds[NUM_LEDS];
led ledActivations[NUM_LEDS];



char buffer[50];
void setup() {
  Serial.begin(9600);

  pinMode(M_SENSOR_BOTTOM, INPUT);
  pinMode(M_SENSOR_TOP, INPUT);

  // put your setup code here, to run once:
  FastLED.addLeds<WS2811, DATA_PIN, BRG>(leds, NUM_LEDS);
  for(int i = V_SENSOR_START; i <= V_SENSOR_END; i++) {
    sprintf(buffer, "Sensor init %d\n",i);
    Serial.print(buffer);
   
    pinMode(i, INPUT);
  }

  for(int i=0; i< NUM_LEDS; i++) {
    ledActivations[i].lastActivationTime = millis();
  }
  
  TurnOffAllLeds();
  Serial.print("Starting...");

}

void loop() {
  int val;
  int motionBotVal;
  int motionTopVal;
  
  motionBotVal=digitalRead(M_SENSOR_BOTTOM);
  if(motionBotVal == 1 && lastMotionBotVal == 0) {
    FadeIn(0);
    sprintf(buffer, "Bottom motion sensor is on\n");
    Serial.print(buffer);
  } 
  lastMotionBotVal=motionBotVal;
  motionTopVal=digitalRead(M_SENSOR_TOP);
  if(motionTopVal == 1 && lastMotionTopVal == 0) {
    FadeIn(8);   
    sprintf(buffer, "Top motion sensor is on\n");
    Serial.print(buffer);
  }
  lastMotionTopVal=motionTopVal;
  
  for(int i= V_SENSOR_START; i <= V_SENSOR_END; i++) {

    val=digitalRead(i);
   // sprintf(buffer, "Val of sensor %d = %d\n",i, val);
   //     Serial.print(buffer);

    if(val==1 && i >= V_SENSOR_START) {
      sprintf(buffer, "Sensor %d is on\n",i);
      Serial.print(buffer);
      FadeIn(i-1);
    }
  }

  for(int i=0; i< NUM_LEDS; i++) {
    unsigned long CurrentTime = millis();
    unsigned long ElapsedTime = CurrentTime - ledActivations[i].lastActivationTime;
    if(ElapsedTime > 5000 && leds[i] ) {
      FadeOff(i);
    }
  }
}

// TODO: better sensor and led numbering
void FadeIn(int led) {
  for(int i = leds[led].r; i <= 255; i+=5) {
    ledActivations[led].lastActivationTime = millis();
    leds[led]=CRGB(i,i,i);
    FastLED.show();
  }
  if(led != 0) {
    for(int i = leds[led-1].r; i <= 125; i+=5) { 
      ledActivations[led-1].lastActivationTime = millis();
      leds[led-1]=CRGB(i,i,i);
      FastLED.show();
    }
  }
  if(led != LED_COUNT) {
    for(int i = leds[led+1].r; i <= 125; i+=5) {
      ledActivations[led+1].lastActivationTime = millis();
      leds[led+1]=CRGB(i,i,i);
      FastLED.show();
    }
  }
}


void TurnOffAllLeds() {
  for(int i = 0; i < NUM_LEDS; i++) {
    FadeOff(i);
  }
}

void FadeOff(int led) {
  sprintf(buffer, "Fade of %d led\n",led);
  Serial.print(buffer);
  leds[led].subtractFromRGB(5);
      FastLED.show();

 /* for(int i = 255; i >= 0; i=-5) {
    leds[led].subtractFromRGB(5);
    FastLED.show();
  }*/
}
