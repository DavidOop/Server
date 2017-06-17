#include "Receive.h"
#include <Windows.h>
//==============================================================================================================
sf::Packet& operator >> (sf::Packet& packet, std::set<sf::Uint32>& vec)
{
	while (!packet.endOfPacket()) {
		sf::Uint32 temp;
		packet >> temp;
		vec.emplace(temp);
	}
	return packet;
}
//==============================================================================================================
sf::Packet& operator >> (sf::Packet& packet, sf::Vector2f& v)
{
	return packet >> v.x >> v.y;
}
//==============================================================================================================
sf::Packet& operator >> (sf::Packet& packet, recPack& p)
{
	return packet >> p._id >> p._rad >> p._ver >> p._vec;
}
//==============================================================================================================
Receive::Receive(Network& net) :m_net(net)
{
	listener.listen(PORT);
	selector.add(listener); // Add the listener to the selector
}
//===============================================================================
Receive::~Receive()
{

}
//==============================================================
void Receive::run() {

	// Endless loop that waits for new connections
	while (true) {
		// Make the selector wait for data on any socket
		// The listener is ready: there is a pending connection
	//	std::cout << "wait\n";
		if (selector.wait())
			(selector.isReady(listener)) ? newClient() : receiveData();
	}


}
//===============================================================================
void Receive::newClient() {

	auto client = new sf::TcpSocket;
	if (listener.accept(*client) == sf::Socket::Done)
	{
		std::unique_lock<std::mutex> lock(m_net.m_mut);
		m_net.clients.push_back(client);
		client->receive(m_packet);//receive image type from user
		sf::Uint32 image;
		m_packet >> image;
		push(recPack{ image });//update board there is a new client
		selector.add(*client);
		lock.unlock();
		m_cv.notify_one();
		{
			std::unique_lock<std::mutex> lock1(m_net.m_new);
			m_net.m_cv.wait(lock1);
		}
	}
	else delete client;
	int a = 0;
	int b = 1;

}
//================================================================================
void Receive::receiveData() {
	// The listener socket is not ready, test all other sockets (the clients)
	static int c = 0;
	for (auto it = m_net.clients.begin(); it != m_net.clients.end();)
	{

		sf::TcpSocket& client = **it;
		if (selector.isReady(client))
		{
			m_packet.clear();
			auto status = client.receive(m_packet);
			if (status == sf::TcpSocket::Disconnected) {
				selector.remove(client);
				std::unique_lock<std::mutex> lock(m_net.m_mut);
				it = m_net.clients.erase(it);
			}
			// The client has sent some data, we can receive it
			else if (status == sf::Socket::Done)
			{
				recPack pack;
				if (m_packet >> pack) {
					push(pack);
					static int c = 0;
				//	std::cout << "recieve: " << c << '\n';
					c++;
				}
			}
		}
		if(it!= m_net.clients.end())
		++it;
	}
	m_cv.notify_one();

}
//==============================================================================================================
recPack Receive::front() {
	std::lock_guard<std::mutex> lock(mut);
	auto rec = queue::front();
	queue::pop();
	return rec;
}
//=========================================================================================================
void Receive::push(const recPack& rec) {
	std::lock_guard<std::mutex> lock(mut);
	queue::push(rec);
}