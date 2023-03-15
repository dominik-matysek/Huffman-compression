/** @file */

#ifndef FUNKCJE_H
#define FUNKCJE_H

#include "struktury.h"
#include <unordered_map>


/** Funkcja wyswietlajaca opis uzycia programu w przypadku podania zlych parametrow. */
void usage();

/** Funkcja alokujaca nowy wezel.
@param character symbol wezla
@param value ilosc wystapien danego symbolu
@param left wskaznik na lewego potomka 
@param right wskaznik na prawego potomka
@return utworzony wezel
*/
Node* createNode(char character, int value, Node* left, Node* right);

/** Funkcja przypisujaca parametry wywolania programu do elementow struktury options.
@param argc liczba parametrow uruchomienia programu
@param argv[] wartosci parametrow wprowadzonych przez uzytkownika
@param data zmienna typu struktury options
*/
void getParams(int argc, char* agrv[], options& data);

/** Funkcja pomocnicza sprawdzajaca czy wezel jest lisciem.
@param root korzen drzewa
@return funkcja zwraca true jezeli wezel nie ma potomkow
*/
bool isOnly(Node* root);

/** Funkcja rekurencyjna kodujaca elementy w zaleznosci od ich pozycji w drzewie.
@param root korzen drzewa
@param str teks pomocniczy przy rekurencji
@param huffmanCode mapa kodowania, w ktorej bedziemy przechowywac symbol oraz odpowiadajacy mu kod prefiksowy
@return wychodzimy z poziomu rekurencji jezeli dojdziemy do konca drzewa
*/
void searchTree(Node* root, std::string str, std::unordered_map<char, std::string>& huffmanCode);

/** Funkcja odpowiedzialna za kompresje pliku. Przy pomocy innych funkcji pobiera zawartosc pliku wejsciowego, tworzy drzewo Huffmana, plik slownika, dokonuje konwersji na bajty i zapisuje plik wyjsciowy.
@param opt zmienna typu struktury options
*/
void compress(options opt);

/** Funkcja odpowiedzialna za dekompresje pliku. Przy pomocy innych funkcji pobiera zawartosc slownika i zakodowanego pliku, dokonuje konwersji na string, usuwa zbedne znaki i zapisuje odkodowana zawartosc w pliku wyjsciowym.
@param opt zmienna typu struktury options
*/
void decompress(options opt);

/** Funkcja dokonujaca konwersji kodu prefiksowego na bajty.
@param encoded tekst z zawartoscia pliku wejsciowego w postaci kodu prefiksowego
@param buffer wektor do ktorego dodajemy bajty
@param displace przesuniecie bitowe
*/
void convert(std::string encoded, std::vector<char>& buffer, int& displace);

/** Funkcja dokonujaca konwersji bajtow z zakodowanego pliku wejsciowego na kod prefiksowy.
@param to_encode tekst z zawartoscia zakodowanego pliku wejsciowego 
*/
void convertString(std::string& to_encode);

#endif
