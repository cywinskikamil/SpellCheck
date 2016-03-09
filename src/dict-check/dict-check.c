/** @defgroup dict-check Moduł dict-check
    Program sprawdający obecność słów w słowniku.
    Wyświetla podpowiedzi.
  */

/** @file
    Główny plik modułu dict-check
    @ingroup dict-check
  */

#include "trie.h"

#include "dictionary.h"
#include <string.h>
#include <stdio.h>

/** Maksymalna długość nazwy pliku.
    Maksymalna długość nazwy pliku bez kończącego znaku '\0'
  */
#define MAX_WORD_LENGTH 100

/** Funkcja tworząca słowo zawierające tylko małe litery.
 * 
 * @param[in] word Zmieniane słowo.
 * @param[in] dlugosc Długość słowa.
 * 
 * @return Słowo posiadające tylko małe litery.
 */
static wchar_t * make_lowercase(wchar_t *word, int dlugosc)
{
  wchar_t * new = malloc(sizeof(wchar_t) * (dlugosc +1));
  for (int i = 0; i < dlugosc; i++)
  {
    if (!iswalpha(word[i]))
      return NULL;
    new[i] = towlower(word[i]);
  }
  new[dlugosc] = '\0';
  return new;
}

/** Funkcja wczytująca słowo z pliku.
 * 
 * @param[in] stream Wejściowy strumień.
 * @return Wczytane słowo.
 */
static wchar_t * wezSlowo(FILE * stream)
{
  int rozmiar = 0;
  wchar_t word[MAX_WORD_LENGTH];
  wchar_t buff;
  while ((buff = fgetwc(stream)) != EOF)
  {
    if (!(iswalpha(buff))){
      ungetwc(buff, stream);
      break;
    }
    word[rozmiar] = buff;
    rozmiar++;
  } 
  wchar_t * prawdziwy = malloc(sizeof(wchar_t) * (rozmiar + 1));
  for (int i = 0; i < rozmiar; i++)
  {
    prawdziwy[i] = word[i];
  }
  prawdziwy[rozmiar] = L'\0';
  return prawdziwy;
}

/** Główna funkcja wykonująca
 * 
 * @param[in] pfile Strumień z pliku.
 * @param[in] czyPodpowiedzi Decyduje czy wypisywać podpowiedzi.
 */
void wykonaj(FILE * pfile, bool czyPodpowiedzi)
{
  struct dictionary * dict = NULL;
  dict = dictionary_load(pfile); 
  
  wchar_t buff;
  int wiersz = 1, kolumna = 1;
  while ((buff = fgetwc(stdin)) != EOF)
  {
    if(iswalpha(buff))
    {
      ungetwc(buff, stdin);     
      wchar_t * pom = wezSlowo(stdin);      
      int dlugosc = wcslen(pom);
      wchar_t * toFind = make_lowercase(pom, dlugosc);
      if (!dictionary_find(dict, toFind))
      {
        wprintf(L"#");
        
        if (czyPodpowiedzi)
        {
          fwprintf(stderr, L"%d,%d %ls: ", wiersz, kolumna, pom);
          struct word_list list;
          dictionary_hints(dict, toFind, &list);
          int rozmiar = word_list_size(&list);    
          if (rozmiar > 0)
          {        
            wchar_t * const *a = word_list_get(&list);
            for (size_t i = 0; i < rozmiar; ++i)
            {
                fwprintf(stderr, L"%ls", a[i]);
                if (i != rozmiar - 1)
                  fwprintf(stderr, L" ");
            }
          } 
          fwprintf(stderr, L"\n");         
          word_list_done(&list);
        }
      }
      wprintf(L"%ls", pom);
      kolumna += dlugosc;
      free(toFind);
      free(pom);
    }
    else
    {
      wprintf(L"%lc", buff);
      kolumna++;
    } 
    if (buff == '\n')
    {
      wiersz++;
      kolumna = 1;
    }
  }
  dictionary_done(dict);
}

/** Funkcja main.  
 * @param[in] argv Pomocnicze parametry, decydują o 
 * rodzaju słownika ora zwyświetlaniu podpowiedzi.
 * @param[in] argc Liczba argumentów.
 */
int main(int argc, const char* argv[])
{
  setlocale(LC_ALL, "pl_PL.UTF-8");
  bool czyPodpowiedzi = 0;
  FILE * pfile;
  if (argc == 3 && !strcmp(argv[1],"-v"))
  {
    pfile = fopen(argv[2], "r");
    czyPodpowiedzi = 1;
  }
  else if (argc == 2)
  {
    pfile = fopen(argv[1], "r");
  }
  else
  {
    wprintf(L"Błędne argumenty\n");
    return 0;
  }
  if (pfile == NULL)
  {
    wprintf(L"Brak pliku o podanej nazwie\n");
    return 0;
  }
  wykonaj(pfile, czyPodpowiedzi);
  fclose(pfile); 
  return 0;
}
