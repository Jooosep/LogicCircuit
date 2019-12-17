#pragma once
#include "Component.h"

class Controller :
    public Component
{
    using Component::Component;

private:
    std::shared_ptr<Output> output;
public:
    Controller(int bitSize);
    ~Controller();
    Controller* cloneImpl() const override { return new Controller(*this); };
    void connectOutput(std::unique_ptr<Component> const& component, int outputNumber = -1, int inputNumber = -1) override;
    void unConnectOutput(int outputNumber) override;
    void setOutput(int value);
    void invertOutput();
    int getOutput();
    void realizeOutputs() override;
    void disconnect();
    void setInputs(int id, int value) override;
    void connectInputs(int senderId, int relevantOutputId, int inputNumber) override;
    bool isConnected() override;
    void setOrderOfOperation(int order) override;
    std::vector<int> getOutputIds() override;
    std::vector<int> getInputIds() override;
    std::vector<std::shared_ptr<Input>> getInputsVec() override;
    std::vector<std::shared_ptr<Output>> getOutputsVec() override;
    std::vector<int> getSenderIds() override;
    std::vector<int> getReceiverIds() override;
    int getValue();
    void setValue(int value);
    std::string getInfo() override;

    int bitSize;
};

