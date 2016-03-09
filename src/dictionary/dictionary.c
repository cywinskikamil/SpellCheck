/** @file
  Prosta implementacja słownika.
  Słownik przechowuje listę słów.

  @ingroup dictionary
  @author Jakub Pawlewicz <pan@mimuw.edu.pl>
  @copyright Uniwerstet Warszawski
  @date 2015-05-11
 */

#include "dictionary.h"
#include "trie.h"
#include "conf.h"
#include <assert.h>
#include <sys/stat.h>
#include <argz.h>
#include <ctype.h>

#define _GNU_SOURCE

/** Pomocnicza zmienna do rozpoznawania "pustych" słów. */
static wchar_t * puste = L"";

/** Struktura przechowujaca tablicę reguł o ustalonym koszcie. */
struct regula 
{ 
  /** Lewa strona reguły. */
  const wchar_t * lewaStrona;

  /** Prawa strona reguły. */
  const wchar_t * prawaStrona;

  /** Reprezentuje flagę przypisaną regule. */
  enum rule_flag flaga;

  /** Koszt podpowiedzi. */
  int koszt;
};

/** Strutktura przechowująca tablicę reguł danego kosztu. */
struct tablica_regul 
{
  /** Liczba reguł o konkretnym koszcie. */
  int liczbaRegulKosztu;

  /** Rozmiar tablicy o danym koszcie. */
  int rozmiarRegulKosztu;

  /** Tablica trzymajaca reguly o danym koszcie. */
  struct regula ** zbiorRegulKosztu;
};

/**
  Struktura przechowująca słownik.
  Implementacja z użyciem drzewa TRIE.
 */
struct dictionary
{
  /** Drzewo zawierające słownik. */
  struct trie * drzewko;
  
  /** Alfabet, zawierający wszystkie do tej pory użyte litery. */
  wchar_t * alfabet;

  /** Liczba liter w alfabecie. */
  int liczbaLiter;

  /** Rozmiar tablicy alfabetu. */
  int rozmiarAlfabetu;

  /** Maksymalny koszt podpowiedzi. */
  int maksymalnyKoszt;

  /** Lista reguł do podpowiedzi */
  struct tablica_regul ** tablicaRegul;

  /** Ogolna liczba reguł w słowniku. */
  int ogolnaLiczbaRegul;
};

/** @name Funkcje pomocnicze
  @{
 */

/** Funkcja inicjalizująca tablicę reguł.
 * 
 * @param tablica Tablica.
 * @return Tablica.
 */
static struct tablica_regul * initalizeTablicaRegul(struct tablica_regul * tablica)
{
  tablica = malloc(sizeof(struct tablica_regul));
  tablica->liczbaRegulKosztu = 0;
  tablica->rozmiarRegulKosztu = 0;
  tablica->zbiorRegulKosztu = NULL;
  return tablica;
}


/** Funkcja poprawiająca tablica_regul, dokladająca nowe reguły.
 * @param [in,out] ileRegulPom Ilość reguł.
 * @param [in] tablica Tablica reguł.
 * @param [in,out] rozmiar Rozmiar tablicy.
 * @param [in] reg Dodawana reguła.
 * @return Zmodyfikowana tablica.
 */
static struct regula ** poprawTabliceRegul (int * ileRegulPom, 
  struct regula ** tablica, int * rozmiar, struct regula * reg)
{
  int nowaDlugosc = *(rozmiar);
  int ileRegul = * ileRegulPom;
  struct regula ** nowaTablica = tablica;
  
  if (ileRegul < nowaDlugosc)
  {
    nowaTablica[ileRegul] = reg;
    ileRegul++;
  }
  else
  {
    if (!nowaDlugosc)
      nowaDlugosc++;    
    nowaDlugosc *= 2;
    nowaTablica = NULL;
    nowaTablica = realloc(tablica, sizeof(struct regula *) * nowaDlugosc);
    nowaTablica[ileRegul] = reg;
    ileRegul++;
  }
  *(rozmiar) = nowaDlugosc;
  *ileRegulPom = ileRegul;
  
  tablica = nowaTablica;
  return tablica;
}

