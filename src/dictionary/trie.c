/** @file
  Drzewo TRIE.
  @ingroup dictionary
 */
#include "trie.h"
#include <assert.h>
#include <string.h>
#include <time.h>

/** Maksymalna długość słowa.* */
#define MAX_WORD_LENGTH 63

/** Pomocnicza zmienna do rozpoznawania "pustych" słów. */
static wchar_t * puste = L"";

/** Alokowanie pamięci dla nowego wierzchołka.
 * @return Zaalokowana pamięć.
 */
static struct trie * newNode()
{
  return malloc(sizeof(struct trie));
}

/** Pomocnicza funkcja powiekszajaca tablice synów wierzchołka.
 * 
 * @param[in] doPowiekszenia Wierzchołek, którego tablica synów jest powiększana.
 * @param[in] doWlozenia Etykieta dodwawanego wierzchołka.
 * @return Zmodyfikowany wierzchołek.
 */
static struct trie * powiekszacz(struct trie * doPowiekszenia, wchar_t doWlozenia)
{
  struct trie ** synowie = doPowiekszenia->synowie;
  int nowaDlugosc = doPowiekszenia->dlugosc;
  if (doPowiekszenia->iluSynow == doPowiekszenia->dlugosc)
  {
    nowaDlugosc += 2;
  }
  int iluSynow = doPowiekszenia->iluSynow;
  struct trie ** nowiSynowie = NULL;
  nowiSynowie = realloc(nowiSynowie, (sizeof(struct trie *) * nowaDlugosc));
  int i = 0;
  while (i < iluSynow && synowie[i]->litera < doWlozenia)
  {
    nowiSynowie[i] = synowie[i];
    i++;
  }
  nowiSynowie[i++] = NULL;
  while (i <= iluSynow) {
    nowiSynowie[i] = synowie[i-1];
    i++;
  }
  while (i < nowaDlugosc)
    nowiSynowie[i++] = NULL;
  free(synowie);
  doPowiekszenia->dlugosc = nowaDlugosc;
  doPowiekszenia->synowie = nowiSynowie;
  return doPowiekszenia;
}

/** Pomocnicza funkcja wyszukująca indeks w tablicy synów, który odpowiada etykiecie,
    za pomocą wyszukiwania binarnego.
 * @param[in] node Wierzchołek, którego tablica synów jest przeszukiwana.
 * @param[in] doWlozenia Etykieta szukanego syna.
 * @return Numer indeksu jeśli taki istnieje. NULL w przeciwnym wypadku.
 */
static int indeksDoWlozenia (const struct trie * node, wchar_t doWlozenia)
{
  struct trie ** tablica = node->synowie;
  int lewy = 0;
  int prawy = node->iluSynow - 1;
  while (lewy <= prawy){
    int srodek = (lewy + prawy) >> 1;
    wchar_t obecnaLitera = tablica[srodek]->litera;
    if (obecnaLitera == doWlozenia)
      return srodek;
    else if (obecnaLitera > doWlozenia)
      prawy = srodek - 1;
    else
      lewy = srodek + 1;
  }
  return -1;
}

bool finder (const wchar_t * slowoDoWlozenia, int rozmiarSlowa, int index, struct trie * root)
{
  if (root == NULL)
    return false;
  if (index == rozmiarSlowa)
  {
    return root->czySlowo;
  }
  int indeksPomocniczy = indeksDoWlozenia(root, slowoDoWlozenia[index]);
  if (indeksPomocniczy == -1)
    return false;
  return finder(slowoDoWlozenia, rozmiarSlowa, index + 1, root->synowie[indeksPomocniczy]);
}

/** Pomocnicza funkcja insert, operująca na wierzchołkach które nie są korzeniami.
 * 
 * @param[in] slowoDoWlozenia Wkładane słowo.
 * @param[in] rozmiarSlowa Długość słowa.
 * @param[in] index Indeks wskazujący która litera słowa jest aktualnie przetwarzana.
 * @param[in] node Modyfikowane drzewo.
 * @param[in] forreal Zmienna decydująca czy słowo nie jest tylko częścią słowa,
 * które może być wrzucane za pomocą funkcji load.
 * @return Zmodyfikowane drzewo.
 */
