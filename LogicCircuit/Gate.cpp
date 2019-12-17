#include "Gate.h"
#include "TableOfComponents.h"

Gate::Gate(int gateNumber)
{
    std::string name;
    switch (gateNumber)
    {
        case NOTGATE:
        {
            name = "NOTGATE";
            int bitSize = 1;
            int nOfInputs = 1;
            int nOfOutputs = 1;
            bool hasSelector = false;
            callback_function algorithm = &notAlgorithm;
            this->gateSpecification = GateSpecification(nOfInputs, nOfOutputs, hasSelector, algorithm, bitSize);
            break;
        }
        case ANDGATE:
        {
            name = "ANDGATE";
            int bitSize = 1;
            int nOfInputs = 2;
            int nOfOutputs = 1;
            bool hasSelector = false;
            callback_function algorithm = &andAlgorithm;
            this->gateSpecification = GateSpecification(nOfInputs, nOfOutputs, hasSelector, algorithm, bitSize);
            break;
        }
        case ORGATE:
        {
            name = "ORGATE";
            int bitSize = 1;
            int nOfInputs = 2;
            int nOfOutputs = 1;
            bool hasSelector = false;
            callback_function algorithm = &orAlgorithm;
            this->gateSpecification = GateSpecification(nOfInputs, nOfOutputs, hasSelector, algorithm, bitSize);
            break;
        }
        case XORGATE:
        {
            name = "XORGATE";
            int bitSize = 1;
            int nOfInputs = 2;
            int nOfOutputs = 1;
            bool hasSelector = false;
            callback_function algorithm = &xorAlgorithm;
            this->gateSpecification = GateSpecification(nOfInputs, nOfOutputs, hasSelector, algorithm, bitSize);
            break;
        }
        case MUXGATE:
        {
            name = "MUXGATE";
            int bitSize = 1;
            int nOfInputs = 2;
            int nOfOutputs = 1;
            bool hasSelector = true;
            callback_function algorithm = &muxAlgorithm;
            this->gateSpecification = GateSpecification(nOfInputs, nOfOutputs, hasSelector, algorithm, bitSize);
            break;
        }
        case DMUXGATE:
        {
            name = "DMUXGATE";
            int bitSize = 1;
            int nOfInputs = 1;
            int nOfOutputs = 2;
            bool hasSelector = true;
            callback_function algorithm = &dmuxAlgorithm;
            this->gateSpecification = GateSpecification(nOfInputs, nOfOutputs, hasSelector, algorithm, bitSize);
            break;
        }
        case NOT16GATE:
        {
            name = "NOT16GATE";
            int bitSize = 16;
            int nOfInputs = 1;
            int nOfOutputs = 1;
            bool hasSelector = false;
            callback_function algorithm = &notAlgorithm;
            this->gateSpecification = GateSpecification(nOfInputs, nOfOutputs, hasSelector, algorithm, bitSize);
            break;
        }
        case AND16GATE:
        {
            name = "AND16GATE";
            int bitSize = 16;
            int nOfInputs = 2;
            int nOfOutputs = 1;
            bool hasSelector = false;
            callback_function algorithm = &andAlgorithm;
            this->gateSpecification = GateSpecification(nOfInputs, nOfOutputs, hasSelector, algorithm, bitSize);
            break;
        }
        case OR16GATE:
        {
            name = "OR16GATE";
            int bitSize = 16;
            int nOfInputs = 2;
            int nOfOutputs = 1;
            bool hasSelector = false;
            callback_function algorithm = &orAlgorithm;
            this->gateSpecification = GateSpecification(nOfInputs, nOfOutputs, hasSelector, algorithm, bitSize);
            break;
        }
        case MUX16GATE:
        {
            name = "MUX16GATE";
            int bitSize = 16;
            int nOfInputs = 2;
            int nOfOutputs = 1;
            bool hasSelector = true;
            callback_function algorithm = &muxAlgorithm;
            this->gateSpecification = GateSpecification(nOfInputs, nOfOutputs, hasSelector, algorithm, bitSize);
            break;
        }
        case OR8WAYGATE:
        {
            name = "OR8WAYGATE";
            int bitSize = 1;
            int nOfInputs = 8;
            int nOfOutputs = 1;
            bool hasSelector = false;
            callback_function algorithm = &orAlgorithm;
            this->gateSpecification = GateSpecification(nOfInputs, nOfOutputs, hasSelector, algorithm, bitSize);
            break;
        }

        case MUX4WAY16GATE:
        {
            name = "MUX4WAY16GATE";
            int bitSize = 16;
            int nOfInputs = 2;
            int nOfOutputs = 1;
            bool hasSelector = true;
            callback_function algorithm = &muxAlgorithm;
            this->gateSpecification = GateSpecification(nOfInputs, nOfOutputs, hasSelector, algorithm, bitSize);
            break;
        }

        case MUX8WAY16GATE:
        {
            name = "MUX16GATE";
            int bitSize = 16;
            int nOfInputs = 2;
            int nOfOutputs = 1;
            bool hasSelector = true;
            callback_function algorithm = &muxAlgorithm;
            this->gateSpecification = GateSpecification(nOfInputs, nOfOutputs, hasSelector, algorithm, bitSize);
            break;
        }
        case DMUX4WAYGATE:
        {
            name = "MUX16GATE";
            int bitSize = 16;
            int nOfInputs = 2;
            int nOfOutputs = 1;
            bool hasSelector = true;
            callback_function algorithm = &muxAlgorithm;
            this->gateSpecification = GateSpecification(nOfInputs, nOfOutputs, hasSelector, algorithm, bitSize);
            break;
        }
        case DMUX8WAYGATE:
        {
            name = "MUX16GATE";
            int bitSize = 16;
            int nOfInputs = 2;
            int nOfOutputs = 1;
            bool hasSelector = true;
            callback_function algorithm = &muxAlgorithm;
            this->gateSpecification = GateSpecification(nOfInputs, nOfOutputs, hasSelector, algorithm, bitSize);
            break;
        }
        case UNSPECIFIED:
        {
            name = "MUX16GATE";
            int bitSize = 16;
            int nOfInputs = 2;
            int nOfOutputs = 1;
            bool hasSelector = true;
            callback_function algorithm = &muxAlgorithm;
            this->gateSpecification = GateSpecification(nOfInputs, nOfOutputs, hasSelector, algorithm, bitSize);
            break;
        
        }
    }
    this->name = name;
    type = gateNumber;
    IdAssigner idGiver;
    id = idGiver.assignNewId();
}



