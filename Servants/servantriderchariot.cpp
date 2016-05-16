#include "servantriderchariot.h"
#include "PlayField.h"

ServantRiderChariot::ServantRiderChariot(int as, Team t, Logger *l) : ServantRider(as, t, l)
{
    name = "Chariot Rider";

    /** Stat modifiers **/
    int resMod = 5;
    int spdMod = 5;
    int defMod = 5;

    spd[0] += spdMod;
    spd[1] += spdMod;
    spd[2] += spdMod;
    res[0] += resMod;
    res[1] += resMod;
    res[2] += resMod;
    def[0] += defMod;
    def[1] += defMod;
    def[2] += defMod;

    actionList[0].push_back("2: Whip of the Master");
    actionList[0].push_back("3: NP: Charge of the Vanguard");
    actionList[1].push_back("2: Whip of the Master");
    actionList[1].push_back("3: NP: Charge of the Vanguard");
    actionList[1].push_back("4: NP: Call In the Cavalry");
    actionList[2].push_back("2: Whip of the Master");
    actionList[2].push_back("3: NP: Charge of the Vanguard");
    actionList[2].push_back("4: NP: Call In the Cavalry");
    actionList[2].push_back("5: NP: Essence of Ionioi Hetairoi");

    actionListTypes[0].push_back(N);
    actionListTypes[0].push_back(S);
    actionListTypes[1].push_back(N);
    actionListTypes[1].push_back(S);
    actionListTypes[1].push_back(N);
    actionListTypes[2].push_back(N);
    actionListTypes[2].push_back(S);
    actionListTypes[2].push_back(N);
    actionListTypes[2].push_back(T);

    actionMPCosts[0].push_back(30);
    actionMPCosts[0].push_back(40);
    actionMPCosts[1].push_back(30);
    actionMPCosts[1].push_back(40);
    actionMPCosts[1].push_back(120);
    actionMPCosts[2].push_back(30);
    actionMPCosts[2].push_back(40);
    actionMPCosts[2].push_back(120);
    actionMPCosts[2].push_back(200);

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
    np1.push_back("Charge of the Vanguard");
    np1.push_back("At a cost of 40 MP, charge forward five spaces, dealing damage to and flattening anyone in your path. All victims receive normal damage and are immobilized for two turns. Must have a target unit.");
    vector<string> np2;
    np2.push_back("Call In the Cavalry");
    np2.push_back("At a cost of 120 MP, gain a 100% increase to MOV for 3 turns. While this effect is active, you get one guaranteed hit classed as a Noble Phantasm attack and one regular hit (subject to an accuracy check) when you attack. Both attacks are for 2 times magic damage.");
    vector<string> np3;
    np3.push_back("Essence of Ionioi Hetairoi");
    np3.push_back("At a cost of 200 MP, create a Reality Marble around the whole playing field. All allied units have a LUK chance of having a second move while the Reality Marble is active, and receive a 10% buff to all stats. The Marble takes 20 MP per turn to maintain and cannot be cast over another Reality Marble.");
    noblePhantasms.push_back(np1);
    noblePhantasms.push_back(np2);
    noblePhantasms.push_back(np3);

    // Charge of the Vanguard range
    npRanges.push_back(getLowToHighRange(1,5));
    // Call In the Cavalry range (doesn't really have one)
    npRanges.push_back(getLowToHighRange(0,0));
    // Essence of Ionioi Hetairoi range (doesn't really have one)
    npRanges.push_back(getLowToHighRange(0,0));
}

/***** Active Skills *****/
int ServantRiderChariot::whipOfTheMaster()
{
    if (actionMPCosts[ascension][1] > currMP)
        return 1; // Not enough MP to attack

    if (whipOfTheMasterActive())
        return 41;

    subMP(actionMPCosts[ascension][1]);

    // Setup the "buff"
    vector<Stat> tS;
    tS.push_back(MOV);
    vector<int> tA;
    tA.push_back(2);
    Debuff *whip = new Debuff("Whip of the Master",
                               "You whip your horse to encourage it to run faster.",
                               getTeam(), tS, tA, 3);

    addDebuff(whip);

    log->addToEventLog(getFullName() + " whipped their horse to go faster!");
}

/***** Private Helper Functions *****/
bool ServantRiderChariot::callInTheCavalryActive()
{
    bool isExtended = false;
    for (int i = 0; i < (int) debuffs.size(); i++)
    {
        if(debuffs[i]->getDebuffName().compare("Call In the Cavalry") == 0)
        {
            isExtended = true;
        }
    }

    return isExtended;
}

