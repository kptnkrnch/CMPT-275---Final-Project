#include "other.h"
#include "filething.h"

#include <iostream>
#include <algorithm>

main() 
{
	std::cout << "Hello World\n";
	std::cout << function();
	std::string a;
	std::cout << "\nEnter some text: ";
	std::getline(std::cin, a);
	std::reverse(a.begin(), a.end());
	std::cout << "\n" << a;
	
	int n = readfile("Input");
	writefiles(n);
	
	return 0;
}