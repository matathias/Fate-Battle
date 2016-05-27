#include "servantsaberclaymore.h"
#include "PlayField.h"

ServantSaberClaymore::ServantSaberClaymore(int as, Team t, Logger *l) : ServantSaber(as, t, l)
{
    name = "Claymore Saber";

    /** Stat modifiers **/
    /*int spdMod = -8;
    int strMod = 5;
    int lukMod = 10;
    int maxmpMod = -25;
    int resMod = -5;

    spd[0] += spdMod;
    spd[1] += spdMod;
    spd[2] += spdMod;
    str[0] += strMod;
    str[1] += strMod;
    str[2] += strMod;
    luk[0] += lukMod;
    luk[1] += lukMod;
    luk[2] += lukMod;
    maxMP[0] += maxmpMod;
    maxMP[1] += maxmpMod;
    maxMP[2] += maxmpMod;
    res[0] += resMod;
    res[1] += resMod;
    res[2] += resMod;*/

    actionList[1].push_back("4: NP: Knights of the Square Table");
    actionList[2].push_back("4: NP: Knights of the Square Table");
    actionList[2].push_back("5: NP: X-Calibre");

    actionListTypes[1].push_back(S);
    actionListTypes[2].push_back(S);
    actionListTypes[2].push_back(A);

    actionMPCosts[1].push_back(65);
    actionMPCosts[2].push_back(65);
    actionMPCosts[2].push_back(90);

    actionDodgeable[1].push_back(true);
    actionDodgeable[2].push_back(true);
    actionDodgeable[2].push_back(false);

    actionCounterable[1].push_back(true);
    actionCounterable[2].push_back(true);
    actionCounterable[2].push_back(false);

    vector<string> np1;
    np1.push_back("Valone");
    np1.push_back("Passive Noble Phantasm. Every time damage is received, have a (RES + LUK) chance of healing 20% of the damage afterward.");
    vector<string> np2;
    np2.push_back("Knights of the Square Table");
    np2.push_back("Instead of attacking once, summon the Knights of the Square Table to attack 15 times for half accuracy and 75% damage. Costs 65 MP. Hey, not everyone can afford a round table.");
    vector<string> np3;
    np3.push_back("X-Calibre");
    np3.push_back("Attack everything within a 3-square cone for 5 times normal damage. Also destroys any territories that exist in the range, but has no effect on Reality Marbles. Costs 90 MP.");
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
    vector<Coordinate> npc2;
    npc2.push_back(c1);
    npc2.push_back(c2);
    npc2.push_back(c3);
    npc2.push_back(c4);

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
    //npRanges.push_back(npc1);
    npRanges.push_back(npc2);
    npRanges.push_back(npc3);

    /** Passive Skill modifiers **/
    vector<Stat> hdS;
    hdS.push_back(MAXMP);
    hdS.push_back(RES);
    hdS.push_back(LUK);
    vector<int> hdA;
    hdA.push_back(-25);
    hdA.push_back(-5);
    hdA.push_back(10);
    Debuff *highDivinity3 = new Debuff("The Legend of King Erthure Is Totally A Thing",
                                       "Passive Skill",
                                      t, hdS, hdA, -1);
    addDebuff(highDivinity3);

    hdS.clear();
    hdS.push_back(STR);
    hdS.push_back(SPD);
    hdA.clear();
    hdA.push_back(5);
    hdA.push_back(-8);
    Debuff *highDivinity2 = new Debuff("It's Not Two-handed For Nothing", "Passive Skill",
                                      t, hdS, hdA, -1);
    addDebuff(highDivinity2);

    hdS.clear();
    hdS.push_back(MOV);
    hdA.clear();
    hdA.push_back(0);
    Debuff *highDivinity = new Debuff("Valone", "Passive Noble Phantasm",
                                      t, hdS, hdA, -1);
    addDebuff(highDivinity);
}

void ServantSaberClaymore::subHP(int hp, DamageType dt)
{
    currHP -= hp;
    int mhp = getMaxHP();
    if (currHP > mhp)
        currHP = mhp;

    if (currHP < 0)
    {
        currHP = 0;
        remAllDebuffs(false);
    }
    else // If Valone activates
    {
        if (getRandNum() < (getRes() + getLuk()) / 4)
        {
            log->addToEventLog("Valone activated! " +
                               getTeamName() + " " + getName() + " heals " +
                               to_string(hp / 10) + " HP!");
            addHP(hp / 10);
        }
    }
}

