/* Class file for the Game State.
 *
 * Contains the implementation of certain functions and data structures that are
 * required to track the state of the game, such as which Servants are dead,
 * turn order, what teams the Servants are on, etc.
 *
 */

#pragma once

#include "GameState.h"
#include "Servants/servantsaber.h"
#include "Servants/servantavenger.h"
#include "Servants/servantberserker.h"
#include "Servants/bossrin.h"
#include <cmath>
#include <iostream>

#include <QMessageBox>
#include <QObject>
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>

using namespace std;

/********** Function Definitions **********/
GameState::GameState(vector<Servant *> tO, int l, int w, Logger *lo) : QWidget()
{
    // Get the turn order (based on speed)
    while (tO.size() > 0)
    {
        int highestSpeed = 0;
        Servant *fastest = NULL;
        int index = 0;
        for (unsigned int i = 0; i < tO.size(); i++)
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
    for (unsigned int i = 0; i < turnOrder.size(); i++)
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

    log = lo;
    archerSecondTurn = false;
    ionioiSecondTurn = false;

    field = new PlayField(l, w, turnOrder, servantLocations);

    for (unsigned int i = 0; i < turnOrder.size(); i++)
    {
        turnOrder[i]->setPlayField(field);

        // If the Servant is an Avenger, we need to pass them the team
        if (turnOrder[i]->getClass() == Avenger)
        {
            ServantAvenger *thisAvenger = dynamic_cast<ServantAvenger*>(turnOrder[i]);
            if (thisAvenger->getTeam() == Alpha)
                thisAvenger->setTeam(alphaTeam);
            else if (thisAvenger->getTeam() == Omega)
                thisAvenger->setTeam(omegaTeam);
            else
                thisAvenger->setTeam(bossTeam);
        }
    }


    resetTurnValues();
    currentServant = getNextServant();
    turnStatePreTurn();
}

/***** Slots *****/
void GameState::processDeathComboBox(int index)
{
    deathComboBoxIndex = index;
}

/***** Manipulators *****/
void GameState::addDead(Servant *s)
{
    log->addToEventLog(s->getFullName() + " died!");

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
    bool servFound = false;
    for (unsigned int i = 0; i < dead.size() && !servFound; i++)
    {
        if (dead[i] == s)
        {
            servFound = true;
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
    bool servFound = false;
    for (unsigned int i = 0; i < dead.size() && !servFound; i++)
    {
        if (dead[i] == s)
            servFound = true;
    }

    return servFound;
}

bool GameState::isTeamDead(Team t)
{
    int aDead = 0;
    int oDead = 0;
    int bDead = 0;

    for (unsigned int i = 0; i < dead.size(); i++)
    {
        if (dead[i]->getTeam() == Alpha)
            aDead++;
        else if (dead[i]->getTeam() == Omega)
            oDead++;
        else
            bDead++;
    }

    if (t == Alpha)
        return (aDead == (int) alphaTeam.size());
    else if (t == Omega)
        return (oDead == (int) omegaTeam.size());
    else
        return (bDead == (int) bossTeam.size());
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
    for (unsigned int i = 0; i < selectionRange.size() && !isSelection; i++)
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
    for (unsigned int i = 0; i < validMoves.size() && !isMove; i++)
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
        return All;
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
    for (unsigned int i = 0; i < turnOrder.size() && !foundAlive; i++)
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
    return getSpaceInRange(c, range, getClosest, 1);
}

Coordinate GameState::getSpaceInRange(Coordinate c, vector<Coordinate> range, bool getClosest, int r)
{
    int minDist = 1000;
    Coordinate retLoc;
    retLoc.x = retLoc.y = -1;
    bool found = false;

    for (unsigned int i = 0; i < range.size() && !found; i++)
    {
        int dist = abs(range[i].x - c.x) + abs(range[i].y - c.y);

        if (dist == r)
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
            for (unsigned int j = 0; j < tempMoves.size(); j++)
            {
                bool alreadyIn = false;
                for (unsigned int k = 0; k < moves.size() && !alreadyIn; k++)
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
    for (unsigned int i = 0; i < tempDebuffs.size() && !isProvoked; i++)
    {
        if (tempDebuffs[i]->getDebuffName().compare("Provoked") == 0)
            isProvoked = true;
    }

    if (isProvoked) //unit is provoked by opponent saber
    {
        // Get the location of the enemy saber
        vector<Servant*> enemies = getOpposingTeamAlive(s->getTeam());
        Coordinate saberLoc;
        bool foundSaber = false;
        for (unsigned int i = 0; i < enemies.size() && !foundSaber; i++)
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
            Coordinate moveLocation = getSpaceInRange(saberLoc, moves, true,
                                                      currentServant->getLowRange());

            moves.clear();
            moves.push_back(moveLocation);
        }
    }
    else if ((s->getClass() == Berserker && !s->isGodmindActive()) ||
             s->isBerserk())
    {
        // Find closest enemy unit, then locate closest tile to said unit
        vector<Servant*> enemies = getOpposingTeamAlive(s->getTeam());

        int minDist = 1000;
        Coordinate closeLoc;

        // Locate the nearest enemy unit
        for (unsigned int i = 0; i < enemies.size(); i++)
        {
            Coordinate enLoc = enemies[i]->getCurrLoc();
            int dist = abs(enLoc.x - baseLoc.x) + abs(enLoc.y - baseLoc.y);
            if (dist < minDist)
            {
                minDist = dist;
                closeLoc = enLoc;
            }
        }

        // Get a coordinate either adjacent to the nearest servant, or closest
        // to the nearest servant.
        Coordinate moveLocation = getAdjacentInRange(closeLoc, moves, true);

        // Remove all moves that aren't the "closest location" that we just
        // found
        moves.clear();
        moves.push_back(moveLocation);
    }

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

vector<Servant*> GameState::getTeamDead(Team t)
{
    vector<Servant*> res;
    for (unsigned int i = 0; i < dead.size(); i++)
    {
        if(dead[i]->getTeam() == t)
        {
            res.push_back(dead[i]);
        }
    }

    return res;
}

vector<Servant*> GameState::getTeamAlive(Team t)
{
    vector<Servant*> res = getAlliedTeam(t);
    for (int j = 0; j < (int) res.size(); j++)
    {
        for (unsigned int i = 0; i < dead.size(); i++)
        {
            if(dead[i] == res[j])
            {
                res.erase(res.begin()+j);
                j--;
            }
        }
    }

    return res;
}

Servant* GameState::getTeamAliveServant(Team t, Class c)
{
    vector<Servant*> res = getTeamAlive(t);
    Servant *ret = NULL;
    for (unsigned int i = 0; i < res.size(); i++)
    {
        if (res[i]->getClass() == c)
        {
            ret = res[i];
        }
    }

    return ret;
}

vector<Servant*> GameState::getOpposingTeamDead(Team t)
{
    vector<Servant*> res;
    for (unsigned int i = 0; i < dead.size(); i++)
    {
        if(dead[i]->getTeam() != t)
        {
            res.push_back(dead[i]);
        }
    }

    return res;
}

vector<Servant*> GameState::getOpposingTeamAlive(Team t)
{
    vector<Servant*> res = getEnemyTeam(t);
    for (int j = 0; j < (int) res.size(); j++)
    {
        for (unsigned int i = 0; i < dead.size(); i++)
        {
            if(dead[i] == res[j])
            {
                res.erase(res.begin()+j);
                j--;
            }
        }
    }

    return res;
}

// Returns a list of servants who are not permadead.
vector<Servant*> GameState::filterPermaDead(vector<Servant *> d)
{
    vector<Servant*> res;
    for (unsigned int i = 0; i < d.size(); i++)
    {
        if (!d[i]->isPermaDead())
        {
            res.push_back(d[i]);
        }
    }

    return res;
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

vector<Servant*> GameState::getAlliedTeam(Team t)
{
    if (t == Alpha)
        return getAlphaTeam();
    else if (t == Omega)
        return getOmegaTeam();
    else
        return getBossTeam();
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

// Overloaded to work with team
vector<Servant*> GameState::getEnemyTeam(Team t)
{
    vector<Servant*> enemies;
    vector<Servant*> a = getAlphaTeam();
    vector<Servant*> o = getOmegaTeam();
    vector<Servant*> b = getBossTeam();
    if (t == Alpha)
    {
        for (unsigned int i = 0; i < o.size(); i++)
            enemies.push_back(o[i]);
        for (unsigned int i = 0; i < b.size(); i++)
            enemies.push_back(b[i]);
    }
    else if (t == Omega)
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
            result = endTurnProcess();
            break;
        default:
            // Shouldn't ever get here...
            log->addToErrorLog("Invalid turn state (GameState::nextTurnState())");
            break;
    }

    return result;
}

int GameState::prevTurnState()
{
    int result = 0;

    if (turnState == 2)
    {
        field->moveServant(currentServant, servStart);
    }
    else if (turnState < 2 || turnState > 6)
    {
        log->addToErrorLog("Invalid turn state (GameState::prevTurnState())");
        return 999;
    }
    turnState--;

    return result;
}

int GameState::endTurn()
{
    // If the player has not chosen an action, then recharge some of their
    // MP. (5% of max)
    if (turnState < 4)
    {
        currentServant->addMP(currentServant->getMaxMP() / 20);
    }
    turnState = 6;
    return turnStatePostTurn();
}

int GameState::turnStatePreTurn()
{
    if (turnState != 0)
        return 10;

    // Decrement the turns remaining on any Debuffs on this Servant. Remove any
    // Debuffs as necessary.
    currentServant->decDebuffs();
    log->addEventStartTurn(currentServant->getTeamName(),
                           currentServant->getName());

    // If the servant is an Avenger, update their stats for Avenger's Rage.
    // This only occurs at the beginning of their turn for consistency's sake.
    if (currentServant->getClass() == Avenger)
    {
        ServantAvenger *currServ = dynamic_cast<ServantAvenger*>(currentServant);
        currServ->updateAvengersRage();
    }

    // If the current servant is Rin Tohsaka, run her turnUpdate method for her
    // willpower skill
    if (currentServant->getName().compare("Rin Tohsaka") == 0)
    {
        BossRin *currServ = dynamic_cast<BossRin*>(currentServant);
        currServ->turnUpdate();
    }

    // Check for a Doom debuff. If turns remaining is 1, do the checks. If they
    //  pass, print to the event log. Otherwise, kill them and skip to the
    //  postTurn state.
    vector<Debuff*> currDebuffs = currentServant->getDebuffs();
    for (unsigned int i = 0; i < currDebuffs.size(); i++)
    {
        if (currDebuffs[i]->getDebuffName().compare("Doom") == 0 &&
                currDebuffs[i]->getTurnsRemaining() == 1)
        {
            int r = currentServant->getRandNum();
            if (r > currentServant->getLuk())
            {
                // They failed the check, kill them and add a Permadeath debuff
                vector<Stat> tDebStat;
                tDebStat.push_back(MOV);
                vector<int> tDebAm;
                tDebAm.push_back(0);
                Debuff *newDebuff = new Debuff("Permadeath",
                                               "You are permanently dead.",
                                               currentServant->getTeam(),
                                               tDebStat, tDebAm, -1);
                currentServant->addDebuff(newDebuff);
                currentServant->setHP(0);
                log->addToEventLog(currentServant->getFullName() +
                                   " succombed to the effect of Doom and died permanently!");
                turnState = 6;
                return turnStatePostTurn();
            }
            else
            {
                // They passed the check, say so in the event log
                log->addToEventLog(currentServant->getFullName() +
                                   " survived Doom!");
                currentServant->removeDoom();
            }
        }
    }

    // Apply any relevant Debuffs, either that are on the Servant or the field.
    Debuff *tDebuff = field->getDebuffOnSpace(currentServant->getCurrLoc());
    if (tDebuff != NULL && (tDebuff->getTargetTeam() == currentServant->getTeam()
                            || tDebuff->getTargetTeam() == All))
    {
        // Check if the Debuff is a special Territory or a Reality Marble and
        // act on the current servant accordingly. Otherwise, just add the
        // debuff to the servant's debuff list with a turns remaining value of
        // 1.

        // This is for when the Servant is an enemy of the Dimensional Gate
        //  caster.
        if(tDebuff->getDebuffDescrip().compare("Dimensional Gate") == 0)
        {
            // Servant is teleported to random location on the field
            Coordinate teleCoord;
            teleCoord.x = -1;
            teleCoord.y = -1;
            while (!field->isValidCoordinate(teleCoord))
            {
                teleCoord.x = rand() % field->getFieldWidth();
                teleCoord.y = rand() % field->getFieldLength();
            }
            field->moveServant(currentServant, teleCoord);
            log->addToEventLog("The Dimensional Gate teleported you to (" +
                               to_string(teleCoord.x) + "," +
                               to_string(teleCoord.y) + ")!");
        }
        else if (tDebuff->getDebuffDescrip().compare("Castle Doctrine") == 0)
        {
            // Servant takes "normal" damage at the beginning of their turn.
            //  (attack is subject to accuracy and critical check)
            vector<Servant*> tDef;
            tDef.push_back(currentServant);
            field->realityMarbleServant()->attack(tDef, false);
        }
        else if (tDebuff->getDebuffDescrip().compare("Commander of Life and Death") == 0)
        {
            int hpLoss = currentServant->getMaxHP() / 10;
            if (hpLoss >= currentServant->getCurrHP())
                hpLoss = currentServant->getCurrHP() - 1;

            currentServant->subHP(hpLoss, NP_MAG);

            // Add a permadeath debuff to the servant
            vector<Stat> tDebStat;
            tDebStat.push_back(MOV);
            vector<int> tDebAm;
            tDebAm.push_back(0);
            Debuff *newDebuff = new Debuff("Cursed",
                                           "If you die while this debuff is active, you cannot be ressurected.",
                                           currentServant->getTeam(),
                                           tDebStat, tDebAm, 1);
            currentServant->addDebuff(newDebuff);
        }
        else
        {
            Debuff *newDebuff = new Debuff(tDebuff->getDebuffName(),
                                           tDebuff->getDebuffDescrip(),
                                           tDebuff->getTargetTeam(),
                                           tDebuff->getDebuffStats(),
                                           tDebuff->getDebuffAmounts(), 1);
            currentServant->addDebuff(newDebuff);
        }

    }
    // This is for when the Servant is an ally of the Dimensional Gate caster.
    else if (tDebuff != NULL &&
             tDebuff->getDebuffDescrip().compare("Dimensional Gate") == 0)
    {
        // Servant gets a 50% bonus to MOV.
        // (Just apply the debuff, which in this case is actually a buff).
        vector<Stat> tDebStat;
        tDebStat.push_back(MOV);
        vector<int> tDebAm;
        tDebAm.push_back(currentServant->getMov() / 2);
        Debuff *newDebuff = new Debuff(tDebuff->getDebuffName(),
                                       tDebuff->getDebuffDescrip(),
                                       tDebuff->getTargetTeam(),
                                       tDebStat, tDebAm, 1);
        currentServant->addDebuff(newDebuff);
    }
    // This if for when the Servant is an ally of the Commander of Life and
    // Death caster.
    else if (tDebuff != NULL &&
             tDebuff->getDebuffDescrip().compare("Commander of Life and Death") == 0)
    {
        int hpLoss = currentServant->getMaxHP() / 20;
        if (hpLoss >= currentServant->getCurrHP())
            hpLoss = currentServant->getCurrHP() - 1;

        currentServant->subHP(hpLoss, NP_MAG);

        // Add a permadeath debuff to the servant
        vector<Stat> tDebStat;
        tDebStat.push_back(MOV);
        vector<int> tDebAm;
        tDebAm.push_back(0);
        Debuff *newDebuff = new Debuff("Cursed",
                                       "If you die while this debuff is active, you cannot be ressurected.",
                                       currentServant->getTeam(),
                                       tDebStat, tDebAm, 1);
        currentServant->addDebuff(newDebuff);
    }
    /*else if (tDebuff != NULL)
    {
        Debuff *newDebuff = new Debuff(tDebuff->getDebuffName(),
                                       tDebuff->getDebuffDescrip(),
                                       tDebuff->getTargetTeam(),
                                       tDebuff->getDebuffStats(),
                                       tDebuff->getDebuffAmounts(), 1);
        currentServant->addDebuff(newDebuff);
    }*/

    // If the player is standing within Charisma range of their team's Servant,
    // give them the Charisma buff (for 1 turn).
    Servant *alliedSaber = getTeamAliveServant(currentServant->getTeam(), Saber);
    if (alliedSaber != NULL && alliedSaber != currentServant)
    {
        ServantSaber *alliedSaberC = dynamic_cast<ServantSaber*>(alliedSaber);
        if (abs(currentServant->getCurrLoc().x - alliedSaberC->getCurrLoc().x)
            + abs(currentServant->getCurrLoc().y - alliedSaberC->getCurrLoc().y)
            <= alliedSaberC->getCharismaRange())
        {
            currentServant->addDebuff(alliedSaberC->getCharisma());
        }
    }

    // If the player is a Berserker, check for the chance of activating Mad Roar
    // Also call turnUpdate to do any necessary processing
    if (currentServant->getClass() == Berserker)
    {
        int berLuk = currentServant->getLuk();
        int berSkl = currentServant->getSkl();
        ServantBerserker *currServ = dynamic_cast<ServantBerserker*>(currentServant);
        if(currentServant->getRandNum() <= (berLuk / 2) + (berSkl / 2))
        {
            log->addToEventLog(currentServant->getFullName() +
                               " let loose a Mad Roar!");
            currServ->madRoar(getOpposingTeamAlive(currServ->getTeam()));
        }
        currServ->turnUpdate();
    }

    int hpSub = currentServant->getDebuffAmount(HP);
    int mpSub = currentServant->getDebuffAmount(MP);
    int currHP = currentServant->getCurrHP();

    if (currHP + hpSub <= 0)
        currentServant->setHP(1);
    else
        currentServant->addHP(hpSub);

    currentServant->addMP(mpSub);

    // Determine if there are any ongoing combat effects that belong to this
    // Servant (e.g. Reality Marbles or Caster Territories) and if the player
    // wants to continue them.
    if (field->isRealityMarbleOn() &&
        field->realityMarbleServant() == currentServant)
    {
        // Check that the player has enough MP to continue the reality marble.
        // If they don't, discontinue it automatically.
        if (currentServant->getCurrMP() < currentServant->getRealityMarbleMP())
        {
            log->addToEventLog("You lack the MP to continue your Reality Marble. It has now dissipated.");
            field->endRealityMarble();
        }
        else
        {
            // Pop up a dialog box confirming this action.
            // THIS HAS NOT YET BEEN TESTED.
            string check = "\n" + currentServant->getFullName() +
                           ":\nDo you wish to continue your Reality Marble for " +
                           to_string(currentServant->getRealityMarbleMP()) +
                           " MP?\nYou currently have " +
                           to_string(currentServant->getCurrMP()) + " / " +
                           to_string(currentServant->getMaxMP()) + " MP.";
            QMessageBox checkMessage;
            checkMessage.setWindowTitle(QObject::tr("Final Fate"));
            checkMessage.setText(QString::fromStdString(check));
            checkMessage.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            checkMessage.setDefaultButton(QMessageBox::No);
            if (checkMessage.exec() == QMessageBox::No)
            {
                log->addToEventLog("You chose to discontinue your Reality Marble. It has now dissipated.");
                field->endRealityMarble();
            }
            else
            {
                log->addToEventLog("You chose to continue your Reality Marble.");
                currentServant->subMP(currentServant->getRealityMarbleMP());
                // Check here for reality marble-specific effects
                if(field->getDebuffOnSpace(currentServant->getCurrLoc())->getDebuffDescrip().compare("Unlimited Blade Works") == 0)
                {
                    // UBW
                    log->addToEventLog("The blades of Unlimited Blade Works come to life and attack your foes!");
                    vector<Servant*> aliveServants = getEnemyTeam(currentServant);
                    int rawDamage = currentServant->getStr() + currentServant->getMag();
                    for (unsigned int i = 0; i < aliveServants.size(); i++)
                    {
                        Servant *tempTarget = aliveServants[i];
                        string tN = tempTarget->getTeamName() + " " + tempTarget->getName();
                        int defStat = 0;
                        if (tempTarget->getDef() > tempTarget->getRes())
                            defStat = tempTarget->getDef();
                        else
                            defStat = tempTarget->getRes();
                        int totalDamage = abs(rawDamage - defStat);

                        if(totalDamage >= tempTarget->getCurrHP())
                        {
                            tempTarget->subHP(totalDamage, NP_MAG);
                            log->addToEventLog(tN + " took " +
                                               to_string(totalDamage) +
                                               " damage and died!");
                            addDead(tempTarget);
                            field->servantDead(tempTarget);
                        }
                        else
                        {
                            tempTarget->subHP(totalDamage, NP_MAG);
                            log->addToEventLog(tN + " took " +
                                               to_string(totalDamage) +
                                               " damage!");
                        }
                    }
                }
                else if (field->getDebuffOnSpace(currentServant->getCurrLoc())->getDebuffDescrip().compare("Commander of Life and Death") == 0)
                {
                    // Commander of Life and Death (CLD)
                    // THIS DIALOG HAS NOT YET BEEN TESTED
                    QDialog *deathDialog = new QDialog;
                    QLabel *instructionLabel = new QLabel;
                    QComboBox *chooseServant = new QComboBox;

                    string instrucText = "Choose a player to cast Death on.\n(Note: if they pass a Luck check, then they will live.";
                    instructionLabel->setText(QString::fromStdString(instrucText));
                    instructionLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
                    instructionLabel->setAlignment(Qt::AlignCenter);

                    vector<Servant*> aliveServants = getEnemyTeam(currentServant);
                    for (unsigned int i = 0; i < dead.size(); i++)
                    {
                        bool f = false;
                        for (unsigned int j = 0; j < aliveServants.size() && !f; j++)
                        {
                            if(aliveServants[j] == dead[i])
                            {
                                f = true;
                                aliveServants.erase(aliveServants.begin() + j);
                            }
                        }
                    }

                    for (unsigned int i = 0; i < aliveServants.size(); i++)
                    {
                        chooseServant->addItem(QString::fromStdString(
                                                   aliveServants[i]->getTeamName() + " " +
                                                   aliveServants[i]->getName()));
                    }

                    QObject::connect(chooseServant, SIGNAL(activated(int)), this,
                                     SLOT(processDeathComboBox(int)));

                    QPushButton *okButton = new QPushButton(QWidget::tr("OK"));
                    //QPushButton *cancelButton = new QPushButton(QWidget::tr("Cancel"));
                    QObject::connect(okButton, SIGNAL(clicked()), deathDialog, SLOT(accept()));
                    //QObject::connect(cancelButton, SIGNAL(clicked()), deathDialog, SLOT(reject()));
                    QHBoxLayout *buttons = new QHBoxLayout;
                    buttons->addWidget(okButton);
                    //buttons->addWidget(cancelButton);

                    QVBoxLayout *finalLayout = new QVBoxLayout;
                    finalLayout->addWidget(instructionLabel);
                    finalLayout->addWidget(chooseServant);
                    finalLayout->addLayout(buttons);

                    deathDialog->setLayout(finalLayout);

                    // Display the dialog and get the result
                    deathComboBoxIndex = 0;
                    int result = deathDialog->exec();

                    if (result == QDialog::Rejected)
                    {
                        // We shouldn't ever be here
                        deathComboBoxIndex = 0;
                    }
                    else
                    {
                        Servant *tServ = aliveServants[deathComboBoxIndex];
                        log->addToEventLog("You cast Death on " +
                                           tServ->getTeamName() + " " +
                                           tServ->getName() + ".");

                        int randNum = tServ->getRandNum();
                        int tCurrHP = tServ->getCurrHP();
                        if (randNum > tServ->getLuk() * 2)
                        {
                            if (tCurrHP > 1)
                            {
                                log->addToEventLog(tServ->getTeamName() + " " +
                                                   tServ->getName() +
                                                   " was reduced to 1 HP!");
                                tServ->subHP(tCurrHP - 1, OMNI);
                            }
                            else
                            {
                                log->addToEventLog(tServ->getTeamName() + " " +
                                                   tServ->getName() +
                                                   " was killed!");
                                tServ->subHP(tCurrHP, OMNI);
                                addDead(tServ);
                                field->servantDead(tServ);
                            }
                        }
                        else
                        {
                            log->addToEventLog(tServ->getTeamName() + " " +
                                               tServ->getName() +
                                               " lucked out of taking damage!");
                        }
                    }

                    deathComboBoxIndex = 0;
                }
            }
        }
    }
    else if (currentServant->getClass() == Caster &&
             currentServant->isTerritoryActive())
    {
        // Check that the player has enough MP to continue the territory.
        // If they don't, discontinue it automatically.
        if (currentServant->getCurrMP() < currentServant->getTerritoryMP())
        {
            log->addToEventLog("You lack the MP to continue your Territory. It has now dissolved.");
            field->eraseTerritory(currentServant->getTerritoryName());
            currentServant->endTerritory();
        }
        else if (field->isRealityMarbleOn())
        {
            log->addToEventLog("The Reality Marble killed your Territory!");
            field->eraseTerritory(currentServant->getTerritoryName());
            currentServant->endTerritory();
        }
        else
        {
            // Pop up a dialog box confirming this action.
            // THIS HAS NOT YET BEEN TESTED.
            string check = "\n" + currentServant->getFullName() +
                           ":\nDo you wish to continue your Territory for " +
                           to_string(currentServant->getTerritoryMP()) +
                           " MP?\nYou currently have " +
                           to_string(currentServant->getCurrMP()) + " / " +
                           to_string(currentServant->getMaxMP()) + " MP.";
            QMessageBox checkMessage;
            checkMessage.setWindowTitle(QObject::tr("Final Fate"));
            checkMessage.setText(QString::fromStdString(check));
            checkMessage.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            checkMessage.setDefaultButton(QMessageBox::No);
            if (checkMessage.exec() == QMessageBox::No)
            {
                log->addToEventLog("You chose to discontinue your Territory. It has now dissolved.");
                field->eraseTerritory(currentServant->getTerritoryName());
                currentServant->endTerritory();
            }
            else
            {
                log->addToEventLog("You chose to continue your Territory.");
                currentServant->subMP(currentServant->getTerritoryMP());
            }
        }
    }

    // Get the valid moves for the servant
    validMoves = getValidMoves(currentServant, currentServant->getMov());

    // Display a list of possible actions
    actionList = currentServant->getActionList();
    actionListTypes = currentServant->getActionListTypes();
    actionMPCosts = currentServant->getActionMPCosts();

    servStart = currentServant->getCurrLoc();

    turnState++;
    return 0;
}

int GameState::turnStateMove()
{
    if (turnState != 1)
        return 20;

    // Verify that the clicked space is valid
    bool isValid = false;
    for (unsigned int i = 0; i < validMoves.size() && !isValid; i++)
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
    log->addToEventLog("Move from space (" + to_string(servStart.x) + "," +
                  to_string(servStart.y) + ")" + " to space (" +
                  to_string(servEnd.x) + "," + to_string(servEnd.y) + ").");
    return 0;
}

int GameState::turnStateChoseAction()
{
    if (turnState != 2 && turnState != 1)
        return 30;

    // Ensure that an action was actually chosen
    if (chosenAction < 0 || chosenAction >= (int) actionList.size())
        return 21;

    // Verify that the player has enough MP
    int mpCost = currentServant->getActionMPCost(chosenAction);
    if (mpCost > currentServant->getCurrMP())
    {
        chosenAction = -1;
        return 22;
    }

    chosenActionType = currentServant->getActionType(chosenAction);
    selectionRange.clear();

    // If the chosen action is single-target, get the range and have the player
    // select a target from within that range.
    if (chosenActionType == S)
    {
        selectionRange = currentServant->getActionRange(chosenAction);
        selectionRange = field->pruneRange(selectionRange, currentServant);
        log->addToEventLog("Choose target.");
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
        log->addToEventLog("AOE Attack: Choose direction.");
    }

    // If the action is territory creation, skip straight to the apply action
    // turn state and apply the action.
    else if (chosenActionType == T)
    {
        if (field->isRealityMarbleOn())
        {
            log->addToErrorLog("You cannot do that while a Reality Marble is active!");
            return 25;
        }

        // Pop up a dialog box confirming this action.
        // THIS HAS NOT YET BEEN TESTED.
        string check = "\nYou wish to use the action:\n\n" +
                       currentServant->getActionName(chosenAction) +
                       "\n\nAre you sure?\n";
        QMessageBox checkMessage;
        checkMessage.setWindowTitle(QObject::tr("Final Fate"));
        checkMessage.setText(QString::fromStdString(check));
        checkMessage.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        checkMessage.setDefaultButton(QMessageBox::No);
        if (checkMessage.exec() == QMessageBox::No)
            return 25;

        turnState = 4;
        log->addToEventLog("Chose action " +
                           currentServant->getActionName(chosenAction)
                           + ".");
        if (currentServant->isActionRealityMarble(chosenAction))
            chosenDefenders = getEnemyTeam(currentServant);
        else
            chosenDefenders = field->getAllInRange(currentServant,
                                   currentServant->getActionRange(chosenAction));

        turnState = 4;
        return turnStateApplyAction();
    }

    // If the action is targetting a dead servant, give the player a list of
    // dead servants to select from.
    else if (chosenActionType == D)
    {
        // Process immediately. No need for the extra turnstate.
        // THIS PART REMAINS UNTESTED
        deathComboBoxIndex = 0;
        vector<Servant*> targetList = getDead();
        //if (currentServant->getClass() == Caster &&
        //        currentServant->isKillDeadAction(chosenAction))
        //    targetList = getOpposingTeamDead(currentServant->getTeam());
        //else
        //    targetList = getTeamDead(currentServant->getTeam());

        // Count the number of permadead and regular-dead servants
        int numDead = 0;
        int numPermaDead = 0;
        string permDeadServ = "\nList of Permadead Players:\n\n";
        vector<Servant*> deadServ;
        for (unsigned int i = 0; i < targetList.size(); i++)
        {
            if (targetList[i]->isPermaDead())
            {
                numPermaDead++;
                permDeadServ += targetList[i]->getTeamName() + " " +
                                targetList[i]->getName() + "\n";
            }
            else
            {
                numDead++;
                deadServ.push_back(targetList[i]);
            }
        }

        // If there are no regular-dead servants then back out of the action
        // automatically
        if (numDead == 0 && numPermaDead == 0)
        {
            log->addToErrorLog("No dead Servants to use action on!");
            return 25;
        }
        else if (numDead == 0)
        {
            log->addToErrorLog("No non-permadead Servants to use action on!");
            return 25;
        }
        else if (numPermaDead == 0)
        {
            permDeadServ += "(None)\n";
        }

        // Create the dialog
        QDialog *deathDialog = new QDialog;
        QLabel *permaDeadServants = new QLabel;
        QLabel *instructionLabel = new QLabel;
        QComboBox *chooseServant = new QComboBox;

        string instrucText = "Choose a dead player to use " +
                             currentServant->getActionName(chosenAction) +
                             " on:";
        permaDeadServants->setText(QString::fromStdString(permDeadServ));
        permaDeadServants->setFrameStyle(QFrame::Box | QFrame::Sunken);
        permaDeadServants->setAlignment(Qt::AlignCenter);
        instructionLabel->setText(QString::fromStdString(instrucText));
        instructionLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
        instructionLabel->setAlignment(Qt::AlignCenter);

        for (unsigned int i = 0; i < deadServ.size(); i++)
        {
            chooseServant->addItem(QString::fromStdString(
                                       deadServ[i]->getTeamName() + " " +
                                       deadServ[i]->getName()));
        }

        QObject::connect(chooseServant, SIGNAL(activated(int)), this,
                         SLOT(processDeathComboBox(int)));

        QPushButton *okButton = new QPushButton(QWidget::tr("OK"));
        QPushButton *cancelButton = new QPushButton(QWidget::tr("Cancel"));
        QObject::connect(okButton, SIGNAL(clicked()), deathDialog, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), deathDialog, SLOT(reject()));
        QHBoxLayout *buttons = new QHBoxLayout;
        buttons->addWidget(okButton);
        buttons->addWidget(cancelButton);

        QVBoxLayout *finalLayout = new QVBoxLayout;
        finalLayout->addWidget(permaDeadServants);
        finalLayout->addWidget(instructionLabel);
        finalLayout->addWidget(chooseServant);
        finalLayout->addLayout(buttons);

        deathDialog->setLayout(finalLayout);

        // Display the dialog and get the result
        int result = deathDialog->exec();

        if (result == QDialog::Rejected)
        {
            deathComboBoxIndex = 0;
            return 25;
        }
        else
        {
            chosenDefenders.push_back(deadServ[deathComboBoxIndex]);
            deathComboBoxIndex = 0;
        }

        turnState = 4;
        log->addToEventLog("Chose action " +
                           currentServant->getActionName(chosenAction)
                           + ".");
        return turnStateApplyAction();
    }

    // If the action does not have any targets then process it accordingly.
    else
    {
        // Pop up a dialog box confirming this action.
        // THIS HAS NOT YET BEEN TESTED.
        string check = "\nYou wish to use the action:\n\n" +
                       currentServant->getActionName(chosenAction) +
                       "\n\nAre you sure?\n";
        QMessageBox checkMessage;
        checkMessage.setWindowTitle(QObject::tr("Final Fate"));
        checkMessage.setText(QString::fromStdString(check));
        checkMessage.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        checkMessage.setDefaultButton(QMessageBox::No);
        if (checkMessage.exec() == QMessageBox::No)
            return 25;

        turnState = 4;
        log->addToEventLog("Chose action " +
                           currentServant->getActionName(chosenAction)
                           + ".");
        return turnStateApplyAction();
    }

    turnState = 3;
    log->addToEventLog("Chose action " +
                       currentServant->getActionName(chosenAction)
                       + ".");
    return 0;
}

int GameState::turnStateChoseTargets()
{
    if (turnState != 3)
        return 40;

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
        Servant *targetServ;
        for (unsigned int i = 0; i < selectionRange.size() && servID == -1; i++)
        {
            if (selectionRange[i].x + cLoc.x == clickedX &&
                selectionRange[i].y + cLoc.y == clickedY)
            {
                servID = i;
                target.x = clickedX;
                target.y = clickedY;
            }
        }
        if (servID == -1) // not a valid selection!
        {
            return 31;
        }
        else
        {
            targetServ = field->getServantOnSpace(target);
            /* Show Dialog box allowing player to confirm the action */
            QDialog *atkDialog = new QDialog;
            QLabel *atkLabel = new QLabel;
            QLabel *atkServIcon = new QLabel;
            QLabel *atkServName = new QLabel;
            QLabel *atkHPLab = new QLabel;
            QLabel *atkMPLab = new QLabel;
            QLabel *atkServHP = new QLabel;
            QLabel *atkServMP = new QLabel;
            QLabel *atkWords = new QLabel;
            QLabel *atkStats = new QLabel;
            QLabel *vs = new QLabel;
            QLabel *defLabel = new QLabel;
            QLabel *defServIcon = new QLabel;
            QLabel *defServName = new QLabel;
            QLabel *defHPLab = new QLabel;
            QLabel *defMPLab = new QLabel;
            QLabel *defServHP = new QLabel;
            QLabel *defServMP = new QLabel;
            QLabel *defWords = new QLabel;
            QLabel *defStats = new QLabel;

            // Attacking Servant Labels
            atkLabel->setText("ATTACKER");
            atkServIcon->setPixmap(QPixmap(QString::fromStdString(currentServant->getServantIcon())));
            atkServName->setText(QString::fromStdString(currentServant->getTeamName() + " " + currentServant->getName()));
            atkHPLab->setText("HP:");
            atkServHP->setText(QString::fromStdString(to_string(currentServant->getCurrHP()) + " / " + to_string(currentServant->getMaxHP())));
            atkMPLab->setText("MP:");
            atkServMP->setText(QString::fromStdString(to_string(currentServant->getCurrMP()) + " / " + to_string(currentServant->getMaxMP())));
            bool healAction = currentServant->isHealAction(chosenAction);

            if (currentServant->getClass() == Caster && healAction)
                atkWords->setText("Heal\nHit\nCrit\nMP Cost");
            else
                atkWords->setText("Atk\nHit\nCrit\nMP Cost");

            int atkAttack = 0, atkHit = 0, atkCrit = 0;
            atkHit = capZero(currentServant->getHitRate() - targetServ->getEvade()[0]);
            atkCrit = capZero(currentServant->getCriticalRate() - targetServ->getCriticalEvade());
            int atkCost = currentServant->getActionMPCost(chosenAction);

            if (currentServant->getClass() == Caster && !healAction)
                atkAttack = currentServant->getMag() - targetServ->getRes();
            else if (currentServant->getClass() == Caster && healAction)
            {
                atkAttack = currentServant->getMag();
                atkHit = 100;
                atkCrit = 0;
            }
            else if (currentServant->getClass() == Assassin && currentServant->isPoisonAction(chosenAction))
                atkAttack = 0;
            else
                atkAttack = currentServant->getStr() - targetServ->getDef();
            if (atkCrit < 0)
                atkCrit = 0;

            if (!currentServant->getActionDodgeable(chosenAction))
                atkHit = 100;

            atkAttack = capZero(atkAttack);
            int res = currentServant->isActionNP(chosenAction);
            if (res != -1)
            {
                atkStats->setText(QString::fromStdString("Noble Phantasm: " +
                                                         currentServant->getNoblePhantasms()[res][0] + "\n"
                                                         + to_string(atkHit) + "%\n"
                                                         + to_string(atkCrit) + "\n"
                                                         + to_string(atkCost)));
            }
            else
            {
                atkStats->setText(QString::fromStdString(to_string(atkAttack) + "\n"
                                                         + to_string(atkHit) + "%\n"
                                                         + to_string(atkCrit) + "\n"
                                                         + to_string(atkCost)));
            }
            atkServName->setFrameStyle(QFrame::Box | QFrame::Sunken);
            atkWords->setFrameStyle(QFrame::Box | QFrame::Sunken);
            atkStats->setFrameStyle(QFrame::Box | QFrame::Sunken);
            atkLabel->setAlignment(Qt::AlignCenter);
            atkServIcon->setAlignment(Qt::AlignCenter);
            atkServName->setAlignment(Qt::AlignCenter);
            atkHPLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
            atkMPLab->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
            atkServHP->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            atkServMP->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            atkWords->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
            atkStats->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);


            // Defending Servant Labels
            defLabel->setText("DEFENDER");
            defServIcon->setPixmap(QPixmap(QString::fromStdString(targetServ->getServantIcon())));
            defServName->setText(QString::fromStdString(targetServ->getTeamName() + " " + targetServ->getName()));
            defHPLab->setText(":HP");
            defServHP->setText(QString::fromStdString(to_string(targetServ->getCurrHP()) + " / " + to_string(targetServ->getMaxHP())));
            defMPLab->setText(":MP");
            defServMP->setText(QString::fromStdString(to_string(targetServ->getCurrMP()) + " / " + to_string(targetServ->getMaxMP())));
            defWords->setText("Atk\nHit\nCrit\nMP Cost");
            if (targetServ->getCurrMP() >= targetServ->getActionMPCost(0) &&
                    targetServ->isInRange(currentServant) &&
                    currentServant->getActionCounterable(chosenAction) &&
                    !currentServant->isHealAction(chosenAction))
            {
                int defAttack = 0;
                int defCost = targetServ->getActionMPCost(0);
                if (targetServ->getClass() == Caster)
                    defAttack = targetServ->getMag() - currentServant->getRes();
                else
                    defAttack = targetServ->getStr() - currentServant->getDef();
                int defHit = targetServ->getHitRate() - currentServant->getEvade()[0];
                int defCrit = targetServ->getCriticalRate() - currentServant->getCriticalEvade();
                if (defCrit < 0)
                    defCrit = 0;
                defAttack = capZero(defAttack);
                defHit = capZero(defHit);
                defCrit = capZero(defCrit);
                defStats->setText(QString::fromStdString(to_string(defAttack) + "\n"
                                                         + to_string(defHit) + "%\n"
                                                         + to_string(defCrit) + "\n"
                                                         + to_string(defCost)));
            }
            else
                defStats->setText("-\n-\n-\n-");
            defServName->setFrameStyle(QFrame::Box | QFrame::Sunken);
            defWords->setFrameStyle(QFrame::Box | QFrame::Sunken);
            defStats->setFrameStyle(QFrame::Box | QFrame::Sunken);
            defLabel->setAlignment(Qt::AlignCenter);
            defServIcon->setAlignment(Qt::AlignCenter);
            defServName->setAlignment(Qt::AlignCenter);
            defHPLab->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            defMPLab->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            defServHP->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
            defServMP->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
            defWords->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            defStats->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

            vs->setText(" VS ");
            vs->setAlignment(Qt::AlignCenter);

            // Create the Layouts
            QHBoxLayout *atkAtkStats = new QHBoxLayout;
            atkAtkStats->addWidget(atkWords);
            atkAtkStats->addWidget(atkStats);
            QHBoxLayout *atkHPLay = new QHBoxLayout;
            atkHPLay->addWidget(atkHPLab);
            atkHPLay->addWidget(atkServHP);
            QHBoxLayout *atkMPLay = new QHBoxLayout;
            atkMPLay->addWidget(atkMPLab);
            atkMPLay->addWidget(atkServMP);
            QVBoxLayout *atkHPMPLay = new QVBoxLayout;
            atkHPMPLay->addLayout(atkHPLay);
            atkHPMPLay->addLayout(atkMPLay);
            QVBoxLayout *atkAllLayout = new QVBoxLayout;
            atkAllLayout->addWidget(atkLabel);
            atkAllLayout->addWidget(atkServIcon);
            atkAllLayout->addWidget(atkServName);
            atkAllLayout->addLayout(atkHPMPLay);
            atkAllLayout->addLayout(atkAtkStats);

            QHBoxLayout *defAtkStats = new QHBoxLayout;
            defAtkStats->addWidget(defStats);
            defAtkStats->addWidget(defWords);
            QHBoxLayout *defHPLay = new QHBoxLayout;
            defHPLay->addWidget(defServHP);
            defHPLay->addWidget(defHPLab);
            QHBoxLayout *defMPLay = new QHBoxLayout;
            defMPLay->addWidget(defServMP);
            defMPLay->addWidget(defMPLab);
            QVBoxLayout *defHPMPLay = new QVBoxLayout;
            defHPMPLay->addLayout(defHPLay);
            defHPMPLay->addLayout(defMPLay);
            QVBoxLayout *defAllLayout = new QVBoxLayout;
            defAllLayout->addWidget(defLabel);
            defAllLayout->addWidget(defServIcon);
            defAllLayout->addWidget(defServName);
            defAllLayout->addLayout(defHPMPLay);
            defAllLayout->addLayout(defAtkStats);

            QHBoxLayout *allLayout = new QHBoxLayout;
            allLayout->addLayout(atkAllLayout);
            allLayout->addWidget(vs);
            allLayout->addLayout(defAllLayout);

            QPushButton *okButton = new QPushButton(QWidget::tr("OK"));
            QPushButton *cancelButton = new QPushButton(QWidget::tr("Cancel"));
            QObject::connect(okButton, SIGNAL(clicked()), atkDialog, SLOT(accept()));
            QObject::connect(cancelButton, SIGNAL(clicked()), atkDialog, SLOT(reject()));
            QHBoxLayout *buttons = new QHBoxLayout;
            buttons->addWidget(okButton);
            buttons->addWidget(cancelButton);

            QVBoxLayout *finalLayout = new QVBoxLayout;
            finalLayout->addLayout(allLayout);
            finalLayout->addLayout(buttons);

            atkDialog->setLayout(finalLayout);

            int result = atkDialog->exec();

            if (result == QDialog::Rejected)
                return 35;
        }

        chosenDefenders.push_back(targetServ);
    }
    else if (chosenActionType == A)
    {
        // Ensure the target space is within the selectionRange
        bool found = false;
        Coordinate cLoc = currentServant->getCurrLoc();
        vector<Coordinate> oldRange = selectionRange;
        for (unsigned int i = 0; i < selectionRange.size() && !found; i++)
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
        for (unsigned int i = 0; i < selectionRange.size(); i++)
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

        // Check with player if they really want to do this action. Show a list
        // of all players they would be targetting
        QDialog *aoeAction = new QDialog;
        QLabel *targetPlayersLabel = new QLabel;
        QLabel *targetPlayers = new QLabel;
        targetPlayersLabel->setText("Targetted Players:");
        targetPlayersLabel->setAlignment(Qt::AlignCenter);
        targetPlayersLabel->setFrameStyle(QFrame::Box | QFrame::Sunken);
        string tPlayers = "\n";
        for (unsigned int i = 0; i < chosenDefenders.size(); i++)
        {
            tPlayers = tPlayers + chosenDefenders[i]->getTeamName() + " " +
                       chosenDefenders[i]->getName() + "\n";
        }
        targetPlayers->setText(QString::fromStdString(tPlayers));
        targetPlayers->setAlignment(Qt::AlignCenter);
        targetPlayers->setFrameStyle(QFrame::Box | QFrame::Sunken);

        QPushButton *okButton = new QPushButton(QWidget::tr("OK"));
        QPushButton *cancelButton = new QPushButton(QWidget::tr("Cancel"));
        QObject::connect(okButton, SIGNAL(clicked()), aoeAction, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), aoeAction, SLOT(reject()));
        QHBoxLayout *buttons = new QHBoxLayout;
        buttons->addWidget(okButton);
        buttons->addWidget(cancelButton);
        QVBoxLayout *allLayout = new QVBoxLayout;
        allLayout->addWidget(targetPlayersLabel);
        allLayout->addWidget(targetPlayers);
        allLayout->addLayout(buttons);

        aoeAction->setLayout(allLayout);

        int result = aoeAction->exec();
        if (result == QDialog::Rejected)
        {
            chosenDefenders.clear();
            selectionRange = oldRange;
            return 35;
        }
    }
    else
    {
        // Why are we here?? We should never be here.
        return 34;
    }

    turnState++;
    return turnStateApplyAction();
}

int GameState::turnStateApplyAction()
{
    if (turnState != 4)
        return 50;

    // Apply the chosen action to the chosen targets.
    // If the action is a noble phantasm and one of the targets is the Sai
    //  Avenger with at least 40 MP, then stop the action and deal the
    //  currentServant .8 * MAXMP damage. This only happens at ascension level 1
    //  or higher though
    bool activateNP = true;
    if (currentServant->getAscensionLvl() >= 1 &&
            ((currentServant->getName().compare("Lance Lancer") == 0 &&
            abs(currentServant->isActionNP(chosenAction)) != 1) ||
            (currentServant->getName().compare("Lance Lancer") != 0 &&
             currentServant->isActionNP(chosenAction) >= 0)))
    {
        for (unsigned int i = 0; i < chosenDefenders.size() && activateNP; i++)
        {
            if(chosenDefenders[i]->getName().compare("Sai Avenger") == 0 &&
                    chosenDefenders[i]->getCurrMP() > 40)
            {
                activateNP = false;
                log->addToEventLog("Sai Avenger's Essence of Fragarach activated! " +
                                   currentServant->getFullName() +
                                   "'s Noble Phantasm was stopped!");
                currentServant->subHP((currentServant->getMaxHP() * 8) / 10, OMNI);
                chosenDefenders[i]->subMP(40);
            }
        }
    }
    // If the current servant is a Lance Lancer using Gae Bolg, they still
    // sustain damage, but they also get a guaranteed hit off.
    else if (currentServant->getAscensionLvl() >= 1 &&
             (currentServant->getName().compare("Lance Lancer") == 0 &&
              currentServant->isActionNP(chosenAction) == 1))
    {
        for (unsigned int i = 0; i < chosenDefenders.size() && activateNP; i++)
        {
            if(chosenDefenders[i]->getName().compare("Sai Avenger") == 0 &&
                    chosenDefenders[i]->getCurrMP() > 40)
            {
                log->addToEventLog("Sai Avenger's Essence of Fragarach activated! But " +
                                   currentServant->getFullName() +
                                   "'s Gae Bolg was not stopped!");
                currentServant->subHP((currentServant->getMaxHP() * 8) / 10, OMNI);
                chosenDefenders[i]->subMP(40);

                // Set the Avenger's luck to 0, to ensure that Gae Bolg hits
                vector<Stat> tDebStat;
                tDebStat.push_back(LUK);
                vector<int> tDebAm;
                tDebAm.push_back(-1 * chosenDefenders[i]->getLuk());
                Debuff *newDebuff = new Debuff("GB",
                                               "Gae Bolg will kill you.",
                                               chosenDefenders[i]->getTeam(),
                                               tDebStat, tDebAm, 1);
                chosenDefenders[i]->addDebuff(newDebuff);
            }
        }
    }

    int ret = 0;
    if (activateNP)
        ret = currentServant->doAction(chosenAction, chosenDefenders);

    if (ret != 0)
    {
        // Something went wrong! Return now.
        if (ret == 41)
        {
            // The something that went wrong is that the action is invalid.
            // Reset the turnstate to the choose Action state.
            turnState = 2;
            chosenAction = -1;
            selectionRange.clear();
            chosenDefenders.clear();
        }
        return ret;
    }

    // If the servant died for some reason
    if (currentServant->getCurrHP() <= 0)
    {
        turnState += 2;
        return endTurnProcess();
    }

    // If the Servant is a Rider, allow them to move again.
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
    else if (currentServant->getClass() == Archer && !archerSecondTurn)
    {
        int r = currentServant->getRandNum();
        if (r <= currentServant->getLuk())
        {
            log->addToEventLog("Your 'Independent Action' allows you to take another turn!");
            archerSecondTurn = true;
            turnState = 1;
            validMoves = getValidMoves(currentServant, currentServant->getMov());

            // Process death and revivals

            // Check if anyone has died and modify the death list accordingly.
            for (unsigned int i = 0; i < turnOrder.size(); i++)
            {
                if (turnOrder[i]->getCurrHP() <= 0 && !isServantDead(turnOrder[i]))
                {
                    addDead(turnOrder[i]);
                }
            }

            // Make sure that everyone in the dead list is off of the playing field.
            for (unsigned int i = 0; i < dead.size(); i++)
            {
                field->servantDead(dead[i]);
            }

            // If a Servant has been revived, put them back on the playing field.
            for (int i = 0; i < (int) dead.size(); i++)
            {
                if (dead[i]->getCurrHP() > 0)
                {
                    field->servantRevived(dead[i]);
                    removeDead(dead[i]);
                    i--;
                }
            }
            revived.clear(); // Don't want to revive the servants a second time

            return 0;
        }
    }

    // If the "Essence of Ionioi Hetairoi" reality marble is active and this
    // Servant is on the same team as the caster, check against their LUK to
    // see if they get a second turn.
    else if (!ionioiSecondTurn &&
             field->isRealityMarbleOn() &&
             field->getDebuffOnSpace(currentServant->getCurrLoc())->getDebuffDescrip().compare("Essence of Ionioi Hetairoi") == 0
             && field->getDebuffOnSpace(currentServant->getCurrLoc())->getTargetTeam() == currentServant->getTeam())
    {
        int r = currentServant->getRandNum();
        if (r <= currentServant->getLuk())
        {
            log->addToEventLog("Your ally's 'Essence of Ionioi Hetairoi' allows you to take another turn!");
            ionioiSecondTurn = true;
            turnState = 1;
            validMoves = getValidMoves(currentServant, currentServant->getMov());

            // Process death and revivals

            // Check if anyone has died and modify the death list accordingly.
            for (unsigned int i = 0; i < turnOrder.size(); i++)
            {
                if (turnOrder[i]->getCurrHP() <= 0 && !isServantDead(turnOrder[i]))
                {
                    addDead(turnOrder[i]);
                }
            }

            // Make sure that everyone in the dead list is off of the playing field.
            for (unsigned int i = 0; i < dead.size(); i++)
            {
                field->servantDead(dead[i]);
            }

            // If a Servant has been revived, put them back on the playing field.
            for (int i = 0; i < (int) dead.size(); i++)
            {
                if (dead[i]->getCurrHP() > 0)
                {
                    field->servantRevived(dead[i]);
                    removeDead(dead[i]);
                    i--;
                }
            }
            revived.clear(); // Don't want to revive the servants a second time

            return 0;
        }
    }


    // End the non-Rider turn by calling the Post-turn turn state.
    turnState += 2;
    return endTurnProcess();
}

int GameState::turnStateExtraMove()
{
    if (turnState != 5)
        return 60;

    // Verify that the clicked space is valid
    bool isValid = false;
    for (unsigned int i = 0; i < validMoves.size() && !isValid; i++)
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
    log->addToEventLog("Move from space (" + to_string(servStart.x) + "," +
                  to_string(servStart.y) + ")" + " to space (" +
                  to_string(servEnd.x) + "," + to_string(servEnd.y) + ").");
    return endTurnProcess();
}

// If this function returns 1000, then team Alpha is dead. If it returns 1001,
// team Omegaa is dead. If it returns 1002, team Boss is dead.
int GameState::turnStatePostTurn()
{
    if (turnState != 6)
        return 70;

    int returnValue = 0;

    // Check the Servant's ending location to see if any end-of-turn affects
    // should be applied.
    Debuff *tDebuff = field->getDebuffOnSpace(currentServant->getCurrLoc());
    if (tDebuff != NULL && tDebuff->getTargetTeam() == currentServant->getTeam())
    {
        if (tDebuff->getDebuffDescrip().compare("Castle Doctrine") == 0 &&
                !(servStart.x == servEnd.x && servStart.y == servEnd.y))
        {
            vector<Servant*> tDef;
            tDef.push_back(currentServant);
            field->realityMarbleServant()->attack(tDef, false);
        }
    }

    // Check if anyone has died and modify the death list accordingly.
    for (unsigned int i = 0; i < turnOrder.size(); i++)
    {
        if (turnOrder[i]->getCurrHP() <= 0 && !isServantDead(turnOrder[i]))
        {
            addDead(turnOrder[i]);
        }
    }

    // Make sure that everyone in the dead list is off of the playing field.
    for (unsigned int i = 0; i < dead.size(); i++)
    {
        field->servantDead(dead[i]);
    }

    // If a Servant has been revived, put them back on the playing field.
    for (int i = 0; i < (int) dead.size(); i++)
    {
        if (dead[i]->getCurrHP() > 0)
        {
            field->servantRevived(dead[i]);
            removeDead(dead[i]);
            i--;
        }
    }
    revived.clear(); // Don't want to revive the servants a second time

    // Check if any of the teams have died.
    for (unsigned int i = 0; i < activeTeams.size(); i++)
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

    log->addEventEndTurn(currentServant->getTeamName(),
                         currentServant->getName());

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

    return returnValue;
}

int GameState::endTurnProcess()
{
    int result = endTurn();
    if (result == 10)
    {
        log->addToErrorLog("An error occured (endTurn()->turnStatePreTurn()).");
    }
    else if (result == 70)
    {
        log->addToErrorLog("An error occured (endTurn()).");
    }
    else if (result == 1000)
    {
        QMessageBox messageBox;
        messageBox.setWindowTitle(QObject::tr("Final Fate"));
        messageBox.setText(QObject::tr("Team Alpha Loses!"));
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.setDefaultButton(QMessageBox::Ok);
        if (messageBox.exec() == QMessageBox::Ok)
            result = 666;
    }
    else if (result == 1001)
    {
        QMessageBox messageBox;
        messageBox.setWindowTitle(QObject::tr("Final Fate"));
        messageBox.setText(QObject::tr("Team Omega Loses!"));
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.setDefaultButton(QMessageBox::Ok);
        if (messageBox.exec() == QMessageBox::Ok)
            result = 666;
    }
    else if (result == 1002)
    {
        QMessageBox messageBox;
        messageBox.setWindowTitle(QObject::tr("Final Fate"));
        messageBox.setText(QObject::tr("Boss Team Loses!"));
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.setDefaultButton(QMessageBox::Ok);
        if (messageBox.exec() == QMessageBox::Ok)
            result = 666;
    }

    return result;
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

int GameState::capZero(int num)
{
    if (num < 0)
        return 0;
    else
        return num;
}
