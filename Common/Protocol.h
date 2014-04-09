namespace cn
{
	enum PacketType
	{
		PlayerConnected,
		PlayerDisconnected,
		PlayerInput,
		PlayerMessage,
		PlayerMove,
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