static struct trie * insertPom (const wchar_t * slowoDoWlozenia,
  int rozmiarSlowa, int index, struct trie * node, bool forreal)
{
  if (node == NULL && index < rozmiarSlowa)
  {
    node = newNode();
    node->litera = slowoDoWlozenia[index++];
    node->iluSynow = 0;
    node->dlugosc = 0;
    node->synowie = NULL;
    node->czySlowo = 0;
  }

  if (index < rozmiarSlowa){
    if(node->synowie == NULL)
    {
      node->synowie = malloc(sizeof(struct trie *));
      node->iluSynow = 1;
      node->dlugosc = 1;
      node->synowie[0] = NULL;
      node->synowie[0] = insertPom(slowoDoWlozenia, rozmiarSlowa, index, node->synowie[0], forreal);
      node->synowie[0]->ojciec = node;
    }
    else
    {
      int indexPomocniczy = indeksDoWlozenia(node, slowoDoWlozenia[index]);
      if (indexPomocniczy != -1)
      {
        node->synowie[indexPomocniczy] = insertPom (slowoDoWlozenia, rozmiarSlowa, \
          index + 1, node->synowie[indexPomocniczy],forreal);
        node->synowie[indexPomocniczy]->ojciec = node;
      }
      else
      {
        node = powiekszacz(node, slowoDoWlozenia[index]);
        for (int i = 0; i <= node->iluSynow; i++)
        {
          if (node->synowie[i] == NULL)
          {
            node->synowie[i] = insertPom(slowoDoWlozenia, rozmiarSlowa, index, \
              node->synowie[i],forreal);
            node->synowie[i]->ojciec = node;
            node->iluSynow++;
            break;
          }
        }
      }
      }
  }
  if (index == rozmiarSlowa)
  {
    node->czySlowo = forreal;
  }
  return node;
}

/** Funkcja inicjalizująca korzeń drzewa.
 * 
 * @param[in] root Korzeń drzewa.
 * @return Zmodyfikowany korzeń.
 */
static struct trie * rootInitalize(struct trie * root)
{
  if (root == NULL)
  {
    root = newNode();
    root->litera = '\0';
    root->czySlowo = 0;
    root->iluSynow = 0;
    root->dlugosc = 1;
    root->synowie = malloc(sizeof(struct trie *));
    root->synowie[0] = NULL;
    root->ojciec = root;
  }
  return root;
}

struct trie * insert (const wchar_t * slowoDoWlozenia, int dlugoscSlowa, struct trie * root, bool forreal){

  if (root == NULL);
    root = rootInitalize(root);
  int index = indeksDoWlozenia(root, slowoDoWlozenia[0]);

  if (index != -1)
  {
    root->synowie[index] = insertPom(slowoDoWlozenia, dlugoscSlowa, 1,
      root->synowie[index],forreal);
    root->synowie[index]->ojciec = root;
  }
  else
  {
    root = powiekszacz(root, slowoDoWlozenia[0]);
    for (int i = 0; i <= root->dlugosc; i++)
    {
      if (root->synowie[i] == NULL)
      {
        root->synowie[i] = insertPom(slowoDoWlozenia, dlugoscSlowa, 0,
          root->synowie[i],forreal);
        root->synowie[i]->ojciec = root;
        root->iluSynow++;
        break;
      }
    }
  }
  return root;
}

void clean(struct trie * node)
{
  if (node == NULL)
    return;
  for (int i = 0; i < node->iluSynow; i++)
  {
    clean(node->synowie[i]);
    node->synowie[i] = NULL;
  }
  if (node->synowie != NULL){
    free(node->synowie);
    node->synowie = NULL;
  }
  free(node);
  node = NULL;
}

/** Funkcja porządkująca tablicę synów, wyrzuca NULL na koniec tablicy.
 * @param[in,out] root Modyfikowane drzewo.
 */
static void skurcz(struct trie * root){
  int i = 0;
  while (i < root->iluSynow && root->synowie[i] !=NULL)
  {
    i++;
  }
  while (i < root->iluSynow)                                                
  {
    root->synowie[i] = root->synowie[i+1];
    i++;
  }
  root->iluSynow--;
}

