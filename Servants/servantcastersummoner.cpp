#pragma once

#include "servantcastersummoner.h"
#include "PlayField.h"

#include <QMessageBox>
#include <QObject>

ServantCasterSummoner::ServantCasterSummoner(int as, Team t, Logger *l) : ServantCaster(as, t, l)
{
    name = "Summoner Caster";

    golemSummoned = false;
    golemHP = 0;
    golemCost = 100;
    numMinorGates = 0;
    minorGateCost = 50;

    /** Stat modifiers **/
    /*int movMod = 1;
    int spdMod = 5;
    int sklMod = 5;

    mov[0] += movMod;
    mov[1] += movMod;
    mov[2] += movMod;
    spd[0] += spdMod;
    spd[1] += spdMod;
    spd[2] += spdMod;
    skl[0] += sklMod;
    skl[1] += sklMod;
    skl[2] += sklMod;*/

    // Action List
    actionList[0].push_back("5: Item Construction - Minor Gate");
    actionList[0].push_back("6: Territory Creation - Dimensionl Gate");
    actionList[0].push_back("7: Summon Golem");
    actionList[0].push_back("8: NP: Solar Rift");

    actionList[1].push_back("5: Item Construction - Minor Gate");
    actionList[1].push_back("6: Territory Creation - Dimensionl Gate");
    actionList[1].push_back("7: Summon Golem");
    actionList[1].push_back("8: NP: Solar Rift");
    actionList[1].push_back("9: NP: Black Hole Rift");

    actionList[2].push_back("6: Item Construction - Minor Gate");
    actionList[2].push_back("7: Territory Creation - Dimensional Gate");
    actionList[2].push_back("8: Summon Golem");
    actionList[2].push_back("9: NP: Solar Rift");
    actionList[2].push_back("10: NP: Black Hole Rift");
    actionList[2].push_back("11: NP: Universal Rift");

    // Action List Types
    actionListTypes[0].push_back(N);
    actionListTypes[0].push_back(T);
    actionListTypes[0].push_back(N);
    actionListTypes[0].push_back(S);

    actionListTypes[1].push_back(N);
    actionListTypes[1].push_back(T);
    actionListTypes[1].push_back(N);
    actionListTypes[1].push_back(S);
    actionListTypes[1].push_back(S);

    actionListTypes[2].push_back(N);
    actionListTypes[2].push_back(T);
    actionListTypes[2].push_back(N);
    actionListTypes[2].push_back(S);
    actionListTypes[2].push_back(S);
    actionListTypes[2].push_back(A);

    // Action MP Costs
    actionMPCosts[0].push_back(minorGateCost);
    actionMPCosts[0].push_back(maxMP[0] / 5);
    actionMPCosts[0].push_back(golemCost);
    actionMPCosts[0].push_back(250);

    actionMPCosts[1].push_back(minorGateCost);
    actionMPCosts[1].push_back(maxMP[1] / 5);
    actionMPCosts[1].push_back(golemCost);
    actionMPCosts[1].push_back(250);
    actionMPCosts[1].push_back(600);

    actionMPCosts[2].push_back(minorGateCost);
    actionMPCosts[2].push_back(maxMP[2] / 5);
    actionMPCosts[2].push_back(golemCost);
    actionMPCosts[2].push_back(250);
    actionMPCosts[2].push_back(600);
    actionMPCosts[2].push_back(850);

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
    np1.push_back("Solar Rift");
    np1.push_back("Spend 250 MP to open a portal to the core of the sun on top of an opponent within five spaces. Said opponent and all units within two spaces of them receive .2*MAXHP in damage as well as a “Radioactive” debuff that decreases max MP by 10% for 3 turns. Lots of radiation in the core of the sun, after all.");
    vector<string> np2;
    np2.push_back("Black Hole Rift");
    np2.push_back("Spend 600 MP to create a Black Hole with a 3-space radius on top of an enemy player within six spaces. All players under the Black Hole receive 3 times normal damage and are teleported to the coordinate that is the inverse of their current location. If that coordinate is already taken then both the player that was standing there and the player that is moving there are obliterated. All victims receive a “Hawking Radiation” debuff that permanently decreases max HP by 10% of the current max.");
    vector<string> np3;
    np3.push_back("Universal Rift");
    np3.push_back("As a Summoner, you possess an intricate understanding of the Universe itself. You can now spend 850 MP to share this understanding with every player on the playing field -- but the overwhelming knowledge brings incredible mental and physical stress, bringing everyone (but you) down to .1*MAXHP. You can spend a further 130 MP and 30 HP to shield your allies from this effect. All victims receive an “Ultimate Exhaustion” debuff (MOV and SPD decreased by 75% for 3 turns) and are teleported to random locations on the field.");
    noblePhantasms.push_back(np1);
    noblePhantasms.push_back(np2);
    noblePhantasms.push_back(np3);

    // Solar Rift Range
    npRanges.push_back(getLowToHighRange(1, 5));

    // Black Hole Rift Range
    npRanges.push_back(getLowToHighRange(1, 6));

    /** Passive Skill modifiers **/
    vector<Stat> hdS;
    hdS.push_back(MOV);
    hdS.push_back(SPD);
    hdS.push_back(SKL);
    vector<int> hdA;
    hdA.push_back(1);
    hdA.push_back(5);
    hdA.push_back(5);
    Debuff *highDivinity = new Debuff("Summoner's Contract", "Passive Skill",
                                      t, hdS, hdA, -1);
    addDebuff(highDivinity);
}

