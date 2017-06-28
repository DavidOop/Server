#pragma once
#include <SFML\Graphics.hpp>
//=============================================================================
//					singleton for the time
//=============================================================================
class TimeClass
{
public:
	inline static TimeClass& instance() { static TimeClass tone; return tone; }
	~TimeClass() {}

	inline float RestartClock() { return m_clock.restart().asSeconds(); }
private:
	TimeClass() = default;
	TimeClass(const TimeClass&) = delete;
	TimeClass& operator=(const TimeClass&) = delete;

	sf::Clock m_clock;
};

