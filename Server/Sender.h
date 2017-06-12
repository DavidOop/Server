#pragma once
#include "Network.h"
#include "DataStructures.h"

//===========================================================================


//===========================================================================


class Sender : private std::queue<sendPack>
{
	typedef std::queue<sendPack> queue;
public:
	Sender(Network&);
	~Sender() {}
	void send();

	sendPack front();
	void push(const sendPack& rec);
	bool empty()const { return queue::empty(); }
	void notify_one() { m_cv.notify_one(); }//notify the sender there is data to send
	
private:
	Network& m_net;
	sf::Packet m_packet;
	std::mutex m_mut;
	std::condition_variable m_cv;
};

//===========================================================================
