#pragma once
#include "Component.h"


struct GateSpecification
{
    int bitSize;
    callback_function  algorithm; // variable to store function pointer type
    int nOfOutputs;
    int nOfInputs;
    bool hasSelector;
    std::vector<std::shared_ptr<Input>> inputs;
    std::vector< std::shared_ptr<Output>> outputs;
    Input selector;

    //default constructor
    GateSpecification() {};
    
    //copy constructor
    GateSpecification(const GateSpecification& other)
    {
        this->bitSize = other.bitSize;
        this->nOfInputs = other.nOfInputs;
        this->nOfOutputs = other.nOfOutputs;
        this->bitSize = other.bitSize;
        this->hasSelector = other.hasSelector;
        this->algorithm = other.algorithm;
        this->inputs = other.inputs;
        this->outputs = other.outputs;
    }
    
    //move constructor
    GateSpecification(GateSpecification &&other)
    {
        std::cout << "Move ctor (currently empty)" << std::endl;
    }

    //move assignment operator
    GateSpecification& operator=(GateSpecification&& other)
    {
        if (this == &other)
        {
            return *this;
        }

        std::cout << "MOVE assign" << std::endl;
        this->bitSize = other.bitSize;
        this->nOfInputs = other.nOfInputs;
        this->nOfOutputs = other.nOfOutputs;
        this->bitSize = other.bitSize;
        this->hasSelector = other.hasSelector;
        this->algorithm = other.algorithm;
        this->inputs = other.inputs;
        this->outputs = other.outputs;

        return *this;
    }

    //move assignment operator 2
    GateSpecification& operator=(const GateSpecification& other)
    {
        if (this == &other)
        {
            return *this;
        }

        std::cout << "copy assign" << std::endl;
        this->bitSize = other.bitSize;
        this->nOfInputs = other.nOfInputs;
        this->nOfOutputs = other.nOfOutputs;
        this->bitSize = other.bitSize;
        this->hasSelector = other.hasSelector;
        this->algorithm = other.algorithm;
        this->inputs = other.inputs;
        this->outputs = other.outputs;

        return *this;
    }

    GateSpecification(int nOfInputs, int nOfOutputs, bool hasSelector, callback_function algorithm, int bitSize)
        :nOfInputs(nOfInputs), nOfOutputs(nOfOutputs), bitSize(bitSize), hasSelector(hasSelector), algorithm(algorithm)
    {
        inputs.reserve(nOfInputs);
        outputs.reserve(nOfOutputs);
        for (int i = 0; i < nOfInputs; i++)
        {
            inputs.emplace_back(std::make_shared<Input>(i, -1, bitSize));
        }
        if (hasSelector)
        {
            inputs.emplace_back(std::make_shared<Input>(nOfInputs, -1, bitSize));
        }
        for (int i = 0; i < nOfOutputs; i++)
        {
            outputs.emplace_back(std::make_shared<Output>(i, -1, bitSize));
        }
    }
};


class Gate :
    public Component
{
    using Component::Component;

private:
    GateSpecification gateSpecification;
    
public:

    Gate(int gateNumber);

    ~Gate();

    Gate* cloneImpl() const override { return new Gate(*this); };
    void connectOutput(std::unique_ptr<Component>const& component, int outputNumber = -1, int inputNumber = -1);
    void unConnectOutput(int outputNumber) override;
    void connectInputs(int senderId, int relevantOutputId, int inputNumber) override;
    void setInputs(int id, int value) override;
    bool isConnected() override;
    void realizeOutputs() override;
    void setOrderOfOperation(int order) override;
    std::vector<int> getInputIds() override;
    std::vector<int> getOutputIds() override;
    std::vector<std::shared_ptr<Input>> getInputsVec() override;
    std::vector<std::shared_ptr<Output>> getOutputsVec() override;
    std::vector<int> getSenderIds() override;
    std::vector<int> getReceiverIds() override;
    bool hasSelector();
    std::string getInfo() override;
    AlgBundle getAlgBundle();
};

