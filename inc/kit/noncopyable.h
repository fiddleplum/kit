#pragma once

// Once MSVC allows delete constructors, fix this.

namespace kit
{
	class Noncopyable
	{
	public:
		Noncopyable () {}
		virtual ~Noncopyable () {}

	private:
		Noncopyable (Noncopyable const &);
		Noncopyable & operator = (Noncopyable const &);
	};
}

