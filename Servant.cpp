/* Class file for servants.
 * 
 * This file contains the implementation for a simple Servant. It tracks the
 * stats and Ascension level, as well as holding a pointer to the Servant's 
 * weapon.
 *
 */

#include "Servant.h"
#include "PlayField.h"
#include <cmath>
#include <iostream>

/********** Function definitions **********/
// Constructor
Servant::Servant(int as, Team t, Logger *l)
{
    ascension = as;
    team = t;
    field = NULL;
    log = l;

    territoryActive = false;
    realityMarbleActive = false;

    //vector<Debuff*> debuffs (0);
}

void Servant::setPlayField(PlayField *f)
{
    field = f;
}

//***** Manipulators *****
void Servant::setHP(int hp)
{
    currHP = hp;
    int mhp = getMaxHP();
    if (currHP > mhp)
        currHP = mhp;
    else if (currHP <= 0)
    {
        currHP = 0;
        remAllDebuffs(false);
    }
}

void Servant::setMP(int mp)
{
    currMP = mp;
    int mmp = getMaxMP();
    if (currMP > mmp)
        currMP = mmp;
    else if (currMP < 0)
        currMP = 0;
}

void Servant::addHP(int hp)
{
    currHP += hp;
    int mhp = getMaxHP();
    if (currHP > mhp)
        currHP = mhp;
    else if (currHP < 0)
        currHP = 0;
}

void Servant::addMP(int mp)
{
    currMP += mp;
    int mmp = getMaxMP();
    if (currMP > mmp)
        currMP = mmp;
    else if (currMP < 0)
        currMP = 0;
}

void Servant::subHP(int hp, DamageType dt)
{
    currHP -= hp;
    int mhp = getMaxHP();
    if (currHP > mhp)
        currHP = mhp;
    else if (currHP <= 0)
    {
        currHP = 0;
        remAllDebuffs(false);
    }
}

void Servant::subMP(int mp)
{
    currMP -= mp;
    int mmp = getMaxMP();
    if (currMP > mmp)
        currMP = mmp;
    else if (currMP < 0)
        currMP = 0;
}

void Servant::setLoc(Coordinate newLoc)
{
    currLoc = newLoc;
}

//***** Retrievers *****
string Servant::getName()
{
    return name;
}

string Servant::getTeamName()
{
    if (team == Alpha)
        return "Alpha";
    else if (team == Omega)
        return "Omega";
    else
        return "Boss";
}

string Servant::getFullName()
{
    return getTeamName() + " " + getName();
}

string Servant::getServantIcon()
{
    return ":/ServantIcons/" + name + ".png";
}

string Servant::getTeamIcon()
{
    if (team == Alpha)
        return ":/TeamIcons/Alpha.png";
    else if (team == Omega)
        return ":/TeamIcons/Omega.png";
    else
        return ":/TeamIcons/Psi.png";
}

// Stat Retrievers
int Servant::getMaxHP()
{
    int d = getDebuffAmount(MAXHP);
    int ret = maxHP[ascension] + d;
    if (ret <= 0)
        ret = 1;
    return ret;
}

int Servant::getCurrHP()
{
    int mhp = getMaxHP();
    if (currHP > mhp)
        currHP = mhp;
    return currHP;
}

int Servant::getMaxMP()
{
    int d = getDebuffAmount(MAXMP);
    int ret = maxMP[ascension] + d;
    if (ret < 0)
        ret = 0;
    return ret;
}

int Servant::getCurrMP()
{
    int mmp = getMaxMP();
    if (currMP > mmp)
        currMP = mmp;
    return currMP;
}

int Servant::getMov()
{
    int d = getDebuffAmount(MOV);
    int ret = mov[ascension] + d;
    if (ret < 0)
        ret = 0;
    return ret;
}

int Servant::getStr()
{
    int d = getDebuffAmount(STR);
    int ret = str[ascension] + d;
    if (ret < 0)
        ret = 0;
    return ret;
}

int Servant::getMag()
{
    int d = getDebuffAmount(MAG);
    int ret = mag[ascension] + d;
    if (ret < 0)
        ret = 0;
    return ret;
}