/** Funkcja powiększająca tablicę reguł. 
 * @param staraTablica Stara tablica.
 * @param maksymalnyKoszt Maksymalny koszt.
 * @return Powiększona tablica.
 */
static struct tablica_regul ** powiekszTabliceSpecjalnaRegul 
  (struct tablica_regul ** staraTablica, int maksymalnyKoszt)
{
  struct tablica_regul ** nowaTablica = realloc(staraTablica, sizeof(struct tablica_regul *) * maksymalnyKoszt); 
  return nowaTablica;
}

/** Funkcja powiekszająca dwukrotnie tablicę, do tego wstawia nowy element.
 * @param[in] tablica Powiekszana tablica.
 * @param[in,out] rozmiar Rozmiar tablicy.
 * @param[in] doWlozenia Wkładane słowo.
 * @param[in,out] liczbaSlownikow Ilość słów w słowniku.
 * @return Zmodyfikowana tablica.
 */
static char ** poprawTablice2(char ** tablica, int * rozmiar, char * doWlozenia,
 int *liczbaSlownikow)
{
  int nowaDlugosc = *(rozmiar);
  char ** nowaTablica = tablica;
  int ileSlow = *liczbaSlownikow;

  if (ileSlow < nowaDlugosc)
  {
    nowaTablica[ileSlow] = doWlozenia;
    ileSlow++;
  }
  else
  {
    if (!nowaDlugosc)
      nowaDlugosc++;    
    nowaDlugosc *= 2;
    nowaTablica = NULL;
    nowaTablica = realloc(tablica, sizeof(char *) * (nowaDlugosc + 1));    
    nowaTablica[ileSlow++] = doWlozenia;
  }
  
  *liczbaSlownikow = ileSlow;
  *(rozmiar) = nowaDlugosc;  
  return nowaTablica;
}

/**
  Czyszczenie pamięci słownika
  @param[in,out] dict słownik
 */
static void dictionary_free(struct dictionary *dict)
{
 clean(dict->drzewko);
}

struct dictionary * dictionary_new()
{
  struct dictionary *dict = malloc(sizeof(struct dictionary));
  dict->drzewko = NULL;
  dict->alfabet = NULL;
  dict->liczbaLiter = 0;
  dict->rozmiarAlfabetu = 0;
  dict->maksymalnyKoszt = 0;
  dict->tablicaRegul = NULL;
  dict->ogolnaLiczbaRegul = 0;
  return dict;
}

void dictionary_done(struct dictionary *dict)
{
  dictionary_free(dict);
  free(dict->alfabet);
  free(dict);
}

int dictionary_insert(struct dictionary *dict, const wchar_t *word)
{
    if (dictionary_find(dict, word))
      return 0;
    int dlugosc = wcslen(word);

    for (int i = 0; i < dlugosc; i++)
    {
      if (dict->alfabet == NULL)
      {
        dict->alfabet = malloc(sizeof(wchar_t) * 2);
        dict->alfabet[0] = word[i];
        dict->alfabet[1] = '\0';
        dict->liczbaLiter++;
        dict->rozmiarAlfabetu++;
        continue;
      }
      if (!(czyJest(dict->liczbaLiter, dict->alfabet, word[i])))
      {
        dict->alfabet = poprawAlfabet(&dict->rozmiarAlfabetu, &dict->liczbaLiter,
         dict->alfabet, word[i]);
      }
    }

    dict->drzewko = insert(word, dlugosc, dict->drzewko, 1);
    return 1;
}

int dictionary_delete(struct dictionary *dict, const wchar_t *word)
{
    if (dictionary_find(dict, word))
    {
      dict->drzewko = delete(word, wcslen(word), 0, dict->drzewko);
      return 1;
    }
    return 0;
}

bool dictionary_find(const struct dictionary *dict, const wchar_t* word)
{
  return finder(word, wcslen(word), 0, dict->drzewko);
}

