#pragma once

#include "servanttest.h"
#include <iostream>

ServantTest::ServantTest(int as, Team t, Logger *l) : ServantSaber(as, t, l)
{
    name = "Saber Test";

    actionList[0].push_back("4: NP: Straight Slash");
    actionList[1].push_back("4: NP: Straight Slash");
    actionList[1].push_back("5: NP: Side Slash");
    actionList[2].push_back("4: NP: Straight Slash");
    actionList[2].push_back("5: NP: Side Slash");
    actionList[2].push_back("6: NP: Omni Slash");

    actionListTypes[0].push_back(S);
    actionListTypes[1].push_back(S);
    actionListTypes[1].push_back(S);
    actionListTypes[2].push_back(S);
    actionListTypes[2].push_back(S);
    actionListTypes[2].push_back(A);

    actionMPCosts[0].push_back(25);
    actionMPCosts[1].push_back(25);
    actionMPCosts[1].push_back(50);
    actionMPCosts[2].push_back(25);
    actionMPCosts[2].push_back(50);
    actionMPCosts[2].push_back(100);

    actionDodgeable[0].push_back(true);
    actionDodgeable[1].push_back(true);
    actionDodgeable[1].push_back(false);
    actionDodgeable[2].push_back(true);
    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(false);

    actionCounterable[0].push_back(true);
    actionCounterable[1].push_back(true);
    actionCounterable[1].push_back(true);
    actionCounterable[2].push_back(true);
    actionCounterable[2].push_back(true);
    actionCounterable[2].push_back(false);

    vector<string> np1;
    np1.push_back("Straight Slash");
    np1.push_back("Attack straight-on for thrice normal damage.");
    vector<string> np2;
    np2.push_back("Side Slash");
    np2.push_back("Attack from the side, catching your opponent off-guard and dealing undodgeable five times normal damage.");
    vector<string> np3;
    np3.push_back("Omni Slash");
    np3.push_back("Attack all adjacent opponents for undodgeable five times normal damage. No counterattacks or enemy skill activations.");
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
    npc2.push_back(c1);
    npc2.push_back(c2);
    npc2.push_back(c3);
    npc2.push_back(c4);
    vector<Coordinate> npc3;
    npc3.push_back(c1);
    npc3.push_back(c2);
    npc3.push_back(c3);
    npc3.push_back(c4);
    npRanges.push_back(npc1);
    npRanges.push_back(npc2);
    npRanges.push_back(npc3);
}

// Straight Slash
// Attack straight-on for thrice normal damage.
int ServantTest::activateNP1(vector<Servant *> defenders)
{
    int ret = 0;
    if (actionMPCosts[ascension][3] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][3]);
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
            else if (opEvade.size() > 1)
            {
                for (unsigned int j = 1; j < opEvade.size() && !hit; j++)
                {
                    r = getRandNum();
                    if (opEvade[j] <= r)
                        hit = true;
                }
            }

            // If you hit, calculate crit chance
            if (hit)
            {
                int attackMult = 3;
                int critChance = capZero(getCriticalRate() -
                                 defenders[i]->getCriticalEvade());
                r = getRandNum();
                if (critChance >= r)
                    attackMult *= 3;

                // Deal the damage
                dam = capZero(getStr() - defenders[i]->getDef()) * attackMult;
                if (dam < 0)
                    dam = 0;
                defenders[i]->subHP(dam, NP_STR);
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
                        subHP(dam, C_STR);
                    }
                }
                // Call "attack" on the defending servant for their
                // counterattack, if you are in their range.
                if (defenders[i]->isInRange(this))
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

    return ret;
}

// Side Slash
// Attack from the side, catching your opponent off-guard and dealing
// undodgeable five times normal damage.
int ServantTest::activateNP2(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][4] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][4]);
        for (unsigned int i = 0; i < defenders.size(); i++)
        {
            // Check to see if you get a critical
            int attackMult = 5;
            int critChance = capZero(getCriticalRate() -
                             defenders[i]->getCriticalEvade());
            int r = getRandNum();
            if (critChance >= r)
                attackMult *= 3;

            // Deal the damage
            int dam = capZero(getStr() - defenders[i]->getDef()) * attackMult;
            if (dam < 0)
                dam = 0;
            defenders[i]->subHP(dam, NP_STR);

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
                        subHP(dam, C_STR);
                    }
                }
                // Call "attack" on the defending servant for their
                // counterattack, if you are in their range.
                if (defenders[i]->isInRange(this))
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

// Omni Slash
// Attack all adjacent opponents for undodgeable five times normal damage. No
// counterattacks or enemy skill activations.
int ServantTest::activateNP3(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][5] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][5]);
        for (unsigned int i = 0; i < defenders.size(); i++)
        {
            // Check to see if you get a critical
            int attackMult = 5;
            int critChance = capZero(getCriticalRate() -
                             defenders[i]->getCriticalEvade());
            int r = getRandNum();
            if (critChance >= r)
                attackMult *= 3;

            // Deal the damage
            int dam = capZero(getStr() - defenders[i]->getDef()) * attackMult;
            if (dam < 0)
                dam = 0;
            defenders[i]->subHP(dam, NP_STR);
        }
    }
    return 0;
}

int ServantTest::doAction(int actionNum, vector<Servant *> defenders)
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
            ret = activateNP1(defenders);
            break;
        case 4:
            ret = activateNP2(defenders);
            break;
        case 5:
            ret = activateNP3(defenders);
            break;
        default:
            return 2; // Not a valid choice
            break;
    }
    return ret;
}
