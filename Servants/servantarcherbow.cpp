#include "servantarcherbow.h"
#include "PlayField.h"

ServantArcherBow::ServantArcherBow(int as, Team t, Logger *l) : ServantArcher(as, t, l)
{
    name = "Bow Archer";

    /** Stat modifiers **/
    /*int strMod = 5;
    int magMod = 25;

    str[0] += strMod;
    str[1] += strMod;
    str[2] += strMod;
    mag[0] += magMod;
    mag[1] += magMod;
    mag[2] += magMod;*/

    actionList[0].push_back("2: Magic Bow");
    actionList[0].push_back("3: NP: Ballista");
    actionList[1].push_back("2: Magic Bow");
    actionList[1].push_back("3: NP: Ballista");
    actionList[2].push_back("2: Magic Bow");
    actionList[2].push_back("3: NP: Ballista");
    actionList[2].push_back("4: NP: Rain of Arrows");

    actionListTypes[0].push_back(S);
    actionListTypes[0].push_back(S);
    actionListTypes[1].push_back(S);
    actionListTypes[1].push_back(S);
    actionListTypes[2].push_back(S);
    actionListTypes[2].push_back(S);
    actionListTypes[2].push_back(S);

    actionMPCosts[0].push_back(3);
    actionMPCosts[0].push_back(15);
    actionMPCosts[1][0] += 3;       // Costs 3 more due to Certain Blow
    actionMPCosts[1].push_back(6);  // Costs 3 more due to Certain Blow
    actionMPCosts[1].push_back(15);
    actionMPCosts[2][0] += 3;       // Costs 3 more due to Certain Blow
    actionMPCosts[2].push_back(6);  // Costs 3 more due to Certain Blow
    actionMPCosts[2].push_back(15);
    actionMPCosts[2].push_back(80);

    actionDodgeable[0].push_back(true);
    actionDodgeable[0].push_back(true);
    actionDodgeable[1][0] = false;       // Certain Blow
    actionDodgeable[1].push_back(false);
    actionDodgeable[1].push_back(true);
    actionDodgeable[2][0] = false;       // Certain Blow
    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(true);
    actionDodgeable[2].push_back(false);

    actionCounterable[0].push_back(true);
    actionCounterable[0].push_back(false);
    actionCounterable[1].push_back(true);
    actionCounterable[1].push_back(false);
    actionCounterable[2].push_back(true);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);

    vector<string> np1;
    np1.push_back("Ballista");
    np1.push_back("Costs 15 MP. Fire a powerful shot at a square within your range and deal normal damage to every servant on or adjacent to the target square. Accuracy and critical checks apply. Requires a target unit to activate.");
    vector<string> np2;
    np2.push_back("Certain Blow");
    np2.push_back("Passive Noble Phantasm. Normal attack and Magic Bow (but not Ballista) cost 3 more MP to use, but always hit. (Skills that offer another evasion check such as Eye of the Mind can still activate however.)");
    vector<string> np3;
    np3.push_back("Rain of Arrows");
    np3.push_back("Costs 80 MP. Temporarily boosting your abilities with mana you fire a hail of arrows at a square within 7 spaces of you. All enemy units within two spaces of the target square take 3 times normal damage. Always hits and there is a critical check. Requires a target unit to activate.");
    noblePhantasms.push_back(np1);
    noblePhantasms.push_back(np2);
    noblePhantasms.push_back(np3);

    // Range for Ballista
    npRanges.push_back(getLowToHighRange(getLowRange(), getHighRange()));
    // Range for Rain of Arrows
    npRanges.push_back(getLowToHighRange(getLowRange(), 7));

    /** Passive Skill modifiers **/
    vector<Stat> hdS;
    hdS.push_back(STR);
    hdS.push_back(MAG);
    vector<int> hdA;
    hdA.push_back(5);
    hdA.push_back(25);
    Debuff *highDivinity = new Debuff("Fictional Bows Are Stronger", "Passive Skill",
                                      t, hdS, hdA, -1);
    addDebuff(highDivinity);

    if (ascension >= 1)
    {
        hdS.clear();
        hdS.push_back(MOV);
        hdA.clear();
        hdA.push_back(0);
        Debuff *highDivinity2 = new Debuff("Certain Blow", "Passive Noble Phantasm",
                                          t, hdS, hdA, -1);
        addDebuff(highDivinity2);
    }
}

int ServantArcherBow::doAction(int actionNum, vector<Servant *> defenders)
{
    int ret = 0;
    switch (actionNum)
    {
        case 0:
            ret = attack(defenders, true);
            break;
        case 1:
            ret = magicBow(defenders);
            break;
        case 2:
            ret = activateNP1(defenders);
            break;
        case 3:
            ret = activateNP2(defenders);
            break;
        default:
            return 2; // Not a valid choice
            break;
    }
    return ret;
}

int ServantArcherBow::isActionNP(int action)
{
    int ret = -1;
    switch(action)
    {
        case 2:
            ret = 0;
            break;
        case 3:
            ret = 1;
            break;
        default:
            break;
    }

    return ret;
}

