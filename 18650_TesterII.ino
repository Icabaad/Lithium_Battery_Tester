
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, 2, NEO_GRB + NEO_KHZ800);

#include <LiquidCrystal_I2C.h>      // using the LCD I2C Library from https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
//LiquidCrystal lcd(12, 11, 3, 4, 5, 6); Current LCD
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address //Test LCD

const int relayPin[] = {
  0, 1, 2, 3, 4, 5, 6, 7
};

int analogueRead[] = {
  0, 1, 2, 3, 4, 5, 6, 7
};

// Change the values directly below to suit your discharge resistor
int slowResistor = 4.7; // a 15 ohm resistor would be optimal
int slowMaxWatts = 5; // if you exceed this value you will fry your resistor   //Need to add code and circuit to break circuit if this is exceeded
float powerV = 4.87; // If readings are off check arduino supply voltage and adjust here
float voltageCutoff = 2.5; // if the LiPo battery does not have internal cutoff security this will stop the test before damaging battery.
int rateSwitch = 0; //?? Cant remember what this is for?
// Thats it!

//int analogueRead;
int capacitymAh0, capacitymAh1, capacitymAh2, capacitymAh3, capacitymAh4, capacitymAh5, capacitymAh6, capacitymAh7;
int time0, time1, time2, time3, time4, time5, time6, time7;

//float batV0, batV1, batV2, batV3, batV4, batV5, batV6, batV7;
float batV[] = {
  0, 0, 0, 0, 0, 0, 0, 0,
};

float shuntV0, shuntV1, shuntV2, shuntV3, shuntV4, shuntV5, shuntV6, shuntV7;


void setup() {
  //Neopixil Strip
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  //LCD
  lcd.begin(20, 4);  //test

  //Set Relays to charge Position by default
  for (int thisPin = 0; thisPin < 8; thisPin++) {
    pinMode(relayPin[thisPin], OUTPUT);
    digitalWrite(relayPin[thisPin], LOW);
  }
    //Serial start
    Serial.begin(9600);


    Serial.println("18650 Battery Rolla 27/12/2017");
  };

  void loop() {
    
    for (int in = 0; in < 8; in++) {
      analogueRead[in] = analogRead[in];
      batV[in] = (analogueRead[in] * powerV) / 1024; //calculates Voltage from Analogue readings

      //Below Prints A1(Battery 1 reading A) B1 (Battery 1 reading B) and increments to the next Battarah
      Serial.print(" A"), Serial.print((in + 1)), Serial.print(":"), Serial.print(analogueRead[in]);
      Serial.print(" B"), Serial.print((in + 1)), Serial.print(":"), Serial.print(batV[in]);
      Serial.println();

      int val = map(analogueRead[in], 0, 1023, 0, 255); //maps RGB colour for Neo Pixel below

      if (batV[in] >= 4.00) {
        strip.setPixelColor(in, 0, val, 0);
        //      strip.show();
      }
      if (batV[in] >= 3.00 && batV[in] <= 4.00) {
        strip.setPixelColor(in, 0, 0, val);
        //     strip.show();
      }
      if (batV[in] < 3.00) {
        strip.setPixelColor(in, val, 0, 0);
        //      strip.show();
      }
      strip.show();
      if (in == 7) {
        Serial.println();
      }
    }
  }