int dictionary_save(const struct dictionary *dict, FILE* stream)
{
  struct regula * reg;
  struct tablica_regul ** tablicaRegul = dict->tablicaRegul;
  fwprintf(stream, L"%d\n", dict->maksymalnyKoszt);
  fwprintf(stream, L"%d\n", dict->ogolnaLiczbaRegul);
  for (int i = 0; i < dict->maksymalnyKoszt; i++)
  {
    if (tablicaRegul[i] != NULL)
    {
    
      for (int j = 0; j < tablicaRegul[i]->liczbaRegulKosztu; j++)
      { 
        reg = tablicaRegul[i]->zbiorRegulKosztu[j];
        const wchar_t * lewa = reg->lewaStrona;
        const wchar_t * prawa = reg->prawaStrona;
        fwprintf(stream, L"%d|%ls|%d|%ls|%d|%d\n", wcslen(lewa), lewa, wcslen(prawa), prawa, reg->koszt, reg->flaga);
      }
    }
  }
  zapis(dict->drzewko, stream, -1);  

  return 0;
}

struct dictionary * dictionary_load(FILE* stream)
{
  struct dictionary * new = dictionary_new();
  int liczbaRegul, pom = 0;

  fwscanf(stream, L"%d", &liczbaRegul);
  if (!pom)
    pom = dictionary_hints_max_cost(new, liczbaRegul);
  fwscanf(stream, L"%d", &liczbaRegul);    
  wchar_t buff = NULL;
  for (int j = 0; j < liczbaRegul; j++)
  {
  
    fwscanf(stream, L"%d", &pom);
    buff = fgetwc(stream);
    wchar_t * lewaStrona;    
    if (!pom)
      lewaStrona = L"";
    else
    {
      lewaStrona = malloc(sizeof(wchar_t) * (pom + 1));
      for (int i = 0; i < pom; i++)
      {
        buff = fgetwc(stream);
        lewaStrona[i] = buff;
      }
      lewaStrona[pom] = L'\0';
    }
    buff = fgetwc(stream);
    fwscanf(stream, L"%d", &pom);
    buff = fgetwc(stream);
    wchar_t * prawaStrona;
    if (!pom)
      prawaStrona = L"";
    else
    {
      prawaStrona = malloc(sizeof(wchar_t) * (pom + 1));
      for (int i = 0; i < pom; i++)
      {
        buff = fgetwc(stream);
        prawaStrona[i] = buff;
      }
      prawaStrona[pom] = L'\0';
    }
    buff = fgetwc(stream);
    fwscanf(stream, L"%d", &pom);
    int koszt = pom;
    buff = fgetwc(stream);
    fwscanf(stream, L"%d", &pom);
    int flaga = pom;
    dictionary_rule_add(new, lewaStrona, prawaStrona, 0, koszt, flaga);  
    buff = fgetwc(stream);
  }  
  
  if (!pom)
    buff = fgetwc(stream);

  new->drzewko = wczyt(stream, &(new->rozmiarAlfabetu), &(new->liczbaLiter), 
    &(new->alfabet));
  return new;
}

/** Pomocniczy komparator potrzebndy do sortowania word_listy. 
 * @param[in] a Pierwsze słowo.
 * @param[in] b Drugie słowo.
 * @return -1 Jeśli pierwsze słowo jest leksykograficznie mniejsze,
 * 0 jeśli są sobie równe, 1 wpp.
 */
static int cmp (const void * a, const void * b)
{
 return wcscoll(*(wchar_t * const *) a, *(wchar_t * const *) b);
}

/** Pomocnicza funkcja pobierająca słowo ze strumienia.
 * @param file Strumień.
 * @param pom Długość słowa.
 * 
 * @return Pobrane słowo.
 */
static char * wezSlowo2(FILE * file, int pom)
{ 
  char buff;
  char * slowo = malloc(pom + 1);
  for (int i = 0; i < pom; i++)
  {
    buff = fgetc(file);    
    slowo[i] = buff;
  }
  slowo[pom] = '\0';
  return slowo;   
}


