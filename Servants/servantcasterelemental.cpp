#include "servantcasterelemental.h"
#include "PlayField.h"

ServantCasterElemental::ServantCasterElemental(int as, Team t, Logger *l) : ServantCaster(as, t, l)
{
    name = "Elemental Caster";

    /** Stat modifiers **/
    int magMod = 10;

    mag[0] += magMod;
    mag[1] += magMod;
    mag[2] += magMod;

    // Action List
    actionList[0].push_back("5: Item Construction - Flame Blade");
    actionList[0].push_back("6: Territory Creation - Flames");
    actionList[0].push_back("7: Cleansing Flame");
    actionList[0].push_back("8: NP: Elemental Beam");

    actionList[1].push_back("5: Item Construction - Flame Blade");
    actionList[1].push_back("6: Territory Creation - Flames");
    actionList[1].push_back("7: Cleansing Flame");
    actionList[1].push_back("8: NP: Elemental Beam");
    actionList[1].push_back("9: NP: Meteor Slam");

    actionList[2].push_back("6: Item Construction - Flame Blade");
    actionList[2].push_back("7: Territory Creation - Flames");
    actionList[2].push_back("8: Cleansing Flame");
    actionList[2].push_back("9: NP: Elemental Beam");
    actionList[2].push_back("10: NP: Meteor Slam");
    actionList[2].push_back("11: NP: Searing Maelstrom");

    // Action List Types
    actionListTypes[0].push_back(N);
    actionListTypes[0].push_back(T);
    actionListTypes[0].push_back(S);
    actionListTypes[0].push_back(A);

    actionListTypes[1].push_back(N);
    actionListTypes[1].push_back(T);
    actionListTypes[1].push_back(S);
    actionListTypes[1].push_back(A);
    actionListTypes[1].push_back(A);

    actionListTypes[2].push_back(N);
    actionListTypes[2].push_back(T);
    actionListTypes[2].push_back(S);
    actionListTypes[2].push_back(A);
    actionListTypes[2].push_back(A);
    actionListTypes[2].push_back(A);

    // Action MP Costs
    actionMPCosts[0].push_back(75);
    actionMPCosts[0].push_back(maxMP[0] / 5);
    actionMPCosts[0].push_back(50);
    actionMPCosts[0].push_back(100);

    actionMPCosts[1].push_back(75);
    actionMPCosts[1].push_back(maxMP[1] / 5);
    actionMPCosts[1].push_back(50);
    actionMPCosts[1].push_back(100);
    actionMPCosts[1].push_back(400);

    actionMPCosts[2].push_back(75);
    actionMPCosts[2].push_back(maxMP[2] / 5);
    actionMPCosts[2].push_back(50);
    actionMPCosts[2].push_back(100);
    actionMPCosts[2].push_back(400);
    actionMPCosts[2].push_back(800);

    // Action Dodgeable?
    actionDodgeable[0].push_back(false);
    actionDodgeable[0].push_back(false);
    actionDodgeable[0].push_back(false);
    actionDodgeable[0].push_back(false);

    actionDodgeable[1].push_back(false);
    actionDodgeable[1].push_back(false);
    actionDodgeable[1].push_back(false);
    actionDodgeable[1].push_back(false);
    actionDodgeable[1].push_back(false);

    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(false);

    // Action Counterable?
    actionCounterable[0].push_back(false);
    actionCounterable[0].push_back(false);
    actionCounterable[0].push_back(false);
    actionCounterable[0].push_back(false);

    actionCounterable[1].push_back(false);
    actionCounterable[1].push_back(false);
    actionCounterable[1].push_back(false);
    actionCounterable[1].push_back(false);
    actionCounterable[1].push_back(false);

    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);

    vector<string> np1;
    np1.push_back("Elemental Beam");
    np1.push_back("Spend 100 MP to fire a beam of flame in a straight line to the edge of the map. Does normal damage. No accuracy or critical check.");
    vector<string> np2;
    np2.push_back("Meteor Slam");
    np2.push_back("Spend 400 MP to call down 4 meteors to the battlefield, each of radius 3, each targeting a random enemy unit. It is possible for multiple meteors to target the same unit. Enemy and allies alike lose 3 times normal damage when hit. No accuracy or critical check.");
    vector<string> np3;
    np3.push_back("Searing Maelstrom");
    np3.push_back("Spend 800 MP to bathe the entire battlefield in searing flame. Allies gain .5*MAXHP while enemies lose .5*MAXHP and gain a fire debuff for 5 turns.");
    noblePhantasms.push_back(np1);
    noblePhantasms.push_back(np2);
    noblePhantasms.push_back(np3);

    flameBladeCreated = false;
}

