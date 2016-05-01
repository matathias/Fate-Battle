/* Class file for the Game State.
 *
 * Contains the implementation of certain functions and data structures that are
 * required to track the state of the game, such as which Servants are dead,
 * turn order, what teams the Servants are on, etc.
 *
 */

#pragma once

#include "GameState.h"
#include <cmath>
#include <iostream>

using namespace std;

/********** Function Definitions **********/
GameState::GameState(vector<Servant *> tO, int l, int w)
{
    // Get the turn order (based on speed) from
    while (tO.size() > 0)
    {
        int highestSpeed = 0;
        Servant *fastest = NULL;
        int index = 0;
        for (int i = 0; i < tO.size(); i++)
        {
            if(tO[i]->getSpd() > highestSpeed)
            {
                highestSpeed = tO[i]->getSpd();
                fastest = tO[i];
                index = i;
            }
        }
        turnOrder.push_back(fastest);
        tO.erase(tO.begin()+index);

        if (fastest->getTeam() == Alpha)
            alphaTeam.push_back(fastest);
        else if (fastest->getTeam() == Omega)
            omegaTeam.push_back(fastest);
        else
            bossTeam.push_back(fastest);
    }

    // Determine the locations of the servants on the board, as well as
    // seperating them out into teams.
    vector<Coordinate> servantLocations;
    int aCounter = 0, oCounter = 0, bCounter = 0;
    for (int i = 0; i < turnOrder.size(); i++)
    {
        if (turnOrder[i]->getTeam() == Alpha)
        {
            Coordinate thisLoc;
            thisLoc.x = (w / 2) + 4 - aCounter;
            thisLoc.y = 0;
            aCounter++;
            servantLocations.push_back(thisLoc);
            turnOrder[i]->setLoc(thisLoc);
        }
        else if (turnOrder[i]->getTeam() == Omega)
        {
            Coordinate thisLoc;
            thisLoc.x = (w / 2) - 4 + oCounter;
            thisLoc.y = l - 1;
            oCounter++;
            servantLocations.push_back(thisLoc);
            turnOrder[i]->setLoc(thisLoc);
        }
        else // Boss Team
        {
            Coordinate thisLoc;
            if (alphaTeam.size() > 0 && omegaTeam.size() == 0)
            {
                thisLoc.x = (w / 2) - 1 + bCounter;
                thisLoc.y = l - 1;
            }
            else if (alphaTeam.size() == 0 && omegaTeam.size() > 0)
            {
                thisLoc.x = (w / 2) + 1 - bCounter;
                thisLoc.y = 0;
            }
            else
            {
                thisLoc.x = (w / 2) + 1 - bCounter;
                thisLoc.y = l / 2;
            }
            bCounter++;
            servantLocations.push_back(thisLoc);
            turnOrder[i]->setLoc(thisLoc);
        }
    }

    if (aCounter > 0)
        activeTeams.push_back(Alpha);
    if (oCounter > 0)
        activeTeams.push_back(Omega);
    if (bCounter > 0)
        activeTeams.push_back(Boss);

    field = new PlayField(l, w, turnOrder, servantLocations);
    resetTurnValues();
    currentServant = getNextServant();
    turnStatePreTurn();
}