int Servant::getDef()
{
    int d = getDebuffAmount(DEF);
    int ret = def[ascension] + d;
    if (ret < 0)
        ret = 0;
    return ret;
}

int Servant::getRes()
{
    int d = getDebuffAmount(RES);
    int ret = res[ascension] + d;
    if (ret < 0)
        ret = 0;
    return ret;
}

int Servant::getSpd()
{
    int d = getDebuffAmount(SPD);
    int ret = spd[ascension] + d;
    if (ret < 0)
        ret = 0;
    return ret;
}

int Servant::getSkl()
{
    int d = getDebuffAmount(SKL);
    int ret = skl[ascension] + d;
    if (ret < 0)
        ret = 0;
    return ret;
}

int Servant::getLuk()
{
    int d = getDebuffAmount(LUK);
    int ret = luk[ascension] + d;
    if (ret < 0)
        ret = 0;
    return ret;
}

// Ascension level, class, and team retrievers
int Servant::getAscensionLvl()
{
    return ascension;
}

Class Servant::getClass()
{
    return clss;
}

Team Servant::getTeam()
{
    return team;
}

// Debuffs and skills
vector<Debuff*> Servant::getDebuffs()
{
    return debuffs;
}

void Servant::addDebuff(Debuff *d)
{
    debuffs.push_back(d);
}

void Servant::decDebuffs()
{
    for (int i = 0; i < (int) debuffs.size(); i++)
    {
        debuffs[i]->decrementTurnsRemaining();
        if (debuffs[i]->getTurnsRemaining() == 0)
        {
            debuffs.erase(debuffs.begin()+i);
            i--;
        }
    }
}

void Servant::remAllDebuffs(bool purgePermadead)
{
    for (int i = 0; i < (int) debuffs.size(); i++)
    {
        if(!(debuffs[i]->getDebuffName().compare("Permadeath") == 0 && !purgePermadead) &&
                !(debuffs[i]->getDebuffName().compare("Cursed") != 0 && !purgePermadead) &&
                debuffs[i]->getDebuffDescrip().compare("Passive Skill") != 0 &&
                debuffs[i]->getDebuffDescrip().compare("Passive Noble Phantasm") != 0)
        {
            debuffs.erase(debuffs.begin()+i);
            i--;
        }
        else
        {
            debuffs[i]->setTurnsRemaining(-1);
        }
    }
}

void Servant::remAllDebuffs2(bool purgePermadeath, bool purgeDoom)
{
    for (int i = 0; i < (int) debuffs.size(); i++)
    {
        if((purgePermadeath && debuffs[i]->getDebuffName().compare("Permadeath") == 0) ||
                (purgePermadeath && debuffs[i]->getDebuffName().compare("Cursed") == 0))
        {
            debuffs.erase(debuffs.begin()+i);
            i--;
        }
        else if (purgeDoom && debuffs[i]->getDebuffName().compare("Doom") == 0)
        {
            debuffs.erase(debuffs.begin()+i);
            i--;
        }
        else if (debuffs[i]->getDebuffName().compare("Permadeath") != 0 &&
                 debuffs[i]->getDebuffName().compare("Doom") != 0 &&
                 debuffs[i]->getDebuffDescrip().compare("Passive Skill") != 0 &&
                 debuffs[i]->getDebuffDescrip().compare("Passive Noble Phantasm") != 0)
        {
            debuffs.erase(debuffs.begin()+i);
            i--;
        }
    }
}

void Servant::removeDoom()
{
    for (int i = 0; i < (int) debuffs.size(); i++)
    {
        if (debuffs[i]->getDebuffName().compare("Doom") == 0)
        {
            debuffs.erase(debuffs.begin()+i);
            i--;
        }
    }
}

vector<Debuff*> Servant::getCastedDebuffs()
{
    return castedDebuffs;
}

void Servant::addCastedDebuff(Debuff *d)
{
    castedDebuffs.push_back(d);
}

void Servant::decCastedDebuffs()
{
    for (int i = 0; i < (int) debuffs.size(); i++)
    {
        castedDebuffs[i]->decrementTurnsRemaining();
        if (castedDebuffs[i]->getTurnsRemaining() == 0)
        {
            castedDebuffs.erase(castedDebuffs.begin()+i);
            i--;
        }
    }
}

