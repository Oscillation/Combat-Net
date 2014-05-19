#include "GameManager.h"

GameManager::Branch::Branch(){}

GameManager::Branch::Branch(const unsigned int & p_x, const unsigned int & p_y) 
	: m_x(p_x),
	m_y(p_y){}

GameManager::Branch::~Branch(){}

void GameManager::Branch::update(Client & p_client){
	std::vector<Client*>::iterator it = std::find(m_clientList.begin(), m_clientList.end(), &p_client);
	if (sf::Rect<float>(p_client.getPosition().x, p_client.getPosition().y, 40, 40).intersects(sf::Rect<float>(m_x*64, m_y*64, 64*m_size, 64*m_size)))
	{
		if (it == m_clientList.end())
		{
			m_clientList.push_back(&p_client);
		}
	}else
	{
		if (it != m_clientList.end())
		{
			m_clientList.erase(it);
		}
	}
}

void GameManager::Branch::update(Projectile & p_projectile){
	std::vector<Projectile*>::iterator it = std::find(m_projectiles.begin(), m_projectiles.end(), &p_projectile);
	if (sf::Rect<float>(p_projectile.getPosition().x, p_projectile.getPosition().y, 5, 5).intersects(sf::Rect<float>(m_x*64, m_y*64, 64*m_size, 64*m_size)))
	{
		if (it == m_projectiles.end())
		{
			m_projectiles.push_back(&p_projectile);
		}
	}else
	{
		if (it != m_projectiles.end())
		{
			m_projectiles.erase(it);
		}
	}
}

GameManager::GameManager(){

}

GameManager::GameManager(std::map<std::string, Client>* ptr_clientList, std::vector<Projectile>* ptr_projectiles, const unsigned int & p_width, const unsigned int & p_height) : m_mapSize(p_width/m_size, p_height/m_size){
	for (unsigned int y = 0, x = 0; y < p_height; y+=m_size)
	{
		for (x = 0; x < p_width; x+=m_size)
		{
			m_branches.push_back(Branch(x, y));
		}
	}
}

GameManager::~GameManager(){

}

void GameManager::update(Client & p_client){
	sf::Vector2<int> clientPoints[4] = {
		sf::Vector2<int>(p_client.getPosition().x/(64*m_size), p_client.getPosition().y/(64*m_size)),
		sf::Vector2<int>((p_client.getPosition().x + 40)/(64*m_size), p_client.getPosition().y/(64*m_size)),
		sf::Vector2<int>(p_client.getPosition().x/(64*m_size), (p_client.getPosition().y + 40)/(64*m_size)),
		sf::Vector2<int>((p_client.getPosition().x + 40)/(64*m_size), (p_client.getPosition().y + 40)/(64*m_size))
	};

	for (int i = 0; i < 4; i++)
	{
		if ((clientPoints[i].x+((clientPoints[i].y)*m_mapSize.x)) >= 0 && (clientPoints[i].x+((clientPoints[i].y)*m_mapSize.x)) < m_branches.size())
		{
			m_branches[clientPoints[i].x+((clientPoints[i].y)*m_mapSize.x)].update(p_client);
		}
	}
}

void GameManager::update(Projectile & p_projectile){
	sf::Vector2<int> projectilePoints[4] = {
		sf::Vector2<int>(p_projectile.getPosition().x/(64*m_size), p_projectile.getPosition().y/(64*m_size)),
		sf::Vector2<int>((p_projectile.getPosition().x + 40)/(64*m_size), p_projectile.getPosition().y/(64*m_size)),
		sf::Vector2<int>(p_projectile.getPosition().x/(64*m_size), (p_projectile.getPosition().y + 40)/(64*m_size)),
		sf::Vector2<int>((p_projectile.getPosition().x + 40)/(64*m_size), (p_projectile.getPosition().y + 40)/(64*m_size))
	};

	for (int i = 0; i < 4; i++)
	{
		if ((projectilePoints[i].x+((projectilePoints[i].y)*m_mapSize.x)) >= 0 && (projectilePoints[i].x+((projectilePoints[i].y)*m_mapSize.x)) < m_branches.size())
		{
			m_branches[projectilePoints[i].x+((projectilePoints[i].y)*m_mapSize.x)].update(p_projectile);
		}
	}
}

