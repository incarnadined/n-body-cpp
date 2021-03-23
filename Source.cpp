#include <iostream>

#include "Vec3.h"

void print(Vec3f a)
{
	std::cout << "Vec3f(" << a.GetX() << ", " << a.GetY() << ", " << a.GetZ() << ")" << std::endl;
}

int main()
{
	print(Vec3f(1.0, 3.0, 5.0) / 5);
	std::cin.get();
}