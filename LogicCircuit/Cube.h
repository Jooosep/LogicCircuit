#pragma once
#include "Incident.h"
class Cube :
	public Incident
{
	using Incident::Incident;

private:

	bool commenceCalled = false;
	int phase = -1;
	const size_t vertexCount = 10;
	sf::VertexArray triangleStrip;
	sf::VertexArray quadTop;
	sf::VertexArray quadBottom;
	std::vector<sf::Vector2f> stripTargetPositions;
	std::vector<sf::Vector2f> topTargetPositions;
	std::vector<sf::Vector2f> bottomTargetPositions;
	std::vector<float> translateAngles;
	std::vector<float> quadBottomTranslateAngles;
	std::vector<float> quadTopTranslateAngles;

	const int cubeHeight = 200;
	const int cubeWidth = 200;
	const int frontXDif = -70;
	const int frontYDif = 90;
	std::vector<int> rndR;
	std::vector<int> rndG;
	std::vector<int> rndB;
	sf::Vector2f zeroPoint = sf::Vector2f(center.x - cubeWidth / 2.f, center.y - cubeHeight / 2.f);//800, 400
	sf::Transform rotation;
	sf::Clock incidentTimer;

public:

	void commenceIncident() override;

	void phaser();

	bool moveVertexTowardTargetPosition(sf::Vertex& point, float& angle, sf::Vector2f& tarPos);

};