void Servant::remCastedDebuffs()
{
    int i = 0;
    while (i < (int) castedDebuffs.size())
    {
        castedDebuffs[i]->setTurnsRemaining(1);
        castedDebuffs.erase(castedDebuffs.begin()+i);
    }
}

int Servant::getDebuffAmount(Stat s)
{
    int d = 0;

    for (unsigned int i = 0; i < debuffs.size(); i++)
    {
        vector<Stat> dStats = debuffs[i]->getDebuffStats();
        vector<int> dAmount = debuffs[i]->getDebuffAmounts();
        for (unsigned int j = 0; j < dStats.size(); j++)
        {
            if (dStats[j] == s)
                d += dAmount[j];
        }
    }

    return d;
}

bool Servant::hasDebuff(string d)
{
    bool cooldown = false;
    for (int i = 0; i < (int) debuffs.size(); i++)
    {
        if(debuffs[i]->getDebuffName().compare(d) == 0)
        {
            cooldown = true;
        }
    }

    return cooldown;
}

// Caster territory functions
bool Servant::isTerritoryActive()
{
    return territoryActive;
}

string Servant::removeTerritory()
{
    string territoryName = "";
    for (int i = 0; i < (int) castedDebuffs.size(); i++)
    {
        string n = castedDebuffs[i]->getDebuffName();
        if (n.compare("Territory") == 0)
        {
            territoryName = castedDebuffs[i]->getDebuffDescrip();
            castedDebuffs.erase(castedDebuffs.begin()+i);
            i--;
        }
    }

    return territoryName;
}

// Current location
Coordinate Servant::getCurrLoc()
{
    return currLoc;
}

bool Servant::isAdjacent(Servant *s)
{
    Coordinate them = s->getCurrLoc();
    int xDif = abs(currLoc.x - them.x);
    int yDif = abs(currLoc.y - them.y);

    return (xDif + yDif == 1);
}

bool Servant::isInRange(Servant *s)
{
    Coordinate them = s->getCurrLoc();
    int xDif = abs(currLoc.x - them.x);
    int yDif = abs(currLoc.y - them.y);

    return (xDif + yDif >= getLowRange()) && (xDif + yDif <= getHighRange());
}

int Servant::getLowRange()
{
    return lowRange;
}

int Servant::getHighRange()
{
    return hiRange;
}

// Battle formula retrievers
int Servant::getHitRate()
{
    // Originally was = Skill * 2 + Luck
    return (getSkl() * 3) + getLuk();
}

// This function should be overridden by Servants with multiple evasion
// chances (e.g. the Knight classes with the Eye of the Mind skill)
vector<int> Servant::getEvade()
{
    // Evasion = Speed * 2 + Luck
    vector<int> evade;
    evade.push_back(getInitialEvade());
    return evade;
}

int Servant::getInitialEvade()
{
    // Original was Speed * 2 + Luck
    return getSpd() + getLuk();
}

int Servant::getCriticalRate()
{
    // Critical Rate = Skill / 2
    return (getSkl() / 2);
}

int Servant::getCriticalEvade()
{
    // Critical Evade = Luck
    return getLuk();
}

// Action retrievers
ActionType Servant::getActionType(int action)
{
    return actionListTypes[ascension][action];
}

int Servant::getActionMPCost(int action)
{
    return actionMPCosts[ascension][action];
}

bool Servant::getActionCounterable(int action)
{
    return actionCounterable[ascension][action];
}

bool Servant::getActionDodgeable(int action)
{
    return actionDodgeable[ascension][action];
}

string Servant::getActionName(int action)
{
    if (action < actionList[ascension].size())
        return actionList[ascension][action];
    else
        return "";
}

int Servant::isActionNP(int action)
{
    int ret = -1;
    switch(action)
    {
        case 1:
            ret = 0;
            break;
        case 2:
            ret = 1;
            break;
        case 3:
            ret = 2;
            break;
        default:
            break;
    }

    return ret;
}

bool Servant::isActionRealityMarble(int action)
{
    return false;
}

int Servant::getRealityMarbleMP()
{
    return 10;
}

