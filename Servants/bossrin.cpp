#include "bossrin.h"
#include "PlayField.h"

BossRin::BossRin(int as, Team t, Logger *l) : ServantCaster(as, t, l)
{
    name = "Rin Tohsaka";

    /** Stat modifiers **/
    int magMod = 15;
    int resMod = 15;
    int defMod = 5;
    int sklMod = 5;
    int spdMod = 5;
    int maxHPMod = 50;

    mag[0] += magMod;
    mag[1] += magMod;
    mag[2] += magMod;
    res[0] += resMod;
    res[1] += resMod;
    res[2] += resMod;
    def[0] += defMod;
    def[1] += defMod;
    def[2] += defMod;
    skl[0] += sklMod;
    skl[1] += sklMod;
    skl[2] += sklMod;
    spd[0] += spdMod;
    spd[1] += spdMod;
    spd[2] += spdMod;
    maxHP[0] += maxHPMod;
    maxHP[1] += maxHPMod;
    maxHP[2] += maxHPMod;

    currHP = maxHP[ascension];

    // Action List
    actionList[0].push_back("5: Leg Reinforcement");
    actionList[0].push_back("6: Eins, Zwei, Drei");

    actionList[1].push_back("5: Leg Reinforcement");
    actionList[1].push_back("6: Eins, Zwei, Drei");
    actionList[1].push_back("7: Gandr Shot");

    actionList[2].push_back("6: Leg Reinforcement");
    actionList[2].push_back("7: Eins, Zwei, Drei");
    actionList[2].push_back("8: Gandr Shot");
    actionList[2].push_back("9: Azoth Sword");

    // Action List Types
    actionListTypes[0].push_back(N);
    actionListTypes[0].push_back(A);

    actionListTypes[1].push_back(N);
    actionListTypes[1].push_back(A);
    actionListTypes[1].push_back(S);

    actionListTypes[2].push_back(N);
    actionListTypes[2].push_back(A);
    actionListTypes[2].push_back(S);
    actionListTypes[2].push_back(S);

    // Action MP Costs
    actionMPCosts[0].push_back(50);
    actionMPCosts[0].push_back(50);

    actionMPCosts[1].push_back(50);
    actionMPCosts[1].push_back(50);
    actionMPCosts[1].push_back(150);

    actionMPCosts[2].push_back(50);
    actionMPCosts[2].push_back(50);
    actionMPCosts[2].push_back(150);
    actionMPCosts[2].push_back(300);

    // Action Dodgeable?
    actionDodgeable[0].push_back(false);
    actionDodgeable[0].push_back(true);

    actionDodgeable[1].push_back(false);
    actionDodgeable[1].push_back(true);
    actionDodgeable[1].push_back(false);

    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(true);
    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(true);

    // Action Counterable?
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
    np1.push_back("Eins, Zwei, Drei");
    np1.push_back("Rin throws gems at all players in a two-radius semicircle in front of her while simultaneously jumping backward two spaces. Targets have a half chance of dodging. Costs 50 MP.");
    vector<string> np2;
    np2.push_back("Gandr Shot");
    np2.push_back("Choose an enemy player to target with Gandr. All players within two spaces of the chosen target take thrice normal damage, while all players on the path between Rin and the target spot take regular damage. The detonation always hits, but the other attacks have accuracy checks. Costs 150 MP.");
    vector<string> np3;
    np3.push_back("Azoth Sword");
    np3.push_back("Casts Doom (3-turns) on the victim and deals unresisted magic damage. Target has a half chance at dodging and evasion skills cannot activate. Costs 300 MP, has a cooldown of three turns, and can only be used twice per battle.");
    noblePhantasms.push_back(np1);
    noblePhantasms.push_back(np2);
    noblePhantasms.push_back(np3);

    azothUseCount = 0;
}

