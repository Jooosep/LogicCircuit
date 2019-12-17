#include "LineConnection.h"
#include "TableOfComponents.h"


LineConnection::LineConnection()
{
    name = "line";
    IdAssigner idGiver;
    id = idGiver.assignNewId();

}


LineConnection::~LineConnection()
{
}
void LineConnection::connectInputs(int senderId, int relevantOutputId, int inputNumber = 0)
{
    input.senderId = senderId;
    input.relevantOutputId = relevantOutputId;
}

void LineConnection::connectOutput(std::unique_ptr<Component> const& component, int outputNumber, int inputNumber)
{
    if (outputNumber < 0 || outputNumber >= outputs.size())
    {
        int newOutputNumber = outputs.size();
        outputs.emplace_back(newOutputNumber, component->getId());
        if (inputNumber < 0)
        {
            component->connectInputs(id, input.relevantOutputId, -1);
        }
        else
        {
            component->connectInputs(id, input.relevantOutputId, inputNumber);
        }
    }
    else
    {
        outputs[outputNumber].receiverId = component->getId();
        if (inputNumber < 0)
        {
            component->connectInputs(id, -1);
        }
        else
        {
            component->connectInputs(id, inputNumber);
        }
    }
}

void LineConnection::unConnectOutput(int outputNumber)
{
    if (outputNumber > -1 && outputs.size() > outputNumber)
    {
        outputs[outputNumber].receiverId = -1;
    }
}

void LineConnection::setInputs(int id, int value)
{
    if (input.senderId == id)
    {
        input.value = value;
    }
    else
    {
        std::cout << "input received from unconnected source" << std::endl;
    }
}

void LineConnection::realizeOutputs()
{
    for (auto& output : outputs)
    {
        if (output.receiverId > -1)
        {
            if (table.getComponent(output.receiverId) != nullptr)
            {
                table.getComponent(output.receiverId)->setInputs(id, input.value());
            }
        }
    }
}

bool LineConnection::isConnected()
{
    for (auto& output : outputs)
    {
        if (output.receiverId > -1)
        {
            return true;
        }
    }
    return false;
}

void LineConnection::setOrderOfOperation(int order)
{
    orderOfOperation = order;
    for (auto& output : outputs)
    {
        if (output.receiverId > -1)
        {
            int nextOrder = table.getComponent(output.receiverId)->orderOfOperation;
            if (nextOrder != 0 && nextOrder < order && table.getComponent(output.receiverId)->orderSetBy != id)
            {
                table.getComponent(output.receiverId)->setOrderOfOperation(order + 1);
                table.getComponent(output.receiverId)->orderSetBy = id;
            }
        }
    }
}
std::vector<int> LineConnection::getInputIds()
{
    return {input.senderId};
}
std::vector<int> LineConnection::getOutputIds()
{
    std::vector<int> outputIds;
    for (auto& out : outputs)
    {
        outputIds.push_back(out.receiverId);
    }
    return outputIds;
}

std::vector<int > LineConnection::getSenderIds()
{
    return {};
}
std::vector<int > LineConnection::getReceiverIds()
{
    return {};
}

std::vector<std::shared_ptr<Input>> LineConnection::getInputsVec()
{
    return {};
}

std::vector<std::shared_ptr<Output>> LineConnection::getOutputsVec()
{
    return {};
}
bool LineConnection::getInput()
{
    return input.value();
}

std::string LineConnection::getInfo()
{
    std::string infoStr = "id: "+ std::to_string(id) + "\n" + "orderofO: " + std::to_string(orderOfOperation) + "\n"  + std::to_string(id) + "\n" + "name: " + name + "\n";
    infoStr += "Value: " + std::bitset<32>(input.value()).to_string().substr(31 - 16, 16) + "\n";
    return infoStr;
}