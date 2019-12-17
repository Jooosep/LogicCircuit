#include "Listener.h"
#include "TableOfComponents.h"

Listener::Listener(int numberOfInputs)
{
    this->numberOfInputs = numberOfInputs;
    name = "listener";

    IdAssigner idGiver;
    id = idGiver.assignNewId();

    inputs.reserve(numberOfInputs);
    for (int i = 0; i < numberOfInputs; i++)
    {
        inputs.emplace_back(i, -1);
    }
}

Listener::~Listener()
{
}

void Listener::connectInputs(int senderId, int relevantOutputId, int inputNumber = -1)
{
    if (inputNumber < 0)
    {
        for (auto& input : inputs)
        {
            if (input.senderId < 0)
            {
                input.senderId = senderId;
                input.relevantOutputId = relevantOutputId;
                break;
            }
        }
    }
    else if (inputNumber < inputs.size())
    {
        inputs[inputNumber].senderId = senderId;
        inputs[inputNumber].relevantOutputId = relevantOutputId;
    }
    else
    {
        std::cout << "input couldn't be connected, attempt from id: " << senderId << " to id " << id << std::endl;
    }
}

void Listener::setInputs(int id, int value)
{
    bool inputSet = false;
    for (auto& input : inputs)
    {
        if (input.senderId = id)
        {
            input.value = value;
            inputSet = true;
        }
        if (inputSet)
        {
            break;
        }
    }
    if (!inputSet)
    {
        std::cout << "input received from unconnected source" << std::endl;
    }
}

void Listener::displayInputs()
{
    for (auto& input : inputs)
    {
        if (input.senderId > -1)
        {
            std::cout << "inputNo: " << input.number << "   input value: " << std::bitset<32>(input.value()) << std::endl;
        }
    }
}

bool Listener::isConnected()
{
    for (auto& input : inputs)
    {
        if (input.senderId > -1)
        {
            return true;
        }
    }
    std::cout << "component not connected(id:"  << id << ")" << std::endl;
    return false;
}

void Listener::realizeOutputs()
{
    this->displayInputs();
}

void Listener::setOrderOfOperation(int order)
{
    orderOfOperation = order;
}