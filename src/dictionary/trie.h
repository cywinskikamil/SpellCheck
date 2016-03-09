/** @defgroup dictionary Moduł dictionary
    Biblioteka obsługująca drzewo TRIE.
  */
/** @file 
    Interfejs biblioteki obsługującej drzewo TRIE.
    @ingroup dictionary
 */

#ifndef __TRIE_H__
#define __TRIE_H__

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <stdbool.h>
#include <wctype.h>


/**
 * Drzewo używan do trzymania słów w słowniku.
 */
struct trie
{
	/**
		Etykieta wierzchołka, zawiera literę.
	 */
 	wchar_t litera;

 	/**
 	 * Zmienna logiczna, określa czy słowo kończące się w danym miejscu
 	 * faktycznie było wstawiane do słownika.
 	 */
	bool czySlowo;

	/**
	 * Zmienna opisująca liczbę dzieci wierzchołka.
	 */
	int iluSynow;

	/**
	 * Zmienna opisująca rozmiar tablicy dzieci wierzchołka.
	 */
	int dlugosc;

	/**
	 * Wskaźnik na ojca wierzchołka. Umożliwia poruszanie się w górę drzewa. 
	 */
	struct trie * ojciec;

	/**
	 * Tablica dzieci wierzchołka.
	 */
	struct trie ** synowie;
};


/** Funkcja sprawdzajaca czy dane slowo wystepuje w słowniku.
 * 
 * @param slowoDoWlozenia Szukane słowo.
 * @param rozmiarSlowa Długość słowa.
 * @param index Indeks wskazujący która litera słowa jest aktualnie przetwarzana.
 * @param root Przeszukiwane drzewo.
 * @return True jeśli słowo znajduje się w słowniku, false wpp.
 */
bool finder (const wchar_t * slowoDoWlozenia, int rozmiarSlowa, int index,
  struct trie * root);

/** Funkcja wstawiająca słowo do drzewa, operuje na korzeniu oraz wywołuje
	funkcję pomocniczą do wstawiania słowa na kolejnych dzieciach korzenia.
 * @param[in] slowoDoWlozenia Wkładane słowo.
 * @param[in] dlugoscSlowa Długość słowa.
 * @param[in] root Modyfikowane drzewo.
 * @param[in] forreal Zmienna decydująca czy słowo nie jest tylko częścią słowa,
 * które może być wrzucane za pomocą funkcji load.
 * @return Zmodyfikowane drzewo.
 */
struct trie * insert (const wchar_t * slowoDoWlozenia, int dlugoscSlowa,
  struct trie * root, bool forreal);

/** Funkcja czyszcząca drzewo.
 * @param[in,out] node Oczyszczane drzewo.
 */
void clean(struct trie * node);

/** Funkcja usuwająca z drzewa dane słowo. 
 * @param[in] slowoDoUsuniecia Usuwane słowo.
 * @param[in] rozmiarSlowa Długość słowa.
 * @param[in] index Indeks wskazujący ktora litera jest aktualnie przetwarzana.
 * @param[in] root2 Modyfikowane drzewo.
 * @return Zmodyfikowane drzewo.
 */
struct trie * delete (const wchar_t * slowoDoUsuniecia, int rozmiarSlowa,
  int index, struct trie * root2);

/** Funkcja zapisująca drzewo do pliku.
 * @param[in] node Zapisywane drzewo.
 * @param[in] stream Strumień do zapisu.
 * @param[in] glebokosc Aktualne zagłębienie w drzewie. 
 */
void zapis(const struct trie *node, FILE * stream, int glebokosc);

/** Funkcja wczytująca drzewo z pliku.
 * 
 * @param[in] stream Strumień do wczytywania.
 * @param[in,out] rozmiarAlfabetu Rozmiar alfabetu aktualnie użytych liter.
 * @param[in,out] liczbaLiter Liczba liter w alfabecie.
 * @param[in,out] alfabet Zbiór liter do tej pory wczytanych.
 * @return Wczytane drzewo.
 */
struct trie * wczyt(FILE * stream, int * rozmiarAlfabetu, int * liczbaLiter, wchar_t ** alfabet);

/** Funkcja sprawdzająca czy dana litera jest w alfabecie. 
 * @param[in] liczbaLiterPom Liczba liter w alfabecie.
 * @param[in] tablica Tablica liter.
 * @param[in] doWlozenia Sprawdzana litera.
 * @return True jeśli litera jest w alfabecie, false wpp.
 */
bool czyJest(int liczbaLiterPom, wchar_t * tablica, wchar_t doWlozenia);

/** Funkcja wstawiająca daną literę do alfabetu.
 * @param[in,out] rozmiarAlfabetu Rozmiar tablicy.
 * @param[in,out] liczbaLiter Liczba liter w alfabecie.
 * @param[in] tablica Tablica alfabetu.
 * @param[in] doWlozenia Wkładana litera.
 * @return Zmodyfikowana tablica alfabetu.
 */
wchar_t * poprawAlfabet(int * rozmiarAlfabetu, int * liczbaLiter, wchar_t * tablica, wchar_t doWlozenia);

/** Funkcja zwracająca listę podpowiedzi.
 * @param[in] node Drzewo słownikowe.
 * @param[in] slowo Sprawdzane słowo.
 * @param[in] alfabet Alfabet pomocniczy.
 * @param[in] liczbaLiter Liczba liter w alfabecie.
 * @param[in,out] liczbaHintow Liczba podpowiedzi.
 * @return Lista podpowiedzi.
 */
wchar_t ** hints(struct trie * node, const wchar_t * slowo, wchar_t * alfabet, int liczbaLiter, int * liczbaHintow);

/**
 * @brief sad
 * @details adsfaf
 * 
 * @param suf asdf
 * @param trie asdf
 */
void pokazStany(wchar_t * suf, struct trie * node);

#endif
