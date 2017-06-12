#pragma once
#include "DataStructures.h"

class Circle
{
public:
	Circle(sf::Uint32 id, const sf::Vector2f& ver, float r) :_id(id), _vertex(ver), _radius(r) {}
	Circle(const Circle& c) :Circle(c._id, c._vertex, c._radius) {}
	virtual ~Circle() {}

	virtual sf::Uint32 getId() const { return _id; }
	virtual const sf::Vector2f& getVertex()const { return _vertex; }
	virtual float getRadius()const { return _radius; }

	virtual void setVertex(const sf::Vector2f& ver) { _vertex = ver; }
	virtual void setRadius(float r) { _radius = r; }
protected:
	sf::Uint32 _id;
	sf::Vector2f _vertex;
	float _radius;
};
//================================================================================
class Player :public Circle {
public:
	Player(sf::Uint32 id, sf::Uint32 im, const sf::Vector2f& ver, float r = PLAYER_RADIUS) :Circle(id, ver, r), _image(im) {}
	Player(const Player& pl) :Player(pl._id, pl._image, pl._vertex, pl._radius) {}
	~Player() {}

	sf::Uint32 getImage()const { return _image; }
	void setImage(sf::Uint32 im) { _image = im; }

private:
	sf::Uint32 _image;
};
