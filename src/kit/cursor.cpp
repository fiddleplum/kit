#include "cursor.h"

namespace kit
{
	bool Cursor::isValid () const
	{
		return _isValid;
	}

	Vector2i Cursor::getPosition () const
	{
		return _position;
	}

	void Cursor::setValidity (bool is)
	{
		_isValid = is;
	}

	bool Cursor::isConsumed () const
	{
		return _isConsumed;
	}

	void Cursor::resetConsumed ()
	{
		_isConsumed = _isValid;
	}

	void Cursor::consume ()
	{
		_isConsumed = true;
	}

	void Cursor::setPosition (Vector2i position)
	{
		_position = position;
	}
}