int ServantSaberClaymore::isActionNP(int action)
{
    int ret = -1;
    switch(action)
    {
        case 3:
            ret = 0;
            break;
        case 4:
            ret = 1;
            break;
        default:
            break;
    }

    return ret;
}

int ServantSaberClaymore::doAction(int actionNum, vector<Servant *> defenders)
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
        default:
            return 2; // Not a valid choice
            break;
    }
    return ret;
}

// Knights of the Square Table
// Instead of attacking once, summon the Knights of the Square Table to attack
//  15 times for half accuracy and 75% damage. Costs 65 MP (.65*MAX). Hey, not
//  everyone can afford a round table.
int ServantSaberClaymore::activateNP1(vector<Servant *> defenders)
{
    int ret = 0;
    if (actionMPCosts[ascension][3] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][3]);
        log->addToEventLog(getTeamName() + " " + getName() +
                           " used Knights of the Square Table!");
        for (unsigned int i = 0; i < defenders.size(); i++)
        {
            for (int z = 0; z < 15; z++)
            {
                int dam = 0;
                // Check if you hit the targets
                vector<int> opEvade = defenders[i]->getEvade();
                bool hit = false;
                // Calculate accuracy
                int accuracy = (capZero(getHitRate() - opEvade[0])) / 2;

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
                    double attackMult = 0.75;
                    int critChance = capZero(getCriticalRate() -
                                     defenders[i]->getCriticalEvade());
                    r = getRandNum();
                    if (critChance >= r)
                        attackMult *= 3;

                    // Deal the damage
                    dam = (int) capOne(capOne(getStr() - defenders[i]->getDef())
                                       * attackMult);

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
                    z = 15;
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
        }
    }

    return ret;
}

int ServantSaberClaymore::activateNP2(vector<Servant *> defenders)
{
    Direction atkDir = NORTH;

    if (actionMPCosts[ascension][4] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][4]);
        log->addToEventLog(getFullName() + " used X-Calibre!");

        for (unsigned int i = 0; i < defenders.size(); i++)
        {
            // Check to see if you get a critical
            int attackMult = 5;

            // Deal the damage
            int dam = capZero(getStr() - defenders[i]->getDef()) * attackMult;
            log->addToEventLog(getFullName() + " dealt " +
                               to_string(dam) + " damage to " +
                               defenders[i]->getFullName() + ".");
            defenders[i]->subHP(dam, NP_STR);
            atkDir = getDirectionAtoB(getCurrLoc(), defenders[i]->getCurrLoc());
        }
    }

    // Erase any territories that X-Calibre hits
    vector<Coordinate> selectionRange = getNPRange(2);
    for (unsigned int i = 0; i < selectionRange.size(); i++)
    {
        int tempX = 0;
        switch(atkDir)
        {
        case NORTH: // All ranges are defined in the NORTH direction, so
                    // no need to do anything
            break;
        case SOUTH: // i.e. rotate 180 degrees
            selectionRange[i].x *= -1;
            selectionRange[i].y *= -1;
            break;
        case EAST: // i.e. rotate 90 degrees clockwise
            tempX = selectionRange[i].x;
            selectionRange[i].x = selectionRange[i].y;
            selectionRange[i].y = tempX * -1;
            break;
        case WEST: // i.e. rotate 90 degrees counterclockwise
            tempX = selectionRange[i].x;
            selectionRange[i].x = selectionRange[i].y * -1;
            selectionRange[i].y = tempX;
            break;
        default:
            // We should never reach here!
            return 33;
        }
    }

    vector<vector<Debuff*>> terrs = field->getTerritories();
    for (unsigned int i = 0; i < selectionRange.size(); i++)
    {
        Debuff *tempTerr = terrs[selectionRange[i].x][selectionRange[i].y];
        if(tempTerr != NULL && tempTerr->getDebuffName().compare("Territory") == 0)
        {
            log->addToEventLog("X-Calibre destroyed the " +
                               tempTerr->getDebuffDescrip() + " Territory!");
            field->eraseTerritory(tempTerr->getDebuffDescrip());
        }
    }

    return 0;
}
