// Flame Wave Effect with 2 Reds, Yellow, and Warm White (Reduced Intensity)

int whitePin = 3;   // Bottom (Warm White)
int yellowPin = 5;  // Middle
int redPin = 6;     // Top

// Flashing LED strip
int REDflashPin = 12;
int GreenFlashPin = 13;

int switchPin = 2;    // Switch pin

bool isGateOpen = false; //Gate status
bool isRedLedFlash = false;
bool isGreenLedFlash = false;

int pirPin = 7;    // Switch pin
int motionLight = 8;    // Motion detect pin



float brightnessFactor = 0.2; // 🔅 Adjust 0.1–1.0 for dim to bright
int speed = 10;                // 🔁 Wave speed (lower = slower flame)

unsigned long startTime;               // Time tracking
unsigned long runDuration = 5UL * 60UL * 1000UL; // 5 minutes (ms)


/// Timing variables
unsigned long lastFlashCycle = 0;        // Tracks start of 10-second cycle
const unsigned long flashCycleInterval = 90000; // 10 seconds
const unsigned long flashOnTime = 200;         // LED ON duration (ms)
const unsigned long flashOffTime = 200;        // LED OFF duration (ms)
int flashCount = 0;
bool flashState = false;                  // true = LED ON, false = LED OFF
unsigned long lastFlashChange = 0;

unsigned long motionDetectedTime = 0;
unsigned long delayTime = 15000; // 15 seconds

