#include "SIMSynthConfig.h"

// Include Mozzi stuff
#include <Mozzi.h>
#include <mozzi_midi.h>
#include <Sample.h>
#include <Metronome.h>


#include "src/Instruments/SynthWave2/SynthWave2.h"
#include "src/Instruments/ChaosSynth/ChaosSynth.h"
#include "src/Instruments/GentleSynth/GentleSynth.h"
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
#define INSTRUMENT_POT3_PIN 3 
#define TEMPO_POT_PIN 4

// Digital pins
#define INSTRUMENT_SWITCH_PIN1 0
#define INSTRUMENT_SWITCH_PIN2 1
#define DRUM_KICK_SWITCH_PIN 2
#define DRUM_CLAP_SWITCH_PIN 3
#define DRUM_IHAT_SWITCH_PIN 4
#define BEAT_LED_1_PIN 6
#define BEAT_LED_2_PIN 7
#define BEAT_LED_3_PIN 8
#define BEAT_LED_4_PIN 9
#define ENVELOPE_SELECTOR_1_PIN 10
#define ENVELOPE_SELECTOR_2_PIN 11
#define GATE_SYNC_PIN 12

// Generic pot controllers (0-1 range)
PotController pot1(INSTRUMENT_POT1_PIN, 0.1, 1.0);
PotController pot2(INSTRUMENT_POT2_PIN, 0.1, 1.0);
PotController pot3(INSTRUMENT_POT3_PIN, 0.1, 1.0);
PotController tempot(TEMPO_POT_PIN, 60.0, 140.0);

// Instruments
ChaosSynth chaosSynth(midiToFreqWrapper);
SynthwaveSynth synthWave(midiToFreqWrapper);
GentleSine gentleSine(midiToFreqWrapper);



Instrument* currentInstrument = &synthWave;
enum InstrumentType {
  CHAOS_SYNTH,
  SYNTH_WAVE,
  GENTLE_SINE,
} currentInstrumentType = SYNTH_WAVE;
const int INSTRUMENT_VOLUMES[] = { // 0-255
  180, // CHAOS_SYNTH volume
  220, // SYNTH_WAVE volume
  100, // GENTLE_SINE volume
};

// Drums
Sample<BD_NUM_CELLS, MOZZI_AUDIO_RATE> kickSample(BD_DATA);
Sample<CLAP_NUM_CELLS, MOZZI_AUDIO_RATE> clapSample(CLAP_DATA);
Sample<IHAT_NUM_CELLS, MOZZI_AUDIO_RATE> ihatSample(IHAT_DATA);


// Kick patterns
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

// Envelope
byte currentEnvelopeNumber = 255;

// Beat stuff
Metronome metro;
byte sixteenthBeatIndex = 0;
byte beatIndex = 0;
const byte beatLedPins[] = { BEAT_LED_1_PIN, BEAT_LED_2_PIN, BEAT_LED_3_PIN, BEAT_LED_4_PIN };

byte noteBeingOn = 0;



// Debug mode 
bool debugMode = false;  // Set to false when you want to use MIDI
unsigned long lastNoteTime = 0;
const unsigned long NOTE_DURATION = 500;  // 500ms per note
byte debugNoteIndex = 0;
const byte debugNotes[] = {60, 64, 67, 72, 67, 64};  // C4, E4, G4, C5, G4, E4
const byte NUM_DEBUG_NOTES = 6;



void noteOn(byte note, byte velocity) {
  noteBeingOn = note;
  currentInstrument->noteOn(note, velocity);
}

void noteOff(byte note, byte velocity) {
  if(noteBeingOn!=note){
    return;
  }
  currentInstrument->noteOff(note, velocity);
}

void switchInstrument() {
  // Read switch state
  bool switchState1 = digitalRead(INSTRUMENT_SWITCH_PIN1);
  bool switchState2 = digitalRead(INSTRUMENT_SWITCH_PIN2);

  InstrumentType newInstrumentType = switchState1==LOW ? CHAOS_SYNTH : (switchState2==LOW ? GENTLE_SINE : SYNTH_WAVE);

  // Only switch if there's actually a change
  if (newInstrumentType != currentInstrumentType) {
    SerialDebug.print("Switched to ");
    if(newInstrumentType==SYNTH_WAVE){
      currentInstrument = static_cast<Instrument*>(&synthWave);
      SerialDebug.println("SYNTH_WAVE");
    } else if (newInstrumentType==CHAOS_SYNTH){
      currentInstrument = static_cast<Instrument*>(&chaosSynth);
      SerialDebug.println("CHAOS_SYNTH");
    } else {
      currentInstrument = static_cast<Instrument*>(&gentleSine);
      SerialDebug.println("GENTLE_SINE");
    }
    currentInstrumentType = newInstrumentType;
    currentInstrument->setup();  // Reinitialize the new instrument
  }
}

