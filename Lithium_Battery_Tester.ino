
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, 2, NEO_GRB + NEO_KHZ800);

#include <LiquidCrystal_I2C.h>      // using the LCD I2C Library from https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
//LiquidCrystal lcd(12, 11, 3, 4, 5, 6); Current LCD
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address //Test LCD

//const float arduino5V = 5; //take voltage reading of arduino 5v output and change here
//const float shuntResistorOhms = 4.7; //Resistance of Shunt resistors

const int relayPin[] = {
  0, 1, 2, 3, 4, 5, 6, 7
};

int batteryRead[] = {
  0, 0, 0, 0, 0, 0, 0, 0
};

int shuntRead[] = {
  0, 0, 0, 0, 0, 0, 0, 0
};

int chargeState[] = {
  0, 0, 0, 0, 0, 0, 0, 0
};


int watts[] = {
  0, 0, 0, 0, 0, 0, 0, 0
};

// Change the values directly below to suit your discharge resistor
float slowResistor = 4.7; // a 15 ohm resistor would be optimal
int slowMaxWatts = 5; // if you exceed this value you will fry your resistor   //Need to add code and circuit to break circuit if this is exceeded
float powerV = 4.87; // If readings are off check arduino supply voltage and adjust here
float voltageCutoff = 2.5; // if the LiPo battery does not have internal cutoff security this will stop the test before damaging battery.
int rateSwitch = 0; //?? Cant remember what this is for?
// Thats it!

//int analogueRead;
//int capacitymAh0, capacitymAh1, capacitymAh2, capacitymAh3, capacitymAh4, capacitymAh5, capacitymAh6, capacitymAh7;
float capacitymAH[] = {
  0, 1, 2, 3, 4, 5, 6, 7
};

int time0, time1, time2, time3, time4, time5, time6, time7;

//float batV0, batV1, batV2, batV3, batV4, batV5, batV6, batV7; //replaced with below array
float batV[] = {
  0, 0, 0, 0, 0, 0, 0, 0
};

//float shuntV0, shuntV1, shuntV2, shuntV3, shuntV4, shuntV5, shuntV6, shuntV7; //replaced with below array
float shuntV[] = {
  0, 0, 0, 0, 0, 0, 0, 0
};


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
    chargeState[thisPin] = 0;
  }
  //Serial start
  Serial.begin(9600);


  Serial.println("18650 Battery Rolla 27/12/2017");
};

void loop() {

  for (int in = 0; in < 8; in++) {
    //Serial.print("Battery Read Before:"), Serial.println(batteryRead[in]);
    batteryRead[in] = analogRead(in);
    batV[in] = (batteryRead[in] * powerV) / 1024; //calculates Voltage from Analogue readings
    //Serial.print("Battery Read After:"), Serial.println(batteryRead[in]);
    shuntV[in] = (shuntRead[in] * powerV) / 1024; //calculates Voltage from Analogue readings
    //Below Prints A1(Battery 1 reading A) B1 (Battery 1 reading B) and increments to the next Battarah
    Serial.print(" A"), Serial.print((in + 1)), Serial.print(":"), Serial.print(batteryRead[in]);
    Serial.print(" B"), Serial.print((in + 1)), Serial.print(":"), Serial.println(batV[in]);

    Serial.print(" A"), Serial.print((in + 1)), Serial.print(":"), Serial.print(shuntRead[in]);
    Serial.print(" B"), Serial.print((in + 1)), Serial.print(":"), Serial.println(shuntV[in]);
    Serial.println();

    if (batV[in] > 4.19 & chargeState[in] == 0) { //Battery Charged? begin discharge phase
      chargeState[in] = 1;

      digitalWrite(relayPin[in] + 30, HIGH);
      Serial.print("Battery "), Serial.print((in+1)), Serial.println(" Charged!");
      Serial.print("Battery "), Serial.print((in+1)), Serial.print(" Voltage: "), Serial.println(batV[in]);
    }

    else if (batV[in] <= 2.5 & chargeState[in] == 1) {
      digitalWrite(relayPin[in] + 30, LOW);
      Serial.print("Battery "), Serial.print((in+1)), Serial.println("Discharged!");
      Serial.print("Battery "), Serial.print((in+1)), Serial.print("Voltage: "), Serial.println(batV[in]);
    }

    if (chargeState[in] == 1) {
      capacitymAH[in] = capacitymAH[in] + (shuntV[in] / slowResistor) / 3600;
      watts[in] = shuntV[in] / slowResistor;
      capacitymAH[in] = capacitymAH[in] * 1000;
      Serial.print("Battery "), Serial.print((in+1)), Serial.print("Capacity: "), Serial.println(capacitymAH[in]);
      Serial.print("Discharge Wattage "), Serial.print((in+1)), Serial.print(": "), Serial.println(watts[in]);
    }


    int val = map(batteryRead[in], 0, 1023, 0, 255); //maps RGB colour for Neo Pixel below

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

  delay(2000);
}
