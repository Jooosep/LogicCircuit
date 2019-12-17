#pragma once
#include "Component.h"
class Listener :
    public Component
{
    using Component::Component;

private:

    int numberOfInputs;
    std::vector<Input> inputs;
public:
    Listener(int numberOfInputs);
    ~Listener();

    void setInputs(int id, int value) override;
    void connectInputs(int senderId, int relevantOutputId, int inputNumber) override;
    bool isConnected() override;
    void displayInputs();
    void realizeOutputs() override;
    void setOrderOfOperation(int order) override;
    
};

