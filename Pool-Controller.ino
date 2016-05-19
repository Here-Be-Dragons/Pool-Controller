// This #include statement was automatically added by the Particle IDE.
#include "Adafruit_SSD1306/Adafruit_SSD1306.h" //OLED

// Refer to README.md for additional information
// https://github.com/Here-Be-Dragons/Pool-Controller

////
//Start of user configurations
////

// Update the below array with the settings from 
// the pump for output to the Screen, SmartThings, and
// estimating turnover/energy use.  This doesn't actually
// change the speeds on the pump, it's just bookkeeping.
// speedRPM[0] must be 0 to turn the pump off
// 1200RPM is the minimum speed for chlorinator flow sensor
// 2750RPM is the recommended speed for 9 solar panels (~45GPM)
// 3450RPM for pool cleaning
uint16_t speedRPM[8] = {0,600,1200,1800,2300,2750,3000,3450};
// Manually trigger each of the above speeds and record their
// energy usage below for accurate consumption tracking
uint16_t energyConsum[8] = {5,100,200,300,400,500,600,700};
uint16_t expiryLength = 60; //How long to run manual Overrides

// Speed activation times. If two speeds have the same
// time entered, the higher speed takes precidence.
uint16_t aSpeed1[] = {0,                  2100}
uint16_t aSpeed2[] = {                        }
uint16_t aSpeed3[] = { 600,   1200,   1500    }
uint16_t aSpeed4[] = {                        }
uint16_t aSpeed5[] = {                        }
uint16_t aSpeed6[] = {    1100,   1300        }
uint16_t aSpeed7[] = {                        }
uint16_t aSpeed8[] = {                        }

////
//End of user configurations
////

uint16_t currentTime;     //The current time
uint16_t currentSpeed = 1;    //The current motor speed
uint16_t lastChange;      //Time the speed was last changed via scheduler
uint8_t newSpeed = 1;       //Valid values 1-8, relates to the speed setting to call
bool autoOverride = 0;        //0 for scheduled, 1 for override.  Changes when solar kicks on
bool manualOverride = 0;      //0 for scheduled, 1 for override.  Changes from user intervention
uint16_t OverrideStarted = 0;  //This is set when a manual override is triggered 

// Assign pins to relays
// D0 reserved for SDA of OLED
// D1 reserved for SCL of OLED
#define pPumpRelay1     D2
#define pPumpRelay2     D3
#define pPumpRelay3     D4
#define pSolarRelay1    D5
#define OLED_RESET      D6

Adafruit_SSD1306 oled(OLED_RESET);

void setup() {
    pinMode(pPumpRelay1,  OUTPUT);
    pinMode(pPumpRelay2,  OUTPUT);
    pinMode(pPumpRelay3,  OUTPUT);
    pinMode(pSolarRelay1, INPUT );

    oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64 screen)
    oled.display(); // show splashscreen
    Particle.function("mOverride", mOverride); //Force speed from user input
    Time.zone(-5);
    delay(5000); //Give it time to stabilize the RTC and get a time from NTP
    setPumpSpeed(1);
}

void loop() {
  currentTime = Time.hour() * 100 + Time.minute();
  //checkAutoOverride(); //Need a way to check if we should be runing in auto override
  if( manualOverride == 1 && OverrideStarted <= currentTime + expiryLength ){
    endOverride();
  }
  newSpeed = findScheduledSpeed(currentTime);
  if( newspeed != 0 && newSpeed != currentSpeed ){
    setPumpSpeed(newSpeed);
  }
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(WHITE);
  oled.setCursor(0,0);
  oled.println("5-hr rain forecast:");
  oled.println("Units: Inches");
  oled.println();
  oled.setTextSize(3);
  oled.print(precipInSum);
  oled.display();
}

