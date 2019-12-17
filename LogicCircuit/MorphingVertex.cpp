#include "MorphingVertex.h"


void MorphingVertex::phaser()
{
	long ms = incidentTimer.getElapsedTime().asMilliseconds();

	if (ms < 1500)
	{
		phase = 1;

		for (int i = 0; i < numOfVertex; i++)
		{
			quad[i].position += sf::Vector2f(std::cos(rndPos[i]) * 0.25, std::sin(rndPos[i]) * 0.25);
		}
	}
	else if (ms < 3000)
	{
		if (phase == 1)
		{
			phase = 2;
		}

		for (int i = 0; i < numOfVertex; i++)
		{
			rndR[i] += 1;
			rndG[i] += 1;
			rndB[i] += 1;
			quad[i].color = sf::Color(rndR[i], rndG[i], rndB[i], rndA[i]);
		}
	}

	else
	{
		if (phase == 2)
		{
			int x = rand() % 100 - 50;
			int y = rand() % 100 - 50;
			for (int i = 0; i < numOfVertex; i++)
			{
				int x = rand() % 100 - 50;
				int y = rand() % 100 - 50;
				rotationPoints.emplace_back(sf::Vector2f(center.x + x, center.y + y));
			}

			for (int i = 0; i < numOfVertex; i++)
			{
				sf::Transform t;
				int rnd = rand() % 40 - 20;
				rotations.emplace_back(t.rotate(rnd *0.04, rotationPoints[i].x, rotationPoints[i].y));
			}
			phase = 3;
		}

		for (int i = 0; i < numOfVertex; i++)
		{
			sf::Vector2f posAfterRotate = rotations[i].transformPoint(quad[i].position);
			quad[i].position = posAfterRotate;
		}

		for (int i = 0; i < numOfVertex; i++)
		{
			int x = rand() % 100 - 50;
			int y = rand() % 100 - 50;
			sf::Vector2f newPos = sf::Vector2f(quad[i].position.x + x, quad[i].position.y + y);
			if (newPos.x < width && newPos.x > 0 && newPos.y < height && newPos.y > 0)
			{
				quad[i].position += sf::Vector2f(x, y);
			}
		}
	}
	data->window->draw(quad);
}

void MorphingVertex::commenceIncident()
{
	if (!commenceCalled)
	{
		
		quad = sf::VertexArray(sf::TrianglesStrip, numOfVertex);	
		rotations.reserve(numOfVertex);
		rndPos.reserve(numOfVertex);
		rndR.reserve(numOfVertex);
		rndG.reserve(numOfVertex);
		rndB.reserve(numOfVertex);
		rndA.reserve(numOfVertex);

		for (int i = 0; i < numOfVertex; i++)
		{
			rndPos.emplace_back(Incident::degreeToRad(rand() % 360));
			quad[i].position = center;
			rndR.emplace_back(rand() % 255);
			rndG.emplace_back(rand() % 255);
			rndB.emplace_back(rand() % 255);
			rndA.emplace_back(rand() % 155 + 100);

			quad[i].color = sf::Color(rndR[i], rndG[i], rndB[i], rndA[i]);

		}

		commenceCalled = true;
		incidentTimer.restart();
	}
	phaser();
}