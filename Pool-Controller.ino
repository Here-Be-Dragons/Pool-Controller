// Automated Pool Controller
// v2.1
// by Jerad Jacob
// Refer to README.md for additional information and latest updates
// https://github.com/Here-Be-Dragons/Pool-Controller

// This #include statement was automatically added by the Particle IDE.
#include "Adafruit_SSD1306/Adafruit_SSD1306.h" //OLED

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

// Estimate total gallons of flow per minute for each speed
uint16_t flowCalc[8] = {0,18,35,53,68,81,88,102};

// Default length (in seconds) to run manual Overrides
// Applied if no time is specified in override
uint16_t defaultOverride = 3600;

// Time Zone offset
int16_t timeZone = -5;

// Calibration for light sensor.
// 30 is a reasonable value if the box is tightly sealed.
uint16_t dispMinBrightness = 30;

// Speed activation times in HHMM format. If two speeds have the same
// time entered, the higher speed takes precidence.  Leave off any
// preceeding zeros, as they will prevent a proper match.

/*
/////
//Winter Schedule
/////
uint16_t aSpeed1[] = {0,                  1700}; //    0 RPM
uint16_t aSpeed2[] = {                        }; //  600 RPM
uint16_t aSpeed3[] = { 900,   1100,   1400    }; // 1200 RPM
uint16_t aSpeed4[] = {                        }; // 1800 RPM
uint16_t aSpeed5[] = {                        }; // 2300 RPM
uint16_t aSpeed6[] = {    1000,   1300        }; // 2750 RPM
uint16_t aSpeed7[] = {                        }; // 3000 RPM
uint16_t aSpeed8[] = {                        }; // 3450 RPM
*/

/////
//Summer Schedule
/////
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

// This value persists permanently on the Photon even after flashes/reboots
STARTUP(WiFi.selectAntenna(ANT_EXTERNAL)); // Use external antenna
//STARTUP(WiFi.selectAntenna(ANT_INTERNAL)); // Use internal antenna

time_t currentEpochTime = 0;    //The current Epoch time set at the beginning of each loop in getTimes()
uint16_t currentTime = 0;       //Friendly time converted from currentEpochTime via convertTime(), 10:00 PM is referenced as 2200
uint16_t previousTime = 0;      //The time as of the last loop, set at the bottom of loop()
double WhTally = 0;             //Daily count of Wh consumption, to upload to Google Docs for tracking
double gallonTally = 0;         //Daily count of Gallons pumped, to upload to Google Docs for tracking
char publishString[40];         //Temporary string to use for Particle.publish of WhTally
uint16_t currentSpeed = 0;      //The current motor speed setting number (1-8)
uint16_t overrideSpeed;         //Stores the override speed when set manually
//uint16_t lastChange;          //Time the speed was last changed via scheduler (not currently used)
uint16_t scheduledSpeed = 0;    //What speed the schedule says you should be at
uint8_t autoOverride = 1;       //0 for scheduled, 1 for override.  Changes when solar kicks on
uint8_t manualOverride = 0;     //0 for scheduled, 1 for override.  Changes via user intervention
time_t overrideStarted;	        //This is set to currentEpochTime when a manual override is triggered
time_t overrideEnds = 0;        //This is set to currentEpochTime + overrideLength when a manual override is triggered
uint32_t overrideLength = 0;    //Recieved override length in seconds.
time_t previousDataPublish;     //Webhook publish tracking (Epoch)
bool isBright = 1;              //Tracks whether the screen should be lit or not.
uint8_t lastDay;                //Used for onceADay()

String sSpeed;
String sWattage;
String sFlow;
String sSolar;
String sOverride;
String sTempSolar;
String sTempPool;
String sTempRoof;

// Assign pins to relays
// D0 reserved for SDA of OLED
// D1 reserved for SCL of OLED
#define pPumpRelay1     D2
#define pPumpRelay2     D3
#define pPumpRelay3     D4
#define pRelay4         D5
#define pSolarRelay1    D6
#define OLED_RESET      D7 //Not sure what this is for, not connected
#define pButtons        A0
#define pIllum          A1
#define pTempSolar      A2
#define pTempPool       A3
#define pTempRoof       A4

Adafruit_SSD1306 oled(OLED_RESET);

void setup() {
    
    pinMode(pPumpRelay1,        OUTPUT); // Pump Relay 1
    pinMode(pPumpRelay2,        OUTPUT); // Pump Relay 2
    pinMode(pPumpRelay3,        OUTPUT); // Pump Relay 3
    pinMode(pRelay4,        OUTPUT); // Solar Actuator Relay
    pinMode(pSolarRelay1,   INPUT); // Solar panel on pulls this up to 3.3v.
    pinMode(pButtons,       INPUT);
    pinMode(pIllum,         INPUT);
    pinMode(pTempSolar,     INPUT);
    pinMode(pTempPool,      INPUT);
    pinMode(pTempRoof,      INPUT);

    oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);    // Initialize with the I2C addr 0x3D (for the 128x64 screen)
    Particle.function("mOverride", mOverride); // Listen for a manual override via remote user input
    Particle.variable("pumpSpeed", sSpeed);
    Particle.variable("pumpWattage", sWattage);
    Particle.variable("waterFlow", sFlow);
    Particle.variable("solarAct", sSolar);
    Particle.variable("override", sOverride);
    Particle.variable("tempSolar", sTempSolar);
    Particle.variable("tempPool", sTempPool);
    Particle.variable("tempRoof", sTempRoof);
    for (int x = 3; x >= 0; x--){                   // Give it x seconds to stabilize the RTC and get a time from NTP
        oled.clearDisplay();                       // Clear Screen
        oled.drawRect(0,0,128,64, WHITE);
        oled.drawRect(1,1,127,63, WHITE);
        oled.setTextColor(WHITE);
        oled.setCursor(5,5);
        oled.setTextSize(1);
        oled.println("Pool-Controller v2.1");
        oled.print("    by Jerad Jacob");
        oled.setCursor(30,50);
        oled.print("Startup: ");
        oled.print(x);
        oled.display();
        delay(1000);
    }
    
    Time.setDSTOffset(1);
    Time.zone(timeZone);
    
    onceADay();
    getTimes();	        // Set up initial times
    returnToSchedule(); // Find what the current speed should be
}

