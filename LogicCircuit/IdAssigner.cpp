#include "IdAssigner.h"



IdAssigner::IdAssigner()
{
    reader.open(fileName);
    if (isEmpty(reader))
    { 
        reader.close();
        myfile.open(fileName);
        myfile << "-1";
        myfile.close();
    }
    else
    {
        reader.close();
    }
}


IdAssigner::~IdAssigner()
{
}

int IdAssigner::assignNewId()
{
    reader.open(fileName);
    if (reader.is_open())
    {
        reader.seekg(-1, std::ios_base::end);

        bool keepLooping = true;
        while (keepLooping) 
        {
            char ch;
            reader.get(ch);

            if ((int)reader.tellg() <= 1)
            {
                reader.seekg(0);
                keepLooping = false;
            }
            else if (ch == '\n') 
            {
                keepLooping = false;
            }
            else
            {
                reader.seekg(-2, std::ios_base::cur);
            }
        }
        std::string lastLine;
        std::getline(reader, lastLine);
        int lastId = std::stoi(lastLine);
        int newId = lastId + 1;
        std::cout << newId << std::endl;

        reader.close();

        myfile.open(fileName);
        myfile << '\n' << newId;
        myfile.close();

        return newId;
    }
    return -1;
}

bool IdAssigner::isEmpty(std::ifstream& pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}
