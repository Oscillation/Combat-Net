namespace cn
{
	enum PacketType
	{
/*0*/	PlayerConnected,
/*1*/	PlayerDisconnected,
/*2*/	PlayerInput,
/*3*/	PlayerMessage,
/*4*/	PlayerMove,
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