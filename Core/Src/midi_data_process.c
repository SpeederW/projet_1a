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
	HAL_Delay(ACTUATION_TIME);

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
	switch (note % 12){
		case (0):
				return OUT1_Pin;
				break;
		case (1):
				return OUT1_Pin;
				break;
		case (2):
				return OUT2_Pin;
				break;
		case (3):
				return OUT2_Pin;
				break;
		case (4):
				return OUT3_Pin;
				break;
		case (5):
				return OUT4_Pin;
				break;
		case (6):
				return OUT4_Pin;
				break;
		case (7):
				return OUT5_Pin;
				break;
		case (8):
				return OUT5_Pin;
				break;
		case (9):
				return OUT6_Pin;
				break;
		case (10):
				return OUT6_Pin;
				break;
		case (11):
				return OUT7_Pin;
				break;
	}
	perror("erreur, note non reconnue");
	return -1;
}