int Servant::getTerritoryMP()
{
    return 5;
}

string Servant::getTerritoryName()
{
    return "N/A";
}

bool Servant::isHealAction(int action)
{
    return false;
}

bool Servant::isPoisonAction(int action)
{
    return false;
}

bool Servant::isKillDeadAction(int action)
{
    return false;
}

bool Servant::isGodmindActive()
{
    return false;
}

vector<Coordinate> Servant::getActionRange(int action)
{
    // Figure out what action this is and return the appropriate range
    vector<Coordinate> range;

    if (action == 0)
    {
        for (int i = -1*getHighRange(); i <= getHighRange(); i++)
        {
            for (int j = -1*getHighRange(); j <= getHighRange(); j++)
            {
                if (abs(i) + abs(j) >= getLowRange() && abs(i) + abs(j) <= getHighRange())
                {
                    Coordinate inRange;
                    inRange.x = i;
                    inRange.y = j;
                    range.push_back(inRange);
                }
            }
        }
    }
    else
    {
        range = getNPRange(action-1);
    }

    return range;
}

vector<string> Servant::getActionList()
{
    return actionList[ascension];
}

vector<ActionType> Servant::getActionListTypes()
{
    return actionListTypes[ascension];
}

vector<int> Servant::getActionMPCosts()
{
    return actionMPCosts[ascension];
}

vector<bool> Servant::getActionsDodgeable()
{
    return actionDodgeable[ascension];
}

vector<bool> Servant::getActionsCounterable()
{
    return actionCounterable[ascension];
}

vector<vector<string>> Servant::getNoblePhantasms()
{
    return noblePhantasms;
}

vector<vector<Coordinate>> Servant::getNPRanges()
{
    return npRanges;
}

vector<Coordinate> Servant::getNPRange(int np)
{
    if (np >= (int) npRanges.size())
    {
        vector<Coordinate> ran;
        return ran;
    }
    else
        return npRanges[np];
}

bool Servant::isPermaDead()
{
    bool ret = false;
    for (unsigned int i = 0; i < debuffs.size() && !ret; i++)
    {
        if (debuffs[i]->getDebuffName().compare("Permadeath") == 0 ||
                debuffs[i]->getDebuffName().compare("Cursed") == 0)
        {
            ret = true;
        }
    }

    return ret;
}

// This only needs to be overriden if the Servant has an attack that isn't
// simply single-target.
int Servant::attack(vector<Servant *> defenders, bool counter)
{
    if (actionMPCosts[ascension][0] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][0]);
        for (unsigned int i = 0; i < defenders.size(); i++)
        {
            int dam = 0;
            // Check if you hit the targets
            vector<int> opEvade = defenders[i]->getEvade();
            bool hit = false;
            // Calculate accuracy
            int accuracy = capZero(getHitRate() - opEvade[0]);

            int r = getRandNum();
            if (accuracy >= r)
                hit = true;

            if (opEvade.size() > 1 && hit)
            {
                for (unsigned int j = 1; j < opEvade.size() && hit; j++)
                {
                    r = getRandNum();
                    if (opEvade[j] >= r)
                        hit = false;
                }
            }

            // If you hit, calculate crit chance
            if (hit)
            {
                int attackMult = 1;
                int critChance = capZero(getCriticalRate() -
                                 defenders[i]->getCriticalEvade());
                r = getRandNum();
                if (critChance >= r)
                    attackMult *= 3;

                // Deal the damage
                dam = capZero(getStr() - defenders[i]->getDef()) * attackMult;
                log->addToEventLog(getFullName() + " dealt " +
                                   to_string(dam) + " damage to " +
                                   defenders[i]->getFullName() + ".");
                defenders[i]->subHP(dam, D_STR);
            }
            else
            {
                log->addToEventLog(getFullName() + " missed " +
                                   defenders[i]->getFullName() + "!");
            }

            // Check to see if the defender is dead. If they are, do not call
            // the counterattack. Additionally, if they are an Avenger and they
            // die, activate Final Revenge.
            // If they are not dead but they are a Berserker, check to see if
            // Mad Counter activates.
            if(defenders[i]->getCurrHP() > 0)
            {
                // Check if the defender is a Berserker. If they are, and they
                // are adjacent to this unit, check to see if Mad Counter
                // activates.
                if (defenders[i]->getClass() == Berserker &&
                    isAdjacent(defenders[i]))
                {
                    r = getRandNum();
                    if (defenders[i]->getLuk() >= r)
                    {
                        // Mad Counter activated! The attacking servant takes
                        // damage equal to the damage they dealt.
                        log->addToEventLog(defenders[i]->getFullName() +
                                           "'s Mad Counter activated, dealing " +
                                           to_string(dam) + " damage back to " +
                                           getFullName() + ".");
                        subHP(dam, C_STR);
                    }
                }
                // Call "attack" on the defending servant for their
                // counterattack, if you are in their range and you are the
                // initiating servant.
                if (defenders[i]->isInRange(this) && counter)
                {
                    vector<Servant *> you;
                    you.push_back(this);
                    defenders[i]->attack(you, false);
                }
            }
            else
            {
                if (defenders[i]->getClass() == Avenger)
                {
                    // Activate Final Revenge
                    Debuff *finRev = defenders[i]->finalRevenge();
                    addDebuff(finRev);
                    if (defenders[i]->getAscensionLvl() == 2)
                    {
                        subHP(.1 * getMaxHP(), OMNI);
                        subMP(.1 * getMaxMP());

                        if (getCurrHP() == 0)
                        {
                            setHP(1);
                        }
                    }
                }
            }
        }
    }

    return 0;
}

