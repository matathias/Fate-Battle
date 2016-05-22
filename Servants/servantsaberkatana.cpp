#include "servantsaberkatana.h"

ServantSaberKatana::ServantSaberKatana(int as, Team t, Logger *l) : ServantSaber(as, t, l)
{
    name = "Katana Saber";

    /** Stat modifiers **/
    /*int strMod = -5;
    int spdMod = 5;

    spd[0] += spdMod;
    spd[1] += spdMod;
    spd[2] += spdMod;

    str[0] += strMod;
    str[1] += strMod;
    str[2] += strMod;*/

    actionList[0].push_back("4: NP: Tensutoraiki");
    actionList[1].push_back("4: NP: Tensutoraiki");
    actionList[1].push_back("5: NP: Handoreddosutoraiki");
    actionList[2].push_back("4: NP: Tensutoraiki");
    actionList[2].push_back("5: NP: Handoreddosutoraiki");
    actionList[2].push_back("6: NP: Sauzandosutoraiki");

    actionListTypes[0].push_back(S);
    actionListTypes[1].push_back(S);
    actionListTypes[1].push_back(S);
    actionListTypes[2].push_back(S);
    actionListTypes[2].push_back(S);
    actionListTypes[2].push_back(S);

    actionMPCosts[0].push_back(30);
    actionMPCosts[1].push_back(30);
    actionMPCosts[1].push_back(75);
    actionMPCosts[2].push_back(30);
    actionMPCosts[2].push_back(75);
    actionMPCosts[2].push_back(100);

    actionDodgeable[0].push_back(true);
    actionDodgeable[1].push_back(true);
    actionDodgeable[1].push_back(true);
    actionDodgeable[2].push_back(true);
    actionDodgeable[2].push_back(true);
    actionDodgeable[2].push_back(true);

    actionCounterable[0].push_back(false);
    actionCounterable[1].push_back(false);
    actionCounterable[1].push_back(false);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);

    vector<string> np1;
    np1.push_back("Tensutoraiki");
    np1.push_back("At a cost of 30 MP, attack an adjacent opponent ten times in a row for half damage each strike. Each attack has its own accuracy and critical check. Enemy cannot counterattack but enemy skills can activate. GNSFOOT cannot activate.");
    vector<string> np2;
    np2.push_back("Handoreddosutoraiki");
    np2.push_back("At a cost of 75 MP, attack an adjacent opponent one hundred times for 10% damage each strike. Each attack has its own accuracy and critical check. Enemy cannot counterattack but enemy skills can activate. GNSFOOT cannot activate.");
    vector<string> np3;
    np3.push_back("Sauzandosutoraiki");
    np3.push_back("At a cost of 100 MP, attack an adjacent opponent one thousand times for 5% damage each strike. Each attack has its own accuracy and critical check. Enemy cannot counterattack and enemy skills cannot activate. GNSFOOT can activate.");
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

    /** Passive Skills **/
    vector<Stat> hdS;
    hdS.push_back(MAXMP);
    vector<int> hdA;
    hdA.push_back(0);
    Debuff *highDivinity3 = new Debuff("GLORIOUS NIPPON STEEL",
                                       "Passive Skill",
                                      t, hdS, hdA, -1);
    addDebuff(highDivinity3);

    hdS.clear();
    hdS.push_back(STR);
    hdS.push_back(SPD);
    hdA.clear();
    hdA.push_back(-5);
    hdA.push_back(8);
    Debuff *highDivinity2 = new Debuff("Katanas Are Actually Pretty Weak", "Passive Skill",
                                      t, hdS, hdA, -1);
    addDebuff(highDivinity2);
}

int ServantSaberKatana::doAction(int actionNum, vector<Servant *> defenders)
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

int ServantSaberKatana::attack(vector<Servant *> defenders, bool counter)
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

                // Skill: GLORIOUS NIPPON STEEL FOLDED OVER 100 TIMES
                r = getRandNum();
                if (getSkl() >= r)
                    attackMult *= 2;

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

