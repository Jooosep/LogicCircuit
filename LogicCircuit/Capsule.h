#pragma once
#include "Component.h"

class Capsule :
    public Component
{
    using Component::Component;

private:
    int bitSize;
    FullTrace fulltrace;
    std::vector<std::shared_ptr<Input>> inputs;
    std::vector<std::shared_ptr<Output>> outputs;
public:
    Capsule(FullTrace fullTrace, std::string name);
    ~Capsule();
    Capsule* cloneImpl() const override { return new Capsule(*this); };
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
    std::string getTraceText();
    std::string getInfo() override;
};

