/** @file  */

#ifndef STRUKTURY_H
#define STRUKTURY_H

#include <string>

/** Struktura przechowujaca sciezki do plikow oraz tryb korzystania z programu. */
struct options
{
	std::string input; ///< sciezka do pliku wejsciowego
	std::string output; ///< sciezka do pliku wyjsciowego
	std::string tryb; ///< tryb (k - kompresja, d - dekompresja)
	std::string dictionary; ///< sciezka do slownika, w ktorym zapisane sa symbole oraz odpowiadajace im kody
};

/** Struktura wezla, podstawowego elementu drzewa kodowania. */
struct Node
{
	char character; ///< symbol 
	int value; ///< ilosc wystapien danego symbolu

	Node* left, ///< wskaznik na lewego potomka
		* right; ///< wskaznik na prawego potomka

	Node(char c, int v, Node* l = nullptr, Node* r = nullptr): character(c), value(v), left(l), right(r) ///< konstruktor struktury (potomkowie s¹ wstepnie nullptr)
	{
	}
	
	~Node() ///< destruktor struktury
	{
		delete left; 
		delete right;
	}

};

/** Struktura porownujaca dwa liscie z drzewa. */
struct compare
{
	bool operator() (Node* l, Node* r)
	{
		return l->value > r->value;
	}
};

#endif