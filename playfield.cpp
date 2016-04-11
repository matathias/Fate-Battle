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
                     vector<Coordinate> servantLocations, GameState *gs)
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
        int x = servantLocations[i].x;
        int y = servantLocations[i].y;

        field[x][y] = servantList[i];
    }

    gs = GS;
}

/***** Active Functions *****/
Coordinate PlayField::moveServant(Servant *s, Coordinate c)
{
    field[s->currLoc.x][s->currLoc.y] = NULL;
    field[c.x][c.y] = s;

    Coordinate oldLoc = s->currLoc;
    s->currLoc = c;

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
            field[x][y] == NULL);
}

vector<Coordinate> PlayField::getValidMoves(Servant *s)
{
    Coordinate baseLoc = s->currLoc;
    vector<Coordinate> moves;
    int mov = s->getMov();

    if (/*unit is provoked by opponent saber*/)
    {
        /***** TODO *****/
    }
    else if (s->getClass() == Berserker)
    {
        // Find closest enemy unit, then locate closest tile to said unit
        vector<Servant*> enemies;
        if (s->getTeam() == Alpha)
            enemies = gs->getOmegaTeam();
        else
            enemies = gs->getAlphaTeam();

        int minDist = 1000;
        Servant* closest = NULL;
        Coordinate closeLoc;

        // Locate the nearest enemy unit
        for (int i = 0; i < enemies.size(); i++)
        {
            Coordinate enLoc = enemies[i]->getCurrLoc();
            int dist = sqrt(pow((enLoc.x - baseLoc.x), 2) +
                            pow((enLoc.y - baseLoc.y), 2));
            if (dist < minDist)
            {
                minDist = dist;
                closest = enemies[i];
                closeLoc = enLoc;
            }
        }

        // Find the closest valid moves to the closest enemy unit
        if (abs(closeLoc.x - baseLoc.x) + abs(closeLoc.y - baseLoc.y) > mov)
        {
            // If the Berserker cannot reach the enemy this turn...
            Coordinate newLoc;
            if (abs(closeLoc.y - baseLoc.y) <= mov)
            {
                // If the Berserker has enough movement to close the entire
                // north-south distance...
                // Move the Berserker up to the same y coordinate, and then
                // move as close on the x coordinate as possible.
                newLoc.y = closeLoc.y;
                mov -= abs(closeLoc.y - baseLoc.y);
                if (baseLoc.x < closeLoc.x)
                {
                    newLoc.x = baseLoc.x + mov;
                }
                else
                {
                    newLoc.x = baseLoc.x - mov;
                }
            }
            else
            {
                // If the Berserker does NOT have enough movement to close the
                // entire north-south distance...
                // Move the Berserker as far as possible along the north-south
                // distance.
                if (baseLoc.y < closeLoc.y)
                {
                    newLoc.x = baseLoc.x;
                    newLoc.y = baseLoc.y + mov;
                }
                else
                {
                    newLoc.x = baseLoc.x;
                    newLoc.y = baseLoc.y - mov;
                }
            }
            moves.push_back(newLoc);
        }
        else
        {
            // If the Berserker CAN reach the enemy this turn...
            // Get the four adjacent squares to the enemy and check which ones
            // are closest.

            Coordinate north, south, east, west;
            north.x = closeLoc.x;
            north.y = closeLoc.y + 1;

            south.x = closeLoc.x;
            south.y = closeLoc.y - 1;

            east.x = closeLoc.x - 1;
            east.y = closeLoc.y;

            west.x = closeLoc.x + 1;
            west.y = closeLoc.y;

            northDist = abs(north.x - baseLoc.x) + abs(north.y - baseLoc.y);
            southDist = abs(south.x - baseLoc.x) + abs(south.y - baseLoc.y);
            eastDist = abs(east.x - baseLoc.x) + abs(east.y - baseLoc.y);
            westDist = abs(west.x - baseLoc.x) + abs(west.y - baseLoc.y);

            if (isValidCoordinate(north) && northDist < southDist &&
                    northDist < eastDist && northDist < westDist)
                moves.push_back(north);
            else if (isValidCoordinate(south) && southDist < northDist &&
                     southDist < eastDist && southDist < westDist)
                moves.push_back(south);
            else if (isValidCoordinate(east) && eastDist < northDist &&
                     eastDist < southDist && eastDist < westDist)
                moves.push_back(east);
            else if (isValidCoordinate(west))
                moves.push_back(west);
            else
                // Should never get here, since it means there's no valid
                // adjacent space
                moves.push_back(baseLoc);
        }
    }
    else
    {
        moves.push_back(baseLoc);
        /***** TODO *****/
    }

    return moves;
}

vector<Servant*> PlayField::getAllInRange(Servant *s, vector<Coordinate> range)
{
    Coordinate baseLoc = s->currLoc;
    vector<Servant*> targets;

    for (int i = 0; i < range.size(); i++)
    {
        int x = baseLoc.x + range[i].x;
        int y = baseLoc.y + range[i].y;
        if (x >= 0 && x < w && y >= 0 && y < l)
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
