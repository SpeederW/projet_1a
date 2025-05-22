#include "serial_port.h"
#include <stdint.h>


HANDLE open_port() {
    HANDLE port = CreateFileA("COM4", GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (port == INVALID_HANDLE_VALUE) {
        printf("open_port: Erreur fatale à l'ouverture du port COM\n");
        return INVALID_HANDLE_VALUE;
    }

    // Nettoyage du port
    if (!FlushFileBuffers(port)) {
        printf("serial_port: Echec du nettoyage du port série\n");
        CloseHandle(port);
        return INVALID_HANDLE_VALUE;
    }

    // Configuration des timeouts
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 100;
    timeouts.WriteTotalTimeoutMultiplier = 0;

    if (!SetCommTimeouts(port, &timeouts)) {
        printf("serial_port: Echec de la configuration des timeouts\n");
        CloseHandle(port);
        return INVALID_HANDLE_VALUE;
    }

    // Configuration de l'état du port série
    DCB serial_state = { 0 };
    serial_state.DCBlength = sizeof(DCB);

    if (!GetCommState(port, &serial_state)) {
        printf("serial_port: Echec de la récupération de l'état initial du port\n");
        CloseHandle(port);
        return INVALID_HANDLE_VALUE;
    }

    serial_state.BaudRate = 115200;
    serial_state.ByteSize = 8;
    serial_state.Parity = NOPARITY;
    serial_state.StopBits = ONESTOPBIT;
    serial_state.fDtrControl = DTR_CONTROL_ENABLE;
    serial_state.fRtsControl = RTS_CONTROL_ENABLE;

    if (!SetCommState(port, &serial_state)) {
        printf("serial_port: Echec de la configuration de l'état du port\n");
        CloseHandle(port);
        return INVALID_HANDLE_VALUE;
    }

    printf("serial_port: Port serie configure avec succes.\n");
    return port;
}

int write_port(HANDLE port, char* buffer, size_t size) {
    DWORD written;
    if (!WriteFile(port, buffer, (DWORD)size, &written, NULL)) {
        printf("serial_port: Echec de l'ecriture sur le port (1)\n");
        return 0;
    }
    if (written != size) {
        printf("serial_port: Echec de l'écriture sur le port (2) - %lu/%zu octets envoyés\n", written, size);
        return 0;
    }

    return 1;
}

int read_port(HANDLE port, char* buffer, size_t size) {
    DWORD bytesRead;
    if (!ReadFile(port, buffer, (DWORD)size, &bytesRead, NULL)) {
        printf("serial_port: Echec de la lecture du port\n");
        return 0;
    }
    return (int)bytesRead;
}

void print_message(HANDLE port) {
    char buffer[256];
    int bytesRead = read_port(port, buffer, sizeof(buffer) - 1);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        printf("Message reçu : %s\n", buffer);
    } else {
        printf("serial_port: Aucun message recu ou erreur.\n");
    }
}

void transmit_size(HANDLE port, int event_count) {
    uint8_t buffer = (uint8_t)(event_count * 2);
    if (write_port(port, (char*)&buffer, 1)) {
        printf("serial_port: Transmission de la taille (%d octets)\n", buffer);
    }
}

void transmit_event(HANDLE port, int note, int actuation_time) {
    uint8_t buffer[2] = { (uint8_t)note, (uint8_t)actuation_time };
    if (write_port(port, (char*)buffer, 2)) {
        printf("serial_port: Transmission d'un evenement : note=%d, temps=%d\n", note, actuation_time);
    }
}
