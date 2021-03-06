#pragma once
#include "Client.h"
#include "..\..\Common\Projectile.h"
#include "..\..\Common\Map.h"
#include "..\..\Common\Power.h"
#include <map>
#include <iostream>

class GameManager{
public:
	
	///<summary>(tiles/side)/branch.</summary>
	const static unsigned short m_size = 2;

	struct Branch{
		Branch();
		Branch(const unsigned int & p_x, const unsigned int & p_y);
		~Branch();

		unsigned int m_x, m_y;

		void update(Client & p_client);
		void update(Projectile & p_projectile);
		void update(Power & p_power);

		std::vector<Client*> m_clientList;
		std::vector<Projectile*> m_projectiles;
		std::vector<Power*> m_powers;
	};

	GameManager();
	GameManager(std::map<std::string, Client>* ptr_clientList, std::vector<Projectile>* ptr_projectiles, const unsigned int & p_width, const unsigned int & p_height);
	~GameManager();

	bool intersect(Client & p_client, Projectile & p_projectile) const;
	bool intersect(Client & p_clientA, Client & p_clientB) const;
	bool intersect(Projectile & p_projectileA, Projectile & p_projectileB) const;
	bool intersect(Client & p_client, Power & p_power) const;

	std::vector<Branch> m_branches;

	///<summary>Returns a vector of clients that share branches with the given projectile.</summary>
	std::vector<Client> getClients(Projectile & p_projectile) const;

	///<summary>Returns a vector of clients that share branches with the given client.</summary>
	std::vector<Client> getClients(Client & p_client) const;

	///<summary>Returns a vector of powers that share branches with the given client.</summary>
	std::vector<Power> getPowers(Client & p_client) const;

	///<summary>Returns a vector of projectiles that share branches with the given projectile.</summary>
	std::vector<Projectile> getProjectiles(Projectile & p_projectile) const;

	///<summary>Returns a vector of projectiles that share branches with the given client.</summary>
	std::vector<Projectile> getProjectiles(Client & p_client) const;

	void update(Client & p_client);
	void update(Projectile & p_projectile);
	void update(Power & p_power);

	sf::Vector2<int> m_mapSize;

	void erase(Client & p_client);
	void erase(Projectile & p_projectile);
	void erase(Power & p_power);

	void clean();

	///<summary>Returns a spawn location in tiles.</summary>
	sf::Vector2<int> selectSpawn(const std::vector<sf::Vector2<int>> p_spawnPositions);

private:

	bool shareBranch(Client & p_client, Projectile & p_projectile) const;
	bool shareBranch(Client & p_clientA, Client & p_clientB) const;
	bool shareBranch(Projectile & p_projectileA, Projectile & p_projectileB) const;
	bool shareBranch(Client & p_client, Power & p_power) const;

	bool exists(Client & p_client, std::vector<Client> & p_clients) const;
	bool exists(Projectile & p_projectile, std::vector<Projectile> & p_projectiles) const;
	bool exists(Power & p_power, std::vector<Power> & p_powers) const;

	bool exists(Client & p_client, std::vector<Client*> & p_clients) const;
	bool exists(Projectile & p_projectile, std::vector<Projectile*> & p_projectiles) const;
	bool exists(Power & p_power, std::vector<Power*> & p_powers) const;
};