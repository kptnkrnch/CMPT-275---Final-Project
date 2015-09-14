#include "other.h"
#include "filething.h"

#include <iostream>
#include <algorithm>

main() 
{
	std::cout << "hi\n";
	std::cout << function();
	std::string a;
	std::cout << "\ntext: ";
	std::getline(std::cin, a);

	std::cout << "\n" << a;
	
	int n = readfile("Input");
	writefiles(n);
	
	return 0;
}