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
#include "Explosion.h"
#include "MorphingVertex.h"
#include "Cube.h"
#include "Wind.h"
#include "Fog.h"
#include "Rain.h"
#include "ParticleRoom.h"
#include "IdAssigner.h"
#include "Component.h"
#include "Controller.h"
#include "Capsule.h"
#include "Receiver.h"
#include "LineConnection.h"
#include "Listener.h"
#include "SfmlKeyToStr.h"
#include <windows.h>
#include <typeinfo>
#include "Gate.h"
#include "BitArray.h"
#include "SpecialEffects.h"
#include <filesystem>
#include <boost/algorithm/string.hpp> 
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/pending/indirect_cmp.hpp>
#define WEST 0
#define NORTH 1
#define EAST 2
#define SOUTH 3


enum CompassPoints { west, north, east, south };
const unsigned int width = 1600;
const unsigned int height = 1000;
sf::Vector2f windowCenter = sf::Vector2f(width / 2.f, height / 2.f);
sf::Vector2f mouse;
sf::Font font;
sf::Font courier;

float separatorTopEdge = height - 0.09 * height;
float separatorBotEdge = height - 0.08 * height;
sf::Vector2f previousMouse;
const float gateWidth = width * 0.025;
const float gateTopEdge = height - 0.06 * height;
const float gateBotEdge = height - 0.04 * height;
const float gateHeight = width * 0.025;
//const sf::Vector2f west = sf::Vector2f(0.f, gateHeight * 0.5f);
//const sf::Vector2f east = sf::Vector2f(gateWidth, gateHeight * 0.5f);
//const sf::Vector2f north = sf::Vector2f(gateWidth * 0.5f, 0);
//const sf::Vector2f south = sf::Vector2f(gateWidth * 0.5f, gateHeight);

const float controlPointSide = gateWidth / 4.f;
const float IOSideLength = gateWidth / 8.f;
const float EnlargedIOSideLength = gateWidth / 4.f;
const sf::Vector2f cp0 = sf::Vector2f(-0.5f * controlPointSide, -0.5f * controlPointSide);
const sf::Vector2f cp1 = sf::Vector2f(-0.5f * controlPointSide, 0.5f * controlPointSide);
const sf::Vector2f cp2 = sf::Vector2f(0.5f * controlPointSide, 0.5f * controlPointSide);
const sf::Vector2f cp3 = sf::Vector2f(0.5f * controlPointSide, -0.5f * controlPointSide);
//std::vector<sf::Vector2f> gatePositions = { west , north , east, south };
std::vector<sf::Vector2f> cpPositions = { cp0 , cp1 , cp2, cp3 };

const float controllerSide = width * 0.03;

bool LMB = false; // is left mouse button hit ?
bool LMBRelease = false;
bool RMBRelease = false;
bool Left = false;
bool Right = false;
bool Up = false;
bool Down = false;
bool Z = false;
bool ZUp = false;
bool S = false;
bool C = false;
bool R = false;
bool CRelease = false;
bool RRelease = false;
bool ERelease = false;
bool TRelease = false;
bool Space = false;
bool Shift = false;
bool ORelease = false;
bool IRelease = false;
std::vector<bool> FKeyRelease = { false, false, false, false, false, false, false, false, false, false, false, false };
bool Alt = false;
bool Delete = false;
bool Tab = false;
bool ScrollUp = false;
bool ScrollDown = false;

template <size_t> struct fib;

SfmlKeyToStr keyToStr;

//Báse cases
template <> struct fib<0> { const static auto value = 1; };
template<> struct fib<1> { const static auto value = 1; };

struct GetSideMidPoint
{
    GetSideMidPoint(float height, float width) 
        : height(height), width(width) 
    {}

    sf::Vector2f operator()(int y) const 
    { 
        switch (y)
        {
        case 0:
            return west;
        case 1:
            return north;
        case 2:
            return east;
        case 3:
            return south;
        }
    }

    private:
        float height; float width;
        sf::Vector2f west = sf::Vector2f(0.f, height * 0.5f);
        sf::Vector2f east = sf::Vector2f(width, height * 0.5f);
        sf::Vector2f north = sf::Vector2f(width * 0.5f, 0);
        sf::Vector2f south = sf::Vector2f(width * 0.5f, height);
};
struct GetSelectorInputPosition
{
    GetSelectorInputPosition()
    {
    }
    GetSelectorInputPosition(float gateLength, float IOlength)

    {
        pos = sf::Vector2f(gateLength / 2.f, gateLength - IOlength);
    }

    sf::Vector2f operator()() const
    {
        return pos;
    }

    sf::Vector2f pos;
};

struct IOSet
{
    sf::VertexArray IOvertexArray;
    sf::VertexArray enlargedIOVertexArray;
    int nOfIO;
    sf::VertexArray backGround;
    CompassPoints location;

    IOSet()
    {

    }

    IOSet(int nOfIO,
        sf::VertexArray backGround,
        CompassPoints location)
        :
        IOvertexArray(sf::VertexArray(sf::Quads, nOfIO * 4)),
        enlargedIOVertexArray(sf::VertexArray(sf::Quads, nOfIO * 4)),
        nOfIO(nOfIO),
        backGround(backGround),
        location(location)
    {
    }

    void colorize(int i, sf::Color color)
    {
        int index = i * 4;
        enlargedIOVertexArray[index++].color = color;
        enlargedIOVertexArray[index++].color = color;
        enlargedIOVertexArray[index++].color = color;
        enlargedIOVertexArray[index].color = color;
    }

};

struct IOVertexArrays
{
    IOVertexArrays()
    {

    }
    IOVertexArrays(sf::FloatRect box, int nOfInputs, int nOfOutputs, bool hasSelector)
    {
        sf::Vector2f relativePoint = sf::Vector2f(box.left, box.top);
        sf::VertexArray west = sf::VertexArray(sf::Quads, 4);
        sf::VertexArray north = sf::VertexArray(sf::Quads, 4);
        sf::VertexArray east = sf::VertexArray(sf::Quads, 4);
        sf::VertexArray south = sf::VertexArray(sf::Quads, 4);
        float scale = 2.2f;
        float addedLength = 0.0f;
        
        if (box.height > box.width)
        {
            addedLength = (box.height - box.width) * (1.02f);
        }
        
        west[0].position = sf::Vector2f(box.left - (box.width * 0.25f + addedLength), box.top - (((scale - 1.f)* 0.5f)* box.height));
        west[1].position = west[0].position + sf::Vector2f(0.f, box.height * scale);
        west[2].position = west[1].position + sf::Vector2f((box.width + (addedLength * 2.f)) * 0.5f, 0.f);
        west[3].position = west[0].position + sf::Vector2f((box.width + (addedLength * 2.f)) * 0.5f, 0.f);
        sf::Vector2f center = sf::Vector2f(box.left + box.width / 2.f, box.top + box.height / 2.f);
        sf::Transform transform;
        transform.rotate(90.f, center);

        for (int i = 0; i < 4; i++)
        {
            north[i].position = transform.transformPoint(west[i].position);
        }
        for (int i = 0; i < 4; i++)
        {
            east[i].position = transform.transformPoint(north[i].position);
        }
        for (int i = 0; i < 4; i++)
        {
            south[i].position = transform.transformPoint(east[i].position);
        }
        for (int i = 0; i < 4; i++)
        {
            west[i].color = sf::Color(169, 169, 169, 255);
            north[i].color = sf::Color(169, 169, 169, 255);
            east[i].color = sf::Color(169, 169, 169, 255);
            south[i].color = sf::Color(169, 169, 169, 255);
        }

        std::vector<sf::VertexArray> bgForEnlargedIO = { west, north, east, south };
        std::vector<IOSet *> IOSetPtrs;
        inputSet = IOSet(nOfInputs, bgForEnlargedIO[WEST], CompassPoints::west);
        IOSetPtrs.push_back(&inputSet);
        outputSet = IOSet(nOfOutputs, bgForEnlargedIO[EAST], CompassPoints::east);
        IOSetPtrs.push_back(&outputSet);
        if (hasSelector)
        {
            selectorSet = IOSet(1, bgForEnlargedIO[SOUTH], CompassPoints::south);
            IOSetPtrs.push_back(&selectorSet);
        }

        for (auto& IOPtr : IOSetPtrs)
        {

            int IOCount = IOPtr->nOfIO;
            float gap = box.height / (IOCount + 1);
            for (int i = 0; i < IOCount; i++)
            {
                int index = i * 4;

                sf::Vector2f leftTop = relativePoint;
                if (IOPtr->location == CompassPoints::east)
                {
                    leftTop += sf::Vector2f(box.width - IOSideLength * 1.5f, (gap * (i + 1)) - IOSideLength * 0.5f);
                }
                else if (IOPtr->location == CompassPoints::west)
                {
                    leftTop += sf::Vector2f(IOSideLength * 0.5f, (gap * (i + 1)) - IOSideLength * 0.5f);
                }
                else if (IOPtr->location == CompassPoints::south)
                {
                    leftTop += sf::Vector2f(gap * (i + 1) - IOSideLength * 0.5f, box.height - IOSideLength);
                }

                IOPtr->IOvertexArray[index++].position = leftTop;
                IOPtr->IOvertexArray[index++].position = leftTop + sf::Vector2f(0.f, IOSideLength);
                IOPtr->IOvertexArray[index++].position = leftTop + sf::Vector2f(IOSideLength, IOSideLength);
                IOPtr->IOvertexArray[index].position = leftTop + sf::Vector2f(IOSideLength, 0.f);
            }

            sf::VertexArray bg = IOPtr->backGround;
            sf::FloatRect bgBox = bg.getBounds();
            
            float maxIOLength;
            float IOLength = 0.8f * box.height / IOPtr->nOfIO;
            
            if (bgBox.width > bgBox.height)
            {
                maxIOLength = 0.75f * bgBox.height;
                IOLength = 0.8f * bgBox.width / IOCount;
                if (IOLength > maxIOLength)
                {
                    IOLength = maxIOLength;
                }
                float gap = bgBox.width / (IOCount + 1);
                for (int i = 0; i < IOCount; i++)
                {
                    int index = i * 4;
                    sf::Vector2f leftTop = sf::Vector2f(bgBox.left + gap * (i + 1) - 0.5f * IOLength, bgBox.top + 0.5 * (bgBox.height) - 0.5 * IOLength);
                    IOPtr->enlargedIOVertexArray[index++].position = leftTop;
                    IOPtr->enlargedIOVertexArray[index++].position = leftTop + sf::Vector2f(0.f, IOLength);
                    IOPtr->enlargedIOVertexArray[index++].position = leftTop + sf::Vector2f(IOLength, IOLength);
                    IOPtr->enlargedIOVertexArray[index].position = leftTop + sf::Vector2f(IOLength, 0.f);

                }
            }
            else
            {
                maxIOLength = 0.75f * bgBox.width;
                IOLength = 0.8f * bgBox.height / IOCount;
                if (IOLength > maxIOLength)
                {
                    IOLength = maxIOLength;
                }
                float gap = bgBox.height / (IOCount + 1);
                for (int i = 0; i < IOCount; i++)
                {
                    int index = i * 4;
                    sf::Vector2f leftTop = sf::Vector2f(bgBox.left + 0.5 * (bgBox.width) - 0.5 * IOLength, bgBox.top + gap * (i + 1) - 0.5f * IOLength);
                    IOPtr->enlargedIOVertexArray[index++].position = leftTop;
                    IOPtr->enlargedIOVertexArray[index++].position = leftTop + sf::Vector2f(0.f, IOLength);
                    IOPtr->enlargedIOVertexArray[index++].position = leftTop + sf::Vector2f(IOLength, IOLength);
                    IOPtr->enlargedIOVertexArray[index].position = leftTop + sf::Vector2f(IOLength, 0.f);
                }
            }
        }

        for (int i = 0; i < inputSet.IOvertexArray.getVertexCount(); i++)
        {
            inputSet.IOvertexArray[i].color = sf::Color::White;
            inputSet.enlargedIOVertexArray[i].color = sf::Color::White;
        }
        for (int i = 0; i < outputSet.IOvertexArray.getVertexCount(); i++)
        {
            outputSet.IOvertexArray[i].color = sf::Color::Black;
            outputSet.enlargedIOVertexArray[i].color = sf::Color::White;
        }
        for (int i = 0; i < selectorSet.IOvertexArray.getVertexCount(); i++)
        {
            selectorSet.IOvertexArray[i].color = sf::Color::Cyan;
            selectorSet.enlargedIOVertexArray[i].color = sf::Color::White;
        }
    }

