#include "sequence.h"

// Decode a sequence, check header for format
int sequence_decode(sequence sequence) {
	uint16_t pin;
	for(uint16_t i=0; i<sequence.size; i++) {
		pin = sequence.data[i].pin;
		HAL_GPIO_TogglePin(sequence.data[i].GPIO_type, pin);
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
		HAL_Delay(ACTUATION_TIME);
		HAL_GPIO_TogglePin(sequence.data[i].GPIO_type, pin);
		HAL_Delay(sequence.data[i].delay);
	}
	return 1;
}
