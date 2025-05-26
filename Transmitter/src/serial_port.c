#include "serial_port.h"
#include <stdint.h>

/**
 * @brief Ouvre et configure un port série.
 *
 * Cette fonction initialise la connexion avec le port série spécifié,
 * configure les paramètres de communication (tels que la vitesse de transmission,
 * le nombre de bits de données, la parité, et les bits d'arrêt), et prépare
 * le port pour l'envoi et la réception de données.
 *
 * @return Un descripteur de fichier du port ouvert en cas de succès, ou une valeur négative en cas d'erreur.
 */
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


/**
 * @brief Écrit les octets contenus dans le buffer sur le port série spécifié.
 *
 * Cette fonction tente d'écrire 'size' octets du buffer sur le port série représenté par 'port'.
 * Elle utilise la fonction WriteFile de l'API Windows pour effectuer l'écriture.
 * En cas d'échec de l'écriture ou si le nombre d'octets écrits est différent de 'size',
 * un message d'erreur est affiché et la fonction retourne 0.
 *
 * @param port  HANDLE du port série sur lequel écrire.
 * @param buffer Pointeur vers le buffer contenant les données à écrire.
 * @param size  Nombre d'octets à écrire depuis le buffer.
 * @return int  1 si l'écriture a réussi, 0 sinon.
 */
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



/**
 * @brief Lit des octets depuis un port série spécifié.
 *
 * Cette fonction tente de lire jusqu'à 'size' octets depuis le port série représenté par 'port'
 * et stocke les données lues dans le tampon 'buffer'. Elle utilise l'API Windows ReadFile pour effectuer la lecture.
 *
 * @param port   HANDLE du port série à partir duquel lire les données.
 * @param buffer Pointeur vers le tampon où les données lues seront stockées.
 * @param size   Nombre maximal d'octets à lire.
 * @return       Le nombre d'octets effectivement lus, ou 0 en cas d'échec de la lecture.
 */
int read_port(HANDLE port, char* buffer, size_t size) {
    DWORD bytesRead;
    if (!ReadFile(port, buffer, (DWORD)size, &bytesRead, NULL)) {
        printf("serial_port: Echec de la lecture du port\n");
        return 0;
    }
    return (int)bytesRead;
}

/**
 * @brief Lit un message depuis le port série et l'affiche à l'écran.
 *
 * Cette fonction lit les données reçues sur le port série spécifié par le handle `port`,
 * puis affiche le message reçu sur la sortie standard. Si aucun message n'est reçu ou
 * en cas d'erreur de lecture, un message d'erreur est affiché.
 *
 * @param port Handle du port série à partir duquel lire le message.
 */
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

/**
 * @brief Transmet la taille des données à envoyer via le port série.
 *
 * Cette fonction calcule la taille totale des données à transmettre en multipliant
 * le nombre d'événements par 2 (chaque événement occupant 2 octets), puis envoie
 * cette taille sur le port série spécifié.
 *
 * @param port        Le handle du port série utilisé pour la transmission.
 * @param event_count Le nombre d'événements à transmettre.
 */
void transmit_size(HANDLE port, int event_count) {
    uint8_t buffer = (uint8_t)(event_count * 2);
    if (write_port(port, (char*)&buffer, 1)) {
        printf("serial_port: Transmission de la taille (%d octets)\n", buffer);
    }
}

/**
 * @brief Transmet un événement via le port série.
 *
 * Cette fonction envoie un événement composé d'une note et d'un temps d'activation
 * sur le port série spécifié. Les données sont envoyées sous forme de deux octets :
 * le premier pour la note, le second pour le temps d'activation.
 *
 * @param port            Le handle du port série à utiliser pour la transmission.
 * @param note            La valeur de la note à transmettre (0-255).
 * @param actuation_time  Le temps d'activation à transmettre (0-255).
 */
void transmit_event(HANDLE port, int note, int actuation_time) {
    uint8_t buffer[2] = { (uint8_t)note, (uint8_t)actuation_time };
    if (write_port(port, (char*)buffer, 2)) {
        printf("serial_port: Transmission d'un evenement : note=%d, temps=%d\n", note, actuation_time);
    }
}