void ServantCasterElemental::setPlayField(PlayField *f)
{
    field = f;
    // Elemental Beam Range
    vector<Coordinate> npc1;
    for (int i = 1; i < field->getLargestDimension(); i++)
    {
        Coordinate tc;
        tc.x = 0; tc.y = i;
        npc1.push_back(tc);
    }

    // Both Meteor Slam and Searing Maelstron have the entire field as their
    //  range, though for slightly different reasons
    /*vector<Coordinate> npc2;
    for (int i = 0; i < field->getLargestDimension(); i++)
    {
        for (int j = 0; j < field->getLargestDimension(); j++)
        {
            Coordinate tc;
            tc.x = j; tc.y = i;
            if (!(j == 0 && i == 0))
                npc2.push_back(tc);
        }
    }*/

    npRanges.push_back(npc1);
    npRanges.push_back(getLowToHighRange(1, field->getLargestDimension() * 2));
    npRanges.push_back(getLowToHighRange(0, field->getLargestDimension() * 2));
}

/***** Active Skills *****/
int ServantCasterElemental::cleansingFlame(vector<Servant *> defenders)
{
    if ((ascension == 2 && actionMPCosts[ascension][7] > currMP) ||
        (ascension != 2 && actionMPCosts[ascension][6] > currMP))
        return 1; // Not enough MP

    if (ascension == 2)
        subMP(actionMPCosts[ascension][7]);
    else
        subMP(actionMPCosts[ascension][6]);

    log->addToEventLog(getFullName() + " used Cleansing Flame!");

    for (unsigned int i = 0; i < defenders.size(); i++)
    {
        // Remove the target's debuffs
        defenders[i]->remAllDebuffs2(false, false);
        log->addToEventLog(getFullName() + " cleansed " +
                           defenders[i]->getFullName() + " of status effects!");
    }

    return 0;
}

int ServantCasterElemental::itemConstruction()
{
    if ((ascension == 2 && actionMPCosts[ascension][5] > currMP) ||
        (ascension != 2 && actionMPCosts[ascension][4] > currMP))
        return 1; // Not enough MP

    if (flameBladeCreated)
        return 41;

    if (ascension == 2)
        subMP(actionMPCosts[ascension][5]);
    else
        subMP(actionMPCosts[ascension][4]);

    flameBladeCreated = true;

    log->addToEventLog(getFullName() + " created a Flame Blade!");

    // Modify the action lists
    if (ascension != 2)
    {
        actionList[ascension][4] = "5: Flame Blade";
        actionListTypes[ascension][4] = S;
        actionMPCosts[ascension][4] = 2 * actionMPCosts[ascension][0];
        actionDodgeable[ascension][4] = true;
        actionCounterable[ascension][4] = true;
    }
    else
    {
        actionList[ascension][5] = "6: Flame Blade";
        actionListTypes[ascension][5] = S;
        actionMPCosts[ascension][5] = 2 * actionMPCosts[ascension][0];
        actionDodgeable[ascension][5] = true;
        actionCounterable[ascension][5] = true;
    }

    return 0;
}

