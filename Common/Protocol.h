namespace cn
{
	enum PacketType
	{
		PlayerConnected,
		PlayerDisconnected,
		PlayerInput,
		PlayerMessage,
		PlayerMove,
		Ping,
		Map,
		NameTaken,
		Projectile,
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