void loop() {
    // True once every day
    if (lastDay != Time.day()) {
        onceADay();
        lastDay = Time.day();
    }
    checkIllum();
    getTimes();

    //Check for expired manual Override
    if( manualOverride == 1 ) {
        if (overrideEnds <= currentEpochTime) {
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
    
    getTemps();
  
    //Keep track of energy consumption and speed
    trackData();
}

//Finds the current time each loop
void getTimes(){
    //Gets Time-zone adjusted Epoch Time from the cloud, and converted HHMM comparison time
    currentEpochTime = Time.now();
    currentTime = convertTime(currentEpochTime);
}

void onceADay(){
    //Manual DST logic
    if( Time.month() < 3 || (Time.month() == 3 && Time.day() <=13) || (Time.month() == 11 && Time.day() >=6) || Time.month() > 11 ){
        Time.endDST();
    }else{
        Time.beginDST();
    }
    
}

uint16_t convertTime(uint32_t testTime){
    //Converts Epoch to HHMM
    uint16_t returnTime;
    returnTime = Time.hour(testTime) * 100 + Time.minute(testTime);
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
    char strBuffer[63] = ""; //Max length of the Function String is 63 characters
    command.toCharArray(strBuffer, 63);
    overrideSpeed = atoi(strtok(strBuffer, "~"));
    overrideLength = atoi(strtok(NULL, "~"));
    if( overrideSpeed <= 8 && overrideSpeed >= 1 ){ //These are direct speeds
        manualOverride = 1;
        overrideStarted = currentEpochTime;
        if( overrideLength == 0 ){ //Use default override length
            overrideEnds = currentEpochTime + defaultOverride;
        }else{
            overrideEnds = currentEpochTime + overrideLength;
        }
        setPumpSpeed();
    } else if( overrideSpeed == 9 ){ //9 is a "return to schedule"
        manualOverride = 0;
        returnToSchedule();
    } else { //10 or any other value is a poll for current speed, no changes
        //Just a poll, no changes
    }
    // DEBUG
    String sSpeedRecieved = String(overrideSpeed);
    String sTimeRecieved = String(overrideLength);
    String sOverride = String(manualOverride);
    Particle.publish("Speed_Recieved", sSpeedRecieved + "~" + sTimeRecieved);
    // END DEBUG
    overrideLength = 0; //Reset overrideLength
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

// Prevent OLED burn-in by turning off the screen when
// the door is shut.
void checkIllum(){
    uint8_t currentIllum = analogRead(pIllum);
    if (currentIllum > dispMinBrightness) {
        isBright = 1;
    } else {
        isBright = 0;
    }
}

void getTemps(){
    float poolTempF,solarTempF,roofTempF;
    
    uint16_t solarReadRaw = analogRead(pTempSolar);
    uint16_t poolReadRaw = analogRead(pTempPool);
    uint16_t roofReadRaw = analogRead(pTempRoof);
    
    poolTempF = thermistor(poolReadRaw);
    solarTempF = thermistor(solarReadRaw);
    roofTempF = thermistor(roofReadRaw);
}

float thermistor(int rawADC) {
    // Modified from http://playground.arduino.cc/ComponentLib/Thermistor2
    // Utilizes the Steinhart-Hart Thermistor Equation:
    // Temperature in Kelvin = 1 / {A + B[ln(R)] + C[ln(R)]3}
    // where A = 0.001129148, B = 0.000234125 and C = 8.76741E-08
    
    float vcc = 4.91;                       // only used for display purposes, if used
                                            // set to the measured Vcc.
    float pad = 9850;                       // balance/pad resistor value, set this to
                                            // the measured resistance of your pad resistor
    float thermr = 10000;                   // thermistor nominal resistance
    
    long Resistance;  
    float Temp;  // Dual-Purpose variable to save space.
    Resistance = pad * ((1024.0 / rawADC) - 1); 
    Temp = log(Resistance); // Saving the Log(resistance) so not to calculate  it 4 times later
    Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
    Temp = Temp - 273.15;  // Convert Kelvin to Celsius                      
    Temp = (Temp * 9.0)/ 5.0 + 32.0;  // Convert Celsius to Fahrenheit
    return Temp; // Return the Temperature
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
        sSpeed = String(speedRPM[currentSpeed - 1]);
        sWattage = String(energyConsum[currentSpeed - 1]);
        sFlow = String(flowCalc[currentSpeed - 1]);
        sSolar = String(autoOverride);
        sOverride = String(manualOverride);
        sTempSolar = String(poolTempF);
        sTempPool = String(poolTempF);
        sTempRoof = String(poolTempF);
        Particle.publish("poolLog", "{ \"1\": \"" + sSpeed + "\", \"2\": \"" + sWattage + "\", \"3\": \"" + sFlow + "\", \"4\": \"" + sSolar + "\" }", PRIVATE);
        previousDataPublish = currentEpochTime;
    }
}

void updateDisplay(){ //128x64
    oled.clearDisplay();
    if (isBright == 1) { //Only display if light sensor activates it
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
            oled.println(overrideEnds - currentEpochTime);
        }
        if( autoOverride ) oled.println("AUTO OVERRIDE");
    }
    oled.display();
}
