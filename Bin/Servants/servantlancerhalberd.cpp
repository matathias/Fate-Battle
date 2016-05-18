#include "servantlancerhalberd.h"
#include "PlayField.h"

#include <iostream>

ServantLancerHalberd::ServantLancerHalberd(int as, Team t, Logger *l) : ServantLancer(as, t, l)
{
    name = "Halberd Lancer";

    /** Stat modifiers **/
    int strMod = 5;
    str[0] += strMod;
    str[1] += strMod;
    str[2] += strMod;

    actionList[0].push_back("3: NP: Piercing Blow");
    actionList[1].push_back("3: NP: Piercing Blow");
    actionList[1].push_back("4: NP: Swinging Lunge");
    actionList[2].push_back("3: NP: Piercing Blow");
    actionList[2].push_back("4: NP: Swinging Lunge");
    actionList[2].push_back("5: NP: Dance of Death");

    actionListTypes[0].push_back(S);
    actionListTypes[1].push_back(S);
    actionListTypes[1].push_back(A);
    actionListTypes[2].push_back(S);
    actionListTypes[2].push_back(A);
    actionListTypes[2].push_back(A);

    actionMPCosts[0].push_back(15);
    actionMPCosts[1].push_back(15);
    actionMPCosts[1].push_back(50);
    actionMPCosts[2].push_back(15);
    actionMPCosts[2].push_back(50);
    actionMPCosts[2].push_back(100);

    actionDodgeable[0].push_back(false);
    actionDodgeable[1].push_back(false);
    actionDodgeable[1].push_back(true);
    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(true);
    actionDodgeable[2].push_back(true);

    actionCounterable[0].push_back(false);
    actionCounterable[1].push_back(false);
    actionCounterable[1].push_back(false);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);

    vector<string> np1;
    np1.push_back("Piercing Blow");
    np1.push_back("Spend 15 MP to have one of two things happen: 1) stab your foe with the Halberd’s spear edge, causing -5 to DEF and a Bleedout (-10 MAXHP per turn for 3 turns) debuff, or 2) lay flat your foe with the Halberd’s axe edge, causing -3 to DEF and immobilizing them for 1 turn. There is a 50% chance of either occurring. No accuracy or critical check.");
    vector<string> np2;
    np2.push_back("Swinging Lunge");
    np2.push_back("Spend 50 MP to lunge forward up to 5 squares and attack all enemy servants in or adjacent to your path for 3 times normal damage. Accuracy and critical checks apply to every blow.");
    vector<string> np3;
    np3.push_back("Dance of Death");
    np3.push_back("Costs 100 MP. Whirling your weapon around in a mad frenzy, randomly attack ten players within two squares of you with Piercing Blow (players can be targeted more than once; the process is entirely RNG) at 2 times normal damage. Finish with a Swinging Lunge in the direction of your choice, traveling for up to 8 squares instead of 5 and dealing 6 times normal damage instead of 3.");
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

    vector<Coordinate> npc1;
    npc1.push_back(c1);
    npc1.push_back(c2);
    npc1.push_back(c3);
    npc1.push_back(c4);

    vector<Coordinate> npc2;
    vector<Coordinate> npc3;

    for (int i = 1; i <= 8; i++)
    {
        Coordinate nc1, nc2, nc3;
        nc1.x = 0; nc1.y = i;
        nc2.x = -1; nc2.y = i;
        nc3.x = 1; nc3.y = i;
        if (i <= 5)
        {
            npc2.push_back(nc1);
            npc2.push_back(nc2);
            npc2.push_back(nc3);
        }
        npc3.push_back(nc1);
        npc3.push_back(nc2);
        npc3.push_back(nc3);
    }

    npRanges.push_back(npc1);
    npRanges.push_back(npc2);
    npRanges.push_back(npc3);
}

int ServantLancerHalberd::attack(vector<Servant *> defenders, bool counter)
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

                // If the opponent is a Rider, do triple damage
                if (defenders[i]->getClass() == Rider)
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
    }

    return 0;
}

int ServantLancerHalberd::activateNP1(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][2] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][2]);

        log->addToEventLog(getFullName() + "used Piercing Blow!");

        return piercingBlow(defenders, 1);
    }
}

