#include <fstream>
#include <vector>
#include <math.h>
#include <map>
#include <iostream>
#include <random>
#include <algorithm>
#include <memory>

template<typename T>
class Templates
{
private:
	T classValue;
	const static int c = 5;

	template<int v> struct A
	{
		A()
		{

		}
		void squared()
		{
			std::cout << v * v << std::endl;
		}
	};
public:
	Templates(T v)
	{
		std::cout << "Templates class created" << std::endl;
		classValue = v * 2;
	}
	~Templates()
	{
		std::cout << "Templates class destroyed" << std::endl;
	}

	T getClassValue()
	{
		return classValue;
	}

	A<c> squareDoer;

};

int mains2()
{
	Templates<char> temp('z');
	std::cout << "Class value is " << temp.getClassValue() << std::endl;
	std::cout << "Squaredoer gives us ";
	temp.squareDoer.squared();
	temp.~Templates();
	std::cin.get();
	return 0;
}