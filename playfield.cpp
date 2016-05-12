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

bool isInVector(vector<Coordinate> vec, Coordinate c)
{
    bool found = false;
    for (unsigned int i = 0; i < vec.size() && !found; i++)
    {
        if (vec[i].x == c.x && vec[i].y == c.y)
            found = true;
    }

    return found;
}

Direction getDirectionAtoB(Coordinate a, Coordinate b)
{
    int xDist = b.x - a.x;
    int yDist = b.y - a.y;

    if (xDist > 0) // Eastern
    {
        if(xDist > abs(yDist))
            return EAST;
        else if (yDist > 0)
            return NORTH;
        else
            return SOUTH;
    }
    else // Western
    {
        if(abs(xDist) > abs(yDist))
            return WEST;
        else if (yDist > 0)
            return NORTH;
        else
            return SOUTH;
    }
}


vector<Coordinate> getLowToHighRange(int low, int high)
{
    vector<Coordinate> range;

    for (int i = -1*high; i <= high; i++)
    {
        for (int j = -1*high; j <= high; j++)
        {
            if (abs(i) + abs(j) >= low && abs(i) + abs(j) <= high)
            {
                Coordinate inRange;
                inRange.x = i;
                inRange.y = j;
                range.push_back(inRange);
            }
        }
    }

    return range;
}


/********** Function Definitions **********/
// Constructor
PlayField::PlayField(int l, int w, vector<Servant *> servantList,
                     vector<Coordinate> servantLocations)
{
    length = l; // corresponds to the y coordinate
    width = w;  // corresponds to the x coordinate

    realityMarbleOn = false;
    rmServant = NULL;

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

    for (unsigned int i = 0; i < servantLocations.size(); i++)
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
    Coordinate oldLoc = s->getCurrLoc();

    field[oldLoc.x][oldLoc.y] = NULL;
    field[c.x][c.y] = s;

    s->setLoc(c);

    return oldLoc;
}

void PlayField::startRealityMarble(Servant *owner, Debuff* rm)
{
    realityMarbleOn = true;
    rmServant = owner;

    for (unsigned int i = 0; i < tempEffects.size(); i++)
    {
        for(unsigned int j = 0; j < tempEffects[i].size(); j++)
        {
            tempEffects[i][j] = rm;
        }
    }
}

void PlayField::endRealityMarble()
{
    rmServant->endRealityMarble();
    realityMarbleOn = false;
    rmServant = NULL;

    for (unsigned int i = 0; i < tempEffects.size(); i++)
    {
        for(unsigned int j = 0; j < tempEffects[i].size(); j++)
        {
            tempEffects[i][j] = NULL;
        }
    }
}

void PlayField::eraseTerritory(string n)
{
    for (unsigned int i = 0; i < tempEffects.size(); i++)
    {
        for(unsigned int j = 0; j < tempEffects[i].size(); j++)
        {
            if (tempEffects[i][j] != NULL &&
                tempEffects[i][j]->getDebuffDescrip().compare(n) == 0)
            {
                tempEffects[i][j] = NULL;
            }
        }
    }
}

void PlayField::servantDead(Servant *s)
{
    Coordinate c = s->getCurrLoc();

    // Make sure that the space actually points to this Servant. Don't want to
    // accidentally remove a different Servant.
    if (field[c.x][c.y] == s)
    {
        field[c.x][c.y] = NULL;
        // If the Servant had a reality marble or territory going, erase it
        if (isRealityMarbleOn() && realityMarbleServant() == s)
            endRealityMarble();
    }
}

void PlayField::servantRevived(Servant *s)
{
    Coordinate c = s->getCurrLoc();
    c = getNearestValidCoord(c);

    field[c.x][c.y] = s;
    s->setLoc(c);
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

bool PlayField::isCoordinateInBounds(Coordinate c)
{
    return (c.x >= 0 && c.x < width && c.y >= 0 && c.y < length);
}

bool PlayField::doesSpaceHaveServant(Coordinate c)
{
    return field[c.x][c.y] != NULL;
}

vector<Coordinate> PlayField::pruneRange(vector<Coordinate> range, Servant* source)
{
    for (unsigned int i = 0; i < range.size(); i++)
    {
        Coordinate sc;
        sc.x = range[i].x + source->getCurrLoc().x;
        sc.y = range[i].y + source->getCurrLoc().y;
        if(!doesSpaceHaveServant(sc))
        {
            range.erase(range.begin()+i);
            i--;
        }
    }
    return range;
}

vector<Servant*> PlayField::getAllInRange(Servant *s, vector<Coordinate> range)
{
    Coordinate baseLoc = s->getCurrLoc();
    vector<Servant*> targets;

    for (unsigned int i = 0; i < range.size(); i++)
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

bool PlayField::isRealityMarbleOn()
{
    return realityMarbleOn;
}

Servant* PlayField::realityMarbleServant()
{
    return rmServant;
}

Servant* PlayField::getServantOnSpace(Coordinate c)
{
    if (c.x >= 0 && c.x < width && c.y >= 0 && c.y < length)
        return field[c.x][c.y];
    else
        return NULL;
}

Debuff* PlayField::getDebuffOnSpace(Coordinate c)
{
    if (c.x >= 0 && c.x < width && c.y >= 0 && c.y < length)
        return tempEffects[c.x][c.y];
    else
        return NULL;
}

Coordinate PlayField::getRandomCoord()
{
    bool validCoord = false;
    Coordinate ret;
    while (!validCoord)
    {
        ret.x = rand() % width;
        ret.y = rand() % length;

        if (field[ret.x][ret.y] == NULL)
            validCoord = true;
    }

    return ret;
}

// This function assumes that a valid coordinate exists on the playing field.
Coordinate PlayField::getNearestValidCoord(Coordinate c)
{
    Coordinate ret;
    if (isValidCoordinate(c))
        ret = c;
    else
    {
        bool found = false;
        vector<Coordinate> nextCoords;
        vector<Coordinate> procCoords;
        procCoords.push_back(c);

        int i = 0;
        while (!found)
        {
            nextCoords = getAdjacentSpaces(procCoords[i]);
            i++;

            while (nextCoords.size() > 0 && !found)
            {
                if (isInVector(procCoords, nextCoords[0]))
                {
                    nextCoords.erase(nextCoords.begin());
                }
                else if (isValidCoordinate(nextCoords[0]))
                {
                    ret = nextCoords[0];
                    found = true;
                }
                else
                {
                    procCoords.push_back(nextCoords[0]);
                    nextCoords.erase(nextCoords.begin());
                }
            }
        }
    }

    return ret;
}

// Returns the four adjacent spaces to c.
// If a space is out of bounds it is not included.
vector<Coordinate> PlayField::getAdjacentSpaces(Coordinate c)
{
    vector<Coordinate> spaces;
    Coordinate north, south, east, west;
    north.x = south.x = c.x;
    north.y = c.y + 1;
    south.y = c.y - 1;

    east.y = west.y = c.y;
    east.x = c.x - 1;
    west.x = c.x + 1;

    if (isCoordinateInBounds(north))
        spaces.push_back(north);
    if (isCoordinateInBounds(south))
        spaces.push_back(south);
    if (isCoordinateInBounds(east))
        spaces.push_back(east);
    if (isCoordinateInBounds(west))
        spaces.push_back(west);

    return spaces;
}

int PlayField::getFieldLength()
{
    return length;
}

int PlayField::getFieldWidth()
{
    return width;
}

int PlayField::getLargestDimension()
{
    if (length > width)
        return length;
    else
        return width;
}