int ServantCasterElemental::flameBlade(vector<Servant *> defenders)
{
    if ((ascension == 2 && actionMPCosts[ascension][5] > currMP) ||
        (ascension != 2 && actionMPCosts[ascension][4] > currMP))
        return 1; // Not enough MP

    if (!flameBladeCreated)
        return 41;

    if (ascension == 2)
        subMP(actionMPCosts[ascension][5]);
    else
        subMP(actionMPCosts[ascension][4]);

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

            // Get the flame blade debuff
            vector<Stat> tStats;
            tStats.push_back(HP);
            vector<int> tAmounts;
            tAmounts.push_back(-10);
            Debuff *flameDeb = new Debuff("Burn", "You have been burned!",
                                          defenders[i]->getTeam(), tStats,
                                          tAmounts, 4);

            defenders[i]->addDebuff(flameDeb);

            // Deal the damage
            dam = capZero((int) (getMag() * 1.5) -
                          (defenders[i]->getRes() + defenders[i]->getDef()))
                  * attackMult;
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
                r = getRandNum();
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

    return 0;
}

int ServantCasterElemental::territoryCreation(vector<Servant *> defenders)
{
    if ((ascension == 2 && actionMPCosts[ascension][6] > currMP) ||
        (ascension != 2 && actionMPCosts[ascension][5] > currMP))
        return 1; // Not enough MP

    if (territoryActive)
        return 41;

    if (ascension == 2)
        subMP(actionMPCosts[ascension][6]);
    else
        subMP(actionMPCosts[ascension][5]);

    vector<Coordinate> range = getLowToHighRange(0, 2 + ascension);

    // Create the Debuff
    vector<Stat> tStats;
    tStats.push_back(HP);
    vector<int> tAmounts;
    tAmounts.push_back(-10);
    // Get the Team of the opposing team
    Team otherTeam = All;
    vector<vector<Servant*>> pField = field->getServantLocations();
    for (unsigned int i = 0; i < pField.size(); i++)
    {
        for(unsigned int j = 0; j < pField[i].size(); j++)
        {
            if(pField[i][j] != NULL && pField[i][j]->getTeam() != getTeam())
            {
                otherTeam = pField[i][j]->getTeam();
            }
        }
    }

    Debuff *tFlames = new Debuff("Territory", "Flames",
                                 otherTeam, tStats,
                                 tAmounts, -1);

    // Start the territory
    field->startTerritory(this, tFlames, range);
    territoryActive = true;

    log->addToEventLog(getFullName() + " created a Flames territory!");

    return 0;
}

/***** Re-defined functions *****/
int ServantCasterElemental::getTerritoryMP()
{
    return (getMaxMP() / 20);
}

string ServantCasterElemental::getTerritoryName()
{
    return "Flames";
}

void ServantCasterElemental::addDebuff(Debuff *d)
{
    if(d->getDebuffName().compare("Reality Marble") == 0 ||
            d->getDebuffName().compare("Territory") == 0 ||
            d->getDebuffName().compare("Doom") == 0 ||
            d->getDebuffName().compare("Permadeath") == 0 ||
            d->getDebuffName().compare("Necroally") == 0)
    {
        debuffs.push_back(d);
    }
}

int ServantCasterElemental::isActionNP(int action)
{
    int ret = -1;
    if (ascension != 2)
    {
        if (action >= 7)
            ret = action - 7;
    }
    else
    {
        if (action >= 8)
            ret = action - 8;
    }

    return ret;
}