void ServantCasterSummoner::setPlayField(PlayField *f)
{
    field = f;

    // Univseral Rift has the entire field as its range
    vector<Coordinate> npc2;
    for (int i = 0; i < field->getLargestDimension(); i++)
    {
        for (int j = 0; j < field->getLargestDimension(); j++)
        {
            Coordinate tc;
            tc.x = j; tc.y = i;
            npc2.push_back(tc);
        }
    }
    npRanges.push_back(getLowToHighRange(1, field->getLargestDimension() * 2));
}

/***** Active Skills *****/
int ServantCasterSummoner::summonGolem()
{
    if ((ascension == 2 && actionMPCosts[ascension][7] > currMP) ||
        (ascension != 2 && actionMPCosts[ascension][6] > currMP))
        return 1; // Not enough MP

    if (golemSummoned)
    {
        // Golem already summoned
        log->addToEventLog(getFullName() + " already has an active golem.");
        return 41;
    }

    golemCost *= 3;

    if (ascension == 2)
    {
        subMP(actionMPCosts[ascension][7]);
        actionMPCosts[ascension][7] = golemCost;
    }
    else
    {
        subMP(actionMPCosts[ascension][6]);
        actionMPCosts[ascension][6] = golemCost;
    }

    golemHP = 50;
    golemSummoned = true;

    log->addToEventLog(getFullName() + " summoned a golem with 50 HP.");

    return 0;
}

int ServantCasterSummoner::itemConstruction()
{
    if ((ascension == 2 && actionMPCosts[ascension][5] > currMP) ||
        (ascension != 2 && actionMPCosts[ascension][4] > currMP))
        return 1; // Not enough MP

    minorGateCost *= 4;

    if (ascension == 2)
    {
        subMP(actionMPCosts[ascension][5]);
        actionMPCosts[ascension][5] = minorGateCost;
    }
    else
    {
        subMP(actionMPCosts[ascension][4]);
        actionMPCosts[ascension][4] = minorGateCost;
    }

    numMinorGates++;

    log->addToEventLog(getFullName() + " created a Minor Gate! They now have " +
                       to_string(numMinorGates) + " Minor Gates.");

    return 0;
}

int ServantCasterSummoner::territoryCreation(vector<Servant *> defenders)
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
    tAmounts.push_back(0);
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

    Debuff *tGate = new Debuff("Territory", "Dimensional Gate",
                                 otherTeam, tStats,
                                 tAmounts, -1);

    // Start the territory
    field->startTerritory(this, tGate, range);

    territoryActive = true;

    log->addToEventLog(getFullName() + " created a Dimensional Gate territory!");

    return 0;
}

/***** Function Re-definitions *****/
int ServantCasterSummoner::getTerritoryMP()
{
    return (getMaxHP() / 20);
}

string ServantCasterSummoner::getTerritoryName()
{
    return "Dimensional Gate";
}

