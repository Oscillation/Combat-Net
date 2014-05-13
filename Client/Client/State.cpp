#include "State.h"
#include "StateStack.h"

State::Context::Context(sf::RenderWindow& window, sf::UdpSocket& socket, sf::Font& font, sf::IpAddress address, int port)
	:
	window(&window),
	socket(&socket),
	font(&font),
	address(address),
	port(port)
{
}

State::State(StateStack& stateStack, Context context, States::ID id)
	:
	mStack(&stateStack),
	mContext(context),
	mId(id),
	mIsPersistent(false)
{
}

State::~State()
{
}

bool State::isPersistent() const
{
	return mIsPersistent;
}

void State::setPersistent(bool persitent)
{
	mIsPersistent = persitent;
}

States::ID State::getID() const
{
	return mId;
}

void State::requestStackPush(States::ID stateID)
{
	mStack->pushState(stateID);
}

void State::requestStackPop()
{
	mStack->popState();
}

void State::requestStackClear()
{
	mStack->clearStates();
}

State::Context State::getContext() const
{
	return mContext;
}