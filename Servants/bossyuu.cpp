#include "bossyuu.h"
#include "PlayField.h"

// character's full name: Yuichiro Hyakuya
BossYuu::BossYuu(int as, Team t, Logger *l) : ServantSaber(as, t, l)
{
    name = "Yuichiro";

    /** Stat modifiers **/
    /*int strMod = 10;
    int spdMod = 10;
    int sklMod = 5;
    int defMod = 5;

    spd[0] += spdMod;
    spd[1] += spdMod;
    spd[2] += spdMod;
    str[0] += strMod;
    str[1] += strMod;
    str[2] += strMod;
    skl[0] += sklMod;
    skl[1] += sklMod;
    skl[2] += sklMod;
    def[0] += defMod;
    def[1] += defMod;
    def[2] += defMod;*/

    actionList[0].push_back("4: Drugging Up");
    actionList[0].push_back("5: Asuramaru");
    actionList[1].push_back("4: Drugging Up");
    actionList[1].push_back("5: Asuramaru");
    actionList[1].push_back("6: Asura-Kannon");
    actionList[2].push_back("4: Drugging Up");
    actionList[2].push_back("5: Asuramaru");
    actionList[2].push_back("6: Asura-Kannon");
    actionList[2].push_back("7: Seraph of the End");

    actionListTypes[0].push_back(N);
    actionListTypes[0].push_back(N);
    actionListTypes[1].push_back(N);
    actionListTypes[1].push_back(N);
    actionListTypes[1].push_back(A);
    actionListTypes[2].push_back(N);
    actionListTypes[2].push_back(N);
    actionListTypes[2].push_back(A);
    actionListTypes[2].push_back(N);

    actionMPCosts[0].push_back(0);
    actionMPCosts[0].push_back(25);
    actionMPCosts[1].push_back(0);
    actionMPCosts[1].push_back(25);
    actionMPCosts[1].push_back(50);
    actionMPCosts[2].push_back(0);
    actionMPCosts[2].push_back(25);
    actionMPCosts[2].push_back(50);
    actionMPCosts[2].push_back(100);

    actionDodgeable[0].push_back(false);
    actionDodgeable[0].push_back(false);
    actionDodgeable[1].push_back(false);
    actionDodgeable[1].push_back(false);
    actionDodgeable[1].push_back(true);
    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(true);
    actionDodgeable[2].push_back(false);

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
    np1.push_back("Asuramaru");
    np1.push_back("Call out Asuramaru’s name to get a +15 boost to STR, SPD, and SKL for five turns. Costs 25 MP and has a cooldown of six turns.");
    vector<string> np2;
    np2.push_back("Asura-Kannon");
    np2.push_back("Materialize dozens of floating blades and viciously tear into any opponents directly adjacent to you. Launches 48 attacks, each causing 10% damage and selecting an opponent at random. Accuracy and critical checks apply. Costs 50 MP and has a cooldown of three turns.");
    vector<string> np3;
    np3.push_back("Seraph of the End");
    np3.push_back("At the sacrifice of all higher functions, gain a +25 boost to all stats. Overrides boosts from Asuramaru and Drugging Up, and lasts six turns. Can only use your normal attack while this is active, and must always approach opponents (similar to Berserkers). Normal attack takes on the Noble Phantasm trait while this is active. Costs 100 MP and has a cooldown of ten turns.");
    noblePhantasms.push_back(np1);
    noblePhantasms.push_back(np2);
    noblePhantasms.push_back(np3);

    // Asuramaru range (doesn't really have one)
    npRanges.push_back(getLowToHighRange(0,0));
    // Asura-Kannon range
    npRanges.push_back(getLowToHighRange(1,1));
    // Seraph of the End range (doesn't really have one)
    npRanges.push_back(getLowToHighRange(0,0));

    /** Passive Skill modifiers **/
    vector<Stat> hdS;
    hdS.push_back(STR);
    hdS.push_back(DEF);
    hdS.push_back(SPD);
    hdS.push_back(SKL);
    vector<int> hdA;
    hdA.push_back(10);
    hdA.push_back(5);
    hdA.push_back(10);
    hdA.push_back(5);
    Debuff *highDivinity = new Debuff("Seraph Hybrid", "Passive Skill",
                                      t, hdS, hdA, -1);
    addDebuff(highDivinity);
}