bool GameManager::intersect(Client & p_client, Projectile & p_projectile) const{
	if (shareBranch(p_client, p_projectile))
	{

		if (math::LineIntersectsCircle(sf::Vector2<float>(p_projectile.getPosition().x + 2.5f, p_projectile.getPosition().y + 2.5f),
			sf::Vector2<float>(p_projectile.getPosition().x + 2.5f + p_projectile.getVelocity().x, p_projectile.getPosition().y + 2.5f + p_projectile.getVelocity().y),
			p_client.getPosition(), 20.f))
		{
			return true;
		}else
		{
			return false;
		}
	}else
	{
		return false;
	}
}

bool GameManager::intersect(Client & p_clientA, Client & p_clientB) const{
	if (shareBranch(p_clientA, p_clientB))
	{
		if (math::distance(p_clientA.getPosition(), p_clientB.getPosition()) <= 20)
		{
			return true;
		}else
		{
			return false;
		}
	}else
	{
		return false;
	}
}

bool GameManager::intersect(Projectile & p_projectileA, Projectile & p_projectileB) const{
	if (shareBranch(p_projectileA, p_projectileB))
	{
		if (sf::Rect<float>(p_projectileA.getPosition().x, p_projectileA.getPosition().y, 5, 5).intersects(sf::Rect<float>(p_projectileB.getPosition().x, p_projectileB.getPosition().y, 5, 5)))
		{
			return true;
		}else
		{
			return false;
		}
	}else
	{
		return false;
	}
}

bool GameManager::shareBranch(Client & p_client, Projectile & p_projectile) const{
	sf::Vector2<int> clientPoints[4] = {
		sf::Vector2<int>(p_client.getPosition().x/(64*m_size), p_client.getPosition().y/(64*m_size)),
		sf::Vector2<int>((p_client.getPosition().x + 40)/(64*m_size), p_client.getPosition().y/(64*m_size)),
		sf::Vector2<int>(p_client.getPosition().x/(64*m_size), (p_client.getPosition().y + 40)/(64*m_size)),
		sf::Vector2<int>((p_client.getPosition().x + 40)/(64*m_size), (p_client.getPosition().y + 40)/(64*m_size))
	};

	sf::Vector2<int> projectilePoints[4] = {
		sf::Vector2<int>(p_projectile.getPosition().x/(64*m_size), p_projectile.getPosition().y/(64*m_size)),
		sf::Vector2<int>((p_projectile.getPosition().x + 40)/(64*m_size), p_projectile.getPosition().y/(64*m_size)),
		sf::Vector2<int>(p_projectile.getPosition().x/(64*m_size), (p_projectile.getPosition().y + 40)/(64*m_size)),
		sf::Vector2<int>((p_projectile.getPosition().x + 40)/(64*m_size), (p_projectile.getPosition().y + 40)/(64*m_size))
	};

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (projectilePoints[i].x+((projectilePoints[i].y)*m_mapSize.x) == clientPoints[j].x+((clientPoints[j].y)*m_mapSize.x))
			{
				return true;
			}
		}
	}
	return false;
}

bool GameManager::shareBranch(Client & p_clientA, Client & p_clientB) const{
	sf::Vector2<int> clientAPoints[4] = {
		sf::Vector2<int>(p_clientA.getPosition().x/(64*m_size), p_clientA.getPosition().y/(64*m_size)),
		sf::Vector2<int>((p_clientA.getPosition().x + 40)/(64*m_size), p_clientA.getPosition().y/(64*m_size)),
		sf::Vector2<int>(p_clientA.getPosition().x/(64*m_size), (p_clientA.getPosition().y + 40)/(64*m_size)),
		sf::Vector2<int>((p_clientA.getPosition().x + 40)/(64*m_size), (p_clientA.getPosition().y + 40)/(64*m_size))
	};

	sf::Vector2<int> clientBPoints[4] = {
		sf::Vector2<int>(p_clientB.getPosition().x/(64*m_size), p_clientB.getPosition().y/(64*m_size)),
		sf::Vector2<int>((p_clientB.getPosition().x + 40)/(64*m_size), p_clientB.getPosition().y/(64*m_size)),
		sf::Vector2<int>(p_clientB.getPosition().x/(64*m_size), (p_clientB.getPosition().y + 40)/(64*m_size)),
		sf::Vector2<int>((p_clientB.getPosition().x + 40)/(64*m_size), (p_clientB.getPosition().y + 40)/(64*m_size))
	};
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (clientAPoints[i].x+((clientAPoints[i].y)*m_mapSize.x) == clientBPoints[j].x+((clientBPoints[j].y)*m_mapSize.x))
			{
				return true;
			}
		}
	}
	return false;
}

