byte buzzerpin = 9;
byte previous_played = 0;
byte generating = 0;


// Start Wars Tone 
// NB: ALL NOTES DEFINED WITH STANDARD ENGLISH NAMES, EXCEPT FROM "A" 
//THAT IS CALLED WITH THE ITALIAN NAME "LA" BECAUSE A0,A1...ARE THE ANALOG PINS ON ARDUINO.
// (Ab IS CALLED Ab AND NOT LAb)
#define  C0 16.35
#define Db0 17.32
#define D0  18.35
#define Eb0 19.45
#define E0  20.60
#define F0  21.83
#define Gb0 23.12
#define G0  24.50
#define Ab0 25.96
#define LA0 27.50
#define Bb0 29.14
#define B0  30.87
#define C1  32.70
#define Db1 34.65
#define D1  36.71
#define Eb1 38.89
#define E1  41.20
#define F1  43.65
#define Gb1 46.25
#define G1  49.00
#define Ab1 51.91
#define LA1 55.00
#define Bb1 58.27
#define B1  61.74
#define C2  65.41
#define Db2 69.30
#define D2  73.42
#define Eb2 77.78
#define E2  82.41
#define F2  87.31
#define Gb2 92.50
#define G2  98.00
#define Ab2 103.83
#define LA2 110.00
#define Bb2 116.54
#define B2  123.47
#define C3  130.81
#define Db3 138.59
#define D3  146.83
#define Eb3 155.56
#define E3  164.81
#define F3  174.61
#define Gb3 185.00
#define G3  196.00
#define Ab3 207.65
#define LA3 420.00
#define Bb3 233.08
#define B3  246.94
#define C4  261.63
#define Db4 277.18
#define D4  293.66
#define Eb4 311.13
#define E4  329.63
#define F4  349.23
#define Gb4 369.99
#define G4  392.00
#define Ab4 415.30
#define LA4 440.00
#define Bb4 466.16
#define B4  493.88
#define C5  523.25
#define Db5 554.37
#define D5  587.33
#define Eb5 622.25
#define E5  659.26
#define F5  698.46
#define Gb5 739.99
#define G5  783.99
#define Ab5 830.61
#define LA5 880.00
#define Bb5 932.33
#define B5  987.77
#define C6  1046.50
#define Db6 1108.73
#define D6  1174.66
#define Eb6 1244.51
#define E6  1318.51
#define F6  1396.91
#define Gb6 1479.98
#define G6  1567.98
#define Ab6 1661.22
#define LA6 1760.00
#define Bb6 1864.66
#define B6  1975.53
#define C7  2093.00
#define Db7 2217.46
#define D7  2349.32
#define Eb7 2489.02
#define E7  2637.02
#define F7  2793.83
#define Gb7 2959.96
#define G7  3135.96
#define Ab7 3322.44
#define LA7 3520.01
#define Bb7 3729.31
#define B7  3951.07
#define C8  4186.01
#define Db8 4434.92
#define D8  4698.64
#define Eb8 4978.03
// DURATION OF THE NOTES 
#define BPM 120    //  you can change this value changing all the others
#define H 2*Q //half 2/4
#define Q 60000/BPM //quarter 1/4 
#define E Q/2   //eighth 1/8
#define S Q/4 // sixteenth 1/16
#define W 4*Q // whole 4/4

#include "LowPower.h"

int random10, random11, random12, random14;


byte jumpsensor = A5;
byte battery_sense = A2;
byte battery_charging = 0;
byte get_voltage = A1;
float battery_volts = 0;
byte led_orange = A4;
byte ledpin = A0;

float sense=0;
byte set_complete = true;
byte jump_count = 0;
unsigned long currenttime, previoustime;

int base_freq = 4700; // set and sound profile of your choice

byte ones, tens, hundreds, soundof;
byte sense_active = 1;
byte chirping_done = 1;
void ambulance ();

// chirping
void startup_chirp();
int variation_number = 0; // how many times
byte Numbof_variations;

// to spice things up and add dynamics in sound via varition
int freq_variation [34] = {1500, 1000, 0, 1500, 800, 150, 1905, 1750, 0, 60, 50, 70, 20, 2050, 281, 934, 2198, 2514, 1522, 1866, 51, 51, 51, 1046, 728, 312, 2261, 1755, 1715, 1237, 1583, 1432, 72};

int x = 100; // frequency calculation

// **** sleep
// Use pin 2 as wake up pin
const int wakeUpPin = 2;

void wakeUp()
{
    // Just a handler for the pin interrupt.
}