struct trie * delete (const wchar_t * slowoDoUsuniecia, int rozmiarSlowa,
  int index, struct trie * root2)
{
  struct trie * root = root2;
  while (index < rozmiarSlowa)
  {
    int indeksPomocniczy = indeksDoWlozenia(root, slowoDoUsuniecia[index]);
    root = root->synowie[indeksPomocniczy];
    index++;
  }
  
  root->czySlowo = 0;
  if (!(root->iluSynow))
  {
    while (root->iluSynow < 2 && root->litera != '\0' && !(root->czySlowo))
    {
      struct trie * temp = root->ojciec;
      if (root->synowie != NULL)
      {
        free(root->synowie);
        root->synowie = NULL;
      }
      int hehe = indeksDoWlozenia(root->ojciec, root->litera);
      root->ojciec->synowie[hehe] = NULL;
      free(root);
      root = temp;
    }
    if (root->litera == '\0' && root->iluSynow == 1)
    {
      clean(root);
      return NULL;
    }
    else
    {
      skurcz(root);
    }
  }
  return root2;
}

void zapis(const struct trie *node, FILE * stream, int glebokosc)
{
  if (node == NULL){
    return;
  }
  wchar_t literka = node->litera;
  if (literka != '\0')
  {
    if (node->ojciec->iluSynow > 1)
      fwprintf(stream, L"%d", glebokosc);

    if (node->czySlowo)
    {
      fwprintf(stream, L"%lc", towupper(node->litera));
    }
    else
      fwprintf(stream, L"%lc", node->litera);
  }
  for (int i = 0; i < node->iluSynow; i++){
    zapis(node->synowie[i], stream, glebokosc+1);
  }
}

/** Funkcja pobierająca słowo ze strumienia.
 * @param[in] stream Strumień wejściowy.
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
    if (iswupper(buff))
      break;
  }
  wchar_t * prawdziwy = malloc(sizeof(wchar_t) * (rozmiar + 1));
  for (int i = 0; i < rozmiar; i++)
  {
    prawdziwy[i] = word[i];
  }
  prawdziwy[rozmiar] = L'\0';
  return prawdziwy;
}

/** Funkcja sprawdzająca czy dane słowo należy uznać za prawdziwe
    i wstawić je do słownika.  
 * @param[in] slowo Sprawdzane słowo.
 * @return True, jeśli słowo zawiera dużą literę i przy wstawianiu do słownika
 * należy zaznaczyć, iż słowo jest słowem. False wpp.
 */
static bool isForreal(wchar_t * slowo)
{
  int i = 0;
  while (slowo[i] != '\0')
  {
    if (iswupper(slowo[i]))
      return true;
    i++;
  }
  return false;
}

/** Funkcja pomocnicza, wyszukuje gdzie w drzewie kończy się dane słowo.
 * @param[in] slowo Szukane słowo.
 * @param[in] rozmiarSlowa Długość słowa.
 * @param[in] index Aktualnie przetwarzana litera w słowie.
 * @param[in] root Słownik.
 * @return Wskaźnik do miejsca gdzie kończy się słowo w słowniku.
 */
static struct trie * wskaznikoDo (wchar_t * slowo, int rozmiarSlowa, int index, struct trie * root)
{
  if (index == rozmiarSlowa )
  {
    return root;
  }
  int indeksPomocniczy = indeksDoWlozenia(root, slowo[index]);
  return wskaznikoDo(slowo, rozmiarSlowa, index+1, root->synowie[indeksPomocniczy]);
}

bool czyJest(int liczbaLiterPom, wchar_t * tablica, wchar_t doWlozenia)
{
  if (tablica == NULL)
    return false;
  int lewy = 0;
  assert(tablica != NULL);
  int prawy = liczbaLiterPom - 1;
  while (lewy <= prawy){
    int srodek = (lewy + prawy) >> 1;
    wchar_t obecnaLitera = tablica[srodek];
    if (obecnaLitera == doWlozenia)
    {
      return true;
    }
    else if (obecnaLitera > doWlozenia)
      prawy = srodek - 1;
    else
      lewy = srodek + 1;
  }
  return false;
}

