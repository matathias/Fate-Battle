#include "servantriderwyvern.h"
#include "PlayField.h"

ServantRiderWyvern::ServantRiderWyvern(int as, Team t, Logger *l) : ServantRider (as, t, l)
{
    name = "Wyvern Rider";
    mountKilled = false;

    /** Stat modifiers **/
    int resMod = 5;
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
    mov[2] += movMod;

    actionList[0].push_back("2: Dragonborn");
    actionList[0].push_back("3: NP: Fus");
    actionList[1].push_back("2: Dragonborn");
    actionList[1].push_back("3: NP: Fus");
    actionList[1].push_back("4: NP: Fus Ro");
    actionList[2].push_back("2: Dragonborn");
    actionList[2].push_back("3: NP: Fus");
    actionList[2].push_back("4: NP: Fus Ro");
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
    actionMPCosts[0].push_back(20);
    actionMPCosts[1].push_back(0);
    actionMPCosts[1].push_back(20);
    actionMPCosts[1].push_back(85);
    actionMPCosts[2].push_back(0);
    actionMPCosts[2].push_back(20);
    actionMPCosts[2].push_back(85);
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
    np1.push_back("Fus");
    np1.push_back("Behold the power of the voice! At a cost of 20 MP, shove all units within two spaces back by two spaces.");
    vector<string> np2;
    np2.push_back("Fus Ro");
    np2.push_back("At a cost of 85 MP, shove all units within two spaces back by four spaces. All enemy units receive 15 HP damage.");
    vector<string> np3;
    np3.push_back("Fus Ro Dah");
    np3.push_back("At a cost of 175 MP, shove all units within three spaces to the edge of the map. All enemy units receive .15*MAXHP damage. All enemy units are immobilized for 1 turn.");
    noblePhantasms.push_back(np1);
    noblePhantasms.push_back(np2);
    noblePhantasms.push_back(np3);

    // Fus range
    npRanges.push_back(getLowToHighRange(1,2));
    // Fus Ro range
    npRanges.push_back(getLowToHighRange(1,2));
    // Fus Ro Dah range
    npRanges.push_back(getLowToHighRange(1,3));
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
    dStats.push_back(MOV);
    vector<int> dAmounts;
    dAmounts.push_back(0.2 * getStr());
    dAmounts.push_back(0.2 * getDef());
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
// Fus
int ServantRiderWyvern::activateNP1(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][2] > currMP)
        return 1; // Not enough MP to attack

    subMP(actionMPCosts[ascension][2]);

    int shoveDistance = 2;

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
    }

    return 0;
}

// Fus Ro
int ServantRiderWyvern::activateNP2(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][3] > currMP)
        return 1; // Not enough MP to attack

    subMP(actionMPCosts[ascension][3]);

    int shoveDistance = 5;

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