int Servant::doAction(int actionNum, vector<Servant *> defenders)
{
    int ret = 0;
    switch (actionNum)
    {
        case 0:
            ret = attack(defenders, true);
            break;
        case 1:
            ret = activateNP1(defenders);
            break;
        case 2:
            ret = activateNP2(defenders);
            break;
        case 3:
            ret = activateNP3(defenders);
            break;
        default:
            return 2; // Not a valid choice
            break;
    }
    return ret;
}

int Servant::getRandNum()
{
    int rand1 = rand() % 101;
    int rand2 = rand() % 101;
    return (rand1 + rand2) / 2;
}

int Servant::capZero(int num)
{
    if (num < 0)
        return 0;
    else
        return num;
}

int Servant::capOne(int num)
{
    if (num < 1)
        return 1;
    else
        return num;
}

/* Placeholder definitions. Override in subclasses. */
Debuff* Servant::finalRevenge()
{
    vector<Stat> s;
    s.push_back(MAXHP);
    vector<int> da;
    da.push_back(0);
    Debuff *fR = new Debuff("_","__", getTeam(), s, da, 2);
    return fR;
}

int Servant::activateNP1(vector<Servant *> defenders)
{
    return 10 + defenders.size();
}

int Servant::activateNP2(vector<Servant *> defenders)
{
    return 10 + defenders.size();
}

int Servant::activateNP3(vector<Servant *> defenders)
{
    return 10 + defenders.size();
}

int Servant::endRealityMarble()
{
    realityMarbleActive = false;
    return 0;
}

void Servant::endTerritory()
{
    territoryActive = false;
}

Coordinate Servant::getEndLocation(Coordinate c, int range)
{
    Coordinate n, s, e, w, err;
    n.x = s.x = c.x;
    e.y = w.y = c.y;
    n.y = c.y + 1;
    s.y = c.y - 1;
    e.x = c.x + 1;
    w.x = c.x - 1;

    err.x = err.y = -1;

    if (field->isValidCoordinate(n) &&
            (abs(n.x - currLoc.x) + abs(n.y - currLoc.y)) <= range)
        return n;
    else if (field->isValidCoordinate(s) &&
             (abs(s.x - currLoc.x) + abs(s.y - currLoc.y)) <= range)
        return s;
    else if (field->isValidCoordinate(e) &&
             (abs(e.x - currLoc.x) + abs(e.y - currLoc.y)) <= range)
        return e;
    else if (field->isValidCoordinate(w) &&
             (abs(w.x - currLoc.x) + abs(w.y - currLoc.y)) <= range)
        return w;
    else
        return err;
}

bool Servant::isBerserk()
{
    return false;
}
