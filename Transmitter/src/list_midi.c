// Fichier pris du BE n°10 d'informatique
// Fichier list_midi.c
#include "list_midi.h"
#define NIL NULL

/**
 * @brief Crée et retourne une nouvelle liste MIDI vide.
 *
 * Cette fonction initialise une nouvelle liste MIDI en retournant la valeur NIL,
 * qui représente une liste vide.
 *
 * @return list_midi_t Une nouvelle liste MIDI vide.
 */
list_midi_t list_midi_new() {
  return NIL;
}

/**
 * @brief Vérifie si la liste MIDI est vide.
 *
 * Cette fonction retourne 1 (vrai) si la liste MIDI passée en paramètre est vide (égale à NIL),
 * sinon elle retourne 0 (faux).
 *
 * @param l La liste MIDI à tester.
 * @return int 1 si la liste est vide, 0 sinon.
 */
int list_midi_is_empty( list_midi_t l ) {
  return NIL == l;
}

/**
 * @brief Retourne le premier événement MIDI de la liste.
 *
 * Cette fonction suppose que la liste n'est pas vide. 
 * Elle retourne la valeur du premier élément de la liste MIDI.
 *
 * @param l La liste MIDI à examiner.
 * @return Le premier événement MIDI de la liste.
 * @pre La liste ne doit pas être vide.
 */
MIDI_Event list_midi_first(list_midi_t l){
  assert(!list_midi_is_empty(l));
  return l->val;
}

/**
 * @brief Ajoute un nouvel élément en tête d'une liste chaînée de type list_midi_t.
 *
 * Cette fonction alloue dynamiquement un nouveau maillon, y stocke l'événement MIDI passé en paramètre,
 * puis l'insère en tête de la liste existante. Si l'allocation échoue, la liste d'origine est retournée
 * sans modification et un message d'erreur est affiché sur stderr.
 *
 * @param e L'événement MIDI à ajouter en tête de liste.
 * @param l La liste chaînée existante à laquelle ajouter l'élément.
 * @return La nouvelle tête de la liste chaînée, ou la liste d'origine en cas d'échec d'allocation.
 */
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

/**
 * @brief Supprime le premier élément d'une liste MIDI chaînée.
 *
 * Cette fonction libère la mémoire occupée par le premier élément de la liste
 * MIDI passée en paramètre et retourne un pointeur vers le nouvel élément
 * en tête de liste (c'est-à-dire l'élément suivant).
 *
 * @param l Pointeur vers la tête de la liste MIDI (doit être non vide).
 * @return list_midi_t Pointeur vers la nouvelle tête de la liste MIDI.
 *
 * @pre La liste ne doit pas être vide (assertion vérifiée).
 * @post Le premier élément de la liste est supprimé et libéré.
 */
list_midi_t list_midi_del_first( list_midi_t l ) {
  assert(!list_midi_is_empty(l));

  list_midi_t p = l->next;
  free( l );
  return p;
}

/**
 * @brief Affiche le contenu d'une liste chaînée de notes MIDI sur la sortie standard.
 *
 * Cette fonction parcourt la liste chaînée de type list_midi_t passée en paramètre
 * et affiche chaque note contenue dans la liste entre crochets, le tout entouré de parenthèses.
 * Exemple de sortie : ([60][62][64])
 *
 * @param l La liste chaînée de notes MIDI à afficher.
 */
void list_midi_print(list_midi_t l) {
  list_midi_t p;
  printf("(");
  for ( p=l; ! list_midi_is_empty(p); p = p->next) {
    printf( "[" );
    printf("%d", p->val.note );
    printf( "]" );
  }
  printf(")\n");
}

/**
 * @brief Calcule la longueur d'une liste chaînée de type list_midi_t.
 *
 * Cette fonction parcourt la liste chaînée passée en paramètre et compte le nombre d'éléments qu'elle contient.
 *
 * @param l La liste MIDI à parcourir.
 * @return Le nombre d'éléments dans la liste.
 */
int list_midi_length(list_midi_t l) {
  int len = 0;
  list_midi_t p;
  for( p=l; ! list_midi_is_empty(p) ; p=p->next ) {
    len ++;
  }
  return len;
}

/**
 * @brief Supprime tous les éléments de la liste MIDI.
 *
 * Cette fonction parcourt la liste MIDI passée en paramètre et supprime
 * chaque élément un par un jusqu'à ce que la liste soit vide. 
 * À la fin de l'opération, la fonction retourne NIL pour indiquer que 
 * la liste est désormais vide.
 *
 * @param l La liste MIDI à supprimer.
 * @return NIL, indiquant que la liste est vide.
 */
list_midi_t list_midi_delete(list_midi_t l) {
  while(!list_midi_is_empty(l)) {
	  list_midi_del_first(l);
  }
  return NIL;
}