/***** Manipulators *****/
void GameState::addDead(Servant *s)
{
    dead.push_back(s);
    // Remove all casted debuffs
    // Check if there is an active territory -- if there is, delete all
    //     "territory" debuffs in the casted debuff list and remove all
    //     "territory" debuffs tied to this servant from the playing
    //     field
    if (s->isTerritoryActive())
    {
        string terrName = s->removeTerritory();
        field->eraseTerritory(terrName);
    }

    // Put all other casted debuffs at 1 turn remaining so that they are
    // eliminated at the beginning of the Servant's turn who is
    // suffering them.
    s->remCastedDebuffs();
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

void GameState::setChosenAction(int a)
{
    chosenAction = a;
    chosenActionType = actionListTypes[a];
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
Servant* GameState::isSpaceServant(int x, int y)
{
    Coordinate temp;
    temp.x = x;
    temp.y = y;
    return field->getServantOnSpace(temp);
}

bool GameState::isSpaceSelection(int x, int y)
{
    bool isSelection = false;
    for (int i = 0; i < selectionRange.size() && !isSelection; i++)
    {
        if (x == selectionRange[i].x + currentServant->getCurrLoc().x &&
            y == selectionRange[i].y + currentServant->getCurrLoc().y)
            isSelection = true;
    }
    return isSelection;
}

bool GameState::isSpaceMove(int x, int y)
{
    bool isMove = false;
    for (int i = 0; i < validMoves.size() && !isMove; i++)
    {
        if (x == validMoves[i].x && y == validMoves[i].y)
            isMove = true;
    }
    return isMove;
}

bool GameState::isSpaceRealityMarble(int x, int y)
{
    return field->isRealityMarbleOn();
}

bool GameState::isSpaceDebuff(int x, int y)
{
    Coordinate temp;
    temp.x = x;
    temp.y = y;
    return (field->getDebuffOnSpace(temp) != NULL);
}

Team GameState::spaceDebuffTeam(int x, int y)
{
    Coordinate temp;
    temp.x = x;
    temp.y = y;
    Debuff *t = field->getDebuffOnSpace(temp);
    if (t == NULL)
        return Boss;
    else
        return t->getTargetTeam();
}

vector<Servant*> GameState::getTurnOrder()
{
    return turnOrder;
}

Servant* GameState::getNextServant()
{
    Servant* next = NULL;

    int servantCount = turnOrder.size();
    bool foundAlive = false;
    for (int i = 0; i < servantCount && !foundAlive; i++)
    {
        next = turnOrder.front();
        turnOrder.erase(turnOrder.begin());
        turnOrder.push_back(next);

        if(!isServantDead(next))
        {
            foundAlive = true;
        }
    }

    return next;
}

Servant* GameState::peekNextServant()
{
    int index = 0;
    bool foundAlive = false;
    for (int i = 0; i < turnOrder.size() && !foundAlive; i++)
    {
        if (!isServantDead(turnOrder[i]))
        {
            foundAlive = true;
            index = i;
        }
    }
    return turnOrder[index];
}

Servant* GameState::getCurrentServant()
{
    return currentServant;
}

// Get the servant's class and weapon and return a string with both
string GameState::getCurrentServantType()
{
    return currentServant->getName();
}

string GameState::getCurrentServantTeam()
{
    return currentServant->getTeamName();
}

// Returns the four adjacent spaces to c.
// If a space is out of bounds or occupied it is not included.
vector<Coordinate> GameState::getAdjacentSpaces(Coordinate c)
{
    vector<Coordinate> spaces;
    Coordinate north, south, east, west;
    north.x = south.x = c.x;
    north.y = c.y + 1;
    south.y = c.y - 1;

    east.y = west.y = c.y;
    east.x = c.x - 1;
    west.x = c.x + 1;

    if (field->isValidCoordinate(north))
        spaces.push_back(north);
    if (field->isValidCoordinate(south))
        spaces.push_back(south);
    if (field->isValidCoordinate(east))
        spaces.push_back(east);
    if (field->isValidCoordinate(west))
        spaces.push_back(west);

    return spaces;
}

// If c is not adjacent to any space in range and getClosest is false,
// the returned coordinate will have x = y = -1.
// If c is not adjacent to any space in range and getClosest is true,
// the returned coordinate will be the coordinate in range that is
// closest to c.
// Otherwise, returns a coordinate in range adjacent to c.
Coordinate GameState::getAdjacentInRange(Coordinate c, vector<Coordinate> range,
                                         bool getClosest)
{
    int minDist = 1000;
    Coordinate retLoc;
    retLoc.x = retLoc.y = -1;
    bool found = false;

    for (int i = 0; i < range.size() && !found; i++)
    {
        int dist = abs(range[i].x - c.x) + abs(range[i].y - c.y);

        if (dist == 1)
        {
            retLoc.x = range[i].x;
            retLoc.y = range[i].y;
            found = true;
        }
        else if (getClosest && dist < minDist)
        {
            minDist = dist;
            retLoc.x = range[i].x;
            retLoc.y = range[i].y;
        }
    }

    return retLoc;
}

// Returns the full possible movement range of the servant, accounting for
// obstacles like other servants and the edge of the map
vector<Coordinate> GameState::getFullMoveRange(Servant *s, int moveRange)
{
    Coordinate baseLoc = s->getCurrLoc();
    vector<Coordinate> moves;
    moves.push_back(baseLoc);
    int startIndex = 0;
    int endIndex = 0;
    int i = 0;
    while (moveRange > 0)
    {
        startIndex = endIndex;
        endIndex = moves.size();
        i = startIndex;
        while (i >= startIndex && i < endIndex)
        {
            vector<Coordinate> tempMoves = getAdjacentSpaces(moves[i]);
            for (int j = 0; j < tempMoves.size(); j++)
            {
                bool alreadyIn = false;
                for (int k = 0; k < moves.size() && !alreadyIn; k++)
                {
                    if (tempMoves[j].x == moves[k].x &&
                        tempMoves[j].y == moves[k].y)
                        alreadyIn = true;
                }

                if (!alreadyIn)
                    moves.push_back(tempMoves[j]);
            }
            i++;
        }
        moveRange--;
    }

    return moves;
}

vector<Coordinate> GameState::getValidMoves(Servant *s, int mov)
{
    //cout << "Beginning of GameState::getValidMoves().\n" << std::flush;
    Coordinate baseLoc = s->getCurrLoc();
    vector<Coordinate> moves = getFullMoveRange(s, mov);

    // Find if the unit has a "provoked" debuff
    bool isProvoked = false;
    vector<Debuff*> tempDebuffs = s->getDebuffs();
    for (int i = 0; i < tempDebuffs.size() && !isProvoked; i++)
    {
        if (tempDebuffs[i]->getDebuffName().compare("Provoked") == 0)
            isProvoked = true;
    }

    if (isProvoked) //unit is provoked by opponent saber
    {
        // Get the location of the enemy saber
        vector<Servant*> enemies = getEnemyTeam(s);
        Coordinate saberLoc;
        bool foundSaber = false;
        for (int i = 0; i < enemies.size() && !foundSaber; i++)
        {
            if (enemies[i]->getClass() == Saber)
            {
                foundSaber = true;
                saberLoc = enemies[i]->getCurrLoc();
            }
        }

        if (foundSaber) //If the saber is not found then we just return the full
                        // moverange
        {
            Coordinate moveLocation = getAdjacentInRange(saberLoc, moves, true);

            int i = 0;
            while (i < moves.size())
            {
                if (moveLocation.x == moves[i].x &&
                        moveLocation.y == moves[i].y)
                    i++;
                else
                    moves.erase(moves.begin()+i);
            }
        }
    }
    else if (s->getClass() == Berserker)
    {
        // Find closest enemy unit, then locate closest tile to said unit
        vector<Servant*> enemies = getEnemyTeam(s);

        int minDist = 1000;
        Servant* closest = NULL;
        Coordinate closeLoc;

        // Locate the nearest enemy unit
        for (int i = 0; i < enemies.size(); i++)
        {
            Coordinate enLoc = enemies[i]->getCurrLoc();
            int dist = abs(enLoc.x - baseLoc.x) + abs(enLoc.y - baseLoc.y);
            if (dist < minDist)
            {
                minDist = dist;
                closest = enemies[i];
                closeLoc = enLoc;
            }
        }

        // Get a coordinate either adjacent to the nearest servant, or closest
        // to the nearest servant.
        Coordinate moveLocation = getAdjacentInRange(closeLoc, moves, true);

        int i = 0;
        while (i < (int) moves.size())
        {
            if (moveLocation.x == moves[i].x && moveLocation.y == moves[i].y)
                i++;
            else
                moves.erase(moves.begin()+i);
        }
    }

    //cout << "End of GameState::getValidMoves().\n" << std::flush;
    return moves;
}

vector<string> GameState::getActionList()
{
    return actionList;
}

vector<ActionType> GameState::getActionListType()
{
    return actionListTypes;
}

vector<int> GameState::getActionMPCosts()
{
    return actionMPCosts;
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

vector<Servant*> GameState::getBossTeam()
{
    return bossTeam;
}

vector<Servant*> GameState::getEnemyTeam(Servant *s)
{
    vector<Servant*> enemies;
    vector<Servant*> a = getAlphaTeam();
    vector<Servant*> o = getOmegaTeam();
    vector<Servant*> b = getBossTeam();
    if (s->getTeam() == Alpha)
    {
        for (unsigned int i = 0; i < o.size(); i++)
            enemies.push_back(o[i]);
        for (unsigned int i = 0; i < b.size(); i++)
            enemies.push_back(b[i]);
    }
    else if (s->getTeam() == Omega)
    {
        for (unsigned int i = 0; i < a.size(); i++)
            enemies.push_back(a[i]);
        for (unsigned int i = 0; i < b.size(); i++)
            enemies.push_back(b[i]);
    }
    else
    {
        for (unsigned int i = 0; i < o.size(); i++)
            enemies.push_back(o[i]);
        for (unsigned int i = 0; i < a.size(); i++)
            enemies.push_back(a[i]);
    }

    return enemies;
}

int GameState::getFieldLength()
{
    return field->getFieldLength();
}

int GameState::getFieldWidth()
{
    return field->getFieldWidth();
}

/* All functions having to do with the turn state */
int GameState::getTurnState()
{
    return turnState;
}

int GameState::nextTurnState()
{
    //cout << "Beginning of GameState::nextTurnState().\n" << std::flush;
    int result = 999;
    switch(turnState)
    {
        case 0:
            result = turnStatePreTurn();
            break;
        case 1:
            result = turnStateMove();
            break;
        case 2:
            result = turnStateChoseAction();
            break;
        case 3:
            result = turnStateChoseTargets();
            break;
        case 4:
            result = turnStateApplyAction();
            break;
        case 5:
            result = turnStateExtraMove();
            break;
        case 6:
            result = turnStatePostTurn();
            break;
        default:
            // Shouldn't ever get here...
            addToEventLog("Error: invalid turn state (nextTurnState())");
            break;
    }

    //cout << "End of GameState::nextTurnState().\n" << std::flush;
    return result;
}

int GameState::prevTurnState()
{
    //cout << "Beginning of GameState::prevTurnState().\n" << std::flush;

    int result = 0;

    if (turnState == 2)
    {
        field->moveServant(currentServant, servStart);
    }
    else if (turnState < 2 || turnState > 6)
    {
        addToEventLog("Error: invalid turn state (prevTurnState())");
        return 999;
    }
    turnState--;

    //cout << "End of GameState::prevTurnState().\n" << std::flush;
    return result;
}

int GameState::endTurn()
{
    turnState = 6;
    return turnStatePostTurn();
}

int GameState::turnStatePreTurn()
{
    //cout << "Beginning of GameState::turnStatePreTurn().\n" << std::flush;
    if (turnState != 0)
        return 10;

    // Decrement the turns remaining on any Debuffs on this Servant. Remove any
    // Debuffs as necessary.
    currentServant->decDebuffs();

    // Apply any relevant Debuffs, either that are on the Servant or the field.
    // TODO: check if the debuff is a Dimensional Gate territory and do things
    //       accordingly. Also check for the various Reality Marble effects.
    Debuff *tDebuff = field->getDebuffOnSpace(currentServant->getCurrLoc());
    if (tDebuff != NULL && tDebuff->getTargetTeam() == currentServant->getTeam())
    {
        // Check if the Debuff is a special Territory or a Reality Marble and
        // act on the current servant accordingly. Otherwise, just add the
        // debuff to the servant's debuff list with a turns remaining value of
        // 1.
        // TODO

        Debuff *newDebuff = new Debuff(tDebuff->getDebuffName(),
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

    // Get the valid moves for the servant
    validMoves = getValidMoves(currentServant, currentServant->getMov());

    // Display a list of possible actions
    actionList = currentServant->getActionList();
    actionListTypes = currentServant->getActionListTypes();
    actionMPCosts = currentServant->getActionMPCosts();

    turnState++;
    //cout << "End of GameState::turnStatePreTurn().\n" << std::flush;
    return 0;
}

int GameState::turnStateMove()
{
    //cout << "Beginning of GameState::turnStateMove().\n" << std::flush;
    if (turnState != 1)
        return 20;

    // Verify that the clicked space is valid
    bool isValid = false;
    for (int i = 0; i < validMoves.size() && !isValid; i++)
    {
        if (validMoves[i].x == clickedX && validMoves[i].y == clickedY)
            isValid = true;
    }
    if (!isValid)
    {
        // The selected space was not valid!
        return 11;
    }

    servEnd.x = clickedX;
    servEnd.y = clickedY;

    // Move the player on the Playing Field
    servStart = field->moveServant(currentServant, servEnd);

    remainingMove = currentServant->getMov() -
            (abs(servEnd.x - servStart.x) + abs(servEnd.y - servStart.y));

    turnState++;
    //cout << "End of GameState::turnStateMove().\n" << std::flush;
    return 0;
}

int GameState::turnStateChoseAction()
{
    //cout << "Beginning of GameState::turnStateChoseAction().\n" << std::flush;
    if (turnState != 2)
        return 30;

    // Ensure that an action was actually chosen
    if (chosenAction < 0 || chosenAction >= actionList.size())
        return 21;

    // Verify that the player has enough MP
    int mpCost = currentServant->getActionMPCost(chosenAction);
    /***** TODO *****/
    // leave this the wrong way while testing, but inequality MUST BE FLIPPED
    // for function to work properly
    if (mpCost > currentServant->getCurrMP())
    {
        chosenAction = -1;
        return 22;
    }

    chosenActionType = currentServant->getActionType(chosenAction);

    // If the chosen action is single-target, get the range and have the player
    // select a target from within that range.
    if (chosenActionType == S)
    {
        selectionRange = currentServant->getActionRange(chosenAction);
        selectionRange = field->pruneRange(selectionRange, currentServant);
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
        //cout << "End of GameState::turnStateChoseAction() (1).\n" << std::flush;
        return turnStateApplyAction();
    }

    // If the action is targetting a dead servant, give the player a list of
    // dead servants to select from.
    else if (chosenActionType == D)
    {
        // TODO
    }

    // If the action does not have any targets then process it accordingly.
    else
    {
        turnState = 4;
        //cout << "End of GameState::turnStateChoseAction() (2).\n" << std::flush;
        return turnStateApplyAction();
    }

    turnState++;
    return 0;
}

int GameState::turnStateChoseTargets()
{
    cout << "Beginning of GameState::turnStateChoseTargets().\n" << std::flush;
    if (turnState != 3)
        return 40;

    // TODO:
    // ASK THE PLAYER IF THEY ARE SURE THEY WANT TO TO THIS ACTION (also show
    //    relevant stats like chance of hitting, how much damage would be done
    //    or received, etc.)
    // Ensure that the chosen targets are valid (?) and then call the apply
    // action turn state
    if (chosenActionType == S)
    {
        // Ensure that the target space is within the selectionRange and has
        // a servant; form a defender vector from the target servant
        int servID = -1;
        Coordinate target;
        Coordinate cLoc = currentServant->getCurrLoc();
        for (int i = 0; i < selectionRange.size() && servID == -1; i++)
        {
            if (selectionRange[i].x + cLoc.x == clickedX &&
                selectionRange[i].y + cLoc.y == clickedY)
            {
                cout << "in here?\n" << std::flush;
                servID = i;
                target.x = clickedX;
                target.y = clickedY;
            }
        }
        if (servID == -1) // not a valid selection!
        {
            return 31;
        }

        chosenDefenders.push_back(field->getServantOnSpace(target));
    }
    else if (chosenActionType == A)
    {
        // Ensure the target space is within the selectionRange
        bool found = false;
        Coordinate cLoc = currentServant->getCurrLoc();
        for (int i = 0; i < selectionRange.size() && !found; i++)
        {
            if (selectionRange[i].x + cLoc.x == clickedX &&
                selectionRange[i].y + cLoc.y == clickedY)
            {
                found = true;
                switch(i)
                {
                    case 0:
                        chosenDirection = NORTH;
                        break;
                    case 1:
                        chosenDirection = SOUTH;
                        break;
                    case 2:
                        chosenDirection = EAST;
                        break;
                    case 3:
                        chosenDirection = WEST;
                        break;
                    default:
                        // we should never reach here!
                        return 32;
                        break;
                }
            }
        }
        if (!found) // not a valid selection!
        {
            return 31;
        }

        // Calculate the true selectionRange based on the direction
        selectionRange = currentServant->getActionRange(chosenAction);
        for (int i = 0; i < selectionRange.size(); i++)
        {
            int tempX = 0;
            switch(chosenDirection)
            {
            case NORTH: // All ranges are defined in the NORTH direction, so
                        // no need to do anything
                break;
            case SOUTH: // i.e. rotate 180 degrees
                selectionRange[i].x *= -1;
                selectionRange[i].y *= -1;
                break;
            case EAST: // i.e. rotate 90 degrees clockwise
                tempX = selectionRange[i].x;
                selectionRange[i].x = selectionRange[i].y;
                selectionRange[i].y = tempX * -1;
                break;
            case WEST: // i.e. rotate 90 degrees counterclockwise
                tempX = selectionRange[i].x;
                selectionRange[i].x = selectionRange[i].y * -1;
                selectionRange[i].y = tempX;
                break;
            default:
                // We should never reach here!
                return 33;
            }
        }

        // get all Servants in that range and form a defender vector from that
        // list
        chosenDefenders = field->getAllInRange(currentServant, selectionRange);
    }
    else if (chosenActionType == D)
    {
        // TODO
        // Verify the selection

        // form a defender vector from the selection
    }
    else
    {
        // Why are we here?? We should never be here.
        return 34;
    }

    turnState++;
    cout << "End of GameState::turnStateChoseTargets().\n" << std::flush;
    return turnStateApplyAction();
}

int GameState::turnStateApplyAction()
{
    cout << "Beginning of GameState::turnStateApplyAction().\n" << std::flush;
    if (turnState != 4)
        return 50;

    // Apply the chosen action to the chosen targets.
    int ret = currentServant->doAction(chosenAction, chosenDefenders);
    if (ret != 0)
    {
        // Something went wrong! Return now.
        cout << "here??\n" << std::flush;
        cout << "Action: " << chosenAction << "\n" << std::flush;
        cout << "Result: " << ret << "\n" << std::flush;
        return ret;
    }

    // If the Servant is a Rider, allow them to move again.
    // TODO
    if (currentServant->getClass() == Rider)
    {
        // Get the valid moves for the servant
        validMoves = getValidMoves(currentServant, remainingMove);
        turnState++;
        cout << "End of GameState::turnStateApplyAction() (1).\n" << std::flush;
        return 0;
    }

    // If the Servant is an Archer, check against their LUK if they get a
    // second turn. If they do, set archerSecondTurn to true and go back to
    // the beginning move turn state. If archerSecondTurn is already true then
    // end the Servant's turn.
    // TODO
    else if (currentServant->getClass() == Archer && !archerSecondTurn)
    {
        int r = currentServant->getRandNum();
        if (r <= currentServant->getLuk())
        {
            archerSecondTurn = true;
            turnState = 0;
            validMoves = getValidMoves(currentServant, currentServant->getMov());
        }
        turnState++;
        cout << "End of GameState::turnStateApplyAction() (2).\n" << std::flush;
        return 0;
    }

    // End the non-Rider turn by calling the Post-turn turn state.
    else
    {
        turnState += 2;
        cout << "End of GameState::turnStateApplyAction() (3).\n" << std::flush;
        return turnStatePostTurn();
    }
}

int GameState::turnStateExtraMove()
{
    //cout << "Beginning of GameState::turnStateExtraMove().\n" << std::flush;
    if (turnState != 5)
        return 60;

    // Verify that the clicked space is valid
    bool isValid = false;
    for (int i = 0; i < validMoves.size() && !isValid; i++)
    {
        if (validMoves[i].x == clickedX && validMoves[i].y == clickedY)
            isValid = true;
    }
    if (!isValid)
    {
        // The selected space was not valid!
        return 51;
    }

    servEnd.x = clickedX;
    servEnd.y = clickedY;

    // Move the player on the Playing Field
    servStart = field->moveServant(currentServant, servEnd);

    // End the turn by calling the Post-turn turn state.
    turnState++;
    //cout << "End of GameState::turnStateExtraMove().\n" << std::flush;
    return turnStatePostTurn();
}

// If this function returns 1000, then team Alpha is dead. If it returns 1001,
// team Omegaa is dead. If it returns 1002, team Boss is dead.
int GameState::turnStatePostTurn()
{
    cout << "Beginning of GameState::turnStatePostTurn().\n" << std::flush;
    if (turnState != 6)
        return 70;

    addToEventLog("Turn ended.");
    int returnValue = 0;

    // Check if anyone has died and modify the death list accordingly.
    for (int i = 0; i < turnOrder.size(); i++)
    {
        if (turnOrder[i]->getCurrHP() <= 0)
        {
            addDead(turnOrder[i]);
        }
    }

    // Check if any of the teams have died.
    for (int i = 0; i < activeTeams.size(); i++)
    {
        if (isTeamDead(activeTeams[i]))
        {
            if (activeTeams[i] == Alpha)
                returnValue = 1000;
            else if (activeTeams[i] == Omega)
                returnValue = 1001;
            else
                returnValue = 1002;
        }
    }

    // Reset the turnState and all relevant variables and get the next Servant.
    resetTurnValues();

    currentServant = getNextServant();

    // Automatically execute turnStatePreTurn() if returnValue is not 70, 1000,
    // 1001, or 1002
    if (returnValue != 70 && returnValue != 1000 && returnValue != 1001 &&
            returnValue != 1002)
    {
        returnValue = turnStatePreTurn();
    }

    cout << "End of GameState::turnStatePostTurn().\n" << std::flush;
    return returnValue;
}

void GameState::resetTurnValues()
{
    //cout << "Beginning of GameState::resetTurnValues().\n" << std::flush;
    turnState = 0;
    clickedX = clickedY = 0;
    servStart.x = servStart.y = servEnd.x = servEnd.y = 0;
    chosenAction = 0;
    chosenActionType = S;
    chosenDirection = NORTH;
    remainingMove = 0;
    archerSecondTurn = false;

    chosenDefenders.clear();
    selectionRange.clear();
    validMoves.clear();
    actionList.clear();
    actionListTypes.clear();
    actionMPCosts.clear();

    //cout << "End of GameState::resetTurnValues().\n" << std::flush;
}

vector<string> GameState::getEventLog()
{
    return eventLog;
}

void GameState::addToEventLog(string s)
{
    string num = to_string(eventLog.size() + 1) + ": ";
    eventLog.push_back(num + s);
}
