#pragma once

#include <map>
#include <list>
#include <memory>
#include <iostream>

#include "DataStructures.h"
#include "Network.h"

#define PORT 5555


//=================================

class Receive : private std::queue<recPack>
{
	typedef std::queue<recPack> queue;
public:
	Receive(Network&);
	~Receive();

	recPack front();
	void push(const recPack& rec);
	bool empty()const { return queue::empty(); }

	void run();
	void newClient();
	void receiveData();

	void wait() { std::unique_lock<std::mutex> lock(mut); m_cv.wait(lock); }
	sf::String getName() { sf::String n; m_packet >> n; return n; }
	void notify_one() { m_cv.notify_one(); }
private:

	Network& m_net;
	sf::Packet m_packet;
	std::mutex mut;
	sf::SocketSelector selector; // Create a selector
	sf::TcpListener listener;// Create a socket to listen to new connections
	std::condition_variable m_cv;//order the Board to wait
};
//==============================================================================================================