    IOSet inputSet;
    IOSet outputSet;
    IOSet selectorSet;
};

sf::VertexArray getSquareVertexArray(sf::Vector2f center, float side, sf::Color color)
{
    sf::VertexArray va = sf::VertexArray(sf::Quads, 4);
    va[0].position = sf::Vector2f(center.x - (0.5* side), center.y - (0.5* side));
    va[1].position = sf::Vector2f(center.x - (0.5* side), center.y + (0.5* side));
    va[2].position = sf::Vector2f(center.x + (0.5* side), center.y + (0.5* side));
    va[3].position = sf::Vector2f(center.x + (0.5* side), center.y - (0.5* side));
    for (int i = 0; i < 4; i++)
    {
        va[i].color = color;
    }
    return va;
}

sf::VertexArray getCapsuleVertexArray(sf::Vector2f center, float height, float width, sf::Color color)
{
    sf::VertexArray va = sf::VertexArray(sf::Quads, 4);
    va[0].position = sf::Vector2f(center.x - (0.5* width), center.y - (0.5* height));
    va[1].position = sf::Vector2f(center.x - (0.5* width), center.y + (0.5* height));
    va[2].position = sf::Vector2f(center.x + (0.5* width), center.y + (0.5* height));
    va[3].position = sf::Vector2f(center.x + (0.5* width), center.y - (0.5* height));
    for (int i = 0; i < 4; i++)
    {
        va[i].color = color;
    }
    return va;
}

struct ValueAdjuster
{
    ValueAdjuster()
    {

    }
    ValueAdjuster(sf::FloatRect box, int value, int bitSize)
    {
        bg = sf::VertexArray(sf::Quads, 4);
        bg[0].position = sf::Vector2f(box.left - box.width * 0.6f, box.top + box.height * 0.1f);
        bg[1].position = bg[0].position + sf::Vector2f(0.f, box.height * 0.8f);
        bg[2].position = bg[1].position + sf::Vector2f(box.width  * 2.2f, 0.f);
        bg[3].position = bg[0].position + sf::Vector2f(box.width  * 2.2f, 0.f);

        for (int i = 0; i < 4; i++)
        {
            bg[i].color = sf::Color(150, 150, 150, 255);
        }


        std::string val = std::bitset<32>(value).to_string().substr(31 - bitSize, bitSize);
        if (val.length() > 8)
        {
            val.insert(8, "\n");
        }
        if (val.length() > 16)
        {
            val.insert(16, "\n");
        }
        if (val.length() > 24)
        {
            val.insert(24, "\n");
        }
        sf::Text text;
        text.setCharacterSize(box.width / 2.5f);
        text.setFont(courier);
        text.setString(val);
        text.setFillColor(sf::Color::Black);
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        text.setPosition(sf::Vector2f(box.left + box.width / 2.f, box.top + box.height / 2.f));

        valueText = text;
        sf::String str = valueText.getString();
        for (int i = 0; i < str.getSize(); i++)
        {
            if (str[i] != '\n')
            {
                charPositions.push_back(valueText.findCharacterPos(i));
            }
        }

    }

    int clicked(sf::Vector2f mouse, sf::Transform& transform)
    {

        for (int i = 0; i < charPositions.size(); i++)
        {
            sf::Vector2f charPos = transform.transformPoint(charPositions[i]);
            auto textBox = transform.transformRect(valueText.getGlobalBounds());
            
            if (i != 0 && (i + 1) % 8 == 0 || i == charPositions.size() - 1)
            {
                if ((mouse.x > charPos.x && mouse.x < textBox.left + textBox.width) && mouse.y > charPos.y && mouse.y < charPos.y + valueText.getCharacterSize())
                {
                    return i;
                }
            }
            else
            {
                sf::Vector2f nextCharPos = transform.transformPoint(charPositions[i + 1]);
                if ((mouse.x > charPos.x && mouse.x < nextCharPos.x) && mouse.y > charPos.y && mouse.y < charPos.y + valueText.getCharacterSize())
                {
                    return i;
                }
            }

        } 
        /*
        if (charPositions.size() == 16)
        {
            auto textBox = transform.transformRect(valueText.getGlobalBounds());
            sf::Vector2f charPos = transform.transformPoint(charPositions[7]);
            if ((mouse.x > charPos.x && mouse.x < textBox.left + textBox.width) && mouse.y > charPos.y && mouse.y < (textBox.top + textBox.height / 2))
            {
                    return 7;
            }
            charPos = transform.transformPoint(charPositions[15]);
            if ((mouse.x > charPos.x && mouse.x < textBox.left + textBox.width) && mouse.y > charPos.y && mouse.y < (textBox.top + textBox.height))
            {
                return 15;
            }
        }
        */

        return -1;
    }

    sf::Text valueText;
    sf::VertexArray bg;
    std::vector<sf::Vector2f> charPositions;

};

struct GetPartialRect
{
    GetPartialRect()
    {
    }

    GetPartialRect(sf::Vector2f leftTop, float width, float height)
        : length(length)
    {
        west.left = leftTop.x;
        west.top = leftTop.y + 0.25f * height;
        west.width = 0.33f * width;
        west.height = 0.5 * height;

        east.left = leftTop.x + 0.75f * width;
        east.top = leftTop.y + 0.25f * height;
        east.width = 0.33f * width;
        east.height = 0.5f * height;

        north.left = leftTop.x + 0.25f * width;
        north.top = leftTop.y;
        north.width = 0.5f * width;
        north.height = 0.33f * height;

        south.left = leftTop.x + 0.25f * width;
        south.top = leftTop.y + 0.75f * height;
        south.width = 0.5f * width;
        south.height = 0.33f * height;
    }

    sf::FloatRect operator()(int y) const
    {
        switch (y)
        {
        case 0:
            return west;
        case 1:
            return north;
        case 2:
            return east;
        case 3:
            return south;
        }
    }

private:
    float length;
    sf::FloatRect west;
    sf::FloatRect east;
    sf::FloatRect north;
    sf::FloatRect south;
};

template <size_t n> struct fib
{
    const static auto value = fib<n - 1>::value + fib<n - 2>::value;
};

sf::Vector2f getMidPoint(sf::Vector2f pos1, sf::Vector2f pos2)
{
    return sf::Vector2f(pos2.x - (0.5f * (pos2.x - pos1.x)), pos2.y - (0.5f * (pos2.y - pos1.y)));
}

struct OutputWire
{
    int id;
    int cp;

    OutputWire(int id, int cp)
    {
        this->id = id;
        this->cp = cp;
    }

};

struct WireStartingPoint
{
    int componentId;
    int outputNumber;

    void set(int componentId, int outputNumber)
    {
        this->componentId = componentId;
        this->outputNumber = outputNumber;
    }
};

struct Wire
{
    sf::VertexArray lineVertex;
    std::vector<std::pair<sf::VertexArray, sf::Transform>> controlPointVertices;

    std::unique_ptr<Component> wireComponent;
    WireStartingPoint inputComponent;
    std::vector<int> outputIds;

    std::vector<OutputWire> outputWires;
    
    ~Wire() = default;

    Wire(const Wire& other)
        :wireComponent(other.wireComponent->clone()),
        lineVertex(other.lineVertex),
        controlPointVertices(other.controlPointVertices),
        inputComponent(other.inputComponent),
        outputIds(other.outputIds),
        outputWires(other.outputWires)
    {
        std::cout << "Wire copy construct" << std::endl;
    }

    Wire(Wire && other) = default;

    Wire& operator=(Wire const& other)
    {
        std::cout << "Wire copy assign" << std::endl;
        this->lineVertex = other.lineVertex;
        this->controlPointVertices = other.controlPointVertices;
        this->inputComponent = other.inputComponent;
        this->outputIds = other.outputIds;
        this->outputWires = other.outputWires;
        this->wireComponent = other.wireComponent->clone();

        return *this;
    }

    Wire& operator=(Wire && other) = default;

    Wire(sf::VertexArray lineVertex = sf::VertexArray(sf::LinesStrip, 2), LineConnection wireComponent = LineConnection())
        :lineVertex(lineVertex), wireComponent(std::make_unique<LineConnection>(wireComponent))
    {

        sf::VertexArray cPVertex = sf::VertexArray(sf::Quads, 4);
        sf::Vector2f midPoint = getMidPoint(lineVertex[0].position, lineVertex[1].position);

        for (int i = 0; i < 4; i++)
        {
            cPVertex[i].position = midPoint + cpPositions[i];
            cPVertex[i].color = sf::Color::White;
        }
        controlPointVertices.emplace_back(std::pair<sf::VertexArray, sf::Transform>{cPVertex, sf::Transform::Transform()});

        this->lineVertex.append(sf::Vertex(lineVertex[1].position));
        this->lineVertex[1].position = midPoint;

        for (int i = 0; i < this->lineVertex.getVertexCount(); i++)
        {
            this->lineVertex[i].color = sf::Color::White;
        }
    }

    virtual void realizeOutputs() const
    {
        this->wireComponent->realizeOutputs();
    }

    bool getInput() 
    {
        LineConnection * line = static_cast<LineConnection *>(&(*(this->wireComponent)));
        return line->getInput();
    }
    sf::FloatRect getGlobalBounds(int index)
    {
        return controlPointVertices[index].second.transformRect(controlPointVertices[index].first.getBounds());
    }
};

struct PlantedGate 
{
    sf::VertexArray vArray;
    IOVertexArrays IOVArrays;
    sf::VertexArray valueVArray;
    ValueAdjuster valueAdjuster;
    bool valueAdjusterActive;
    bool inputsEnlarged;
    bool outputsEnlarged;
    bool selectorEnlarged;
    bool hasSelector;
    sf::Transform transform;
    std::unique_ptr<Component> component;
    //std::vector<int> inputIds;
    //std::vector<int> outputIds;

    GetPartialRect getPartialRect;

    ~PlantedGate()
    {
        component.reset();
    }

    PlantedGate(const PlantedGate& other)
        :component(other.component->clone()),
        vArray(other.vArray),
        IOVArrays(other.IOVArrays),
        valueAdjuster(other.valueAdjuster),
        valueAdjusterActive(other.valueAdjusterActive),
        inputsEnlarged(other.inputsEnlarged),
        outputsEnlarged(other.outputsEnlarged),
        selectorEnlarged(other.selectorEnlarged),
        hasSelector(other.hasSelector),
        transform(other.transform),
        //inputIds(other.inputIds),
        //outputIds(other.outputIds),
        getPartialRect(other.getPartialRect)
    {
        std::cout << "Gate copy construct" << std::endl;
    }

    PlantedGate(PlantedGate && other) = default;

