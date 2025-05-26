#include "fifo_midi.h"
#include <stdio.h>
#include <stdlib.h>

// Fichier pris du BE n°10 d'informatique

fifo_midi_t fifo_midi_new(void){
  return NULL;	/* la liste vide est representée par NULL	*/
}

int fifo_midi_is_empty(fifo_midi_t L) {
  	return L==NULL;	/* la liste vide est representée par NULL	*/
}

/**
 * @brief Affiche le contenu de la file MIDI circulaire.
 *
 * Cette fonction parcourt la liste chaînée circulaire de type fifo_midi_t et affiche la valeur de la note
 * contenue dans chaque élément de la file. Si la liste est vide (L == NULL), un message "liste vide" est affiché.
 *
 * @param L Pointeur vers le début de la file MIDI à afficher.
 */
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

/**
 * @brief Ajoute un événement MIDI à la file FIFO circulaire.
 *
 * Cette fonction insère un nouvel événement MIDI à la suite de la tête de la file FIFO.
 * Si la file est vide (f == NULL), elle crée une nouvelle file circulaire contenant un seul élément.
 * Sinon, elle insère le nouvel élément juste après la tête et met à jour les pointeurs en conséquence.
 *
 * @param e L'événement MIDI à ajouter à la file.
 * @param f La file FIFO MIDI existante (peut être NULL).
 * @return La nouvelle tête de la file FIFO après l'ajout.
 */
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

/**
 * @brief Permet d'obtenir, sans le retirer, le prochain événement MIDI dans la file FIFO.
 *
 * Cette fonction retourne la valeur du prochain événement MIDI qui sera lu dans la file FIFO,
 * sans le retirer de la file. Elle permet ainsi de consulter l'événement en tête de file.
 *
 * @param f Pointeur vers la structure de la file FIFO MIDI.
 * @return L'événement MIDI situé en tête de la file FIFO.
 */
MIDI_Event fifo_midi_peek(fifo_midi_t f){
  return (f->next)->val;
}

/**
 * @brief Supprime la tête de la file FIFO MIDI circulaire.
 *
 * Cette fonction supprime l'élément en tête de la file FIFO MIDI circulaire pointée par `f`.
 * Si la file ne contient qu'un seul élément, celui-ci est libéré et la fonction retourne NULL.
 * Sinon, la tête est supprimée, la mémoire correspondante est libérée, et la fonction retourne
 * un pointeur vers le nouvel élément de fin de la file (celui précédant la tête supprimée).
 *
 * @param f Pointeur vers la tête de la file FIFO MIDI.
 * @return fifo_midi_t Pointeur vers le nouvel élément de fin de la file, ou NULL si la file est vide après suppression.
 * @pre La file ne doit pas être vide (assertion).
 */
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

/**
 * @brief Retire et retourne le premier événement MIDI de la file FIFO.
 *
 * Cette fonction extrait l'événement MIDI en tête de la file FIFO pointée par 'af',
 * puis met à jour la file pour supprimer cet élément. L'événement retiré est retourné.
 *
 * @param af Pointeur vers la file FIFO de type fifo_midi_t.
 * @return L'événement MIDI (MIDI_Event) qui était en tête de la file.
 */
MIDI_Event fifo_midi_dequeue(fifo_midi_t* af) {
  MIDI_Event e = fifo_midi_peek(*af);
  *af = fifo_midi_del_head(*af);
  return e;
}

/**
 * @brief Supprime et libère toute la liste FIFO MIDI circulaire.
 *
 * Cette fonction parcourt et libère tous les nœuds de la liste FIFO MIDI passée en paramètre.
 * Si la liste ne contient qu'un seul élément, celui-ci est libéré.
 * Après suppression, une nouvelle FIFO vide est créée et retournée (ou NULL selon la sémantique souhaitée).
 *
 * @param head Pointeur vers la tête de la liste FIFO MIDI à supprimer.
 * @return Un pointeur vers une nouvelle FIFO vide, ou NULL si la création échoue.
 */
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

/**
 * @brief Calcule la longueur de la liste chaînée circulaire MIDI.
 *
 * Cette fonction parcourt la liste chaînée circulaire pointée par L et compte le nombre d'éléments présents.
 * Elle retourne la longueur totale de la liste.
 *
 * @param L Pointeur vers le premier élément de la liste chaînée circulaire MIDI.
 * @return int Nombre d'éléments dans la liste.
 */
int fifo_midi_length(fifo_midi_t L) {
  int l=0;
  fifo_midi_t p = L;
  do {
    l++;
    p = p->next;
  } while(p != L);
  return l;
}

/**
 * @brief Insère un événement MIDI dans une file FIFO triée par temps.
 *
 * Cette fonction insère un nouvel événement MIDI dans une file circulaire triée
 * selon le champ 'time' de l'événement. Si la file est vide, le nouvel élément devient
 * le seul élément de la file. Sinon, l'événement est inséré à la bonne position pour
 * maintenir l'ordre croissant des temps.
 *
 * @param e L'événement MIDI à insérer.
 * @param f La file FIFO existante (peut être NULL si la file est vide).
 * @return La nouvelle file FIFO après insertion. Si l'insertion a lieu avant le premier
 *         élément, la tête ne change pas. Si l'insertion a lieu à la fin ou au milieu,
 *         la nouvelle queue est retournée.
 */
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