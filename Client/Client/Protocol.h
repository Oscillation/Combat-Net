namespace cn
{
	enum PacketType
	{
/*0*/	PlayerConnected,
/*1*/	PlayerDisconnected,
/*2*/	PlayerInput,
/*3*/	PlayerMessage,
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