    PlantedGate& operator=(PlantedGate const& other)
    {
        std::cout << "plantedGate copy assign" << std::endl;
        this->vArray = other.vArray;
        this->IOVArrays = other.IOVArrays;
        this->valueVArray = other.valueVArray;
        this->valueAdjuster = other.valueAdjuster;
        this->valueAdjusterActive = other.valueAdjusterActive;
        this->inputsEnlarged = other.inputsEnlarged;
        this->outputsEnlarged = other.outputsEnlarged;
        this->selectorEnlarged = other.selectorEnlarged;
        this->hasSelector = other.hasSelector;
        this->transform = other.transform;
        this->component =other.component->clone();
        //this->inputIds = other.inputIds;
        //this->outputIds = other.outputIds;
        this->getPartialRect = other.getPartialRect;

        return *this;
    }

    PlantedGate& operator=(PlantedGate && other) = default;

    PlantedGate(sf::VertexArray vArray = sf::VertexArray(sf::Quads, 4), Gate component = Gate(NOTGATE))
        :vArray(vArray), component(std::make_unique<Gate>(Gate(component)))
    {
        this->getPartialRect = GetPartialRect(vArray[0].position, gateHeight, gateHeight);
        this->valueAdjusterActive = false;
        Gate& gt = dynamic_cast<Gate&>(*(this->component));
        this->hasSelector = gt.hasSelector();
        int nOfNormalInputs = component.getInputIds().size() - ((this->hasSelector) ? 1 : 0);
        this->IOVArrays = IOVertexArrays(vArray.getBounds(), nOfNormalInputs, component.getOutputIds().size(), this->hasSelector);
    }

    virtual std::string getInfo() const
    {
        return this->component->getInfo();
    }
    virtual int getId() const
    {
        return this->component->getId();
    }

    virtual std::string getName() const
    {
        return this->component->name;
    }
    virtual void realizeOutputs() const
    {
        this->component->realizeOutputs();
    }
    virtual void setOrderOfOperation(int order) 
    {
        this->component->setOrderOfOperation(order);
    }
    virtual int getNoOfInputs() const
    {
        return this->component->getInputIds().size();
    }
    virtual int getNoOfOutputs() const
    {
        return this->component->getOutputIds().size();
    }
    virtual std::vector<std::shared_ptr<Input>> getInputsVec() const
    {
        return this->component->getInputsVec();
    }
    virtual std::vector<std::shared_ptr<Output>> getOutputsVec() const
    {
        return this->component->getOutputsVec();
    }
    virtual void disconnectOutput(int outputNumber)
    {
        this->component->unConnectOutput(outputNumber);
    }
    virtual float getWidth() const
    {
        return vArray[3].position.x - vArray[0].position.x;
    }
    virtual float getHeight() const
    {
        return vArray[1].position.y - vArray[0].position.y;
    }
    virtual void unConnectOutput(int outputNumber)
    {
        this->component->unConnectOutput(outputNumber);
    }
    virtual void onShiftLeftClick()
    {

    }
    virtual sf::FloatRect getGlobalBounds()
    {
        return transform.transformRect(vArray.getBounds());
    }
    virtual std::vector<int> getSenderIds()
    {
        return this->component->getSenderIds();
    }
    virtual std::vector<int> getReceiverIds()
    {
        return this->component->getReceiverIds();
    }
    virtual void checkValueInteraction(sf::Vector2f mouse, sf::RenderWindow* window)
    {

    }

    virtual bool outputBoundsContainsMouse(int index)
    {
        if (!outputsEnlarged)
        {
            return false;
        }
        else
        {
            sf::VertexArray totalOut = IOVArrays.outputSet.enlargedIOVertexArray;
            sf::VertexArray out = sf::VertexArray(sf::Quads, 4);
            for (int i = 0; i < 4; i++)
            {
                out[i].position = totalOut[index * 4 + i].position;
            }
            sf::FloatRect output = transform.transformRect(out.getBounds());
            if (output.contains(mouse))
            {
                IOVArrays.outputSet.colorize(index, sf::Color::Magenta);
                return true;
            }
            else if (this->component->getOutputsVec()[index]->receiverId != -1)
            {
                IOVArrays.outputSet.colorize(index, sf::Color::Color(70, 70, 70, 255));
                return false;
            }
            else
            {
                IOVArrays.outputSet.colorize(index, sf::Color::White);
                return false;
            }
        }
    }

    virtual bool inputBoundsContainsMouse(int index)
    {
        if (!inputsEnlarged && !selectorEnlarged)
        {
            return false;
        }
        else
        {
            if (hasSelector && index == component->getInputIds().size() - 1)
            {
                sf::VertexArray sel = IOVArrays.selectorSet.enlargedIOVertexArray;
                sf::FloatRect selRect = transform.transformRect(sel.getBounds());
                if (selRect.contains(mouse))
                {
                    return true;
                }
            }

            sf::VertexArray totalIn = IOVArrays.inputSet.enlargedIOVertexArray;
            sf::VertexArray in = sf::VertexArray(sf::Quads, 4);
            for (int i = 0; i < 4; i++)
            {
                in[i].position = totalIn[index * 4 + i].position;
            }
            sf::FloatRect input = transform.transformRect(in.getBounds());
            if (input.contains(mouse))
            {
                IOVArrays.inputSet.colorize(index, sf::Color::Cyan);
                return true;
            }
            else if (this->component->getInputsVec()[index]->senderId != -1)
            {
                IOVArrays.inputSet.colorize(index, sf::Color::Color(70, 70, 70, 255));
                return false;
            }
            else {
                IOVArrays.inputSet.colorize(index, sf::Color::White);
                return false;
            }
        }
    }
    virtual void checkIOHover(sf::RenderWindow* window)
    {
        sf::FloatRect o;
        sf::FloatRect i;
        sf::FloatRect s;
        if (outputsEnlarged)
        {
            o = transform.transformRect(IOVArrays.outputSet.backGround.getBounds());
        }
        else
        {
            o = transform.transformRect(getPartialRect(IOVArrays.outputSet.location));
        }
        if (inputsEnlarged)
        {
            i = transform.transformRect(IOVArrays.inputSet.backGround.getBounds());
        }
        else
        {
            i = transform.transformRect(getPartialRect(IOVArrays.inputSet.location));
        }
        if (hasSelector)
        {
            if (selectorEnlarged)
            {
                s = transform.transformRect(IOVArrays.selectorSet.backGround.getBounds());
            }
            else
            {
                s = transform.transformRect(getPartialRect(IOVArrays.selectorSet.location));
            }
        }

        if (!valueAdjusterActive && i.contains(mouse) && IOVArrays.inputSet.nOfIO > 0)
        {
            inputsEnlarged = true;
            window->draw(IOVArrays.inputSet.backGround, transform);
            window->draw(IOVArrays.inputSet.enlargedIOVertexArray, transform);
            for (int index = 0; index < component->getInputIds().size() - this->hasSelector; index++)
            {
                sf::VertexArray totalIn = IOVArrays.inputSet.enlargedIOVertexArray;
                sf::VertexArray in = sf::VertexArray(sf::Quads, 4);
                for (int i = 0; i < 4; i++)
                {
                    in[i].position = totalIn[index * 4 + i].position;
                }
                sf::FloatRect input = transform.transformRect(in.getBounds());
                if (input.contains(mouse))
                {
                    IOVArrays.inputSet.colorize(index, sf::Color::Cyan);
                }
                else if (this->component->getInputsVec()[index]->senderId != -1)
                {
                    IOVArrays.inputSet.colorize(index, sf::Color::Color(70, 70, 70, 255));
                }
                else
                {
                    IOVArrays.inputSet.colorize(index, sf::Color::White);
                }
            }
        }
        else
        {
            inputsEnlarged = false;
        }

        if (!valueAdjusterActive && o.contains(mouse) && IOVArrays.outputSet.nOfIO > 0)
        {
            outputsEnlarged = true;
            window->draw(IOVArrays.outputSet.backGround, transform);
            window->draw(IOVArrays.outputSet.enlargedIOVertexArray, transform);

            for (int index = 0; index < component->getOutputIds().size(); index++)
            {
                sf::VertexArray totalOut = IOVArrays.outputSet.enlargedIOVertexArray;
                sf::VertexArray out = sf::VertexArray(sf::Quads, 4);
                for (int i = 0; i < 4; i++)
                {
                    out[i].position = totalOut[index * 4 + i].position;
                }
                sf::FloatRect output = transform.transformRect(out.getBounds());
                if (output.contains(mouse))
                {
                    IOVArrays.outputSet.colorize(index, sf::Color::Magenta);
                }
                else if (this->component->getOutputsVec()[index]->receiverId != -1)
                {
                    IOVArrays.outputSet.colorize(index, sf::Color::Color(70, 70, 70, 255));
                }
                else
                {
                    IOVArrays.outputSet.colorize(index, sf::Color::White);
                }
            }
        }
        else
        {
            outputsEnlarged = false;
        }

        if (hasSelector && !valueAdjusterActive && s.contains(mouse))
        {
            selectorEnlarged = true;
            window->draw(IOVArrays.selectorSet.backGround, transform);
            window->draw(IOVArrays.selectorSet.enlargedIOVertexArray, transform);
            
            sf::VertexArray sel = IOVArrays.selectorSet.enlargedIOVertexArray;

            sf::FloatRect selRect = transform.transformRect(sel.getBounds());
            if (selRect.contains(mouse))
            {
                IOVArrays.selectorSet.colorize(0, sf::Color::Red);
            }
            else
            {
                IOVArrays.selectorSet.colorize(0, sf::Color::White);
            }
            
        }
        else
        {
            selectorEnlarged = false;
        }
    }

    virtual sf::Vector2f getInputCenter(int i) const
    {
        
        int index = 4 * i;
        float xOffset = 0.f;
        float yOffset = 0.f;

        if (hasSelector && i == component->getInputIds().size() - 1)
        {
            yOffset = 1.f * IOSideLength;
            return transform.transformPoint(sf::Vector2f(IOVArrays.selectorSet.IOvertexArray[0].position.x + (0.5f * IOSideLength) + xOffset, IOVArrays.selectorSet.IOvertexArray[0].position.y + (0.5f * IOSideLength) + yOffset));
        }

        switch (IOVArrays.inputSet.location)
        {
            
        case WEST:
            xOffset = -1.f * IOSideLength;
            break;
        case EAST:
            xOffset = 1.f * IOSideLength;
            break;
        case NORTH:
            yOffset = -1.f * IOSideLength;
            break;
        case SOUTH:
            yOffset = 1.f * IOSideLength;
            break;
        }
        return transform.transformPoint(sf::Vector2f(IOVArrays.inputSet.IOvertexArray[index].position.x + (0.5f * IOSideLength) + xOffset, IOVArrays.inputSet.IOvertexArray[index].position.y + (0.5f * IOSideLength) + yOffset));
    }

    virtual sf::Vector2f getOutputCenter(int i) const
    {
        float xOffset = 0.f;
        float yOffset = 0.f;

        switch (IOVArrays.outputSet.location)
        {

        case WEST:
            xOffset = -1.f * IOSideLength;
            break;
        case EAST:
            xOffset = 1.f * IOSideLength;
            break;
        case NORTH:
            yOffset = -1.f * IOSideLength;
            break;
        case SOUTH:
            yOffset = 1.f * IOSideLength;
            break;
        }

        int index = 4 * i;
        return transform.transformPoint(sf::Vector2f(IOVArrays.outputSet.IOvertexArray[index].position.x + (0.5f * IOSideLength) + xOffset, IOVArrays.outputSet.IOvertexArray[index].position.y + (0.5f * IOSideLength) + yOffset));
    }

    virtual void draw(sf::RenderWindow * window)
    {
        window->draw(vArray, transform);
        window->draw(IOVArrays.inputSet.IOvertexArray, transform);
        window->draw(IOVArrays.outputSet.IOvertexArray, transform);
        if (this->hasSelector)
        {
            window->draw(IOVArrays.selectorSet.IOvertexArray, transform);
        }
    }
    virtual std::vector<sf::VertexArray*> getDrawables()
    {
        return { &vArray , &IOVArrays.inputSet.IOvertexArray, &IOVArrays.outputSet.IOvertexArray };
    }
};

