#ifndef _list_midi_H
#define _list_midi_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "midi.h"

// Fichier pris du BE n°10 d'informatique

// 1. Définition des types maillon (link_t) et liste (list_midi_t)
typedef struct _link_midi {
  MIDI_Event val; /* un élément de la liste*/
  struct _link_midi *next; /* l'adresse du maillon suivant */
} link_midi_t, *list_midi_t;

// 2. Protoype des fonctions realisant les opérations essentielles du type abstrait Liste

// Crée une liste vide
list_midi_t list_midi_new() ;

// Retourne VRAI si l est une liste vide
int list_midi_is_empty(list_midi_t l);

// Retourne l'élément en tête de liste
// PRECONDITION : liste non vide
MIDI_Event list_midi_first(list_midi_t l);

// Retourne le reste de la liste
// PRECONDITION : liste non vide
list_midi_t list_midi_next(list_midi_t l);

// Ajoute l'élément e en tête de la liste et retourne la nouvelle liste
list_midi_t list_midi_add_first(midi_t e, list_midi_t l);

// Supprime le maillon en tête de liste et retourne la nouvelle liste
// PRECONDITION : liste non vide
list_midi_t list_midi_del_first(list_midi_t l);

// Retourne le nombre d'éléments (ou de maillons) de la liste
int list_midi_length(list_midi_t l);

// Affiche la liste
void list_midi_print(list_midi_t l);

// Libère toute la liste et retourne une liste vide
list_midi_t list_midi_delete(list_midi_t l);


#endif
