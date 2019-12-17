#pragma once
#include "Incident.h"
class MorphingVertex :
	public Incident
{
	using Incident::Incident;

private:

	int phase = 0;
	int numOfVertex = 3;

	sf::VertexArray quad;
	std::vector<int> rndPos;
	std::vector<int> rndR;
	std::vector<int> rndG;
	std::vector<int> rndB;
	std::vector<int> rndA;
	bool commenceCalled = false;


	std::vector<sf::Vector2f> rotationPoints;

	std::vector<sf::Transform> rotations;

	sf::Clock incidentTimer;

public:
	void commenceIncident() override;

	void phaser();
};

