#pragma once

namespace cn
{
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
		PlayerHealth,
		PlayerRespawn,
		ScoreUpdate,
		ProjectileIDCleanUp,
		Power,
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