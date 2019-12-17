#include "Cube.h"


bool Cube::moveVertexTowardTargetPosition(sf::Vertex& point, float& angle, sf::Vector2f& tarPos)
{

	if (abs(atan2((tarPos.y - point.position.y), (tarPos.x - point.position.x)) - angle) > 0.05)
	{
		point.position = tarPos;
		phase++;
		return true;
	}
	else
	{
		point.position += sf::Vector2f(std::cos(angle), std::sin(angle));
		return false;
	}

}

void Cube::phaser()
{
	if (phase == -1)
	{
		for (int i = 0; i < vertexCount; i++)
		{
			translateAngles.emplace_back(atan2((stripTargetPositions[i].y - triangleStrip[i].position.y), (stripTargetPositions[i].x - triangleStrip[i].position.x)));
		}
		for (int i = 0; i < 4; i++)
		{
			quadBottomTranslateAngles.emplace_back(atan2((bottomTargetPositions[i].y - quadBottom[i].position.y), (bottomTargetPositions[i].x - quadBottom[i].position.x)));
			quadTopTranslateAngles.emplace_back(atan2((topTargetPositions[i].y - quadTop[i].position.y), (topTargetPositions[i].x - quadTop[i].position.x)));
		}
		phase = 0;
	}
	if (phase < vertexCount)
	{
		moveVertexTowardTargetPosition(triangleStrip[phase], translateAngles[phase], stripTargetPositions[phase]);
	}
	else if (phase < vertexCount + 8)
	{
		for (int i = 0; i < 4; i++)
		{
			moveVertexTowardTargetPosition(quadBottom[i], quadBottomTranslateAngles[i], bottomTargetPositions[i]);
			moveVertexTowardTargetPosition(quadTop[i], quadTopTranslateAngles[i], topTargetPositions[i]);
		}
	}
	data->window->draw(quadBottom);
	data->window->draw(quadTop);
	data->window->draw(triangleStrip);
}

void Cube::commenceIncident()
{
	if (!commenceCalled)
	{
		commenceCalled = true;
		translateAngles.reserve(vertexCount);

		stripTargetPositions.reserve(vertexCount);
		stripTargetPositions.emplace_back(zeroPoint);//0
		stripTargetPositions.emplace_back(sf::Vector2f(zeroPoint.x, zeroPoint.y + cubeHeight));//1
		stripTargetPositions.emplace_back(sf::Vector2f(zeroPoint.x + cubeWidth, zeroPoint.y));//2
		stripTargetPositions.emplace_back(sf::Vector2f(zeroPoint.x + cubeWidth, zeroPoint.y + cubeHeight));//3
		stripTargetPositions.emplace_back(sf::Vector2f(stripTargetPositions[2].x + frontXDif, stripTargetPositions[2].y + frontYDif));//4
		stripTargetPositions.emplace_back(sf::Vector2f(stripTargetPositions[3].x + frontXDif, stripTargetPositions[3].y + frontYDif));//5
		stripTargetPositions.emplace_back(sf::Vector2f(stripTargetPositions[4].x - cubeWidth, stripTargetPositions[4].y));//6
		stripTargetPositions.emplace_back(sf::Vector2f(stripTargetPositions[5].x - cubeWidth, stripTargetPositions[5].y));//7
		stripTargetPositions.emplace_back(zeroPoint);//8
		stripTargetPositions.emplace_back(sf::Vector2f(zeroPoint.x, zeroPoint.y + cubeHeight));//9



		quadBottom = sf::VertexArray(sf::Quads, 4);
		quadTop = sf::VertexArray(sf::Quads, 4);

		for(int i = 0; i < 9; i++)
		{ 
			if (i % 2 == 0)
			{
				topTargetPositions.emplace_back(stripTargetPositions[i]);
			}
			else
			{
				bottomTargetPositions.emplace_back(stripTargetPositions[i]);
			}
		}

		

		triangleStrip = sf::VertexArray(sf::TrianglesStrip, vertexCount);
		for (int i = 0; i < vertexCount; i++)
		{
			triangleStrip[i].position = center;
			

			rndR.emplace_back(rand() % 100 + 155);
			rndG.emplace_back(rand() % 100 + 155);
			rndB.emplace_back(rand() % 100 + 155);


			triangleStrip[i].color = sf::Color(rndR[i], rndG[i], rndB[i]);
			triangleStrip[i].color.a = 90;
			
		}

		for (int i = 0; i < 4; i++)
		{
			quadTop[i].position = sf::Vector2f(topTargetPositions[i].x, topTargetPositions[i].y - 1000);
			quadBottom[i].position = sf::Vector2f(bottomTargetPositions[i].x, bottomTargetPositions[i].y + 1000);
			quadTop[i].color = sf::Color(rndR[i], rndG[i], rndB[i], 100);
			quadBottom[i].color = sf::Color(rndR[i], rndG[i], rndB[i], 100);
		}

	}

	phaser();
}