wchar_t * poprawAlfabet(int * rozmiar, int * liczbaLiter, wchar_t * tablica, wchar_t doWlozenia)
{
  int nowaDlugosc = *(rozmiar);
  int liczba = *(liczbaLiter);
  wchar_t * nowiSynowie = tablica;
  if (liczba >= nowaDlugosc)
  {
    nowaDlugosc *= 2;
    if (!nowaDlugosc)
      nowaDlugosc++;
    nowiSynowie = realloc(tablica, sizeof(wchar_t) * (nowaDlugosc + 1));
    *(rozmiar) = nowaDlugosc;
  }

  int i = 0;
  while (i < liczba && nowiSynowie[i] < doWlozenia)
  {
    i++;
  }

  wchar_t przesuwany = nowiSynowie[i];
  wchar_t asdf;
  nowiSynowie[i] = doWlozenia;
  i++;
  while (i <= liczba)
  {
    if (i != liczba)
      asdf = nowiSynowie[i];
    nowiSynowie[i] = przesuwany;
    przesuwany = asdf;
    i++;
  }
  liczba++;
  *(liczbaLiter) = liczba;
  *(rozmiar) = nowaDlugosc;
  return nowiSynowie;
}

struct trie * wczyt(FILE * stream, int * rozmiarAlfabetu, int * liczbaLiter, wchar_t ** alfabet)
{
  wchar_t * alfabetPom = *(alfabet);
  int liczbaLiterPom = *(liczbaLiter);
  struct trie * nowy = NULL;
  nowy = rootInitalize(nowy);
  struct trie * pomocniczy = nowy;
  int glebokosc = 0;
  wchar_t buff = NULL;
  while ((buff = fgetwc(stream)) != EOF)
  {
    if(!(iswdigit(buff)))
    {
      ungetwc(buff,stream);
      wchar_t * pom = wezSlowo(stream);
      int dlugosc = wcslen(pom);
      bool czySlowo = isForreal(pom);
      pom[dlugosc-1] = towlower(pom[dlugosc-1]);
      for (int i = 0; i < dlugosc; i++)
      {
        if (alfabetPom == NULL)
        {
          alfabetPom = malloc(sizeof(wchar_t));
        }

        if (!czyJest(liczbaLiterPom, alfabetPom, pom[i]))
        {
          alfabetPom = poprawAlfabet(rozmiarAlfabetu, liczbaLiter, alfabetPom, pom[i]);
          liczbaLiterPom = *(liczbaLiter);
        }
      }
      if (pomocniczy->litera == '\0')
        pomocniczy = insert(pom, dlugosc, pomocniczy, czySlowo);
      else
        pomocniczy = insertPom(pom, dlugosc, 0, pomocniczy, czySlowo);
      pomocniczy = wskaznikoDo(pom, dlugosc, 0, pomocniczy);
      glebokosc += dlugosc;
      free(pom);
    }
    else
    {
      ungetwc(buff, stream);
      int pom = 0;
      fwscanf(stream, L"%d", &pom);
      while (glebokosc > pom)
      {
        pomocniczy = pomocniczy->ojciec;
        glebokosc--;
      }
    }
  }
  *(alfabet) = alfabetPom;
  return nowy;
}

/** Funkcja wstawiająca dane słowo do tablicy. 
 * @param[in,out] ileSlow Ilość słów w tablicy.
 * @param[in,out] tablica Tablica słów.
 * @param[in,out] rozmiar Rozmiar tablicy.
 * @param[in] slowo Wkładane słowo.
 * @param[in] dlugosc Długość słowa.
 * @return Zmodyfikowana tablica.
 */
static wchar_t ** poprawTablice(int ileSlow, wchar_t ** tablica, int * rozmiar, wchar_t * slowo, int dlugosc)
{
  int nowaDlugosc = *(rozmiar);
  wchar_t ** nowaTablica = tablica;
  wchar_t * doWlozenia = malloc(sizeof(wchar_t) * (dlugosc + 2));
  wcscpy(doWlozenia, slowo);
  doWlozenia[dlugosc + 1] = '\0';
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
    nowaTablica = realloc(tablica, sizeof(wchar_t *) * nowaDlugosc);
    nowaTablica[ileSlow] = doWlozenia;
    ileSlow++;
  }
  *(rozmiar) = nowaDlugosc;
  tablica = nowaTablica;
  return tablica;
}

/** Funkcja generująca podpowiedzi poprzez zamienienie jednej z liter na inną z alfabetu. 
 * @param[in] node Drzewo słownikowe.
 * @param[in] alfabet Alfabet liter.
 * @param[in] slowo Słowo do którego generowane są podpowiedzi.
 * @param[in] dlugoscAlfabetu Ilość liter w alfabecie.
 * @param[in,out] liczbaHintow Liczba podpowiedzi.
 * @param[in] dlugosc Rozmiar słowa.
 * @return Lista podpowiedzi.
 */
