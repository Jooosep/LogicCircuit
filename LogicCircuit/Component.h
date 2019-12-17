#pragma once
#include <SFML/graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <fstream>
#include <vector>
#include <math.h>
#include <map>
#include <iostream>
#include <random>
#include <algorithm>
#include <memory>
#include "IdAssigner.h"
#include <bitset>
#include "BitArray.h"

struct IncrementingIdAssigner
{
    int id;
    IncrementingIdAssigner()
    {
        this->id = -1;
    }
    int operator()(void)
    {
        this->id++;
        return this->id; 
    }
};

//global IdContructor
extern IncrementingIdAssigner getIOId;

struct Output
{
    int number;
    int receiverId;
    BitArray value;
    int id;
    int bitSize;

    Output(int number = 0, int receiverId = -1, int bitSize = 1)
    {
        this->number = number;
        this->receiverId = receiverId;
        this->bitSize = bitSize;
        this->id = getIOId();
    }
};


struct Input
{
    int number;
    int senderId;
    int relevantOutputId;
    int id;
    BitArray value;
    int bitSize;
    //bool valueSet;

    Input(int number = 0, int senderId = -1, int bitSize = 1, int relevantOutputId = -1)
    {
        this->number = number;
        this->senderId = senderId;
        this->relevantOutputId = relevantOutputId;
        this->bitSize = bitSize;
        this->id = getIOId();
    }
};


typedef void(*callback_function)(std::vector<std::shared_ptr<Input>>, std::vector<std::shared_ptr<Output>>); // type for conciseness

typedef void(*compile_function)(std::vector<std::shared_ptr<Input>>, std::shared_ptr<Output>);

struct IOBundle
{
    int id;
    BitArray value;
    int bitSize;
};

struct AlgBundle
{
    std::vector<int> inputIds;
    std::vector<int> outputIds;
    callback_function algorithm;
    int inputArgumentVectorIndex;
    int outputArgumentVectorIndex;
    AlgBundle()
    {

    }
    AlgBundle(std::vector<int> inputIds, std::vector<int> outputIds, callback_function algorithm)
    {
        this->inputIds = inputIds;
        this->outputIds = outputIds;
        this->algorithm = algorithm;
    }
    
};

namespace {

    void notAlgorithm(std::vector<std::shared_ptr<Input>> inputs, std::vector<std::shared_ptr<Output>> outputs)
    {
        for (int i = 0; i < inputs.size(); i++)
        {
            outputs[i]->value = ~inputs[i]->value();
        }
    }
    void andAlgorithm(std::vector<std::shared_ptr<Input>> inputs, std::vector<std::shared_ptr<Output>> outputs)
    {
        outputs[0]->value = inputs[0]->value()&inputs[1]->value();
    }
    void orAlgorithm(std::vector<std::shared_ptr<Input>> inputs, std::vector<std::shared_ptr<Output>> outputs)
    {
        outputs[0]->value = inputs[0]->value() | inputs[1]->value();
    }
    void multiOrAlgorithm(std::vector<std::shared_ptr<Input>> inputs, std::vector<std::shared_ptr<Output>> outputs)
    {
        for (int i = 0; i < inputs.size() - 1; i++)
        {
            outputs[0]->value = inputs[i]->value() | inputs[i + 1]->value();
        }
    }
    void xorAlgorithm(std::vector<std::shared_ptr<Input>> inputs, std::vector<std::shared_ptr<Output>> outputs)
    {
        outputs[0]->value = inputs[0]->value() ^ inputs[1]->value();
    }

    //2inputs 1selector, 1 output
    void muxAlgorithm(std::vector<std::shared_ptr<Input>> inputs, std::vector<std::shared_ptr<Output>> outputs)
    {
        (inputs.back()->value()) ? (outputs[0]->value = inputs[1]->value()) : (outputs[0]->value = inputs[0]->value());
    }
    void dmuxAlgorithm(std::vector<std::shared_ptr<Input>> inputs, std::vector<std::shared_ptr<Output>> outputs)
    {
        (inputs.back()->value()) ?
            (outputs[1]->value = inputs[0]->value(), outputs[0]->value = 0)
            : (outputs[0]->value = inputs[0]->value(), outputs[1]->value = 0);
    }