// Tensutoraiki
int ServantSaberKatana::activateNP1(vector<Servant *> defenders)
{
    int ret = 0;
    if (actionMPCosts[ascension][3] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][3]);

        log->addToEventLog(getFullName() + " used Tensutoraiki!");

        for (unsigned int i = 0; i < defenders.size(); i++)
        {
            int totalDam = 0;

            for (int z = 0; z < 10; z++)
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
                    double attackMult = 0.5;
                    int critChance = capZero(getCriticalRate() -
                                     defenders[i]->getCriticalEvade());
                    r = getRandNum();
                    if (critChance >= r)
                        attackMult *= 3;

                    // Deal the damage
                    dam = (int) capOne((capOne(getStr() - defenders[i]->getDef()) *
                                 attackMult));
                    totalDam += dam;
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

                // Check to see if the defender is dead. If they are, do not
                //  call the counterattack. Additionally, if they are an Avenger
                //  and they die, activate Final Revenge.
                // If they are not dead but they are a Berserker, check to see
                // if Mad Counter activates.
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
                }
                else
                {
                    z = 10;
                    if (defenders[i]->getClass() == Avenger)
                    {
                        // Activate Final Revenge
                        Debuff *finRev = defenders[i]->finalRevenge();
                        addDebuff(finRev);
                        if (defenders[i]->getAscensionLvl() == 2)
                        {
                            subHP(0.1 * getMaxHP(), OMNI);
                            subMP(0.1 * getMaxMP());

                            if (getCurrHP() == 0)
                            {
                                setHP(1);
                            }
                        }
                    }
                }
            }

            log->addToEventLog(getFullName() + " dealt " +
                               to_string(totalDam) + " total damage to " +
                               defenders[i]->getFullName() + "!");
        }
    }

    return ret;
}

// Handoreddosutoraiki
int ServantSaberKatana::activateNP2(vector<Servant *> defenders)
{
    int ret = 0;
    if (actionMPCosts[ascension][4] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][4]);

        log->addToEventLog(getFullName() + " used Handoreddosutoraiki!");

        for (unsigned int i = 0; i < defenders.size(); i++)
        {
            int totalDam = 0;
            for (int z = 0; z < 100; z++)
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
                    double attackMult = 0.1;
                    int critChance = capZero(getCriticalRate() -
                                     defenders[i]->getCriticalEvade());
                    r = getRandNum();
                    if (critChance >= r)
                        attackMult *= 3;

                    // Deal the damage
                    dam = (int) capOne(capOne(getStr() - defenders[i]->getDef()) *
                                 attackMult);
                    totalDam += dam;
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

                // Check to see if the defender is dead. If they are, do not
                //  call the counterattack. Additionally, if they are an Avenger
                //  and they die, activate Final Revenge.
                // If they are not dead but they are a Berserker, check to see
                // if Mad Counter activates.
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
                }
                else
                {
                    z = 100;
                    if (defenders[i]->getClass() == Avenger)
                    {
                        // Activate Final Revenge
                        Debuff *finRev = defenders[i]->finalRevenge();
                        addDebuff(finRev);
                        if (defenders[i]->getAscensionLvl() == 2)
                        {
                            subHP(0.1 * getMaxHP(), OMNI);
                            subMP(0.1 * getMaxMP());

                            if (getCurrHP() == 0)
                            {
                                setHP(1);
                            }
                        }
                    }
                }
            }

            log->addToEventLog(getFullName() + " dealt " +
                               to_string(totalDam) + " total damage to " +
                               defenders[i]->getFullName() + "!");
        }
    }

    return ret;
}

// Sauzandosutaoraiki
int ServantSaberKatana::activateNP3(vector<Servant *> defenders)
{
    int ret = 0;
    if (actionMPCosts[ascension][5] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][5]);

        log->addToEventLog(getFullName() + " used Sauzandosutoraiki!");

        for (unsigned int i = 0; i < defenders.size(); i++)
        {
            int totalDam = 0;
            for (int z = 0; z < 1000; z++)
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

                // If you hit, calculate crit chance
                if (hit)
                {
                    double attackMult = 0.05;
                    int critChance = capZero(getCriticalRate() -
                                     defenders[i]->getCriticalEvade());
                    r = getRandNum();
                    if (critChance >= r)
                        attackMult *= 3;

                    // Skill: GLORIOUS NIPPON STEEL FOLDED OVER 100 TIMES
                    r = getRandNum();
                    if (getSkl() >= r)
                        attackMult *= 2;

                    // Deal the damage
                    dam = (int) capOne(capOne(getStr() - defenders[i]->getDef()) *
                                 attackMult);
                    totalDam += dam;
                    log->addToEventLog(getFullName() + " dealt " +
                                       to_string(dam) + " damage to " +
                                       defenders[i]->getFullName() + ".");
                    defenders[i]->subHP(dam, OMNI);
                }
                else
                {
                    log->addToEventLog(getFullName() + " missed " +
                                       defenders[i]->getFullName() + "!");
                }

                // Check to see if the defender is dead.
                if(defenders[i]->getCurrHP() <= 0)
                    z = 1000;
            }

            log->addToEventLog(getFullName() + " dealt " +
                               to_string(totalDam) + " total damage to " +
                               defenders[i]->getFullName() + "!");
        }
    }

    return ret;
}