int ServantLancerHalberd::activateNP2(vector<Servant *> defenders)
{
    int ret = 0;
    if (actionMPCosts[ascension][3] > currMP)
        ret = 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][3]);

        log->addToEventLog(getFullName() + " used Swinging Lunge!");

        ret = swingingLunge(defenders, 3);

        Direction atkDir = getDirectionAtoB(getCurrLoc(), defenders[0]->getCurrLoc());

        Coordinate finalC;
        finalC.x = currLoc.x;
        finalC.y = currLoc.y;
        for (int i = 0; i <= 5; i++)
        {
            Coordinate tempC;
            tempC.x = currLoc.x;
            tempC.y = currLoc.y;
            if(atkDir == NORTH)
            {
                tempC.y = currLoc.y + i;
                if (!field->doesSpaceHaveServant(tempC) && field->isCoordinateInBounds(tempC))
                    finalC = tempC;
            }
            else if (atkDir == SOUTH)
            {
                tempC.y = currLoc.y - i;
                if (!field->doesSpaceHaveServant(tempC) && field->isCoordinateInBounds(tempC))
                    finalC = tempC;
            }
            else if (atkDir == EAST)
            {
                tempC.x = currLoc.x + i;
                if (!field->doesSpaceHaveServant(tempC) && field->isCoordinateInBounds(tempC))
                    finalC = tempC;
            }
            else
            {
                tempC.x = currLoc.x - i;
                if (!field->doesSpaceHaveServant(tempC) && field->isCoordinateInBounds(tempC))
                    finalC = tempC;
            }
        }

        if (finalC.x != currLoc.x || finalC.y != currLoc.y)
        {
            log->addToEventLog("Swinging Lunge launched you from (" +
                               to_string(currLoc.x) + "," + to_string(currLoc.y)
                               + ") to (" + to_string(finalC.x) + "," +
                               to_string(finalC.y) + ")!");
        }
        else
            log->addToEventLog("Swinging Lunge left you in place.");

        field->moveServant(this, finalC);
    }

    return ret;
}

int ServantLancerHalberd::activateNP3(vector<Servant *> defenders)
{
    int ret = 0;
    if (actionMPCosts[ascension][4] > currMP)
        ret = 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][4]);

        log->addToEventLog(getFullName() + " used Dance of Death!");

        // Produce a vector of all spaces within two spaces of the lancer's
        //  current location.
        vector<Coordinate> twoSpaces;
        for (int i = -2; i <= 2; i++)
        {
            for (int j = -2; j <= 2; j++)
            {
                if (abs(i) + abs(j) <= 2 && !(i == 0 && j == 0))
                {
                    Coordinate newLoc;
                    newLoc.x = i;
                    newLoc.y = j;
                    twoSpaces.push_back(newLoc);
                }
            }
        }

        // Get a list of Servants within two spaces of the lancer
        vector<Servant*> targets = field->getAllInRange(this, twoSpaces);
        // Make sure no one in the target list is on the same team as the lancer
        for (int i = 0; i < (int) targets.size(); i++)
        {
            if(targets[i]->getTeam() == getTeam())
            {
                targets.erase(targets.begin()+i);
                i--;
            }
        }

        // Make ten piercing blow attacks against random servants within two
        //  spaces. If there are no servants within two spaces, however, just
        //  skip this.
        for (int i = 0; i < 10 && targets.size() > 0; i++)
        {
            int randVal = rand() % targets.size();
            vector<Servant*> tempDefenders;
            tempDefenders.push_back(targets[randVal]);
            ret = piercingBlow(tempDefenders, 2);
        }

        // Attack the initial defenders list with Swinging Lunge
        ret = swingingLunge(defenders, 6);

        // Get the direction of attack and then launch the player forwards
        Direction atkDir = getDirectionAtoB(getCurrLoc(), defenders[0]->getCurrLoc());

        Coordinate finalC;
        finalC.x = currLoc.x;
        finalC.y = currLoc.y;
        for (int i = 0; i <= 8; i++)
        {
            Coordinate tempC;
            tempC.x = currLoc.x;
            tempC.y = currLoc.y;
            if(atkDir == NORTH)
            {
                tempC.y = currLoc.y + i;
                if (!field->doesSpaceHaveServant(tempC) && field->isCoordinateInBounds(tempC))
                    finalC = tempC;
            }
            else if (atkDir == SOUTH)
            {
                tempC.y = currLoc.y - i;
                if (!field->doesSpaceHaveServant(tempC) && field->isCoordinateInBounds(tempC))
                    finalC = tempC;
            }
            else if (atkDir == EAST)
            {
                tempC.x = currLoc.x + i;
                if (!field->doesSpaceHaveServant(tempC) && field->isCoordinateInBounds(tempC))
                    finalC = tempC;
            }
            else
            {
                tempC.x = currLoc.x - i;
                if (!field->doesSpaceHaveServant(tempC) && field->isCoordinateInBounds(tempC))
                    finalC = tempC;
            }
        }

        if (finalC.x != currLoc.x || finalC.y != currLoc.y)
        {
            log->addToEventLog("Dance of Death launched you from (" +
                               to_string(currLoc.x) + "," + to_string(currLoc.y)
                               + ") to (" + to_string(finalC.x) + "," +
                               to_string(finalC.y) + ")!");
        }
        else
            log->addToEventLog("Dance of Death left you in place.");

        field->moveServant(this, finalC);
    }

    return ret;
}

