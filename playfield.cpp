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

    for (int i = 0; i < tempEffects.size(); i++)
    {
        for(int j = 0; j < tempEffects[i].size(); j++)
        {
            tempEffects[i][j] = rm;
        }
    }
}

void PlayField::endRealityMarble()
{
    realityMarbleOn = false;
    rmServant = NULL;

    for (int i = 0; i < tempEffects.size(); i++)
    {
        for(int j = 0; j < tempEffects[i].size(); j++)
        {
            tempEffects[i][j] = NULL;
        }
    }
}

void PlayField::eraseTerritory(string n)
{
    for (int i = 0; i < tempEffects.size(); i++)
    {
        for(int j = 0; j < tempEffects[i].size(); j++)
        {
            if (tempEffects[i][j] != NULL &&
                tempEffects[i][j]->getDebuffDescrip().compare(n) == 0)
            {
                tempEffects[i][j] = NULL;
            }
        }
    }
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

bool PlayField::doesSpaceHaveServant(Coordinate c)
{
    return field[c.x][c.y] != NULL;
}

vector<Coordinate> PlayField::pruneRange(vector<Coordinate> range, Servant* source)
{
    for (int i = 0; i < range.size(); i++)
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

int PlayField::getFieldLength()
{
    return length;
}

int PlayField::getFieldWidth()
{
    return width;
}