bool GameManager::shareBranch(Projectile & p_projectileA, Projectile & p_projectileB) const{
	sf::Vector2<int> projectileAPoints[4] = {
		sf::Vector2<int>(p_projectileA.getPosition().x/(64*m_size), p_projectileA.getPosition().y/(64*m_size)),
		sf::Vector2<int>((p_projectileA.getPosition().x + 40)/(64*m_size), p_projectileA.getPosition().y/(64*m_size)),
		sf::Vector2<int>(p_projectileA.getPosition().x/(64*m_size), (p_projectileA.getPosition().y + 40)/(64*m_size)),
		sf::Vector2<int>((p_projectileA.getPosition().x + 40)/(64*m_size), (p_projectileA.getPosition().y + 40)/(64*m_size))
	};

	sf::Vector2<int> projectileBPoints[4] = {
		sf::Vector2<int>(p_projectileB.getPosition().x/(64*m_size), p_projectileB.getPosition().y/(64*m_size)),
		sf::Vector2<int>((p_projectileB.getPosition().x + 40)/(64*m_size), p_projectileB.getPosition().y/(64*m_size)),
		sf::Vector2<int>(p_projectileB.getPosition().x/(64*m_size), (p_projectileB.getPosition().y + 40)/(64*m_size)),
		sf::Vector2<int>((p_projectileB.getPosition().x + 40)/(64*m_size), (p_projectileB.getPosition().y + 40)/(64*m_size))
	};
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (projectileAPoints[i].x+((projectileAPoints[i].y)*m_mapSize.x) == projectileBPoints[j].x+((projectileBPoints[j].y)*m_mapSize.x))
			{
				return true;
			}
		}
	}
	return false;
}

std::vector<Client> GameManager::getClients(Projectile & p_projectile) const{
	sf::Vector2<int> projectilePoints[4] = {
		sf::Vector2<int>(p_projectile.getPosition().x/(64*m_size), p_projectile.getPosition().y/(64*m_size)),
		sf::Vector2<int>((p_projectile.getPosition().x + 40)/(64*m_size), p_projectile.getPosition().y/(64*m_size)),
		sf::Vector2<int>(p_projectile.getPosition().x/(64*m_size), (p_projectile.getPosition().y + 40)/(64*m_size)),
		sf::Vector2<int>((p_projectile.getPosition().x + 40)/(64*m_size), (p_projectile.getPosition().y + 40)/(64*m_size))
	};

	std::vector<Client> clients;

	for (int i = 0; i < 4; i++)
	{
		for (auto it = m_branches[projectilePoints[i].x+((projectilePoints[i].y)*m_mapSize.x)].m_clientList.begin(); it != m_branches[projectilePoints[i].x+((projectilePoints[i].y)*m_mapSize.x)].m_clientList.end(); ++it){
			if (!exists(*(*it), clients))
			{
				clients.push_back(*(*it));
			}
		}
	}

	return clients;
}

