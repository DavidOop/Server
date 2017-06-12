#pragma once
#ifdef _DEBUG
#pragma comment(lib, "sfml-main-d.lib")
#pragma comment(lib, "sfml-system-d.lib")
#elif defined(NDEBUG)
#pragma comment(lib, "sfml-main.lib")
#pragma comment(lib, "sfml-system.lib")
#pragma comment(lib, "sfml-network.lib")
#else
#error "Unrecognized configuration!"
#endif
#include <SFML\Network.hpp>
#include <queue>
#include <mutex>
#include <list>
#include <condition_variable>

#include <set>
//=================================================================================================================

struct Network {
	std::vector<sf::TcpSocket*> clients;  // Create a list to store the future clients
	~Network() { auto size = clients.size(); for (size_t i = 0; i < size; ++i)delete  clients[i]; }
	std::mutex m_mut;
	std::mutex m_new;
	std::condition_variable m_cv;//order the receiver to wait
};
//=================================================================================================================
//==============================================================================================================
struct recPack;
struct sendPack;
class Sender;

sf::Packet& operator >> (sf::Packet&, std::set<sf::Uint32>&);
sf::Packet& operator >> (sf::Packet&, sf::Vector2f&);
sf::Packet& operator >> (sf::Packet&, recPack&);

sf::Packet& operator << (sf::Packet&, const sf::Vector2f&);
sf::Packet& operator << (sf::Packet&, const sendPack&);
sf::Packet& operator << (sf::Packet&, Sender&);