void setup(){
//Serial.begin(9600);
//Serial.println("starting");

pinMode(wakeUpPin, INPUT);  
pinMode (jumpsensor, INPUT);
pinMode (battery_sense, INPUT);
pinMode (get_voltage, INPUT);

pinMode (ledpin, OUTPUT);
pinMode (buzzerpin, OUTPUT);
pinMode (led_orange, OUTPUT);

// turn on buzzer warning
tone(buzzerpin, 5000);
digitalWrite (led_orange , HIGH);
delay (500); 
noTone(buzzerpin);
digitalWrite (led_orange , LOW);

} // end void setup

void loop(){

currenttime = millis();  // start turn off after X interval timer



//``````````````````````` Idle state turn off timer ````````````````
if (currenttime - previoustime >= 60000)  // greater than a minute
{
Serial.println("Timeout turning off");
previoustime = currenttime;

// turn on buzzer warning
tone(buzzerpin, 1000);
digitalWrite (led_orange , HIGH);
delay (1000); 
noTone(buzzerpin);
digitalWrite (led_orange , LOW);

// ******** The sleeping action STARTS ***************
    delay (20);
   // Allow wake up pin to trigger interrupt on low.
    attachInterrupt(0, wakeUp, LOW);
//    Serial.println("attach interrupt");
    delay (10);
    // Enter power down state with ADC and BOD module disabled. Wake up when wake up pin is low.
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 
    
    // Disable external pin interrupt on wake up pin.
    detachInterrupt(0); 
    delay (100);
tone(buzzerpin, 4000);
digitalWrite (led_orange , HIGH);
delay (1000); // increase this upon finalization
noTone(buzzerpin);
digitalWrite (led_orange , LOW);

    // ******** The sleeping action ENDS *************** 

} // end if idle for x time 

// ************* Low battery detection ********** 
// read volate to make sure battery is not low    
battery_volts = (2 * (analogRead(get_voltage) * (5.0 / 1023.0)));
if (battery_volts <= 1.5) {

// turn on buzzer warning
tone(buzzerpin, 500);
digitalWrite (led_orange , HIGH);
delay (2000);
digitalWrite (led_orange , LOW);
noTone(buzzerpin);

// turn off circuit
delay (300);
   // Allow wake up pin to trigger interrupt on low.
    attachInterrupt(0, wakeUp, LOW);
//    Serial.println("Low battery");
    delay (50);
    // Enter power down state with ADC and BOD module disabled. Wake up when wake up pin is low.
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 
    
    // Disable external pin interrupt on wake up pin.
    detachInterrupt(0); 
    delay (100);
    // ******** The sleeping action ENDS *************** 

} // end if battery voltages are less

  //``````````````````````` Read sensors ````````````````
  // read sensor status
    int sensorValue = analogRead(jumpsensor);
    sense = sensorValue * (5.0 / 1023.0);

//      Serial.print ("wakeUpPin ");
//      Serial.println (sense);


    if ((sense >= 0.001) && (sense_active == 1))
    {
      sense_active = 0;
      previoustime = currenttime;
      Serial.println("Time Reset");
     
    
// randomly select bird , ambulance or different sounds 
 generating = random(1, 7);

// if random number is same as previous – regenerate until new number generates
 if (generating == previous_played) {
generating = random(1, 7);
} // end if generate

else if (generating != previous_played) {
      soundof =  generating;
      previous_played =  generating;
} // end if generate



      
    } // end if sense >



switch (soundof){
case 0:
//do nothing case
noTone(buzzerpin);
delay (200);
sense_active = 1; // activate motion sensing for next loop

break;

//*******************

case 1:
     // recreate rand number only when one cycle is complete
      if (chirping_done == 1 )
     {
     chirping_done = 0;
      Numbof_variations = random(2, 15);
//      Serial.print ("Numbof_variations: ");
//      Serial.println (Numbof_variations);
      base_freq = random(700, 5000);
      } // end if chirp_done

     startup_chirp ();

break;

case 2:
      ambulance ();
break;

//*******************

case 3:
      // turn on 7 color led
random10 = random(1000, 2000);
random11 = random(4000, 5000);
random12 = random(1500, 2500);
random14 = random(250, 350);

      digitalWrite (ledpin, HIGH);
      tone(buzzerpin, random10);
      delay (random14);
      tone(buzzerpin, random11);
      delay (random14);
      tone(buzzerpin, random12);
      delay (random14);

      noTone(buzzerpin);
      soundof = 0;
//      Serial.println ("limit reached");
      sense_active = 1; // activate motion sensing for next loop
      previoustime = currenttime; // reset idle turn off time
      //``````````````````````` turn off multicor leds ````````````````
     // turn on 7 color led
      digitalWrite (ledpin, LOW);
     noTone(buzzerpin);
break;

//*******************

case 4: // Robot SOund
      // turn on 7 color led
      digitalWrite (ledpin, HIGH);
random10 = random(1000, 2000);
random11 = random(4000, 5000);
random12 = random(1500, 2500);
random14 = random(250, 350);

      tone(buzzerpin, random10);
      delay (150);
      tone(buzzerpin, 1240);
      delay (190);
      tone(buzzerpin, 3084);
      delay (250);
      tone(buzzerpin, random11);
      delay (50);
      tone(buzzerpin, 4979);
      delay (50);
      tone(buzzerpin, random12);
      delay (300);
      tone(buzzerpin, 1393);
      delay (80);
      tone(buzzerpin, 3569);
      delay (100);
      tone(buzzerpin, random14);
      delay (30);
      tone(buzzerpin, 500);
      delay (60);

      soundof = 0;
      sense_active = 1; // activate motion sensing for next loop
      previoustime = currenttime; // reset idle turn off time
      //``````````````````````` turn off multicor leds ````````````````
     // turn on 7 color led
      digitalWrite (ledpin, LOW);
     noTone(buzzerpin);
break;

//*******************
case 5: // Star Was
      // turn on 7 color led
      digitalWrite (ledpin, HIGH);

//tone(pin, note, duration)
tone(buzzerpin, LA3, Q);
delay(1 + Q); //delay duration should always be 1 ms more than the note in order to separate them.
tone(buzzerpin, LA3, Q);
delay(1 + Q);
tone(buzzerpin, LA3, Q);
delay(1 + Q);
tone(buzzerpin, F3, E + S);
delay(1 + E + S);
tone(buzzerpin, C4, S);
delay(1 + S);

tone(buzzerpin, LA3, Q);
delay(1 + Q);
tone(buzzerpin, F3, E + S);
delay(1 + E + S);
tone(buzzerpin, C4, S);
delay(1 + S);
tone(buzzerpin, LA3, H);
delay(1 + H);


      soundof = 0;
      sense_active = 1; // activate motion sensing for next loop
      previoustime = currenttime; // reset idle turn off time
      //``````````````````````` turn off multicor leds ````````````````
     // turn on 7 color led
      digitalWrite (ledpin, LOW);
     noTone(buzzerpin);
break;

case 6: // Toon toon
      // turn on 7 color led
      digitalWrite (ledpin, HIGH);

random10 = random(1000, 2000);
random11 = random(4000, 5000);
random12 = random(1500, 2500);
random14 = random(250, 350);

      tone(buzzerpin, random10);
      delay (500);
      noTone(buzzerpin);
      delay (200);
      tone(buzzerpin, random12);
      delay (300);
      noTone(buzzerpin);
      delay (200);
      tone(buzzerpin, random11);
      delay (500);


      soundof = 0;
      sense_active = 1; // activate motion sensing for next loop
      previoustime = currenttime; // reset idle turn off time
      //``````````````````````` turn off multicor leds ````````````````
     // turn on 7 color led
      digitalWrite (ledpin, LOW);
     noTone(buzzerpin);
break;


} // end switch


} // end void loop


