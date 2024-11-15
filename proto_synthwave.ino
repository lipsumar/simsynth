#include "SIMSynthConfig.h"

// Include Mozzi stuff
#include <Mozzi.h>
#include <mozzi_midi.h>
#include <Sample.h>
#include <Metronome.h>

#include "src/Instruments/SynthWave/SynthWave.h"
#include "src/Instruments/FM/FM.h"
#include "src/PotController/PotController.h"
#include "src/DrumVoice/DrumVoice.h"

#include "src/samples/kick.h"
#include "src/samples/clap.h"
#include "src/samples/ihat.h"

#include "src/MidiHandler/MidiHandler.h"
MidiHandler midiHandler;

// Wrapper function for MIDI to frequency conversion
float midiToFreqWrapper(uint8_t note) {
  return mtof(note);
}

// Analog pins
#define INSTRUMENT_POT1_PIN 1
#define INSTRUMENT_POT2_PIN 2
#define INSTRUMENT_POT3_PIN 3 // currently unused
#define TEMPO_POT_PIN 4

// Digital pins
#define INSTRUMENT_SWITCH_PIN 1
#define DRUM_KICK_SWITCH_PIN 2
#define DRUM_CLAP_SWITCH_PIN 3
#define DRUM_IHAT_SWITCH_PIN 4
#define BEAT_LED_1_PIN 6
#define BEAT_LED_2_PIN 7
#define BEAT_LED_3_PIN 8
#define BEAT_LED_4_PIN 9

// Generic pot controllers (0-1 range)
PotController pot1(INSTRUMENT_POT1_PIN, 0.1, 1.0);
PotController pot2(INSTRUMENT_POT2_PIN, 0.1, 1.0);
PotController tempot(TEMPO_POT_PIN, 60.0, 140.0);

// Instruments
SynthWave synthWave(midiToFreqWrapper);
FMInstrument fMInstrument(midiToFreqWrapper);
Instrument* currentInstrument = &synthWave;
enum InstrumentType {
  SYNTH_WAVE,
  FM
} currentInstrumentType = SYNTH_WAVE;

// Drums
Sample<BD_NUM_CELLS, MOZZI_AUDIO_RATE> kickSample(BD_DATA);
Sample<CLAP_NUM_CELLS, MOZZI_AUDIO_RATE> clapSample(CLAP_DATA);
Sample<IHAT_NUM_CELLS, MOZZI_AUDIO_RATE> ihatSample(IHAT_DATA);

// Drums patterns
// const uint8_t kickPattern1[] = { 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 };  // Basic four-on-the-floor
// const uint8_t kickPattern2[] = { 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0 };  // Variation
// const uint8_t* kickPatterns[] = { kickPattern1, kickPattern2 };

// Ki// Kick patterns
const uint8_t kickPattern1[] = { 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 };  // Basic four-on-the-floor
const uint8_t kickPattern2[] = { 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0 };  // Hip-hop style
const uint8_t kickPattern3[] = { 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0 };  // Bouncy rhythm
const uint8_t kickPattern4[] = { 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0 };  // Energetic double kicks
const uint8_t* kickPatterns[] = { kickPattern1, kickPattern2, kickPattern3, kickPattern4 };

// Clap patterns
const uint8_t clapPattern1[] = { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 };  // Basic backbeat
const uint8_t clapPattern2[] = { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 };  // Backbeat with pickup
const uint8_t clapPattern3[] = { 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0 };  // Syncopated
const uint8_t clapPattern4[] = { 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0 };  // Double claps
const uint8_t* clapPatterns[] = { clapPattern1, clapPattern2, clapPattern3, clapPattern4 };

// Hi-hat patterns
const uint8_t hihatPattern1[] = { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 };  // Basic eighth notes
const uint8_t hihatPattern2[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };  // Steady sixteenths
const uint8_t hihatPattern3[] = { 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1 };  // Alternating pattern
const uint8_t hihatPattern4[] = { 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1 };  // Funky rhythm
const uint8_t* hihatPatterns[] = { hihatPattern1, hihatPattern2, hihatPattern3, hihatPattern4 };

