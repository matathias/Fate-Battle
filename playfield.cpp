/* Class file for the play field.
 *
 * Contains the data structures required for keeping track of the game field,
 * e.g. Servant locations and Caster Territories. Also contains functions for
 * determining a Servant's valid moves and for moving the Servant, as well as
 * gathering a list of all Servants in a certain range.
 *
 */

#pragma once

#include "PlayField.h"
#include "math.h"
#include "stdlib.h"

using namespace std;

/********** Function Definitions **********/
// Constructor
PlayField::PlayField(int l, int w, vector<Servant *> servantList,
                     vector<Coordinate> servantLocations)
{
    length = l; // corresponds to the y coordinate
    width = w;  // corresponds to the x coordinate

    for (int i = 0; i < w; i++)
    {
        vector<Servant*> tField;
        vector<Debuff*> tTemp;
        for (int j = 0; j < l; j++)
        {
            tField.push_back(NULL);
            tTemp.push_back(NULL);
        }

        field.push_back(tField);
        tempEffects.push_back(tTemp);
    }

    for (int i = 0; i < servantLocations.size(); i++)
    {
        Coordinate thisCoord = servantLocations[i];
        int x = thisCoord.x;
        int y = thisCoord.y;

        field[x][y] = servantList[i];
    }
}

/***** Active Functions *****/
Coordinate PlayField::moveServant(Servant *s, Coordinate c)
{
    field[s->getCurrLoc().x][s->getCurrLoc().y] = NULL;
    field[c.x][c.y] = s;

    Coordinate oldLoc = s->getCurrLoc();
    s->setLoc(c);

    return oldLoc;
}

/***** Retrievers *****/
vector<vector<Servant*>> PlayField::getServantLocations()
{
    return field;
}

vector<vector<Debuff*>> PlayField::getTerritories()
{
    return tempEffects;
}

bool PlayField::isValidCoordinate(Coordinate c)
{
    return (c.x >= 0 && c.x < width && c.y >= 0 && c.y < length &&
            field[c.x][c.y] == NULL);
}

vector<Servant*> PlayField::getAllInRange(Servant *s, vector<Coordinate> range)
{
    Coordinate baseLoc = s->getCurrLoc();
    vector<Servant*> targets;

    for (int i = 0; i < range.size(); i++)
    {
        int x = baseLoc.x + range[i].x;
        int y = baseLoc.y + range[i].y;
        if (x >= 0 && x < width && y >= 0 && y < length)
        {
            Servant* potential = field[x][y];
            if (potential != NULL)
            {
                targets.push_back(potential);
            }
        }
    }

    return targets;
}
