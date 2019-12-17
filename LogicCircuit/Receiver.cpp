#include "Receiver.h"
#include "TableOfComponents.h"

Receiver::Receiver(int bitSize)
{
    name = "receiver";
    IdAssigner idGiver;
    id = idGiver.assignNewId();
    input = std::make_shared<Input>(0, -1, bitSize);
    this->bitSize = bitSize;
}

Receiver::~Receiver()
{
}

void Receiver::connectOutput(std::unique_ptr<Component>const& component, int outputNumber, int inputNumber)
{

}

void Receiver::disconnect()
{
    input->senderId = -1;
    input->value = 0;
}

void Receiver::realizeOutputs()
{

}

void Receiver::unConnectOutput(int outputNumber)
{

}

bool Receiver::isConnected()
{
    if (input->senderId > -1)
    {
        return true;
    }
    return false;
}

void Receiver::setInputs(int id, int value)
{
    if (input->senderId == id)
    {
        input->value = value;
    }
    else
    {
        std::cout << "input received from unconnected source" << std::endl;
    }
}

void Receiver::connectInputs(int senderId, int relevantOutputId, int inputNumber = -1)
{
    input->senderId = senderId;
    input->relevantOutputId = relevantOutputId;
}

std::vector<int> Receiver::getOutputIds()
{
    return {};
}

std::vector<int> Receiver::getInputIds()
{
    std::vector<int> inputIds;
    inputIds.push_back(input->senderId);
    return inputIds;
}

std::vector<std::shared_ptr<Input>> Receiver::getInputsVec()
{
    return { input };
}

std::vector<std::shared_ptr<Output>> Receiver::getOutputsVec()
{
    return {};
}

std::vector<int > Receiver::getSenderIds()
{
    return { input->senderId };
}
std::vector<int > Receiver::getReceiverIds()
{
    return {};
}

void Receiver::setOrderOfOperation(int order)
{
    
}

int Receiver::getValue()
{
    return input->value();
}

std::string Receiver::getInfo()
{
    std::string infoStr = "id: " + std::to_string(id) + "\n" + "name : " + name + "\n";
    infoStr += "Input" + std::to_string(input->number) + " value: " + std::bitset<32>(input->value()).to_string().substr(31 - bitSize, bitSize) + "\n";
    return infoStr;
}