std::vector<Client> GameManager::getClients(Client & p_client) const{
	sf::Vector2<int> clientPoints[4] = {
		sf::Vector2<int>(p_client.getPosition().x/(64*m_size), p_client.getPosition().y/(64*m_size)),
		sf::Vector2<int>((p_client.getPosition().x + 40)/(64*m_size), p_client.getPosition().y/(64*m_size)),
		sf::Vector2<int>(p_client.getPosition().x/(64*m_size), (p_client.getPosition().y + 40)/(64*m_size)),
		sf::Vector2<int>((p_client.getPosition().x + 40)/(64*m_size), (p_client.getPosition().y + 40)/(64*m_size))
	};

	std::vector<Client> clients;

	for (int i = 0; i < 4; i++)
	{
		for (auto it = m_branches[clientPoints[i].x+((clientPoints[i].y)*m_mapSize.x)].m_clientList.begin(); it != m_branches[clientPoints[i].x+((clientPoints[i].y)*m_mapSize.x)].m_clientList.end(); ++it){
			if (!exists(*(*it), clients))
			{
				clients.push_back(*(*it));
			}
		}
	}

	return clients;
}

std::vector<Projectile> GameManager::getProjectiles(Projectile & p_projectile) const{
	sf::Vector2<int> projectilePoints[4] = {
		sf::Vector2<int>(p_projectile.getPosition().x/(64*m_size), p_projectile.getPosition().y/(64*m_size)),
		sf::Vector2<int>((p_projectile.getPosition().x + 40)/(64*m_size), p_projectile.getPosition().y/(64*m_size)),
		sf::Vector2<int>(p_projectile.getPosition().x/(64*m_size), (p_projectile.getPosition().y + 40)/(64*m_size)),
		sf::Vector2<int>((p_projectile.getPosition().x + 40)/(64*m_size), (p_projectile.getPosition().y + 40)/(64*m_size))
	};

	std::vector<Projectile> projectiles;

	for (int i = 0; i < 4; i++)
	{
		for (auto it = m_branches[projectilePoints[i].x+((projectilePoints[i].y)*m_mapSize.x)].m_projectiles.begin(); it != m_branches[projectilePoints[i].x+((projectilePoints[i].y)*m_mapSize.x)].m_projectiles.end(); ++it){
			if (!exists(*(*it), projectiles))
			{
				projectiles.push_back(*(*it));
			}
		}
	}

	return projectiles;
}

std::vector<Projectile> GameManager::getProjectiles(Client & p_client) const{
	sf::Vector2<int> clientPoints[4] = {
		sf::Vector2<int>(p_client.getPosition().x/(64*m_size), p_client.getPosition().y/(64*m_size)),
		sf::Vector2<int>((p_client.getPosition().x + 40)/(64*m_size), p_client.getPosition().y/(64*m_size)),
		sf::Vector2<int>(p_client.getPosition().x/(64*m_size), (p_client.getPosition().y + 40)/(64*m_size)),
		sf::Vector2<int>((p_client.getPosition().x + 40)/(64*m_size), (p_client.getPosition().y + 40)/(64*m_size))
	};

	std::vector<Projectile> projectiles;

	for (int i = 0; i < 4; i++)
	{
		for (auto it = m_branches[clientPoints[i].x+((clientPoints[i].y)*m_mapSize.x)].m_projectiles.begin(); it != m_branches[clientPoints[i].x+((clientPoints[i].y)*m_mapSize.x)].m_projectiles.end(); ++it){
			if (!exists(*(*it), projectiles))
			{
				projectiles.push_back(*(*it));
			}
		}
	}

	return projectiles;
}

bool GameManager::exists(Client & p_client, std::vector<Client> & p_clients) const{
	if (p_clients.empty())
	{
		return false;
	}
	for (auto it = p_clients.begin(); it != p_clients.end(); ++it){
		if (*it == p_client)
		{
			return true;
		}
	}
	return false;
}

bool GameManager::exists(Projectile & p_projectile, std::vector<Projectile> & p_projectiles) const{
	if (p_projectiles.empty())
	{
		return false;
	}
	for (auto it = p_projectiles.begin(); it != p_projectiles.end(); ++it){
		if (*it == p_projectile)
		{
			return true;
		}
	}
	return false;
}

bool GameManager::exists(Client & p_client, std::vector<Client*> & p_clients) const{
	if (p_clients.empty())
	{
		return false;
	}
	for (auto it = p_clients.begin(); it != p_clients.end(); ++it){
		if (*(*it) == p_client)
		{
			return true;
		}
	}
	return false;
}