Gate::~Gate() {};


void Gate::connectInputs(int senderId, int relevantOutputId, int inputNumber = -1)
{
    if (inputNumber < 0)
    {
        for (auto& input : gateSpecification.inputs)
        {
            if (input->senderId < 0)
            {
                input->senderId = senderId;
                input->relevantOutputId = relevantOutputId;
                break;
            }
        }
    }
    else if (inputNumber < gateSpecification.inputs.size())
    {
        gateSpecification.inputs[inputNumber]->senderId = senderId;
        gateSpecification.inputs[inputNumber]->relevantOutputId = relevantOutputId;
    }
    else
    {
        std::cout << "input couldn't be connected (doesn't exist), attempt from id: " << senderId << " to id " << id << std::endl;
    }
}

void Gate::setInputs(int id, int value)
{
    for (auto& input : gateSpecification.inputs)
    {
        if (input->senderId == id)
        {
            input->value = value;
        }
    }
}

bool Gate::hasSelector()
{
    return this->gateSpecification.hasSelector;
}
std::vector<int> Gate::getInputIds()
{
    std::vector<int> inputIds;
    for (auto& in : gateSpecification.inputs)
    {
        inputIds.push_back(in->senderId);
    }
    return inputIds;
}

std::vector<int> Gate::getOutputIds()
{
    std::vector<int> outputIds;
    for (auto& out : gateSpecification.outputs)
    {
        outputIds.push_back(out->receiverId);
    }
    return outputIds;
}

