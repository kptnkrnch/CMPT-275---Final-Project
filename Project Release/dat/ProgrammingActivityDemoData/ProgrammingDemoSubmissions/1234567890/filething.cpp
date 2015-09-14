#include "filething.h"

int readfile(std::string filename)
{
	std::fstream f;
	f.open(filename.c_str(), std::fstream::in);
	
	int n;
	f >> n;
	return n;
}

void writefiles(int n)
{
	std::fstream f1;
	f1.open("Output1", std::fstream::out);
	f1 << n;
	f1.close();
	
	std::fstream f2;
	f2.open("Output2", std::fstream::out);
	for (int i = 0; i < n; i++)
	{
		f2 << n << "\n";
	}
	f2.close();
}