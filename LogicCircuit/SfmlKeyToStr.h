#pragma once
#include<string>
#include <SFML/graphics.hpp>
#define KEY sf::Keyboard::Key

class SfmlKeyToStr
{

public:
    explicit SfmlKeyToStr();
    ~SfmlKeyToStr();
    std::string operator()(sf::Keyboard::Key k);
private:

    std::map<KEY, std::string> strMap;
};



SfmlKeyToStr::SfmlKeyToStr()
{
    strMap[KEY::Unknown] = "Unknown";
    strMap[KEY::A] = "A";
    strMap[KEY::B] = "B";
    strMap[KEY::C] = "C";
    strMap[KEY::D] = "D";
    strMap[KEY::E] = "E";
    strMap[KEY::F] = "F";
    strMap[KEY::G] = "G";
    strMap[KEY::H] = "H";
    strMap[KEY::I] = "I";
    strMap[KEY::J] = "J";
    strMap[KEY::K] = "K";
    strMap[KEY::L] = "L";
    strMap[KEY::M] = "M";
    strMap[KEY::N] = "N";
    strMap[KEY::O] = "O";
    strMap[KEY::P] = "P";
    strMap[KEY::Q] = "Q";
    strMap[KEY::R] = "R";
    strMap[KEY::S] = "S";
    strMap[KEY::T] = "T";
    strMap[KEY::U] = "U";
    strMap[KEY::V] = "V";
    strMap[KEY::W] = "W";
    strMap[KEY::X] = "X";
    strMap[KEY::Y] = "Y";
    strMap[KEY::Z] = "Z";
}


SfmlKeyToStr::~SfmlKeyToStr()
{
}

std::string SfmlKeyToStr::operator()(sf::Keyboard::Key k)
{
    return strMap[k];
}