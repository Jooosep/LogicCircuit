#include "Controller.h"
#include "TableOfComponents.h"

Controller::Controller(int bitSize)
{
    name = "controller";
    IdAssigner idGiver;
    id = idGiver.assignNewId();
    output = std::make_shared<Output>(0, -1, bitSize);
    this->bitSize = bitSize;
}

Controller::~Controller()
{
}

void Controller::connectOutput(std::unique_ptr<Component>const& component, int outputNumber, int inputNumber)
{
    output->receiverId = component->getId();
    component->connectInputs(id, output->id);
}

void Controller::unConnectOutput(int outputNumber)
{
    output->receiverId = -1;
}

void Controller::disconnect()
{
    output->receiverId = -1;
    output->value = 0;
    orderOfOperation = -1;
}

void Controller::setOutput(int value)
{

    output->value = value;

}

void Controller::invertOutput()
{

    output->value.invert();

}

void Controller::realizeOutputs()
{

    if (output->receiverId > -1)
    {
        table.getComponent(output->receiverId)->setInputs(id, output->value());
    }

}
int Controller::getOutput()
{

    return output->value();

}

bool Controller::isConnected()
{
    if (output->receiverId > -1)
    {
        return true;
    }
    return false;
}

void Controller::setInputs(int id, int value)
{

}

void Controller::connectInputs(int senderId, int relevantOutputId, int inputNumber = -1)
{

}

std::vector<int> Controller::getOutputIds()
{
    std::vector<int> outputIds;
    outputIds.push_back(output->receiverId);
    return outputIds;
}

std::vector<int> Controller::getInputIds()
{
    return {};
}

std::vector<int > Controller::getSenderIds()
{
    return {};
}
std::vector<int > Controller::getReceiverIds()
{
    return {output->receiverId};
}
std::vector<std::shared_ptr<Input>> Controller::getInputsVec()
{
    return {};
}

std::vector<std::shared_ptr<Output>> Controller::getOutputsVec()
{
    return { output };
}

void Controller::setOrderOfOperation(int order)
{
    orderOfOperation = order;

    if (output->receiverId > -1)
    {
        int nextOrder = table.getComponent(output->receiverId)->orderOfOperation;
        if (nextOrder != 0 && nextOrder < order)
        {
            table.getComponent(output->receiverId)->setOrderOfOperation(order + 1);
        }
    }
}

int Controller::getValue()
{
    return output->value();
}

void Controller::setValue(int value)
{
    output->value = value;
}

std::string Controller::getInfo()
{
    std::string infoStr = "id: " + std::to_string(id) + "\n" + "name : " + name + "\n";
    infoStr += "Output" + std::to_string(output->number) + " value: " + std::bitset<32>(output->value()).to_string().substr(31 - bitSize, bitSize) + "\n";
    return infoStr;
}