#include "Component.h"

//global table
TableOfComponents table;

//global IdContructor
IncrementingIdAssigner getIOId;

int Component::getId()
{
    return id;
}

int Component::getOrderOfOperation()
{
    return orderOfOperation;
}

Component::Component()
{

}

Component::~Component()
{
}

TableOfComponents::TableOfComponents()
{
}

TableOfComponents::~TableOfComponents()
{
}

void TableOfComponents::addComponent(Component& component)
{
    int id = component.getId();
    componentTable[id] = &component;
}
void TableOfComponents::clear()
{
    componentTable.clear();
}


Component* TableOfComponents::getComponent(int id)
{
    if (componentTable.find(id) == componentTable.end())
    {
        std::cout << "component with id " << id << " not found" << std::endl;
    }
    return componentTable[id];
}

void TableOfComponents::printTable()
{
    for (auto it = componentTable.cbegin(); it != componentTable.cend(); ++it)
    {
        std::cout << it->first << " " << *(it->second) << "\n";
    }
}
/*
std::vector<Component*> TableOfComponents::getComponentsByOrder(int orderNumber)
{
    std::vector<Component*> vec;

    for (auto const&[key, val] : componentTable)
    {
        if (val->getOrderOfOperation() == orderNumber)
        {
            vec.push_back(val);
        }
    }
    return vec;
}
*/
int TableOfComponents::getSize()
{
    int i = 0;
    for (auto& comp : componentTable)
    {
        if (comp.second->isConnected())
        {
            i++;
        }
    }
    return  i;
}