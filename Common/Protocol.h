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
		PlayerKilled,
		ScoreUpdate,
		ProjectileIDCleanUp,
		Power,
		ErasePower,
		ActivatePower,
		MatchStart,
		MatchEnd,
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

	enum MatchType
	{
		FreeForAll,
		TeamDeathmatch,
		CaptureTheFlag,
	};
}
