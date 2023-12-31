#include <Adafruit_NeoPixel.h>

#define LED_PIN 5   //Arduino pin for you stirip data line.
#define LED_COUNT 287  //Total LED count
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

//------------for user------------

int brightnessLevel = 25; // Default brightness
const int NUM_MODES = 16 + 1; // The total number of modes(total cases + 1)


//-----------variables------------

int Mode = 0; 
unsigned long currentMillis;  

bool button1 = HIGH;
bool button2 = HIGH;

unsigned long button1ReleaseTime = 0;
unsigned long button2ReleaseTime = 0;

static const int buttonPin1 = 8;    //The first button. Used for changing the mode forward
static const int buttonPin2 = 9;    //The second button. Used for changing the mode backward
static const int buttonPin3 = 7;    //The third button. Used as "Ctrl". If being held + used one of the first two buttons - the brightness changes instead of modes!

bool buttonState1 = false;    
bool buttonState2 = false;    

const int debounceB = 16;        
unsigned long previousButtonMillis;   

//-----------------GENERAL------------------

void setup() {
  strip.begin();
  strip.show();
  strip.setBrightness(brightnessLevel);

  //Serial.begin(9600);

  pinMode(buttonPin1, INPUT_PULLUP); 
  pinMode(buttonPin2, INPUT_PULLUP); 
  pinMode(buttonPin3, INPUT_PULLUP); 
}

void loop() {
  unsigned long currentTime = millis();

  checkButtonState();
  if (buttonState1 || buttonState2) {
    changeMode();
  }

  displayMode(Mode);

  currentMillis = millis();   
  readButtonState();           
}

void readButtonState() {
  if (currentMillis - previousButtonMillis > debounceB) {
    int buttonState1 = digitalRead(buttonPin1);
    int buttonState2 = digitalRead(buttonPin2);
    int button3State = digitalRead(buttonPin3); 

    if (button3State == HIGH) {
      if (buttonState1 == HIGH) {
        brightnessLevel = brightnessLevel + 1;
        if (brightnessLevel > 90) {
          brightnessLevel = 90;
        }
      }

      if (buttonState2 == HIGH) {
        brightnessLevel = brightnessLevel - 1;
        if (brightnessLevel < 3) {
          brightnessLevel = 3;
        }
      }

      strip.setBrightness(brightnessLevel);
    }
  }
}

void checkButtonState() {
  bool newButton1State = digitalRead(buttonPin2); 
  bool newButton2State = digitalRead(buttonPin1);
  int button3State = digitalRead(buttonPin3); 
//Serial.println(button3State);
  if (button3State == LOW) {
  if (newButton1State != button1) {
    if (newButton1State == LOW) {
      button1ReleaseTime = millis();
    } else if (millis() - button1ReleaseTime > debounceB) { 
      buttonState1 = true;
    }
  }

  if (newButton2State != button2) {
    if (newButton2State == LOW) {
      button2ReleaseTime = millis();
    } else if (millis() - button2ReleaseTime > debounceB) { 
      buttonState2 = true;
    }
  }

  button1 = newButton1State;
  button2 = newButton2State;
  }
}

void changeMode() {
  int button3State = digitalRead(buttonPin3);

  if (button3State == LOW) {
    if (buttonState1) {
      Mode = (Mode + 1) % NUM_MODES; // Use NUM_MODES instead of 4
      buttonState1 = false;
    }

    if (buttonState2) {
      // To ensure positive result while moving backward from the first mode
      Mode = (Mode + NUM_MODES - 1) % NUM_MODES;
      buttonState2 = false;
    }
  }
}


//-----------------ACTIVE MODES------------------

uint32_t colors[][3] = { //I'm using this to have same effect with different colors
  {255, 30, 0},   
  {70, 0, 255},
  {0, 102, 204},
  {150, 50, 255},
  {230, 250, 55},
  {0, 100, 50},
  // Add more colors for additional modes if needed. You'll have to list them in the displayMode() ass well.
};

uint32_t waves[][8] = { //I'm using this to have same effect with different colors
  {2, 5000, 0, 20, 20, 0, 200, 40},   
  {3, 6000, 212, 20, 20, 250, 176, 60},   
  {4, 6000, 0, 146, 69, 250, 0, 30},   
  {4, 6000, 100, 45, 140, 240, 10, 210},   
  {2, 6000, 198, 234, 141, 150, 250, 150},   
  {4, 5000, 216, 181, 255, 30, 70, 152},   
  {2, 6000, 195, 55, 60, 30, 40, 110},   
  {7, 5000, 205, 95, 109, 255, 195, 113},   
  {3, 7000, 0, 199, 27, 94, 8, 200},   
  // Add more waves for additional modes if needed. You'll have to list them in the displayMode() ass well.
};


