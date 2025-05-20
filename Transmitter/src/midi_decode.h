#ifndef MIDI_DECODE_H_
#define MIDI_DECODE_H_

#include <stdio.h>
#include "midi.h"

#define ACTUATION_TIME 100 // Temps d'activation en ms

void midi_decode(HANDLE port, fifo_midi_t* sequence, unsigned short num_tracks);

#endif