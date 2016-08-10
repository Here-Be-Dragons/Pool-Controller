// This #include statement was automatically added by the Particle IDE.
#include "Adafruit_SSD1306/Adafruit_SSD1306.h" //OLED

// Refer to README.md for additional information
// https://github.com/Here-Be-Dragons/Pool-Controller

// TODO: Fix SmartThings override

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
// energy usage (Watts) below for accurate consumption tracking
uint16_t energyConsum[8] = {5,100,200,400,800,1100,1400,1700};

// Estimate total gallons of flow per minute
uint16_t flowCalc[8] = {0,18,35,53,68,81,88,102};

// How long (in seconds) to run manual Overrides
uint16_t overrideLength = 3600;

// Time Zone offset
int16_t timeZone = -5;

// Speed activation times in HHMM format. If two speeds have the same
// time entered, the higher speed takes precidence.  Leave off any
// preceeding zeros, as they will prevent a proper match.
uint16_t aSpeed1[] = {0,                  1900}; //    0 RPM
uint16_t aSpeed2[] = {                        }; //  600 RPM
uint16_t aSpeed3[] = { 900,   1100,   1400    }; // 1200 RPM
uint16_t aSpeed4[] = {                        }; // 1800 RPM
uint16_t aSpeed5[] = {                        }; // 2300 RPM
uint16_t aSpeed6[] = {    1000,   1300        }; // 2750 RPM
uint16_t aSpeed7[] = {                        }; // 3000 RPM
uint16_t aSpeed8[] = {                        }; // 3450 RPM

////
//End of user configurations
////

time_t currentEpochTime = 0;     //The current Epoch time set at the beginning of each loop in getTimes()
uint16_t currentTime = 0;        //Friendly time converted from currentEpochTime via convertTime(), 10:00 PM is referenced as 2200
uint16_t previousTime = 0;       //The time as of the last loop, set at the bottom of loop()
double WhTally = 0;              //Daily count of Wh consumption, to upload to Google Docs for tracking
double gallonTally = 0;          //Daily count of Gallons pumped, to upload to Google Docs for tracking
char publishString[40];          //Temporary string to use for Particle.publish of WhTally
uint16_t currentSpeed = 0;       //The current motor speed setting number (1-8)
uint16_t overrideSpeed;          //Stores the override speed when set manually
//uint16_t lastChange;           //Time the speed was last changed via scheduler (not currently used)
uint16_t scheduledSpeed = 0;     //What speed the schedule says you should be at
uint8_t autoOverride = 1;        //0 for scheduled, 1 for override.  Changes when solar kicks on
uint8_t manualOverride = 0;      //0 for scheduled, 1 for override.  Changes via user intervention
uint32_t overrideStarted = 0;    //This is set to currentEpochTime when a manual override is triggered
uint32_t previousDataPublish;    //Webhook publish tracking

// Assign pins to relays
// D0 reserved for SDA of OLED
// D1 reserved for SCL of OLED
#define pPumpRelay1     D2
#define pPumpRelay2     D3
#define pPumpRelay3     D4
#define pSolarRelay1    D6
#define OLED_RESET      D5 //Not sure what this is for, not connected
#define pButtons        A0

Adafruit_SSD1306 oled(OLED_RESET);

void setup() {
    pinMode(pPumpRelay1,  OUTPUT);
    pinMode(pPumpRelay2,  OUTPUT);
    pinMode(pPumpRelay3,  OUTPUT);
    pinMode(pSolarRelay1, INPUT); // Solar panel on pulls this down to ground.  Need INPUT_PULLUP for inactive relay, else it would float
    pinMode(pButtons, INPUT);

    oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);    // Initialize with the I2C addr 0x3D (for the 128x64 screen)
    
    for (int x = 30; x >= 0; x--){                   // Give it x seconds to stabilize the RTC and get a time from NTP
        oled.clearDisplay();                       // Clear Screen
        oled.drawRect(0,0,128,64, WHITE);
        oled.drawRect(1,1,127,63, WHITE);
        oled.setTextColor(WHITE);
        oled.setCursor(5,5);
        oled.setTextSize(1);
        oled.println("Pool-Controller v1.0");
        oled.print("    by Jerad Jacob");
        oled.setCursor(30,50);
        oled.print("Startup: ");
        oled.print(x);
        oled.display();
        delay(1000);
    }
    Particle.function("mOverride", mOverride); // Listen for a manual override via remote user input
    getTimes();								   // Set up initial times
    returnToSchedule();                        // Find what the current speed should be
}

