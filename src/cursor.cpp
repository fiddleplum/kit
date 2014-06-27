#include "cursor_p.h"

namespace kit
{
	bool CursorP::isValid () const
	{
		return _isValid;
	}

	Vector2i CursorP::getPosition () const
	{
		return _position;
	}

	void CursorP::setValidity (bool is)
	{
		_isValid = is;
	}

	bool CursorP::isConsumed () const
	{
		return _isConsumed;
	}

	void CursorP::resetConsumed ()
	{
		_isConsumed = _isValid;
	}

	void CursorP::consume ()
	{
		_isConsumed = true;
	}

	void CursorP::setPosition (Vector2i position)
	{
		_position = position;
	}
}

