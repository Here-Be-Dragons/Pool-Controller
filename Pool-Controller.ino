// This #include statement was automatically added by the Particle IDE.
#include "Adafruit_SSD1306/Adafruit_SSD1306.h" //OLED

// Refer to README.md for information regarding which
// combinations of relay on/off create specific speeds

// Update the below variables with the settings from 
// the pump for output to the Screen, SmartThings, and
// estimating turnover/energy use.  This doesn't actually
// change the speeds on the pump.

uint16_t speed1RPM = 0    // Speed 1 must be 0 to turn pump off
uint16_t speed2RPM = 600
uint16_t speed3RPM = 1200 // Minimum speed for chlorinator flow sensor
uint16_t speed4RPM = 1800
uint16_t speed5RPM = 2300
uint16_t speed6RPM = 2750 // Recommended speed for Solar Panels (~45GPM)
uint16_t speed7RPM = 3000
uint16_t speed8RPM = 3450 // Speed for pool cleaning

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
}

int overrideSchedule(String command){
    
    if(command == "0"){
        setPumpSpeed(1);
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
