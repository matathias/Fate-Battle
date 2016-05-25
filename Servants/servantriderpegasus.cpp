#include "servantriderpegasus.h"
#include "PlayField.h"

ServantRiderPegasus::ServantRiderPegasus(int as, Team t, Logger *l) : ServantRider(as, t, l)
{
    name = "Pegasus Rider";

    /** Stat modifiers **/
    /*int resMod = 10;
    int spdMod = 10;
    int movMod = 1;

    spd[0] += spdMod;
    spd[1] += spdMod;
    spd[2] += spdMod;

    res[0] += resMod;
    res[1] += resMod;
    res[2] += resMod;

    mov[0] += movMod;
    mov[1] += movMod;
    mov[2] += movMod;*/

    actionList[0].push_back("2: Wings of Icarus");
    actionList[0].push_back("3: NP: Rare Foal");
    actionList[1].push_back("2: Wings of Icarus");
    actionList[1].push_back("3: NP: Rare Foal");
    actionList[2].push_back("2: Wings of Icarus");
    actionList[2].push_back("3: NP: Rare Foal");
    actionList[2].push_back("4: NP: Legendary Stallion");

    actionListTypes[0].push_back(N);
    actionListTypes[0].push_back(A);
    actionListTypes[1].push_back(N);
    actionListTypes[1].push_back(A);
    actionListTypes[2].push_back(N);
    actionListTypes[2].push_back(A);
    actionListTypes[2].push_back(S);

    actionMPCosts[0].push_back(25);
    actionMPCosts[0].push_back(40);
    actionMPCosts[1].push_back(25);
    actionMPCosts[1].push_back(40);
    actionMPCosts[2].push_back(25);
    actionMPCosts[2].push_back(40);
    actionMPCosts[2].push_back(200);

    actionDodgeable[0].push_back(false);
    actionDodgeable[0].push_back(false);
    actionDodgeable[1].push_back(false);
    actionDodgeable[1].push_back(false);
    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(false);

    actionCounterable[0].push_back(false);
    actionCounterable[0].push_back(false);
    actionCounterable[1].push_back(false);
    actionCounterable[1].push_back(false);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);

    vector<string> np1;
    np1.push_back("Rare Foal");
    np1.push_back("Your opponents are astounded by the beauty of your mount and become distracted by staring at it, leaving you the opportunity to step on their faces. At a cost of 40 MP, attack all opponents within two spaces for magic damage. Always hits.");
    vector<string> np2;
    np2.push_back("Mythical Steed");
    np2.push_back("The mythical status of your steed is dazzling and inspires in everyone an odd sense of conservation. The urge not to kill your Pegasus is strong -- so strong, in fact, that anyone who attacks you may be subconsciously swayed to purposefully miss. The chance of this occurring is (LUK * 2.5).");
    vector<string> np3;
    np3.push_back("Legendary Stallion");
    np3.push_back("The Pegasus isn’t legendary for nothing! This is your ultimate attack -- charge forth with the fury of a legendary stallion to deal five times magic damage. Charge forward 10 spaces, and deal damage to all in or adjacent to your path. Must have a target unit. Costs 200 MP.");
    noblePhantasms.push_back(np1);
    noblePhantasms.push_back(np2);
    noblePhantasms.push_back(np3);

    // Rare Foal range
    npRanges.push_back(getLowToHighRange(1,2));
    // Mythical Steed doesn't have a range
    npRanges.push_back(getLowToHighRange(0,0));
    // Legendary Stallion range
    npRanges.push_back(getLowToHighRange(1,10));

    /** Passive Skill modifiers **/
    vector<Stat> hdS;
    hdS.push_back(RES);
    hdS.push_back(SPD);
    hdS.push_back(MOV);
    vector<int> hdA;
    hdA.push_back(10);
    hdA.push_back(10);
    hdA.push_back(1);
    Debuff *highDivinity = new Debuff("Is This Fire Emblem?", "Passive Skill",
                                      t, hdS, hdA, -1);
    addDebuff(highDivinity);

    if (ascension >= 1)
    {
        hdS.clear();
        hdS.push_back(MOV);
        hdA.clear();
        hdA.push_back(0);
        Debuff *highDivinity2 = new Debuff("Mythical Steed", "Passive Noble Phantasm",
                                          t, hdS, hdA, -1);
        addDebuff(highDivinity2);
    }
}

/***** Active Skills *****/
int ServantRiderPegasus::wingsOfIcarus()
{
    if (actionMPCosts[ascension][1] > currMP)
        return 1; // Not enough MP to attack

    if (highInTheSky())
        return 41; // You're already up there... for some reason.

    subMP(actionMPCosts[ascension][1]);

    // Setup the "buff"
    vector<Stat> tS;
    tS.push_back(MOV);
    vector<int> tA;
    tA.push_back(0);
    Debuff *wings = new Debuff("Wings of Icarus", "You flew high in the sky!",
                               getTeam(), tS, tA, 1);

    addDebuff(wings);

    log->addToEventLog(getFullName() + " activated Wings of Icarus and flew up high in the sky!");

    return 0;
}

/***** Helper Functions *****/
bool ServantRiderPegasus::highInTheSky()
{
    bool high = false;

    for (unsigned int i = 0; i < debuffs.size() && !high; i++)
    {
        if (debuffs[i]->getDebuffName().compare("Wings of Icarus") == 0)
            high = true;
    }

    return high;
}

