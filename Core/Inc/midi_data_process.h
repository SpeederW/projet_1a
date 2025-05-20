#ifndef DATA_PROCESS_H_
#define DATA_PROCESS_H_

#include <stdint.h>
#include "main.h"

typedef struct {
	int note;
	uint8_t time;
} node;

void process_data(uint8_t* data, uint16_t size);
GPIO_TypeDef* note_to_port(int note);
int note_to_pin(int note);

#endif