static wchar_t ** generowaniePrzezZamiane(struct trie * node, wchar_t * alfabet, wchar_t * slowo,
 int dlugoscAlfabetu, int * liczbaHintow, int dlugosc)
{

  int ileSlow = 0;
  int rozmiar = 0;
  wchar_t ** wynik = NULL; //malloc(sizeof(wchar_t *));
  for (int i = 0; i < dlugosc; i++)
  {
    wchar_t zmieniane = slowo[i];
    for (int j = 0; j < dlugoscAlfabetu; j++)
    {
      // fwprintf(stderr, L"tu %lc %lc %d %d\n", slowo[i], alfabet[j], i, j);
      slowo[i] = alfabet[j];
      // fwprintf(stderr, L"tu\n");
      if (finder(slowo, dlugosc, 0, node))
      {
        wynik = poprawTablice(ileSlow, wynik, &rozmiar, slowo, dlugosc);
        ileSlow++;
      }
      slowo[i] = zmieniane;
    }
  }
  for (int i = ileSlow; i < rozmiar; i++)
    wynik[i] = NULL;
  *(liczbaHintow) = ileSlow;  
  return wynik;
}

/** Funkcja generująca podpowiedzi poprzez usniecie jednej z liter.
 * @param[in] node Drzewo słownikowe.
 * @param[in] slowo Słowo do którego generowane są podpowiedzi.
 * @param[in,out] liczbaHintow Liczba podpowiedzi.
 * @param[in] dlugosc Rozmiar słowa.
 * @return Lista podpowiedzi.
*/
static wchar_t ** generowaniePrzezUsuniecie(struct trie * node, wchar_t * slowo, int * liczbaHintow, int dlugosc)
{
  wchar_t ** wynik = NULL; //malloc(sizeof(wchar_t *));
  int rozmiar = 0;
  int ileSlow = 0;
  wchar_t * pom = malloc(sizeof(wchar_t) * (dlugosc + 2));
  for (int i = 0; i < dlugosc; i++)
  {
    int index = 0;
    for (int j = 0; j < dlugosc; j++)
    {
      if (i == j)
        continue;
      pom[index++] = slowo[j];
    }
    pom[index] = '\0';
    if (finder(pom, dlugosc - 1, 0, node))
    {
      //fwprintf(stderr, L"u slowo %ls\n", pom);
      wynik = poprawTablice(ileSlow, wynik, &rozmiar, pom, dlugosc);
      ileSlow++;
    }
  }

  for (int i = ileSlow; i < rozmiar; i++)
    wynik[i] = NULL;
  *(liczbaHintow) = ileSlow;
  free(pom);
  return wynik;
}
/** Funkcja generująca podpowiedzi poprzez wstawienie jednej z liter z alfabetu. 
 * @param[in] node Drzewo słownikowe.
 * @param[in] alfabet Alfabet liter.
 * @param[in] slowo Słowo do którego generowane są podpowiedzi.
 * @param[in] dlugoscAlfabetu Ilość liter w alfabecie.
 * @param[in,out] liczbaHintow Liczba podpowiedzi.
 * @param[in] dlugosc Rozmiar słowa.
 * @return Lista podpowiedzi.
*/
static wchar_t ** generowaniePrzezWstawienie(struct trie * node, wchar_t * alfabet, wchar_t * slowo,
 int dlugoscAlfabetu, int * liczbaHintow, int dlugosc)
{
  int ileSlow = 0;
  int rozmiar = 0;
  wchar_t ** wynik = NULL; //malloc(sizeof(wchar_t *) );
  wchar_t * pom = malloc(sizeof(wchar_t) * (dlugosc + 3));
  for (int i = 1; i < dlugosc + 1; i++)
  {
    pom[i] = slowo[i-1];
  }
  pom[dlugosc+1] ='\0';
  for (int i = 0; i < dlugosc + 1; i++)
  {
    for (int j = 0; j < dlugoscAlfabetu; j++)
    {
      pom[i] = alfabet[j];
      // fwprintf(stderr, L"w slowo %ls\n", pom);
      if (finder(pom, dlugosc + 1, 0, node))
      {
        // fwprintf(stderr, L"\tw slowo %ls\n", pom);
        wynik = poprawTablice(ileSlow, wynik, &rozmiar, pom, dlugosc);
        ileSlow++;
      }
    }
    if (i < dlugosc)
      pom[i] = pom [i+1];
  }
  for (int i = ileSlow; i < rozmiar; i++)
    wynik[i] = NULL;


  *(liczbaHintow) = ileSlow;
  free(pom);
  return wynik;
}

