#include "MidiHandler.h"
#include "../../SIMSynthConfig.h"

MidiHandler::MidiHandler()
  : Hub(&UsbH), Midi(&UsbH), deviceConnected(false), bFirst(true), noteOnCallback(nullptr), noteOffCallback(nullptr) {
}

bool MidiHandler::begin() {
  SerialDebug.begin(115200);
  SerialDebug.println("Starting MIDI USB Host...");

  USBDevice.attach();
  delay(200);

  uint8_t attempts = 0;
  while (attempts < 3) {
    uint8_t state = UsbH.Init();
    if (state == 0) {
      SerialDebug.println("USB host initialized successfully");
      return true;
    }
    SerialDebug.print("USB host init failed, error code: 0x");
    SerialDebug.println(state, HEX);
    attempts++;
    delay(1000);
  }

  SerialDebug.println("Failed to initialize USB host after 3 attempts");
  return false;
}

void MidiHandler::update() {
  UsbH.Task();

  if (Midi) {
    if (bFirst) {
      vid = Midi.idVendor();
      pid = Midi.idProduct();
      SerialDebug.print("MIDI Device Connected - VID: 0x");
      SerialDebug.print(vid, HEX);
      SerialDebug.print(", PID: 0x");
      SerialDebug.println(pid, HEX);

      deviceConnected = true;
      bFirst = false;
    }
    MIDI_poll();
  } else if (!bFirst) {
    SerialDebug.println("MIDI device disconnected");
    deviceConnected = false;
    bFirst = true;
  }
}

void MidiHandler::MIDI_poll() {
  uint8_t bufMidi[64];
  uint16_t rcvd;

  while (Midi.RecvData(&rcvd, bufMidi) == 0 && rcvd > 0) {
    handleMidiMessage(bufMidi, rcvd);
  }
}

void MidiHandler::handleMidiMessage(uint8_t* data, uint16_t length) {

  // Debug print the raw MIDI message
  // SerialDebug.print("MIDI Message: ");
  // for (uint16_t i = 0; i < length; i++) {
  //   SerialDebug.print("0x");
  //   if (data[i] < 0x10) SerialDebug.print("0");  // Add leading zero for values < 0x10
  //   SerialDebug.print(data[i], HEX);
  //   SerialDebug.print(" ");
  // }
  // SerialDebug.println();

  // Decode and print MIDI message type
  uint8_t status = data[1];
  uint8_t channel = status & 0x0F;      // Extract MIDI channel (0-15)
  uint8_t messageType = status & 0xF0;  // Extract message type

  // SerialDebug.print("Type: ");
  // switch (messageType) {
  //   case 0x80:
  //     SerialDebug.print("Note Off");
  //     break;
  //   case 0x90:
  //     SerialDebug.print("Note On");
  //     break;
  //   case 0xA0:
  //     SerialDebug.print("Polyphonic Aftertouch");
  //     break;
  //   case 0xB0:
  //     SerialDebug.print("Control Change");
  //     break;
  //   case 0xC0:
  //     SerialDebug.print("Program Change");
  //     break;
  //   case 0xD0:
  //     SerialDebug.print("Channel Aftertouch");
  //     break;
  //   case 0xE0:
  //     SerialDebug.print("Pitch Bend");
  //     break;
  //   default:
  //     SerialDebug.print("Unknown (0x");
  //     SerialDebug.print(messageType, HEX);
  //     SerialDebug.print(")");
  // }
  // SerialDebug.print(" - Channel: ");
  // SerialDebug.println(channel + 1);  // Print channel (1-16 instead of 0-15)




  // MIDI message format: [Status byte][Data byte 1][Data byte 2]
  if (length < 3) return;

  //uint8_t status = data[0];
  uint8_t note = data[2];
  uint8_t velocity = data[3];

  // Check if it's a Note On message (0x90-0x9F)
  if ((status & 0xF0) == 0x90) {
    if (velocity > 0 && noteOnCallback) {
      noteOnCallback(note, velocity);
    } else if (velocity == 0 && noteOffCallback) {  // Note On with velocity 0 is equivalent to Note Off
      noteOffCallback(note, velocity);
    }
  }
  // Check if it's a Note Off message (0x80-0x8F)
  else if ((status & 0xF0) == 0x80 && noteOffCallback) {
    noteOffCallback(note, velocity);
  }
}