#pragma once

#include <vector>

/*

Creates unique Ids, returning the least positive free Id.
All functions are O(1).

*/

class UniqueIdFactory
{
public:
    UniqueIdFactory ()
    {
        freeIds.add(0);
    }

    int createId ()
    {
        int id = freeIds.back();
        freeIds.pop();
        if (freeIds.empty())
        {
            freeIds.add(id + 1); // bottom of stack is 1 + the largest created id
        }
        return id;
    }

    void destroyId (int id)
    {
        freeIds.push_back(id);
    }

private:
    std::vector<Id> freeIds;
};