/***** Active Skills *****/
int BossRin::legReinforcement()
{
    if ((ascension == 2 && actionMPCosts[ascension][5] > currMP) ||
        (ascension != 2 && actionMPCosts[ascension][4] > currMP))
        return 1; // Not enough MP

    if (legReinforceCooldown())
    {
        log->addToEventLog("You already used Leg Reinforcement.");
        return 41;
    }

    if (ascension == 2)
        subMP(actionMPCosts[ascension][5]);
    else
        subMP(actionMPCosts[ascension][4]);

    // Create the buff
    vector<Stat> tStats;
    tStats.push_back(SPD);
    tStats.push_back(MOV);
    vector<int> tAmounts;
    tAmounts.push_back(15);
    tAmounts.push_back(2);
    Debuff* du = new Debuff("Leg Reinforcement", "--", getTeam(), tStats,
                            tAmounts, 3);

    // Create the cooldown "buff"
    vector<Stat> tStat;
    tStat.push_back(MOV);
    vector<int> tAmount;
    tAmount.push_back(0);
    Debuff *dud = new Debuff("Leg Reinforcement Cooldown", "--", getTeam(),
                             tStat, tAmount, 5);

    // Add both buffs to the player
    addDebuff(du);
    addDebuff(dud);

    log->addToEventLog(getFullName() + " used Leg Reinforcement!");

    return 0;
}

int BossRin::turnUpdate()
{
    // Randomly pick a stat
    int statChoice = rand() % 7;
    vector<Stat> tStats;
    vector<int> tAmounts;
    switch(statChoice)
    {
    case 0:
        tStats.push_back(STR);
        tAmounts.push_back(0.25 * getStr());
        break;
    case 1:
        tStats.push_back(MAG);
        tAmounts.push_back(0.25 * getMag());
        break;
    case 2:
        tStats.push_back(DEF);
        tAmounts.push_back(0.25 * getDef());
        break;
    case 3:
        tStats.push_back(RES);
        tAmounts.push_back(0.25 * getRes());
        break;
    case 4:
        tStats.push_back(SPD);
        tAmounts.push_back(0.25 * getSpd());
        break;
    case 5:
        tStats.push_back(SKL);
        tAmounts.push_back(0.25 * getSkl());
        break;
    case 6:
        tStats.push_back(LUK);
        tAmounts.push_back(0.25 * getLuk());
        break;
    default:
        tStats.push_back(MAG);
        tAmounts.push_back(0.25 * getMag());
        break;
    }

    Debuff *legRein = new Debuff("My Willpower is A-Plus!",
                                 "Your will to win grants a 25% boost to a random stat!",
                                 getTeam(), tStats, tAmounts, 1);

    addDebuff(legRein);

    return 0;
}

/***** Private Helper Functions *****/
bool BossRin::legReinforceCooldown()
{
    return hasDebuff("Leg Reinforcement Cooldown");
}

bool BossRin::azothCooldown()
{
    return hasDebuff("Azoth Sword Cooldown");
}

/***** Function Re-definitions *****/
void BossRin::setPlayField(PlayField *f)
{
    field = f;
    // Eins, Zwei, Drei range
    vector<Coordinate> npc1;
    for (int i = -2; i <= 2; i++)
    {
        for (int j = 0; j <= 2; j++)
        {
            if (abs(i) + abs(j) <= 2 && abs(i) + abs(j) != 0)
            {
                Coordinate tc;
                tc.x = i; tc.y = j;
                npc1.push_back(tc);
            }
        }
    }

    npRanges.push_back(npc1);

    // Gandr Shot Range
    npRanges.push_back(getLowToHighRange(1, field->getLargestDimension() * 2));
    // Azoth Sword Range
    npRanges.push_back(getLowToHighRange(1,1));
}

int BossRin::isActionNP(int action)
{
    int ret = -1;
    if (ascension != 2)
    {
        if (action >= 5)
            ret = action - 5;
    }
    else
    {
        if (action >= 6)
            ret = action - 6;
    }

    return ret;
}

vector<Coordinate> BossRin::getActionRange(int action)
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
        else if (action == 5) // Leg Reinforcement
        {
            range.push_back(servLoc);
        }
        else // Noble Phantasms
        {
            int ret = isActionNP(action);
            if (ret != -1)
                range = getNPRange(ret);
        }
    }
    else // First or Second Ascension
    {
        if (action == 4) // Leg Reinforcement
        {
            range.push_back(servLoc);
        }
        else // Noble Phantasms
        {
            int ret = isActionNP(action);
            if (ret != -1)
                range = getNPRange(ret);
        }
    }

    return range;
}

