/*  Example of reverb on a synthesised sound
    using Mozzi sonification library.

    Demonstrates ReverbTank, a reverb small enough to fit on
    the Arduino Nano, which for some reason wasn't able to fit a larger version
    which did fit on other 328 based boards.
    It's a pretty horrible reverb which sounds like the inside of a tin can.
    For simplicity, ReverbTank has hardcoded maximum delay sizes
    but also has default delay times which can be changed in the constructor
    or by setting during run time to allow live tweaking.
    The synthesised sound comes from the phasemod synth example.

    Circuit: Audio output on digital pin 9 for STANDARD output on a Uno or similar, or
    see the readme.md file for others.

   Mozzi documentation/API
   https://sensorium.github.io/Mozzi/doc/html/index.html

   Mozzi help/discussion/announcements:
   https://groups.google.com/forum/#!forum/mozzi-users

   Copyright 2013-2024 Tim Barrass and the Mozzi Team

   Mozzi is licensed under the GNU Lesser General Public Licence (LGPL) Version 2.1 or later.
*/

#define MOZZI_CONTROL_RATE 64 // quite fast, keeps modulation smooth
#include <Mozzi.h>
#include <ReverbTank.h>
#include <Oscil.h>
#include <tables/cos8192_int8.h>
#include <tables/envelop2048_uint8.h>

ReverbTank reverb;

// Synth from PhaseMod_Envelope example
Oscil <COS8192_NUM_CELLS, MOZZI_AUDIO_RATE> aCarrier(COS8192_DATA);
Oscil <COS8192_NUM_CELLS, MOZZI_AUDIO_RATE> aModulator(COS8192_DATA);
Oscil <COS8192_NUM_CELLS, MOZZI_AUDIO_RATE> aModWidth(COS8192_DATA);
Oscil <COS8192_NUM_CELLS, MOZZI_CONTROL_RATE> kModFreq1(COS8192_DATA);
Oscil <COS8192_NUM_CELLS, MOZZI_CONTROL_RATE> kModFreq2(COS8192_DATA);
Oscil <ENVELOP2048_NUM_CELLS, MOZZI_AUDIO_RATE> aEnvelop(ENVELOP2048_DATA);


void setup(){
  // synth params
  aCarrier.setFreq(55);
  kModFreq1.setFreq(3.98f);
  kModFreq2.setFreq(3.31757f);
  aModWidth.setFreq(2.52434f);
  aEnvelop.setFreq(9.0f);

  startMozzi();
}


void updateControl(){
  // synth control
  aModulator.setFreq(277.0f + 0.4313f*kModFreq1.next() + kModFreq2.next());
}


AudioOutput updateAudio(){
  int synth = aCarrier.phMod((int)aModulator.next()*(150u+aModWidth.next()));
  synth *= (byte)aEnvelop.next();
  synth >>= 8;
  // here's the reverb
  int arev = reverb.next(synth);
  // add the dry and wet signals
  return MonoOutput::fromAlmostNBit(9, synth + (arev>>3));
}


void loop(){
  audioHook();
}
