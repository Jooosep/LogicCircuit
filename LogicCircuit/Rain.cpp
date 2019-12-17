#include "Rain.h"


void Rain::update()
{
	if (nOfCurPoints < nOfPoints)
	{
		points[nOfCurPoints].position = sf::Vector2f(rand() % (width + 800) - 400, 0);
		nOfCurPoints++;
		particleVector.emplace_back(rand() % 4 + 3.f, windEffect);
	}
	windChangeTime -= particleTimer.getElapsedTime();
	if (windChangeTime.asMilliseconds() < 0);
	{
		windChangeTime = sf::milliseconds(rand() % 500 + 500);
		windChanger = (rand() % 100 - 50) * 0.0004;
	}
	if (windEffect > 5.f || windEffect < -5.f)
	{
		windChanger *= -1;
	}
	windEffect += windChanger;

	for (int i = 0; i < nOfCurPoints; i++)
	{

		if (points[i].position.y > height)
		{
			points[i].position = sf::Vector2f(rand() % (width + 800) - 400, 0);
			particleVector[i] = { rand() % 3 + 3 + abs(windEffect), windEffect};
		}
		float vel = particleVector[i].velocity;
		points[i].position += sf::Vector2f(particleVector[i].xChange, vel);
	}

	data->window->draw(points);
	particleTimer.restart();
}

void Rain::commenceIncident()
{
	particleTimer.restart();
	particleVector.reserve(nOfPoints);
	points = sf::VertexArray(sf::Points, nOfPoints);

	for (int i = 0; i < nOfPoints; i++)
	{
		points[i].position = sf::Vector2f(0, -1);
		points[i].color = sf::Color(150, 150, 255, 230);
	}

}