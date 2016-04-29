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
        while (i < moves.size())
        {
            if (moveLocation.x == moves[i].x && moveLocation.y == moves[i].y)
                i++;
            else
                moves.erase(moves.begin()+i);
        }
    }

    return moves;
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
        for (int i = 0; i < o.size(); i++)
            enemies.push_back(o[i]);
        for (int i = 0; i < b.size(); i++)
            enemies.push_back(b[i]);
    }
    else if (s->getTeam() == Omega)
    {
        for (int i = 0; i < a.size(); i++)
            enemies.push_back(a[i]);
        for (int i = 0; i < b.size(); i++)
            enemies.push_back(b[i]);
    }
    else
    {
        for (int i = 0; i < o.size(); i++)
            enemies.push_back(o[i]);
        for (int i = 0; i < a.size(); i++)
            enemies.push_back(a[i]);
    }

    return enemies;
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
            turnStatePreTurn();
            break;
        case 1:
            turnStateMove();
            break;
        case 2:
            turnStateChoseAction();
            break;
        case 3:
            turnStateChoseTargets();
            break;
        case 4:
            turnStateApplyAction();
            break;
        case 5:
            turnStateExtraMove();
            break;
        case 6:
            turnStatePostTurn();
            break;
        default:
            // Shouldn't ever get here...
            break;
    }
    return turnState;
}

int GameState::prevTurnState()
{
    switch(turnState)
    {
        case 2:
            turnState--;
            field->moveServant(currentServant, servStart);
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
            // Shouldn't ever get here...
            break;
    }

    return turnState;
}

int GameState::turnStatePreTurn()
{
    // Decrement the turns remaining on any Debuffs on this Servant. Remove any
    // Debuffs as necessary.
    currentServant->decDebuffs();

    // Apply any relevant Debuffs, either that are on the Servant or the field.
    // TODO: check if the debuff is a Dimensional Gate territory and do things
    //       accordingly. Also check for the various Reality Marble effects.
    Debuff *tDebuff = field->getDebuffOnSpace(currentServant->getCurrLoc());
    if (tDebuff->getTargetTeam() == currentServant->getTeam())
    {
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

    turnState++;
    return 0;
}

int GameState::turnStateMove()
{
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
        return 1;
    }

    servEnd.x = clickedX;
    servEnd.y = clickedY;

    // Move the player on the Playing Field
    servStart = field->moveServant(currentServant, servEnd);

    remainingMove = currentServant->getMov() -
            (abs(servEnd.x - servStart.x) + abs(servEnd.y - servStart.y));

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

    // Verify that the player has enough MP
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
    else if (chosenActionType == D)
    {
        // TODO
    }

    // If the action does not have any targets then process it accordingly.
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
    if (chosenActionType == S)
    {
        // Ensure that the target space is within the selectionRange and has
        // a servant; form a defender vector from the target servant
        int servID = -1;
        Coordinate target;
        for (int i = 0; i < selectionRange.size() && servID == -1; i++)
        {
            if (selectionRange[i].x == clickedX &&
                selectionRange[i].y == clickedY)
            {
                servID = i;
                target.x = clickedX;
                target.y = clickedY;
            }
        }
        if (servID == -1) // not a valid selection!
        {
            return 1;
        }

        chosenDefenders.push_back(field->getServantOnSpace(target));
    }
    else if (chosenActionType == A)
    {
        // Ensure the target space is within the selectionRange
        bool found = false;
        for (int i = 0; i < selectionRange.size() && !found; i++)
        {
            if (selectionRange[i].x == clickedX &&
                selectionRange[i].y == clickedY)
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
                        return 2;
                        break;
                }
            }
        }
        if (!found) // not a valid selection!
        {
            return 1;
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
                return 3;
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
        return 666;
    }

    turnState++;
    turnStateApplyAction();
    return 0;
}

int GameState::turnStateApplyAction()
{
    // Apply the chosen action to the chosen targets.
    int ret = currentServant->doAction(chosenAction, chosenDefenders);
    if (ret != 0)
    {
        // Something went wrong! Return now.
        return ret;
    }

    // If the Servant is a Rider, allow them to move again.
    // TODO
    if (currentServant->getClass() == Rider)
    {
        // Get the valid moves for the servant
        validMoves = getValidMoves(currentServant, remainingMove);
        turnState++;
        return 0;
    }

    // If the Servant is an Archer, check against their LUK if they get a
    // second turn. If they do, set archerSecondTurn to true and go back to
    // the beginning move turn state. If archerSecondTurn is already true then
    // end the Servant's turn.
    // TODO
    else if (currentServant->getClass() == Archer && !archerSecondTurn)
    {
        //
        int r = currentServant->getRandNum();
        if (r <= currentServant->getLuk())
        {
            archerSecondTurn = true;
            turnState = 0;
            validMoves = getValidMoves(currentServant, currentServant->getMov());
        }
        turnState++;
        return 0;
    }

    // End the non-Rider turn by calling the Post-turn turn state.
    else
    {
        turnState += 2;
        turnStatePostTurn();
        return 0;
    }
}

int GameState::turnStateExtraMove()
{
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
        return 1;
    }

    servEnd.x = clickedX;
    servEnd.y = clickedY;

    // Move the player on the Playing Field
    servStart = field->moveServant(currentServant, servEnd);

    // TODO: Display it somehow?? Have a gui method parse the action list?

    // End the turn by calling the Post-turn turn state.
    turnState++;
    turnStatePostTurn();
    return 0;
}

// If this function returns 1000, then team Alpha is dead. If it returns 1001,
// team Omegaa is dead. If it returns 1002, team Boss is dead.
int GameState::turnStatePostTurn()
{
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

    return returnValue;
}

void GameState::resetTurnValues()
{
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
}