void ServantCasterSummoner::subHP(int hp, DamageType dt)
{
    bool noDamage = false;

    if(golemSummoned && golemHP > 0 && dt != NP_STR && dt != NP_MAG &&
            dt != OMNI)
    {
        golemHP -= hp;
        noDamage = true;
        log->addToEventLog(getFullName() + "'s Golem absorbed damage for them.");
        if (golemHP <= 0)
        {
            golemHP = 0;
            golemSummoned = false;
            log->addToEventLog(getFullName() + "'s Golem died!");
        }
    }
    else if(numMinorGates > 0 && dt != NP_STR && dt != NP_MAG && dt != OMNI)
    {
        // See if a Minor Gate activates
        int r = getRandNum();
        if (r <= getSkl())
        {
            // It activated! Take one off the count and teleport the Caster.
            numMinorGates--;
            // Get the enemy team
            vector<Servant*> enemies;
            vector<vector<Servant*>> pField;
            for (unsigned int i = 0; i < pField.size(); i++)
            {
                for (unsigned int j = 0; j < pField[i].size(); j++)
                {
                    if(pField[i][j] != NULL &&
                            pField[i][j]->getTeam() != getTeam())
                        enemies.push_back(pField[i][j]);
                }
            }

            // teleport the Caster
            int ox = getCurrLoc().x;
            int oy = getCurrLoc().y;
            Coordinate newLoc = field->getFarthestValidFrom(enemies);
            field->moveServant(this, newLoc);

            log->addToEventLog(getFullName() + "'s Minor Gate activated, teleporting them to (" +
                               to_string(newLoc.x) + "," + to_string(newLoc.y)
                               + ") from (" + to_string(ox) + "," +
                               to_string(oy) + ").");

            noDamage = true;
        }
    }

    // If there was no golem and no minor gate activated, the caster takes
    // damage.
    if (!noDamage)
    {
        currHP -= hp;
        int mhp = getMaxHP();
        if (currHP > mhp)
            currHP = mhp;
        else if (currHP <= 0)
        {
            currHP = 0;
            remAllDebuffs(false);
        }
    }
}

int ServantCasterSummoner::attack(vector<Servant *> defenders, bool counter)
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

                // Deal the damage
                dam = capZero(getMag() - defenders[i]->getRes()) * attackMult;
                log->addToEventLog(getFullName() + " dealt " +
                                   to_string(dam) + " damage to " +
                                   defenders[i]->getFullName() + ".");
                defenders[i]->subHP(dam, D_MAG);

                // If the Caster has a golem, see if they attack
                r = getRandNum();
                if (golemSummoned && r <= (getMag() + getLuk()))
                {
                    dam = capZero(getMag() - defenders[i]->getRes()) * attackMult;
                    log->addToEventLog(getFullName() + "'s Golem also dealt " +
                                       to_string(dam) + " damage to " +
                                       defenders[i]->getFullName() + "!");
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
                        subHP(dam, C_MAG);
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

int ServantCasterSummoner::isActionNP(int action)
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

vector<Coordinate> ServantCasterSummoner::getActionRange(int action)
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
        else if (action == 5) // Item Construction
        {
            range.push_back(servLoc);
        }
        else if (action == 6) // Territory Creation
        {
            range = getLowToHighRange(0, 2 + ascension);
        }
        else if (action == 7) // Summon Golem
        {
            range.push_back(servLoc);
        }
        else // Noble Phantasms
        {
            range = getNPRange(action-8);
        }
    }
    else // First or Second Ascension
    {
        if (action == 4) // Item Construction
        {
            range.push_back(servLoc);
        }
        else if (action == 5) // Territory Creation
        {
            range = getLowToHighRange(0, 2 + ascension);
        }
        else if (action == 6) // Summon Golem
        {
            range.push_back(servLoc);
        }
        else // Noble Phantasms
        {
            range = getNPRange(action-7);
        }
    }

    return range;
}

int ServantCasterSummoner::doAction(int actionNum, vector<Servant *> defenders)
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
                ret = itemConstruction();
                break;
            case 5:
                ret = territoryCreation(defenders);
                break;
            case 6:
                ret = summonGolem();
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
                ret = itemConstruction();
                break;
            case 6:
                ret = territoryCreation(defenders);
                break;
            case 7:
                ret = summonGolem();
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
                return 2; // Not a valid choice
                break;
        }
    }
    return ret;
}

