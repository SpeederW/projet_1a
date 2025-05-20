#include "fifo_midi.h"
#include <stdio.h>
#include <stdlib.h>

// Fichier pris du BE n°10 d'informatique

fifo_midi_t fifo_midi_new(void){
  return NULL;	/* la liste vide est representée par NULL	*/
}

int fifo_midi_is_empty(fifo_midi_t L)
{	return L==NULL;	/* la liste vide est representée par NULL	*/
}

void fifo_midi_print(fifo_midi_t L) {
  if(L == NULL)
    printf("liste vide");
  fifo_midi_t p = L;
  do {
    printf("[");
    printf("%d", p->val.note);
    printf("]");
    p = p->next;
  } while(p != L);
  printf("\n");
}

fifo_midi_t fifo_midi_enqueue(MIDI_Event e, fifo_midi_t f) {
  fifo_midi_t c = f;
  fifo_midi_t new = calloc(1, sizeof(*new));
  new->val = e;
  if(f == NULL) {
    new->next = new;
    return new;
  } else {
    new->next = f->next;
    c->next = new;
  }
  return c; 
}

MIDI_Event fifo_midi_peek(fifo_midi_t f){
  return (f->next)->val;
}

fifo_midi_t fifo_midi_del_head(fifo_midi_t f) {
  assert(!fifo_midi_is_empty(f));
  if(f == f->next) {
    free(f);
    return NULL;
  }
  fifo_midi_t c = f;
  while(c->next != f) {
    c = c->next;
  }
  c->next = f->next;
  free(f);
  return c;
}

MIDI_Event fifo_midi_dequeue(fifo_midi_t* af) {
  MIDI_Event e = fifo_midi_peek(*af);
  *af = fifo_midi_del_head(*af);
  return e;
}

fifo_midi_t fifo_midi_delete(fifo_midi_t head) {
    if (head == NULL) {
        return NULL;
    }

    /* Si la liste ne contient qu'un seul élément */
    if (head->next == head) {
        free(head);
        return fifo_midi_new();  // ou NULL selon la sémantique souhaitée
    }

    /* Parcours et libération des autres nœuds */
    fifo_midi_t current = head->next;
    fifo_midi_t next_node;
    while (current != head) {
        next_node = current->next;
        free(current);
        current = next_node;
    }

    /* Libération de la tête */
    free(head);

    /* Retourne une nouvelle FIFO vide */
    return fifo_midi_new();
}

int fifo_midi_length(fifo_midi_t L) {
  int l=0;
  fifo_midi_t p = L;
  do {
    l++;
    p = p->next;
  } while(p != L);
  return l;
}

fifo_midi_t fifo_midi_insert(MIDI_Event e, fifo_midi_t f) {
    fifo_midi_t new = calloc(1, sizeof(*new));
    new->val = e;

    if (f == NULL) {
        new->next = new;
        return new;
    }

    fifo_midi_t current = f->next;
    fifo_midi_t prev = NULL;

    do {
        if (e.time < current->val.time) break;
        prev = current;
        current = current->next;
    } while (current != f);

    if (prev == NULL) { // Insertion avant le premier
        fifo_midi_t head = f->next;
        f->next = new;
        new->next = head;
        return f;  // new devient la nouvelle tête; la queue ne change pas
    } else { // Insertion au milieu de deux élements ou à la fin
        prev->next = new;
        new->next = current;
        return new;  // la queue change
    }
}