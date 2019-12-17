#pragma once
#include "particleIncident.h"
class Wind :
	public particleIncident
{
	using particleIncident::particleIncident;

private:
	template <size_t> struct fib;

	//Báse cases
	template <> struct fib<0> { const static auto value = 1; };
	template<> struct fib<1> { const static auto value = 1; };


	template <size_t n> struct fib
	{
		const static auto value = fib<n - 1>::value + fib<n - 2>::value;
	};



	template <size_t> struct fact;

	//Báse cases
	template <> struct fact<0> { const static auto value = 1; };
	template<> struct fact<1> { const static auto value = 1; };


	template <size_t n> struct fact
	{
		const static auto value = n * fact<n - 1>::value;
	};

    
	std::vector<int> fibs;
	int sizeOfFibs;
	std::vector<int> facts;
	
    sf::VertexArray leftPoints;
    sf::VertexArray rightPoints;
    sf::VertexArray topPoints;
    sf::VertexArray botPoints;

	const int nOfPoints = 2000;
	int nOfCurPoints = 0;
	
	float acceleration = 0.35f;
	
	sf::VertexArray line;
	float lineRotation = 0;
	float rotationJump = 0;
	bool particleReset = 0;
	int constantB = 5;


public:
	void commenceIncident() override;
	void update(bool LMB);
};