    std::map<std::string, callback_function> algos = { {"notAlgorithm", &notAlgorithm},
                                                       {"andAlgorithm", &andAlgorithm},
                                                       {"orAlgorithm", &orAlgorithm},
                                                       {"xorAlgorithm", &xorAlgorithm},
                                                       {"muxAlgorithm", &muxAlgorithm},
                                                       {"dmuxAlgorithm", &dmuxAlgorithm} };

    std::map<callback_function, std::string> algoStrings = { {&notAlgorithm, "notAlgorithm"},
                                                       {&andAlgorithm, "andAlgorithm"},
                                                       {&orAlgorithm, "orAlgorithm"},
                                                       {&xorAlgorithm, "xorAlgorithm"},
                                                       {&muxAlgorithm, "muxAlgorithm"},
                                                       {&dmuxAlgorithm, "dmuxAlgorithm"}, };

}

struct FullTrace
{
    std::vector<AlgBundle> algBundles;
    std::map<int, std::shared_ptr<Input>> inputMap;
    std::map<int, std::shared_ptr<Output>> outputMap;
    std::vector<int> controllerIdsVec;
    std::vector<int> receiverIdsVec;
    std::vector<std::vector<std::shared_ptr<Input>>> argumentInputsVec;
    std::vector<std::vector<std::shared_ptr<Output>>> argumentOutputsVec;
    std::map<int, std::pair<int, int>> idVecPosMap;

    FullTrace() {};

    FullTrace(std::vector<AlgBundle> algBundles, std::map<int, std::shared_ptr<Input>> inputMap, std::map<int, std::shared_ptr<Output>> outputMap, std::vector<int> controllerIdsVec, std::vector<int> receiverIdsVec)
    {
        this->algBundles = algBundles;
        this->inputMap = inputMap;
        this->outputMap = outputMap;
        this->controllerIdsVec = controllerIdsVec;
        this->receiverIdsVec = receiverIdsVec;

        std::vector<std::shared_ptr<Output>> controllers;
        for (int i = 0; i < controllerIdsVec.size(); i++)
        {
            idVecPosMap[controllerIdsVec[i]] = { argumentOutputsVec.size(), controllers.size() };
            controllers.push_back(outputMap[controllerIdsVec[i]]);
            
        }
        argumentOutputsVec.push_back(controllers);

        std::vector<std::shared_ptr<Input>> receivers;
        for (int i = 0; i < receiverIdsVec.size(); i++)
        {
            idVecPosMap[receiverIdsVec[i]] = { argumentInputsVec.size(), receivers.size() };
            receivers.push_back(inputMap[receiverIdsVec[i]]);

        }
        argumentInputsVec.push_back(receivers);

        for (int i = 0; i < algBundles.size(); i++)
        {
            this->algBundles[i].inputArgumentVectorIndex = argumentInputsVec.size();
            this->algBundles[i].outputArgumentVectorIndex = argumentOutputsVec.size();

            std::vector<std::shared_ptr<Input>> inputs;
            for (int j = 0; j < algBundles[i].inputIds.size(); j++)
            {
                int inputId = algBundles[i].inputIds[j];
                idVecPosMap[inputId] = { argumentInputsVec.size(), inputs.size() };
                inputs.push_back(inputMap[inputId]);
            }
            argumentInputsVec.push_back(inputs);

            std::vector<std::shared_ptr<Output>> outputs;
            for (int j = 0; j < algBundles[i].outputIds.size(); j++)
            {
                int outputId = algBundles[i].outputIds[j];
                idVecPosMap[outputId] = { argumentOutputsVec.size(), outputs.size() };
                outputs.push_back(outputMap[outputId]);
            }
            argumentOutputsVec.push_back(outputs);
        }
    }

