#include <stdio.h>
#include "serial_port.h"

// Créer et configure une liaison série avec le STM32.
HANDLE open_port() {
    HANDLE port = CreateFileA(STM_COM_PORT, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(port == INVALID_HANDLE_VALUE) { // Echec de l'ouverture du port
        printf("Fatal error");
        return 0;
    }

    // Nettoyage du port
    BOOL success = FlushFileBuffers(port);
    if (!success) {
      printf("Echec du nettoyage du port série");
      CloseHandle(port);
      return INVALID_HANDLE_VALUE;
    }

    // Configuration du port 
    COMMTIMEOUTS timeouts;
    timeouts.WriteTotalTimeoutConstant=50;
    timeouts.WriteTotalTimeoutMultiplier=10;
    if(!SetCommTimeouts(port, &timeouts)) {
        printf("Echec de la configuration du port série");
    }

    DCB serial_state;
    serial_state.BaudRate = 115200; // Voir config STM
    serial_state.ByteSize = 8;
    serial_state.Parity = NOPARITY;
    serial_state.StopBits = ONESTOPBIT;
    if (!SetCommState(port, &serial_state)) {
      printf("Echec de la configuration du port série");
      CloseHandle(port);
      return INVALID_HANDLE_VALUE;
    }

    printf("Port série configuré avec succès.");
    return port;
}

int write_port(HANDLE port, char* buffer, size_t size) {
  DWORD written;
  if (!WriteFile(port, buffer, size, &written, NULL)) {
    printf("Echec de l'écriture au port");
    return 0;
  }
  if (written != size) {
    printf("Echec de l'écriture au port");
    return 0;
  }
  return 1;
}

int read_port(HANDLE port, char* buffer, size_t size) {
  DWORD bytesRead;
  if (!ReadFile(port, buffer, size, &bytesRead, NULL)) {
    printf("Echec de la lecture du port");
    return 0;
  }
  return bytesRead;
}

void print_message(HANDLE port) {
  char buffer[256]; // Adjust size as needed
  int bytesRead = read_port(port, buffer, sizeof(buffer) - 1);
  if (bytesRead > 0) {
    buffer[bytesRead] = '\0'; // Null-terminate the string
    printf("Message received: %s\n", buffer);
  } else {
    printf("No message received or error occurred.\n");
  }
}

void transmit_size(HANDLE port, int event_count) {
    char* buffer;
    *buffer = event_count*2;
    write_port(port, buffer, sizeof(buffer));
}

void transmit_event(HANDLE port, int note, int actuation_time) {
    char buffer[2];
    buffer[0] = note;
    buffer[1] = actuation_time;
    write_port(port, buffer, sizeof(buffer));
}
