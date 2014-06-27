#include <kit/cursor.h>

namespace kit
{
	bool Cursor::validity () const
	{
		return _validity;
	}

	Vector2i Cursor::position () const
	{
		return _position;
	}

	void Cursor::consume ()
	{
		_consumed = true;
	}

	//void Cursor::setValidity (bool is)
	//{
	//	_isValid = is;
	//}

	//bool Cursor::isConsumed () const
	//{
	//	return _isConsumed;
	//}

	//void Cursor::resetConsumed ()
	//{
	//	_isConsumed = _isValid;
	//}

	//void Cursor::setPosition (Vector2i position)
	//{
	//	_position = position;
	//}
}

