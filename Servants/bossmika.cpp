#include "bossmika.h"
#include "PlayField.h"

// Character's full name: Mikaela Hyakuya
BossMika::BossMika(int as, Team t, Logger *l) : ServantSaber(as, t, l)
{
    name = "Mikaela";

    /** Stat modifiers **/
    /*int strMod = 10;
    int spdMod = 10;
    int sklMod = 0;
    int defMod = 10;

    spd[0] += spdMod;
    spd[1] += spdMod;
    spd[2] += spdMod;
    str[0] += strMod;
    str[1] += strMod;
    str[2] += strMod;
    skl[0] += sklMod;
    skl[1] += sklMod;
    skl[2] += sklMod;
    def[0] += defMod;
    def[1] += defMod;
    def[2] += defMod;*/

    actionList[0].push_back("4: Bloodthirst");
    actionList[1].push_back("4: Bloodthirst");
    actionList[1].push_back("5: Blade Gust");
    actionList[2].push_back("4: Bloodthirst");
    actionList[2].push_back("5: Blade Gust");
    actionList[2].push_back("6: Blood Blast");

    actionListTypes[0].push_back(S);
    actionListTypes[1].push_back(S);
    actionListTypes[1].push_back(A);
    actionListTypes[2].push_back(S);
    actionListTypes[2].push_back(A);
    actionListTypes[2].push_back(A);

    actionMPCosts[0].push_back(10);
    actionMPCosts[1].push_back(10);
    actionMPCosts[1].push_back(50);
    actionMPCosts[2].push_back(10);
    actionMPCosts[2].push_back(50);
    actionMPCosts[2].push_back(75);

    actionDodgeable[0].push_back(false);
    actionDodgeable[1].push_back(false);
    actionDodgeable[1].push_back(false);
    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(false);

    actionCounterable[0].push_back(false);
    actionCounterable[1].push_back(false);
    actionCounterable[1].push_back(false);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);

    vector<string> np1;
    np1.push_back("Blade of the Vampire");
    np1.push_back("Passive Noble Phantasm. Mika’s blade hungers for blood and readily laps up any that Mika loses, which then grants Mika a stat boost. Gain a boost to all stats equal to (percentage of HP loss / 5).");
    vector<string> np2;
    np2.push_back("Blade Gust");
    np2.push_back("Generate an incredible gust of wind with your sword, sending all opponents within two spaces flying backward six spaces. Costs 50 MP.");
    vector<string> np3;
    np3.push_back("Blood Blast");
    np3.push_back("Sacrifice 50 HP (and 75 MP) to attack everyone -- friend and foe alike -- within three spaces with a blast of blood. Does undodgeable thrice normal damage. Over the next three turns, heal the total amount of damage you dealt each turn.");
    noblePhantasms.push_back(np1);
    noblePhantasms.push_back(np2);
    noblePhantasms.push_back(np3);

    // Blade of the Vampire range (doesn't really have one)
    npRanges.push_back(getLowToHighRange(0,0));
    // Blade Gust range
    npRanges.push_back(getLowToHighRange(1,2));
    // Blood Blast range
    npRanges.push_back(getLowToHighRange(1,3));

    /** Passive Skill modifiers **/
    vector<Stat> hdS;
    hdS.push_back(STR);
    hdS.push_back(DEF);
    hdS.push_back(SPD);
    vector<int> hdA;
    hdA.push_back(10);
    hdA.push_back(10);
    hdA.push_back(10);
    Debuff *highDivinity = new Debuff("True Noble Vampire", "Passive Skill",
                                      t, hdS, hdA, -1);
    addDebuff(highDivinity);
}

