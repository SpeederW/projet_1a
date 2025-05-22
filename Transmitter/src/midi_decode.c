#include "midi_decode.h"

void midi_decode(HANDLE port, fifo_midi_t sequence[], unsigned short num_tracks) {
	fifo_midi_t temp = sequence[1]->next; 
	fifo_midi_t temp_start = fifo_midi_new();
	fifo_midi_t buffer = fifo_midi_new();
	fifo_midi_t buffer_start = fifo_midi_new();
	int time_interval = 0;
	int event_count = 0;

	printf("midi_decode: Debut du decodage.\n");

	// On remet tout dans la même file en triant par temps d'activation 
	for (int i = 1; i < num_tracks; i++) {
		fifo_midi_t current = sequence[i];
		if (!current) continue;

		fifo_midi_t start = current;
		do {
			temp = fifo_midi_insert(current->val, temp);  
			current = current->next;
		} while (current != start);
	}

	printf("midi_decode: Tri termine.\n");

	// On peut maintenant jouer les notes
	if(temp == NULL) {
		printf("midi_decode: Pas de notes a jouer.\n");
		return;
	}

	temp_start = temp;
	do {

		// Si l'on a plusieurs notes en même temps
		do {
			buffer = fifo_midi_enqueue(temp->val, buffer);
			temp = temp->next;
			event_count++;
			if (temp == temp_start) break;
       	 	time_interval = (temp->next)->val.time - temp->val.time;
		} while(time_interval == 0);

		// Envoi des données
		if(event_count > 0 && buffer != NULL) {
			buffer_start = buffer;
			if(time_interval < 100) { // On évite d'envoyer trop vite pour que le STM puisse gérer. Cela fausse légérement le rythme
				time_interval = 100;
			}
			Sleep(time_interval);
			transmit_size(port, event_count);
			do {
				transmit_event(port, buffer->val.note, ACTUATION_TIME);
				buffer = buffer->next;
			} while(buffer != buffer_start);
		} 

		// Nettoyage du buffer
		event_count = 0;
		fifo_midi_delete(buffer);
		buffer = fifo_midi_new();
	} while(temp != temp_start);

	// Envoi de l'évenement de fin de transmission
	transmit_size(port, 1);
	transmit_event(port, 255, 255);
	printf("midi_decode: Decodage termine.\n");

	// Nettoyage de la mémoire
	fifo_midi_delete(temp_start);
	fifo_midi_delete(temp);
	fifo_midi_delete(buffer);
	fifo_midi_delete(buffer_start);
}