vector<Coordinate> ServantCasterElemental::getActionRange(int action)
{
    vector<Coordinate> range;
    Coordinate servLoc;
    servLoc.x = 0; servLoc.y = 0;

    if (action == 0) // Regular attack range
    {
        range = getLowToHighRange(getLowRange(), getHighRange());
    }
    else if (action == 1) // Heal
    {
        range = getLowToHighRange(0, field->getFieldLength() + field->getFieldWidth());
    }
    else if (action == 2) // Resurrect
    {
        range.push_back(servLoc);
    }
    else if (action == 3) // Cast From Life
    {
        range.push_back(servLoc);
    }
    else if (ascension == 2) // Final Ascension
    {
        if (action == 4) // Heal All
        {
            range.push_back(servLoc);
        }
        else if (action == 5 && !flameBladeCreated) // Item Construction
        {
            range.push_back(servLoc);
        }
        else if (action == 5 && flameBladeCreated) // Flame Blade
        {
            range = getLowToHighRange(1, 1);
        }
        else if (action == 6) // Territory Creation
        {
            range = getLowToHighRange(0, 2 + ascension);
        }
        else if (action == 7) // Cleansing Flame
        {
            range = getLowToHighRange(1, 1);
        }
        else // Noble Phantasms
        {
            range = getNPRange(action-8);
        }
    }
    else // First or Second Ascension
    {
        if (action == 4 && !flameBladeCreated) // Item Construction
        {
            range.push_back(servLoc);
        }
        else if (action == 4 && flameBladeCreated) // Flame Blade
        {
            range = getLowToHighRange(1, 1);
        }
        else if (action == 5) // Territory Creation
        {
            range = getLowToHighRange(0, 2 + ascension);
        }
        else if (action == 6) // Cleansing Flame
        {
            range = getLowToHighRange(1, 1);
        }
        else // Noble Phantasms
        {
            range = getNPRange(action-7);
        }
    }

    return range;
}

int ServantCasterElemental::doAction(int actionNum, vector<Servant *> defenders)
{
    int ret = 0;
    if (ascension != 2)
    {
        switch (actionNum)
        {
            case 0:
                ret = attack(defenders, true);
                break;
            case 1:
                ret = heal(defenders);
                break;
            case 2:
                ret = resurrect(defenders);
                break;
            case 3:
                ret = castFromLife();
                break;
            case 4:
                if (flameBladeCreated)
                    ret = flameBlade(defenders);
                else
                    ret = itemConstruction();
                break;
            case 5:
                ret = territoryCreation(defenders);
                break;
            case 6:
                ret = cleansingFlame(defenders);
                break;
            case 7:
                ret = activateNP1(defenders);
                break;
            case 8:
                ret = activateNP2(defenders);
                break;
            default:
                return 2; // Not a valid choice
                break;
        }
    }
    else
    {
        switch (actionNum)
        {
            case 0:
                ret = attack(defenders, true);
                break;
            case 1:
                ret = heal(defenders);
                break;
            case 2:
                ret = resurrect(defenders);
                break;
            case 3:
                ret = castFromLife();
                break;
            case 4:
                ret = healAll();
                break;
            case 5:
                if (flameBladeCreated)
                    ret = flameBlade(defenders);
                else
                    ret = itemConstruction();
                break;
            case 6:
                ret = territoryCreation(defenders);
                break;
            case 7:
                ret = cleansingFlame(defenders);
                break;
            case 8:
                ret = activateNP1(defenders);
                break;
            case 9:
                ret = activateNP2(defenders);
                break;
            case 10:
                ret = activateNP3(defenders);
                break;
            default:
                return 2; // Not a valid choice
                break;
        }
    }
    return ret;
}