int dictionary_lang_list(char **list, size_t *list_len)
{
  mkdir(CONF_PATH, S_IRWXU);
  FILE * file;
  if (!(file = fopen(CONF_PATH"/"DICT_LIST, "r")))
  {
    return -1;
  }  
  int pom = 0, pom2 = 0, liczbaSlownikow = 0, rozmiarLancucha = 0, j;
  char ** lancuch = NULL;
  char buff;
  while ((buff = fgetc(file)) != EOF)
  {     
    if (buff == '\n')
      continue;
    if (isdigit(buff))
    {
      ungetc(buff, file);
      j = fscanf(file, "%d", &pom);
      pom2 += pom + 1; 
      char * slowo = wezSlowo2(file, pom);
      lancuch = poprawTablice2(lancuch, &rozmiarLancucha, slowo, &liczbaSlownikow);
    }
  }
  fclose(file);
  j = 0;
  char * listhelp = malloc(pom2 + 1);
  for (int i = 0; i < liczbaSlownikow; i++)
  {
    char * help = lancuch[i];
    int dlugosc2 = strlen(help);
    for (int k = 0; k < dlugosc2; k++)
    {
      listhelp[j++] = help[k];
    }
    free(help);
    listhelp[j++] = '\0';
  }
  free(lancuch);
  *list = listhelp;
  *list_len = pom2;
  return 0;
}

int dictionary_save_lang(const struct dictionary *dict, const char *lang)
{
  mkdir(CONF_PATH, S_IRWXU);
  int lang_len = strlen(lang);
  char buff [strlen(CONF_PATH) + lang_len + 2];
  sprintf(buff, "%s/%s", CONF_PATH, lang);
  FILE * file = fopen(buff, "w");
  if (file == NULL)
    return -1;
  dictionary_save(dict, file);  
  char * langLista = NULL;
  size_t rozmiarLangListy;
  int powiodloSie = dictionary_lang_list(&langLista, &rozmiarLangListy);
  int i = 0;
  if (powiodloSie >= 0)
  {
    char * str = malloc(MAX_LANG_LENGTH);
    while (i < rozmiarLangListy)
    {
      sscanf(langLista + i, "%s %*s", str);   
      if (!strcmp(str, lang))
      {
        free(langLista);
        free(str);
        fclose(file);
        return 0;
      }
      i += strlen(str) + 1;      
    }
    free(str);
  }
  else 
  {
    langLista = NULL;
  }
  if (langLista != NULL)
    free(langLista);
  fclose(file);
  file = fopen(CONF_PATH"/"DICT_LIST, "a");
  if (file == NULL)
  {
    fclose(file);
    return -1;
  }
  fwprintf(file, L"%d%s\n", lang_len, lang);
  fclose(file);
  return 0;
}

struct dictionary * dictionary_load_lang(const char *lang)
{
  mkdir(CONF_PATH, S_IRWXU);
  int lang_len = strlen(lang);
  char buff [strlen(CONF_PATH) + lang_len + 2];
  sprintf(buff, "%s/%s", CONF_PATH, lang);
  FILE * file = fopen(buff, "r");
  if (file == NULL)
    return NULL;
  struct dictionary * hehe = dictionary_load(file);  
  fclose(file);
  return hehe;
}

int dictionary_hints_max_cost(struct dictionary *dict, int new_cost)
{
  int pom = dict->maksymalnyKoszt;
  dict->maksymalnyKoszt = new_cost;
  if (pom < new_cost)
    dict->tablicaRegul = powiekszTabliceSpecjalnaRegul(dict->tablicaRegul, (new_cost +1));
  for (int i = pom; i < new_cost;i++)
    dict->tablicaRegul[i] = NULL;
  return pom;
}

struct regula * newRegula(const wchar_t * left, const wchar_t * right, 
  int cost, enum rule_flag flag)
{
  struct regula * reg = malloc(sizeof(struct regula));
  reg->lewaStrona = left;
  reg->prawaStrona = right;
  reg->koszt = cost;
  reg->flaga = flag;
  return reg;
}

static bool czyZmiennych (const wchar_t * left, const wchar_t * right)
{
  bool T[10];
  int dlugosc1 = wcslen(left);
  int dlugosc2 = wcslen(right);
  for (int i = 0; i < 10; i++)
    T[i] = false;

  for (int i = 0; i < dlugosc1; i++)
  {
    if (isdigit(left[i]))
    {
      int pom = left[i] - '0';
      T[pom] = true;
      fwprintf(stderr, L"%d asd \n", pom);
    }
  }
  bool zaDuzo = false;
  for (int j = 0; j < dlugosc2; j++)
  {
    if (isdigit(right[j]))
    {
      int pom = right[j] - '0';
      if (!T[pom])
      {
        if (zaDuzo)
        {
          return false;
        }
        zaDuzo = true;
        T[pom] = true;
      }
    }
  }
  return true;
}

