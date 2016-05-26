#include "servantriderwyvern.h"
#include "PlayField.h"

ServantRiderWyvern::ServantRiderWyvern(int as, Team t, Logger *l) : ServantRider (as, t, l)
{
    name = "Wyvern Rider";
    mountKilled = false;

    /** Stat modifiers **/
    /*int resMod = 5;
    int spdMod = -15;
    int strMod = 15;
    int defMod = 10;
    int movMod = -1;

    spd[0] += spdMod;
    spd[1] += spdMod;
    spd[2] += spdMod;
    res[0] += resMod;
    res[1] += resMod;
    res[2] += resMod;
    str[0] += strMod;
    str[1] += strMod;
    str[2] += strMod;
    def[0] += defMod;
    def[1] += defMod;
    def[2] += defMod;
    mov[0] += movMod;
    mov[1] += movMod;
    mov[2] += movMod;*/

    actionList[0].push_back("2: Dragonborn");
    actionList[0].push_back("3: NP: Yol Toor Shul");
    actionList[1].push_back("2: Dragonborn");
    actionList[1].push_back("3: NP: Yol Toor Shul");
    actionList[1].push_back("4: NP: Gaan Lah Haas");
    actionList[2].push_back("2: Dragonborn");
    actionList[2].push_back("3: NP: Yol Toor Shul");
    actionList[2].push_back("4: NP: Gaan Lah Haas");
    actionList[2].push_back("5: NP: Fus Ro Dah");

    actionListTypes[0].push_back(N);
    actionListTypes[0].push_back(A);
    actionListTypes[1].push_back(N);
    actionListTypes[1].push_back(A);
    actionListTypes[1].push_back(A);
    actionListTypes[2].push_back(N);
    actionListTypes[2].push_back(A);
    actionListTypes[2].push_back(A);
    actionListTypes[2].push_back(A);

    actionMPCosts[0].push_back(0);
    actionMPCosts[0].push_back(40);
    actionMPCosts[1].push_back(0);
    actionMPCosts[1].push_back(40);
    actionMPCosts[1].push_back(140);
    actionMPCosts[2].push_back(0);
    actionMPCosts[2].push_back(40);
    actionMPCosts[2].push_back(140);
    actionMPCosts[2].push_back(175);

    actionDodgeable[0].push_back(false);
    actionDodgeable[0].push_back(false);
    actionDodgeable[1].push_back(false);
    actionDodgeable[1].push_back(false);
    actionDodgeable[1].push_back(false);
    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(false);

    actionCounterable[0].push_back(false);
    actionCounterable[0].push_back(false);
    actionCounterable[1].push_back(false);
    actionCounterable[1].push_back(false);
    actionCounterable[1].push_back(false);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);

    vector<string> np1;
    np1.push_back("Yol Toor Shul");
    np1.push_back("Inhale air, exhale flame, and behold the Thu’um as inferno. At a cost of 40 MP, deal unresisted magic damage to all in a 3-length cone, leaving all victims with a \"Burned\" (-10 HP/turn for 4 turns) debuff.");
    vector<string> np2;
    np2.push_back("Gaan Lah Haas");
    np2.push_back("Coax both magical and mortal energies from your hapless opponent. At an upfront cost of 100 MP, steal 15 HP and 10 MP from every opponent in a 2-length cone, leaving them with a \"Drain Vitality\" (-10 HP/turn and -5 MP/turn for 3 turns) debuff, and giving yourself a \"Gain Vitality\" (+10 HP/turn and +5 MP/turn for 3 turns) buff for each target.");
    vector<string> np3;
    np3.push_back("Fus Ro Dah");
    np3.push_back("At a cost of 175 MP, shove all units within three spaces to the edge of the map. All enemy units receive .15*MAXHP damage. All enemy units are immobilized for 1 turn.");
    noblePhantasms.push_back(np1);
    noblePhantasms.push_back(np2);
    noblePhantasms.push_back(np3);

    Coordinate c1;
    c1.x = 0; c1.y = -1;
    Coordinate c2;
    c2.x = 0; c2.y = 1;
    Coordinate c3;
    c3.x = 1; c3.y = 0;
    Coordinate c4;
    c4.x = -1; c4.y = 0;

    Coordinate cn1, cn2, cn3, cn4, cn5, cn6, cn7, cn8;
    cn1.x = -1; cn1.y = 2;
    cn2.x = 0; cn2.y = 2;
    cn3.x = 1; cn3.y = 2;
    cn4.x = -2; cn4.y = 3;
    cn5.x = -1; cn5.y = 3;
    cn6.x = 0; cn6.y = 3;
    cn7.x = 1; cn7.y = 3;
    cn8.x = 2; cn8.y = 3;
    vector<Coordinate> npc3;
    npc3.push_back(c2);
    npc3.push_back(cn1);
    npc3.push_back(cn2);
    npc3.push_back(cn3);
    npc3.push_back(cn4);
    npc3.push_back(cn5);
    npc3.push_back(cn6);
    npc3.push_back(cn7);
    npc3.push_back(cn8);
    vector<Coordinate> npc1;
    npc1.push_back(c2);
    npc1.push_back(cn1);
    npc1.push_back(cn2);
    npc1.push_back(cn3);
    // Yol Toor Shul range
    npRanges.push_back(npc3);
    // Gaan Lah Haas range
    npRanges.push_back(npc1);
    // Fus Ro Dah range
    npRanges.push_back(getLowToHighRange(1,3));

    /** Passive Skill modifiers **/
    vector<Stat> hdS;
    hdS.push_back(MOV);
    hdS.push_back(STR);
    hdS.push_back(DEF);
    hdS.push_back(RES);
    hdS.push_back(SPD);
    vector<int> hdA;
    hdA.push_back(-1);
    hdA.push_back(15);
    hdA.push_back(10);
    hdA.push_back(5);
    hdA.push_back(-15);
    Debuff *highDivinity = new Debuff("Strength of the Dragon", "Passive Skill",
                                      t, hdS, hdA, -1);
    addDebuff(highDivinity);
}