/***** Noble Phantasms *****/
// Elemental Beam
int ServantCasterElemental::activateNP1(vector<Servant *> defenders)
{
    if ((ascension == 2 && actionMPCosts[ascension][8] > currMP) ||
        (ascension != 2 && actionMPCosts[ascension][7] > currMP))
        return 1; // Not enough MP

    if (ascension == 2)
        subMP(actionMPCosts[ascension][8]);
    else
        subMP(actionMPCosts[ascension][7]);

    log->addToEventLog(getFullName() + " used Elemental Beam!");

    for (unsigned int i = 0; i < defenders.size(); i++)
    {
        int dam = 0;

        // No accuracy or crit check -- it always hits
        int attackMult = 1;

        // Deal the damage
        dam = capZero(getMag() - defenders[i]->getRes()) * attackMult;
        log->addToEventLog(getFullName() + " dealt " +
                           to_string(dam) + " damage to " +
                           defenders[i]->getFullName() + ".");
        defenders[i]->subHP(dam, NP_MAG);

        // Check to see if the defender is dead. If they are, and they are an
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

    return 0;
}

// Meteor Slam
int ServantCasterElemental::activateNP2(vector<Servant *> defenders)
{
    if ((ascension == 2 && actionMPCosts[ascension][9] > currMP) ||
        (ascension != 2 && actionMPCosts[ascension][8] > currMP))
        return 1; // Not enough MP

    if (ascension == 2)
        subMP(actionMPCosts[ascension][9]);
    else
        subMP(actionMPCosts[ascension][8]);

    log->addToEventLog(getFullName() + " used Meteor Slam!");

    // Get all same-team units out of the defenders vector
    for (int i = 0; i < (int) defenders.size(); i++)
    {
        if (defenders[i]->getTeam() == getTeam())
        {
            defenders.erase(defenders.begin() + i);
            i--;
        }
    }

    // Choose three random units from the defenders vector (do so by randomly
    //  removing units until there are only three left)
    while (defenders.size() > 3)
    {
        int r = rand() % ((int) defenders.size());
        defenders.erase(defenders.begin() + r);
    }

    // Get three servant vectors -- one for each randomly chosen servant,
    //  containing all servants within 3 spaces of the target
    vector<Servant*> allDefenders;
    for (unsigned int i = 0; i < defenders.size(); i++)
    {
        vector<Servant*> nearby = field->getAllInRange(defenders[i],
                                                       getLowToHighRange(0, 3));
        for (unsigned int j = 0; j < nearby.size(); j++)
        {
            allDefenders.push_back(nearby[j]);
        }
    }

    // Apply three times normal damage to all sevants in those vectors
    for (unsigned int i = 0; i < allDefenders.size(); i++)
    {
        int dam = 0;

        // No accuracy or crit check -- it always hits
        int attackMult = 3;

        // Deal the damage
        dam = capZero(getMag() - allDefenders[i]->getRes()) * attackMult;
        log->addToEventLog(getFullName() + " dealt " +
                           to_string(dam) + " damage to " +
                           allDefenders[i]->getFullName() + ".");
        allDefenders[i]->subHP(dam, NP_MAG);

        // Check to see if the defender is dead. If they are, and they are an
        // Avenger, activate Final Revenge.
        if(allDefenders[i]->getCurrHP() <= 0)
        {
            if (allDefenders[i]->getClass() == Avenger)
            {
                // Activate Final Revenge
                Debuff *finRev = allDefenders[i]->finalRevenge();
                addDebuff(finRev);
                if (allDefenders[i]->getAscensionLvl() == 2)
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

// Searing Maelstrom
int ServantCasterElemental::activateNP3(vector<Servant *> defenders)
{
    if ((ascension == 2 && actionMPCosts[ascension][10] > currMP) ||
        (ascension != 2 && actionMPCosts[ascension][9] > currMP))
        return 1; // Not enough MP

    if (ascension == 2)
        subMP(actionMPCosts[ascension][10]);
    else
        subMP(actionMPCosts[ascension][9]);

    log->addToEventLog(getFullName() + " enveloped the entire field in a Searing Maelstrom!");

    // Go through defenders vector; same-team units gain 50% of MAXHP while
    //  non-same-team units lose 50% of MAXHP and gain a fire debuff for 5 turns
    for (unsigned int i = 0; i < defenders.size(); i++)
    {
        int dam = defenders[i]->getMaxHP() / 2;

        if (defenders[i]->getTeam() == getTeam())
        {
            //heal 50% maxhp
            defenders[i]->addHP(dam);
            log->addToEventLog(defenders[i]->getFullName() + " healed " +
                               to_string(dam) + " HP.");
        }
        else
        {
            //take 50% maxhp in damage
            defenders[i]->subHP(dam, NP_MAG);
            log->addToEventLog(getFullName() + " dealt " +
                               to_string(dam) + " damage to " +
                               defenders[i]->getFullName() + ".");
        }

        // Check to see if the defender is dead. If they are, and they are an
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

    return 0;
}
