#ifndef MIDI_DECODE_H_
#define MIDI_DECODE_H_

#include "midi.h"
#include "fifo_midi.h"

void midi_decode(fifo_midi_t* sequence);
void note_to_pin(int note);

#endif
