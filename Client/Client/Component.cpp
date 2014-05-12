#pragma once

#include "Component.h"

#include <SFML\Window\Event.hpp>


gui::Component::Component()
{
}

gui::Component::~Component()
{
}

bool gui::Component::isSelected() const
{
	return mSelected;
}

void gui::Component::select()
{
	mSelected = true;
}

void gui::Component::deselect()
{
	mSelected = false;
}

bool gui::Component::isActive() const
{
	return mActive;
}

void gui::Component::activate()
{
	mActive = true;
}

void gui::Component::deactivate()
{
	mActive = false;
}