struct ControllerObject : public PlantedGate
{
    ControllerObject() {};

    ControllerObject(sf::VertexArray vArray, Controller component)
    {
        this->component = std::make_unique<Controller>(component);
        this->vArray = vArray;
        this->valueAdjusterActive = false;
        //this->outputIds = {-1};
        //this->inputIds = {};
        this->valueVArray = sf::VertexArray(sf::Quads, 4);
        this->getPartialRect = GetPartialRect(vArray[0].position, controllerSide, controllerSide);

        Controller& ctr = dynamic_cast<Controller&>(*(this->component));
        this->IOVArrays = IOVertexArrays(vArray.getBounds(), component.getInputIds().size(), component.getOutputIds().size(), false);
        this->valueAdjuster = ValueAdjuster(vArray.getBounds(), ctr.getValue(), ctr.bitSize);

        sf::FloatRect selfBox = vArray.getBounds();
        valueVArray[0].position = sf::Vector2f(selfBox.left, selfBox.top);
        valueVArray[1].position = valueVArray[0].position + sf::Vector2f(0.f, selfBox.height * 0.2f);
        valueVArray[2].position = valueVArray[1].position + sf::Vector2f(selfBox.width * 0.2f, 0.f);
        valueVArray[3].position = valueVArray[0].position + sf::Vector2f(selfBox.width * 0.2f, 0.f);
        for (int i = 0; i < 4; i++)
        {
            valueVArray[i].color = sf::Color::Red;
        }
    }
    
    void onShiftLeftClick()
    {
        Controller& ctr = dynamic_cast<Controller&>(*(this->component));
        bool currentValue = ctr.getOutput();
        ctr.invertOutput();
        if (currentValue)
        {
            for (int i = 0; i < 4; i++)
            {
                vArray[i].color = sf::Color::White;
            }
        }
        else
        {
            for (int i = 0; i < 4; i++)
            {
                vArray[i].color = sf::Color::Red;
            }
        }
    }

    bool inputBoundsContainsMouse(int index)
    {
        return false;
    }

    void checkValueInteraction(sf::Vector2f mouse, sf::RenderWindow* window)
    {
        if (valueAdjusterActive && LMBRelease)
        {
            Controller& ctr = dynamic_cast<Controller&>(*(this->component));
            auto target = valueAdjuster.clicked(mouse, transform);
            if (target != -1)
            {
                int oldVal = ctr.getValue();
                int singleBit = 1 << (ctr.bitSize - target);


                ctr.setValue(oldVal ^ singleBit);
                std::cout << "value set, letterIndex: " << ctr.bitSize - target << std::endl;

                std::string val = std::bitset<32>(ctr.getValue()).to_string().substr(31 - ctr.bitSize, ctr.bitSize);
                if (val.length() > 8)
                {
                    val.insert(8, "\n");
                }
                valueAdjuster.valueText.setString(val);
            }
        }

        sf::FloatRect valueBounds = transform.transformRect(valueVArray.getBounds());
        sf::FloatRect adjusterBounds = transform.transformRect(valueAdjuster.bg.getBounds());
        if (valueBounds.contains(mouse) || (valueAdjusterActive && adjusterBounds.contains(mouse)))
        {
            Controller& ctr = dynamic_cast<Controller&>(*(this->component));
            std::string val = std::bitset<32>(ctr.getValue()).to_string().substr(31 - ctr.bitSize, ctr.bitSize);
            if (val.length() > 8)
            {
                val.insert(8, "\n");
            }
            valueAdjuster.valueText.setString(val);

            valueAdjusterActive = true;
            window->draw(valueAdjuster.bg, transform);
            window->draw(valueAdjuster.valueText, transform);
        }
        else
        {
            valueAdjusterActive = false;
        }

        
    }

    sf::FloatRect getInputPartialRect() const
    {
        sf::FloatRect rect;
        return rect;
    }

     void draw(sf::RenderWindow * window)
    {
        window->draw(vArray, transform);
        window->draw(IOVArrays.inputSet.IOvertexArray, transform);
        window->draw(IOVArrays.outputSet.IOvertexArray, transform);
        window->draw(valueVArray, transform);
    }
     std::vector<sf::VertexArray*> getDrawables()
     {
         return { &vArray , &IOVArrays.inputSet.IOvertexArray, &IOVArrays.outputSet.IOvertexArray, &valueVArray};
     }
};

struct ReceiverObject : public ControllerObject
{
    ReceiverObject() {};

    ReceiverObject(sf::VertexArray vArray, Receiver component)
    {
        this->component = std::make_unique<Receiver>(component);
        this->vArray = vArray;
        this->valueAdjusterActive = false;
        //this->outputIds = {};
        //this->inputIds = { -1 };
        this->valueVArray = sf::VertexArray(sf::Quads, 4);
        this->getPartialRect = GetPartialRect(vArray[0].position, controllerSide, controllerSide);

        Receiver& rcr = dynamic_cast<Receiver&>(*(this->component));
        this->IOVArrays = IOVertexArrays(vArray.getBounds(), component.getInputIds().size(), component.getOutputIds().size(), false);
        this->valueAdjuster = ValueAdjuster(vArray.getBounds(), rcr.getValue(), rcr.bitSize);

        sf::FloatRect selfBox = vArray.getBounds();
        valueVArray[0].position = sf::Vector2f(selfBox.left +  selfBox.width * 0.8f, selfBox.top);
        valueVArray[1].position = valueVArray[0].position + sf::Vector2f(0.f, selfBox.height * 0.2f);
        valueVArray[2].position = valueVArray[1].position + sf::Vector2f(selfBox.width * 0.2f, 0.f);
        valueVArray[3].position = valueVArray[0].position + sf::Vector2f(selfBox.width * 0.2f, 0.f);
        for (int i = 0; i < 4; i++)
        {
            valueVArray[i].color = sf::Color::Red;
        }
    }

    bool outputBoundsContainsMouse(int index)
    {
        return false;
    }

    bool inputBoundsContainsMouse(int index)
    {
        return PlantedGate::inputBoundsContainsMouse(index);
    }

    void onShiftLeftClick()
    {

    }
    void checkValueInteraction(sf::Vector2f mouse, sf::RenderWindow* window)
    {

        sf::FloatRect valueBounds = transform.transformRect(valueVArray.getBounds());
        sf::FloatRect adjusterBounds = transform.transformRect(valueAdjuster.bg.getBounds());
        if (valueBounds.contains(mouse) || (valueAdjusterActive && adjusterBounds.contains(mouse)))
        {
            Receiver& rcr = dynamic_cast<Receiver&>(*(this->component));
            std::string val = std::bitset<32>(rcr.getValue()).to_string().substr(31 - rcr.bitSize, rcr.bitSize);
            if (val.length() > 8)
            {
                val.insert(8, "\n");
            }
            valueAdjuster.valueText.setString(val);
            valueAdjusterActive = true;
            window->draw(valueAdjuster.bg, transform);
            window->draw(valueAdjuster.valueText, transform);
        }
        else
        {
            valueAdjusterActive = false;
        }
    }
};

struct CapsuleObject : public PlantedGate
{

    ~CapsuleObject() {};

   
    CapsuleObject(sf::VertexArray vArray, Capsule component)
    {
        this->component = std::make_unique<Capsule>(component);
        this->vArray = vArray;
        this->valueAdjusterActive = false;
        //this->outputIds = { -1 };
        //this->inputIds = {};
        this->getPartialRect = GetPartialRect(vArray[0].position, vArray.getBounds().width, vArray.getBounds().height);
        this->IOVArrays = IOVertexArrays(vArray.getBounds(), component.getInputIds().size(), component.getOutputIds().size(), false);
    }

    void draw(sf::RenderWindow * window)
    {
        window->draw(vArray, transform);
        window->draw(IOVArrays.inputSet.IOvertexArray, transform);
        window->draw(IOVArrays.outputSet.IOvertexArray, transform);
    }
    std::string getTraceText()
    {
        Capsule& cap = dynamic_cast<Capsule&>(*(this->component));
        return cap.getTraceText();
    }
    void saveToFile(std::string name)
    {
        std::string filename = "resources/capsules/" + name + ".txt";
        std::ofstream capsuleFile(filename);
        if (capsuleFile.is_open())
        {
            capsuleFile << "name " << this->getName() << "\n";
            capsuleFile << this->getTraceText();
        }
    }
};

struct ObjectCarriedByMouse
{
    sf::VertexArray vArray;
    Gate component;
    ObjectCarriedByMouse(Gate component = Gate(NOTGATE), sf::VertexArray vArray = sf::VertexArray(sf::Quads, 4))
        :component(component), vArray(vArray)
    {

    }
};

ObjectCarriedByMouse objectCarriedByMouse;
sf::Transform carriedObjectTransform;
sf::Color carryObjectColor = sf::Color::Black;
bool objectReadyToPlant = false;
bool movingObject = false;
int objectMovedId = -1;
std::map<int, std::unique_ptr<PlantedGate>> plantedGates;
std::map<int, int> gateVertexIds;

//capsule
std::vector<Capsule> createdCapsules;
std::map<int, int> capsuleSlots;
bool promptCapsuleName = false;
sf::Keyboard::Key keySentToPrompt = sf::Keyboard::Unknown;
sf::Text NamePromptText;
std::string capsulePromptBaseString = "Name the capsule: ";
std::string currentAddedPromptString = "";


//connecting wires
bool wireConnectionInitiated = false;
bool wireConnected = false;
sf::VertexArray probeWire(sf::LinesStrip, 2);
WireStartingPoint wireStartingPoint;
int freshWireId;
std::map<int, std::unique_ptr<Wire>> wires;
struct WireCpMoved
{
    int wireId;
    int cpNumber;
};

WireCpMoved wireCpCarried;
bool movingWireCp = false;
bool wireForkInitiated = false;
std::pair<int, int> wireForkSource;
sf::VertexArray probeForkWire(sf::LinesStrip, 2);

std::map<int, ControllerObject> controllerObjects;

bool traceStarted = false;
bool circuitStarted = false;
int circuitIterator = 0;
std::vector<Component*> fullCircuit;

void repositionControlPoint(std::map<int, Wire>& wires, sf::VertexArray& cP, sf::Vector2f mP, int wireKey, int cPPosition)
{
    cP[0].position = mP + sf::Vector2f(-0.5f * controlPointSide, -0.5f * controlPointSide);
    cP[1].position = mP + sf::Vector2f(-0.5f * controlPointSide, 0.5f * controlPointSide);
    cP[2].position = mP + sf::Vector2f(0.5f * controlPointSide, 0.5f * controlPointSide);
    cP[3].position = mP + sf::Vector2f(0.5f * controlPointSide, -0.5f * controlPointSide);

    if (wires[wireKey].outputIds.size() > 0)
    {
        for (auto& outputId : wires[wireKey].outputIds)
        {
            wires[outputId].lineVertex[0].position = mP;
        }
    }
}

