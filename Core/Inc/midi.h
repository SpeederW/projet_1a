#ifndef MIDI_H_
#define MIDI_H_

#include "midi_decode.h"

typedef struct {
	uint32_t time;
	int note;
	uint8_t force;
} MIDI_Event;

unsigned int read_vlq(FILE* file);
unsigned short read_ushort(FILE *file);
unsigned int read_uint(FILE *file);
const char* note_name(int note, char* buffer);
fifo_midi_t process_track(FILE *file, unsigned short division, int track_number);
fifo_midi_t* midi_file_read(FILE* file);

#endif
