/** @file
  Implementacja listy słów.

  @ingroup dictionary
  @author Jakub Pawlewicz <pan@mimuw.edu.pl>
  @copyright Uniwerstet Warszawski
  @date 2015-05-10
 */

#include "word_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

/** @name Elementy interfejsu
   @{
 */

void word_list_init(struct word_list *list)
{
    list->size = 0;
    list->buffer_size = 0;
    list->array = NULL; //malloc(sizeof(wchar_t *));
}

void word_list_done(struct word_list *list)
{
  wchar_t ** pom = list->array;  
  for (int i = 0; i < list->size;i++)
  {
      free(pom[i]);
  }
  free(pom);
}

static struct word_list * resize (struct word_list *list2)
{
  struct word_list * list = list2;
  if (!list->buffer_size)
    list-> buffer_size++;
  wchar_t ** current = list->array;
  wchar_t ** newArray = realloc(current, sizeof(wchar_t *) * (list->buffer_size * 2));
  current = newArray;
  list->buffer_size *= 2;
  list->array = current;
  return list;
}

int word_list_add(struct word_list *list, wchar_t *word)
{
    if (list->size >= list->buffer_size)
      list = resize(list);
    list->array[list->size++] = word;
    return 1;
}

/**@}*/
