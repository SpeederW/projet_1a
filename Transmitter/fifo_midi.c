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
    midi_print(p->val);
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
  midi_t e = fifo_midi_peek(*af);
  *af = fifo_midi_del_head(*af);
  return e;
}

fifo_midi_t fifo_midi_delete(fifo_midi_t f) {
  fifo_midi_t p = f->next;
  fifo_midi_t tete = f;
  while (p->next != tete) {
    p = p->next;
    free(f);
    f = p;
  }
  free(tete);
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