/***** Active Skills *****/
int BossYuu::druggingUp()
{
    if (actionMPCosts[ascension][3] > currMP)
        return 1; // Not enough MP to attack

    if (druggedUp())
    {
        log->addToEventLog("You're already drugged up!");
        return 41;
    }

    subMP(actionMPCosts[ascension][3]);

    // Create the buff
    // Magnitude of debuff
    int statDrawback = -10;
    int statBonus = 5 + abs(statDrawback);

    vector<Stat> tStats;
    tStats.push_back(STR);
    tStats.push_back(MAG);
    tStats.push_back(DEF);
    tStats.push_back(RES);
    tStats.push_back(SPD);
    tStats.push_back(SKL);
    tStats.push_back(LUK);
    vector<int> tAmounts;
    tAmounts.push_back(statBonus);
    tAmounts.push_back(statBonus);
    tAmounts.push_back(statBonus);
    tAmounts.push_back(statBonus);
    tAmounts.push_back(statBonus);
    tAmounts.push_back(statBonus);
    tAmounts.push_back(statBonus);
    Debuff* du = new Debuff("Drugging Up Upper",
                            "You drugged up!", getTeam(), tStats, tAmounts, 3);

    // Create the cooldown "buff"
    vector<Stat> tStat;
    tStat.push_back(STR);
    tStat.push_back(MAG);
    tStat.push_back(DEF);
    tStat.push_back(RES);
    tStat.push_back(SPD);
    tStat.push_back(SKL);
    tStat.push_back(LUK);
    vector<int> tAmount;
    tAmount.push_back(statDrawback);
    tAmount.push_back(statDrawback);
    tAmount.push_back(statDrawback);
    tAmount.push_back(statDrawback);
    tAmount.push_back(statDrawback);
    tAmount.push_back(statDrawback);
    tAmount.push_back(statDrawback);
    Debuff *dud = new Debuff("Drugging Up Downer",
                             "The drawbacks of drugging up...", getTeam(),
                             tStat, tAmount, 8);

    // Add both buffs to the player
    addDebuff(du);
    addDebuff(dud);

    log->addToEventLog(getFullName() + " got drugged up!");

    return 0;
}

/***** Cooldown Functions *****/
bool BossYuu::druggedUp()
{
    bool cooldown = false;
    for (int i = 0; i < (int) debuffs.size(); i++)
    {
        if(debuffs[i]->getDebuffName().compare("Drugging Up Downer") == 0)
        {
            cooldown = true;
        }
    }

    return cooldown;
}

bool BossYuu::asuramaruCooldown()
{
    bool cooldown = false;
    for (int i = 0; i < (int) debuffs.size(); i++)
    {
        if(debuffs[i]->getDebuffName().compare("Asuramaru Cooldown") == 0)
        {
            cooldown = true;
        }
    }

    return cooldown;
}

bool BossYuu::asuraKannonCooldown()
{
    bool cooldown = false;
    for (int i = 0; i < (int) debuffs.size(); i++)
    {
        if(debuffs[i]->getDebuffName().compare("Asura-Kannon Cooldown") == 0)
        {
            cooldown = true;
        }
    }

    return cooldown;
}

bool BossYuu::seraphOfTheEndCooldown()
{
    bool cooldown = false;
    for (int i = 0; i < (int) debuffs.size(); i++)
    {
        if(debuffs[i]->getDebuffName().compare("Seraph of the End Cooldown") == 0)
        {
            cooldown = true;
        }
    }

    return cooldown;
}

