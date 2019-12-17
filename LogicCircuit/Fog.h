#pragma once
#include "Incident.h"
class Fog :
	public Incident
{
	using Incident::Incident;

private:
	const int nOfPoints = 80000;
	int nOfCurPoints = 0;
	sf::VertexArray points;
	struct Particle
	{
		float velocity;
		sf::Time lifeTime;
		Particle(float velocity, sf::Time lifeTime = sf::milliseconds(2000))
			:velocity(velocity), lifeTime(lifeTime)
		{

		}
	};
	sf::Clock particleTimer;
	std::vector<Particle> particleVector;


public:
	void commenceIncident() override;
	void update();
};