void addControlPoint(std::map<int, std::unique_ptr<Wire>>& wires, int key, int cpPos)
{
    sf::VertexArray cp = sf::VertexArray(sf::Quads, 4);
    sf::VertexArray lv = sf::VertexArray(sf::Lines, 2);
    wires[key]->controlPointVertices.emplace_back(std::pair<sf::VertexArray, sf::Transform>{cp, sf::Transform::Transform()});
    int newCpPos = cpPos + 1;
    int newLinePos = newCpPos + 1;
    sf::Vector2f midPoint = getMidPoint(wires[key]->lineVertex[newCpPos].position, wires[key]->lineVertex[newLinePos].position);
    if (wires[key]->controlPointVertices.size() < cpPos)
    {

    }
    else
    {
        for (int i = wires[key]->controlPointVertices.size() - 1; i > newCpPos; i--)
        {
            wires[key]->controlPointVertices[i] = wires[key]->controlPointVertices[i - 1];
        }
    }

    wires[key]->lineVertex.append(wires[key]->lineVertex[wires[key]->lineVertex.getVertexCount() - 1]);
    for (int i = wires[key]->lineVertex.getVertexCount() - 1; i > newLinePos; i--)
    {
        wires[key]->lineVertex[i] = wires[key]->lineVertex[i - 1];
    }
    wires[key]->lineVertex[newLinePos] = midPoint;

    for (int i = 0; i < 4; i++)
    {
        wires[key]->controlPointVertices[newCpPos].first[i].position = midPoint + cpPositions[i];
        wires[key]->controlPointVertices[newCpPos].first[i].color = sf::Color::White;
    }
    wires[key]->controlPointVertices[newCpPos].second = sf::Transform::Transform();
}

void repositionWires(PlantedGate& component, std::map<int, std::unique_ptr<Wire>>& wires, sf::Vector2f translation)
{

    for (auto& inputId : component.component->getInputIds())
    {
        if (inputId > -1)
        {
            wires[inputId]->lineVertex[wires[inputId]->lineVertex.getVertexCount() - 1].position += translation;
        }
    }

    for (auto& outputId : component.component->getOutputIds())
    {
        if (outputId > -1)
        {
            wires[outputId]->lineVertex[0].position += translation;
        }
    }
}

typedef boost::adjacency_list < boost::vecS, boost::vecS, boost::directedS, PlantedGate*, std::vector<Wire*>> ComponentGraph;
typedef boost::graph_traits < ComponentGraph >::vertices_size_type size_type;

template < typename TimeMap > class dfs_component_visitor : public boost::default_dfs_visitor {
    typedef typename boost::property_traits < TimeMap >::value_type T;
public:
    dfs_component_visitor(TimeMap dmap, TimeMap fmap, T & t)
        : m_dtimemap(dmap), m_ftimemap(fmap), m_time(t) {
    }
    template < typename Vertex, typename Graph >
    void discover_vertex(Vertex u, const Graph & g) const
    {
        put(m_dtimemap, u, m_time++);
    }
    template < typename Vertex, typename Graph >
    void finish_vertex(Vertex u, const Graph & g) const
    {
        put(m_ftimemap, u, m_time++);
    }
    TimeMap m_dtimemap;
    TimeMap m_ftimemap;
    T & m_time;
};

template < typename TimeMap > class bfs_time_visitor :public boost::default_bfs_visitor {
    typedef typename boost::property_traits < TimeMap >::value_type T;
public:
    bfs_time_visitor(TimeMap tmap, T & t) :m_timemap(tmap), m_time(t) { }
    template < typename Vertex, typename Graph >
    void discover_vertex(Vertex u, const Graph & g) const
    {
        put(m_timemap, u, m_time++);
    }
    
    template < typename Edge, typename Graph>
    void examine_edge(Edge e, const Graph & g)
    {
        for (auto& wire : g[e])
        {
            
        }
    }
    
    TimeMap m_timemap;
    T & m_time;
};

typedef
boost::iterator_property_map<std::vector<size_type>::iterator,
    boost::property_map<ComponentGraph, boost::vertex_index_t>::const_type>
    time_pm_type;


ComponentGraph componentGraph;

void checkForObjectInteractions(PlantedGate& component, std::map<int, std::unique_ptr<Wire>>& wires, sf::RenderWindow* window, int& toDelete)
{
    //mouse hovering interactions
    if (!movingObject)
    {
        component.checkValueInteraction(mouse, window);
        component.checkIOHover(window);

    }
    //mouse clicking interactions
    if (RMBRelease && !movingObject)
    {
        if (wireConnectionInitiated)
        {
            for (int i = 0; i < component.getNoOfInputs(); i++)
            {
                if (component.inputBoundsContainsMouse(i))
                {
                    if (component.getId() == wireStartingPoint.componentId)
                    {
                        wireConnectionInitiated = false;
                    }
                    else
                    {
                        wireConnectionInitiated = false;
                                
                        sf::VertexArray newWire = probeWire;
                        newWire[1].position = component.getInputCenter(i);
                        LineConnection line;
                        freshWireId = line.getId();
                        std::unique_ptr<Wire> wirePtr = std::make_unique<Wire>(Wire(newWire, line));
                        wires[freshWireId] = std::move(wirePtr);
                        plantedGates[wireStartingPoint.componentId]->component->connectOutput(wires[freshWireId]->wireComponent, wireStartingPoint.outputNumber);
                        wires[freshWireId]->wireComponent->connectOutput(component.component, -1, i);
                        wires[freshWireId]->inputComponent.set(wireStartingPoint.componentId, wireStartingPoint.outputNumber);
                        auto e = boost::add_edge(gateVertexIds[wireStartingPoint.componentId], gateVertexIds[component.getId()], std::vector<Wire*>{ &(*(wires[freshWireId])) }, componentGraph);
                    }
                    return;
                }
            }
            for (int i = 0; i < component.getNoOfOutputs(); i++)
            {
                if (component.outputBoundsContainsMouse(i))
                {
                    if (component.getId() == wireStartingPoint.componentId)
                    {
                        component.disconnectOutput(i);
                        wireStartingPoint.set(-1, -1);
                        wireConnectionInitiated = false;
                    }
                    return;
                }
            }
        }
        else if (wireForkInitiated)
        {
            if (wires[wireForkSource.first]->getGlobalBounds(wireForkSource.second).contains(mouse))
            {
                wireForkInitiated = false;
                return;
            }
            for (int i = 0; i < component.getNoOfInputs(); i++)
            {
                if (component.inputBoundsContainsMouse(i))
                {
                    probeForkWire[1].position = component.getInputCenter(i);
                    LineConnection line;
                    int newLineId = line.getId();
                    std::unique_ptr<Wire> wirePtr = std::make_unique<Wire>(Wire(probeForkWire, line));
                    wires[newLineId] = std::move(wirePtr);
                    wires[wireForkSource.first]->outputWires.emplace_back(OutputWire(newLineId, wireForkSource.second));
                    wires[wireForkSource.first]->wireComponent->connectOutput(wires[newLineId]->wireComponent);
                    wires[newLineId]->wireComponent->connectOutput(component.component, -1, i);
                    wires[newLineId]->inputComponent = wires[wireForkSource.first]->inputComponent;
                    auto e = boost::add_edge(gateVertexIds[wires[newLineId]->inputComponent.componentId], gateVertexIds[component.getId()], std::vector<Wire*>{ &(*(wires[wireForkSource.first])), &(*(wires[newLineId])) }, componentGraph);
                    wireForkInitiated = false;

                    return;
                }
            }
        }
        else
        {
            for (int i = 0; i < component.component->getInputIds().size(); i++)
            {
                if (component.inputBoundsContainsMouse(i))
                {
                    int senderId = component.component->getInputIds()[i];
                    if (senderId != -1)
                    {
                        wireConnectionInitiated = true;

                        boost::remove_edge(gateVertexIds[wires[senderId]->inputComponent.componentId], gateVertexIds[component.getId()], componentGraph);

                        wireStartingPoint.set(wires[senderId]->inputComponent.componentId, wires[senderId]->inputComponent.outputNumber);
                        wires.erase(senderId);

                        probeWire[0].position = plantedGates[wireStartingPoint.componentId]->getOutputCenter(wireStartingPoint.outputNumber);
                        probeWire[1].position = mouse;
                        probeWire[0].color = sf::Color::White;
                        probeWire[1].color = sf::Color::White;
                    }
                    return;
                }
            }

            for (int i = 0; i < component.component->getOutputIds().size(); i++)
            {
                if (component.outputBoundsContainsMouse(i))
                {
                    int receiverId = component.component->getOutputIds()[i];
                    if (receiverId == -1)
                    {
                        wireConnectionInitiated = true;
                        wireStartingPoint.set(component.getId(), i);
                        probeWire[0].position = component.getOutputCenter(i);
                        probeWire[1].position = mouse;
                        probeWire[0].color = sf::Color::White;
                        probeWire[1].color = sf::Color::White;
                    }
                    return;
                }
            }
        }
    }
    if (component.getGlobalBounds().contains(mouse) && !movingObject)
    {
        if (LMBRelease && !Shift && !wireConnectionInitiated && !wireForkInitiated && !component.valueAdjusterActive)
        {
            objectMovedId = component.getId();
            movingObject = true;
            previousMouse = mouse;
        }
        else if (LMBRelease && Shift && !wireConnectionInitiated && !wireForkInitiated)
        {
            component.onShiftLeftClick();
        }
        else if (Delete)
        {
            toDelete = component.getId();
        }
    }
    if (movingObject)
    {
        if (component.getId() == objectMovedId)
        {
            sf::Vector2f translation = mouse - previousMouse;
            component.transform.translate(translation);
            previousMouse = mouse;
            repositionWires(component, wires, translation);
        }
    }
}

bool sortbysec(const std::pair<int, int> &a,
    const std::pair<int, int> &b)
{
    return (a.second < b.second);
}

