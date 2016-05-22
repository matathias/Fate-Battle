#include "servantlancernaginata.h"
#include "PlayField.h"

ServantLancerNaginata::ServantLancerNaginata(int as, Team t, Logger *l) : ServantLancer(as, t, l)
{
    name = "Naginata Lancer";

    /** Stat modifiers **/
    /*int strMod = -5;
    int spdMod = 10;
    str[0] += strMod;
    str[1] += strMod;
    str[2] += strMod;
    spd[0] += spdMod;
    spd[1] += spdMod;
    spd[2] += spdMod;*/

    actionList[0].push_back("3: NP: Blade Dash");
    actionList[1].push_back("3: NP: Blade Dash");
    actionList[1].push_back("4: NP: Blade Charge");
    actionList[2].push_back("3: NP: Blade Dash");
    actionList[2].push_back("4: NP: Blade Charge");
    actionList[2].push_back("5: NP: Blade Force");

    // The NPs are treated as single-target so that the player is forced to
    // choose a target unit. The AOE affects are calculated later.
    actionListTypes[0].push_back(S);
    actionListTypes[1].push_back(S);
    actionListTypes[1].push_back(S);
    actionListTypes[2].push_back(S);
    actionListTypes[2].push_back(S);
    actionListTypes[2].push_back(S);

    actionMPCosts[0].push_back(10);
    actionMPCosts[1].push_back(10);
    actionMPCosts[1].push_back(30);
    actionMPCosts[2].push_back(10);
    actionMPCosts[2].push_back(30);
    actionMPCosts[2].push_back(50);

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
    np1.push_back("Blade Dash");
    np1.push_back("Costs 10 MP. Dash forward up to 2 spaces and attack an enemy unit adjacent to your ending location. Attack for 1.5 times normal damage. Always hits. Cannot be used without a target unit.");
    vector<string> np2;
    np2.push_back("Blade Charge");
    np2.push_back("Costs 30 MP. Dash forward up to 3 spaces and attack all enemies adjacent to your ending location. Attack for 3 times normal damage. Always hits. Cannot be used without a target unit.");
    vector<string> np3;
    np3.push_back("Blade Force");
    np3.push_back("Costs 50 MP. Dash forward up to 4 spaces and attack all enemies within 2 spaces of your ending location. Attack for 5 times normal damage. Always hits. Cannot be used without a target unit.");
    noblePhantasms.push_back(np1);
    noblePhantasms.push_back(np2);
    noblePhantasms.push_back(np3);

    vector<Coordinate> npc1;
    vector<Coordinate> npc2;
    vector<Coordinate> npc3;

    int hiNPRange = 5;
    for (int i = -(hiNPRange); i <= hiNPRange; i++)
    {
        for (int j = -(hiNPRange); j <= hiNPRange; j++)
        {
            Coordinate tempC;
            tempC.x = i; tempC.y = j;

            if (abs(i) + abs(j) <= 3)
                npc1.push_back(tempC);
            if (abs(i) + abs(j) <= 4)
                npc2.push_back(tempC);
            if (abs(i) + abs(j) <= 5)
                npc3.push_back(tempC);
        }
    }

    npRanges.push_back(npc1);
    npRanges.push_back(npc2);
    npRanges.push_back(npc3);

    /** Passive Skill modifiers **/
    vector<Stat> hdS;
    hdS.push_back(STR);
    hdS.push_back(SPD);
    vector<int> hdA;
    hdA.push_back(-5);
    hdA.push_back(10);
    Debuff *highDivinity = new Debuff("Dancing Blade", "Passive Skill",
                                      t, hdS, hdA, -1);
    addDebuff(highDivinity);

    hdS.clear();
    hdS.push_back(MOV);
    hdA.clear();
    hdA.push_back(0);
    Debuff *highDivinity2 = new Debuff("Blade Deflector", "Passive Skill",
                                      t, hdS, hdA, -1);
    addDebuff(highDivinity2);
}

vector<int> ServantLancerNaginata::getEvade()
{
    // Evasion = Speed * 2 + Luck
    vector<int> evade;
    evade.push_back(getInitialEvade());
    // Eye of the Mind: (SKL / 4) chance of taking no damage.
    evade.push_back(getSkl() / 4);
    // Blade Deflector: (SKL / 4) chance of taking no damage.
    evade.push_back(getSkl() / 4);
    return evade;
}

