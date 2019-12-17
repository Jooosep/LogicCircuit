#pragma once
#include <iostream>
#include <bitset>

class BitArray
{

private:
    unsigned int array;
public:
    BitArray();
    ~BitArray();
    int operator()(void) const
    {
        return array;
    }
    void operator=(int y)
    {
        array = y;
    }
    void print()
    {
        std::cout << std::bitset<32>(array) << std::endl;
    }
    void invert()
    {
        array = ~array;
    }
};

