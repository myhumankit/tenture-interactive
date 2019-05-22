/*Ruban de leds RVB type neopixel WS2812

   PESIS AES 2018-2019, Askoria, mars 2019
   arduino 1.8.8 @ ubuntu 18 LTS, MATE Desktop Environment 1.12.1
   + lib. Adafruit Neopixel 1.1.3
   + lib teensy audio shield
   + Teensyduino 1.45

  Code : Delphine Bézier
  Licence : CC BY SA

*/

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav1;     //xy=154,129
AudioPlaySdWav           playSdWav2; //xy=157,198
AudioMixer4              mixer1;         //xy=429,245
AudioOutputI2S           i2s1;           //xy=626,130
AudioConnection          patchCord1(playSdWav1, 1, mixer1, 1);
AudioConnection          patchCord2(playSdWav2, 1, mixer1, 3);
AudioConnection          patchCord3(mixer1, 0, i2s1, 0);
AudioConnection          patchCord4(mixer1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=617,217
// GUItool: end automatically generated code


// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14
////////////////////////////////////////
#include <Adafruit_NeoPixel.h>
#include <OneButton.h>

#define BROCHE_LED      2
#define BROCHE_BOUTON   8
#define NUMPIXELS       35

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, BROCHE_LED, NEO_GRB + NEO_KHZ800);

byte mode = 0;
int compteur = 0;//nb d'étapes sur la tenture (5)



void setup() {
  Serial.begin(38400);
  ////////play sd card_teensy audio shield

  AudioMemory(8);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
  delay(1000);

  //////////////
  pixels.begin();
  pixels.clear();

  pinMode(BROCHE_BOUTON, INPUT_PULLUP);

}

void loop() {

  if (digitalRead(BROCHE_BOUTON) == 0) {
    mode1();

  }
  pixels.setBrightness(60);


  if ((compteur == 4) && (playSdWav1.positionMillis() == playSdWav1.lengthMillis() - 1)) {
    compteur = 5;

  }
  if ((compteur == 5) && (playSdWav2.positionMillis() >= playSdWav2.lengthMillis() - 10)) {
    Serial.println("compteur = 6 ");
    compteur = 6;

  }

  switch (compteur) {
    case 0:
      switch_off_all();
      break;

    case 1:
      for (int i = 0; i < 2; i++) {
        pixels.setPixelColor(i, pixels.Color(255, 0, 0) );

      }

      snake();

      break;
    case 2:
      // switch_off();
      for (int i = 2; i < 4; i++) {
        pixels.setPixelColor(i, pixels.Color(255, 255, 255) );
      }
      snake();
      break;

    case 3:
      // switch_off();
      for (int i = 4; i < 6; i++) {
        pixels.setPixelColor(i, pixels.Color(255, 0, 0) );
      }
      snake();
      break;

    case 4:
      switch_off();
      for (int i = 6; i < 10; i++) {
        pixels.setPixelColor(i, pixels.Color(255, 255, 255) );
      }
      // snake();
      if (playSdWav1.isPlaying() == false) {
        // Serial.println("Start playing");
        playSdWav1.play("OURSBRUN.WAV");
     

        delay(10); // wait for library to parse WAV info
       // Serial.print("lengthmillis wave1 :");
       // Serial.println(playSdWav1.lengthMillis());
       // Serial.println(playSdWav1.positionMillis());
      }
      break;

    case 5:
      playSdWav1.stop();
      switch_off();
      switch_off_final();
      for (int i = 11; i < 18; i++) {
        pixels.setPixelColor(i, pixels.Color(255, 255, 255) );
      }

      if (playSdWav2.isPlaying() == false) {
        playSdWav2.play("SOMMEIL2.WAV");
       delay(10); //
      }
      break;

    case 6:
      playSdWav1.stop();
      playSdWav2.stop();
      switch_off_all();
     // Serial.println("compteur:");
      //Serial.println(compteur);
      break;
      pixels.show();

  }
}
void mode1() {
  Serial.println("mode1");
  compteur = compteur + 1;
  if (compteur > 6) {
    compteur = 0;
  }
 // Serial.println("compteur:");
 // Serial.println(compteur);
  delay(500);
}

//éteindre la route
void switch_off() {
  for (int i = 18; i < 35; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0) );
  }
  pixels.show();
}

//éteindre les 4 premières étapes (dents, WC, calins, histoire)
void switch_off_final() {
  switch_off();
  for (int i = 0; i < 11; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0) );
  }
  pixels.show();
}

//éteindre toutes les leds
void switch_off_all() {
  for (int i = 0; i < 35; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0) );
  }
  pixels.show();
}

//route qui avance
void snake() {

  //pour chaque led numéro 18 à 34
  for (int i = 18; i < 35; i++) {

    //si le bouton est appuyé (mode pullup), éteindre la route
    if (digitalRead(BROCHE_BOUTON) == 0) {
      switch_off();
      break;

    }

    //si l'itération dans la boucle n'est pas sur la première led de la route
    if (i != 18) {

      //éteindre la led précédente
      pixels.setPixelColor(i - 1, pixels.Color(0, 0, 0) );
    }
    //sinon
    else {

      //éteindre la dernière led de la route
      pixels.setPixelColor(NUMPIXELS - 1, pixels.Color(0, 0, 0) );
    }

    //puis allumer la led correspondant au numéro d'itération de la boucle

    pixels.setPixelColor(i, pixels.Color(255, 255, 255) );
    pixels.show();
    //vitesse "d'avancement des leds " de la route, plus le delay est long plus la vitesse est lente
    delay(200);

  }
}
