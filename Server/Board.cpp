#include "Board.h"
#include "TimeClass.h"
#include <ctime>

//==================================================================================================================
sf::Packet& operator << (sf::Packet& packet, const std::unique_ptr<Circle>& c)
{

	packet << c->getId() << c->getVertex().x << c->getVertex().y;
	if (auto it = dynamic_cast<Player*>(c.get())) //if its a player 
		packet << it->getRadius() << it->getImage();
	return packet;
}
//==================================================================================================================
sf::Packet& operator << (sf::Packet& packet, const std::pair<const sf::Uint32, std::unique_ptr<Circle>>& pair)
{
	return packet << pair.second;
}

//===========================================================================================

Board::Board(Network& net, Receive& rec, Sender& send/*, Computer& c1*/)
	:m_network(net), m_receive(rec), m_sender(send),/* m_Cplayer(c1),*/ m_numOfFood(0), m_numOfBombs(0)
{

	add(FOOD, FOOD_UPPER, FOOD_LOWER, FOOD_RADIUS, FOOD, m_numOfFood);
	add(BOMBS, BOMBS_UPPER, BOMBS_LOWER, BOMB_RADIUS, BOMBS, m_numOfBombs);
	//addPlayer()
}

//=======================================================================================================================================
Board::~Board()
{
}

//=====================================================================================
void Board::run() {

	std::queue<recPack> temp;

	while (true) {
		wait(); // wait for data from recieve
		addFoodAndBombs();
		receiveLoop(temp);
		precessLoop(temp);
		notify_one();
	}

}
//========================================================================================================
sf::Uint32 Board::findId(const sf::Uint32 l, const sf::Uint32 u) {
	sf::Uint32 id; //make new id
	do {
		id = sf::Uint32((rand() % (u - l)) + l);
	} while (find(id) != end());
	return id;
}
//=========================================================================================================
//add food to baord
void Board::add(co_Uint MAX, co_Uint UPPER, co_Uint LOWER, const float RADIUS, co_Uint limit, sf::Uint32& num) {
	if (num >= MAX)
		return;

	for (sf::Uint32 i = num; i < MAX; ++i) {
		auto id = findId(LOWER, UPPER);
		sf::Vector2f ver{ float(rand() % int(BOARD_SIZE.x)) ,float(rand() % int(BOARD_SIZE.y)) };
		emplace(id, Circle{ id, ver, RADIUS });
		++num;
		m_sender.push(sendPack{ id, find(id)->second->getVertex() });
	}
	notify_one();//notify sender
}
//=============================================================================================================
sf::Vector2f Board::addVertex(float radius) {
	sf::Vector2f ver;
	do {
		ver.x = float(rand() % int(BOARD_SIZE.x));
		ver.y = float(rand() % int(BOARD_SIZE.y));
	} while (!collide(ver, radius));
	return ver;
}
//=============================================================================================================
bool Board::collide(sf::Vector2f ver, float r) {
	for (auto it = begin(); it != end(); ++it)
		if (isIntersect(ver, (*it).second->getVertex(), r, (*it).second->getRadius())) return true;
	return false;
}
//=========================================================================================================================================
void Board::addFoodAndBombs() {
	static float stopwatch = 0;
	stopwatch += TimeClass::instance().RestartClock();
	if (stopwatch >= 5.f) {
		stopwatch = 0;
		add(FOOD, FOOD_UPPER, FOOD_LOWER, FOOD_RADIUS, FOOD, m_numOfFood);
		add(BOMBS, BOMBS_UPPER, BOMBS_LOWER, BOMB_RADIUS, BOMBS, m_numOfBombs);
	}
}
//=========================================================================================================================================

void Board::receiveLoop(std::queue<recPack>& temp) {
	while (!m_receive.empty())
		temp.push(recPack{ m_receive.front() });
}
//=========================================================================================================================================

void Board::precessLoop(std::queue<recPack>& temp) {
	while (!temp.empty()) {
		auto player = temp.front()._id;
		if (player < MAX_IMAGE)
			addClient(*this, player);//a = std::thread(addClient, *this);			std::Ref(*this)
		else {
			if (find(player) == end())
				continue;
			m_sender.push(sendPack{ player, temp.front()._ver });
			find(player)->second->setVertex(temp.front()._ver);
			find(player)->second->setRadius(temp.front()._rad);
			for (auto it = temp.front()._vec.begin(); it != temp.front()._vec.end(); ++it) {
				if (find(*it) != end()) {
					(find(*it)->first >= BOMBS_LOWER) ? --m_numOfBombs : --m_numOfFood; //update number of food or bombs
					erase(find(*it));
				}
			}
		}
		temp.pop();
	}
}


//==========================================================================================
/*                       add new player                                                */

//==========================================================================================

void addClient(Board& board, sf::Uint32 image) {
	std::unique_lock<std::mutex> lock(board.net().m_mut);

	sf::Packet packet;
	auto id = board.findId(PLAYER_LOWER, PLAYER_UPPER); //make new id
	auto ver = board.addVertex(PLAYER_RADIUS);
	//auto lock = board.unique_lock();
	/*send the board*/
	for (auto it = board.begin(); it != board.end(); ++it) {
		packet << it->second;
	}

	//add player to board
	board.emplace(id, Player{ id, image, ver });

	packet << id << ver << PLAYER_RADIUS << image;
	if (board.net().clients[board.net().clients.size() - 1]->send(packet) != sf::TcpSocket::Done)
		std::cout << "didnt send map\n";

	//update all players the new player
	packet.clear();
	packet << id << board.find(id)->second->getVertex() << dynamic_cast<Player*>(board.find(id)->second.get())->getImage();
	for (auto it = board.net().clients.begin(); it != board.net().clients.end(); ++it)
		(*it)->send(packet);

	lock.unlock();
	//notify receiver and sender
	board.net().m_cv.notify_all();

}
//=================================================================================
//this function adds a computer player
void Board::addComputerPlayers() {
	LPCWSTR computer = L"C:\\Users\\davidwer\\Desktop\\project\\server_b\\Debug\\client.exe";
	LPTSTR run_cmd = L"client.exe";
	PROCESS_INFORMATION pif;
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	if (!CreateProcess(
		computer,		// path to executtable file
		run_cmd,        // run Command 
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pif))          // Pointer to PROCESS_INFORMATION structure
		std::cout << "cant find computer player\n";
}