int ServantLancerNaginata::activateNP1(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][2] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        // Get the landing location
        //  If there is no valid landing location next to the target servant,
        //  stop processing and return 41.
        Coordinate landingCoord = getEndLocation(defenders[0]->getCurrLoc(), 2);
        if(landingCoord.x == -1 && landingCoord.y == -1)
        {
            // No valid adjacent space
            log->addToErrorLog("No valid adjacent space to target.");
            return 41;
        }

        subMP(actionMPCosts[ascension][2]);

        // Move to be adjacent to the target servant
        log->addToEventLog(getFullName() + " used Blade Dash!");
        log->addToEventLog(getFullName() + " dashed to space (" +
                           to_string(landingCoord.x) + "," +
                           to_string(landingCoord.y) + ") from (" +
                           to_string(currLoc.x) + "," + to_string(currLoc.y) +
                           ") and attacked " + defenders[0]->getFullName() +
                           ".");
        field->moveServant(this, landingCoord);

        // Do damage to the target servant
        return doDamage(defenders[0], 1.5);
    }
}

int ServantLancerNaginata::activateNP2(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][3] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        // Get the landing location
        //  If there is no valid landing location next to the target servant,
        //  stop processing and return 41.
        Coordinate landingCoord = getEndLocation(defenders[0]->getCurrLoc(), 3);
        if(landingCoord.x == -1 && landingCoord.y == -1)
        {
            // No valid adjacent space
            log->addToErrorLog("No valid adjacent space to target.");
            return 41;
        }

        subMP(actionMPCosts[ascension][3]);

        // Move to be adjacent to the target servant
        log->addToEventLog(getFullName() + " used Blade Charge!");
        log->addToEventLog(getFullName() +
                           " dashed to space (" + to_string(landingCoord.x) +
                           "," + to_string(landingCoord.y) + ") from (" +
                           to_string(currLoc.x) + "," + to_string(currLoc.y) +
                           ") and attacked all adjacent units.");
        field->moveServant(this, landingCoord);

        // Get all Servants adjacent to this location
        vector<Coordinate> landRange = getLowToHighRange(1,1);
        vector<Servant*> trueDefenders = field->getAllInRange(this, landRange);

        for (unsigned int i = 0; i < trueDefenders.size(); i++)
        {
            // Do damage to the target servant
            doDamage(trueDefenders[i], 3);
        }

        return 0;
    }
}

int ServantLancerNaginata::activateNP3(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][4] > currMP)
        return 1; // Not enough MP to attack
    else
    {
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

        subMP(actionMPCosts[ascension][4]);

        // Move to be adjacent to the target servant
        log->addToEventLog(getFullName() + " used Blade Force!");
        log->addToEventLog(getFullName() +
                           " dashed to space (" + to_string(landingCoord.x) +
                           "," + to_string(landingCoord.y) + ") from (" +
                           to_string(currLoc.x) + "," + to_string(currLoc.y) +
                           ") and attacked all adjacent units.");
        field->moveServant(this, landingCoord);

        // Get all Servants adjacent to this location
        vector<Coordinate> landRange = getLowToHighRange(1,2);
        vector<Servant*> trueDefenders = field->getAllInRange(this, landRange);

        for (unsigned int i = 0; i < trueDefenders.size(); i++)
        {
            // Do damage to the target servant
            doDamage(trueDefenders[i], 5);
        }

        return 0;
    }
}

/***** Helper functions *****/
vector<Coordinate> ServantLancerNaginata::getCoordRange(int range)
{
    vector<Coordinate> retC;
    for (int i = -(range); i <= range; i++)
    {
        for (int j = -(range); j <= range; j++)
        {
            if (abs(i) + abs(j) <= range)
            {
                Coordinate rc;
                rc.x = i; rc.y = j;
                retC.push_back(rc);
            }
        }
    }

    return retC;
}

int ServantLancerNaginata::doDamage(Servant *defender, double damMult)
{
    int dam = capZero(getStr() - defender->getDef()) * damMult;
    log->addToEventLog(getFullName() + " dealt " +
                       to_string(dam) + " damage to " +
                       defender->getFullName() + ".");
    defender->subHP(dam, NP_STR);

    // Check to see if the defender is dead.
    if(defender->getCurrHP() <= 0)
    {
        if (defender->getClass() == Avenger)
        {
            // Activate Final Revenge
            Debuff *finRev = defender->finalRevenge();
            addDebuff(finRev);
            if (defender->getAscensionLvl() == 2)
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

    return 0;
}
