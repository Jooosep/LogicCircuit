#include "SpecialEffects.h"



SpecialEffects::SpecialEffects(sf::RenderWindow* window)
    :window(window)
{
    timer.restart();
}


SpecialEffects::~SpecialEffects()
{
}

SpecialEffects* SpecialEffects::fade(sf::Int32 duration, bool fadeAway, std::vector<sf::VertexArray*> vArrays, sf::Int32 startAfterDelay)
{
    std::vector<sf::VertexArray>storedVArray;
    std::vector<sf::VertexArray*>storedVArrayPtrs;
    if (fadeAway)
    {
        for (auto& ptr : vArrays)
        {
            storedVArray.push_back(*ptr);

        }
        vArrayList.push_back(storedVArray);

        for (auto& va : vArrayList.back())
        {
            storedVArrayPtrs.push_back(&va);
        }


        fades.emplace_back(timer.getElapsedTime().asMilliseconds(), duration, fadeAway, storedVArrayPtrs, startAfterDelay);
    }
    else
    {
        fades.emplace_back(timer.getElapsedTime().asMilliseconds(), duration, fadeAway, vArrays, startAfterDelay);
    }
    return this;
}

SpecialEffects* SpecialEffects::translate(sf::Int32 duration, bool fadeAway, sf::Vector2f targetPos, std::vector<sf::VertexArray*> vArrays, sf::Int32 startAfterDelay)
{
    std::vector<sf::VertexArray>storedVArray;
    std::vector<sf::VertexArray*>storedVArrayPtrs;
    if (fadeAway)
    {
        for (auto& ptr : vArrays)
        {
            storedVArray.push_back(*ptr);

        }
        vArrayList.push_back(storedVArray);

        for (auto& va : vArrayList.back())
        {
            storedVArrayPtrs.push_back(&va);
        }


        fades.emplace_back(timer.getElapsedTime().asMilliseconds(), duration, fadeAway, storedVArrayPtrs, startAfterDelay);
    }
    else
    {
        fades.emplace_back(timer.getElapsedTime().asMilliseconds(), duration, fadeAway, vArrays, startAfterDelay);
    }
    return this;
}

SpecialEffects* SpecialEffects::colorTransition(sf::Int32 duration, bool fadeAway, sf::Color targetColor, std::vector<sf::VertexArray*> vArrays, sf::Int32 startAfterDelay)
{
    std::vector<sf::VertexArray>storedVArray;
    std::vector<sf::VertexArray*>storedVArrayPtrs;
    if (fadeAway)
    {
        for (auto& ptr : vArrays)
        {
            storedVArray.push_back(*ptr);     
        }
        vArrayList.push_back(storedVArray);
        for (auto& va : vArrayList.back())
        {
            storedVArrayPtrs.push_back(&va);
        }

        colorTransitions.emplace_back(timer.getElapsedTime().asMilliseconds(), duration, fadeAway, targetColor, storedVArrayPtrs, startAfterDelay);
    }
    else
    {
        colorTransitions.emplace_back(timer.getElapsedTime().asMilliseconds(), duration, fadeAway, targetColor, vArrays, startAfterDelay);
    }
    return this;
}
SpecialEffects* SpecialEffects::zoom(sf::Int32 duration, float zoom, sf::Int32 startAfterDelay)
{
    return this;
}
void SpecialEffects::refresh()
{

    refresh<Fade>(fades);
    refresh<ColorTransition>(colorTransitions);
}

template<class T> void SpecialEffects::refresh(std::vector<T> vec)
{
    auto i = std::begin(vec);
    int j = 0;
    std::vector<int> itVec;

    while (i != std::end(vec))
    {
        
        sf::VertexArray* va = (*i)(timer.getElapsedTime().asMilliseconds());
        if (va != nullptr)
        {
            if ((*i).fadeAway)
            {
                window->draw(*va);
            }
        }
        else
        {
            itVec.push_back(j);
        }
        ++i;
        ++j;
    }

    for (std::vector<int>::reverse_iterator it = itVec.rbegin(); it != itVec.rend(); ++it) 
    {
        vec.erase(vec.begin() + *it);
    }
}
template<> void SpecialEffects::refresh(std::vector<Translation> vec)
{
    auto i = std::begin(vec);
    int j = 0;
    std::vector<int> itVec;

    while (i != std::end(vec))
    {

        sf::VertexArray* va = (*i)(timer.getElapsedTime().asMilliseconds());
        if (va != nullptr)
        {
            if ((*i).fadeAway)
            {
                window->draw(*va, i->transform);
            }
        }
        else
        {
            itVec.push_back(j);
        }
        ++i;
        ++j;
    }

    for (std::vector<int>::reverse_iterator it = itVec.rbegin(); it != itVec.rend(); ++it)
    {
        vec.erase(vec.begin() + *it);
    }
}