int findScheduledSpeed(uint16_t atTime){ // Find the Scheduled Speed
  for (int i=0; i < sizeof(aSpeed1) / sizeof(uint16_t); i++) {
    if ( aSpeed1[i] == atTime ) {
      newSpeed = 1;
    }
  }
  for (int i=0; i < sizeof(aSpeed2) / sizeof(uint16_t); i++) {
    if ( aSpeed2[i] == atTime ) {
      newSpeed = 2;
    }
  }
  for (int i=0; i < sizeof(aSpeed3) / sizeof(uint16_t); i++) {
    if ( aSpeed3[i] == atTime ) {
      newSpeed = 3;
    }
  }
  for (int i=0; i < sizeof(aSpeed4) / sizeof(uint16_t); i++) {
    if ( aSpeed4[i] == atTime ) {
      newSpeed = 4;
    }
  }
  for (int i=0; i < sizeof(aSpeed5) / sizeof(uint16_t); i++) {
    if ( aSpeed5[i] == atTime ) {
      newSpeed = 5;
    }
  }
  for (int i=0; i < sizeof(aSpeed6) / sizeof(uint16_t); i++) {
    if ( aSpeed6[i] == atTime ) {
      newSpeed = 6;
    }
  }
  for (int i=0; i < sizeof(aSpeed7) / sizeof(uint16_t); i++) {
    if ( aSpeed7[i] == atTime ) {
      newSpeed = 7;
    }
  }
  for (int i=0; i < sizeof(aSpeed8) / sizeof(uint16_t); i++) {
    if ( aSpeed1[8] == atTime ) {
      newSpeed = 8;
    }
  }
  return newSpeed;
}

void setPumpSpeed(newSpeed) {
  if (manualOverride != 1) { // Is there an active manual override?
    if (autoOverride != 1) { // Is there an active automatic override?
      switch (newSpeed) {
        case 1:
          digitalWrite(pPumpRelay1, LOW );
          digitalWrite(pPumpRelay2, LOW );
          digitalWrite(pPumpRelay3, LOW );
          break;
        case 2:
          digitalWrite(pPumpRelay1, HIGH);
          digitalWrite(pPumpRelay2, LOW );
          digitalWrite(pPumpRelay3, LOW );
          break;
        case 3:
          digitalWrite(pPumpRelay1, LOW );
          digitalWrite(pPumpRelay2, HIGH);
          digitalWrite(pPumpRelay3, LOW );
          break;
        case 4:
          digitalWrite(pPumpRelay1, HIGH);
          digitalWrite(pPumpRelay2, HIGH);
          digitalWrite(pPumpRelay3, LOW );
          break;
        case 5:
          digitalWrite(pPumpRelay1, LOW );
          digitalWrite(pPumpRelay2, LOW );
          digitalWrite(pPumpRelay3, HIGH);
          break;
        case 6:
          digitalWrite(pPumpRelay1, HIGH);
          digitalWrite(pPumpRelay2, LOW );
          digitalWrite(pPumpRelay3, HIGH);
          break;
        case 7:
          digitalWrite(pPumpRelay1, LOW );
          digitalWrite(pPumpRelay2, HIGH);
          digitalWrite(pPumpRelay3, HIGH);
          break;
        case 8:
          digitalWrite(pPumpRelay1, HIGH);
          digitalWrite(pPumpRelay2, HIGH);
          digitalWrite(pPumpRelay3, HIGH);
          break;
        default:
          break;
      }
      currentSpeed = newSpeed;
      newSpeed = 0;
}

int mOverride(String command){ //Triggered by SmartThings
  uint8_t intCommand = atoi(command);
  manualOverride = 1;
  overrideStarted = Time.hour() * 100 + Time.minute();
  //overrideSchedule();
  setPumpSpeed(intCommand);
}

void endOverride(){
  int testTime = currentTime;
  newSpeed = 0;
  while( newSpeed == 0 ){
    newSpeed = findScheduledSpeed(testTime);
    testTime--;
  }
  manualOverride = 0;
}

int aOverride(){
  autoOverride = 1
}

/*void overrideSchedule(){
  currentTime = Time.hour() * 100 + Time.minute();
  setPumpSpeed(); //Convert string to int, then use that to set Pump Speed

  if(command == "1"){
      setPumpSpeed(1);
  }
  
  else if(command == "2"){
      setPumpSpeed(2);
  }
  
  else if(command == "3"){
      setPumpSpeed(3);
  }

  else if(command == "4"){
      setPumpSpeed(4);
          
      }  
  else if(command == "5") {
      setPumpSpeed(5);
          
      }  
  else if(command == "6") {
      setPumpSpeed(6);
          
      } 
  else if(command == "7") {
      setPumpSpeed(7);
      }
  
  else if(command == "8"){
      setPumpSpeed(8);
  }*/
}