bool GameManager::exists(Projectile & p_projectile, std::vector<Projectile*> & p_projectiles) const{
	if (p_projectiles.empty())
	{
		return false;
	}
	for (auto it = p_projectiles.begin(); it != p_projectiles.end(); ++it){
		if (*(*it) == p_projectile)
		{
			return true;
		}
	}
	return false;
}

void GameManager::erase(Client & p_client){
	sf::Vector2<int> clientPoints[4] = {sf::Vector2<int>(p_client.getPosition().x/(64*m_size), p_client.getPosition().y/(64*m_size)), sf::Vector2<int>((p_client.getPosition().x + 40)/(64*m_size), p_client.getPosition().y/(64*m_size)), sf::Vector2<int>(p_client.getPosition().x/(64*m_size), (p_client.getPosition().y + 40)/(64*m_size)), sf::Vector2<int>((p_client.getPosition().x + 40)/(64*m_size), (p_client.getPosition().y + 40)/(64*m_size))};

	for (int i = 0; i < 4; i++)
	{
		if (!m_branches[clientPoints[i].x+((clientPoints[i].y)*m_mapSize.x)].m_clientList.empty())
		{
			for (auto it = m_branches[clientPoints[i].x+((clientPoints[i].y)*m_mapSize.x)].m_clientList.begin(); it != m_branches[clientPoints[i].x+((clientPoints[i].y)*m_mapSize.x)].m_clientList.end(); ++it){
				if (m_branches[clientPoints[i].x+((clientPoints[i].y)*m_mapSize.x)].m_clientList.empty())
				{
					break;
				}else
				{
					if (*it != nullptr)
					{
						if (*(*it) == p_client)
						{
							m_branches[clientPoints[i].x+((clientPoints[i].y)*m_mapSize.x)].m_clientList.erase(it);
							break;
						}
					}
				}
			}
		}
	}
}

void GameManager::erase(Projectile & p_projectile){
	sf::Vector2<int> projectilePoints[4] = {sf::Vector2<int>(p_projectile.getPosition().x/(64*m_size), p_projectile.getPosition().y/(64*m_size)), sf::Vector2<int>((p_projectile.getPosition().x + 40)/(64*m_size), p_projectile.getPosition().y/(64*m_size)), sf::Vector2<int>(p_projectile.getPosition().x/(64*m_size), (p_projectile.getPosition().y + 40)/(64*m_size)), sf::Vector2<int>((p_projectile.getPosition().x + 40)/(64*m_size), (p_projectile.getPosition().y + 40)/(64*m_size))};

	for (int i = 0; i < 4; i++)
	{
		if (projectilePoints[i].x+((projectilePoints[i].y)*m_mapSize.x) >= 0 && projectilePoints[i].x+((projectilePoints[i].y)*m_mapSize.x) < m_branches.size())
		{
			if (!m_branches[projectilePoints[i].x+((projectilePoints[i].y)*m_mapSize.x)].m_projectiles.empty())
			{
				for (auto it = m_branches[projectilePoints[i].x+((projectilePoints[i].y)*m_mapSize.x)].m_projectiles.begin();
					it != m_branches[projectilePoints[i].x+((projectilePoints[i].y)*m_mapSize.x)].m_projectiles.end(); ++it){
						if (m_branches[projectilePoints[i].x+((projectilePoints[i].y)*m_mapSize.x)].m_projectiles.empty())
						{
							break;
						}else
						{
							if (*it != nullptr)
							{
								if (*(*it) == p_projectile)
								{
									m_branches[projectilePoints[i].x+((projectilePoints[i].y)*m_mapSize.x)].m_projectiles.erase(it);
									break;
								}
							}
						}
				}
			}
		}
	}
}

void GameManager::clean(){
	for (auto it = m_branches.begin(); it != m_branches.end(); ++it){
		it->m_clientList.clear();
		it->m_projectiles.clear();
	}
}

