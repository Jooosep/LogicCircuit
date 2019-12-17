#include "Wind.h"

void Wind::update(bool LMB)
{

	sf::Transform t;

	lineRotation += rotationJump;
	rotationJump += 0.0001;
	sf::Vector2i mouse = sf::Mouse::getPosition(*(data->window));
	line[1].position = data->window->mapPixelToCoords(mouse);


	if (nOfCurPoints < nOfPoints)
	{
		points[nOfCurPoints].position = line[1].position;
		float vel = line[0].position.y / 250.f;
        Particle v = Particle(nOfCurPoints, vel, acceleration, line[1].position, sf::Vector2f(std::sin(lineRotation), std::cos(lineRotation)), 0);
		particleVector.emplace_back(nOfCurPoints, vel, acceleration, line[1].position, sf::Vector2f(std::sin(lineRotation), std::cos(lineRotation)), 0);
		nOfCurPoints++;
	}

	for (auto& v : particleVector)
	{
		v.lifeTime -= particleTimer.getElapsedTime();
        v.accTime -= particleTimer.getElapsedTime();
        v.dirTime -= particleTimer.getElapsedTime();
        v.velTime -= particleTimer.getElapsedTime();
		if (LMB)
		{
			v.acceleration = 16.f * acceleration ;
		}

        v.alg(v);
        

		v.velocity = v.velocity + ((particleTimer.getElapsedTime().asSeconds() * v.acceleration));

		if (v.lifeTime.asMilliseconds() < 0)
		{
			if (!particleReset)
			{
				points[v.pointIndex].position = line[1].position;
				float vel = abs(line[1].position.y - center.y) / 200.f;
				float acc = acceleration * -1.f;
				if (LMB)
				{
					v = { v.pointIndex, vel, acc, line[1].position, sf::Vector2f(std::sin(lineRotation), std::cos(lineRotation)), 0};
				}
				else
				{
					v = { v.pointIndex, vel, acceleration, line[1].position, sf::Vector2f(std::sin(lineRotation), std::cos(lineRotation)), 0};
				}
				particleReset = 1;
			}
			else 
			{
                points[v.pointIndex].position = sf::Vector2f(2000, 2000);
			}
		}
        points[v.pointIndex].position += sf::Vector2f(v.velocity * v.dir.x, v.velocity * v.dir.y);
	}

	particleReset = 0;
	data->window->draw(points, t);
    //data->window->draw(leftPoints, t);
    //data->window->draw(rightPoints, t);
    //data->window->draw(topPoints, t);
    //data->window->draw(botPoints, t);
	particleTimer.restart();

}

void Wind::commenceIncident()
{
	
	fibs.push_back(fib<1>::value);
	fibs.push_back(fib<2>::value);
	fibs.push_back(fib<3>::value);
	fibs.push_back(fib<4>::value);
	
	fibs.push_back(fib<5>::value);
	fibs.push_back(fib<6>::value);
	fibs.push_back(fib<7>::value);
	fibs.push_back(fib<8>::value);
	fibs.push_back(fib<9>::value);
	fibs.push_back(fib<10>::value);
	fibs.push_back(fib<11>::value);
	
	fibs.push_back(fib<12>::value);
	fibs.push_back(fib<13>::value);
	fibs.push_back(fib<14>::value);
	fibs.push_back(fib<15>::value);//987
	
	sizeOfFibs = fibs.size();
	int i = 0;
	for (auto& f : fibs)
	{
		i++;
		std::cout << i << ": " << f << std::endl;
	}

	facts.push_back(fact<1>::value);
	facts.push_back(fact<2>::value);
	facts.push_back(fact<3>::value);
	facts.push_back(fact<4>::value);
	facts.push_back(fact<5>::value);
	facts.push_back(fact<6>::value);

	particleTimer.restart();
	particleVector.reserve(nOfPoints);
	points = sf::VertexArray(sf::Points, nOfPoints);

	line = sf::VertexArray(sf::Lines, 2);
	for (int i = 0; i < nOfPoints; i++)
	{
		points[i].position = sf::Vector2f(data->window->getSize().x -1, data->window->getSize().y + 1);
		points[i].color = sf::Color(57, 255, 20, 255);
	}

	line[0].position = sf::Vector2f(center.x, center.y + 70);
	line[1].position = sf::Vector2f(center.x, center.y - 70);
}