/***** Function Re-definitions *****/
bool BossYuu::isBerserk()
{
    bool berserk = false;
    for (int i = 0; i < (int) debuffs.size(); i++)
    {
        if(debuffs[i]->getDebuffName().compare("Seraph of the End") == 0)
        {
            berserk = true;
        }
    }

    return berserk;
}

int BossYuu::doAction(int actionNum, vector<Servant *> defenders)
{
    int ret = 0;
    if (isBerserk())
    {
        if (actionNum == 0)
            ret = attack(defenders, true);
        else
        {
            log->addToEventLog("You are berserk! You cannot do that!");
            return 41;
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
                ret = guardianKnight(defenders);
                break;
            case 2:
                ret = manaBurst(defenders);
                break;
            case 3:
                ret = druggingUp();
                break;
            case 4:
                ret = activateNP1(defenders);
                break;
            case 5:
                ret = activateNP2(defenders);
                break;
            case 6:
                ret = activateNP3(defenders);
                break;
            default:
                return 2; // Not a valid choice
                break;
        }
    }
    return ret;
}

vector<Coordinate> BossYuu::getActionRange(int action)
{
    // Figure out what action this is and return the appropriate range
    vector<Coordinate> range;

    if (action == 0) // Regular attack range
    {
        range = getLowToHighRange(1,1);
    }
    else if (action == 1) // Guardian Knight range
    {
        range = getLowToHighRange(1,3);
    }
    else if (action == 2 || action == 3) // Mana Burst/Drugging Up Range
    {
        range = getLowToHighRange(0,0);
    }
    else
    {
        range = getNPRange(action-4);
    }

    return range;
}

int BossYuu::isActionNP(int action)
{
    int ret = -1;
    if (action >= 4)
        ret = action - 4;

    return ret;
}

int BossYuu::attack(vector<Servant *> defenders, bool counter)
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
                dam = capZero(getStr() - defenders[i]->getDef()) * attackMult;
                log->addToEventLog(getFullName() + " dealt " +
                                   to_string(dam) + " damage to " +
                                   defenders[i]->getFullName() + ".");
                if (isBerserk())
                    defenders[i]->subHP(dam, NP_STR);
                else
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
                                           "'s Mad Counter activated, dealing " +
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

/***** Noble Phantasms *****/
// Asuramaru
int BossYuu::activateNP1(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][4] > currMP)
        return 1; // Not enough MP to attack

    if (asuramaruCooldown())
    {
        log->addToEventLog("Asuramaru is on cooldown!");
        return 41;
    }

    subMP(actionMPCosts[ascension][4]);

    // Create the buff
    // Magnitude of debuff
    int statBonus = 15;

    vector<Stat> tStats;
    tStats.push_back(STR);
    tStats.push_back(SPD);
    tStats.push_back(SKL);
    vector<int> tAmounts;
    tAmounts.push_back(statBonus);
    tAmounts.push_back(statBonus);
    tAmounts.push_back(statBonus);
    Debuff* asura = new Debuff("Asuramaru",
                               "You called on the demon blade to grow stronger!",
                               getTeam(), tStats, tAmounts, 5);

    // Create the cooldown "buff"
    vector<Stat> tS;
    tS.push_back(MOV);
    vector<int> tA;
    tA.push_back(0);
    Debuff *asuraCooldown = new Debuff("Asuramaru Cooldown",
                                       "--", getTeam(), tS, tA, 6);

    // Add both buffs to the player
    addDebuff(asura);
    addDebuff(asuraCooldown);

    log->addToEventLog(getFullName() + " activated Asuramaru!");

    return 0;
}

