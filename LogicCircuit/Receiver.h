#pragma once
#include "Component.h"

class Receiver :
    public Component
{
    using Component::Component;

private:
    std::shared_ptr<Input> input;
public:
    Receiver(int bitSize);
    ~Receiver();
    Receiver* cloneImpl() const override { return new Receiver(*this); };
    void connectOutput(std::unique_ptr<Component> const& component, int outputNumber = -1, int inputNumber = -1) override;
    void unConnectOutput(int outputNumber) override;
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
    std::string getInfo() override;

    int bitSize;
}; 
