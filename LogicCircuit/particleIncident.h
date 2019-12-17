#pragma once
#include "Incident.h"

typedef void(*alg_function)(struct Particle& p);

extern std::vector<alg_function> algoFunctions;


struct Particle
{
    int pointIndex;
    float velocity;
    sf::Vector2f origin;
    float acceleration;
    sf::Vector2f dir;
    int iterator;
    sf::Time lifeTime;
    sf::Time dirTime;
    sf::Time velTime;
    sf::Time accTime;
    alg_function alg;
    bool live;

    Particle(int pointIndex,
        float velocity,
        float acceleration,
        sf::Vector2f origin,
        sf::Vector2f dir,
        int algorithmIndex,
        int iterator = 0,
        sf::Time lifeTime = sf::milliseconds(1000),
        sf::Time dirTime = sf::milliseconds(500),
        sf::Time velTime = sf::milliseconds(300),
        sf::Time accTime = sf::milliseconds(50))
        :pointIndex(pointIndex), velocity(velocity), acceleration(acceleration), origin(origin), iterator(iterator), dir(dir), alg(algoFunctions[algorithmIndex]), lifeTime(lifeTime), velTime(velTime), accTime(accTime), dirTime(dirTime), live(true)
    {

    }
};

namespace {

    void alg1(struct Particle& p)
    {
        if (p.dirTime.asMilliseconds() < 0)
        {
            float angle = 90.f;
            p.dir = sf::Vector2f(std::cos(angle * p.dir.x) - std::sin(angle * p.dir.y), std::sin(angle * p.dir.x) - std::cos(angle * p.dir.y));
            p.dirTime = sf::milliseconds(500);
        }
        if (p.velTime.asMilliseconds() < 0)
        {
            p.velocity *= -1.f;
            p.velTime = sf::milliseconds(800);
        }
        if (p.accTime.asMilliseconds() < 0)
        {
            if (p.acceleration > 4.f)
            {
                p.acceleration *= 0.9f;
            }
            p.acceleration *= 1.1f;
            p.accTime = sf::milliseconds(50);
            
        }
    }
    void alg2(struct Particle& p)
    {

    }
    void alg3(struct Particle& p)
    {

    }
    void alg4(struct Particle& p)
    {

    }
}

class particleIncident :
	public Incident
{
	using Incident::Incident;

protected:
	sf::Clock particleTimer;
	std::vector<Particle> particleVector;
	sf::VertexArray points;
	
public:

};

