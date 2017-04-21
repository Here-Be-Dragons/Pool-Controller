#define buttonPin       A1   // the number of the pushbutton pin
#define ledPin          D7   // the number of the LED pin for testing.  D7 is integrated into the photon.

// Array for each button consists of {low,high} values that should
// count as a button press.  If you don't know the values yet, press each
// button and monitor the output of the "resistance" Particle variable
const uint16_t aButton1[2] = {2420,2450};
const uint16_t aButton2[2] = {2780,2810};
const uint16_t aButton3[2] = {3340,3370};
const uint16_t aButton4[2] = {3710,3740};
 

uint8_t buttonState = 0;     // the current reading from the input pin
uint8_t lastButtonState = 0; // the previous reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;   // the last time the output pin was toggled
int debounceDelay = 50;     // the debounce time; increase if the output flickers

String sResistance;
String sButton;

void setup() {
    pinMode(ledPin, OUTPUT);
    Particle.variable("resistance",sResistance);
    Particle.variable("buttonpr",sButton);
}

void loop() {
    // read the state of the switch into a local variable:
    uint16_t reading = analogRead(buttonPin);
    sResistance = String(reading);
    uint8_t tmpButtonState = 0;
    
    if(reading > aButton4[0] && reading < aButton4[1]){
        tmpButtonState = 4;
    }else if(reading > aButton3[0] && reading < aButton3[1]){
        tmpButtonState = 3;
    }else if(reading > aButton2[0] && reading < aButton2[1]){
        tmpButtonState = 2;
    }else if(reading > aButton1[0] && reading < aButton1[1]){
        tmpButtonState = 1;
    }else{
        tmpButtonState = 0; //No button is pressed;
    }
    
    // check to see if you just pressed the button 
    // (i.e. the input went from LOW to a buttonState), and you've waited 
    // long enough since the last press to ignore any noise:  
    
    // If the switch changed (due to noise OR pressing)
    if (tmpButtonState != lastButtonState) {
        // reset the debouncing timer
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        // Assume this state is legitimate, so set it as buttonState
        buttonState = tmpButtonState;
        switch(buttonState){
            case 1:
                digitalWrite(ledPin, HIGH);
                sButton = String("BUTTON 1: Display");
                break;
            case 2:
                digitalWrite(ledPin, HIGH);
                sButton = String("BUTTON 2: Stop");
                break;
            case 3:
                digitalWrite(ledPin, HIGH);
                sButton = String("BUTTON 3: Clean");
                break;
            case 4:
                digitalWrite(ledPin, HIGH);
                sButton = String("BUTTON 4: Resume");
                break;
            default:
                digitalWrite(ledPin, LOW);
                sButton = String("---");
                break;
        }
    }
    
    lastButtonState = tmpButtonState;
}
