#pragma once

#include "Receive.h"
#include "Sender.h"
#include "Circle.h"

#include <unordered_map>
#include <memory>

#include <process.h>
#include <Windows.h>

typedef std::unordered_map<sf::Uint32, std::unique_ptr<Circle>> un_map;
//===================================================================================================
class Board
{
public:
	Board() = default;
	Board(Network&, Receive&, Sender&/*, Computer&*/);

	~Board();

	//map functions
	auto begin() { return m_data.begin(); }
	auto end() { return m_data.end(); }

	//override functions for tha data map
	template<typename T>
	auto find(const T& key) { return m_data.find(key); }
	template<typename T>
	auto emplace(const sf::Uint32& key, T& val) { return m_data.emplace(key, std::make_unique<T>(val)); }
	template<typename T>
	auto erase(const T& key) { return m_data.erase(key); }


	Network& net() { return m_network; }

	sf::Uint32 findId(const sf::Uint32 l, const sf::Uint32 u);
	sf::Vector2f addVertex(float);
	bool collide(sf::Vector2f, float);
	void run();
	void addFoodAndBombs();
	void add(co_Uint, co_Uint, co_Uint, const float, co_Uint, sf::Uint32&);

	void addComputerPlayers();
	inline void pushToSender(const sendPack& s) { m_sender.push(s); }
	inline void notify_one() { m_sender.notify_one(); }//notify the sender there is data to send

	inline void wait() { m_receive.wait(); }
	//inline auto unique_lock() { return m_receive.unique_lock(); }
	inline void notify_rec() { m_receive.notify_one(); }

	void receiveLoop(std::queue<recPack>&);
	void precessLoop(std::queue<recPack>&);

	void addClient(Board&, sf::Uint32);

private:
	un_map m_data;
	Network& m_network;
	Receive& m_receive;
	Sender& m_sender;


	//Computer& m_Cplayer;
	//std::set<Computer&> m_Cplayer;//{ c1,c2 };
	unsigned m_numOfFood, m_numOfBombs;
};
//=================================================================
