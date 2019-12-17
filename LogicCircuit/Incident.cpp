#include "Incident.h"


template<typename Tfirst,typename... Trest>
void Incident::printData(Tfirst first, Trest... rest)
{
	std::cout << first << std::endl;
	printData(rest);
}

Incident::Incident(sf::RenderWindow* window)
	:data(new struct Data)
{
	this->data->window = window;
	data->lifeTime.restart();
	width = data->window->getSize().x;
	height = data->window->getSize().y;
	center = sf::Vector2f(width / 2.f, height / 2.f);
}

Incident::~Incident()
{
	delete data;
}

//move constructor
Incident::Incident(Incident &&other)
	: data(other.data)
{
	other.data = nullptr;
	std::cout << "Move ctor" << std::endl;
}

//move assignment operator
Incident& Incident::operator=(Incident&& other)
{
	if (this == &other)
	{
		return *this;
	}

	std::cout << "MOVE operation" << std::endl;

	data = other.data;
	other.data = nullptr;

	return *this;
}

std::ostream& operator<<(std::ostream& os, const Incident& i)
{
	if (i.data != nullptr)
	{
		os << i.data->window << ' ' << i.data->lifeTime.getElapsedTime().asMilliseconds() << " ms" << std::endl;
	}
	else
	{
		os << "object data is null" << std::endl;
	}
	return os;
}

float Incident::degreeToRad(int degree)
{
	return degree * (PI / 180.f);
}
