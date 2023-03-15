#include <iostream>

#include "funkcje.h"

int main(int argc, char* argv[])
{
	options opcje;

	getParams(argc, argv, opcje);

	if (opcje.tryb == "k")
	{
		compress(opcje);
	}
	else if (opcje.tryb == "d")
	{
		decompress(opcje);
	}
	else
	{
		usage();
		return 0;
	}
	std::cout << std::endl;
	return 0;

}