/** Funkcja łącząca trzy tablicę słów w jedną.
 * @param[in] list1 Pierwsza tablica
 * @param[in] list2 Druga tablica.
 * @param[in] list3 Trzecia tablica.
 * @param[in] rozmiar1 Rozmiar pierwszej tablicy.
 * @param[in] rozmiar2 Rozmiar drugiej tablicy.
 * @param[in] rozmiar3 Rozmiar trzeciej tablicy.
 * @return [description]
 */
static wchar_t ** mergeLists (wchar_t ** list1, wchar_t ** list2, wchar_t ** list3, int rozmiar1, int rozmiar2, int rozmiar3)
{
  wchar_t ** result = malloc(sizeof(wchar_t *) * (rozmiar1 + rozmiar2 + rozmiar3 + 2));
  for (int i = 0; i < rozmiar1; i++)
  {
    result[i] = list1[i];
    // fwprintf(stderr, L"przepisuje zam %ls\n", list1[i]);
  }
  for (int i = 0; i < rozmiar2; i++)
  {
    result[rozmiar1 + i] = list2[i];
    // fwprintf(stderr, L"przepisuje usu %ls\n", list2[i]);
  }
  for (int i = 0; i < rozmiar3; i++)
  {
    result[rozmiar1 + rozmiar2 + i] = list3[i];
    // fwprintf(stderr, L"przepisuje wsr %ls\n", list3[i]);
  }
  return result;
}

wchar_t ** hints(struct trie * node, const wchar_t * slowo, wchar_t * alfabet, int liczbaLiter, int * liczbaHintow)
{
  wchar_t * pom = (wchar_t *) slowo;
  wchar_t ** wynik;
  wchar_t ** wynik1;
  wchar_t ** wynik2;
  wchar_t ** wynik3;
  int dlugoscSlowa = wcslen(pom);
  int ilosc1 = 0, ilosc2 = 0, ilosc3 = 0;

  wynik1 = generowaniePrzezZamiane(node, alfabet, pom, liczbaLiter, liczbaHintow, dlugoscSlowa);
  ilosc1 = *liczbaHintow;

  wynik2 = generowaniePrzezUsuniecie(node, pom, liczbaHintow, dlugoscSlowa);
  ilosc2 = *liczbaHintow;

  wynik3 = generowaniePrzezWstawienie(node, alfabet, pom, liczbaLiter, liczbaHintow, dlugoscSlowa);
  ilosc3 = *liczbaHintow;

  *liczbaHintow = ilosc1 + ilosc2 + ilosc3;

  if (*liczbaHintow == 0)
  {
    return NULL;
  }

  wynik = mergeLists(wynik1, wynik2, wynik3, ilosc1, ilosc2, ilosc3);

  if (wynik1 != NULL)  
    free(wynik1);
  if (wynik2 != NULL)  
    free(wynik2);
  if (wynik3 != NULL)  
    free(wynik3);
  return wynik;
}

static wchar_t * nextSuf(wchar_t * slowo)
{
  int dlugosc = wcslen(slowo);
  if (dlugosc == 1)
    return puste;
  wchar_t * next = malloc(sizeof(wchar_t) * dlugosc);
  for (int i = 0; i < dlugosc - 1; i++){
    next[i] = slowo[i+1];
  }
  next[dlugosc-1] = L'\0';
  free(slowo);
  return next;
}

void pokazStany(wchar_t * suf, struct trie * node)
{
  
  while (wcscoll(suf, puste)){    
    if (node->litera != L'\0')
      fwprintf(stderr, L"%lc literka\n", node->litera);
    int indeks = indeksDoWlozenia(node, suf[0]);
    if (indeks == -1)
      return;
    node = node->synowie[indeks];
    suf = nextSuf(suf);
  }
  fwprintf(stderr, L"%lc literka\n", node->litera);
}