void startup_chirp () {

//``````````````````````` turn on multicor leds ````````````````
// turn on 7 color led
digitalWrite (ledpin, HIGH);


//``````````````````````` Musical notes ````````````````
  // pick a random frequencies now
  x = base_freq + freq_variation[random(33)];


//  Serial.print ("Freq: ");
//  Serial.println (x);


  // perform chirping
  for ( int i = base_freq; i <= x; i++)
  {
    tone(buzzerpin, i);
    if (i >= x)
    {
      variation_number ++;

      if (variation_number >= Numbof_variations)
      {
        variation_number = 0;
        soundof = 0; // go to empty case
        chirping_done = 1;
        Serial.print ("limit: ");
       sense_active = 1; // activate motion sensing for next loop
       previoustime = currenttime; // reset idle turnoff time
      }
    }
  }
  noTone(buzzerpin);
  delay (random(300));

//``````````````````````` turn off multicor leds ````````````````
// turn on 7 color led
digitalWrite (ledpin, LOW);

}  // end void
void ambulance (){
//``````````````````````` turn on multicor leds ````````````````
// turn on 7 color led
digitalWrite (ledpin, HIGH);

//``````````````````````` Ambulance tone ````````````````
for (int j = 0; j<3; j++){
//Serial.print ("J");
//Serial.println (j);

tone(buzzerpin, 2000);
delay (500);
tone(buzzerpin, 3000);
delay (500);

// now turn off tone after cycle is complete
if (j >=2){
noTone(buzzerpin);
soundof = 0;
//Serial.println ("limit reached");
sense_active = 1; // activate motion sensing for next loop
previoustime = currenttime; // reset idle turn off time

} // end if limit reached

} // end for loop

//``````````````````````` turn off multicor leds ````````````````
// turn on 7 color led
digitalWrite (ledpin, LOW);

} //end void ambulance 