// Asura-Kannon
int BossYuu::activateNP2(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][5] > currMP)
        return 1; // Not enough MP to attack

    if (asuraKannonCooldown())
    {
        log->addToEventLog("Asura-Kannon is on cooldown!");
        return 41;
    }

    subMP(actionMPCosts[ascension][5]);

    // Move to be adjacent to the target servant
    log->addToEventLog(getFullName() + " used Asura-Kannon!");

    // Remove teammates from the defenders vector
    for (int i = 0; i < (int) defenders.size(); i++)
    {
        if (defenders[i]->getTeam() == getTeam())
        {
            defenders.erase(defenders.begin()+i);
            i--;
        }
    }

    // 30 times in a row, randomly pick a target and do damage
    bool allDead = false;
    for (int i = 0; i < 48 && !allDead && defenders.size() > 0; i++)
    {
        // Randomly pick a target
        Servant *target = defenders[0];
        int tInd = 0;
        do
        {
            tInd = rand() % defenders.size();
            target = defenders[tInd];
        } while (target->getCurrHP() <= 0);

        // Check if you hit the target
        vector<int> opEvade = defenders[0]->getEvade();
        bool hit = false;
        // Calculate accuracy
        int accuracy = capZero(getHitRate() - opEvade[0]);
        int dam = 0;

        int r = getRandNum();
        if (accuracy >= r)
            hit = true;

        if (hit)
        {
            // Check to see if it crits
            double attackMult = 0.1;
            int critChance = capZero(getCriticalRate() -
                                     target->getCriticalEvade());
            r = getRandNum();
            if (critChance >= r)
                attackMult *= 3;

            dam = (int) capOne(capOne(getStr() - target->getDef()) * attackMult);
            log->addToEventLog(getFullName() + " dealt " +
                               to_string(dam) + " damage to " +
                               target->getFullName() + ".");
            target->subHP(dam, NP_STR);
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
        for (unsigned int j = 0; j < defenders.size() && allDead; j++)
        {
            if (defenders[j]->getCurrHP() > 0)
                allDead = false;
        }
    }

    return 0;
}

// Seraph of the End
int BossYuu::activateNP3(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][6] > currMP)
        return 1; // Not enough MP to attack

    if (isBerserk())
    {
        log->addToEventLog("You are already berserk!");
        return 41;
    }

    if (seraphOfTheEndCooldown())
    {
        log->addToEventLog("Seraph of the End is on cooldown!");
        return 41;
    }

    subMP(actionMPCosts[ascension][6]);

    // Remove the boosts from Drugging Up and Asuramaru, if they exist
    for (int i = 0; i < (int) debuffs.size(); i++)
    {
        if(debuffs[i]->getDebuffName().compare("Asuramaru") == 0 ||
                debuffs[i]->getDebuffName().compare("Asuramaru Cooldown") == 0 ||
                debuffs[i]->getDebuffName().compare("Drugging Up Upper") == 0 ||
                debuffs[i]->getDebuffName().compare("Drugging Up Downer") == 0)
        {
            debuffs.erase(debuffs.begin()+i);
            i--;
        }
    }

    // Create the Seraph of the End buff
    // Magnitude of debuff
    int statBonus = 25;

    vector<Stat> tStats;
    tStats.push_back(STR);
    tStats.push_back(MAG);
    tStats.push_back(DEF);
    tStats.push_back(RES);
    tStats.push_back(SPD);
    tStats.push_back(SKL);
    tStats.push_back(LUK);
    vector<int> tAmounts;
    tAmounts.push_back(statBonus);
    tAmounts.push_back(statBonus);
    tAmounts.push_back(statBonus);
    tAmounts.push_back(statBonus);
    tAmounts.push_back(statBonus);
    tAmounts.push_back(statBonus);
    tAmounts.push_back(statBonus);
    Debuff* seraph = new Debuff("Seraph of the End",
                                "You've gone berserk!",
                                getTeam(), tStats, tAmounts, 6);

    // Create the cooldown "buff"
    vector<Stat> tS;
    tS.push_back(MOV);
    vector<int> tA;
    tA.push_back(0);
    Debuff *seraphCooldown = new Debuff("Seraph of the End Cooldown",
                                        "--", getTeam(), tS, tA, 10);

    // Add both buffs to the player
    addDebuff(seraph);
    addDebuff(seraphCooldown);

    log->addToEventLog(getFullName() + " activated Seraph of the End and went berserk!");

    return 0;
}
