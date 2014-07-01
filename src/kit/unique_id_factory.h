#pragma once

#include <stack>

namespace kit
{
	/*
	Creates unique ids, returning the least positive free id. All functions are O(1).
	*/
	class UniqueIdFactory
	{
	public:
		UniqueIdFactory ()
		{
			freeIds.push(0);
		}

		int createId ()
		{
			int id = freeIds.top();
			freeIds.pop();
			if (freeIds.empty())
			{
				freeIds.push(id + 1); // bottom of stack is 1 + the largest created id
			}
			return id;
		}

		void destroyId (int id)
		{
			freeIds.push(id);
		}

	private:
		std::stack<unsigned int> freeIds;
	};
}