void setup() {
  pinMode(whitePin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  randomSeed(analogRead(0)); // Seed for randomness
  startTime = millis(); // remember start time
  pinMode(REDflashPin, OUTPUT);
  pinMode(GreenFlashPin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP); // use internal pull-up resistor
  pinMode(pirPin, INPUT); // PIR input pin
  pinMode(motionLight, OUTPUT); // Motion light pin
}

void fireFormat1 () {
  for (int wave = 0; wave < 255; wave += random(3, 8)) {
    // Flame flicker from bottom (white) to top (red)
    int whiteVal  = constrain(160 + sinWave(wave, 0)   * 70 + random(-20, 15), 0, 255);
    int yellowVal = constrain(140 + sinWave(wave, 60)  * 100 + random(-25, 25), 0, 255);
    int redVal    = constrain(100 + sinWave(wave, 120) * 90 + random(-30, 25), 0, 255);

    // 🔅 Apply brightness scaling
    whiteVal  *= brightnessFactor;
    yellowVal *= brightnessFactor;
    redVal    *= brightnessFactor;

    // Output brightness to LEDs
    analogWrite(whitePin, whiteVal);
    analogWrite(yellowPin, yellowVal);
    analogWrite(redPin, redVal);

    delay(random(20, 50)); // Flicker speed
  }
}

void fireFormat2 () {
  // 🔅 Apply brightness scaling
  int whiteVal  =  255 * brightnessFactor;
  int yellowVal = 255 * brightnessFactor;
  int redVal    = 255 * brightnessFactor;

  analogWrite(whitePin, whiteVal);
  delay(200);
  analogWrite(yellowPin, yellowVal);
  delay(200);
  analogWrite(redPin, redVal);
  delay(200);
  analogWrite(redPin, 0);
  delay(200);
  analogWrite(redPin, redVal);

  delay(200);
  analogWrite(redPin, 0);
  delay(200);
  analogWrite(yellowPin, 0);
  delay(200);
  analogWrite(whitePin, 0);
}

void fireFormat3 () {
  // 🔅 Apply brightness scaling
  int whiteVal  =  255 * brightnessFactor;
  int yellowVal = 255 * brightnessFactor;
  int redVal    = 255 * brightnessFactor;

  analogWrite(whitePin, whiteVal);
  delay(100);
  analogWrite(yellowPin, yellowVal);
  delay(100);
  analogWrite(redPin, redVal);
  
  delay(100);
  analogWrite(whitePin, 0);
  delay(100);
  analogWrite(yellowPin, 0);
  delay(100);
  analogWrite(redPin, 0); 
  delay(100);
}

void fireFormat4 () {
  for (int wave = 0; wave < 255; wave += 4) {
  // Create rising wave (bottom leads, top lags)
  float base = wave * 3.14159 / 128.0;

  // Flame intensity per section (offset phases)
  int whiteVal  = constrain(150 + (sin(base) * 105) + random(-10, 10), 0, 255);
  int yellowVal = constrain(120 + (sin(base - 0.8) * 130) + random(-15, 15), 0, 255);
  int redVal    = constrain(90  + (sin(base - 1.5) * 150) + random(-20, 20), 0, 255);

  // Apply overall brightness
  whiteVal  *= brightnessFactor;
  yellowVal *= brightnessFactor;
  redVal    *= brightnessFactor;

  // Output PWM to each LED
  analogWrite(whitePin, whiteVal);
  analogWrite(yellowPin, yellowVal);
  analogWrite(redPin, redVal);

  delay(speed);
  }
}

void fireFormat5 () {
  for (int i = 0; i < 255; i += 4) {
    float base = sin(i * 3.14159 / 128.0);
    int whiteVal  = constrain(150 + base * 80 + random(-15, 15), 0, 255);
    int yellowVal = constrain(120 + base * 100 + random(-20, 20), 0, 255);
    int redVal    = constrain(90  + base * 110 + random(-25, 25), 0, 255);

    // Apply overall brightness
    whiteVal  *= brightnessFactor;
    yellowVal *= brightnessFactor;
    redVal    *= brightnessFactor;

    analogWrite(whitePin, whiteVal);
    analogWrite(yellowPin, yellowVal);
    analogWrite(redPin, redVal);

    delay(random(20, 50));
  }
}

void fireFormat6 () {
    int whiteVal  = 0;
    int yellowVal = 255;
    int redVal    = 255;

    // Apply overall brightness
    whiteVal  *= brightnessFactor;
    yellowVal *= brightnessFactor;
    redVal    *= brightnessFactor;

    analogWrite(whitePin, whiteVal);

    analogWrite(yellowPin, yellowVal);
    delay(100);
    analogWrite(redPin, redVal);
    delay(100);
    analogWrite(redPin, 0);
    delay(random(10, 200));
    analogWrite(redPin, redVal);
    delay(100);

    delay(random(20, 50));
}

void fireFullOff () {
  analogWrite(whitePin, 0);
  analogWrite(yellowPin, 0);
  analogWrite(redPin, 0);
}

void fireFullOn () {
  analogWrite(whitePin, 255);
  analogWrite(yellowPin, 255);
  analogWrite(redPin, 255);
}

void blinkRED () {
  while(flashCount < 5) {
    digitalWrite(REDflashPin, HIGH);
    delay(200);
    digitalWrite(REDflashPin, LOW);
    delay(200);
    flashCount++;
  }
  isRedLedFlash = true;
  isGreenLedFlash = false;
  digitalWrite(GreenFlashPin, HIGH);
}

void blinkGREEN () {
  while(flashCount < 5) {
    digitalWrite(GreenFlashPin, HIGH);
    delay(200);
    digitalWrite(GreenFlashPin, LOW);
    delay(200);
    flashCount++;
  }
  isGreenLedFlash = true;
  isRedLedFlash = false;
  digitalWrite(REDflashPin, HIGH);
}

void playFire () {
  if(random(1,10) == 2) {
    fireFormat4();
    fireFormat2();
  } else if(random(1,10) == 4) {
    fireFormat3();
  } else if(random(1,3) == 2) {
    fireFormat1();
  } else if(random(1,3) == 2) {
    fireFormat2();
  } else if(random(1,4) == 2) {
    fireFormat5();
  } else if(random(1,6) == 2) {
    fireFormat6();
  }
}


void loop() {


  int motion = digitalRead(pirPin);
  if (motion == HIGH) {
    digitalWrite(motionLight, HIGH);  // Turn on LED
    motionDetectedTime = millis(); // Save current time
  }

  // Keep LED ON for delayTime after last motion
  if (millis() - motionDetectedTime > delayTime) {
    digitalWrite(motionLight, LOW);  // Turn off LED
  }

  // delay(500);


  // return;

  isGateOpen = digitalRead(switchPin) != LOW;
  if (isGateOpen && !isRedLedFlash) {
    fireFullOff();
    flashCount = 0;
    blinkRED();
  } else if (!isGateOpen && !isGreenLedFlash) {
    fireFullOff();
    flashCount = 0;
    blinkGREEN();
  } else {
    playFire();
  }





//   if(random(1,2) == 2) {
//     fireFullOff(); 
// blinkRED();
//   } else {
// playFire();
//   }

  // return;


// playFire();
  

  // fireFullOff();
  // fireFullOn();


  // return;

}

float sinWave(int x, int phase) {
  return sin((x + phase) * 3.14159 / 128.0);
}
