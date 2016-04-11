/* Class file for Noble Phantasms.
 *
 * Contains the data relevant to Noble Phantasms such as name, description,
 * and range.
 *
 * Also has the header for the 'activate' function, which will handle the events
 * that occur as a result of activating the Noble Phantasm. However the actual
 * implementation of this will have to be in subclasses for each NP.
 *
 */

#pragma once
#include "NoblePhantasm.h"

using namespace std;

/********** Function Definitions **********/
// Constructor
NoblePhantasm::NoblePhantasm(string n, string d)
{
    name = n;
    descrip = d;
}

/***** Active Functions *****/
void NoblePhantasm::activate(Servant attacker, vector<Servant> defenders)
{
    //Nothing here. Actual implementation is in subclasses.
    //TODO: MAKE SUBCLASSES FOR EVERY NP
}

/***** Retrievers *****/
string NoblePhantasm::getName()
{
    return name;
}

string NoblePhantasm::getDescrip()
{
    return descrip;
}

// Direction must be specified. 0 = North, 1 = East, 2 = South, 3 = West
vector<Coordinate> NoblePhantasm::getRange(int direction)
{
    vector<Coordinate> directionalRange;

    if (direction == 0)
        return range;
    else if (direction == 1)
    {
        for (int i = 0; i < range.size(); i++)
        {
            Coordinate newLoc;
            newLoc.x = range[i].y;
            newLoc.y = range[i].x * -1;
            directionalRange.push_back(newLoc);
        }
        return directionalRange;
    }
    else if (direction == 2)
    {
        for (int i = 0; i < range.size(); i++)
        {
            Coordinate newLoc;
            newLoc.x = range[i].y;
            newLoc.y = range[i].x;
            directionalRange.push_back(newLoc);
        }
        return directionalRange;
    }
    else
    {
        for (int i = 0; i < range.size(); i++)
        {
            Coordinate newLoc;
            newLoc.x = range[i].y * -1;
            newLoc.y = range[i].x;
            directionalRange.push_back(newLoc);
        }
        return directionalRange;
    }
}
