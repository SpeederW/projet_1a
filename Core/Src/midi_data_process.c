#include "midi_data_process.h"
#include <stdio.h>

int process_data(uint8_t* data, uint8_t size) {
	node queue[size/2];
	node temp;
	uint8_t i;
	int note;

	if(data[0] == 255 && data[1] == 255) { // Fin de transmission
		return 1;
	}

	for(i=0; i<=size/2; i += 2) {
		temp.note = data[i];
		temp.time = data[i+1];
		queue[i] = temp;
	}

	// Activation des pins
	for(i=0; i<size/2; i++) {
		note = queue[i].note;
		HAL_GPIO_TogglePin(note_to_port(note), note_to_pin(note));
	}

	// Temps d'activation
	HAL_Delay(30);

	// Désactivation des pins
	for(i=0; i<size/2; i++) {
		note = queue[i].note;
		HAL_GPIO_TogglePin(note_to_port(note), note_to_pin(note));
	}
	return 0;
}

GPIO_TypeDef* note_to_port(int note) {
	if (note %12 == 11){
		return GPIOB;
	}
	else
		return GPIOA;
}

int note_to_pin(int note) {
	switch (note){
		case (note %12 == 0):
				return GPIO_OUT1;
		case (note %12 == 1):
				return GPIO_OUT1;
		case (note %12 == 2):
				return GPIO_OUT2;
		case (note %12 == 3):
				return GPIO_OUT2;
		case (note %12 == 4):
				return GPIO_OUT2;
		case (note %12 == 5):
				return GPIO_OUT3;
		case (note %12 == 6):
				return GPIO_OUT4;
		case (note %12 == 7):
				return GPIO_OUT5;
		case (note %12 == 8):
				return GPIO_OUT5;
		case (note %12 == 9):
				return GPIO_OUT6;
		case (note %12 == 10):
				return GPIO_OUT6;
		case (note %12 == 11):
				return GPIO_OUT7;
	}
}