/***** Active Skills *****/
int ServantRiderWyvern::dragonborn()
{
    // No MP cost, so no need to check for that. We DO need to check that the
    //  player is actually under 20% HP, and that they haven't already used this
    //  skill
    if (mountKilled)
    {
        log->addToErrorLog("You already killed your mount!");
        return 41;
    }
    else if (getCurrHP() > 0.25 * getMaxHP())
    {
        log->addToErrorLog("Your HP isn't low enough to use Skill: Dragonborn!");
        return 41;
    }

    // Replenish HP
    addHP(0.25 * getMaxHP());

    // Replenish MP
    addMP(0.5 * getMaxMP());

    // Create the Dragonborn buff
    vector<Stat> dStats;
    dStats.push_back(STR);
    dStats.push_back(DEF);
    dStats.push_back(SPD);
    dStats.push_back(MOV);
    vector<int> dAmounts;
    dAmounts.push_back(0.5 * getStr());
    dAmounts.push_back(0.5 * getDef());
    dAmounts.push_back(0.25 * getSpd());
    dAmounts.push_back(-1 * (getMov() - 4));
    Debuff *dragon = new Debuff("Dragonborn",
                                "You absorbed the soul of a dragon, thereby gaining a stat boost.",
                                getTeam(), dStats, dAmounts, -1);

    addDebuff(dragon);

    log->addToEventLog(getFullName() + " killed their mount and absorbed its soul!");
    mountKilled = true;

    return 0;
}

/***** Function Re-Definitions *****/
int ServantRiderWyvern::isActionNP(int action)
{
    int ret = -1;
    if (action >= 2)
    {
        ret = action-2;
    }
    return ret;
}

vector<Coordinate> ServantRiderWyvern::getActionRange(int action)
{
    vector<Coordinate> range;

    // Attack
    if (action == 0)
        range = getLowToHighRange(getLowRange(), getHighRange());
    // Dragonborn
    else if (action == 1)
        range = getLowToHighRange(0,0);
    // Noble Phantasms
    else
        range = getNPRange(action - 2);

    return range;
}

int ServantRiderWyvern::doAction(int actionNum, vector<Servant *> defenders)
{
    int ret = 0;
    if (actionNum == 0)
        ret = attack(defenders, true);
    else if (actionNum == 1)
        ret = dragonborn();
    else if (actionNum == 2)
        ret = activateNP1(defenders);
    else if (actionNum == 3)
        ret = activateNP2(defenders);
    else
        ret = activateNP3(defenders);

    return ret;
}

