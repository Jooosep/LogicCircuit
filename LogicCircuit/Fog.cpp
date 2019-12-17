#include "Fog.h"

void Fog::update()
{
	if (nOfCurPoints < nOfPoints)
	{
		points[nOfCurPoints].position = sf::Vector2f(rand() % width, height);
		nOfCurPoints++;
		particleVector.emplace_back(rand() % 3 + 1, sf::milliseconds((rand() % 1000) + 1000));
	}

	for (int i = 0; i < nOfCurPoints; i++)
	{
		particleVector[i].lifeTime -= particleTimer.getElapsedTime();
		if (particleVector[i].lifeTime.asMilliseconds() < 0)
		{
			points[i].position = sf::Vector2f(rand() % width, height);
			int rnd = rand() % 50 - 25;
			int rnd2 = rand() % 3 + 1;
			//particleVector[i] = { line[1].position.y / 300.f, sf::Vector2f(rnd * 0.01, rnd2) , sf::milliseconds((rand() % 1000) + 1000) };
			particleVector[i] = { (rand() % 10 + 10) * 0.1f, sf::milliseconds((rand() % 1000) + 1000) };
		}
		float vel = particleVector[i].velocity;
		points[i].position += sf::Vector2f((rand() % 20 - 10) * 0.02, -0.1 * vel);
	}

	data->window->draw(points);
	particleTimer.restart();
}

void Fog::commenceIncident()
{
	particleTimer.restart();
	particleVector.reserve(nOfPoints);
	points = sf::VertexArray(sf::Points, nOfPoints);

	for (int i = 0; i < nOfPoints; i++)
	{
		points[i].position = sf::Vector2f(0, height + 1);
		points[i].color = sf::Color(220, 200, 230, 60);
	}

}
