#pragma once

#include <vector>
#include <map>
#include <functional>
#include <utility>

#include "State.h"
#include "StateIdentifiers.h"

#include <SFML\System\NonCopyable.hpp>


namespace sf
{
	class Event;
	class RenderWindow;
}

class StateStack : private sf::NonCopyable
{
public:
	enum Action
	{
		Push,
		Pop,
		Clear,
	};

public:
	StateStack(State::Context context);

	template <typename T>
	void						registerState(States::ID stateID);

	void						update(sf::Time dt);
	void						draw();
	void						handleEvent(const sf::Event& event);

	void						pushState(States::ID stateID);
	void						popState();
	void						clearStates();

	bool						isEmpty() const;

private:
	State::ptr					createState(States::ID stateID);
	void						applyPendingChanges();


private:
	struct PendingChange
	{
		PendingChange(Action action, States::ID stateID = States::None);
		Action				action;
		States::ID			stateID;
	};

private:
	std::vector<State::ptr>					mStack;
	std::vector<PendingChange>				mPendingList;
	std::map<States::ID, 
		std::function<State::ptr()>>		mFactories;

	std::map<States::ID, State::ptr>		mPersitantStates;
	State::Context							mContext;
};

template <typename T>
void StateStack::registerState(States::ID stateID)
{
	mFactories[stateID] = [this, &stateID] ()
	{
		return State::ptr(new T(*this, mContext, stateID));
	};
}