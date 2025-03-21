#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "stm32f0xx_hal.h"

#define ACTUATION_TIME 10 // TODO: measure

// Define corresponding note pins (see config)
#define PIN_NOTE_1 GPIO_PIN_0

typedef struct {
	uint16_t pin;
	uint16_t delay; // in milliseconds
	GPIO_TypeDef* GPIO_type;
} note;

typedef struct {
	note* data;
	uint16_t size;
} sequence;

int sequence_decode(sequence sequence);

#endif
