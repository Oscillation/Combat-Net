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
	};

	enum InputType
	{
		MoveUp,
		MoveDown,
		MoveLeft,
		MoveRight,
		Shoot,
	};
};