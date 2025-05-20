#ifndef TRANSMIT_H
#define TRANSMIT_H

#include <stdio.h>
#include <windows.h>

#define STM_COM_PORT TEXT("COM6")

HANDLE open_port();
void transmit_event(HANDLE port, int note, int actuation_time);
void transmit_size(HANDLE port, int data_count);
void print_message(HANDLE port);
int write_port(HANDLE port, char* buffer, size_t size);
int read_port(HANDLE port, char* buffer, size_t size);

#endif