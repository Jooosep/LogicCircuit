#pragma once
#include <SFML/graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <fstream>
#include <vector>
#include <list>
#include <math.h>
#include <map>
#include <iostream>
#include <random>
#include <algorithm>
#include <memory>
#include <array>

struct Fade
{
    sf::Int32 duration;
    sf::Int32 startingTime;
    sf::Int32 delay;
    bool fadeAway;
    std::vector<sf::VertexArray*> vArrays;

    sf::VertexArray* operator()(sf::Int32 elapsedMs)
    {
        if (delay > 0)
        {
            delay -= elapsedMs;
        }
        else
        {
            for (auto& va : vArrays)
            {
                for (int i = 0; i < va->getVertexCount(); i++)
                {
                    if (fadeAway)
                        (*va)[i].color.a = (1.f - ((float(elapsedMs - startingTime) / float(duration)))) * 255;
                }
            }
            if (elapsedMs - startingTime < duration)
            {
                return vArrays[0];
            }
            else
            {
                return  nullptr;
            }
        }
    }
    Fade(sf::Int32 elapsedMs, sf::Int32 duration, bool fadeAway, std::vector<sf::VertexArray*> vArrays, sf::Int32 startAfterDelay = 0)
        :startingTime(elapsedMs), duration(duration), fadeAway(fadeAway), vArrays(vArrays), delay(startAfterDelay)
    {
    }
};

struct Translation
{
    sf::Int32 duration;
    sf::Int32 startingTime;
    sf::Int32 delay;
    bool fadeAway;
    std::vector<sf::VertexArray*> vArrays;
    sf::Vector2f target;
    sf::Transform transform;
    sf::Vector2f diff;

    sf::VertexArray* operator()(sf::Int32 elapsedMs)
    {
        if (delay > 0)
        {
            delay -= elapsedMs;
        }
        else
        {
            float multiplier = (((float(elapsedMs - startingTime) / float(duration)))) * 1.f;
            transform.translate(sf::Vector2f(diff.x * multiplier, diff.y * multiplier));

            if (elapsedMs - startingTime < duration)
            {
                return vArrays[0];
            }
            else
            {
                return  nullptr;
            }
        }

    }
    Translation(sf::Int32 elapsedMs, sf::Int32 duration, bool fadeAway, sf::Vector2f target, std::vector<sf::VertexArray*> vArrays, sf::Int32 startAfterDelay = 0)
        :startingTime(elapsedMs), duration(duration), fadeAway(fadeAway), target(target), vArrays(vArrays), delay(startAfterDelay)
    {
        transform.translate(sf::Vector2f(0.f, 0.f));
        diff = target - this->vArrays[0]->operator[](0).position;
    }
};

struct ColorTransition
{
    sf::Int32 duration;
    sf::Int32 startingTime;
    sf::Int32 delay;
    bool fadeAway;
    std::vector<sf::VertexArray*> vArrays;
    int rDiff;
    int gDiff;
    int bDiff;
    int aDiff;

    sf::VertexArray* operator()(sf::Int32 elapsedMs)
    {
        if (delay > 0)
        {
            delay -= elapsedMs;
        }
        else
        {
            for (auto& va : vArrays)
            {
                for (int i = 0; i < va->getVertexCount(); i++)
                {
                    (*va)[i].color.r += (((float(elapsedMs - startingTime) / float(duration)))) * rDiff;
                    (*va)[i].color.g += (((float(elapsedMs - startingTime) / float(duration)))) * gDiff;
                    (*va)[i].color.b += (((float(elapsedMs - startingTime) / float(duration)))) * bDiff;
                    (*va)[i].color.a += (((float(elapsedMs - startingTime) / float(duration)))) * aDiff;
                }

            }
            if (elapsedMs - startingTime < duration)
            {
                return vArrays[0];
            }
            else
            {
                return  nullptr;
            }
        }
      
    }
    ColorTransition(sf::Int32 elapsedMs, sf::Int32 duration, bool fadeAway, sf::Color target, std::vector<sf::VertexArray*> vArrays, sf::Int32 startAfterDelay = 0)
        :startingTime(elapsedMs), duration(duration), fadeAway(fadeAway), vArrays(vArrays), delay(startAfterDelay)
    {
        sf::Color currentColor = (*(this->vArrays[0]))[0].color;
        rDiff = target.r - currentColor.r;
        gDiff = target.g - currentColor.g;
        bDiff = target.b - currentColor.b;
        aDiff = target.a - currentColor.a;
    }


};

class SpecialEffects
{
public:
    SpecialEffects(sf::RenderWindow* window);
    ~SpecialEffects();
    SpecialEffects* fade(sf::Int32 duration, bool fadeAway, std::vector<sf::VertexArray*> vArrays, sf::Int32 startAfterDelay = 0);
    SpecialEffects* translate(sf::Int32 duration, bool fadeAway, sf::Vector2f targetPos, std::vector<sf::VertexArray*> vArrays, sf::Int32 startAfterDelay = 0);
    SpecialEffects* colorTransition(sf::Int32 duration, bool fadeAway, sf::Color targetColor, std::vector<sf::VertexArray*> vArrays, sf::Int32 startAfterDelay = 0);
    SpecialEffects* zoom(sf::Int32 duration, float zoom, sf::Int32 startAfterDelay = 0);
    void refresh();

private:
    template<class T> void refresh(std::vector<T> vec);

    std::list<std::vector<sf::VertexArray>> vArrayList;
    std::vector<Fade> fades;
    std::vector<Translation> translations;
    std::vector<ColorTransition> colorTransitions;
    sf::Clock timer;
    sf::RenderWindow* window;
};

