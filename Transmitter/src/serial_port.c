#include "serial_port.h"
#include <stdint.h>

// Créer et configure une liaison série avec le STM32.
HANDLE open_port() {
    HANDLE port = CreateFileA(TEXT("COM6"), GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(port == INVALID_HANDLE_VALUE) { // Echec de l'ouverture du port
        printf("open_port: Fatal error");
        return 0;
    }

    // Nettoyage du port
    if (!FlushFileBuffers(port)) {
      printf("serial_port: Echec du nettoyage du port serie");
      CloseHandle(port);
      return INVALID_HANDLE_VALUE;
    }

    // Configuration du port 
    COMMTIMEOUTS timeouts;
    timeouts.WriteTotalTimeoutConstant=50;
    timeouts.WriteTotalTimeoutMultiplier=10;
    if(!SetCommTimeouts(port, &timeouts)) {
        printf("serial_port: Echec de la configuration du port serie");
    }

    DCB serial_state;
    serial_state.BaudRate = 9600; // Voir config STM
    serial_state.ByteSize = 8;
    serial_state.Parity = NOPARITY;
    serial_state.StopBits = ONESTOPBIT;
    if (!SetCommState(port, &serial_state)) {
      printf("serial_port: Echec de la configuration du port serie");
      CloseHandle(port);
      return INVALID_HANDLE_VALUE;
    }

    printf("serial_port: Port serie configure avec succes.\n");
    return port;
}

int write_port(HANDLE port, char* buffer, size_t size) {
  DWORD written;
  if (!WriteFile(port, buffer, size, &written, NULL)) {
    printf("serial_port: Echec de l'ecriture au port (1)");
    return 0;
  }
  if (written != size) {
    printf("serial_port: Echec de l'ecriture au port (2)");
    return 0;
  }
  return 1;
}

int read_port(HANDLE port, char* buffer, size_t size) {
  DWORD bytesRead;
  if (!ReadFile(port, buffer, size, &bytesRead, NULL)) {
    printf("serial_port: Echec de la lecture du port");
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
    printf("serial_port: No message received or error occurred.\n");
  }
}

void transmit_size(HANDLE port, int event_count) {
    char buffer = event_count*2;
    write_port(port, (char*) &buffer, 1);
    printf("serial_port: Transmission de la taille, %d octets\n", event_count*2);
}

void transmit_event(HANDLE port, int note, int actuation_time) {
    uint8_t buffer[2] = {note, actuation_time};
    write_port(port, (char*) buffer, 2);
    printf("serial_port: Transmission des notes, %d octets\n", 2);
}
