#ifndef TRANSMIT_H
#define TRANSMIT_H

#include <windows.h>

#define STM_COM_PORT TEXT("COM10")

HANDLE open_port();
void transmit_note(HANDLE port);
void transmit_size(HANDLE port, int data_count);
int write_port(HANDLE port, char* buffer, size_t size);

#endif