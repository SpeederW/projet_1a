#ifndef DATA_PROCESS_H_
#define DATA_PROCESS_H_

typedef struct {
	int note;
	uint8_t time;
} node;

process_data(uint8_t data[32], uint16_t size);

#endif
