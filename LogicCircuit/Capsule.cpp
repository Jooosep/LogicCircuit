#include "Capsule.h"
#include "TableOfComponents.h"


Capsule::Capsule(FullTrace fullTrace, std::string name)
{
    this->fulltrace = fullTrace;
    int nOfInputs = this->fulltrace.controllerIdsVec.size();
    int nOfOutputs = this->fulltrace.receiverIdsVec.size();
    inputs.reserve(nOfInputs);
    outputs.reserve(nOfOutputs);
    for (int i = 0; i < nOfInputs; i++)
    {
        inputs.push_back(std::make_shared<Input>(i, -1, this->fulltrace.outputMap[this->fulltrace.controllerIdsVec[i]]->bitSize));
    }
    for (int i = 0; i < nOfOutputs; i++)
    {
        outputs.push_back(std::make_shared<Output>(i, -1, this->fulltrace.inputMap[this->fulltrace.receiverIdsVec[i]]->bitSize));
    }
    
    this->name = name;
    IdAssigner idGiver;
    id = idGiver.assignNewId();
}

Capsule::~Capsule()
{
}

void Capsule::connectInputs(int senderId, int relevantOutputId, int inputNumber = -1)
{
    if (inputNumber < 0)
    {
        for (auto& input : inputs)
        {
            if (input->senderId < 0)
            {
                input->senderId = senderId;
                input->relevantOutputId = relevantOutputId;
                break;
            }
        }
    }
    else if (inputNumber < inputs.size())
    {
        inputs[inputNumber]->senderId = senderId;
        inputs[inputNumber]->relevantOutputId = relevantOutputId;
    }
    else
    {
        std::cout << "input couldn't be connected (doesn't exist), attempt from id: " << senderId << " to id " << id << std::endl;
    }
}

void Capsule::setInputs(int id, int value)
{
    for (auto& input : inputs)
    {
        if (input->senderId == id)
        {
            input->value = value;
        }
    }
}

bool Capsule::hasSelector()
{
    return false;
}

std::vector<int> Capsule::getInputIds()
{
    std::vector<int> inputIds;
    for (auto& in : inputs)
    {
        inputIds.push_back(in->senderId);
    }
    return inputIds;
}

std::vector<int> Capsule::getOutputIds()
{
    std::vector<int> outputIds;
    for (auto& out : outputs)
    {
        outputIds.push_back(out->receiverId);
    }
    return outputIds;
}

std::vector<std::shared_ptr<Input>> Capsule::getInputsVec()
{
    return this->inputs;
}

std::vector<std::shared_ptr<Output>> Capsule::getOutputsVec()
{
    return this->outputs;
}

std::vector<int > Capsule::getSenderIds()
{
    std::vector<int> idVec;
    for (auto& input : inputs)
    {
        idVec.push_back(input->senderId);
    }
    return idVec;
}

std::vector<int > Capsule::getReceiverIds()
{
    std::vector<int> idVec;
    for (auto& output : outputs)
    {
        idVec.push_back(output->receiverId);
    }
    return idVec;
}

void Capsule::connectOutput(std::unique_ptr<Component>const& component, int outputNumber, int inputNumber)
{
    if (outputNumber < 0)
    {
        for (auto& output : outputs)
        {
            if (output->receiverId < 0)
            {
                output->receiverId = component->getId();
                component->connectInputs(id, output->id);
                break;
            }
            else
            {
                std::cout << "no available connections(Gate::connectOutput)" << std::endl;
            }
        }
    }
    else if (outputNumber < outputs.size())
    {
        outputs[outputNumber]->receiverId = component->getId();
        component->connectInputs(id, outputs[outputNumber]->id);
    }
    else
    {
        std::cout << "output with that outputNumber " << outputNumber << " not specified(Capsule::connectOutput)" << std::endl;
    }
}

void Capsule::unConnectOutput(int outputNumber)
{
    if (outputNumber > -1 && outputs.size() > outputNumber)
    {
        outputs[outputNumber]->receiverId = -1;
    }
}

void Capsule::realizeOutputs()
{
    if (outputs.size() > 0)
    {
        fulltrace.realizeTrace(inputs, outputs);
    }
    for (auto& output : outputs)
    {
        if (output->receiverId > -1)
        {
            table.getComponent(output->receiverId)->setInputs(id, output->value());
        }
    }
}

bool Capsule::isConnected()
{
    for (auto& output : outputs)
    {
        if (output->receiverId > -1)
        {
            return true;
        }
    }
    std::cout << "component not connected(id:" << id << ")" << std::endl;
    return false;
}

void Capsule::setOrderOfOperation(int order)
{
    orderOfOperation = order;
    for (auto& output : outputs)
    {
        if (output->receiverId > -1)
        {
            int nextOrder = table.getComponent(output->receiverId)->orderOfOperation;
            if (nextOrder != 0 && nextOrder < order &&  table.getComponent(output->receiverId)->orderSetBy != id)
            {
                table.getComponent(output->receiverId)->orderSetBy = id;
                table.getComponent(output->receiverId)->setOrderOfOperation(order + 1);
            }
        }
    }
}
std::string Capsule::getTraceText()
{
    return this->fulltrace.getAsString();
}

std::string Capsule::getInfo()
{
    std::string infoStr = "id: " + std::to_string(id) + "\n" + "orderofO: " + std::to_string(orderOfOperation) + "\n" + "name: " + name + "\n" + "bitSize: " + std::to_string(bitSize) + "\n";
    for (auto& input : inputs)
    {
        infoStr += "input " + std::to_string(input->number) + " value: " + std::bitset<32>(input->value()).to_string().substr(31 - input->bitSize, input->bitSize) + "\n";
    }
    for (auto& output : outputs)
    {
        infoStr += "output " + std::to_string(output->number) + " value: " + std::bitset<32>(output->value()).to_string().substr(31 - output->bitSize, output->bitSize) + "\n";
    }
    return infoStr;
}