#include "servantlancerlance.h"
#include "PlayField.h"

#include <iostream>

ServantLancerLance::ServantLancerLance(int as, Team t, Logger *l) : ServantLancer(as, t, l)
{
    name = "Lance Lancer";

    /** Stat modifiers **/
    //int strMod = 10;
    //int spdMod = -10;
    hiRange += 1;

    /*spd[0] += spdMod;
    spd[1] += spdMod;
    spd[2] += spdMod;

    str[0] += strMod;
    str[1] += strMod;
    str[2] += strMod;*/

    actionList[0].push_back("3: NP: Titanreach");
    actionList[1].push_back("3: NP: Titanreach");
    actionList[1].push_back("4: NP: Essence of Gae Bolg");
    actionList[2].push_back("3: NP: Titanreach");
    actionList[2].push_back("4: NP: Essence of Gae Bolg");
    actionList[2].push_back("5: NP: Essence of Gae Bolg (Anti-Army)");

    actionListTypes[0].push_back(A);
    actionListTypes[1].push_back(A);
    actionListTypes[1].push_back(S);
    actionListTypes[2].push_back(A);
    actionListTypes[2].push_back(S);
    actionListTypes[2].push_back(A);

    actionMPCosts[0].push_back(15);
    actionMPCosts[1].push_back(15);
    actionMPCosts[1].push_back(80);
    actionMPCosts[2].push_back(15);
    actionMPCosts[2].push_back(80);
    actionMPCosts[2].push_back(95);

    actionDodgeable[0].push_back(false);
    actionDodgeable[1].push_back(false);
    actionDodgeable[1].push_back(false);
    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(false);

    actionCounterable[0].push_back(false);
    actionCounterable[1].push_back(false);
    actionCounterable[1].push_back(true);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(true);
    actionCounterable[2].push_back(false);

    vector<string> np1;
    np1.push_back("Titanreach");
    np1.push_back("Using your weapon’s incredible range, you lash out faster than any opponent can respond. Attack all enemy units in a two-length cone for undodgeable 2 times normal damage. Costs 15 MP.");
    vector<string> np2;
    np2.push_back("Essence of Gae Bolg");
    np2.push_back("Gotta have them fate references right? At the cost of 80 MP, instigate an insta-kill attack against an adjacent target. Target has a chance equal to (Their LUK - Your LUK) of taking no damage. Unlike the real Gae Bolg, this attack consumes a lot of mana due to not being attached to the actual spear or heroic spirit.");
    vector<string> np3;
    np3.push_back("Essence of Gae Bolg (Anti-Army)");
    np3.push_back("Attack everything in a straight line until the edge of the playing field for 20 times normal damage. Costs 95 MP.");
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

    Coordinate cn1, cn2, cn3;
    cn1.x = -1; cn1.y = 2;
    cn2.x = 0; cn2.y = 2;
    cn3.x = 1; cn2.y = 2;
    vector<Coordinate> npc1;
    npc1.push_back(c2);
    npc1.push_back(cn1);
    npc1.push_back(cn2);
    npc1.push_back(cn3);

    vector<Coordinate> npc2;
    npc2.push_back(c1);
    npc2.push_back(c2);
    npc2.push_back(c3);
    npc2.push_back(c4);

    npRanges.push_back(npc1);
    npRanges.push_back(npc2);

    /** Passive Skill modifiers **/
    vector<Stat> hdS;
    hdS.push_back(STR);
    hdS.push_back(SPD);
    vector<int> hdA;
    hdA.push_back(10);
    hdA.push_back(-10);
    Debuff *highDivinity = new Debuff("Lance Arent For Infantry", "Passive Skill",
                                      t, hdS, hdA, -1);
    addDebuff(highDivinity);

    hdS.clear();
    hdS.push_back(MOV);
    hdA.clear();
    hdA.push_back(0);
    Debuff *highDivinity2 = new Debuff("The Longest Polearm", "Passive Skill",
                                      t, hdS, hdA, -1);
    addDebuff(highDivinity2);
}

void ServantLancerLance::setPlayField(PlayField *f)
{
    field = f;
    int ran = field->getLargestDimension();

    vector<Coordinate> npc3;
    for (int i = 1; i <= ran; i++)
    {
        Coordinate nc1, nc2, nc3;
        nc1.x = 0; nc1.y = i;
        nc2.x = -1; nc2.y = i;
        nc3.x = 1; nc3.y = i;
        npc3.push_back(nc1);
        npc3.push_back(nc2);
        npc3.push_back(nc3);
    }

    npRanges.push_back(npc3);
}

// Titanreach
int ServantLancerLance::activateNP1(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][2] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][2]);

        log->addToEventLog(getFullName() + " used Titanreach!");

        for (unsigned int i = 0; i < defenders.size(); i++)
        {
            // Check to see if you get a critical
            int attackMult = 2;
            int critChance = capZero(getCriticalRate() -
                             defenders[i]->getCriticalEvade());
            int r = getRandNum();
            if (critChance >= r)
                attackMult *= 3;

            // Deal the damage
            int dam = capZero(getStr() - defenders[i]->getDef()) * attackMult;
            log->addToEventLog(getFullName() + " dealt " +
                               to_string(dam) + " damage to " +
                               defenders[i]->getFullName() + ".");
            defenders[i]->subHP(dam, NP_STR);

            // Check to see if the defender is dead. If they are, and if they
            //  are an Avenger, activate Final Revenge.
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
    }
    return 0;
}

// Essence of Gae Bolg
int ServantLancerLance::activateNP2(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][3] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][3]);

        log->addToEventLog(getFullName() + " used Essence of Gae Bolg!");

        for (unsigned int i = 0; i < defenders.size(); i++)
        {
            int dam = defenders[i]->getMaxHP();

            // See if the opposing servant passes the luck check
            if (defenders[i]->getRandNum() <= defenders[i]->getLuk() - getLuk())
            {
                // They passed the check!
                dam = 0;
            }

            // Deal the damage
            log->addToEventLog(getFullName() + " dealt " +
                               to_string(dam) + " damage to " +
                               defenders[i]->getFullName() + ".");
            defenders[i]->subHP(dam, GAEBOLG);

            // Check to see if the defender is dead. If they are, do not call
            // the counterattack. Additionally, if they are an Avenger and they
            // die, activate Final Revenge.
            if(defenders[i]->getCurrHP() > 0)
            {
                // Call "attack" on the defending servant for their
                // counterattack, if you are in their range and you are the
                // initiating servant.
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

// Essence of Gae Bolg (Anti-Army)
int ServantLancerLance::activateNP3(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][4] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][4]);

        log->addToEventLog(getFullName() + " used Essence of Gae Bolg (Anti-Army)!");

        for (unsigned int i = 0; i < defenders.size(); i++)
        {
            int attackMult = 20;

            // Deal the damage
            int dam = capZero(getStr() - defenders[i]->getDef()) * attackMult;
            log->addToEventLog(getFullName() + " dealt " +
                               to_string(dam) + " damage to " +
                               defenders[i]->getFullName() + ".");
            defenders[i]->subHP(dam, NP_STR);
        }
    }

    return 0;
}
