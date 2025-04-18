/*  Example pausing Mozzi, restoring timers to previous states,
    during which Arduino delay() works, then resuming Mozzi again.
    
    This may be useful when using sensors or other libraries which need to use
    the same timers as Mozzi.  (Atmel Timer 0, Timer 1, and in HIFI mode, Timer 2).
  
    Circuit:
    Pushbutton on digital pin D4
       button from the digital pin to +5V (3.3V on Teensy 3.1)
       10K resistor from the digital pin to ground
    Audio output on digital pin 9 on a Uno or similar, or
    DAC/A14 on Teensy 3.1, or 
    check the README or https://sensorium.github.io/Mozzi/
    
    Mozzi help/discussion/announcements:
    https://groups.google.com/forum/#!forum/mozzi-users
  
    Tim Barrass 2013, CC by-nc-sa.
*/

//#include <ADC.h>  // Teensy 3.1 uncomment this line and install https://github.com/pedvide/ADC
#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/brownnoise8192_int8.h> // recorded audio wavetable
#include <mozzi_rand.h>

Oscil<BROWNNOISE8192_NUM_CELLS, AUDIO_RATE> aNoise(BROWNNOISE8192_DATA);

#define STOP_PIN 4

void setup(){
  pinMode(STOP_PIN, INPUT);
  aNoise.setFreq(2.f);
  startMozzi();
}


void updateControl(){
  static int previous;
  int current = digitalRead(STOP_PIN);
  if(previous==LOW && current==HIGH){
    pauseMozzi();
  }else if(previous==HIGH && current==LOW){
    unPauseMozzi();
  }
  previous=current;
  // jump to a new spot in the noise table to stop it sounding like it repeats
  aNoise.setPhase(rand(BROWNNOISE8192_NUM_CELLS));
}


int updateAudio(){
  return aNoise.next();
}


void loop(){
  audioHook();
}




