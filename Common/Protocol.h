#pragma once

#include <SFML\Network\Packet.hpp>

namespace cn
{
	struct Score
	{
		int points;
		int deaths;
		int kills;
	};

	enum PacketType
	{
		MegaPacket,
		PlayerConnected,
		PlayerDisconnected,
		PlayerInput,
		PlayerMessage,
		PlayerMove,
		Ping,
		Map,
		NameTaken,
		Projectile,
		EraseProjectile,
		PlayerDamaged,
		PlayerKilled,
		ScoreUpdate,
	};

	enum InputType
	{
		MoveUp,
		MoveDown,
		MoveLeft,
		MoveRight,
		ShootUp,
		ShootDown,
		ShootLeft,
		ShootRight,
	};
};


sf::Packet& operator>>(sf::Packet & p_packet, cn::Score & p_score);

sf::Packet& operator<<(sf::Packet & p_packet, cn::Score & p_score);