int BossRin::doAction(int actionNum, vector<Servant *> defenders)
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
                ret = legReinforcement();
                break;
            case 5:
                ret = activateNP1(defenders);
                break;
            case 6:
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
                ret = legReinforcement();
                break;
            case 6:
                ret = activateNP1(defenders);
                break;
            case 7:
                ret = activateNP2(defenders);
                break;
            case 8:
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
// Eins, Zwei, Drei
int BossRin::activateNP1(vector<Servant *> defenders)
{
    if ((ascension == 2 && actionMPCosts[ascension][6] > currMP) ||
        (ascension != 2 && actionMPCosts[ascension][5] > currMP))
        return 1; // Not enough MP

    if (ascension == 2)
        subMP(actionMPCosts[ascension][6]);
    else
        subMP(actionMPCosts[ascension][5]);

    log->addToEventLog(getFullName() + " used Eins, Zwei, Drei!");

    Direction atkDir = NORTH;

    for (unsigned int i = 0; i < defenders.size(); i++)
    {
        int dam = 0;
        // Check if you hit the targets
        vector<int> opEvade = defenders[i]->getEvade();
        bool hit = false;
        // Calculate accuracy
        int accuracy = capZero(getHitRate() - (opEvade[0] / 2));

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
            defenders[i]->subHP(dam, NP_MAG);
        }
        else
        {
            log->addToEventLog(getFullName() + " missed " +
                               defenders[i]->getFullName() + "!");
        }

        atkDir = getDirectionAtoB(getCurrLoc(), defenders[i]->getCurrLoc());

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

    // Jump back two spaces
    Coordinate targetLoc;
    targetLoc.x = getCurrLoc().x;
    targetLoc.y = getCurrLoc().y;

    switch (atkDir)
    {
        case NORTH:
            targetLoc.y -= 2;
            if (targetLoc.y < 0)
                targetLoc.y = 0;
            break;
        case SOUTH:
            targetLoc.y += 2;
            if (targetLoc.y >= field->getFieldLength())
                targetLoc.y = field->getFieldLength() - 1;
            break;
        case EAST:
            targetLoc.x -= 2;
            if (targetLoc.x < 0)
                targetLoc.x = 0;
            break;
        case WEST:
            targetLoc.x += 2;
            if (targetLoc.x >= field->getFieldWidth())
                targetLoc.x = field->getFieldWidth() - 1;
            break;
        default:
            break;
    }

    Coordinate realLoc = field->getNearestValidCoord(targetLoc);
    field->moveServant(this, realLoc);

    return 0;
}

