#pragma once

#include <usbh_midi.h>
#include <usbhub.h>

// Interrupt-based code from this example: 
// https://github.com/gdsports/USB_Host_Library_SAMD/pull/15
// Note the USB_Host_Library_SAMD lib needs to be patched just like 
// the PR shows, as that PR wasn't merged yet (at the time of writing)
// This interrupt-based version is needed in order not to loose midi messages

#define Is_uhd_in_received0(p)                    ((USB->HOST.HostPipe[p].PINTFLAG.reg&USB_HOST_PINTFLAG_TRCPT0) == USB_HOST_PINTFLAG_TRCPT0)
#define Is_uhd_in_received1(p)                    ((USB->HOST.HostPipe[p].PINTFLAG.reg&USB_HOST_PINTFLAG_TRCPT1) == USB_HOST_PINTFLAG_TRCPT1)
#define uhd_ack_in_received0(p)                   USB->HOST.HostPipe[p].PINTFLAG.reg = USB_HOST_PINTFLAG_TRCPT0
#define uhd_ack_in_received1(p)                   USB->HOST.HostPipe[p].PINTFLAG.reg = USB_HOST_PINTFLAG_TRCPT1
#define uhd_byte_count0(p)                        usb_pipe_table[p].HostDescBank[0].PCKSIZE.bit.BYTE_COUNT
#define uhd_byte_count1(p)                        usb_pipe_table[p].HostDescBank[1].PCKSIZE.bit.BYTE_COUNT
#define Is_uhd_in_ready0(p)                       ((USB->HOST.HostPipe[p].PSTATUS.reg&USB_HOST_PSTATUS_BK0RDY) == USB_HOST_PSTATUS_BK0RDY)  
#define uhd_ack_in_ready0(p)                       USB->HOST.HostPipe[p].PSTATUSCLR.reg = USB_HOST_PSTATUSCLR_BK0RDY
#define Is_uhd_in_ready1(p)                       ((USB->HOST.HostPipe[p].PSTATUS.reg&USB_HOST_PSTATUS_BK1RDY) == USB_HOST_PSTATUS_BK1RDY)  
#define uhd_ack_in_ready1(p)                       USB->HOST.HostPipe[p].PSTATUSCLR.reg = USB_HOST_PSTATUSCLR_BK1RDY
#define uhd_current_bank(p)                       ((USB->HOST.HostPipe[p].PSTATUS.reg&USB_HOST_PSTATUS_CURBK) == USB_HOST_PSTATUS_CURBK)  
#define Is_uhd_toggle(p)                          ((USB->HOST.HostPipe[p].PSTATUS.reg&USB_HOST_PSTATUS_DTGL) == USB_HOST_PSTATUS_DTGL)  
#define Is_uhd_toggle_error0(p)                   usb_pipe_table[p].HostDescBank[0].STATUS_PIPE.bit.DTGLER
#define Is_uhd_toggle_error1(p)                   usb_pipe_table[p].HostDescBank[1].STATUS_PIPE.bit.DTGLER


// Buffer declarations need to be aligned for SAMD21
extern uint8_t bufBk0[64] __attribute__((aligned(4)));  //Bank0
extern uint8_t bufBk1[64] __attribute__((aligned(4)));  //Bank1

void CUSTOM_UHD_Handler(void);
void handleBank0(uint32_t epAddr);
void handleBank1(uint32_t epAddr);

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
  void setNoteOnCallback(NoteCallback callback) {
    noteOnCallback = callback;
  }
  void setNoteOffCallback(NoteCallback callback) {
    noteOffCallback = callback;
  }

  // Get connection status
  bool isConnected() const {
    return deviceConnected;
  }

  // Process MIDI message from interrupt buffer
  void processMidiMessage(uint8_t* buffer, int length);

  // Static instance accessor (needed for interrupt handler)
  static MidiHandler* getInstance() {
    return instance;
  }

private:
  static MidiHandler* instance;  // Singleton instance

  USBHost UsbH;
  USBHub Hub;
  USBH_MIDI Midi;

  NoteCallback noteOnCallback;
  NoteCallback noteOffCallback;

  bool deviceConnected;
  bool bFirst;
  bool doPipeConfig;
  uint16_t pid, vid;

  void configurePipe();
  void handleDeviceConnected();
  void handleDeviceDisconnected();

  friend void CUSTOM_UHD_Handler(void);
  friend void handleBank0(uint32_t epAddr);
  friend void handleBank1(uint32_t epAddr);
};