    void realizeTrace(std::vector<std::shared_ptr<Input>> inputs, std::vector<std::shared_ptr<Output>> outputs)
    {
        int i = 0;
        for (auto& output : argumentOutputsVec[0])
        {
            output->value = inputs[i]->value;
            i++;
        }

        for (auto& algBundle : algBundles)
        {
            int inputVecIndex = algBundle.inputArgumentVectorIndex;
            int outputVecIndex = algBundle.outputArgumentVectorIndex;

            for (auto& input : argumentInputsVec[inputVecIndex])
            {
                std::pair<int,int> outputIndexes = idVecPosMap[input->relevantOutputId];
                input->value = argumentOutputsVec[outputIndexes.first][outputIndexes.second]->value;
            }
            algBundle.algorithm(argumentInputsVec[inputVecIndex], argumentOutputsVec[outputVecIndex]);
        }

        i = 0;
        for (auto& input : argumentInputsVec[0])
        {
            std::pair<int, int> outputIndexes = idVecPosMap[input->relevantOutputId];
            input->value = argumentOutputsVec[outputIndexes.first][outputIndexes.second]->value;
            outputs[i]->value = input->value;
            i++;
        }
        
    }
    std::string getAsString()
    {
        //algbundles
        std::string fullString;
        for (auto& bundle : this->algBundles)
        {
            fullString.append("alg " + algoStrings[bundle.algorithm]);
            for (auto& inputId : bundle.inputIds)
            {
                fullString.append(" i" + std::to_string(inputId));
            }
            for (auto& outputId : bundle.inputIds)
            {
                fullString.append(" o" + std::to_string(outputId));
            }
            fullString.append("/n");
        }
        fullString.append("in ");
        for (auto const&[key, val] : this->inputMap)
        {
            fullString.append("key " + std::to_string(key) + " vals " + std::to_string(val->bitSize) + " " + std::to_string(val->relevantOutputId));
        }
        fullString.append("\n");
         
        fullString.append("out ");
        for (auto const&[key, val] : this->outputMap)
        {
            fullString.append("key " + std::to_string(key) + " vals " + std::to_string(val->bitSize));
        }
        fullString.append("\n");

        fullString.append("con");
        for (auto& controllerId : this->controllerIdsVec)
        {
            fullString.append(" " + std::to_string(controllerId));
        }
        fullString.append("\n");

        fullString.append("rec");
        for (auto& receiverId : this->receiverIdsVec)
        {
            fullString.append(" " + std::to_string(receiverId));
        }
        fullString.append("\n");
        return fullString;
    }
    
};



enum Components
{
    NOTGATE,
    ANDGATE,
    ORGATE,
    XORGATE,
    MUXGATE,
    DMUXGATE,
    NOT16GATE,
    AND16GATE,
    OR16GATE,
    MUX16GATE,
    OR8WAYGATE,
    MUX4WAY16GATE,
    MUX8WAY16GATE,
    DMUX4WAYGATE,
    DMUX8WAYGATE,
    UNSPECIFIED

};


class Component
{
protected:

    int id;

public:
    Component();
    ~Component();

    auto clone() const
    {
        return std::unique_ptr<Component>(cloneImpl());
    }
    virtual Component* cloneImpl() const = 0;
    virtual void connectInputs(int senderId, int relevantOutputId, int inputNumber = 0) = 0;
    virtual void connectOutput(std::unique_ptr<Component> const& component, int outputNumber = -1, int inputNumber = -1) = 0;
    virtual void unConnectOutput(int outputNumber) = 0;
    virtual void setInputs(int id, int value) = 0;
    virtual void realizeOutputs() = 0;
    virtual bool isConnected() = 0;
    virtual void setOrderOfOperation(int order) = 0;
    virtual std::vector<int> getInputIds() = 0;
    virtual std::vector<int> getOutputIds() = 0;
    virtual std::string getInfo() = 0;
    virtual std::vector<std::shared_ptr<Input>> getInputsVec() = 0;
    virtual std::vector<std::shared_ptr<Output>> getOutputsVec() = 0;
    virtual std::vector<int> getSenderIds() = 0;
    virtual std::vector<int> getReceiverIds() = 0;
    
    int getOrderOfOperation();
    int getId();
    friend std::ostream& operator<<(std::ostream& os, const Component& cp);

    std::string name;
    int orderOfOperation = -1;
    int orderSetBy = -1;
    int type = UNSPECIFIED;
};

inline
std::ostream& operator<<(std::ostream& os, const Component& cp)
{
    os << cp.name;
    return os;
}

class TableOfComponents
{
private:
    std::map<int, Component*> componentTable;
public:
    TableOfComponents();
    ~TableOfComponents();
    void addComponent(Component& component);
    void clear();
    Component* getComponent(int id);
    //std::vector<Component*> getComponentsByOrder(int orderNumber);
    void printTable();
    int getSize();
};

extern TableOfComponents table;

