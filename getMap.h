#ifndef GETMAP_H
#define GETMAP_H

#include <fstream>
#include <iostream>
#include <cstdlib>

class getMap
{
	const char* fn;
    int xres;
    int yres;
	int* mapdat;
public:
	getMap(const char* fn_, int x_, int y_, int* md) : fn(fn_), xres(x_), yres(y_), mapdat(md)
	{
		std::ifstream file(fn, std::ios::in | std::ios::binary);
		if(!file.good())
		{
			std::cerr<<"Error opening file: "<<fn<<std::endl;
			exit(2);
		}
        for (auto i = 0; !file.eof(); i++)
            file.read(reinterpret_cast<char*>(&mapdat[i]), sizeof(int));
	}
};
/*
int main()
{
	int level[3072];
	getMap("map.dat", 64, 48, level);

	for(auto i = 0; i < 3072; i++)
	std::cout<<level[i]<<" ";
	return 0;
};
*/
#endif