/***** Function Re-definitions *****/
void ServantRiderPegasus::subHP(int hp, DamageType dt)
{
    bool noDamage = false;

    if (dt != OMNI && highInTheSky())
    {
        log->addToEventLog(getFullName() + " is flying high in the sky! They take no damage!");
        noDamage = true;
    }

    if (!noDamage)
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
}

vector<int> ServantRiderPegasus::getEvade()
{
    // Evasion = Speed * 2 + Luck
    vector<int> evade;
    evade.push_back(getInitialEvade());
    // Mythical Steed = Luck (only add this one at the right ascension)
    if (ascension >= 1)
        evade.push_back(getLuk());
    return evade;
}

int ServantRiderPegasus::isActionNP(int action)
{
    int ret = -1;
    if (action == 2)
        ret = 0;
    else if (action == 3)
        ret = 1;

    return ret;
}

vector<Coordinate> ServantRiderPegasus::getActionRange(int action)
{
    vector<Coordinate> range;

    // Attack
    if (action == 0)
        range = getLowToHighRange(getLowRange(), getHighRange());
    // Wings of Icarus
    else if (action == 1)
        range = getLowToHighRange(0,0);
    // Rare Foal
    else if (action == 2)
        range = getNPRange(0);
    // Legendary Stallion
    else
        range = getNPRange(2);

    return range;
}

int ServantRiderPegasus::doAction(int actionNum, vector<Servant *> defenders)
{
    int ret = 0;
    if (actionNum == 0)
        ret = attack(defenders, true);
    else if (actionNum == 1)
        ret = wingsOfIcarus();
    else if (actionNum == 2)
        ret = activateNP1(defenders);
    else
        ret = activateNP2(defenders);

    return ret;
}

/***** Noble Phantasms *****/
int ServantRiderPegasus::activateNP1(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][2] > currMP)
        return 1; // Not enough MP to attack

    subMP(actionMPCosts[ascension][2]);

    log->addToEventLog(getFullName() + " engaged Rare Foal!");

    for (unsigned int i = 0; i < defenders.size(); i++)
    {
        int dam = 0;
        // No accuracy check since Rare Foal always hits

        // If you hit, calculate crit chance
        int attackMult = 1;
        int critChance = capZero(getCriticalRate() -
                         defenders[i]->getCriticalEvade());
        int r = getRandNum();
        if (critChance >= r)
            attackMult *= 3;

        // Deal the damage
        dam = capZero(getMag() - defenders[i]->getRes()) * attackMult;
        log->addToEventLog(getFullName() + " dealt " +
                           to_string(dam) + " damage to " +
                           defenders[i]->getFullName() + ".");
        defenders[i]->subHP(dam, NP_MAG);

        // Check to see if the defender is dead. If they are and they are an
        //  Avenger, activate Final Revenge.
        // If they are not dead but they are a Berserker, check to see if
        //  Mad Counter activates.
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

int ServantRiderPegasus::activateNP2(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][3] > currMP)
        return 1; // Not enough MP to attack

    // Get the landing location
    //  If there is no valid landing location next to the target servant,
    //  stop processing and return 41.
    Coordinate landingCoord = getEndLocation(defenders[0]->getCurrLoc(), 4);
    if(landingCoord.x == -1 && landingCoord.y == -1)
    {
        // No valid adjacent space
        log->addToErrorLog("No valid adjacent space to target.");
        return 41;
    }

    subMP(actionMPCosts[ascension][3]);
    Coordinate startCoord = getCurrLoc();

    // Move to be adjacent to the target servant
    log->addToEventLog(getFullName() + " engaged Legendary Stallion!");
    log->addToEventLog(getFullName() +
                       " charged to space (" + to_string(landingCoord.x) +
                       "," + to_string(landingCoord.y) + ") from (" +
                       to_string(currLoc.x) + "," + to_string(currLoc.y) +
                       ") and attacked everyone on the way!");
    field->moveServant(this, landingCoord);

    // Get the path
    vector<Coordinate> path = field->pathFind(startCoord, landingCoord);

    // Get all spaces adjacent to the path
    vector<Coordinate> allPath = field->getAdjacentToRange(path);

    // Get all Servants on those spaces
    vector<Servant*> targets = field->getAllInRange2(allPath);

    // Deal 5 times magic damage to all of the targets
    for (unsigned int i = 0; i < targets.size(); i++)
    {
        // Don't wanna accidentally attack yourself
        if (targets[i] == this)
            continue;

        int dam = 0;
        // No accuracy check since Legendary Stallion always hits

        // Calculate crit chance
        int attackMult = 5;
        int defCritEv = targets[i]->getCriticalEvade();
        int critChance = capZero(getCriticalRate() - defCritEv);
        int r = getRandNum();
        if (critChance >= r)
            attackMult *= 3;

        // Deal the damage
        dam = capZero(getMag() - targets[i]->getRes()) * attackMult;
        log->addToEventLog(getFullName() + " dealt " +
                           to_string(dam) + " damage to " +
                           targets[i]->getFullName() + ".");
        targets[i]->subHP(dam, NP_MAG);

        // Check to see if the defender is dead. If they are and they are an
        //  Avenger, activate Final Revenge.
        // If they are not dead but they are a Berserker, check to see if
        //  Mad Counter activates.
        if(targets[i]->getCurrHP() <= 0)
        {
            if (targets[i]->getClass() == Avenger)
            {
                // Activate Final Revenge
                Debuff *finRev = targets[i]->finalRevenge();
                addDebuff(finRev);
                if (targets[i]->getAscensionLvl() == 2)
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
