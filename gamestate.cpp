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
    archerSecondTurn = false;
    clickedX = 0;
    clickedY = 0;

    // Determine the locations of the servants on the board, as well as
    // seperating them out into teams.
    vector<Coordinate> servantLocations;
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

void GameState::setClickedX(int x)
{
    clickedX = x;
}

void GameState::setClickedY(int y)
{
    clickedY = y;
}

bool GameState::isServantDead(Servant *s)
{
    bool found = false;
    for (int i = 0; i < dead.size() && !found; i++)
    {
        if (dead[i] == s)
            found = true;
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

/* All functions having to do with the turn state */
int GameState::getTurnState()
{
    return turnState;
}

int GameState::nextTurnState()
{
    switch(turnState)
    {
        case 0:
            turnState++;
            turnStateMove();
            break;
        case 1:
            turnState++;
            turnStateChoseAction();
            break;
        case 2:
            turnState++;
            turnStateChoseTargets();
            break;
        case 3:
            turnState++;
            turnStateApplyAction();
            break;
        case 4:
            turnState++;
            turnStateExtraMove();
            break;
        case 5:
            turnState++;
            turnStatePostTurn();
            break;
        default:
    }
    return turnState;
}

int GameState::prevTurnState()
{
    switch(turnState)
    {
        case 2:
            turnState--;
            currentServant->setLoc(servStart);
            turnStateMove();
            break;
        case 3:
            turnState--;
            turnStateChoseAction();
            break;
        case 4:
            turnState--;
            turnStateChoseTargets();
            break;
        case 5:
            turnState--;
            turnStateApplyAction();
            break;
        case 6:
            turnState--;
            turnStateExtraMove();
            break;
        default:
    }

    return turnState;
}

int GameState::turnStatePreTurn()
{
    // Decrement the turns remaining on any Debuffs on this Servant. Remove any
    // Debuffs as necessary.
    currentServant->decDebuffs();

    // Apply any relevant Debuffs, either that are on the Servant or the field.
    Debuff *tDebuff = field->getDebuffOnSpace(currentServant->getCurrLoc());
    if (tDebuff->getTargetTeam() == currentServant->getTeam())
    {
        Debuff *newDebuff = (tDebuff->getDebuffName(),
                             tDebuff->getDebuffDescrip(),
                             tDebuff->getTargetTeam(),
                             tDebuff->getDebuffStats(),
                             tDebuff->getDebuffAmounts(), 1);
        currentServant->addDebuff(newDebuff);
    }
    int hpSub = currentServant->getDebuffAmount(HP);
    int mpSub = currentServant->getDebuffAmount(MP);
    int currHP = currentServant->getCurrHP();

    if (currHP - hpSub <= 0)
        currentServant->setHP(1);
    else
        currentServant->subHP(hpSub, OMNI);

    currentServant->subMP(mpSub);

    // Determine if there are any ongoing combat effects that belong to this
    // Servant (e.g. Reality Marbles or Caster Territories) and if the player
    // wants to continue them.
    // TODO
    if (field->isRealityMarbleOn() &&
        field->realityMarbleServant() == currentServant)
    {
        // Do some gui stuff
        // Make a new turnState that can handle this dialogue?
    }
    else if (currentServant->getClass() == Caster &&
             currentServant->isTerritoryActive())
    {
        // Do some gui stuff
        // Make a new turnState that can handle this dialogue?
    }

    turnState++;
    return 0;
}

int GameState::turnStateMove()
{
    // move the player
    //servStart = currentServant->getCurrLoc();
    currentServant->setLoc(tSCoord);
    servEnd = tSCoord;

    // Move the player on the Playing Field
    servStart = field->moveServant(currentServant, tsCoord);

    // Display a list of possible actions at their location.
    actionList = currentServant->getActionList();
    actionListTypes = currentServant->getActionListTypes();
    actionMPCosts = currentServant->getActionMPCosts();

    // TODO: Display it somehow?? Have a gui method parse the action list?

    turnState++;
    return 0;
}

int GameState::turnStateChoseAction()
{
    // Ensure that an action was actually chosen
    if (chosenAction < 0)
        return 1;

    // Verify that the chosen action is legitimate (i.e. isn't a noble phantasm
    // that isn't available, player has enough MP)
    int mpCost = currentServant->getActionMPCost(chosenAction);
    if (mpCost < currentServant->getCurrMP())
    {
        chosenAction = -1;
        return 2;
    }

    chosenActionType = currentServant->getActionType(chosenAction);

    // If the chosen action is single-target, get the range and have the player
    // select a target from within that range.
    if (chosenActionType == S)
    {
        selectionRange = currentServant->getActionRange(chosenAction);
        selectionRange = field->pruneRange(selectionRange);
    }

    // If the action is AOE, have the player determine the direction of
    // application and get all the targets in that range.
    else if (chosenActionType == A)
    {
        Coordinate n, s, e, w;
        n.x = 0;
        n.y = 1;
        s.x = 0;
        s.y = -1;
        e.x = 1;
        e.y = 0;
        w.x = -1;
        w.y = 0;
        selectionRange.push_back(n);
        selectionRange.push_back(s);
        selectionRange.push_back(e);
        selectionRange.push_back(w);
    }

    // If the action is territory creation, skip straight to the apply action
    // turn state and apply the action.
    else if (chosenActionType == T)
    {
        turnState = 4;
        turnStateApplyAction();
        return 0;
    }

    // If the action is targetting a dead servant, give the player a list of
    // dead servants to select from.
    // TODO
    else if (chosenActionType == D)
    {

    }

    // If the action does not have any targets then process it accordingly.
    // TODO
    else
    {
        turnState = 4;
        turnStateApplyAction();
        return 0;
    }

    turnState++;
    return 0;
}

int GameState::turnStateChoseTargets()
{
    // Ensure that the chosen targets are valid (?) and then call the apply
    // action turn state
    // This turn state may not actually be necessary...
    // TODO
    if (chosenActionType == S)
    {
        //
    }
    else if (chosenActionType == A)
    {
        //
    }
    else if (chosenActionType == D)
    {
        //
    }
    else
    {
        // Why are we here??
        return 1;
    }

    return 0;
}

int GameState::turnStateApplyAction()
{
    // Apply the chosen action to the chosen targets.
    // TODO

    // If the Servant is a Rider, allow them to move again.
    // TODO

    // If the Servant is an Archer, check against their LUK if they get a
    // second turn. If they do, set archerSecondTurn to true and go back to
    // the beginning move turn state. If archerSecondTurn is already true then
    // end the Servant's turn.
    // TODO

    // End the non-Rider turn by calling the Post-turn turn state.
    // TODO

    return 0;
}

int GameState::turnStateExtraMove()
{
    // move the player
    servStart = currentServant->getCurrLoc();
    currentServant->setLoc(tSCoord);
    servEnd = tSCoord;

    // Move the player on the Playing Field
    // TODO

    // End the turn by calling the Post-turn turn state.
    // TODO

    return 0;
}

int GameState::turnStatePostTurn()
{
    // Check if the player's target location has any relevant debuffs and apply
    // them.
    // TODO

    // Check if anyone has died and modify the death list accordingly.
    // TODO

    // Reset the turnState and all relevant variables and get the next Servant.
    // TODO

    return 0;
}
