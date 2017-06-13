#include "Controller.h"

void send(Sender& sender) {
	sender.send();
}
//=========================================================================

void receive(Receive& rec) {
	rec.run();
}
//=======================================================================
void Controller::run() {

	Network m_network;
	Receive rec{ m_network };
	Sender sender{ m_network };
	Board board{ m_network, rec , sender /*,c1*/ };

	std::thread a([&rec] {receive(rec); });
	std::thread b([&sender] {send(sender); });

	board.run();

}

//=========================================================================
int main()
{
	std::cout << sf::IpAddress::getLocalAddress().toString() << '\n';
	srand(unsigned(time(NULL)));
	Controller controller;
	controller.run();
	
	return 0;
}