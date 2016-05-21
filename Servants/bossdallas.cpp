#include "bossdallas.h"
#include "PlayField.h"

BossDallas::BossDallas(int as, Team t, Logger *l) : ServantArcher(as, t, l)
{
    name = "Dallas Jones";

    /** Stat modifiers **/
    int sklMod = 50;
    int strMod = 10;
    int spdMod = 25;
    int lukMod = 15;
    int defMod = 20;
    int magMod = -10;
    int resMod = -20;
    int maxHPMod = 100;
    int movMod = 4;

    skl[0] += sklMod;
    skl[1] += sklMod;
    skl[2] += sklMod;
    str[0] += strMod;
    str[1] += strMod;
    str[2] += strMod;
    def[0] += defMod;
    def[1] += defMod;
    def[2] += defMod;
    mag[0] += magMod;
    mag[1] += magMod;
    mag[2] += magMod;
    res[0] += resMod;
    res[1] += resMod;
    res[2] += resMod;
    spd[0] += spdMod;
    spd[1] += spdMod;
    spd[2] += spdMod;
    luk[0] += lukMod;
    luk[1] += lukMod;
    luk[2] += lukMod;
    mov[0] += movMod;
    mov[1] += movMod;
    mov[2] += movMod;
    maxHP[0] += maxHPMod;
    maxHP[1] += maxHPMod;
    maxHP[2] += maxHPMod;

    currHP = maxHP[ascension];

    hiRange += 1;
    lowRange += 1;

    actionList[2].push_back("2: Hidden Arsenal");

    actionListTypes[2].push_back(A);

    actionMPCosts[2].push_back(100);

    actionDodgeable[2].push_back(true);

    actionCounterable[2].push_back(false);

    vector<string> np1;
    np1.push_back("Master Duelist");
    np1.push_back("Passive Noble Phantasm. Years of experience as a duelist -- and later experimentation with gun magic -- make Dallas an absolute nightmare in one-on-one confrontations. +50 to SKL, +25 to SPD, +15 to LUK, +10 to STR, and +1 to range. Has a (Crit Chance / 2) chance of ignoring an opponent’s defense when he attacks (calculated separately for each one of his attacks).");
    vector<string> np2;
    np2.push_back("Stand Your Grand");
    np2.push_back("Passive Noble Phantasm. When attacked, counterattack thrice for full damage each time. All three attacks are subject to accuracy and critical checks.");
    vector<string> np3;
    np3.push_back("Hidden Arsenal");
    np3.push_back("The one thing Dallas likes about magic is that it lets him bring his entire arsenal everywhere without actually carrying anything. Begins by sniping the two farthest enemy units for full damage and doubled crit chance, follows by machine-gunning all enemy units within three spaces (attacks each player 100 times for 5% damage each), and ends by shotgunning all opponents within two spaces, injuring each target and any adjacent players for half damage and shoving them back two spaces. All attacks except the shotgun blasts are subject to accuracy checks. Costs 100 MP, has a cooldown of five turns, and can only be used twice per battle.");
    noblePhantasms.push_back(np1);
    noblePhantasms.push_back(np2);
    noblePhantasms.push_back(np3);

    hiddenArsenalCount = 0;
}

/***** Private Helper Functions *****/
bool BossDallas::hiddenArsenalCooldown()
{
    return hasDebuff("Hidden Arsenal Cooldown");
}

/***** Function Re-definitions *****/
void BossDallas::setPlayField(PlayField *f)
{
    field = f;

    // NP Ranges
    // Master Duelist and Stand Your Ground don't have ranges
    npRanges.push_back(getLowToHighRange(0,0));
    npRanges.push_back(getLowToHighRange(0,0));
    // Hidden Arsenal
    npRanges.push_back(getLowToHighRange(1,field->getLargestDimension() * 2));
}

