#include "GameManager.h"

GameManager::Branch::Branch(){}

GameManager::Branch::Branch(const int & p_x, const int & p_y) 
	: m_x(p_x),
	m_y(p_y){}

GameManager::Branch::~Branch(){}

void GameManager::Branch::update(Client & p_client){
	std::vector<Client*>::iterator it = std::find(m_clientList.begin(), m_clientList.end(), &p_client);
	if (sf::Rect<float>(p_client.getPosition().x, p_client.getPosition().y, 40, 40).intersects(sf::Rect<float>(m_x, m_y, 128, 128)))
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
	if (sf::Rect<float>(p_projectile.getPosition().x, p_projectile.getPosition().y, 5, 5).intersects(sf::Rect<float>(m_x, m_y, 128, 128)))
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

GameManager::GameManager(std::map<std::string, Client>* ptr_clientList, std::vector<Projectile>* ptr_projectiles, const int & p_width, const int & p_height) : m_mapSize(p_width/2, p_height/2){
	for (unsigned int x = 0, y = 0; x < p_width*64; x+=128)
	{
		for (y = 0; y < p_height*64; y+=128)
		{
			m_branches.push_back(Branch(x, y));
		}
	}
}

GameManager::~GameManager(){

}

void GameManager::update(Client & p_client){
	sf::Vector2<int> clientPoints[4] = {sf::Vector2<int>(p_client.getPosition().x/128, p_client.getPosition().y/128), sf::Vector2<int>((p_client.getPosition().x + 40)/128, p_client.getPosition().y/128), sf::Vector2<int>(p_client.getPosition().x/128, (p_client.getPosition().y + 40)/128), sf::Vector2<int>((p_client.getPosition().x + 40)/128, (p_client.getPosition().y + 40)/128)};

	for (int i = 0; i < 4; i++)
	{
		m_branches[clientPoints[i].x+((clientPoints[i].y)*m_mapSize.x)].update(p_client);
	}
}

void GameManager::update(Projectile & p_projectile){
	sf::Vector2<int> projectilePoints[4] = {sf::Vector2<int>(p_projectile.getPosition().x/128, p_projectile.getPosition().y/128), sf::Vector2<int>((p_projectile.getPosition().x + 40)/128, p_projectile.getPosition().y/128), sf::Vector2<int>(p_projectile.getPosition().x/128, (p_projectile.getPosition().y + 40)/128), sf::Vector2<int>((p_projectile.getPosition().x + 40)/128, (p_projectile.getPosition().y + 40)/128)};

	for (int i = 0; i < 4; i++)
	{
		m_branches[projectilePoints[i].x+((projectilePoints[i].y)*m_mapSize.x)].update(p_projectile);
	}
}

bool GameManager::intersect(Client & p_client, Projectile & p_projectile) const{
	if (shareBranch(p_client, p_projectile))
	{
		if (math::circleIntersectsRect(sf::Vector2<float>(p_client.getPosition().x - 33, p_client.getPosition().y - 33), 17, sf::Rect<float>(p_projectile.getPosition().x, p_projectile.getPosition().y, 5, 5)))
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
	sf::Vector2<int> clientPoints[4] = {sf::Vector2<int>(p_client.getPosition().x, p_client.getPosition().y), sf::Vector2<int>(p_client.getPosition().x + 40, p_client.getPosition().y), sf::Vector2<int>(p_client.getPosition().x, p_client.getPosition().y + 40), sf::Vector2<int>(p_client.getPosition().x + 40, p_client.getPosition().y + 40)};
	sf::Vector2<int> projectilePoints[4] = {sf::Vector2<int>(p_projectile.getPosition().x, p_projectile.getPosition().y), sf::Vector2<int>(p_projectile.getPosition().x + 40, p_projectile.getPosition().y), sf::Vector2<int>(p_projectile.getPosition().x, p_projectile.getPosition().y + 40), sf::Vector2<int>(p_projectile.getPosition().x + 40, p_projectile.getPosition().y + 40)};
	for (int i = 4; i < 0; i--)
	{
		for (int j = 0; j < 4; j++)
		{
			if (sf::Vector2<int>((int)clientPoints[i].x/128, (int)clientPoints[i].y/128) == sf::Vector2<int>((int)projectilePoints[j].x/128, (int)projectilePoints[j].y/128))
			{
				return true;
			}
		}
	}
	return false;
}

bool GameManager::shareBranch(Client & p_clientA, Client & p_clientB) const{
	sf::Vector2<int> clientPoints[4] = {sf::Vector2<int>(p_clientA.getPosition().x, p_clientA.getPosition().y), sf::Vector2<int>(p_clientA.getPosition().x + 40, p_clientA.getPosition().y), sf::Vector2<int>(p_clientA.getPosition().x, p_clientA.getPosition().y + 40), sf::Vector2<int>(p_clientA.getPosition().x + 40, p_clientA.getPosition().y + 40)};
	sf::Vector2<int> projectilePoints[4] = {sf::Vector2<int>(p_clientB.getPosition().x, p_clientB.getPosition().y), sf::Vector2<int>(p_clientB.getPosition().x + 40, p_clientB.getPosition().y), sf::Vector2<int>(p_clientB.getPosition().x, p_clientB.getPosition().y + 40), sf::Vector2<int>(p_clientB.getPosition().x + 40, p_clientB.getPosition().y + 40)};
	for (int i = 4; i < 0; i--)
	{
		for (int j = 0; j < 4; j++)
		{
			if (sf::Vector2<int>((int)clientPoints[i].x/128, (int)clientPoints[i].y/128) == sf::Vector2<int>((int)projectilePoints[j].x/128, (int)projectilePoints[j].y/128))
			{
				return true;
			}
		}
	}
	return false;
}

bool GameManager::shareBranch(Projectile & p_projectileA, Projectile & p_projectileB) const{
	sf::Vector2<int> clientPoints[4] = {sf::Vector2<int>(p_projectileA.getPosition().x, p_projectileA.getPosition().y), sf::Vector2<int>(p_projectileA.getPosition().x + 40, p_projectileA.getPosition().y), sf::Vector2<int>(p_projectileA.getPosition().x, p_projectileA.getPosition().y + 40), sf::Vector2<int>(p_projectileA.getPosition().x + 40, p_projectileA.getPosition().y + 40)};
	sf::Vector2<int> projectilePoints[4] = {sf::Vector2<int>(p_projectileB.getPosition().x, p_projectileB.getPosition().y), sf::Vector2<int>(p_projectileB.getPosition().x + 40, p_projectileB.getPosition().y), sf::Vector2<int>(p_projectileB.getPosition().x, p_projectileB.getPosition().y + 40), sf::Vector2<int>(p_projectileB.getPosition().x + 40, p_projectileB.getPosition().y + 40)};
	for (int i = 4; i < 0; i--)
	{
		for (int j = 0; j < 4; j++)
		{
			if (sf::Vector2<int>((int)clientPoints[i].x/128, (int)clientPoints[i].y/128) == sf::Vector2<int>((int)projectilePoints[j].x/128, (int)projectilePoints[j].y/128))
			{
				return true;
			}
		}
	}
	return false;
}

std::vector<Client> GameManager::getClients(Projectile & p_projectile) const{
	sf::Vector2<int> projectilePoints[4] = {sf::Vector2<int>(p_projectile.getPosition().x/128, p_projectile.getPosition().y/128), sf::Vector2<int>((p_projectile.getPosition().x + 40)/128, p_projectile.getPosition().y/128), sf::Vector2<int>(p_projectile.getPosition().x/128, (p_projectile.getPosition().y + 40)/128), sf::Vector2<int>((p_projectile.getPosition().x + 40)/128, (p_projectile.getPosition().y + 40)/128)};
	std::vector<Client> clients;

	for (int i = 0; i < 4; i++)
	{
		for (auto it = m_branches[projectilePoints[i].x+((projectilePoints[i].y)*m_mapSize.x)].m_clientList.begin(); it != m_branches[projectilePoints[i].x+((projectilePoints[i].y)*m_mapSize.x)].m_clientList.end(); ++it){
			if (shareBranch(*(*it), p_projectile))
			{
				clients.push_back(*(*it));
			}
		}
	}

	return clients;
}

std::vector<Client> GameManager::getClients(Client & p_client) const{
	sf::Vector2<int> clientPoints[4] = {sf::Vector2<int>(p_client.getPosition().x/128, p_client.getPosition().y/128), sf::Vector2<int>((p_client.getPosition().x + 40)/128, p_client.getPosition().y/128), sf::Vector2<int>(p_client.getPosition().x/128, (p_client.getPosition().y + 40)/128), sf::Vector2<int>((p_client.getPosition().x + 40)/128, (p_client.getPosition().y + 40)/128)};
	std::vector<Client> clients;

	for (int i = 0; i < 4; i++)
	{
		for (auto it = m_branches[clientPoints[i].x+((clientPoints[i].y)*m_mapSize.x)].m_clientList.begin(); it != m_branches[clientPoints[i].x+((clientPoints[i].y)*m_mapSize.x)].m_clientList.end(); ++it){
			if (shareBranch(*(*it), p_client))
			{
				clients.push_back(*(*it));
			}
		}
	}

	return clients;
}

std::vector<Projectile> GameManager::getProjectiles(Projectile & p_projectile) const{
	sf::Vector2<int> projectilePoints[4] = {sf::Vector2<int>(p_projectile.getPosition().x/128, p_projectile.getPosition().y/128), sf::Vector2<int>((p_projectile.getPosition().x + 40)/128, p_projectile.getPosition().y/128), sf::Vector2<int>(p_projectile.getPosition().x/128, (p_projectile.getPosition().y + 40)/128), sf::Vector2<int>((p_projectile.getPosition().x + 40)/128, (p_projectile.getPosition().y + 40)/128)};
	std::vector<Projectile> projectiles;

	for (int i = 0; i < 4; i++)
	{
		for (auto it = m_branches[projectilePoints[i].x+((projectilePoints[i].y)*m_mapSize.x)].m_projectiles.begin(); it != m_branches[projectilePoints[i].x+((projectilePoints[i].y)*m_mapSize.x)].m_projectiles.end(); ++it){
			if (shareBranch(*(*it), p_projectile))
			{
				projectiles.push_back(*(*it));
			}
		}
	}

	return projectiles;
}

std::vector<Projectile> GameManager::getProjectiles(Client & p_client) const{
	sf::Vector2<int> clientPoints[4] = {sf::Vector2<int>(p_client.getPosition().x/128, p_client.getPosition().y/128), sf::Vector2<int>((p_client.getPosition().x + 40)/128, p_client.getPosition().y/128), sf::Vector2<int>(p_client.getPosition().x/128, (p_client.getPosition().y + 40)/128), sf::Vector2<int>((p_client.getPosition().x + 40)/128, (p_client.getPosition().y + 40)/128)};
	std::vector<Projectile> projectiles;

	for (int i = 0; i < 4; i++)
	{
		for (auto it = m_branches[clientPoints[i].x+((clientPoints[i].y)*m_mapSize.x)].m_projectiles.begin(); it != m_branches[clientPoints[i].x+((clientPoints[i].y)*m_mapSize.x)].m_projectiles.end(); ++it){
			if (shareBranch(p_client, *(*it)))
			{
				projectiles.push_back(*(*it));
			}
		}
	}

	return projectiles;
}