void loop() {
    //Finds the current time each loop
    getTimes();

    //Check for expired manual Override
    if( manualOverride == 1 ) {
        uint32_t ovr = overrideStarted + overrideLength;
    if (ovr <= currentEpochTime) {
        returnToSchedule();
    }
}

    //Check for new scheduled speed
    scheduledSpeed = findScheduledSpeed(currentTime);

    //Check for Solar Speed Override.  Applies in setPumpSpeed()
    autoOverride = digitalRead(pSolarRelay1);

    //Activate new speed if there is one
    setPumpSpeed();

    //Update the oled display
    updateDisplay();
  
    //Keep track of energy consumption and speed
    trackData();
}

void getTimes(){
    //Time.zone() does not suppport DST, so implement it manually
    if( Time.month() < 3 || (Time.month() == 3 && Time.day() <=13) || (Time.month() == 11 && Time.day() >=6) || Time.month() > 11 ){
        Time.zone(timeZone);
    }else{
        Time.zone(timeZone + 1);
    }
    //Gets Time-zone adjusted Epoch Time from my butt, and converted HHMM comparison time
    currentEpochTime = Time.now();
    currentTime = convertTime(currentEpochTime);
}

uint16_t convertTime(uint32_t testTime1){
    //Converts Epoch to HHMM
    uint16_t returnTime;
    returnTime = Time.hour(testTime1) * 100 + Time.minute(testTime1);
    return returnTime;
}

uint16_t findScheduledSpeed(uint16_t atTime){ // Find the Scheduled Speed
    uint16_t x = scheduledSpeed;
    for (uint16_t i=0; i < sizeof(aSpeed1) / sizeof(uint16_t); i++) {
        if ( aSpeed1[i] == atTime ) {
            x = 1;
        }
    }
    for (uint16_t i=0; i < sizeof(aSpeed2) / sizeof(uint16_t); i++) {
        if ( aSpeed2[i] == atTime ) {
            x = 2;
        }
    }
    for (uint16_t i=0; i < sizeof(aSpeed3) / sizeof(uint16_t); i++) {
        if ( aSpeed3[i] == atTime ) {
            x = 3;
        }
    }
    for (uint16_t i=0; i < sizeof(aSpeed4) / sizeof(uint16_t); i++) {
        if ( aSpeed4[i] == atTime ) {
            x = 4;
        }
    }
    for (uint16_t i=0; i < sizeof(aSpeed5) / sizeof(uint16_t); i++) {
        if ( aSpeed5[i] == atTime ) {
            x = 5;
        }
    }
    for (uint16_t i=0; i < sizeof(aSpeed6) / sizeof(uint16_t); i++) {
        if ( aSpeed6[i] == atTime ) {
            x = 6;
        }
    }
    for (uint16_t i=0; i < sizeof(aSpeed7) / sizeof(uint16_t); i++) {
        if ( aSpeed7[i] == atTime ) {
            x = 7;
        }
    }
    for (uint16_t i=0; i < sizeof(aSpeed8) / sizeof(uint16_t); i++) {
        if ( aSpeed1[8] == atTime ) {
            x = 8;
        }
    }
    return x;
}

void setPumpSpeed() {
    uint8_t newSpeed;
    newSpeed = scheduledSpeed;
  
    //Set Manual Override Speed to newSpeed if active
    if( manualOverride == 1 ){
        newSpeed = overrideSpeed;
    }
  
    //If auto override is active, set a minimum speed
    if( autoOverride == 1 && newSpeed < 6 ){
        newSpeed = 6;
    }
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
	    //This condition should never happen, but default to Speed 1 if it does
        default:
            digitalWrite(pPumpRelay1, LOW );
            digitalWrite(pPumpRelay2, LOW );
            digitalWrite(pPumpRelay3, LOW );
            returnToSchedule(); //Try a schedule reset
            break;
    }
    currentSpeed = newSpeed; //Update current speed value
}