sf::Vector2<int> GameManager::selectSpawn(const std::vector<sf::Vector2<int>> p_spawnPositions){
	std::vector<int> spawns;
	spawns.resize(p_spawnPositions.size(), 0);

	int lowIndex = 0;

	for (int i = 0; i < p_spawnPositions.size(); i++){
		for (int x = (p_spawnPositions[i].x - 10) >= 0 ? p_spawnPositions[i].x - 10:0, y = (p_spawnPositions[i].y - 10) >= 0 ? p_spawnPositions[i].y - 10:0; x < p_spawnPositions[i].x + 10; x++)
		{
			for (y = (p_spawnPositions[i].y - 10) >= 0 ? p_spawnPositions[i].y - 10:0; y < p_spawnPositions[i].y + 10; y++)
			{
				if (x+(y*m_mapSize.x) >= 0 && x+(y*m_mapSize.x) < m_branches.size())
				{
					spawns[i] += m_branches[x+(y*m_mapSize.x)].m_clientList.size();
				}
			}
		}
		/*spawns[i] = m_branches[(p_spawnPositions[i].x/m_size)+((p_spawnPositions[i].y/m_size)*m_mapSize.x)].m_clientList.size() +
		(((p_spawnPositions[i].x/m_size) - 1 + (((p_spawnPositions[i].y/m_size))*m_mapSize.x)) < m_branches.size() ? (m_branches[(p_spawnPositions[i].x/m_size) - 1 + (((p_spawnPositions[i].y/m_size))*m_mapSize.x)].m_clientList.size()):0) +
		(((p_spawnPositions[i].x/m_size) + 1 + (((p_spawnPositions[i].y/m_size))*m_mapSize.x)) < m_branches.size() ? (m_branches[(p_spawnPositions[i].x/m_size) + 1 + (((p_spawnPositions[i].y/m_size))*m_mapSize.x)].m_clientList.size()):0) +
		(((p_spawnPositions[i].x/m_size) + (((p_spawnPositions[i].y/m_size) - 1)*m_mapSize.x)) < m_branches.size() ? (m_branches[(p_spawnPositions[i].x/m_size) + (((p_spawnPositions[i].y/m_size) - 1)*m_mapSize.x)].m_clientList.size()):0) +
		(((p_spawnPositions[i].x/m_size) + (((p_spawnPositions[i].y/m_size) + 1)*m_mapSize.x)) < m_branches.size() ? (m_branches[(p_spawnPositions[i].x/m_size) + (((p_spawnPositions[i].y/m_size) + 1)*m_mapSize.x)].m_clientList.size()):0) +
		(((p_spawnPositions[i].x/m_size) - 1 + (((p_spawnPositions[i].y/m_size) - 1)*m_mapSize.x)) < m_branches.size() ? (m_branches[(p_spawnPositions[i].x/m_size) - 1 + (((p_spawnPositions[i].y/m_size) - 1)*m_mapSize.x)].m_clientList.size()):0) +
		(((p_spawnPositions[i].x/m_size) + 1 + (((p_spawnPositions[i].y/m_size) - 1)*m_mapSize.x)) < m_branches.size() ? (m_branches[(p_spawnPositions[i].x/m_size) + 1 + (((p_spawnPositions[i].y/m_size) - 1)*m_mapSize.x)].m_clientList.size()):0) +
		(((p_spawnPositions[i].x/m_size) - 1 + (((p_spawnPositions[i].y/m_size) + 1)*m_mapSize.x)) < m_branches.size() ? (m_branches[(p_spawnPositions[i].x/m_size) - 1 + (((p_spawnPositions[i].y/m_size) + 1)*m_mapSize.x)].m_clientList.size()):0) +
		(((p_spawnPositions[i].x/m_size) + 1 + (((p_spawnPositions[i].y/m_size) + 1)*m_mapSize.x)) < m_branches.size() ? (m_branches[(p_spawnPositions[i].x/m_size) + 1 + (((p_spawnPositions[i].y/m_size) + 1)*m_mapSize.x)].m_clientList.size()):0);
		*/
		if (spawns[i] <= spawns[lowIndex])
		{
			if (spawns[i] == spawns[lowIndex])
			{
				if (math::random(1, 100) < 50)
				{
					lowIndex = i;
				}
			}else
			{
				lowIndex = i;
			}
		}
	}

	return p_spawnPositions[lowIndex];
}