/***** Helper functions *****/
int ServantLancerHalberd::piercingBlow(vector<Servant *> defenders, int damMult)
{
    for (unsigned int i = 0; i < defenders.size(); i++)
    {
        // Deal the damage

        // If the opponent is a Rider, do triple damage
        if (defenders[i]->getClass() == Rider)
            damMult *= 3;

        int dam = capZero(getStr() - defenders[i]->getDef()) * damMult;
        log->addToEventLog(getFullName() + " dealt " +
                           to_string(dam) + " damage to " +
                           defenders[i]->getFullName() + ".");
        defenders[i]->subHP(dam, NP_STR);

        // Check to see if the defender is dead.
        if(defenders[i]->getCurrHP() > 0)
        {
            // Apply one of the two debuffs from Piercing Blow
            int randVal = getRandNum() % 100;
            if (randVal < 50)
                piercingTip(defenders[i]);
            else
                piercingEdge(defenders[i]);

            // Check if the defender is a Berserker. If they are, and they
            // are adjacent to this unit, check to see if Mad Counter
            // activates.
            if (defenders[i]->getClass() == Berserker &&
                isAdjacent(defenders[i]))
            {
                int r = getRandNum();
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

int ServantLancerHalberd::piercingTip(Servant* defender)
{
    vector<Stat> debStat;
    debStat.push_back(DEF);
    debStat.push_back(MAXHP);
    vector<int> debAmount;
    debAmount.push_back(-5);
    debAmount.push_back(-10);

    Debuff *tipDebuff = new Debuff("Piercing Blow - Stabbed",
                                   "You were stabbed by a Halberdier's Piercing Blow.",
                                   defender->getTeam(), debStat, debAmount, 4);
    defender->addDebuff(tipDebuff);

    return 0;
}

int ServantLancerHalberd::piercingEdge(Servant* defender)
{
    vector<Stat> debStat;
    debStat.push_back(DEF);
    debStat.push_back(MOV);
    vector<int> debAmount;
    debAmount.push_back(-3);
    debAmount.push_back(-1 * defender->getMov());

    Debuff *tipDebuff = new Debuff("Piercing Blow - Flattened",
                                   "You were flattened by a Halberdier's Piercing Blow.",
                                   defender->getTeam(), debStat, debAmount, 2);
    defender->addDebuff(tipDebuff);

    return 0;
}

int ServantLancerHalberd::swingingLunge(vector<Servant *> defenders, int damMult)
{
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
            int attackMult = damMult;
            int critChance = capZero(getCriticalRate() -
                             defenders[i]->getCriticalEvade());
            r = getRandNum();
            if (critChance >= r)
                attackMult *= 3;

            // If the opponent is a Rider, do triple damage
            if (defenders[i]->getClass() == Rider)
                attackMult *= 3;

            // Deal the damage
            dam = capZero(getStr() - defenders[i]->getDef()) * attackMult;
            log->addToEventLog(getFullName() + " dealt " +
                               to_string(dam) + " damage to " +
                               defenders[i]->getFullName() + ".");
            defenders[i]->subHP(dam, NP_STR);
        }
        else
        {
            log->addToEventLog(getFullName() + " missed " +
                               defenders[i]->getFullName() + "!");
        }

        // Check to see if the defender is dead. If they are and they are an
        //  Avenger, activate Final Revenge.
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
