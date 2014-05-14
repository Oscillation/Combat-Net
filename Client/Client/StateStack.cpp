#include "StateStack.h"

#include <cassert>

StateStack::StateStack(State::Context context)
	:
	mStack(),
	mPendingList(),
	mFactories(),
	mContext(context),
	mPersitantStates()
{
}

State::ptr StateStack::createState(States::ID stateID)
{
	auto found = mFactories.find(stateID);
	assert(found != mFactories.end());

	return found->second();
}

void StateStack::update(sf::Time dt)
{
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		if (!(*itr)->update(dt))
			break;
	}

	applyPendingChanges();
}

void StateStack::draw()
{
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		if (!(*itr)->draw())
			break;
	}
}

void StateStack::handleEvent(const sf::Event& event)
{
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		if (!(*itr)->handleEvents(event))
			break;
	}
}

void StateStack::pushState(States::ID stateID)
{
	mPendingList.push_back(PendingChange(Action::Push, stateID));
}

void StateStack::popState()
{
	mPendingList.push_back(PendingChange(Action::Pop));
}

void StateStack::clearStates()
{
	mPendingList.push_back(PendingChange(Action::Clear));
}

bool StateStack::isEmpty() const
{
	return mStack.size() == 0;
}

void StateStack::applyPendingChanges()
{
	for (PendingChange change : mPendingList)
	{
		if( change.action == Action::Push)
		{
			size_t i = mPersitantStates.count(change.stateID);
			if (i < 1)
			{
				mStack.push_back(createState(change.stateID));
			}
			else
			{
				mStack.push_back(std::move(mPersitantStates[change.stateID]));
			}
		}
		else if (change.action == Action::Pop)
		{
			if (mStack.back()->isPersistent())
			{
				mPersitantStates[mStack.back()->getID()] = std::move(mStack.back());
			}
			mStack.pop_back();
		}
		else if (change.action == Action::Clear) 
		{
			mStack.clear();
		}
	}
	mPendingList.clear();
}

StateStack::PendingChange::PendingChange(Action action, States::ID stateID)
	:
	action(action),
	stateID(stateID)
{
}