/***** Noble Phantasms *****/
// Solar Rift
int ServantCasterSummoner::activateNP1(vector<Servant *> defenders)
{
    if ((ascension == 2 && actionMPCosts[ascension][8] > currMP) ||
        (ascension != 2 && actionMPCosts[ascension][7] > currMP))
        return 1; // Not enough MP

    if (ascension == 2)
        subMP(actionMPCosts[ascension][8]);
    else
        subMP(actionMPCosts[ascension][7]);

    log->addToEventLog(getFullName() + " cast Solar Rift!");

    for (unsigned int i = 0; i < defenders.size(); i++)
    {
        vector<Servant*> nearby = field->getAllInRange(defenders[i],
                                                       getLowToHighRange(0, 2));
        for (unsigned int j = 0; j < nearby.size(); j++)
        {
            // Place the Debuff
            vector<Stat> tStats;
            tStats.push_back(MAXMP);
            vector<int> tAmounts;
            tAmounts.push_back(-1 * (nearby[j]->getMaxMP() / 10));
            Debuff *radioactive = new Debuff("Radioactive",
                                             "Radiation sickness decreases your MP capacity.",
                                             nearby[j]->getTeam(), tStats,
                                             tAmounts, 3);
            nearby[j]->addDebuff(radioactive);

            // Deal the damage
            int dam = nearby[j]->getMaxHP() / 5;
            log->addToEventLog(getFullName() + " dealt " +
                               to_string(dam) + " damage to " +
                               nearby[j]->getFullName() + ".");
            nearby[j]->subHP(dam, NP_MAG);

            // Check to see if the defender is dead. If they are, and they are
            //  an Avenger, activate Final Revenge.
            if(nearby[j]->getCurrHP() <= 0)
            {
                if (nearby[j]->getClass() == Avenger)
                {
                    // Activate Final Revenge
                    Debuff *finRev = nearby[j]->finalRevenge();
                    addDebuff(finRev);
                    if (nearby[j]->getAscensionLvl() == 2)
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

// Black Hole Rift
int ServantCasterSummoner::activateNP2(vector<Servant *> defenders)
{
    if ((ascension == 2 && actionMPCosts[ascension][9] > currMP) ||
        (ascension != 2 && actionMPCosts[ascension][8] > currMP))
        return 1; // Not enough MP

    if (ascension == 2)
        subMP(actionMPCosts[ascension][9]);
    else
        subMP(actionMPCosts[ascension][8]);

    log->addToEventLog(getFullName() + " cast Black Hole Rift!");

    for (unsigned int i = 0; i < defenders.size(); i++)
    {
        vector<Servant*> nearby = field->getAllInRange(defenders[i],
                                                       getLowToHighRange(0, 3));
        for (unsigned int j = 0; j < nearby.size(); j++)
        {
            // Place the Debuff
            vector<Stat> tStats;
            tStats.push_back(MAXHP);
            vector<int> tAmounts;
            tAmounts.push_back(-1 * (nearby[j]->getMaxHP() / 10));
            Debuff *radioactive = new Debuff("Hawking Radiation",
                                             "Quantum Mechanics hates you.",
                                             nearby[j]->getTeam(), tStats,
                                             tAmounts, -1);
            nearby[j]->addDebuff(radioactive);

            int attackMult = 3;

            // Deal the damage
            int dam = capZero(getMag() - nearby[j]->getRes()) * attackMult;
            log->addToEventLog(getFullName() + " dealt " +
                               to_string(dam) + " damage to " +
                               nearby[j]->getFullName() + ".");
            nearby[j]->subHP(dam, OMNI);

            // Get the inverse location
            Coordinate inverse;
            inverse.x = nearby[j]->getCurrLoc().y;
            inverse.y = nearby[j]->getCurrLoc().x;

            // See if there is a Servant on the location
            Servant *target = field->getServantOnSpace(inverse);
            // If there is, obliterate both that servant and this servant
            if (target != NULL)
            {
                nearby[j]->setHP(0);
                target->setHP(0);

                log->addToEventLog(nearby[j]->getFullName() +
                                   " was teleported to the same space as " +
                                   target->getFullName() +
                                   ", causing them to obliterate each other!");

                // add Permadeath to the targets
                vector<Stat> pStats;
                pStats.push_back(MOV);
                vector<int> pAmounts;
                pAmounts.push_back(0);
                Debuff *perma = new Debuff("Permadeath",
                                           "You have been permanetly killed.",
                                           defenders[i]->getTeam(), pStats, pAmounts,
                                           -1);
                nearby[j]->addDebuff(perma);
                target->addDebuff(perma);
            }
            // Otherwise just move the servant to the inverse location
            else
            {
                int ox = nearby[j]->getCurrLoc().x;
                int oy = nearby[j]->getCurrLoc().y;
                field->moveServant(nearby[j], inverse);

                log->addToEventLog(nearby[j]->getFullName() + " was teleported to (" +
                                   to_string(inverse.x) + "," + to_string(inverse.y)
                                   + ") from (" + to_string(ox) + "," +
                                   to_string(oy) + ").");
            }
        }
    }

    return 0;
}

// Universal Rift
int ServantCasterSummoner::activateNP3(vector<Servant *> defenders)
{
    if ((ascension == 2 && actionMPCosts[ascension][10] > currMP) ||
        (ascension != 2 && actionMPCosts[ascension][9] > currMP))
        return 1; // Not enough MP

    if (ascension == 2)
        subMP(actionMPCosts[ascension][10]);
    else
        subMP(actionMPCosts[ascension][9]);

    log->addToEventLog(getFullName() + " cast Universal Rift!");

    // See if the player has enough MP and HP to protect their teammates.
    // If they do, ask the player if they wish to spend the MP and HP to do so.
    bool saveTeammates = false;
    if (getCurrMP() >= 130 && getCurrHP() > 30)
    {
        string check = "\nDo you wish to protect your teammates at a cost of\n130 MP and 30 HP?\n";
        QMessageBox checkMessage;
        checkMessage.setWindowTitle(QObject::tr("Final Fate"));
        checkMessage.setText(QString::fromStdString(check));
        checkMessage.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        checkMessage.setDefaultButton(QMessageBox::No);
        if (checkMessage.exec() == QMessageBox::Yes)
        {
            saveTeammates = true;
            subMP(130);
            subHP(30, OMNI);
            log->addToEventLog(getFullName() + " sacrifices 130 MP and 30 HP to protect their teammates from the Universal Rift!");
        }
    }

    // If the caster saved their teammates, remove them from the defender
    //  vector.
    for (int i = 0; i < (int) defenders.size(); i++)
    {
        if((defenders[i]->getTeam() == getTeam() && saveTeammates) ||
                defenders[i] == this)
        {
            defenders.erase(defenders.begin() + i);
            i--;
        }
    }

    for (unsigned int i = 0; i < defenders.size(); i++)
    {
        // Bring everyone down to .1 * MAXHP (unless they are already below that
        //  threshold)
        if(defenders[i]->getCurrHP() > defenders[i]->getMaxHP() * 0.1)
        {
            defenders[i]->setHP(defenders[i]->getMaxHP() * 0.1);
        }

        // Apply the "Ultimate Exhaustion" debuff
        vector<Stat> pStats;
        pStats.push_back(MOV);
        pStats.push_back(SPD);
        vector<int> pAmounts;
        pAmounts.push_back(-1 * (defenders[i]->getMov() * 0.75));
        pAmounts.push_back(-1 * (defenders[i]->getSpd() * 0.75));
        Debuff *ultEx = new Debuff("Ultimate Exhaustion",
                                   "Being exposed to the Universe's secrets sapped you of your energy.",
                                   defenders[i]->getTeam(), pStats, pAmounts,
                                   4);

        defenders[i]->addDebuff(ultEx);

        // Teleport to a random location on the field
        Coordinate newLoc = field->getRandomCoord();
        int ox = defenders[i]->getCurrLoc().x;
        int oy = defenders[i]->getCurrLoc().y;

        field->moveServant(defenders[i], newLoc);

        log->addToEventLog(defenders[i]->getFullName() + " was teleported to (" +
                           to_string(newLoc.x) + "," + to_string(newLoc.y)
                           + ") from (" + to_string(ox) + "," +
                           to_string(oy) + ").");
    }

    return 0;
}