DrumVoice<BD_NUM_CELLS> kickVoice(&kickSample, kickPatterns, 4);
DrumVoice<CLAP_NUM_CELLS> clapVoice(&clapSample, clapPatterns, 4);
DrumVoice<IHAT_NUM_CELLS> ihatVoice(&ihatSample, hihatPatterns, 4);

// Beat stuff
Metronome metro;
byte sixteenthBeatIndex = 0;
byte beatIndex = 0;
const byte beatLedPins[] = { BEAT_LED_1_PIN, BEAT_LED_2_PIN, BEAT_LED_3_PIN, BEAT_LED_4_PIN };

void noteOn(byte note, byte velocity) {
  currentInstrument->noteOn(note, velocity);
}

void noteOff(byte note, byte velocity) {
  currentInstrument->noteOff(note, velocity);
}

void switchInstrument() {
  // Read switch state
  bool switchState = digitalRead(INSTRUMENT_SWITCH_PIN);

  InstrumentType newInstrumentType = switchState ? FM : SYNTH_WAVE;

  // Only switch if there's actually a change
  if (newInstrumentType != currentInstrumentType) {
    currentInstrument = (newInstrumentType == SYNTH_WAVE)
                          ? static_cast<Instrument*>(&synthWave)
                          : static_cast<Instrument*>(&fMInstrument);
    currentInstrumentType = newInstrumentType;
    currentInstrument->setup();  // Reinitialize the new instrument
    SerialDebug.print("Switched to ");
    SerialDebug.println(currentInstrumentType == SYNTH_WAVE ? "SynthWave" : "FM");
  }
}

void setup() {
  SerialDebug.begin(115200);

  pinMode(INSTRUMENT_SWITCH_PIN, INPUT_PULLUP);
  pinMode(DRUM_KICK_SWITCH_PIN, INPUT_PULLUP);
  pinMode(DRUM_CLAP_SWITCH_PIN, INPUT_PULLUP);
  pinMode(DRUM_IHAT_SWITCH_PIN, INPUT_PULLUP);

  pinMode(BEAT_LED_1_PIN, OUTPUT);
  pinMode(BEAT_LED_2_PIN, OUTPUT);
  pinMode(BEAT_LED_3_PIN, OUTPUT);
  pinMode(BEAT_LED_4_PIN, OUTPUT);

  // Initialize MIDI handling
  if (!midiHandler.begin()) {
    SerialDebug.println("Failed to initialize MIDI. Check connections.");
    while (1)
      ;  // halt
  }

  startMozzi();
  currentInstrument->setup();

  midiHandler.setNoteOnCallback(noteOn);
  midiHandler.setNoteOffCallback(noteOff);

  metro.setBPM(120.0 * 4);
  metro.start();
}


void updateControl() {
  midiHandler.update();

  switchInstrument();

  // Read pots and send to current instrument
  currentInstrument->setPot1Value(pot1.read());
  currentInstrument->setPot2Value(pot2.read());

  currentInstrument->updateControl();

  metro.setBPM(tempot.read() * 4);

  if (metro.ready()) {
    digitalWrite(beatLedPins[beatIndex], LOW);

    // update beat
    sixteenthBeatIndex++;
    if (sixteenthBeatIndex >= 16) {
      sixteenthBeatIndex = 0;
    }

    beatIndex = sixteenthBeatIndex / 4;
    digitalWrite(beatLedPins[beatIndex], HIGH);

    kickVoice.tick(sixteenthBeatIndex, digitalRead(DRUM_KICK_SWITCH_PIN) == LOW);
    clapVoice.tick(sixteenthBeatIndex, digitalRead(DRUM_CLAP_SWITCH_PIN) == LOW);
    ihatVoice.tick(sixteenthBeatIndex, digitalRead(DRUM_IHAT_SWITCH_PIN) == LOW);
  }
}


AudioOutput updateAudio() {
  int16_t voice = currentInstrument->updateAudio();
  int kick = kickVoice.updateAudio();
  int clap = clapVoice.updateAudio();
  int ihat = ihatVoice.updateAudio();
  //return voice + kick;
  return voice
         + MonoOutput::fromAlmostNBit(14, kick).clip()
         + MonoOutput::fromAlmostNBit(14, clap).clip()
         + MonoOutput::fromAlmostNBit(14, ihat).clip();
}

void loop() {
  audioHook();
}