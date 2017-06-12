#include "Sender.h"

//==============================================================================================================
sf::Packet& operator << (sf::Packet& packet, const sf::Vector2f& v)
{
	return packet << v.x << v.y;
}
//==================================================================================================================
sf::Packet& operator << (sf::Packet& packet, const sendPack& p)
{
	return packet << p._id << p._ver;
}
//==================================================================================================================
sf::Packet& operator << (sf::Packet& packet, Sender& que)
{
	while (!que.empty())
		packet << que.front();

	return packet;
}
//==================================================================================================================
Sender::Sender(Network& net) :m_net(net)
{

}


//=====================================================================================
void Sender::send() {
	while (true) {
		{
			std::unique_lock<std::mutex> lock(m_mut);
			m_cv.wait(lock);//wait for the Board 
		}
		m_packet.clear();

		m_packet << (*this);

		// The listener socket is not ready, test all other sockets (the clients)
		std::unique_lock<std::mutex> lock(m_net.m_mut);
		for (auto it = m_net.clients.begin(); it != m_net.clients.end(); ++it)
			(*it)->send(m_packet);
	}
}
//==============================================================================================================
sendPack Sender::front() {
	std::lock_guard<std::mutex> lock(m_mut);
	auto rec = queue::front();
	queue::pop();
	return rec;
}
//=========================================================================================================
void Sender::push(const sendPack& rec) {
	std::lock_guard<std::mutex> lock(m_mut);
	queue::push(rec);
}