bool ServantRiderChariot::whipOfTheMasterActive()
{
    bool isExtended = false;
    for (int i = 0; i < (int) debuffs.size(); i++)
    {
        if(debuffs[i]->getDebuffName().compare("Whip of the Master") == 0)
        {
            isExtended = true;
        }
    }

    return isExtended;
}

/***** Function Re-definitions *****/
int ServantRiderChariot::attack(vector<Servant *> defenders, bool counter)
{
    if (actionMPCosts[ascension][0] > currMP)
        return 1; // Not enough MP to attack

    subMP(actionMPCosts[ascension][0]);
    for (unsigned int i = 0; i < defenders.size(); i++)
    {
        int dam = 0;
        int attackMult = 1;
        int critChance = capZero(getCriticalRate() -
                         defenders[i]->getCriticalEvade());
        int r = 0;

        // If Call In the Cavalry is active, get a free guaranteed hit
        if (callInTheCavalryActive())
        {
            r = getRandNum();
            if (r <= critChance)
                attackMult *= 3;

            attackMult *= 2;
            dam = capZero(getMag() - defenders[i]->getRes()) * attackMult;
            log->addToEventLog(getFullName() + " dealt " +
                               to_string(dam) + " damage to " +
                               defenders[i]->getFullName() + ".");
            defenders[i]->subHP(dam, NP_MAG);
            attackMult = 1;
        }

        // Check if you hit the targets
        vector<int> opEvade = defenders[i]->getEvade();
        bool hit = false;
        // Calculate accuracy
        int accuracy = capZero(getHitRate() - opEvade[0]);

        r = getRandNum();
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
            r = getRandNum();
            if (critChance >= r)
            {
                attackMult *= 3;
                // Add a trample debuff to the defender
                string descrip = "You were trampled by " + getFullName() +
                                " and immobilized!";
                vector<Stat> dStats;
                dStats.push_back(MOV);
                vector<int> dAmount;
                dAmount.push_back(-1 * defenders[i]->getMov());
                Debuff *trample = new Debuff("Trample", descrip,
                                             defenders[i]->getTeam(),
                                             dStats, dAmount, 2);
                defenders[i]->addDebuff(trample);
            }

            // Deal the damage
            // If Call In the Cavalry is active, hit twice for 2 times magic
            // damage. Otherwise, just hit once for physical damage.
            if (!callInTheCavalryActive())
            {
                dam = capZero(getStr() - defenders[i]->getDef()) * attackMult;
                log->addToEventLog(getFullName() + " dealt " +
                                   to_string(dam) + " damage to " +
                                   defenders[i]->getFullName() + ".");
                defenders[i]->subHP(dam, D_STR);
            }
            else
            {
                attackMult *= 2;
                dam = capZero(getMag() - defenders[i]->getRes()) * attackMult;
                log->addToEventLog(getFullName() + " dealt " +
                                   to_string(dam) + " damage to " +
                                   defenders[i]->getFullName() + ".");
                defenders[i]->subHP(dam, D_MAG);
            }
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
                                       "' Mad Counter activated, dealing " +
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

    return 0;
}

int ServantRiderChariot::isActionNP(int action)
{
    int ret = -1;
    if (action >= 2)
    {
        ret = action-2;
    }
    return ret;
}

bool ServantRiderChariot::isActionRealityMarble(int action)
{
    return (action == 4);
}

vector<Coordinate> ServantRiderChariot::getActionRange(int action)
{
    vector<Coordinate> range;

    // Attack
    if (action == 0)
        range = getLowToHighRange(getLowRange(), getHighRange());
    // Whip of the Master
    else if (action == 1)
        range = getLowToHighRange(0,0);
    // Noble Phantasms
    else
        range = getNPRange(action - 2);

    return range;
}

int ServantRiderChariot::getRealityMarbleMP()
{
    return 20;
}

int ServantRiderChariot::doAction(int actionNum, vector<Servant *> defenders)
{
    int ret = 0;
    if (actionNum == 0)
        ret = attack(defenders, true);
    else if (actionNum == 1)
        ret = whipOfTheMaster();
    else if (actionNum == 2)
        ret = activateNP1(defenders);
    else if (actionNum == 3)
        ret = activateNP2(defenders);
    else
        ret = activateNP3(defenders);

    return ret;
}

/***** Noble Phantasms *****/
// Charge of the Vanguard
int ServantRiderChariot::activateNP1(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][2] > currMP)
        return 1; // Not enough MP to attack

    // Get the landing location
    //  If there is no valid landing location next to the target servant,
    //  stop processing and return 41.
    Coordinate landingCoord = getEndLocation(defenders[0]->getCurrLoc(), 10);
    if(landingCoord.x == -1 && landingCoord.y == -1)
    {
        // No valid adjacent space
        log->addToErrorLog("No valid adjacent space to target.");
        return 41;
    }

    subMP(actionMPCosts[ascension][2]);
    Coordinate startCoord = getCurrLoc();

    // Move to be adjacent to the target servant
    log->addToEventLog(getFullName() + " initiated Charge of the Vanguard!");
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

    // Deal undodgeable physical damage and immobilize to all of the targets
    for (unsigned int i = 0; i < targets.size(); i++)
    {
        // Don't wanna accidentally attack yourself
        if (targets[i] == this)
            continue;

        int dam = 0;
        // No accuracy check since Charge of the Vanguard always hits

        // Calculate crit chance
        int attackMult = 1;
        int critChance = capZero(getCriticalRate() -
                         defenders[i]->getCriticalEvade());
        int r = getRandNum();
        if (critChance >= r)
            attackMult *= 3;

        // Deal the damage
        dam = capZero(getStr() - defenders[i]->getDef()) * attackMult;
        log->addToEventLog(getFullName() + " dealt " +
                           to_string(dam) + " damage to " +
                           defenders[i]->getFullName() + ".");
        defenders[i]->subHP(dam, NP_STR);

        // Immobilize the target for 2 turns.
        string descrip = "You were trampled by " + getFullName() +
                        " and immobilized!";
        vector<Stat> dStats;
        dStats.push_back(MOV);
        vector<int> dAmount;
        dAmount.push_back(-1 * defenders[i]->getMov());
        Debuff *trample = new Debuff("Trample", descrip,
                                     defenders[i]->getTeam(),
                                     dStats, dAmount, 3);
        defenders[i]->addDebuff(trample);

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

// Call In the Cavalry
int ServantRiderChariot::activateNP2(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][3] > currMP)
        return 1; // Not enough MP to attack

    if (callInTheCavalryActive())
        return 41; // Can't use this NP while it is already active

    subMP(actionMPCosts[ascension][3]);

    log->addToEventLog(getFullName() + " called in the cavalry!");

    // Add a "Call In the Cavalry" buff to yourself
    vector<Stat> dStats;
    dStats.push_back(MOV);
    vector<int> dAmount;
    dAmount.push_back(getMov());
    Debuff *cavalry = new Debuff("Call In the Cavalry",
                                 "You ARE the cavalry, and as such receive a significant boost!",
                                 getTeam(), dStats, dAmount, 4);
    addDebuff(cavalry);

    return 0;
}

