#include "ParticleRoom.h"



void ParticleRoom::update(bool LMBTrue)
{
	if (nOfCurPoints < nOfPoints)
	{
		if (LMBTrue)
		{
			sf::Vector2i mouse = sf::Mouse::getPosition(*(data->window));
			points[nOfCurPoints].position = center;
			
			float angle = atan2(mouse.y - center.y, mouse.x - center.x);
			particleVector.emplace_back(nOfCurPoints, 5.f, sf::Vector2f(std::cos(angle), std::sin(angle)));
			nOfCurPoints++;
		}
	}

	for (auto& v : particleVector)
	{
		if (points[v.pointIndex].position.x > width)
		{
			auto dot = v.dir.x * rightWallNormal.x + v.dir.y * rightWallNormal.y;
			v.dir -= sf::Vector2f(2.f * dot * rightWallNormal.x, 2.f * dot * rightWallNormal.y);
		}
		else if (points[v.pointIndex].position.y > height)
		{
			auto dot = v.dir.x * bottomWallNormal.x + v.dir.y * bottomWallNormal.y;
			v.dir -= sf::Vector2f(2.f * dot * bottomWallNormal.x, 2.f * dot * bottomWallNormal.y);
		}
		else if (points[v.pointIndex].position.x < 0)
		{
			auto dot = v.dir.x * leftWallNormal.x + v.dir.y * leftWallNormal.y;
			v.dir -= sf::Vector2f(2.f * dot * leftWallNormal.x, 2.f * dot * leftWallNormal.y);
		}
		else if (points[v.pointIndex].position.y < 0)
		{
			auto dot = v.dir.x * topWallNormal.x + v.dir.y * topWallNormal.y;
			v.dir -= sf::Vector2f(2.f * dot * topWallNormal.x, 2.f * dot * topWallNormal.y);
		}
		points[v.pointIndex].position += sf::Vector2f(v.dir.x * v.velocity, v.dir.y * v.velocity);
	}


	data->window->draw(points);
}

void ParticleRoom::commenceIncident()
{

	particleVector.reserve(nOfPoints);
	points = sf::VertexArray(sf::Points, nOfPoints);

	for (int i = 0; i < nOfPoints; i++)
	{
		points[i].position = sf::Vector2f(0, height + 1);
		points[i].color = sf::Color(255, 200, 30, 255);
	}

}

