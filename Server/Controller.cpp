#include "Controller.h"


//=========================================================================
//
//void comp() {
//	c1.run();
//}
//=========================================================================

void send(Sender& sender) {
	sender.send();
}
//=========================================================================

void receive(Receive& rec) {
	rec.run();
}
//=======================================================================
void Controller::run() {

	//auto c = std::thread(comp);
	Network m_network;
	Receive rec{ m_network };
	Sender sender{ m_network };
	//Computer c1;
	Board board{ m_network, rec , sender /*,c1*/ };

	std::thread a([&rec] {receive(rec); });
	std::thread b([&sender] {send(sender); });
	//	receive();
	//	send();
	board.run();

}

//=========================================================================
int main()
{
	srand(unsigned(time(NULL)));
	Controller controller;
	controller.run();
	
	return 0;
}