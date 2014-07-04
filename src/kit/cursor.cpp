#include "cursor.h"

namespace kit
{
	Cursor::Cursor ()
	{
		_isConsumed = false;
		_isWithinWindow = true;
		_isVisible = true;
		_position.set(0, 0);
	}

	Vector2i Cursor::getPosition () const
	{
		return _position;
	}

	bool Cursor::isPositionValid () const
	{
		return !_isConsumed;
	}

	void Cursor::resetConsumed ()
	{
		_isConsumed = !(_isWithinWindow && _isVisible);
	}

	void Cursor::consume ()
	{
		_isConsumed = true;
	}

	void Cursor::setPosition (Vector2i position)
	{
		_position = position;
	}

	void Cursor::setWithinWindow (bool is)
	{
		_isWithinWindow = is;
	}
}

