# SIMSynth

> A simple Arduino synthesizer

_Thanks to Kevin and his [wonderful blog](https://diyelectromusic.com/2020/08/18/arduino-fm-midi-synthesis-with-mozzi/) who helped me along the way._

This README is still incomplete, ping me if you need to know more!

## Samples

Samples were obtained from https://www.musicradar.com/news/drums/1000-free-drum-samples.

To create a sample (with Audacity):
1. drop a wav in Audacity
2. trim it 
3. Tracks > Mix > Mix stereo down to mono
4. Tracks > Resample (set to 16384 Hz)
5. Audio setup > Audio setting > set Project Sample Rate to 16384 Hz
6. click the down arrow on the track > Format > 16bit PCM (not sure this is really required)
7. File > Export > Export audio (Other uncompressed file > RAW (header less) - Signed 8-bit PCM) 

This should produce a .raw file. It's a good idea to import it back into Audacity and play it to make sure it's correct before continuing. (I absolutely not lost 2 days because of this)

Then use `char2mozzi` to create the header file:

```
cd Arduino/libraries/Mozzi/extras/python
python char2mozzi.py <raw-file> <h-file-output> <sample-name> 16384

# for example:
python char2mozzi.py beep.raw beep.h BEEP 16384
```

## Working on the synth

Because the Arduino USB is in "host mode", you need to double press the arduino reset button to upload:
1. Click "upload" in Arduino IDE
2. Wait for the status to switch to "uploading" (after compilation)
3. Double press the reset button
4. It should then upload normally

It's normal that the Arduino doesn't show up in the Tools > Port menu - because it's in host mode.

### Debugging via serial

Since USB serial is unusable (because the keyboard is connected), debugging should be done via a USB serial cable. 

On the cable i have, i connect:
- black to GND
- green to RX
- white to TX

When this 
