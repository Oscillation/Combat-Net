#pragma once

#include <SFML\Window\Event.hpp>
#include <SFML\System\Time.hpp>

#include <memory>

#include "StateIdentifiers.h"

#include <SFML\Network\IpAddress.hpp>

#include "..\..\Common\Score.h"
#include "Player.h"
#include <map>

namespace sf
{
	class RenderWindow;
	class UdpSocket;
	class Font;
	class IpAddress;
}

class StateStack;

class State 
{
public:
	typedef std::unique_ptr<State> ptr;

	struct Context
	{
		Context(sf::RenderWindow& window, sf::UdpSocket& socket, sf::Font& font, sf::IpAddress address, int port);

		sf::RenderWindow* window;
		sf::UdpSocket* socket;
		sf::Font* font;
		sf::IpAddress* address;
		std::string username;
		int port;
		std::map<std::string, std::unique_ptr<Player>>* scores;
	};

public:
	State(StateStack& stateStack, Context& context, States::ID id);
	virtual				~State();

	virtual bool		update(sf::Time & p_deltaTime) = 0;
	virtual bool		draw() = 0;
	virtual bool		handleEvents(const sf::Event& event) = 0;

	bool				isPersistent() const;
	void				setPersistent(bool persitent);

	virtual States::ID	getID() const;

protected:
	void				requestStackPush(States::ID stateID);
	void				requestStackPop();
	void				requestStackClear();

	Context*			getContext();

private:
	StateStack*			mStack;
	Context*			mContext;
	bool				mIsPersistent;
	States::ID			mId;
};