/***** Noble Phantasms *****/
// Yol Toor Shul
int ServantRiderWyvern::activateNP1(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][2] > currMP)
        return 1; // Not enough MP to attack

    subMP(actionMPCosts[ascension][2]);

    // Original Fus Version
    /*int shoveDistance = 2;

    log->addToEventLog(getFullName() + " used the Shout \"Fus\"!");

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
    }*/

    log->addToEventLog(getFullName() + " used Yol Toor Shul!");

    for (unsigned int i = 0; i < defenders.size(); i++)
    {
        // Unresisted Magic Damage
        int dam = getMag();

        vector<Stat> tStats;
        tStats.push_back(HP);
        vector<int> tAmounts;
        tAmounts.push_back(-10);
        Debuff *flameDeb = new Debuff("Burn", "You have been burned!",
                                      defenders[i]->getTeam(), tStats,
                                      tAmounts, 5);

        defenders[i]->addDebuff(flameDeb);

        log->addToEventLog(getFullName() + " dealt " +
                           to_string(dam) + " damage to " +
                           defenders[i]->getFullName() + ".");
        defenders[i]->subHP(dam, NP_MAG);

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

// Gaan Lah Haas
int ServantRiderWyvern::activateNP2(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][3] > currMP)
        return 1; // Not enough MP to attack

    subMP(actionMPCosts[ascension][3]);

    // Original Fus Ro code
    /*int shoveDistance = 5;

    log->addToEventLog(getFullName() + " used the Shout \"Fus Ro\"!");

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

        // Have the Servant take 15 HP in damage
        defenders[i]->subHP(15, NP_STR);

        log->addToEventLog(defenders[i]->getFullName() +
                           " was shoved from (" +
                           to_string(targetLoc.x) + "," + to_string(targetLoc.y)
                           + ") to (" + to_string(actualLoc.x) + "," +
                           to_string(actualLoc.y) + ") and took 15 damage.");
    }*/

    log->addToEventLog(getFullName() + " used Gaan Lah Haas!");

    for (unsigned int i = 0; i < defenders.size(); i++)
    {
        int dam = 15;
        int mpDam = 10;

        defenders[i]->subHP(dam, NP_MAG);
        addHP(dam);
        defenders[i]->subMP(mpDam);
        addMP(mpDam);

        vector<Stat> tStats;
        tStats.push_back(HP);
        tStats.push_back(MP);
        vector<int> tAmounts;
        tAmounts.push_back(-10);
        tAmounts.push_back(-5);
        Debuff *drain = new Debuff("Drain Vitality", "You fell victim to the Thu'um...",
                                   defenders[i]->getTeam(), tStats,
                                   tAmounts, 4);
        defenders[i]->addDebuff(drain);

        vector<Stat> tStat;
        tStat.push_back(HP);
        tStat.push_back(MP);
        vector<int> tAmount;
        tAmount.push_back(10);
        tAmount.push_back(5);
        Debuff *gain = new Debuff("Gain Vitality",
                                  "Your Thu'um strengthens you.",
                                  getTeam(), tStat, tAmount, 4);
        addDebuff(gain);

        log->addToEventLog(getFullName() + " absorbed " +
                           to_string(dam) + " HP and " + to_string(mpDam) +
                           " MP from " + defenders[i]->getFullName() + ".");

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

// Fus Ro Dah
int ServantRiderWyvern::activateNP3(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][4] > currMP)
        return 1; // Not enough MP to attack

    subMP(actionMPCosts[ascension][4]);

    log->addToEventLog(getFullName() + " used the Shout \"Fus Ro Dah\"!!");

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
            finalLoc.y = field->getFieldLength() - 1;
        }
        else if (fromPlayer == SOUTH)
        {
            finalLoc.x = targetLoc.x;
            finalLoc.y = 0;
        }
        else if (fromPlayer == EAST)
        {
            finalLoc.x = field->getFieldWidth() - 1;
            finalLoc.y = targetLoc.y;
        }
        else // West
        {
            finalLoc.x = 0;
            finalLoc.y = targetLoc.y;
        }

        // Get the nearest valid coordinate from the "finalLoc"
        Coordinate actualLoc = field->getNearestValidCoord(finalLoc);

        // Move the target Servant to this location
        field->moveServant(defenders[i], actualLoc);

        // Have the Servant take .15*MAXHP in damage
        int dam = 0.15 * defenders[i]->getMaxHP();
        defenders[i]->subHP(dam, NP_STR);

        // Give the target an Immobilized Debuff
        vector<Stat> tStats;
        tStats.push_back(MOV);
        vector<int> tAmounts;
        tAmounts.push_back(-1 * defenders[i]->getMov());
        Debuff *immob = new Debuff("Immobilized",
                                   "Fus Ro Dah left you reeling and unable to move!",
                                   defenders[i]->getTeam(), tStats, tAmounts,
                                   2);

        defenders[i]->addDebuff(immob);

        log->addToEventLog(defenders[i]->getFullName() +
                           " was shoved from (" +
                           to_string(targetLoc.x) + "," + to_string(targetLoc.y)
                           + ") to (" + to_string(actualLoc.x) + "," +
                           to_string(actualLoc.y) + ") and took " +
                           to_string(dam) + " damage.");
    }

    return 0;
}
