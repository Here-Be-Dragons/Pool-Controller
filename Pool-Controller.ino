// This #include statement was automatically added by the Particle IDE.
#include "Adafruit_SSD1306/Adafruit_SSD1306.h" //OLED

// Refer to README.md for information regarding which
// combinations of relay on/off create specific speeds

// Update the below array with the settings from 
// the pump for output to the Screen, SmartThings, and
// estimating turnover/energy use.  This doesn't actually
// change the speeds on the pump.
// speedRPM[0] must be 0 to turn the pump off
// 1200RPM is the minimum speed for chlorinator flow sensor
// 2750RPM is the recommended speed for 9 solar panels (~45GPM)
// 3450RPM for pool cleaning
uint16_t speedRPM[8] = {0,600,1200,1800,2300,2750,3000,3450};

// Schedule.  Higher speeds take precidence
#define speed1HourStart   0
#define speed1MinuteStart 0
#define speed1HourEnd     23
#define speed1MinuteEnd   59
#define speed2HourStart   0
#define speed2MinuteStart 0
#define speed2HourEnd     23
#define speed2MinuteEnd   59
#define speed3HourStart   0
#define speed3MinuteStart 0
#define speed3HourEnd     23
#define speed3MinuteEnd   59
#define speed3HourStart   0
#define speed3MinuteStart 0
#define speed3HourEnd     23
#define speed3MinuteEnd   59
#define speed3HourStart   0
#define speed3MinuteStart 0
#define speed3HourEnd     23
#define speed3MinuteEnd   59
#define speed3HourStart   0
#define speed3MinuteStart 0
#define speed3HourEnd     23
#define speed3MinuteEnd   59
#define speed3HourStart   0
#define speed3MinuteStart 0
#define speed3HourEnd     23
#define speed3MinuteEnd   59
#define speed3HourStart   0
#define speed3MinuteStart 0
#define speed3HourEnd     23
#define speed3MinuteEnd   59

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

void setPumpSpeed(int motorSpeed, int durationSecs) {
  switch (motorSpeed) {
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
      digitalWrite(pPumpRelay1, LOW );
      digitalWrite(pPumpRelay2, LOW );
      digitalWrite(pPumpRelay3, LOW );
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