int mOverride(String command) { //Triggered by SmartThings
    overrideSpeed = atoi(command); //Convert string to integer
    if( overrideSpeed <= 8 ){ //These are direct speeds
        manualOverride = 1;
        overrideStarted = currentEpochTime;
        setPumpSpeed();
    } else {
        if( overrideSpeed == 9 ){ //9 is "return to schedule"
            manualOverride = 0;
            returnToSchedule();
        } else if( overrideSpeed == 10){ //10 is a poll for current speed, no changes
        //Just a poll, no changes
        }
    }
    return speedRPM[currentSpeed-1];
}

void returnToSchedule() {
    scheduledSpeed = 0;
    uint32_t testTimeEpoch = currentEpochTime;
    uint16_t testTime;
    while( scheduledSpeed == 0 ) {
	    testTime = convertTime(testTimeEpoch);
        scheduledSpeed = findScheduledSpeed(testTime);
        testTimeEpoch-=60;
    }
    oled.print ("SUCCESS! ");
    oled.print("Speed: ");
    oled.println(scheduledSpeed);
    oled.display();
    manualOverride = 0;
    overrideSpeed = 0;
    overrideStarted = 0;
    delay(2000);
}

void trackData(){
    if (currentTime != previousTime) {
        if (currentTime == 0) {
	        sprintf(publishString, "%.5f", WhTally);     // Convert double WhTally to char[40] for Particle.publish()
	        Particle.publish("24Hr_kWh", publishString);
            sprintf(publishString, "%.5f", gallonTally); // Convert double gallonTally to char[40] for Particle.publish()
            Particle.publish("24Hr_gal", publishString); 
	        WhTally = 0;
            gallonTally = 0;
        }
        WhTally += (double) ( energyConsum[currentSpeed-1] / 60 ); // Add 1 minute worth of kWh
        gallonTally += (double) ( flowCalc[currentSpeed-1] );      // Add 1 minute worth of water flow
        previousTime = currentTime;
    }
    if (previousDataPublish + 300 <= currentEpochTime) { // Publish to thingspeak channel every 5 minutes
        String sSpeed = String(speedRPM[currentSpeed - 1]);
        String sWattage = String(energyConsum[currentSpeed - 1]);
        String sFlow = String(flowCalc[currentSpeed - 1]);
        String sSolar = String(autoOverride);
        Particle.publish("poolLog", "{ \"1\": \"" + sSpeed + "\", \"2\": \"" + sWattage + "\", \"3\": \"" + sFlow + "\", \"4\": \"" + sSolar + "\" }", PRIVATE);
        previousDataPublish = currentEpochTime;
    }
}

void updateDisplay(){ //128x64
    oled.clearDisplay();
    oled.drawRect(80,0,48,55, WHITE);
    oled.drawRect(81,1,46,53, WHITE);
    oled.setTextSize(6);
    oled.setTextColor(WHITE);
    oled.setCursor(88,6);
    oled.print(currentSpeed);
    oled.setTextSize(1);
    oled.setCursor(80,57);
    oled.print(speedRPM[currentSpeed-1]);
    oled.print(" RPM");
    oled.setCursor(0,0);
    oled.print("Time: ");
    oled.println(currentTime);
    oled.print("Wh: ");
    oled.println(WhTally);
    oled.print("Scheduled: ");
    oled.println(scheduledSpeed);
    if( manualOverride ) {
        oled.println("MANUAL OVERRIDE:");
        oled.print("Spd: ");
        oled.println(overrideSpeed);
        oled.print("Timer: ");
        oled.println(overrideStarted + overrideLength - currentEpochTime);
    }
    if( autoOverride ) oled.println("AUTO OVERRIDE");
    oled.display();
}
