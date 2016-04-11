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
    vector<Coordinate> servantLocations = new vector<Coordinate>;
    int aCounter = 0, oCounter = 0;
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
            turnOrder[i]->currLoc = thisLoc;
        }
        else
        {
            omegaTeam.push_back(turnOrder[i]);
            Coordinate thisLoc;
            thisLoc.x = (w / 2) - 4 + oCounter;
            thisLoc.y = l - 1;
            oCounter++;
            servantLocations.push_back(thisLoc);
            turnOrder[i]->currLoc = thisLoc;
        }
    }

    *field = new PlayField(l, w, turnOrder, servantLocations);
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