vector<Coordinate> ServantArcherBow::getActionRange(int action)
{
    // Figure out what action this is and return the appropriate range
    vector<Coordinate> range;

    if (action == 0 || action == 1) // Regular attack and Magic Bow range
    {
        range = getLowToHighRange(getLowRange(), getHighRange());
    }
    else
    {
        range = getNPRange(action-2);
    }

    return range;
}

int ServantArcherBow::attack(vector<Servant *> defenders, bool counter)
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
            // If ascension level is 1 or higher, Certain Blow means we always
            //  hit
            if (ascension == 0)
            {
                int accuracy = capZero(getHitRate() - opEvade[0]);

                int r = getRandNum();
                if (accuracy >= r)
                    hit = true;
            }
            else
            {
                hit = true;
            }

            // Certain Blow does not block extra evasion skills though
            if (opEvade.size() > 1 && hit)
            {
                for (unsigned int j = 1; j < opEvade.size() && hit; j++)
                {
                    int r = getRandNum();
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
                int r = getRandNum();
                if (critChance >= r)
                    attackMult *= 3;

                // Archers deal thrice damage to Pegasus Riders
                if (defenders[i]->getName().compare("Pegasus Rider") == 0)
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

int ServantArcherBow::magicBow(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][1] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][1]);
        log->addToEventLog(getFullName() + " used Magic Bow!");
        for (unsigned int i = 0; i < defenders.size(); i++)
        {
            int dam = 0;
            // Check if you hit the targets
            vector<int> opEvade = defenders[i]->getEvade();
            bool hit = false;
            // Calculate accuracy
            // If ascension level is 1 or higher, Certain Blow means we always
            //  hit
            if (ascension == 0)
            {
                int accuracy = capZero(getHitRate() - opEvade[0]);

                int r = getRandNum();
                if (accuracy >= r)
                    hit = true;
            }
            else
            {
                hit = true;
            }

            // Certain Blow does not block extra evasion skills though
            if (opEvade.size() > 1 && hit)
            {
                for (unsigned int j = 1; j < opEvade.size() && hit; j++)
                {
                    int r = getRandNum();
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
                int r = getRandNum();
                if (critChance >= r)
                    attackMult *= 3;

                // Archers deal thrice damage to Pegasus Riders
                if (defenders[i]->getName().compare("Pegasus Rider") == 0)
                    attackMult *= 3;

                // Deal the damage
                // Since this is Magic Bow, the damage is magical, not physical
                dam = capZero(getMag() - defenders[i]->getRes()) * attackMult;
                log->addToEventLog(getFullName() + " dealt " +
                                   to_string(dam) + " damage to " +
                                   defenders[i]->getFullName() + ".");
                defenders[i]->subHP(dam, D_MAG);
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
                    int r = getRandNum();
                    if (defenders[i]->getLuk() >= r)
                    {
                        // Mad Counter activated! The attacking servant takes
                        // damage equal to the damage they dealt.
                        log->addToEventLog(defenders[i]->getFullName() +
                                           "' Mad Counter activated, dealing " +
                                           to_string(dam) + " damage back to " +
                                           getFullName() + ".");
                        subHP(dam, C_MAG);
                    }
                }
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

// Noble Phantasm: Ballista
int ServantArcherBow::activateNP1(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][2] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][2]);
        log->addToEventLog(getFullName() + " used Ballista!");

        // Get Servants on the adjacent squares and add them to defenders
        vector<Servant*> adjServs =
                    field->getAllInRange(defenders[0], getLowToHighRange(1,1));

        for (unsigned int i = 0; i < adjServs.size(); i++)
        {
            defenders.push_back(adjServs[i]);
        }

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

                // Archers deal thrice damage to Pegasus Riders
                if (defenders[i]->getName().compare("Pegasus Rider") == 0)
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
            // Avenger, activate Final Revenge.
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

// Noble Phantasm: Rain of Arrows
int ServantArcherBow::activateNP2(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][3] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][3]);
        log->addToEventLog(getFullName() + " used Rain of Arrows!");

        // Get Servants on the adjacent squares and add them to defenders
        vector<Servant*> adjServs =
                    field->getAllInRange(defenders[0], getLowToHighRange(1,2));

        for (unsigned int i = 0; i < adjServs.size(); i++)
        {
            defenders.push_back(adjServs[i]);
        }

        for (unsigned int i = 0; i < defenders.size(); i++)
        {
            int dam = 0;
            // This always hits, so don't bother with accuracy.
            int attackMult = 3;
            int critChance = capZero(getCriticalRate() -
                             defenders[i]->getCriticalEvade());
            int r = getRandNum();
            if (critChance >= r)
                attackMult *= 3;

            // Archers deal thrice damage to Pegasus Riders
            if (defenders[i]->getName().compare("Pegasus Rider") == 0)
                attackMult *= 3;

            // Deal the damage
            // Rain of Arrows is a magic attack
            dam = capZero(getMag() - defenders[i]->getRes()) * attackMult;
            log->addToEventLog(getFullName() + " dealt " +
                               to_string(dam) + " damage to " +
                               defenders[i]->getFullName() + ".");
            defenders[i]->subHP(dam, NP_MAG);

            // Check to see if the defender is dead. If they are and they are an
            // Avenger, activate Final Revenge.
            if(defenders[i]->getCurrHP() > 0)
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
