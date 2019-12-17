#pragma once
#include <SFML/graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <fstream>
#include <vector>
#include <math.h>
#include <map>
#include <iostream>
#include <random>
#include <algorithm>
#include <memory>

#define RAD_TO_DEGREE 57.2957795130823
#define FLAME_MAX 2800
using multiVector = std::vector<std::vector<sf::Vector2f>>;
typedef std::mt19937 rng_type;
std::uniform_int_distribution<rng_type::result_type> udist(0, 20);

rng_type rng;

struct Line
{
	float x;
	float a;
};

struct Fuel
{
	double distFromFlameCenter;
	double material;
	double ignition;
	double spread;
	struct Line line;
	sf::Vector2f pos;
	sf::Vector2f dir;
	int angleInDeg;
	double force;
	double forceMultiplier;
	double distanceTravelled;

	Fuel(sf::Vector2f pos, sf::Vector2f dir, struct Line line, int angleInDeg, double material = 1.0, double spread = 0.0, double ignition = 0.0, double force = 2.0, double forceMultiplier = 0.9995, double distanceTravelled = 0.0)
		: pos(pos), dir(dir), line(line), angleInDeg(angleInDeg), material(material), spread(spread), ignition(ignition), force(force), forceMultiplier(forceMultiplier), distanceTravelled(distanceTravelled)
	{

	}
};

struct Burner
{
	void operator() (struct Fuel& f, sf::CircleShape& fuelParticle, sf::RenderWindow& window, float* angularForces)
	{
		if (f.ignition < 1.0)
		{
			f.distanceTravelled += 2.5;
			sf::Transform t;
			t.translate(f.pos.x, f.pos.y);

			f.pos += sf::Vector2f(f.dir.x * f.force * 0.5, f.dir.y * f.force * 0.5);

			f.ignition += 0.1;
			f.force *= f.forceMultiplier;
			f.forceMultiplier *= 0.9999;
			fuelParticle.setFillColor(sf::Color(209, 233, 255, 150));
			t.scale(2, 2);
			window.draw(fuelParticle, t);
		}
		else
		{
			f.distanceTravelled += 2.5;
			f.distFromFlameCenter = abs(f.pos.y + ((-1 * f.line.x) * f.pos.x) - f.line.a) / sqrt(pow(f.line.x, 2) + 1);
			sf::Transform t;
			t.translate(f.pos.x, f.pos.y);
			f.pos += sf::Vector2f(f.dir.x * f.force * 0.5, f.dir.y * f.force * 0.5);
			f.force *= f.forceMultiplier;
			f.forceMultiplier *= 0.9999;
			f.material -= 0.01 * (1.f / (angularForces[f.angleInDeg / 5] + 0.1));
			fuelParticle.setFillColor(sf::Color(242, ((12 * int(f.distFromFlameCenter) < 200) ? (12 * int(f.distFromFlameCenter)) : 200), 0));
			float r = 5 / (int(f.distFromFlameCenter * 0.05) + 1);
			if (f.distanceTravelled < 200)
			{
				r += f.distanceTravelled * 0.07;
			}
			else
			{
				r += 200 * 0.07;
			}
			t.scale(r, r);
			window.draw(fuelParticle, t);
		}
	}
};

bool hasNoMaterial(struct Fuel flame)
{
	return(flame.material < 0);
}



int numberOfNeighbors(sf::Vector2f * center, std::vector<struct Fuel> * neighbors)
{
	int n = 0;
	for (auto& v : *neighbors)
	{
		if (pow(v.pos.x - center->x, 2) + pow(v.pos.y - center->y, 2) < 4.f)
			n++;
	}
	return n;
}


void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

float jump(int time, float v0, float scale)
{
	float t = time / 1000.f;
	float yPos0 = 0.f;
	float a = -9.8;
	float v = v0 + a * t;
	float y = v0 * t + 0.5f * a * pow(t, 2.f);
	return y * scale;
}

int rndm()
{
	// seed rng first:
	rng_type::result_type const seedval = rand(); // get this from somewhere
	rng.seed(seedval);

	rng_type::result_type random_number = udist(rng);

	return random_number;
}

