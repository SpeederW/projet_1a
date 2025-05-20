#ifndef MIDI_H_
#define MIDI_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "fifo_midi.h"
#include "midi_decode.h"
#include "serial_port.h"

#define NUM_THREADS 2

unsigned int read_vlq(FILE* file);
unsigned short read_ushort(FILE *file);
unsigned int read_uint(FILE *file);
const char* note_name(int note, char* buffer);
fifo_midi_t process_track(FILE *file, unsigned short division, int track_number);
fifo_midi_t* midi_file_read(FILE* file);

#endif
