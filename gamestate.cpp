/* Class file for the Game State.
 *
 * Contains the implementation of certain functions and data structures that are
 * required to track the state of the game, such as which Servants are dead,
 * turn order, what teams the Servants are on, etc.
 *
 */

#pragma once

#include "GameState.h"
using namespace std;

/********** Function Definitions **********/
GameState::GameState(vector<Servant *> tO, int l, int w)
{
    turnOrder = tO;

    // Determine the locations of the servants on the board, as well as
    // seperating them out into teams.
    vector<Coordinate> servantLocations;// = new vector<Coordinate>;
    int aCounter = 0, oCounter = 0, bCounter = 0;
    for (int i = 0; i < turnOrder.size(); i++)
    {
        if (turnOrder[i]->getTeam() == Alpha)
        {
            alphaTeam.push_back(turnOrder[i]);
            Coordinate thisLoc;
            thisLoc.x = (w / 2) + 4 - aCounter;
            thisLoc.y = 0;
            aCounter++;
            servantLocations.push_back(thisLoc);
            turnOrder[i]->setLoc(thisLoc);
        }
        else if (turnOrder[i]->getTeam() == Omega)
        {
            omegaTeam.push_back(turnOrder[i]);
            Coordinate thisLoc;
            thisLoc.x = (w / 2) - 4 + oCounter;
            thisLoc.y = l - 1;
            oCounter++;
            servantLocations.push_back(thisLoc);
            turnOrder[i]->setLoc(thisLoc);
        }
        else // Boss Team
        {
            bossTeam.push_back(turnOrder[i]);
            Coordinate thisLoc;
            if (aCounter > 0)
            {
                thisLoc.x = (w / 2) - 1 + bCounter;
                thisLoc.y = l - 1;
            }
            else
            {
                thisLoc.x = (w / 2) + 1 - bCounter;
                thisLoc.y = 0;
            }
            bCounter++;
            servantLocations.push_back(thisLoc);
            turnOrder[i]->setLoc(thisLoc);
        }
    }

    field = new PlayField(l, w, turnOrder, servantLocations);
}

/***** Manipulators *****/
void GameState::addDead(Servant *s)
{
    dead.push_back(s);
}

void GameState::removeDead(Servant *s)
{
    bool found = false;
    for (int i = 0; i < dead.size() && !found; i++)
    {
        if (dead[i] == s)
        {
            found = true;
            dead.erase(dead.begin()+i);
        }
    }
}

bool GameState::isServantDead(Servant *s)
{
    bool found = false;
    for (int i = 0; i < dead.size() && !found; i++)
    {
        if (dead[i] == s)
        {
            found = true;
        }
    }

    return found;
}

bool GameState::isTeamDead(Team t)
{
    int aDead = 0;
    int oDead = 0;
    int bDead = 0;

    for (int i = 0; i < dead.size(); i++)
    {
        if (dead[i]->getTeam() == Alpha)
            aDead++;
        else if (dead[i]->getTeam() == Omega)
            oDead++;
        else
            bDead++;
    }

    if (t == Alpha)
    {
        return (aDead == alphaTeam.size());
    }
    else if (t == Omega)
    {
        return (oDead == omegaTeam.size());
    }
    else
    {
        return (bDead == bossTeam.size());
    }
}

/***** Retrievers *****/
vector<Servant*> GameState::getTurnOrder()
{
    return turnOrder;
}

Servant* GameState::getNextServant()
{
    Servant* next = turnOrder.front();
    turnOrder.erase(turnOrder.begin());
    turnOrder.push_back(next);

    return next;
}

Servant* GameState::peekNextServant()
{
    return turnOrder.front();
}

// rewrite/modify to work from within GameState instead of PlayField
/*vector<Coordinate> GameState::getValidMoves(Servant *s)
{
    Coordinate baseLoc = s->currLoc;
    vector<Coordinate> moves;
    int mov = s->getMov();

    if (true) //unit is provoked by opponent saber*)
    {
        // ***** TODO *****
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
        // ***** TODO *****
    }

    return moves;
}*/

vector<Servant*> GameState::getDead()
{
    return dead;
}

vector<Servant*> GameState::getAlphaTeam()
{
    return alphaTeam;
}

vector<Servant*> GameState::getOmegaTeam()
{
    return omegaTeam;
}

vector<Servant*> GameState::getBossTeam()
{
    return bossTeam;
}