// Gandr Shot
int BossRin::activateNP2(vector<Servant *> defenders)
{
    if ((ascension == 2 && actionMPCosts[ascension][7] > currMP) ||
        (ascension != 2 && actionMPCosts[ascension][6] > currMP))
        return 1; // Not enough MP

    if (ascension == 2)
        subMP(actionMPCosts[ascension][7]);
    else
        subMP(actionMPCosts[ascension][6]);

    log->addToEventLog(getFullName() + " used Gandr Shot!");

    // Get all of the servants within two spaces of the defender
    vector<Servant*> targets = field->getAllInRange(defenders[0],
                                                    getLowToHighRange(0,2));

    // Deal twice normal damage (undodgeable)
    for (unsigned int i = 0; i < targets.size(); i++)
    {
        int dam = 0;

        // No accuracy check
        int attackMult = 2;
        int critChance = capZero(getCriticalRate() -
                         targets[i]->getCriticalEvade());
        int r = getRandNum();
        if (critChance >= r)
            attackMult *= 3;

        // Deal the damage
        dam = capZero(getMag() - targets[i]->getRes()) * attackMult;
        log->addToEventLog("The Gandr detonation dealt " +
                           to_string(dam) + " damage to " +
                           targets[i]->getFullName() + "!");
        targets[i]->subHP(dam, NP_MAG);

        // Check to see if the defender is dead. If they are, and they are an
        // Avenger, activate Final Revenge.
        if(targets[i]->getCurrHP() <= 0)
        {
            if (targets[i]->getClass() == Avenger)
            {
                // Activate Final Revenge
                Debuff *finRev = targets[i]->finalRevenge();
                addDebuff(finRev);
                if (targets[i]->getAscensionLvl() == 2)
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

    // Get all of the servants on a line to the defender
    vector<Coordinate> path = field->pathFind(getCurrLoc(),
                                              defenders[0]->getCurrLoc());
    vector<Coordinate> allPath = field->getAdjacentToRange(path);

    vector<Servant*> pathTargets = field->getAllInRange2(allPath);

    // Do normal damage (dodgeable)
    for (unsigned int i = 0; i < pathTargets.size(); i++)
    {
        if (pathTargets[i] == this)
            continue;

        int dam = 0;
        // Check if you hit the targets
        vector<int> opEvade = pathTargets[i]->getEvade();
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
                             pathTargets[i]->getCriticalEvade());
            r = getRandNum();
            if (critChance >= r)
                attackMult *= 3;

            // Deal the damage
            dam = capOne(getMag() - pathTargets[i]->getRes()) * attackMult;
            log->addToEventLog("Gandr Shot dealt " +
                               to_string(dam) + " damage to " +
                               pathTargets[i]->getFullName() + ".");
            pathTargets[i]->subHP(dam, NP_MAG);
        }
        else
        {
            log->addToEventLog("Gandr Shot missed " +
                               pathTargets[i]->getFullName() + "!");
        }

        // Check to see if the defender is dead.
        if(pathTargets[i]->getCurrHP() < 0)
        {
            if (pathTargets[i]->getClass() == Avenger)
            {
                // Activate Final Revenge
                Debuff *finRev = pathTargets[i]->finalRevenge();
                addDebuff(finRev);
                if (pathTargets[i]->getAscensionLvl() == 2)
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

// Azoth Sword
int BossRin::activateNP3(vector<Servant *> defenders)
{
    if (ascension == 2 && actionMPCosts[ascension][8] > currMP)
        return 1; // Not enough MP

    if (ascension != 2)
    {
        // We shouldn't be here??
        log->addToErrorLog("Your ascension level isn't high enough to use Azoth Sword!");
        return 41;
    }

    if (azothUseCount >= 2)
    {
        log->addToErrorLog("Azoth Sword has already been used twice! It cannot be used again!");
        return 41;
    }

    if (azothCooldown())
    {
        log->addToErrorLog("Azoth Sword is on cooldown!");
        return 41;
    }

    subMP(actionMPCosts[ascension][8]);
    azothUseCount++;

    log->addToEventLog(getFullName() + " used Azoth Sword!");

    for (unsigned int i = 0; i < defenders.size(); i++)
    {
        int dam = 0;
        // Check if you hit the targets
        vector<int> opEvade = defenders[i]->getEvade();
        bool hit = false;
        // Calculate accuracy
        int accuracy = capZero(getHitRate() - (opEvade[0] / 2));

        int r = getRandNum();
        if (accuracy >= r)
            hit = true;

        // If you hit, calculate crit chance
        if (hit)
        {
            // Deal the damage
            dam = getStr();
            log->addToEventLog(getFullName() + " dealt " +
                               to_string(dam) + " damage to " +
                               defenders[i]->getFullName() + ".");
            defenders[i]->subHP(dam, NP_MAG);

            // Curse the defender
            vector<Stat> tDebStat;
            tDebStat.push_back(MOV);
            vector<int> tDebAm;
            tDebAm.push_back(0);
            Debuff *newDebuff = new Debuff("Doom",
                                           "Your days are numbered...",
                                           defenders[i]->getTeam(),
                                           tDebStat, tDebAm, 4);

            defenders[i]->addDebuff(newDebuff);
            log->addToEventLog(defenders[i]->getFullName() + " was doomed!");
        }
        else
        {
            log->addToEventLog(getFullName() + " missed " +
                               defenders[i]->getFullName() + "!");
        }

        // Check to see if the defender is dead.
        if(defenders[i]->getCurrHP() < 0)
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

    // Add the Azoth cooldown "debuff"
    vector<Stat> tStat;
    tStat.push_back(MOV);
    vector<int> tAmount;
    tAmount.push_back(0);
    Debuff *dud = new Debuff("Azoth Sword Cooldown", "--", getTeam(),
                             tStat, tAmount, 4);

    addDebuff(dud);

    return 0;
}