/***** Active Skills *****/
int BossMika::bloodThirst(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][3] > currMP)
        return 1; // Not enough MP to attack

    subMP(actionMPCosts[ascension][3]);

    log->addToEventLog(getFullName() + " used Bloodthirst!");

    for (unsigned int i = 0; i < defenders.size(); i++)
    {
        // Always hits, never crits

        // Deal the damage
        int dam = min(25, defenders[i]->getCurrHP());
        log->addToEventLog(getFullName() + " absorbed " +
                           to_string(dam) + " HP from " +
                           defenders[i]->getFullName() + ".");
        defenders[i]->subHP(dam, OMNI);
        addHP(dam);

        // Check to see if the defender is dead.
        if(defenders[i]->getCurrHP() <= 0)
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

    return 0;
}

/***** Helper Functions *****/
bool BossMika::bloodBlastActive()
{
    bool cooldown = false;
    for (int i = 0; i < (int) debuffs.size(); i++)
    {
        if(debuffs[i]->getDebuffName().compare("Blood Blast") == 0)
        {
            cooldown = true;
        }
    }

    return cooldown;
}

/***** Function Re-definitions *****/
void BossMika::subHP(int hp, DamageType dt)
{
    currHP -= hp;
    int mhp = getMaxHP();
    if (currHP > mhp)
        currHP = mhp;

    if (currHP <= 0)
    {
        currHP = 0;
        remAllDebuffs(false);
    }
    else
    {
        // Remove previous Blade of the Vampire buff if it exists
        for (int i = 0; i < (int) debuffs.size(); i++)
        {
            if(debuffs[i]->getDebuffName().compare("Blade of the Vampire") == 0)
            {
                debuffs.erase(debuffs.begin()+i);
                i--;
            }
        }

        // Blade of the Vampire passive Noble Phantasm
        double cHPLoss = getMaxHP() - (currHP * 1.0);
        double mHP = getMaxHP() * 1.0;
        int statBonus = (cHPLoss / mHP) * 20;

        vector<Stat> tStats;
        tStats.push_back(STR);
        tStats.push_back(MAG);
        tStats.push_back(DEF);
        tStats.push_back(RES);
        tStats.push_back(SPD);
        tStats.push_back(SKL);
        tStats.push_back(LUK);
        vector<int> tAmounts;
        tAmounts.push_back(statBonus);
        tAmounts.push_back(statBonus);
        tAmounts.push_back(statBonus);
        tAmounts.push_back(statBonus);
        tAmounts.push_back(statBonus);
        tAmounts.push_back(statBonus);
        tAmounts.push_back(statBonus);
        Debuff* du = new Debuff("Blade of the Vampire",
                                "--", getTeam(), tStats, tAmounts, -1);
        addDebuff(du);
    }
}

int BossMika::doAction(int actionNum, vector<Servant *> defenders)
{
    int ret = 0;

    switch (actionNum)
    {
        case 0:
            ret = attack(defenders, true);
            break;
        case 1:
            ret = guardianKnight(defenders);
            break;
        case 2:
            ret = manaBurst(defenders);
            break;
        case 3:
            ret = bloodThirst(defenders);
            break;
        case 4:
            ret = activateNP1(defenders);
            break;
        case 5:
            ret = activateNP2(defenders);
            break;
        default:
            return 2; // Not a valid choice
            break;
    }

    return ret;
}

vector<Coordinate> BossMika::getActionRange(int action)
{
    // Figure out what action this is and return the appropriate range
    vector<Coordinate> range;

    if (action == 0 || action == 3) // Regular attack/Bloodthirst range
    {
        range = getLowToHighRange(1,1);
    }
    else if (action == 1) // Guardian Knight range
    {
        range = getLowToHighRange(1,3);
    }
    else if (action == 2) // Mana Burst
    {
        range = getLowToHighRange(0,0);
    }
    else
    {
        int ret = isActionNP(action);
        if (ret != -1)
            range = getNPRange(ret);
    }

    return range;
}

int BossMika::isActionNP(int action)
{
    int ret = -1;
    if (action == 4)
        ret = 1;
    else if (action == 5)
        ret = 2;
    return ret;
}

