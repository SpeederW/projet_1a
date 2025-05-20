#include <midi_data_process.h>
#include <stdio.h>

void process_data(uint8_t data[32], uint8_t size) {
	node queue[size/2];
	node temp;
	uint8_t i;
	int note;
	for(i=0; i<size/2; i++) {
		temp.note = data[i];
		temp.time = data[i*2];
		queue[i] = temp;
	}

	// Activation des pins
	for(i=0; i<size/2; i++) {
		note = queue[k].note;
		HAL_GPIO_TogglePin(note_to_port(note), note_to_pin(note));
	}

	// Temps d'activation
	HAL_Delay(100);

	// Désactivation des pins
	for(i=0; i<size/2; i++) {
		note = queue[k].note;
		HAL_GPIO_TogglePin(note_to_port(note), note_to_pin(note));
	}
}
