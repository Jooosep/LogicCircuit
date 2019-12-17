#pragma once
#include "Incident.h"


class Explosion :
	public Incident
{
	using Incident::Incident;

private:
	sf::CircleShape circle;
	const float startingRadius = 1;
	sf::Texture texture;
	float rotate = 0.f;
	bool commenceCalled = false;

	const int downtime = 250;
	const int timeLimit = 3000;
	

public:
	void commenceIncident() override;
};

