#include <stdio.h>
#include "midi_decode.h"

void midi_decode(fifo_midi_t* sequence, unsigned short num_tracks) {
	fifo_midi_t temp = sequence[0];
	fifo_midi_t temp_start = fifo_midi_new();
	fifo_midi_t p = fifo_midi_new();
	fifo_midi_t l = temp;
	fifo_midi_t buffer = fifo_midi_new();
	fifo_midi_t buffer_start = fifo_midi_new();
	int time_interval, actuation_time;

	// On remet tout dans la même file en triant par temps d'activation
	for(int i=1; i<num_tracks; i++) {
		p = sequence[i];
		do {
			temp_start = temp;
			do {
				if(p->val.time <= temp->val.time) { // Insertion d'un nouveau noeud
					_fifolink new = {p->val, temp};
					l->next = &new;
				}
				l = temp;
				temp = temp->next;
			} while(temp != temp_start);
			p = p->next;
		} while(p != sequence[i]);
	}

	// On peut maintenant jouer les notes
	temp_start = temp;
	do {
		time_interval = (temp->next)->val.time - temp->val.time;

		// Si l'on a plusieurs notes en même temps
		while(time_interval = 0) {
			time_interval = (temp->next)->val.time - temp->val.time;
			list_midi_add_first(temp->val, buffer);
			temp = temp->next;
		}
		list_midi_add_first(temp->val, buffer);

		// Calcul du temps d'activation (ms)
		actuation_time = 50; // arbitraire

		// Activation des pins
		buffer_start = buffer;
		do {
			note_to_pin_toggle(buffer->val.note);
			buffer = buffer->next;
		} while(buffer != buffer_start);

		HAL_Delay(actuation_time);

		// Désactivation des pins
		do {
			note_to_pin_toggle(buffer->val.note);
			buffer = buffer->next;
		} while(buffer != buffer_start);

		// Nettoyage du buffer
		buffer = list_midi_delete(buffer);
		temp = temp->next;
	} while(temp != temp_start);

	// Nettoyage de la mémoire
	fifo_midi_delete(p);
	fifo_midi_delete(l);
	fifo_midi_delete(temp_start);
	fifo_midi_delete(temp);
	fifo_midi_delete(buffer);
	fifo_midi_delete(buffer_start);
}

void note_to_pin_toggle(int note) {
	switch(note) { // TODO

	}
}