int dictionary_rule_add(struct dictionary *dict, const wchar_t *left,
 const wchar_t *right, bool bidirectional, int cost, enum rule_flag flag)
{
  if (!wcscoll(left, puste) && !wcscoll(left,right))
    return -1;    
  
  int licznik = 0;
  struct tablica_regul ** tablicaRegul = dict->tablicaRegul;  
  struct regula * reg;
  bool ho = czyZmiennych(left, right);
  
  if (ho)
  {
    reg = newRegula(left, right, cost, flag);
    
    if (tablicaRegul[cost] == NULL)
      tablicaRegul[cost] = initalizeTablicaRegul(tablicaRegul[cost]);
    tablicaRegul[cost]->zbiorRegulKosztu = poprawTabliceRegul
      (&(tablicaRegul[cost]->liczbaRegulKosztu), tablicaRegul[cost]->zbiorRegulKosztu,
       &(tablicaRegul[cost]->rozmiarRegulKosztu), reg);
    if (tablicaRegul[cost]->zbiorRegulKosztu == NULL)
    {
      free(reg);
      return -1;
    }
    licznik++; 
  }
  if (bidirectional)
  {
    ho = czyZmiennych(right, left);
    if (ho)
    {
      reg = newRegula(right, left, cost, flag);
      if (tablicaRegul[cost] == NULL)
      tablicaRegul[cost] = initalizeTablicaRegul(tablicaRegul[cost]);
      tablicaRegul[cost]->zbiorRegulKosztu = poprawTabliceRegul(&(tablicaRegul[cost]
        ->liczbaRegulKosztu), tablicaRegul[cost]->zbiorRegulKosztu, &(tablicaRegul[cost]->
          rozmiarRegulKosztu), reg);
      if (tablicaRegul[cost]->zbiorRegulKosztu == NULL)
      {
        free(reg);
        return -1;  
      }
      licznik++;
    }
  }
  dict->ogolnaLiczbaRegul += licznik;
  return licznik;
}

void dictionary_rule_clear(struct dictionary *dict)
{
  struct tablica_regul ** zbior = dict->tablicaRegul;
  for (int i = 0; i < dict->maksymalnyKoszt; i++)
  {
    struct tablica_regul * pom = zbior[i];
    if (pom != NULL)
    {
      for (int j = 0; j < pom->rozmiarRegulKosztu; j++)
          free(pom->zbiorRegulKosztu[j]);
    free(pom);
    }
  }
  free(zbior);
}

void dictionary_hints(const struct dictionary *dict, const wchar_t* word,
        struct word_list *list)
{
  // int liczbaHintow = 0;
  // wchar_t ** hinty = hints(dict->drzewko, word, dict->alfabet, dict->liczbaLiter,
  //  &liczbaHintow);
  // if (hinty == NULL)
  // {
  //   word_list_init(list);
  //   list->size = 0;
  //   list->buffer_size = 0;
  //   free(list->array);
  //   list->array = NULL;
  //   return;
  // }
  word_list_init(list);
  dictionary_hints_max_cost((struct dictionary *) dict, 5);
  dictionary_rule_add((struct dictionary *) dict, L"012", L"0123", 1, 1, 1);
  fwprintf(stderr, L"liczba regul %d\n", dict->ogolnaLiczbaRegul);
  // qsort(hinty, liczbaHintow, sizeof(wchar_t *), cmp);
  // int i = 0;
  // while (i < liczbaHintow)
  // {
  //   int j = i;
  //   word_list_add(list, hinty[i++]);
  //   while (i < liczbaHintow && !wcscoll(hinty[j],hinty[i]))
  //   {
  //     free(hinty[i++]);
  //   }
  // }
  // free(hinty);
  
  // pokazStany((wchar_t *) word, dict->drzewko);
}

/**@}*/