void switchEnvelope(){
  bool switchState1 = digitalRead(ENVELOPE_SELECTOR_1_PIN);
  bool switchState2 = digitalRead(ENVELOPE_SELECTOR_2_PIN);

  byte enveloppeNumber = switchState1==LOW ? 0 : (switchState2==LOW ? 2 : 1);
  if(currentEnvelopeNumber != enveloppeNumber){
    currentInstrument->setEnvelope(enveloppeNumber);
    currentEnvelopeNumber = enveloppeNumber;
    SerialDebug.print("Envelope switched to ");
    SerialDebug.println(currentEnvelopeNumber);
  }
}

void setup() {
  SerialDebug.begin(115200);

  pinMode(INSTRUMENT_SWITCH_PIN1, INPUT_PULLUP);
  pinMode(INSTRUMENT_SWITCH_PIN2, INPUT_PULLUP);
  pinMode(DRUM_KICK_SWITCH_PIN, INPUT_PULLUP);
  pinMode(DRUM_CLAP_SWITCH_PIN, INPUT_PULLUP);
  pinMode(DRUM_IHAT_SWITCH_PIN, INPUT_PULLUP);

  pinMode(ENVELOPE_SELECTOR_1_PIN, INPUT_PULLUP);
  pinMode(ENVELOPE_SELECTOR_2_PIN, INPUT_PULLUP);

  pinMode(BEAT_LED_1_PIN, OUTPUT);
  pinMode(BEAT_LED_2_PIN, OUTPUT);
  pinMode(BEAT_LED_3_PIN, OUTPUT);
  pinMode(BEAT_LED_4_PIN, OUTPUT);
  
  pinMode(GATE_SYNC_PIN, OUTPUT);

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


unsigned long syncHighAt = 0;
bool isSyncPinHigh = false;
void updateControl() {

  if (debugMode) {
    unsigned long currentTime = millis();
    if (currentTime - lastNoteTime >= NOTE_DURATION) {
      // Turn off previous note
      noteOff(debugNotes[debugNoteIndex], 100);
      
      // Move to next note
      debugNoteIndex = (debugNoteIndex + 1) % NUM_DEBUG_NOTES;
      
      // Play new note
      noteOn(debugNotes[debugNoteIndex], 100);
      
      lastNoteTime = currentTime;
    }
  }



  midiHandler.update();

  switchInstrument();
  switchEnvelope();

  // Read pots and send to current instrument
  currentInstrument->setPot1Value(pot1.read());
  currentInstrument->setPot2Value(pot2.read());
  currentInstrument->setPot3Value(pot3.read());

  currentInstrument->updateControl();

  metro.setBPM(tempot.read() * 4);

  if (metro.ready()) {
    isSyncPinHigh = true;
    digitalWrite(beatLedPins[beatIndex], LOW);

    // update beat
    sixteenthBeatIndex++;
    if (sixteenthBeatIndex >= 16) {
      sixteenthBeatIndex = 0;
    }

    if(sixteenthBeatIndex==0 || sixteenthBeatIndex==4 || sixteenthBeatIndex==8 || sixteenthBeatIndex==12){
      digitalWrite(GATE_SYNC_PIN, HIGH);
      syncHighAt = millis();
    }
    

    beatIndex = sixteenthBeatIndex / 4;
    digitalWrite(beatLedPins[beatIndex], HIGH);

    kickVoice.tick(sixteenthBeatIndex, digitalRead(DRUM_KICK_SWITCH_PIN) == LOW);
    clapVoice.tick(sixteenthBeatIndex, digitalRead(DRUM_CLAP_SWITCH_PIN) == LOW);
    ihatVoice.tick(sixteenthBeatIndex, digitalRead(DRUM_IHAT_SWITCH_PIN) == LOW);
  }

  if(millis() > syncHighAt+100 && isSyncPinHigh){
    digitalWrite(GATE_SYNC_PIN, LOW);
    isSyncPinHigh = false;
  }
}


const int VOICE_BITS = 9;  // Adjust this based on your voice output
//uint8_t voiceVolume = 120;  // 0-255
uint8_t drumVolume = 250;   // 0-255

AudioOutput updateAudio() {
  // Get base signals
  int32_t voice = currentInstrument->updateAudio();
  int32_t kick = kickVoice.updateAudio();
  int32_t clap = clapVoice.updateAudio();
  int32_t ihat = ihatVoice.updateAudio();
  
  // Scale voice if needed (adjust VOICE_BITS if audio is too quiet/loud)
  voice = voice << (14 - VOICE_BITS);
  
  // Mix drums together first
  int32_t drums = kick + clap + ihat;
  
  // Apply volume controls and mix
  uint8_t voiceVolume = INSTRUMENT_VOLUMES[currentInstrumentType];
  voice = (voice * voiceVolume) >> 5;
  drums = (drums * drumVolume) >> 5;
  
  // Combine and protect against overflow
  int32_t mixed = (voice + drums) >> 1;  // Divide by 2 for headroom
  
  return MonoOutput::fromAlmostNBit(14, mixed).clip();
}

void loop() {
  audioHook();
}