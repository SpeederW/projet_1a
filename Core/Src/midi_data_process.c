#include "midi_data_process.h"
#include <stdio.h>

/**
 * @brief Traite les données MIDI reçues et active/désactive les pins correspondantes.
 *
 * Cette fonction prend un tableau de données MIDI, le découpe en paires (note, temps),
 * puis active les pins GPIO correspondantes aux notes reçues. Après un délai prédéfini
 * (ACTUATION_TIME), elle désactive les mêmes pins. Si la séquence de fin de transmission
 * (255, 255) est détectée au début du tableau, la fonction retourne 1 pour indiquer la fin.
 *
 * @param data Pointeur vers le tableau de données MIDI à traiter.
 * @param size Taille du tableau de données (doit être un multiple de 2).
 * @return 1 si la fin de transmission est détectée, 0 sinon.
 */
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

/**
 * @brief Retourne le port GPIO associé à une note MIDI donnée.
 *
 * Cette fonction prend en entrée un numéro de note MIDI et retourne le port GPIO correspondant.
 * Si la note correspond à la note 11 (c'est-à-dire si note % 12 == 11), la fonction retourne GPIOB.
 * Pour toutes les autres notes, la fonction retourne GPIOA.
 *
 * @param note Numéro de la note MIDI (entier).
 * @return GPIO_TypeDef* Pointeur vers le port GPIO associé à la note.
 */
GPIO_TypeDef* note_to_port(int note) {
	if (note %12 == 11){
		return GPIOB;
	}
	else
		return GPIOA;
}

/**
 * @brief Associe une note MIDI à une broche de sortie spécifique.
 *
 * Cette fonction prend en entrée une note MIDI (entier) et retourne la broche de sortie
 * correspondante (OUTx_Pin) en fonction du modulo 12 de la note, afin de gérer les différentes
 * notes sur des sorties distinctes. Si la note n'est pas reconnue, une erreur est affichée
 * et la fonction retourne -1.
 *
 * @param note La valeur de la note MIDI à traiter.
 * @return La broche de sortie associée à la note, ou -1 en cas d'erreur.
 */
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

