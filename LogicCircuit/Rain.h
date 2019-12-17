#pragma once
#include "Incident.h"
class Rain :
	public Incident
{
	using Incident::Incident;

private:
	const int nOfPoints = 3000;
	int nOfCurPoints = 0;
	sf::VertexArray points;

	struct Particle
	{
		float velocity;
		float xChange;
		Particle(float velocity, float xChange)
			:velocity(velocity), xChange(xChange)
		{

		}
	};

	sf::Clock particleTimer;
	std::vector<Particle> particleVector;

	float windEffect = 0;
	float windChanger = 0;
	sf::Time windChangeTime = sf::milliseconds(1000);

public:
	void commenceIncident() override;
	void update();
};

