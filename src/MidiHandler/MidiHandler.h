#pragma once

#include <usbh_midi.h>
#include <usbhub.h>

class MidiHandler {
public:
    // Callback function types
    typedef void (*NoteCallback)(byte note, byte velocity);
    
    MidiHandler();
    
    // Initialize MIDI handling
    bool begin();
    
    // Main update function - call this in loop()
    void update();
    
    // Set callback functions
    void setNoteOnCallback(NoteCallback callback) { noteOnCallback = callback; }
    void setNoteOffCallback(NoteCallback callback) { noteOffCallback = callback; }
    
    // Get connection status
    bool isConnected() const { return deviceConnected; }

private:
    USBHost UsbH;
    USBHub Hub;
    USBH_MIDI Midi;
    
    NoteCallback noteOnCallback;
    NoteCallback noteOffCallback;
    
    bool deviceConnected;
    bool bFirst;
    uint16_t pid, vid;
    uint32_t lastConnectionAttempt;
    static const uint32_t CONNECTION_RETRY_TIME = 2000;
    
    void handleMidiMessage(uint8_t* data, uint16_t length);
    void MIDI_poll();
};