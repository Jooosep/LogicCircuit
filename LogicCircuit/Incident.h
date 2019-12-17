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
#include <array>
#define PI 3.14159265359

struct Data {
	sf::RenderWindow* window;
	sf::Clock lifeTime;
	sf::Vector2f pos;
};

class Incident
{
protected:
	
	struct Data * data;

	int width;
	int height;
	sf::Vector2f center;

	template<typename Tfirst, typename... Trest>
	void printData(Tfirst first, Trest... rest);

public:
	Incident(sf::RenderWindow* window);
	Incident(Incident &&other);
	
	~Incident();

	Incident& operator=(Incident&& other);

	friend std::ostream& operator<<(std::ostream& os, const Incident& i);

	virtual void commenceIncident() = 0;

	float degreeToRad(int degree);
};

