//#pragma once
//
//#include <kit/cursor.h>
//
//namespace kit
//{
//	class CursorP : virtual public Cursor
//	{
//	public:
//		bool isValid () const override;
//
//		Vector2i getPosition () const override;
//
//		void setValidity (bool);
//
//		bool isConsumed () const;
//
//		void resetConsumed ();
//
//		void consume ();
//
//		void setPosition (Vector2i);
//
//	private:
//		bool _isConsumed; // has been consumed by a widget for a particular event. resets every event.
//		bool _isValid; // is within window and is visible
//		Vector2i _position; // relative to window position
//	};
//}
//
