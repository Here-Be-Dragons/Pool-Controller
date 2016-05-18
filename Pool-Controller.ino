// This #include statement was automatically added by the Particle IDE.
#include "Adafruit_SSD1306/Adafruit_SSD1306.h" //OLED

// Refer to README.md for information regarding which
// combinations of relay on/off create specific speeds

// Update the below array with the settings from 
// the pump for output to the Screen, SmartThings, and
// estimating turnover/energy use.  This doesn't actually
// change the speeds on the pump, it's just bookkeeping.
// speedRPM[0] must be 0 to turn the pump off
// 1200RPM is the minimum speed for chlorinator flow sensor
// 2750RPM is the recommended speed for 9 solar panels (~45GPM)
// 3450RPM for pool cleaning
uint16_t speedRPM[8] = {0,600,1200,1800,2300,2750,3000,3450};
// Manually trigger each of the above speeds and record their energy usage here
uint16_t energyConsum[8] = {5,100,200,300,400,500,600,700};

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

uint32_t currentTime = 0;     //The current time
uint32_t currentSpeed = 1;    //The current motor speed
uint32_t lastChange = 0;      //Time the speed was last changed via scheduler
uint32_t lastLoop = 0;        //Time of the last successful loop
uint8_t motorSpeed = 1;       //Valid values 1-8, relates to the speed setting to call
bool autoOverride = 0;        //0 for scheduled, 1 for override.  Changes when solar kicks on
bool manualOverride = 0;      //0 for scheduled, 1 for override.  Changes from user intervention
uint16_t overrideExpiry = 0;  //This is set when a manual override is triggered 

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
    Particle.function("Override Schedule", overrideSchedule); //Force speed from user input
    Time.zone(-5);
    delay(5000); //Give it time to stabilize the RTC and get a time from NTP
    findScheduledSpeed();
    
}

void loop() {
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

  if (Time.hour() < 6 || Time.hour() > 20) {
        refresh = 1000; //~17 Minutes
    }
    else {
        refresh = 109; //70 seconds
    }
}

int findScheduledSpeed(){
  currentTime = Time.hour() * 100 + Time.minute();
  for (int i=0; i < sizeof(aSpeed1) / sizeof(uint16_t); i++) {// Find the Scheduled Speed
    if ( aSpeed1[i] == currentTime ) {
      motorSpeed = 1;
      lastChange = Time.hour() * 100 + Time.minute();
    }
    if ( aSpeed2[i] == currentTime ) {
      motorSpeed = 2;
      lastChange = Time.hour() * 100 + Time.minute();
    }
    if ( aSpeed3[i] == currentTime ) {
      motorSpeed = 3;
      lastChange = Time.hour() * 100 + Time.minute();
    }
    if ( aSpeed4[i] == currentTime ) {
      motorSpeed = 4;
      lastChange = Time.hour() * 100 + Time.minute();
    }
    if ( aSpeed5[i] == currentTime ) {
      motorSpeed = 5;
      lastChange = Time.hour() * 100 + Time.minute();
    }
    if ( aSpeed6[i] == currentTime ) {
      motorSpeed = 6;
      lastChange = Time.hour() * 100 + Time.minute();
    }
    if ( aSpeed7[i] == currentTime ) {
      motorSpeed = 7;
      lastChange = Time.hour() * 100 + Time.minute();
    }
    if ( aSpeed1[8] == currentTime ) {
      motorSpeed = 8;
      lastChange = Time.hour() * 100 + Time.minute();
    }
  }
}

void setPumpSpeed(motorSpeed, int durationSecs) {
  if (manualOverride != 1) { // Is there an active manual override?
    if (autoOverride != 1) { // Is there an active automatic override?
      switch (motorSpeed) {
        case 1:
          digitalWrite(pPumpRelay1, LOW );
          digitalWrite(pPumpRelay2, LOW );
          digitalWrite(pPumpRelay3, LOW );
          currentSpeed=1;
          break;
        case 2:
          digitalWrite(pPumpRelay1, HIGH);
          digitalWrite(pPumpRelay2, LOW );
          digitalWrite(pPumpRelay3, LOW );
          currentSpeed=2;
          break;
        case 3:
          digitalWrite(pPumpRelay1, LOW );
          digitalWrite(pPumpRelay2, HIGH);
          digitalWrite(pPumpRelay3, LOW );
          currentSpeed=3;
          break;
        case 4:
          digitalWrite(pPumpRelay1, HIGH);
          digitalWrite(pPumpRelay2, HIGH);
          digitalWrite(pPumpRelay3, LOW );
          currentSpeed=4;
          break;
        case 5:
          digitalWrite(pPumpRelay1, LOW );
          digitalWrite(pPumpRelay2, LOW );
          digitalWrite(pPumpRelay3, HIGH);
          currentSpeed=5;
          break;
        case 6:
          digitalWrite(pPumpRelay1, HIGH);
          digitalWrite(pPumpRelay2, LOW );
          digitalWrite(pPumpRelay3, HIGH);
          currentSpeed=6;
          break;
        case 7:
          digitalWrite(pPumpRelay1, LOW );
          digitalWrite(pPumpRelay2, HIGH);
          digitalWrite(pPumpRelay3, HIGH);
          currentSpeed=7;
          break;
        case 8:
          digitalWrite(pPumpRelay1, HIGH);
          digitalWrite(pPumpRelay2, HIGH);
          digitalWrite(pPumpRelay3, HIGH);
          currentSpeed=8;
          break;
        default: 
          digitalWrite(pPumpRelay1, LOW );
          digitalWrite(pPumpRelay2, LOW );
          digitalWrite(pPumpRelay3, LOW );
          currentSpeed=1;
          break;
      }
}

void overrideSchedule(String command){
    
    if(command == "0"){
        setPumpSpeed(1,);
    }
    
    else if(command == "1"){
        setPumpSpeed(2);
    }
    
    else if(command == "2"){
        setPumpSpeed(3);
    }

    else if(command == "3"){
        setPumpSpeed(4);
            
        }  
    else if(command == "4") {
        setPumpSpeed(5);
            
        }  
    else if(command == "5") {
        setPumpSpeed(6);
            
        } 
    else if(command == "6") {
        setPumpSpeed(7);
        }
    
    else if(command == "7"){
        setPumpSpeed(8);
    }  
}