int flame()
{
	int width = 1800;
	int height = 1000;
	sf::RenderWindow window(sf::VideoMode(width, height), "vertex");

	sf::RenderStates states = sf::RenderStates(sf::BlendMode::BlendMode());

	srand(time(NULL));
	sf::Time t1 = sf::milliseconds(0);

	//Lets define moveable character and set the initial pos
	float characterHeight = 40.f;
	float characterWidth = 40.f;
	float initialX = width / 2;
	float initialY = height - 40;
	float jumpVelocity = 7.f;
	sf::Texture particleTexture;
	particleTexture.loadFromFile("resources/Images/particleBlu.png");
	sf::RectangleShape character(sf::Vector2f(characterWidth, characterHeight));
	character.setTexture(&particleTexture);
	character.setPosition(sf::Vector2f(initialX, initialY));
	sf::Vector2f curPos;
	int centerX;
	int centerY;
	float characterAngle = 0.f;
	sf::Transform characterTransform;


	//Lets create a lightning effect

	sf::Texture particleTexture2;
	particleTexture2.loadFromFile("resources/Images/lightning3.png");
	float lSize = 50.0;
	sf::RectangleShape bigLightning(sf::Vector2f(lSize, lSize));
	bigLightning.setTexture(&particleTexture2);
	multiVector lightningVector;
	std::vector<sf::Vector2f> v;
	for (int i = 0; i < 10; i++)
	{
		lightningVector.push_back(v);
	}


	//Lets create a fire effect

	float fOriginalSize = 7.f;
	sf::Texture particleTexture3;
	particleTexture3.loadFromFile("resources/Images/burn4.png");
	sf::RectangleShape fireParticle(sf::Vector2f(fOriginalSize, fOriginalSize));
	fireParticle.setTexture(&particleTexture3);

	sf::CircleShape fuelParticle;

	fuelParticle.setFillColor(sf::Color::Cyan);
	fuelParticle.setRadius(1.f);
	fuelParticle.setOrigin(sf::Vector2f(1.f, 1.f));
	std::vector<struct Fuel> fuelVector;

	fuelVector.reserve(FLAME_MAX);
	std::vector<struct Fuel> fuelSpreadVector;
	fuelSpreadVector.reserve(FLAME_MAX / 5);


	float flameAngle = 600.f;
	sf::Vector2f flameLaunchPoint = sf::Vector2f(0, 0);
	Line flameLine;
	sf::VertexArray line(sf::Lines, 2);
	line[0].position = sf::Vector2f(0, 0);
	line[1].position = sf::Vector2f(0, 0);
	float rotate = 0;
	float multiplier = 0.999;
	float fGravity = 1.0;
	
	float angularForces[73];
	for (int i = 0; i < 73; i++)
	{
		angularForces[i] = 0.f;
	}

	int radToDegree;


	

	struct Burner burner;

	bool LMB = false; // is left mouse button hit ?
	bool Left = false;
	bool Right = false;
	bool Up = false;
	bool Down = false;
	bool Z = false;
	bool ZUp = false;
	bool S = false;
	bool Space = false;

	bool JumpInitiated = false;
	bool LightningLaunched = false;
	bool FlameLaunched = false;
	int shootingTime;
	float scale = 800.f / 3.f;

	sf::Clock jumpTimer;
	sf::Clock lightningTimer;
	sf::Clock flameTimer;
	sf::Clock flameEvaporateTimer;
	flameEvaporateTimer.restart();
	sf::Clock lightningSizeTimer;
	lightningSizeTimer.restart();
	sf::Clock clock;


	sf::Clock flameDelay;
	sf::Clock deltaTime; // SFML clock to keep track of our framerate
	float dt = 0; // the time a frame requires to be drawn (=1/framerate). I set it at 0.0000001 to avoid getting undefined behaviors during the very first frame

	while (window.isOpen())
	{

		sf::Event event;
		while (window.pollEvent(event))
		{

			// Request for closing the window
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed)
			{

				if (event.key.code == sf::Keyboard::Space)
				{
				}
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				LMB = true;
			}
			else
			{
				LMB = false;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				Right = true;
			}
			else
			{
				Right = false;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				Down = true;
			}
			else
			{
				Down = false;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				Left = true;
			}
			else
			{
				Left = false;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				Up = true;
			}
			else
			{
				Up = false;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
			{
				Z = true;
			}
			else
			{
				Z = false;
				ZUp = true;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				S = true;
			}
			else
			{
				S = false;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				Space = true;
			}
			else
			{
				Space = false;
			}
		}
		curPos = character.getPosition();
		centerX = curPos.x + characterWidth / 2;
		centerY = curPos.y + characterHeight / 2;


		if (Right)
		{
			if (!Space)
			{
				if (character.getPosition().x < width - characterWidth)
				{

					sf::Vector2f newPos = sf::Vector2f(curPos.x + 1.f, curPos.y);
					character.setPosition(newPos);
				}
			}
			else
			{
				characterAngle += 0.01;
			}
		}
		if (Left)
		{
			if (!Space)
			{
				if (character.getPosition().x > 0)
				{
					sf::Vector2f newPos = sf::Vector2f(curPos.x - 1.f, curPos.y);
					character.setPosition(newPos);
				}
			}
			else
			{
				characterAngle -= 0.01;
			}
		}
		if (!Space)
		{
			if (Up && !JumpInitiated)
			{
				JumpInitiated = true;
				jumpTimer.restart();
			}
		}


		if (JumpInitiated)
		{
			float t = jumpTimer.getElapsedTime().asMilliseconds();
			float jumpHeight = jump(t, jumpVelocity, scale);
			sf::Vector2f curPos = character.getPosition();
			if (jumpHeight < 0 && t > 0)
			{
				JumpInitiated = false;
			}
			else
			{
				character.setPosition(curPos.x, initialY - jumpHeight);
			}
		}
		if (Z)
		{
			LightningLaunched = true;
			lightningTimer.restart();
			if (ZUp)
			{
				for (auto& v : lightningVector)
				{

					if (v.empty())
					{
						ZUp = false;
						v.push_back(sf::Vector2f(curPos.x + characterWidth / 2.f, curPos.y + characterHeight / 2.f));
						break;
					}
				}
			}
		}
		if (S)
		{
			FlameLaunched = true;
			flameTimer.restart();
			if (flameAngle != characterAngle || (flameLaunchPoint != curPos))
			{
				flameAngle = characterAngle;
				flameLaunchPoint = curPos;
				sf::Vector2f A = sf::Vector2f(centerX, centerY);
				sf::Vector2f dir = sf::Vector2f(std::cos(characterAngle) * 200, std::sin(characterAngle) * 200);
				sf::Vector2f B = sf::Vector2f(centerX, centerY) + dir;
				flameLine.x = (A.y - B.y) / (A.x - B.x);
				flameLine.a = A.y + flameLine.x * (-1 * A.x);

				if (flameAngle < 0)
				{
					radToDegree = 360 + (int(flameAngle * RAD_TO_DEGREE)) % 360;
				}
				else if (flameAngle > 0)
				{
					radToDegree = (int(flameAngle * RAD_TO_DEGREE)) % 360;
				}
				else
				{
					radToDegree = 0;
				}
			}
			
			if(angularForces[radToDegree / 5] < 1.0)
				angularForces[radToDegree / 5] += 0.005;
			float offset = 0.05;
			if (flameDelay.getElapsedTime().asMilliseconds() > 4)
			{
				flameDelay.restart();

				float var = (rand() % 1500 - 750) * 1e-4;
				float var2 = (rand() % 1500 - 750) * 1e-4;
				sf::Vector2f direction = sf::Vector2f(std::cos(characterAngle + var) * 5, std::sin(characterAngle + var2) * 5);
				fuelVector.emplace_back(sf::Vector2f(centerX, centerY) + direction, direction, flameLine, radToDegree);

				var = (rand() % 1500 - 750) * 1e-4;
				var2 = (rand() % 1500 - 750) * 1e-4;
				direction = sf::Vector2f(std::cos(characterAngle + offset + var) * 5, std::sin(characterAngle + offset + var2) * 5);
				fuelVector.emplace_back(sf::Vector2f(centerX, centerY) + direction, direction, flameLine, radToDegree);

				var = (rand() % 1500 - 750) * 1e-4;
				var2 = (rand() % 1500 - 750) * 1e-4;
				direction = sf::Vector2f(std::cos(characterAngle - offset + var) * 5, std::sin(characterAngle - offset + var2) * 5);
				fuelVector.emplace_back(sf::Vector2f(centerX, centerY) + direction, direction, flameLine, radToDegree);

			}
		}

		if (LightningLaunched)
		{
			for (auto& v : lightningVector)
			{
				if (!v.empty())
				{
					sf::Vector2f addition = v.back();
					int x = rndm() - 10;
					int y = rndm() - 10;
					if (Left)
						x -= 5;
					if (Right)
						x += 5;
					if (Up)
						y -= 5;
					if (Down)
						y += 5;
					addition += sf::Vector2f(x, y);

					v.push_back(addition);

					if (addition.x > width || addition.x < 0 || addition.y > height || addition.y < 0)
					{
						shootingTime = lightningTimer.getElapsedTime().asMilliseconds();
						v.clear();
					}
					if (v.size() > 70)
					{
						v.erase(v.begin());
					}
				}
			}
		}

		window.clear();

		if (LightningLaunched)
		{

			for (auto& v : lightningVector)
			{
				if (!v.empty())
				{
					if ((Left || Right || Up || Down) && lightningTimer.getElapsedTime().asMilliseconds() > 50)
					{
						lightningSizeTimer.restart();
						lSize = rand() % 120;
					}
					else if (lightningSizeTimer.getElapsedTime().asMilliseconds() > 60)
					{
						lightningSizeTimer.restart();
						lSize = rand() % 80 + 20;
					}
					bigLightning.setSize(sf::Vector2f(lSize, lSize));
					for (auto& posVect : v)
					{
						bigLightning.setPosition(sf::Vector2f(float(posVect.x) - lSize / 2.f, float(posVect.y) - lSize / 2.f));
						window.draw(bigLightning);
					}
				}
			}
		}
		for (auto& f : fuelVector)
		{
			burner(f, fuelParticle, window, angularForces);
		}
		fuelVector.insert(fuelVector.end(), fuelSpreadVector.begin(), fuelSpreadVector.end());
		fuelSpreadVector.clear();




		fuelVector.erase(
			std::remove_if(fuelVector.begin(), fuelVector.end(), hasNoMaterial),
			fuelVector.end());


		window.draw(character);

		window.display();


		dt = deltaTime.restart().asMilliseconds(); // measure the time it took to complete all the calculations for this particular frame (dt=1/framerate) 
	}
	return 0;
}