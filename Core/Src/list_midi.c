// Fichier pris du BE n°10 d'informatique
// Fichier list_midi.c
#include "list_midi.h"
#define NIL NULL

list_midi_t list_midi_new() {
  return NIL;
}

int list_midi_is_empty( list_midi_t l ) {
  return NIL == l;
}

// Precondition : list_midie non vide
MIDI_Event list_midi_first(list_midi_t l){
  assert(!list_midi_is_empty(l));
  return l->val;
}

list_midi_t list_midi_add_first( MIDI_Event e, list_midi_t l ) {
  list_midi_t p = calloc( 1, sizeof( *p ) );
  if ( NULL == p ) {
    fprintf( stderr, "Fatal: Unable to allocate new list_midi link.\n" );
    return l;
  }
  p->val  = e;
  p->next = l;
  return p;
}

// Precondition : list_midie non vide
list_midi_t list_midi_del_first( list_midi_t l ) {
  assert(!list_midi_is_empty(l));

  list_midi_t p = l->next;
  free( l );
  return p;
}

void list_midi_print(list_midi_t l) {
  list_midi_t p;
  printf("(");
  for ( p=l; ! list_midi_is_empty(p); p = p->next) {
    printf( "[" );
    midi_print( p->val );
    printf( "]" );
  }
  printf(")\n");
}

int list_midi_length(list_midi_t l) {
  int len = 0;
  list_midi_t p;
  for( p=l; ! list_midi_is_empty(p) ; p=p->next ) {
    len ++;
  }
  return len;
}


list_midi_t list_midi_delete(list_midi_t l) {
  while(!list_midi_is_empty(l)) {
	  list_midi_del_first(l);
  }
  return NIL;
}
