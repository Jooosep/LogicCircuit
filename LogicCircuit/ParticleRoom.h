#pragma once
#include "Incident.h"

class ParticleRoom :
	public Incident
{
	using Incident::Incident;

private:
	const int nOfPoints = 5000;
	int nOfCurPoints = 0;
	sf::VertexArray points;
	struct Particle
	{
		float velocity;
		sf::Vector2f dir;
		size_t pointIndex;

		Particle(size_t pointIndex, float velocity, sf::Vector2f dir)
			:pointIndex(pointIndex), velocity(velocity), dir(dir)
		{

		}
	};

	std::vector<Particle> particleVector;

	sf::Vector2f rightWallNormal = sf::Vector2f(std::sin(degreeToRad(90)) * -1, std::cos(degreeToRad(90)));
	sf::Vector2f bottomWallNormal = sf::Vector2f(std::sin(degreeToRad(180)) * -1, std::cos(degreeToRad(180)));
	sf::Vector2f leftWallNormal = sf::Vector2f(std::sin(degreeToRad(270)) * -1, std::cos(degreeToRad(270)));
	sf::Vector2f topWallNormal = sf::Vector2f(std::sin(degreeToRad(360)) * -1, std::cos(degreeToRad(360)));

	

public:
	void commenceIncident() override;
	void update(bool LMBTrue);
};