int main()
{
    /*
    Controller controller1;
    Controller controller2;
    Gate notGate1(NOTGATE);
    Gate notGate2(NOTGATE);
    Gate andGate1(ANDGATE);
    Wire wire1;
    Wire wire2;
    Wire wire3;
    Wire wire4;

    PlantedGate gate1(sf::VertexArray(sf::Quads, 4), notGate1);
    PlantedGate gate2(sf::VertexArray(sf::Quads, 4), notGate2);
    PlantedGate gate3(sf::VertexArray(sf::Quads, 4), andGate1);
    ControllerObject ctr1(sf::VertexArray(sf::Quads, 4), controller1);
    ControllerObject ctr2(sf::VertexArray(sf::Quads, 4), controller2);

    
    auto v1 = boost::add_vertex(ctr1, componentGraph);
    auto v2 = boost::add_vertex(gate1, componentGraph);
    auto v3 = boost::add_vertex(gate2, componentGraph);
    auto v4 = boost::add_vertex(gate3, componentGraph);
    auto v5 = boost::add_vertex(ctr2, componentGraph);

    auto e1 = boost::add_edge(v1, v2, std::vector<Wire>{ wire1 }, componentGraph);
    auto e2 = boost::add_edge(v2, v3, std::vector<Wire>{ wire2 }, componentGraph);
    auto e3 = boost::add_edge(v2, v4, std::vector<Wire>{ wire2, wire3 }, componentGraph);
    auto e4 = boost::add_edge(v5, v3, std::vector<Wire>{ wire4 }, componentGraph);
    */
    
    sf::RenderWindow window(sf::VideoMode(width, height), "vertex");
    sf::RenderStates states = sf::RenderStates(sf::BlendMode::BlendMode());
    SpecialEffects effects(&window);

    NamePromptText.setCharacterSize(width / 100);
    NamePromptText.setFillColor(sf::Color::Green);
    NamePromptText.setPosition(windowCenter);
    NamePromptText.setFont(font);
    NamePromptText.setString(capsulePromptBaseString);

    sf::Text bitSizeSelectorText;

    bitSizeSelectorText.setFillColor(sf::Color::Green);
    bitSizeSelectorText.setFont(font);
    bitSizeSelectorText.setCharacterSize(width / 100);
    bitSizeSelectorText.setPosition(windowCenter);
    bitSizeSelectorText.setString("1");
    bool bitSizePrompt = false;

    //load capsules
    for (int i = 0; i < 12; i++)
    {
        capsuleSlots[i + 85] = -1;
    }
    std::vector<std::string> capsuleFilePaths;
    std::string capsulePath = "resources/capsules/";
    for (const auto & entry : std::filesystem::directory_iterator(capsulePath))
    {
        std::cout << entry.path().filename() << std::endl;
        capsuleFilePaths.push_back(entry.path().string());
    }
    for (auto& path : capsuleFilePaths)
    {
        std::string name;
        std::vector<AlgBundle> algBundles;
        std::map<int, std::shared_ptr<Input>> inputMap;
        std::map<int, std::shared_ptr<Output>> outputMap;
        std::vector<int> controllerIdsVec;
        std::vector<int> receiverIdsVec;

        std::ifstream capsuleFile(path);
        if (capsuleFile.is_open())
        {
            std::string line;
            while (std::getline(capsuleFile, line))
            {
                std::vector<std::string> words;
                boost::split(words, line, boost::is_any_of(" "));
                if (words[0] == "name" && words.size() > 1) 
                {
                    if (words[1][words[1].length() - 1] == '\n')
                    {
                        words[1].erase(line.length() - 1);
                    }
                    name = words[1];
                }
                else if (words[0] == "alg" && words.size() > 1)
                {
                    callback_function alg = algos[words[1]];
                    std::vector<int> inputIds;
                    std::vector<int> outputIds;
                    words.erase(words.begin());
                    words.erase(words.begin());
                    for (auto& word : words)
                    {
                        if (word[0] == 'i')
                        {
                            inputIds.push_back(stoi(word.substr(1)));
                        }

                        else if ((word[0] == 'o'))
                        {
                            outputIds.push_back(stoi(word.substr(1)));
                        }
                    }
                    algBundles.push_back(AlgBundle(inputIds, outputIds, alg));
                }
                else if ((words[0] == "in"  || words[0] == "out") && words.size() > 1)
                {
                    bool in = false;
                    if (words[0] == "in")
                    {
                        in = true;
                    }
                    words.erase(words.begin());

                    int id;
                    int bitSize;
                    int relevantOutputId;

                    for (int i = 0; i < words.size(); i++)
                    {
                        if (words[i] == "key")
                        {
                            id = stoi(words[i + 1]);
                        }
                        if (words[i] == "vals")
                        {
                            bitSize = stoi(words[i + 1]);
                            if (in)
                            {
                                relevantOutputId = stoi(words[i + 2]);
                                Input input;
                                input.id = id;
                                input.bitSize = bitSize;
                                input.relevantOutputId = relevantOutputId;
                                inputMap[id] = std::make_shared<Input>(input);
                            }
                            else
                            {
                                Output output;
                                output.id = id;
                                output.bitSize = bitSize;
                                outputMap[id] = std::make_shared<Output>(output);
                            }
                        }
                    }
                }
                else if (words[0] == "rec"  && words.size() > 1)
                {
                    words.erase(words.begin());
                    for (auto& word : words)
                    {
                        receiverIdsVec.push_back(stoi(word));
                    }
                }
                else if (words[0] == "con"  && words.size() > 1)
                {
                    words.erase(words.begin());
                    for (auto& word : words)
                    {
                        controllerIdsVec.push_back(stoi(word));
                    }
                }
            }
        }
        FullTrace fullTrace(algBundles, inputMap, outputMap, controllerIdsVec, receiverIdsVec);
        createdCapsules.push_back(Capsule(fullTrace, name));
    }

    font.loadFromFile("resources/fonts/arial.ttf");
    courier.loadFromFile("resources/fonts/cour.ttf");

    // component info text
    sf::Text componentInfo("hello", font);
    componentInfo.setCharacterSize(20);
    componentInfo.setStyle(sf::Text::Bold);
    componentInfo.setFillColor(sf::Color::Red);
    componentInfo.setPosition(sf::Vector2f(1.f, 1.f));

    //FKey texts
    std::vector<sf::Text> FKeyTexts;
    std::vector<std::string> FKeyStrs = { "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12" };
    float textYPos = 30.f;
    float textXPos = width / 24.f;
    for (int i = 0; i < 12; i++)
    {
        sf::Text FKeyText("hello", courier);
        FKeyText.setCharacterSize(25);
        FKeyText.setStyle(sf::Text::Bold);
        FKeyText.setFillColor(sf::Color::Color(200, 20, 100, 180));
        FKeyText.setPosition(sf::Vector2f(textXPos, textYPos));
        textXPos += (width / 24.f) * 2.f;
        FKeyText.setString(FKeyStrs[i]);
        sf::FloatRect textRect = FKeyText.getLocalBounds();
        FKeyText.setOrigin(textRect.left + textRect.width / 2.0f,
            textRect.top + textRect.height / 2.0f);
        FKeyTexts.push_back(FKeyText);
    }
    std::vector<sf::VertexArray> linesAroundFKeyTexts;
    sf::VertexArray horizontalLine(sf::LinesStrip, 2);
    float lowY = 60.f;
    horizontalLine[0].position = sf::Vector2f(0.f, lowY);
    horizontalLine[1].position = sf::Vector2f(width, lowY);
    linesAroundFKeyTexts.push_back(horizontalLine);
    float lineXPos = width / 12.f;
    for (int i = 0; i < 11; i++)
    {
        sf::VertexArray verticalLine(sf::LinesStrip, 2);
        verticalLine[0].position = sf::Vector2f(lineXPos, 0.f);
        verticalLine[1].position = sf::Vector2f(lineXPos, lowY);
        lineXPos += width / 12.f;
        linesAroundFKeyTexts.push_back(verticalLine);
    }
    for (auto& l : linesAroundFKeyTexts)
    {
        l[0].color = sf::Color::Color(182, 182, 182, 200);
        l[1].color = sf::Color::Color(182, 182, 182, 200);
    }



    srand(time(NULL));
    //planting gates

    probeForkWire[0].color = sf::Color::White;
    probeForkWire[1].color = sf::Color::White;

    sf::VertexArray controller(sf::Quads, 4);

    for (int i = 0; i < 4; i++)
    {
        controller[i].color = sf::Color(255, 255, 255, 255);
    }
    std::map<int, ControllerObject> controllerObjects;

    const int nOfElementaryGates = 15;
    float gateXPositions[nOfElementaryGates];

    for (int i = 0; i < nOfElementaryGates; i++)
    {
        gateXPositions[i] = (width * 0.05) + (width * 0.05) * i;
    }

    unsigned int gateColorR = 0;
    unsigned int gateColorG = 20;
    unsigned int gateColorB = 120;
    

    sf::VertexArray separator(sf::Quads, 4);
    separator[0].position = sf::Vector2f(0, separatorTopEdge);
    separator[1].position = sf::Vector2f(0, separatorBotEdge);
    separator[2].position = sf::Vector2f(width, separatorBotEdge);
    separator[3].position = sf::Vector2f(width, separatorTopEdge);
    for (int i = 0; i < 4; i++)
    {
        separator[i].color = sf::Color(255, 255, 255, 255);
    }
    std::vector<sf::VertexArray> gates;
    gates.reserve(15);
    sf::VertexArray notGate(sf::Quads, 4);
    sf::VertexArray andGate(sf::Quads, 4);
    sf::VertexArray orGate(sf::Quads, 4);
    sf::VertexArray xorGate(sf::Quads, 4);
    sf::VertexArray muxGate(sf::Quads, 4);
    sf::VertexArray dmuxGate(sf::Quads, 4);
    sf::VertexArray not16Gate(sf::Quads, 4);
    sf::VertexArray and16Gate(sf::Quads, 4);
    sf::VertexArray or16Gate(sf::Quads, 4);
    sf::VertexArray mux16Gate(sf::Quads, 4);
    sf::VertexArray or8WayGate(sf::Quads, 4);
    sf::VertexArray mux4Way16Gate(sf::Quads, 4);
    sf::VertexArray mux8Way16Gate(sf::Quads, 4);
    sf::VertexArray dmux4WayGate(sf::Quads, 4);
    sf::VertexArray dmux8WayGate(sf::Quads, 4);
    gates.push_back(notGate);
    gates.push_back(andGate);
    gates.push_back(orGate);
    gates.push_back(xorGate);
    gates.push_back(muxGate);
    gates.push_back(dmuxGate);
    gates.push_back(not16Gate);
    gates.push_back(and16Gate);
    gates.push_back(or16Gate);
    gates.push_back(mux16Gate);
    gates.push_back(or8WayGate);
    gates.push_back(mux4Way16Gate);
    gates.push_back(mux8Way16Gate);
    gates.push_back(dmux4WayGate);
    gates.push_back(dmux8WayGate);

    std::vector<std::string> gateNames = { "NOT", "AND", "OR", "XOR", "MUX", "DMUX", "NOT16", "AND16", "OR16", "MUX16", "OR-8-WAY", "MUX-4-WAY-16", "MUX-8-WAY-16", "DMUX-4-WAY", "DMUX-8-WAY" };
    // virtual hardware added in order of operation

    //hardware connections
    for (int i = 0; i < nOfElementaryGates; i++)
    {
        gates[i][0].position = sf::Vector2f(gateXPositions[i], gateTopEdge);//left-top
        gates[i][1].position = sf::Vector2f(gateXPositions[i], gateBotEdge);//left-bot
        gates[i][2].position = sf::Vector2f(gateXPositions[i] + gateWidth, gateBotEdge);//right-bot
        gates[i][3].position = sf::Vector2f(gateXPositions[i] + gateWidth, gateTopEdge);//right-top
        for (int j = 0; j < 4; j++)
        {
            gates[i][j].color = sf::Color(gateColorR, gateColorB, gateColorG);
        }
        if (gateColorR < 249)
        {
            gateColorR += 105;
            gateColorB += 50;
            gateColorG += 50;
        }
        else if (gateColorB < 249)
        {
            gateColorB += 125;
            gateColorG += 61;
            gateColorR += 55;
        }
        else
        {
            gateColorG += 105;
            gateColorR += 31;
            gateColorB += 61;

        }
    }

    /*
    std::vector<Explosion> expVec;
    for (int i = 0; i < 10; i++)
    {
        expVec.emplace_back(&window);
        Sleep(rand() % 400);
    }
    */
    Cube cube(&window);
    //Wind wind(&window);
    //Fog fog(&window);
    //ParticleRoom particleroom(&window);
    //fog.commenceIncident();
    //Rain rain(&window);
    //rain.commenceIncident();
    //wind.commenceIncident();
    //particleroom.commenceIncident();

    sf::View view1(sf::FloatRect(0.f, 0.f, width, height));
    float zoom = 1.0;

    while (window.isOpen())
    {
        mouse = sf::Vector2f(sf::Mouse::getPosition(window));

        

        sf::Event event;
        while (window.pollEvent(event))
        {

            // Request for closing the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::TextEntered)
            {
                sf::Event::TextEvent textEv = event.text;
            }
            if (event.type == sf::Event::KeyPressed && promptCapsuleName)
            {
                keySentToPrompt = event.key.code;
            }
            if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    RMBRelease = true;
                }
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    LMBRelease = true;
                }
            }
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
                    std::cout << "wheel type: vertical" << std::endl;
                else if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel)
                    std::cout << "wheel type: horizontal" << std::endl;
                else
                    std::cout << "wheel type: unknown" << std::endl;

                std::cout << "wheel movement: " << event.mouseWheelScroll.delta << std::endl;
                if (event.mouseWheelScroll.delta == 1)
                {
                    ScrollUp = true;
                }
                else if (event.mouseWheelScroll.delta == -1)
                {
                    ScrollDown = true;
                }
                std::cout << "mouse x: " << event.mouseWheelScroll.x << std::endl;
                std::cout << "mouse y: " << event.mouseWheelScroll.y << std::endl;
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                for (int i = 0; i < 12; i++)
                {
                    if (event.key.code == i + 85)
                    {
                        FKeyRelease[i] = true;
                    }
                }
                if (event.key.code == sf::Keyboard::O)
                {
                    ORelease = true;
                }
                else if (event.key.code == sf::Keyboard::I)
                {
                    IRelease = true;
                }
                else if (event.key.code == sf::Keyboard::C)
                {
                    CRelease = true;
                }
                else if (event.key.code == sf::Keyboard::R)
                {
                    RRelease = true;
                }
                else if (event.key.code == sf::Keyboard::E)
                {
                    ERelease = true;
                }
                else if (event.key.code == sf::Keyboard::T)
                {
                    TRelease = true;
                }
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                LMB = true;

            }
            else
            {
                LMB = false;

            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            {
                Shift = true;
            }
            else
            {
                Shift = false;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Delete))
            {
                Delete = true;
            }
            else
            {
                Delete = false;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                Right = true;
            }
            else
            {
                Right = false;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                Down = true;
            }
            else
            {
                Down = false;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                Left = true;
            }
            else
            {
                Left = false;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                Up = true;
            }
            else
            {
                Up = false;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
            {
                Alt = true;
            }
            else
            {
                Alt = false;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
            {
                Tab = true;
            }
            else
            {
                Tab = false;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
            {
                Z = true;
            }
            else
            {
                Z = false;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                S = true;
            }
            else
            {
                S = false;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
            {
                C = true;
            }
            else
            {
                C = false;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
            {
                R = true;
            }
            else
            {
                R = false;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                Space = true;
            }
            else
            {
                Space = false;
            }
        }
        if (Z)
        {
            view1.setSize(view1.getSize() + sf::Vector2f(-0.001f * width, -0.001f * height));
        }
        if (S)
        {
            view1.setSize(view1.getSize() + sf::Vector2f(0.001f * width, 0.001f * height));
        }
        view1.setCenter(mouse);
        window.clear();
        /*
        for (auto& v : expVec)
        {
            v.commenceIncident();
        }
        */


        //fog.update();
        //wind.update(LMB);
        //rain.update();
        //particleroom.update(LMB);

        
        window.draw(separator);

        int i = 0;
        for (auto& gate : gates)
        {
            window.draw(gate);
            if (gate.getBounds().contains(mouse))
            {
                componentInfo.setString(gateNames[i]);
                window.draw(componentInfo);
            }
            if (LMBRelease && gate.getBounds().contains(mouse) && !wireConnectionInitiated && !wireForkInitiated && !movingObject && !movingWireCp)
            {
                for (int i = 0; i < 4; i++)
                {
                    objectCarriedByMouse.vArray[i].position = gate[i].position;
                    objectCarriedByMouse.vArray[i].color = gate[0].color;
                    objectCarriedByMouse.vArray[i].color.a = 125;
                }

                objectCarriedByMouse.component = Gate(i);
                LMBRelease = false;
                objectReadyToPlant = true;
                previousMouse = mouse;
                carriedObjectTransform = sf::Transform::Identity;
                break;
            }
            i++;
        }
        if (!promptCapsuleName)
        {
            if (LMBRelease && objectReadyToPlant)
            {
                if (mouse.y < (separatorTopEdge - gateHeight))
                {
                    sf::Color color = objectCarriedByMouse.vArray[0].color;
                    color.a = 255;
                    sf::VertexArray gateVertex = getSquareVertexArray(mouse, gateHeight, color);

                    std::unique_ptr<PlantedGate> newGate = std::make_unique<PlantedGate>(gateVertex, objectCarriedByMouse.component);
                    int gateId = newGate->getId();
                    plantedGates[gateId] = std::move(newGate);
                    auto v = boost::add_vertex(&(*(plantedGates[gateId])), componentGraph);
                    gateVertexIds[gateId] = v;
                }
                objectReadyToPlant = false;
                LMBRelease = false;
            }

            if (objectReadyToPlant)
            {
                sf::Vector2f translation = mouse - previousMouse;
                carriedObjectTransform.translate(translation);
                previousMouse = mouse;
                window.draw(objectCarriedByMouse.vArray, carriedObjectTransform);
            }
            if ((Shift && C) || (Shift && R))
            {
                bitSizePrompt = true;
            }
            if ((CRelease && Shift) || (CRelease && bitSizePrompt))
            {

                if (mouse.y < (separatorTopEdge - controllerSide))
                {
                    sf::VertexArray controllerVertex = getSquareVertexArray(mouse, controllerSide, sf::Color::White);

                    std::unique_ptr<ControllerObject> newController;
                    if (Alt)
                    {
                        newController = std::make_unique<ControllerObject>(controllerVertex, Controller(16));
                    }
                    else
                    {
                        int bitSz = std::stoi(bitSizeSelectorText.getString().toAnsiString());
                        newController = std::make_unique<ControllerObject>(controllerVertex, Controller(bitSz));
                        bitSizeSelectorText.setString("1");
                    }
                    int ctrId = newController->getId();
                    plantedGates[ctrId] = std::move(newController);
                    auto v = boost::add_vertex(&(*(plantedGates[ctrId])), componentGraph);
                    gateVertexIds[ctrId] = v;
                }
                CRelease = false;
                bitSizePrompt = false;
            }
            if ((RRelease && Shift) || (CRelease && bitSizePrompt))
            {

                if (mouse.y < (separatorTopEdge - controllerSide))
                {
                    sf::VertexArray receiverVertex = getSquareVertexArray(mouse, controllerSide, sf::Color::Color(50, 50, 50, 200));

                    std::unique_ptr<ReceiverObject> newReceiver;
                    if (Alt)
                    {
                        newReceiver = std::make_unique<ReceiverObject>(receiverVertex, Receiver(16));
                    }
                    else
                    {
                        int bitSz = std::stoi(bitSizeSelectorText.getString().toAnsiString());
                        newReceiver = std::make_unique<ReceiverObject>(receiverVertex, Receiver(1));
                        bitSizeSelectorText.setString("1");
                    }
                    int rcrId = newReceiver->getId();
                    plantedGates[rcrId] = std::move(newReceiver);
                    auto v = boost::add_vertex(&(*(plantedGates[rcrId])), componentGraph);
                    gateVertexIds[rcrId] = v;
                }
                RRelease = false;
                bitSizePrompt = false;
            }
            for (int i = 0; i < createdCapsules.size() % 12; i++)
            {
                if (FKeyRelease[i])
                {

                    if (mouse.y < (separatorTopEdge - controllerSide))
                    {
                        int nOfInputs = createdCapsules[i].getInputsVec().size();
                        int nOfOutputs = createdCapsules[i].getOutputsVec().size();
                        float m = float(std::max(nOfInputs, nOfOutputs));
                        sf::VertexArray capsuleVertex = getCapsuleVertexArray(mouse, m * (controllerSide * 0.5f), controllerSide, sf::Color::Color(100, 100, 100, 200));

                        std::unique_ptr<CapsuleObject> newCapsule;
                        newCapsule = std::make_unique<CapsuleObject>(capsuleVertex, createdCapsules[i]);

                        int capId = newCapsule->getId();
                        plantedGates[capId] = std::move(newCapsule);
                        auto v = boost::add_vertex(&(*(plantedGates[capId])), componentGraph);
                        gateVertexIds[capId] = v;
                    }
                    FKeyRelease[i] = false;
                    break;

                }
            }
            if (Alt && ERelease)
            {
                //Encapsulate current circuit
                //ask for name

            }
            if (wireConnectionInitiated)
            {
                probeWire[1].position = mouse;
                window.draw(probeWire);
            }

            if (movingWireCp)
            {
                sf::Vector2f translation = mouse - previousMouse;
                wires[wireCpCarried.wireId]->lineVertex[wireCpCarried.cpNumber + 1].position += translation;

                wires[wireCpCarried.wireId]->controlPointVertices[wireCpCarried.cpNumber].second.translate(translation);;
                previousMouse = mouse;

                for (auto& outputWire : wires[wireCpCarried.wireId]->outputWires)
                {
                    if (outputWire.cp == wireCpCarried.cpNumber)
                    {
                        wires[outputWire.id]->lineVertex[0].position += translation;
                    }
                }

                if (LMBRelease)
                {
                    movingWireCp = false;
                    LMBRelease = false;
                }
            }

            if (wireForkInitiated)
            {
                probeForkWire[1].position = mouse;
                window.draw(probeForkWire);
                if (RMBRelease && wires[wireForkSource.first]->getGlobalBounds(wireForkSource.second).contains(mouse))
                {
                    wireForkInitiated = false;
                    RMBRelease = false;
                }
            }
            for (auto&[key, val] : wires)
            {
                window.draw(val->lineVertex);
                for (int i = 0; i < val->controlPointVertices.size(); i++)
                {
                    if (val->getGlobalBounds(i).contains(mouse))
                    {
                        componentInfo.setString(wires[key]->wireComponent->getInfo());
                        window.draw(componentInfo);
                    }
                    if (LMBRelease && val->getGlobalBounds(i).contains(mouse))
                    {
                        if (Shift)
                        {
                            addControlPoint(wires, key, i);
                        }
                        else
                        {
                            movingWireCp = true;
                            previousMouse = mouse;
                            wireCpCarried = { key, i };
                        }
                    }
                    if (RMBRelease && val->getGlobalBounds(i).contains(mouse))
                    {
                        RMBRelease = false;
                        wireForkInitiated = true;
                        wireForkSource = { key, i };
                        probeForkWire[0].position = wires[key]->lineVertex[i + 1].position;
                        probeForkWire[1].position = wires[key]->lineVertex[i + 1].position;
                    }
                    window.draw(val->controlPointVertices[i].first, val->controlPointVertices[i].second);
                }
            }

            if (Space && !circuitStarted)
            {

                for (auto& gate : plantedGates)
                {
                    table.addComponent(*(gate.second->component));
                }
                for (auto& wire : wires)
                {
                    table.addComponent(*(wire.second->wireComponent));
                }

                int circuitSize = 0;


                //fullCircuit.reserve(circuitSize);

                for (int i = 0; i < circuitSize; i++)
                {
                    std::vector<Component*> currentOrderComps;
                    fullCircuit.insert(fullCircuit.end(), currentOrderComps.begin(), currentOrderComps.end());
                }
                circuitStarted = true;
            }
            if (TRelease && !circuitStarted)
            {
                if (plantedGates.size() > 1 && wires.size() > 0 && !promptCapsuleName)
                {
                    for (auto& gate : plantedGates)
                    {
                        table.addComponent(*(gate.second->component));
                    }
                    for (auto& wire : wires)
                    {
                        table.addComponent(*(wire.second->wireComponent));
                    }

                    TRelease = false;
                    traceStarted = true;
                }
            }
            if (traceStarted)
            {
                //tracing data
                std::vector<AlgBundle> algBundles;
                std::map<int, std::shared_ptr<Input>> inputMap;
                std::map<int, std::shared_ptr<Output>> outputMap;
                std::vector<int> controllerIdsVec;
                std::vector<int> receiverIdsVec;
                std::vector<std::pair<int, float>> controllerIdsAndYcoords;
                std::vector<std::pair<int, float>> receiverIdsAndYcoords;

                int v1Id = -1;
                for (auto& gate : plantedGates)
                {
                    //need to get inputs and outputs vec of gates.
                    for (int i = 0; i < gate.second->getNoOfInputs(); i++)
                    {

                    }

                    if (gate.second->getName() == "controller")
                    {
                        controllerIdsAndYcoords.push_back({ gate.second->getOutputsVec()[0]->id, gate.second->getGlobalBounds().top });
                        if (v1Id == -1)
                        {
                            v1Id = gate.second->getId();
                        }
                        for (auto& output : gate.second->getOutputsVec())
                        {
                            outputMap[output->id] = output;
                        }

                    }
                    else if (gate.second->getName() == "receiver")
                    {
                        receiverIdsAndYcoords.push_back({ gate.second->getInputsVec()[0]->id, gate.second->getGlobalBounds().top });
                        for (auto& input : gate.second->getInputsVec())
                        {
                            inputMap[input->id] = input;
                        }
                    }
                    else
                    {
                        for (auto& output : gate.second->getOutputsVec())
                        {
                            outputMap[output->id] = output;
                        }
                        for (auto& input : gate.second->getInputsVec())
                        {
                            inputMap[input->id] = input;
                        }
                    }
                }

                sort(controllerIdsAndYcoords.begin(), controllerIdsAndYcoords.end(), sortbysec);
                for (auto& IdAndYCoord : controllerIdsAndYcoords)
                {
                    controllerIdsVec.push_back(IdAndYCoord.first);
                }

                sort(receiverIdsAndYcoords.begin(), receiverIdsAndYcoords.end(), sortbysec);
                for (auto& IdAndYCoord : receiverIdsAndYcoords)
                {
                    receiverIdsVec.push_back(IdAndYCoord.first);
                }

                auto noOfVertices = boost::num_vertices(componentGraph);
                std::vector<size_type> dtime(noOfVertices);
                std::vector<size_type> dtime2(noOfVertices);
                std::vector<size_type> ftime(noOfVertices);

                time_pm_type dtime_pm(dtime.begin(), get(boost::vertex_index, componentGraph));
                time_pm_type dtime_pm2(dtime2.begin(), get(boost::vertex_index, componentGraph));
                time_pm_type ftime_pm(ftime.begin(), get(boost::vertex_index, componentGraph));
                size_type t = 0;
                size_type t2 = 0;
                bfs_time_visitor<time_pm_type> vis2(dtime_pm2, t2);

                boost::breadth_first_search(componentGraph, gateVertexIds[v1Id], visitor(vis2));
                std::vector<std::string> discover_order(noOfVertices);
                std::cout << "breadth first: " << std::endl;
                std::deque<int> vertQue = {};
                for (int j = 0; vertQue.size() < noOfVertices - 1; j++)
                {
                    for (int i = 0; i < noOfVertices; i++)
                    {
                        if (boost::get(dtime_pm2, i) == j)
                        {
                            vertQue.push_back(i);
                        }
                    }
                }
                for (auto& v : vertQue)
                {
                    if (componentGraph[v] != nullptr)
                    {
                        componentGraph[v]->realizeOutputs();
                        std::string compName = componentGraph[v]->getName();
                        if (compName != "receiver" && compName != "controller")
                        {
                            Gate& gt = dynamic_cast<Gate&>(*(componentGraph[v]->component));
                            algBundles.push_back(gt.getAlgBundle());
                        }

                        auto eIter = boost::out_edges(v, componentGraph);
                        for (auto it = eIter.first; it != eIter.second; it++)
                        {
                            for (auto& wire : componentGraph[*it])
                            {
                                wire->realizeOutputs();
                                if (wire->getInput())
                                {
                                    for (int i = 0; i < wire->lineVertex.getVertexCount(); i++)
                                    {
                                        wire->lineVertex[i].color = sf::Color::Red;
                                    }

                                }
                                else
                                {
                                    for (int i = 0; i < wire->lineVertex.getVertexCount(); i++)
                                    {
                                        wire->lineVertex[i].color = sf::Color::White;
                                    }
                                }
                            }

                        }
                    }
                }
                FullTrace fullTrace(algBundles, inputMap, outputMap, controllerIdsVec, receiverIdsVec);
                createdCapsules.push_back(Capsule(fullTrace, ""));
                table.clear();
                for (auto& gate : plantedGates)
                {
                    effects.colorTransition(6000, true, sf::Color::Color(0, 150, 230, 185), gate.second->getDrawables());
                }
                for (auto& wire : wires)
                {
                    effects.colorTransition(6000, true, sf::Color::Color(0, 220, 250, 165), { &wire.second->lineVertex });
                }
                plantedGates.clear();
                wires.clear();
                componentGraph.clear();
                gateVertexIds.clear();
                traceStarted = false;
                promptCapsuleName = true;
            }

            if (circuitStarted)
            {
                int v1Id;
                for (auto& gate : plantedGates)
                {
                    if (gate.second->getName() == "controller")
                    {
                        v1Id = gate.second->getId();
                        break;
                    }
                }

                auto noOfVertices = boost::num_vertices(componentGraph);
                std::vector<size_type> dtime(noOfVertices);
                std::vector<size_type> dtime2(noOfVertices);
                std::vector<size_type> ftime(noOfVertices);

                time_pm_type dtime_pm(dtime.begin(), get(boost::vertex_index, componentGraph));
                time_pm_type dtime_pm2(dtime2.begin(), get(boost::vertex_index, componentGraph));
                time_pm_type ftime_pm(ftime.begin(), get(boost::vertex_index, componentGraph));
                size_type t = 0;
                size_type t2 = 0;
                bfs_time_visitor<time_pm_type> vis2(dtime_pm2, t2);

                boost::breadth_first_search(componentGraph, gateVertexIds[v1Id], visitor(vis2));
                std::vector<std::string> discover_order(noOfVertices);
                std::cout << "breadth first: " << std::endl;
                std::deque<int> vertQue = {};
                for (int j = 0; vertQue.size() < noOfVertices - 1; j++)
                {
                    for (int i = 0; i < noOfVertices; i++)
                    {
                        if (boost::get(dtime_pm2, i) == j)
                        {
                            vertQue.push_back(i);
                        }
                    }
                }
                for (auto& v : vertQue)
                {
                    if (componentGraph[v] != nullptr)
                    {
                        componentGraph[v]->realizeOutputs();
                        auto eIter = boost::out_edges(v, componentGraph);
                        for (auto it = eIter.first; it != eIter.second; it++)
                        {
                            for (auto& wire : componentGraph[*it])
                            {
                                wire->realizeOutputs();
                                if (wire->getInput())
                                {
                                    for (int i = 0; i < wire->lineVertex.getVertexCount(); i++)
                                    {
                                        wire->lineVertex[i].color = sf::Color::Red;
                                    }

                                }
                                else
                                {
                                    for (int i = 0; i < wire->lineVertex.getVertexCount(); i++)
                                    {
                                        wire->lineVertex[i].color = sf::Color::White;
                                    }
                                }
                            }

                        }
                    }
                }
                /*
                if (circuitIterator == fullCircuit.size())
                {
                    circuitIterator = 0;
                }
                fullCircuit[circuitIterator]->realizeOutputs();
                if (fullCircuit[circuitIterator]->name == "line")
                {
                    LineConnection * wire = (LineConnection *)fullCircuit[circuitIterator];
                    if (wire->getInput())
                    {
                        for (int i = 0; i < wires[fullCircuit[circuitIterator]->getId()]->lineVertex.getVertexCount(); i++)
                        {
                            wires[fullCircuit[circuitIterator]->getId()]->lineVertex[i].color = sf::Color::Red;
                        }

                    }
                    else
                    {
                        for (int i = 0; i < wires[fullCircuit[circuitIterator]->getId()]->lineVertex.getVertexCount(); i++)
                        {
                            wires[fullCircuit[circuitIterator]->getId()]->lineVertex[i].color = sf::Color::White;
                        }
                    }
                }
                circuitIterator++;
                */
            }

            if (movingObject)
            {
                if (LMBRelease && mouse.y < (separatorTopEdge - gateHeight))
                {
                    movingObject = false;
                    LMBRelease = false;
                }
            }
        }

        int toDelete = -1;
        for (auto& gate : plantedGates)
        {

            if (gate.second->getGlobalBounds().contains(mouse))
            {
                componentInfo.setString(gate.second->component->getInfo());
                window.draw(componentInfo);
            }
            gate.second->draw(&window);

            checkForObjectInteractions(*(gate.second), wires, &window, toDelete);
        }
        if (toDelete > -1)
        {
            effects.fade(1000, true, plantedGates[toDelete]->getDrawables());
            plantedGates.erase(toDelete);
            //clean circuit from wires that are not connected
            int unconnectedWiresFound = 1;
            while (unconnectedWiresFound > 0)
            {
                int wireToDelete = -1;
                unconnectedWiresFound = 0;
                for (auto& wire : wires)
                {
                    auto idVec = wire.second->wireComponent->getOutputIds();
                    auto idVec2 = wire.second->wireComponent->getInputIds();
                    for (auto& id : idVec)
                    {
                        if (plantedGates.find(id) == plantedGates.end() && wires.find(id) == wires.end())
                        {
                            unconnectedWiresFound++;
                            wireToDelete = wire.first;
                            break;
                        }
                    }
                    for (auto& id : idVec2)
                    {
                        if (plantedGates.find(id) == plantedGates.end() && wires.find(id) == wires.end())
                        {
                            unconnectedWiresFound++;
                            wireToDelete = wire.first;
                            break;
                        }
                    }
                }
                if (wireToDelete != -1)
                {
                    wires.erase(wireToDelete);
                }
            }
            boost::clear_vertex(gateVertexIds[toDelete], componentGraph);
            boost::remove_vertex(gateVertexIds[toDelete], componentGraph);
            //clear unconnected gates
            for (auto& gate : plantedGates)
            {
                auto idVec = gate.second->component->getOutputIds();
                int outputIter = 0;
                for (auto& id : idVec)
                {
                    if (wires.find(id) == wires.end())
                    {
                        gate.second->component->unConnectOutput(outputIter);
                    }
                    outputIter++;
                }
            }
        }
        if (Tab)
        {
            for (auto& text : FKeyTexts)
            {
                window.draw(text);
            }
            for (auto& line : linesAroundFKeyTexts)
            {
                window.draw(line);
            }

        }
        if (promptCapsuleName)
        {
            if (keySentToPrompt > -1 && keySentToPrompt < 26)
            {
                std::string append = keyToStr(keySentToPrompt);
                if (!Shift)
                {
                    boost::algorithm::to_lower(append);
                }
                currentAddedPromptString.append(append);
            }
            else if (keySentToPrompt == sf::Keyboard::BackSpace)
            {
                currentAddedPromptString = currentAddedPromptString.substr(0, currentAddedPromptString.length() - 1);
            }
            else if (keySentToPrompt == sf::Keyboard::Enter)
            {
                createdCapsules.back().name = currentAddedPromptString;
                promptCapsuleName = false;
            }
            NamePromptText.setString(capsulePromptBaseString + currentAddedPromptString);
            window.draw(NamePromptText);
            keySentToPrompt = sf::Keyboard::Unknown;
        }
        
        if(bitSizePrompt)
        {
            window.draw(bitSizeSelectorText);
            int bitSz = std::stoi(bitSizeSelectorText.getString().toAnsiString());
            if (ScrollUp && bitSz < 32)
            {
                bitSizeSelectorText.setString(std::to_string(bitSz + 1));
            }
            else if (ScrollDown && bitSz > 1)
            {
                bitSizeSelectorText.setString(std::to_string(bitSz + -1));
            }
        }
        effects.refresh();
        
        
        //window.setView(view1);
        window.display();

        LMB = false; // is left mouse button hit ?
        LMBRelease = false;
        RMBRelease = false;
        Left = false;
        Right = false;
        Up = false;
        Down = false;
        Z = false;
        ZUp = false;
        S = false;
        CRelease = false;
        RRelease = false;
        ERelease = false;
        TRelease = false;
        Space = false;
        Shift = false;
        ORelease = false;
        IRelease = false;
        ScrollDown = false;
        ScrollUp = false;
        FKeyRelease = { false, false, false, false, false, false, false, false, false, false, false, false };
        Alt = false;
    }

    return 0;
}