// Essence of Ionioi Hetairoi
int ServantRiderChariot::activateNP3(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][4] > currMP)
        return 1; // Not enough MP to attack

    subMP(actionMPCosts[ascension][4]);

    // Do the actual reality marble stuff
    realityMarbleActive = true;

    // Stat boost to allied units
    vector<Stat> tstats;
    tstats.push_back(MAXHP);
    tstats.push_back(MAXMP);
    tstats.push_back(STR);
    tstats.push_back(MAG);
    tstats.push_back(DEF);
    tstats.push_back(RES);
    tstats.push_back(SPD);
    tstats.push_back(SKL);
    tstats.push_back(LUK);
    int buffAmount = 5;
    vector<int> tAmounts;
    tAmounts.push_back(buffAmount);
    tAmounts.push_back(buffAmount);
    tAmounts.push_back(buffAmount);
    tAmounts.push_back(buffAmount);
    tAmounts.push_back(buffAmount);
    tAmounts.push_back(buffAmount);
    tAmounts.push_back(buffAmount);
    tAmounts.push_back(buffAmount);
    tAmounts.push_back(buffAmount);
    Debuff *eih = new Debuff("Reality Marble", "Essence of Ionioi Hetairoi",
                             getTeam(), tstats, tAmounts, -1);

    field->startRealityMarble(this, eih);

    log->addToEventLog(getFullName() + " activated a Reality Marble! Essence of Ionioi Hetairoi is now in effect!");

    return 0;
}