/***** Noble Phantasms *****/
// Blade Gust
int BossMika::activateNP1(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][4] > currMP)
        return 1; // Not enough MP to attack

    subMP(actionMPCosts[ascension][4]);

    int shoveDistance = 8;

    log->addToEventLog(getFullName() + " used Blade Gust!");

    for (unsigned int i = 0; i < defenders.size(); i++)
    {
        // Get the target's current location
        Coordinate targetLoc = defenders[i]->getCurrLoc();

        // Figure out which direction from the player they are
        Direction fromPlayer = getDirectionAtoB(getCurrLoc(), targetLoc);

        // Create a new target location for the target based on their current
        //  location and their direction from the player
        Coordinate finalLoc;
        if (fromPlayer == NORTH)
        {
            finalLoc.x = targetLoc.x;
            finalLoc.y = targetLoc.y + shoveDistance;
            if (finalLoc.y >= field->getFieldLength())
                finalLoc.y = field->getFieldLength() - 1;
        }
        else if (fromPlayer == SOUTH)
        {
            finalLoc.x = targetLoc.x;
            finalLoc.y = targetLoc.y - shoveDistance;
            if (finalLoc.y < 0)
                finalLoc.y = 0;
        }
        else if (fromPlayer == EAST)
        {
            finalLoc.x = targetLoc.x + shoveDistance;
            finalLoc.y = targetLoc.y;
            if (finalLoc.x >= field->getFieldWidth())
                finalLoc.x = field->getFieldWidth() - 1;
        }
        else // West
        {
            finalLoc.x = targetLoc.x - shoveDistance;
            finalLoc.y = targetLoc.y;
            if (finalLoc.x < 0)
                finalLoc.x = 0;
        }

        // Get the nearest valid coordinate from the "finalLoc"
        Coordinate actualLoc = field->getNearestValidCoord(finalLoc);

        // Move the target Servant to this location
        field->moveServant(defenders[i], actualLoc);

        log->addToEventLog(defenders[i]->getFullName() +
                           " was shoved from (" +
                           to_string(targetLoc.x) + "," + to_string(targetLoc.y)
                           + ") to (" + to_string(actualLoc.x) + "," +
                           to_string(actualLoc.y) + ").");
    }

    return 0;
}

// Blood Blast
int BossMika::activateNP2(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][5] > currMP)
        return 1; // Not enough MP to attack

    if (bloodBlastActive())
    {
        log->addToEventLog("You are still healing from your last Blood Blast!");
        return 41;
    }

    if (getCurrHP() <= 50)
    {
        log->addToEventLog("You do not have enough HP to sacrifice!");
        return 41;
    }

    subMP(actionMPCosts[ascension][5]);
    subHP(50, OMNI);

    log->addToEventLog(getFullName() + " used Blood Blast!");

    int totalDam = 0;
    for (unsigned int i = 0; i < defenders.size(); i++)
    {
        // Check to see if you get a critical
        int attackMult = 3;

        int critChance = capZero(getCriticalRate() -
                         defenders[i]->getCriticalEvade());
        int r = getRandNum();
        if (critChance >= r)
            attackMult *= 3;

        // Deal the damage
        int dam = capOne(capOne(getStr() - defenders[i]->getDef()) * attackMult);
        log->addToEventLog(getFullName() + " dealt " +
                           to_string(dam) + " damage to " +
                           defenders[i]->getFullName() + ".");
        defenders[i]->subHP(dam, NP_STR);

        totalDam += dam;
    }

    log->addToEventLog("Blood Blast did " + to_string(totalDam) + " total damage!");

    // Create the Blood Blast buff
    vector<Stat> dStats;
    dStats.push_back(HP);
    vector<int> dAmount;
    dAmount.push_back(totalDam);
    Debuff *blood = new Debuff("Blood Blast",
                               "Hurting others heals you...",
                               getTeam(), dStats, dAmount, 4);
    addDebuff(blood);

    return 0;
}
