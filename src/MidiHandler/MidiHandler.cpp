#include "MidiHandler.h"
#include "../../SIMSynthConfig.h"

// Initialize static instance
MidiHandler* MidiHandler::instance = nullptr;

// Global buffer definitions
uint8_t bufBk0[64] __attribute__ ((aligned (4)));
uint8_t bufBk1[64] __attribute__ ((aligned (4)));

MidiHandler::MidiHandler()
    : Hub(&UsbH), Midi(&UsbH), deviceConnected(false), bFirst(true), 
      doPipeConfig(false), noteOnCallback(nullptr), noteOffCallback(nullptr) {
    instance = this;  // Set singleton instance
}

bool MidiHandler::begin() {
    SerialDebug.begin(115200);
    SerialDebug.println("Starting MIDI USB Host...");

    if (UsbH.Init()) {
        SerialDebug.println("USB host failed to start");
        return false;
    }

    // Set up interrupt handler
    USB_SetHandler(&CUSTOM_UHD_Handler);
    delay(200);
    return true;
}

void MidiHandler::update() {
    // Handle enumeration only, avoid polling in main loop
    if (doPipeConfig || (!deviceConnected && (UsbH.getUsbTaskState() != USB_DETACHED_SUBSTATE_WAIT_FOR_DEVICE))) {
        UsbH.Task();
    } else if (deviceConnected && (UsbH.getUsbTaskState() != USB_STATE_RUNNING)) {
        UsbH.Task();
    }

    if (deviceConnected && (UsbH.getUsbTaskState() == USB_STATE_RUNNING)) {
        if (Midi && (Midi.GetAddress() != Hub.GetAddress()) && (Midi.GetAddress() != 0)) {
            if (doPipeConfig) {
                configurePipe();
            }
        }
    } else {
        USB_SetHandler(&CUSTOM_UHD_Handler);
        if (Midi) {
            USB->HOST.HostPipe[Midi.GetEpAddress()].PINTENCLR.reg = 0xFF; // Disable pipe interrupts
        }
    }
}

void MidiHandler::configurePipe() {
    uint32_t epAddr = Midi.GetEpAddress();
    doPipeConfig = false;
    uint16_t rcvd;

    while (deviceConnected && (USB->HOST.HostPipe[epAddr].PCFG.bit.PTYPE != 0x03)) {
        UsbH.Task();
        Midi.RecvData(&rcvd, bufBk0);
    }

    // Configure pipe for interrupt-based operation
    USB->HOST.HostPipe[epAddr].BINTERVAL.reg = 0x01;
    usb_pipe_table[epAddr].HostDescBank[0].ADDR.reg = (uint32_t)bufBk0;
    usb_pipe_table[epAddr].HostDescBank[1].ADDR.reg = (uint32_t)bufBk1;
    USB->HOST.HostPipe[epAddr].PCFG.bit.PTOKEN = tokIN;
    USB->HOST.HostPipe[epAddr].PSTATUSCLR.reg = USB_HOST_PSTATUSCLR_BK0RDY;
    uhd_unfreeze_pipe(epAddr);
    USB->HOST.HostPipe[epAddr].PINTENSET.reg = 0x3;  // Enable pipe interrupts

    SerialDebug.println("Pipe configured for interrupts");
}

void MidiHandler::processMidiMessage(uint8_t* buffer, int length) {
    // Process buffer in chunks of 4 bytes (USB-MIDI packet format)
    for (int i = 0; i < length; i++) {
        if (buffer[i] > 0) {  // Found start of a message
            uint8_t midiMsg[3];
            midiMsg[0] = buffer[i + 1];  // Status byte
            midiMsg[1] = buffer[i + 2];  // Data byte 1
            midiMsg[2] = buffer[i + 3];  // Data byte 2

            uint8_t status = midiMsg[0] & 0xF0;  // Strip channel
            uint8_t note = midiMsg[1];
            uint8_t velocity = midiMsg[2];
            
            if (noteOnCallback && status == 0x90 && velocity > 0) {
                noteOnCallback(note, velocity);
            } else if (noteOffCallback && (status == 0x80 || (status == 0x90 && velocity == 0))) {
                noteOffCallback(note, velocity);
            }
            
            i += 3;  // Skip the remaining bytes of this message
        }
    }
}

// Global interrupt handler functions
void CUSTOM_UHD_Handler(void) {
    MidiHandler* handler = MidiHandler::getInstance();
    if (!handler) return;

    uint32_t epAddr = handler->Midi.GetEpAddress();

    if (USB->HOST.INTFLAG.reg == USB_HOST_INTFLAG_DCONN) {
        SerialDebug.println("Connected");
        handler->doPipeConfig = true;
        handler->deviceConnected = true;
    } else if (USB->HOST.INTFLAG.reg == USB_HOST_INTFLAG_DDISC) {
        SerialDebug.println("Disconnected");
        handler->deviceConnected = false;
        USB->HOST.HostPipe[epAddr].PINTENCLR.reg = 0xFF;
    }

    UHD_Handler();
    uhd_freeze_pipe(epAddr);

    // Handle both banks if needed
    if (Is_uhd_in_received0(epAddr) && Is_uhd_in_received1(epAddr) && uhd_current_bank(epAddr)) {
        handleBank1(epAddr);
    }
    if (Is_uhd_in_received0(epAddr)) {
        handleBank0(epAddr);
    }
    if (Is_uhd_in_received1(epAddr)) {
        handleBank1(epAddr);
    }
    uhd_unfreeze_pipe(epAddr);
}

void handleBank0(uint32_t epAddr) {
    MidiHandler* handler = MidiHandler::getInstance();
    if (!handler) return;

    int rcvd = uhd_byte_count0(epAddr);
    if (rcvd > 0) {
        handler->processMidiMessage(bufBk0, rcvd);
    }
    uhd_ack_in_received0(epAddr);
    uhd_ack_in_ready0(epAddr);
}

void handleBank1(uint32_t epAddr) {
    MidiHandler* handler = MidiHandler::getInstance();
    if (!handler) return;

    int rcvd = uhd_byte_count1(epAddr);
    if (rcvd > 0) {
        handler->processMidiMessage(bufBk1, rcvd);
    }
    uhd_ack_in_received1(epAddr);
    uhd_ack_in_ready1(epAddr);
}