void Gate::connectOutput(std::unique_ptr<Component>const& component, int outputNumber, int inputNumber)
{
    if (outputNumber < 0)
    {
        for (auto& output : gateSpecification.outputs)
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
    else if (outputNumber < gateSpecification.outputs.size())
    {
        gateSpecification.outputs[outputNumber]->receiverId = component->getId();
        component->connectInputs(id, gateSpecification.outputs[outputNumber]->id);
    }
    else
    {
        std::cout << "output with that outputNumber " << outputNumber << " not specified(Gate::connectOutput)" << std::endl;
    }
}

void Gate::unConnectOutput(int outputNumber)
{
    if (outputNumber > -1 && gateSpecification.outputs.size() > outputNumber)
    {
        gateSpecification.outputs[outputNumber]->receiverId = -1;
    }
}

void Gate::realizeOutputs()
{
    if (gateSpecification.nOfOutputs > 0)
    {
        gateSpecification.algorithm(gateSpecification.inputs, gateSpecification.outputs);
    }
    for (auto& output : gateSpecification.outputs)
    {
        if (output->receiverId > -1)
        {
            table.getComponent(output->receiverId)->setInputs(id, output->value());
        }
    }
}

bool Gate::isConnected()
{
    for (auto& output : gateSpecification.outputs)
    {
        if (output->receiverId > -1)
        {
            return true;
        }
    }
    std::cout << "component not connected(id:" << id << ")" << std::endl;
    return false;
}

void Gate::setOrderOfOperation(int order)
{
    orderOfOperation = order;
    for (auto& output : gateSpecification.outputs)
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
std::vector<std::shared_ptr<Input>> Gate::getInputsVec()
{
    return this->gateSpecification.inputs;
}

std::vector<std::shared_ptr<Output>> Gate::getOutputsVec()
{
    return this->gateSpecification.outputs;
}

std::vector<int > Gate::getSenderIds()
{
    std::vector<int> idVec;
    for (auto& input : this->gateSpecification.inputs)
    {
        idVec.push_back(input->senderId);
    }
    return idVec;
}
std::vector<int > Gate::getReceiverIds()
{
    std::vector<int> idVec;
    for (auto& output : this->gateSpecification.outputs)
    {
        idVec.push_back(output->receiverId);
    }
    return idVec;
}

AlgBundle Gate::getAlgBundle()
{
    std::vector<int> inputIds;
    std::vector<int> outputIds;
    callback_function algorithm;
    for (auto& input : this->gateSpecification.inputs)
    {
        inputIds.push_back(input->id);
    }
    for (auto& output : this->gateSpecification.outputs)
    {
        outputIds.push_back(output->id);
    }
    return AlgBundle(inputIds, outputIds, this->gateSpecification.algorithm);
}

std::string Gate::getInfo()
{
    std::string infoStr = "id: " + std::to_string(id) + "\n" + "orderofO: " + std::to_string(orderOfOperation) + "\n" + "name: " + name + "\n" + "bitSize: " + std::to_string(gateSpecification.bitSize) + "\n";
    for (auto& input : gateSpecification.inputs)
    {
        if (this->gateSpecification.hasSelector && input->number == this->getInputIds().size() - 1)
        {
            infoStr += "selector value: " + std::bitset<32>(input->value()).to_string().substr(31 - gateSpecification.bitSize, gateSpecification.bitSize) + "\n"; 
        }
        else
        {
            infoStr += "input " + std::to_string(input->number) + " value: " + std::bitset<32>(input->value()).to_string().substr(31 - gateSpecification.bitSize, gateSpecification.bitSize) + "\n";
        }
    }
    for (auto& output : gateSpecification.outputs)
    {
        infoStr += "output " + std::to_string(output->number) + " value: " + std::bitset<32>(output->value()).to_string().substr(31 - gateSpecification.bitSize, gateSpecification.bitSize) + "\n";
    }
    return infoStr;
}


