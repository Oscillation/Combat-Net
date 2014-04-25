#pragma once
#include "Client.h"
#include "..\..\Common\Projectile.h"
#include <map>

class GameManager{
	struct Branch{
		Branch();
		Branch(const int & p_x, const int & p_y);
		~Branch();

		int m_x, m_y;

		void update(Client & p_client);
		void update(Projectile & p_projectile);

		std::vector<Client*> m_clientList;
		std::vector<Projectile*> m_projectiles;
	};

public:
	GameManager();
	GameManager(std::map<std::string, Client>* ptr_clientList, std::vector<Projectile>* ptr_projectiles, const int & p_width, const int & p_height);
	~GameManager();

	bool intersect(Client & p_client, Projectile & p_projectile) const;
	bool intersect(Client & p_clientA, Client & p_clientB) const;
	bool intersect(Projectile & p_projectileA, Projectile & p_projectileB) const;

	std::vector<Branch> m_branches;

	///<summary>Returns a vector of clients that share branches with the given projectile.</summary>
	std::vector<Client> getClients(Projectile & p_projectile) const;

	///<summary>Returns a vector of clients that share branches with the given client.</summary>
	std::vector<Client> getClients(Client & p_client) const;

	///<summary>Returns a vector of projectiles that share branches with the given projectile.</summary>
	std::vector<Projectile> getProjectiles(Projectile & p_projectile) const;

	///<summary>Returns a vector of projectiles that share branches with the given client.</summary>
	std::vector<Projectile> getProjectiles(Client & p_client) const;

	void update(Client & p_client);
	void update(Projectile & p_projectile);

private:

	bool shareBranch(Client & p_client, Projectile & p_projectile) const;
	bool shareBranch(Client & p_clientA, Client & p_clientB) const;
	bool shareBranch(Projectile & p_projectileA, Projectile & p_projectileB) const;

	sf::Vector2<int> m_mapSize;
};