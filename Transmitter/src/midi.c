#include "midi.h"

unsigned int read_vlq(FILE *file) {
    unsigned int value = 0;
    unsigned char byte;
    do {
        if (fread(&byte, 1, 1, file) != 1) break;
        value = (value << 7) | (byte & 0x7F);
    } while (byte & 0x80);
    return value;
}

unsigned short read_ushort(FILE *file) {
    unsigned char bytes[2];
    fread(bytes, 1, 2, file);
    return (bytes[0] << 8) | bytes[1];
}

unsigned int read_uint(FILE *file) {
    unsigned char bytes[4];
    fread(bytes, 1, 4, file);
    return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
}

const char* note_name(int note, char* buffer) {
    const char* names[] = {"DO", "DO#", "RE", "RE#", "MI", "FA", "FA#", "SOL", "SOL#", "LA", "LA#", "SI"};
    int octave = (note / 12) - 1;
    snprintf(buffer, 8, "%s,%d", names[note % 12], octave);
    return buffer;
}

fifo_midi_t process_track(FILE *file, unsigned short division, int track_number) {
    unsigned int tempo = 500000;
    double ticks_per_quarter = division;
    double time_ms = 0;
    unsigned char byte, status = 0;
    unsigned int chunk_size = read_uint(file);
    long track_end = ftell(file) + chunk_size;
    fifo_midi_t event_queue = fifo_midi_new();

    while (ftell(file) < track_end) {
        unsigned int delta_time = read_vlq(file);
        double delta_ms = ((double)delta_time / ticks_per_quarter) * ((double)tempo / 1000.0);
        time_ms += delta_ms;

        if (fread(&byte, 1, 1, file) != 1) break;

        if (byte == 0xFF) {
            unsigned char type;
            if (fread(&type, 1, 1, file) != 1) break;
            unsigned int length = read_vlq(file);

            if (type == 0x51 && length == 3) {
                unsigned char tempo_bytes[3];
                if (fread(tempo_bytes, 1, 3, file) != 3) break;
                tempo = (tempo_bytes[0] << 16) | (tempo_bytes[1] << 8) | tempo_bytes[2];
            } else {
                fseek(file, length, SEEK_CUR);
            }
        } else if (byte == 0xF0 || byte == 0xF7) {
            unsigned int length = read_vlq(file);
            fseek(file, length, SEEK_CUR);
        } else {
            if (byte & 0x80) {
                status = byte;
                if (fread(&byte, 1, 1, file) != 1) break;
            }

            unsigned char data1 = byte;
            unsigned char data2 = 0;

            int status_type = status & 0xF0;
            int channel = status & 0x0F;

            if (status_type != 0xC0 && status_type != 0xD0) {
                if (fread(&data2, 1, 1, file) != 1) break;
            }

            char name[8];
            note_name(data1, name);
            if (status_type == 0x90 && data2 != 0) {
                printf("[Piste %d][Canal %d] Note ON  - Note: %3d (%s)  Force: %3d  Temps: %.2f ms\n", track_number, channel, data1, name, data2, time_ms);
                MIDI_Event event = {time_ms, data1, data2};
                event_queue = fifo_midi_enqueue(event, event_queue);
            } else if (status_type == 0x80 || (status_type == 0x90 && data2 == 0)) {
                printf("[Piste %d][Canal %d] Note OFF - Note: %3d (%s)  Temps: %.2f ms\n", track_number, channel, data1, name, time_ms);
            } else if (status_type == 0xC0) {
                printf("[Piste %d][Canal %d] Program Change - Program: %3d  Temps: %.2f ms\n", track_number, channel, data1, time_ms);
            } else {
            	// TODO handle error
                printf("[Piste %d][Canal %d] Evenement inconnu - Statut: 0x%02X, Data1: 0x%02X, Data2: 0x%02X\n", track_number, channel, status, data1, data2);
            }
        }
    }

    return event_queue;
}

int main() {
    HANDLE port = open_port();

    while(1) {

        char path[256];
        printf("Entrez le chemin du fichier MIDI (ou 'exit' pour quitter) : ");
        scanf("%s", path);
        if (strcmp(path, "exit") == 0) {
            break;
        }

        FILE *file = fopen(path, "rb");
        if (!file) {
            perror("Erreur d'ouverture du fichier");
            return 1;
        }

        char chunk_id[5] = {0};
        fread(chunk_id, 1, 4, file);
        if (strncmp(chunk_id, "MThd", 4) != 0) {
            fprintf(stderr, "Ce n'est pas un fichier MIDI valide\n");
            fclose(file);
            return 1;
        }

        fseek(file, 4, SEEK_CUR);
        unsigned short format = read_ushort(file);
        unsigned short num_tracks = read_ushort(file);
        unsigned short division = read_ushort(file);
        fifo_midi_t* tracks = calloc(num_tracks, sizeof(fifo_midi_t));
        if (!tracks) {
            fprintf(stderr, "Erreur d'allocation de mémoire\n");
            fclose(file);
            return 1;
        }
    
        for (int i = 0; i < num_tracks; ++i) {
            if (fread(chunk_id, 1, 4, file) != 4 || strncmp(chunk_id, "MTrk", 4) != 0) {
                fprintf(stderr, "Erreur de lecture de la piste %d\n", i);
                break;
            }
            printf("--- Lecture de la piste %d ---\n", i);
            tracks[i] = process_track(file, division, i);
            if(tracks[i] == NULL) {
                printf("Piste %d vide\n", i);
            } else {
                printf("Piste %d non vide\n", i);
            }
        }

        midi_decode(port, tracks, num_tracks);
        
        printf("Fin de la lecture du fichier MIDI\n");
        
        fclose(file);
    }
    CloseHandle(port);
    return 0;
}