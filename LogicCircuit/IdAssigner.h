#pragma once
#include <iostream>
#include <fstream>
#include <sstream>

class IdAssigner
{
private:
    const std::string fileName = "componentIds.txt";
    std::ofstream myfile;
    std::ifstream reader;
public:
    IdAssigner();
    ~IdAssigner();
    int assignNewId();
    bool isEmpty(std::ifstream& pFile);
};