int BossDallas::doAction(int actionNum, vector<Servant *> defenders)
{
    int ret = 0;
    switch(actionNum)
    {
        case 0:
            ret = attack(defenders, true);
            break;
        case 1:
            ret = activateNP1(defenders);
            break;
        default:
            return 2; // Not a valid choice
            break;
    }
    return ret;
}

int BossDallas::isActionNP(int action)
{
    int ret = -1;
    if (action == 1)
        ret = 2;
    return ret;
}

vector<Coordinate> BossDallas::getActionRange(int action)
{
    vector<Coordinate> range;

    if (action == 0)
    {
        range = getLowToHighRange(getLowRange(), getHighRange());
    }
    else
    {
        int ret = isActionNP(action);
        if (ret != -1)
            range = getNPRange(ret);
    }

    return range;
}

int BossDallas::attack(vector<Servant *> defenders, bool counter)
{
    if (actionMPCosts[ascension][0] > currMP)
        return 1; // Not enough MP to attack

    subMP(actionMPCosts[ascension][0]);

    for (unsigned int i = 0; i < defenders.size(); i++)
    {
        // STAND YOUR GROUND
        // If counterattacking, attack thrice. Otherwise, attack once.
        // Only if ascension level is 1 or higher, of course.
        int attackTimes = 1;
        if (!counter && ascension >= 1)
            attackTimes = 3;

        bool enemyDead = false;
        for (int j = 0; j < attackTimes && !enemyDead; j++)
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
                double attackMult = 1.0;
                int critChance = capZero(getCriticalRate() -
                                 defenders[i]->getCriticalEvade());
                r = getRandNum();
                if (critChance >= r)
                    attackMult *= 3;

                // Deal the damage
                // MASTER DUELIST: Crit Chance / 2 chance of ignoring opponents'
                // defense
                r = getRandNum();
                if (r <= critChance / 2)
                {
                    dam = (int) capZero(getStr() * attackMult);
                    log->addToEventLog(getFullName() + " the Master Duelist bypassed " +
                                       defenders[i]->getFullName() + "'s defenses!");
                }
                else
                    dam = (int) (capZero(getStr() - defenders[i]->getDef()) * attackMult);

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
                if (defenders[i]->isInRange(this) && counter && j == attackTimes - 1)
                {
                    vector<Servant *> you;
                    you.push_back(this);
                    defenders[i]->attack(you, false);
                }
            }
            else
            {
                enemyDead = true;
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

/***** Noble Phantasms *****/
// Hidden Arsenal (Ultimate NP)
int BossDallas::activateNP1(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][1] > currMP)
        return 1; // Not enough MP to attack

    if (hiddenArsenalCount >= 2)
    {
        log->addToErrorLog("You've already used Hidden Arsenal twice!");
        return 41;
    }

    if (hiddenArsenalCooldown())
    {
        log->addToErrorLog("Hidden Arsenal is still on Cooldown!");
        return 41;
    }

    subMP(actionMPCosts[ascension][1]);
    hiddenArsenalCount++;

    log->addToEventLog(getFullName() + " used his Hidden Arsenal!");

    // Remove allied players from the defenders vector
    for (int i = 0; i < (int) defenders.size(); i++)
    {
        if (defenders[i]->getTeam() == getTeam())
        {
            defenders.erase(defenders.begin()+i);
            i--;
        }
    }

    /** Get farthest two players, deal full damage with doubled crit chance **/
    vector<Servant*> farthestTwo;
    Servant* maxOne = NULL;
    Servant* maxTwo = NULL;
    int maxDist1 = 0;
    int maxDist2 = 0;
    for (unsigned int i = 0; i < defenders.size(); i++)
    {
        int dist = getDistance(defenders[i]->getCurrLoc(), getCurrLoc());
        if (dist >= maxDist1)
        {
            maxDist2 = maxDist1;
            maxTwo = maxOne;
            maxDist1 = dist;
            maxOne = defenders[i];
        }
        else if (dist >= maxDist2)
        {
            maxDist2 = dist;
            maxTwo = defenders[i];
        }
    }

    farthestTwo.push_back(maxOne);
    farthestTwo.push_back(maxTwo);

    // Do the damage
    for (unsigned int i = 0; i < farthestTwo.size(); i++)
    {
        int dam = 0;
        // Check if you hit the targets
        vector<int> opEvade = farthestTwo[i]->getEvade();
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
        // Sniper shots have doubled crit chance
        if (hit)
        {
            double attackMult = 1.0;
            int critChance = capZero(getCriticalRate() -
                             farthestTwo[i]->getCriticalEvade()) * 2;
            r = getRandNum();
            if (critChance >= r)
                attackMult *= 3;

            // Deal the damage
            // MASTER DUELIST: Crit Chance / 2 chance of ignoring opponents'
            // defense
            r = getRandNum();
            if (r <= critChance / 2)
            {
                dam = (int) capZero(getStr() * attackMult);
                log->addToEventLog(getFullName() + " the Master Duelist bypassed " +
                                   farthestTwo[i]->getFullName() + "'s defenses!");
            }
            else
                dam = (int) (capZero(getStr() - farthestTwo[i]->getDef()) * attackMult);

            log->addToEventLog(getFullName() + " sniped " +
                               farthestTwo[i]->getFullName() + " for " +
                               to_string(dam) + " damage!");
            farthestTwo[i]->subHP(dam, NP_STR);
        }
        else
        {
            log->addToEventLog(getFullName() + " missed " +
                               farthestTwo[i]->getFullName() + "!");
        }

        // Check to see if the defender is dead.
        if(farthestTwo[i]->getCurrHP() <= 0)
        {
            if (farthestTwo[i]->getClass() == Avenger)
            {
                // Activate Final Revenge
                Debuff *finRev = farthestTwo[i]->finalRevenge();
                addDebuff(finRev);
                if (farthestTwo[i]->getAscensionLvl() == 2)
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

    /** Get all players within three spaces, randomly pick opponents 100 times
        and deal 5% damage. **/
    vector<Servant*> range3 = field->getAllInRange(this, getLowToHighRange(1,3));
    // Remove allied units
    for (int i = 0; i < (int) range3.size(); i++)
    {
        if (range3[i]->getTeam() == getTeam())
        {
            range3.erase(range3.begin() + i);
            i--;
        }
    }

    bool allDead = false;
    for (int i = 0; i < 100 && !allDead && range3.size() > 0; i++)
    {
        // Randomly pick a target
        Servant *target = range3[0];
        int tInd = 0;
        do
        {
            tInd = rand() % range3.size();
            target = range3[tInd];
        } while (target->getCurrHP() <= 0);

        int dam = 0;
        // Check if you hit the targets
        vector<int> opEvade = target->getEvade();
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
            double attackMult = 0.05;
            int critChance = capZero(getCriticalRate() -
                             target->getCriticalEvade());
            r = getRandNum();
            if (critChance >= r)
                attackMult *= 3;

            // Deal the damage
            // MASTER DUELIST: Crit Chance / 2 chance of ignoring opponents'
            // defense
            r = getRandNum();
            if (r <= critChance / 2)
            {
                dam = (int) capZero(getStr() * attackMult);
                log->addToEventLog(getFullName() + " the Master Duelist bypassed " +
                                   target->getFullName() + "'s defenses!");
            }
            else
                dam = (int) (capZero(getStr() - target->getDef()) * attackMult);

            log->addToEventLog(getFullName() + " machine gunned " +
                               target->getFullName() + " for " +
                               to_string(dam) + " damage!");
            target->subHP(dam, NP_STR);
        }
        else
        {
            log->addToEventLog(getFullName() + " missed " +
                               target->getFullName() + "!");
        }

        // Check to see if the defender is dead.
        if(target->getCurrHP() <= 0)
        {
            if (target->getClass() == Avenger)
            {
                // Activate Final Revenge
                Debuff *finRev = target->finalRevenge();
                addDebuff(finRev);
                if (target->getAscensionLvl() == 2)
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

        // Check to see if all of the defenders are dead; if they are, we stop
        //  the attack early.
        allDead = true;
        for (unsigned int j = 0; j < range3.size() && allDead; j++)
        {
            if (range3[j]->getCurrHP() > 0)
                allDead = false;
        }
    }

    /** Get all players within two spaces, damage all for half damage and push
        them back four spaces. **/
    vector<Servant*> range2 = field->getAllInRange(this, getLowToHighRange(1,2));
    // Remove allied units
    for (int i = 0; i < (int) range2.size(); i++)
    {
        if (range2[i]->getTeam() == getTeam())
        {
            range2.erase(range2.begin() + i);
            i--;
        }
    }

    // Do the damage
    for (unsigned int i = 0; i < range2.size(); i++)
    {
        int shoveDistance = 4;
        // Get the target's current location
        Coordinate targetLoc = range2[i]->getCurrLoc();

        // Figure out which direction from the player they are
        Direction fromPlayer = getDirectionAtoB(getCurrLoc(), targetLoc);

        // Create a new target location for the target based on their current
        //  location and their direction from the player
        Coordinate finalLoc;
        if (fromPlayer == NORTH)
        {
            finalLoc.x = targetLoc.x;
            finalLoc.y = targetLoc.y + shoveDistance;
            if (finalLoc.y >= field->getFieldLength())
                finalLoc.y = field->getFieldLength() - 1;
        }
        else if (fromPlayer == SOUTH)
        {
            finalLoc.x = targetLoc.x;
            finalLoc.y = targetLoc.y - shoveDistance;
            if (finalLoc.y < 0)
                finalLoc.y = 0;
        }
        else if (fromPlayer == EAST)
        {
            finalLoc.x = targetLoc.x + shoveDistance;
            finalLoc.y = targetLoc.y;
            if (finalLoc.x >= field->getFieldWidth())
                finalLoc.x = field->getFieldWidth() - 1;
        }
        else // West
        {
            finalLoc.x = targetLoc.x - shoveDistance;
            finalLoc.y = targetLoc.y;
            if (finalLoc.x < 0)
                finalLoc.x = 0;
        }

        // Get the nearest valid coordinate from the "finalLoc"
        Coordinate actualLoc = field->getNearestValidCoord(finalLoc);

        // Move the target Servant to this location
        field->moveServant(range2[i], actualLoc);

        // Do the damage
        int dam = 0;
        double attackMult = 0.5;
        int critChance = capZero(getCriticalRate() -
                         range2[i]->getCriticalEvade()) * 2;
        int r = getRandNum();
        if (critChance >= r)
            attackMult *= 3;

        // Deal the damage
        // MASTER DUELIST: Crit Chance / 2 chance of ignoring opponents'
        // defense
        r = getRandNum();
        if (r <= critChance / 2)
        {
            dam = (int) capZero(getStr() * attackMult);
            log->addToEventLog(getFullName() + " the Master Duelist bypassed " +
                               range2[i]->getFullName() + "'s defenses!");
        }
        else
            dam = (int) (capZero(getStr() - range2[i]->getDef()) * attackMult);

        log->addToEventLog(getFullName() + " shotgunned " +
                           range2[i]->getFullName() + " for " +
                           to_string(dam) + " damage and blasted them backward!");
        range2[i]->subHP(dam, NP_STR);

        // Check to see if the defender is dead.
        if(range2[i]->getCurrHP() <= 0)
        {
            if (range2[i]->getClass() == Avenger)
            {
                // Activate Final Revenge
                Debuff *finRev = range2[i]->finalRevenge();
                addDebuff(finRev);
                if (range2[i]->getAscensionLvl() == 2)
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

    // Create Cooldown debuff
    vector<Stat> tStat;
    tStat.push_back(STR);
    vector<int> tAmount;
    tAmount.push_back(0);
    Debuff *dud = new Debuff("Hidden Arsenal Cooldown", "--", getTeam(),
                             tStat, tAmount, 6);
    addDebuff(dud);


    return 0;
}