void displayMode(int mode) { //list your modes here
  //Serial.println(mode);
  switch (mode) {
    case 0: 
      fillSolidColor(strip.Color(colors[0][0], colors[0][1], colors[0][2])); 
      break;
    case 1: 
      fillSolidColor(strip.Color(colors[1][0], colors[1][1], colors[1][2])); 
      break;
    case 2: 
      fillSolidColor(strip.Color(colors[2][0], colors[2][1], colors[2][2])); 
      break;
    case 3: 
      fillSolidColor(strip.Color(colors[3][0], colors[3][1], colors[3][2])); 
      break;
    case 4: 
      fillSolidColor(strip.Color(colors[4][0], colors[4][1], colors[4][2])); 
      break;
    case 5: 
      fillSolidColor(strip.Color(colors[5][0], colors[5][1], colors[5][2])); 
      break;
    case 6: 
      threeColorSmoothMovingWaves(waves[0][0], waves[0][1], waves[0][2], waves[0][3], waves[0][4], waves[0][5], waves[0][6], waves[0][7]);
      break;
    case 7: 
      threeColorSmoothMovingWaves(waves[1][0], waves[1][1], waves[1][2], waves[1][3], waves[1][4], waves[1][5], waves[1][6], waves[1][7]);
      break;
    case 8: 
      threeColorSmoothMovingWaves(waves[2][0], waves[2][1], waves[2][2], waves[2][3], waves[2][4], waves[2][5], waves[2][6], waves[2][7]);
      break;
    case 9: 
      threeColorSmoothMovingWaves(waves[3][0], waves[3][1], waves[3][2], waves[3][3], waves[3][4], waves[3][5], waves[3][6], waves[3][7]);
      break;
    case 10: 
      threeColorSmoothMovingWaves(waves[4][0], waves[4][1], waves[4][2], waves[4][3], waves[4][4], waves[4][5], waves[4][6], waves[4][7]);
      break;
    case 11: 
      threeColorSmoothMovingWaves(waves[5][0], waves[5][1], waves[5][2], waves[5][3], waves[5][4], waves[5][5], waves[5][6], waves[5][7]);
      break;
    case 12: 
      threeColorSmoothMovingWaves(waves[6][0], waves[6][1], waves[6][2], waves[6][3], waves[6][4], waves[6][5], waves[6][6], waves[6][7]);
      break;
    case 13: 
      threeColorSmoothMovingWaves(waves[7][0], waves[7][1], waves[7][2], waves[7][3], waves[7][4], waves[7][5], waves[7][6], waves[7][7]);
      break;
    case 14: 
      threeColorSmoothMovingWaves(waves[8][0], waves[8][1], waves[8][2], waves[8][3], waves[8][4], waves[8][5], waves[8][6], waves[8][7]);
      break;
    case 15: 
      smoothColorChanging(3000, 26);
      break;
    case 16: 
      rainbowCycle(20, 0, 250, 250);
      break;
    
  }
}


//-----------------MODES------------------
//you can add more modes here




void fillSolidColor(uint32_t color) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}




void randomMode() {
  for (int i = 0; i < strip.numPixels(); i++) {
    uint32_t color = strip.Color(random(256), random(256), random(256)); 
    strip.setPixelColor(i, color);
  }
  strip.show();
}




void threeColorSmoothMovingWaves(int waves, float waveSpeed, int redIntensity1, int greenIntensity1, int blueIntensity1, int redIntensity2, int greenIntensity2, int blueIntensity2) {
  unsigned long currentMillis = millis(); 

  for (int i = 0; i < strip.numPixels(); i++) {
    float ratio = float(i) / float(strip.numPixels() - 1);
    float phaseShift = float(currentMillis) / waveSpeed;
    float wave = 0.5 + 0.5 * sin((ratio * waves + phaseShift) * 2 * PI);

    uint32_t currentColor = strip.Color(
      redIntensity1 * wave + redIntensity2 * (1 - wave),
      greenIntensity1 * wave + greenIntensity2 * (1 - wave),
      blueIntensity1 * wave + blueIntensity2 * (1 - wave)
    );

    strip.setPixelColor(i, currentColor);
  }
  strip.show();
}




void rainbowCycle(int transitionDelay, int red, int green, int blue) {
  static int startColor = 0;
  
  for (int i = 0; i < LED_COUNT; i++) {
    int color = wheel(((i * 256 / LED_COUNT) + startColor) & 255, red, green, blue);
    strip.setPixelColor(i, color);
  }
  
  strip.show();
  delay(transitionDelay);
  startColor += 1; // Rate of color change
}
uint32_t wheel(byte WheelPos, int red, int green, int blue) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color((255 - WheelPos * 3) * red / 255, 0, (WheelPos * 3) * blue / 255);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color((255 - WheelPos * 3) * red / 255, (WheelPos * 3) * green / 255, 0);
  } else {
    WheelPos -= 170;
    return strip.Color(0, (255 - WheelPos * 3) * green / 255, (WheelPos * 3) * blue / 255);
  }
}




void smoothColorChanging(int animationLength, int animationSpeed) {
  static uint16_t colorIndex = 0;
  static unsigned long previousMillis = 0;
  static unsigned long animationDuration = 0;

  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= animationSpeed) {
    previousMillis = currentMillis;

    for (int i = 0; i < LED_COUNT; i++) {
      int red = (sin((i + colorIndex) * 0.03) * 127 + 128);
      int green = (sin((i + colorIndex) * 0.03 + 2) * 127 + 128);
      int blue = (sin((i + colorIndex) * 0.03 + 4) * 127 + 128);
      strip.setPixelColor(i, strip.Color(red, green, blue));
    }

    strip.show();
    colorIndex++;

    animationDuration = currentMillis - previousMillis;
    if (animationDuration >= animationLength) {
      // Reset variables after completing the animation
      colorIndex = 0;
      previousMillis = 0;
      animationDuration = 0;
    }
  }
}