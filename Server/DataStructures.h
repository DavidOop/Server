#pragma once
#include <SFML\Graphics.hpp>
#include <set>

typedef const sf::Uint32 co_Uint;
//======================================================================
struct recPack {
	recPack() = default;
	recPack(sf::Uint32 id) :_id(id) {}
	recPack(sf::Uint32 id, sf::Vector2f ver, float r, const std::set<sf::Uint32>&vec) :_ver(ver), _id(id), _rad(r) { for (auto v : vec)_vec.emplace(v); }
	recPack(const recPack& r) :recPack{ r._id, r._ver, r._rad, r._vec } {}
	sf::Vector2f _ver{}; //vertex
	sf::Uint32 _id{ 0 };
	std::set<sf::Uint32> _vec{};// vector of food snd bombs to delete
	float _rad{ 0 };
};
//======================================================================

struct sendPack {
	sendPack(sf::Uint32 id, const sf::Vector2f& ver) :_id(id), _ver(ver) {}
	sendPack(const sendPack& s) :_id(s._id), _ver(s._ver) {}
	sf::Uint32 _id;
	sf::Vector2f _ver;
};

//================================================================================
const sf::Vector2f BOARD_SIZE{ 3000.f,3000.f };
const unsigned MAX_IMAGE = 100;
const float PLAYER_RADIUS = 60.f;
const float FOOD_RADIUS = 10.f;
const float BOMB_RADIUS = 50.f;

co_Uint FOOD = 200;
co_Uint BOMBS = 15;

co_Uint PLAYER_LOWER = 200;
co_Uint PLAYER_UPPER = 300;

co_Uint FOOD_LOWER = 1000;
co_Uint FOOD_UPPER = 5000;

co_Uint BOMBS_LOWER = 6000;
co_Uint BOMBS_UPPER = 10000;

//===============================================================================================================

static float distance(const sf::Vector2f& left, const sf::Vector2f& right) { return sqrt(pow(right.x - left.x, 2) + pow(right.y - left.y, 2)); }
static bool isIntersect(const sf::Vector2f& verL, const sf::Vector2f& verR, float rL, float rR) { return distance(verL, verR) < rL + rR; }
//=============================================================================================================