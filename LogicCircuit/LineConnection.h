#pragma once
#include "Component.h"
class LineConnection :
    public Component
{
    using Component::Component;

private:
    Input input = Input(0, -1, 16);
    std::vector<Output> outputs;
public:
    LineConnection();
    ~LineConnection();
    LineConnection* cloneImpl() const override { return new LineConnection(*this); };
    void connectInputs(int senderId, int relevantOutputId, int inputNumber);
    void connectOutput(std::unique_ptr<Component> const& component, int outputNumber = -1, int inputNumber = -1) override;
    void unConnectOutput(int outputNumber) override;
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
    std::string getInfo() override;
    bool getInput();
};

