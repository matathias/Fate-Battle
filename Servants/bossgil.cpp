#include "bossgil.h"
#include "PlayField.h"

BossGil::BossGil(int as, Team t, Logger *l) : ServantArcher(as, t, l)
{
    name = "Gilgamesh";

    /** Passive Skill modifiers **/
    int mainBonus = 10;
    int extraBonus = 15;
    vector<Stat> hdS;
    hdS.push_back(MOV);
    hdS.push_back(STR);
    hdS.push_back(DEF);
    hdS.push_back(MAG);
    hdS.push_back(RES);
    hdS.push_back(SPD);
    hdS.push_back(SKL);
    hdS.push_back(LUK);
    vector<int> hdA;
    hdA.push_back(1);
    hdA.push_back(mainBonus + extraBonus);
    hdA.push_back(mainBonus);
    hdA.push_back(mainBonus);
    hdA.push_back(mainBonus);
    hdA.push_back(mainBonus);
    hdA.push_back(mainBonus + extraBonus);
    hdA.push_back(mainBonus);
    Debuff *highDivinity = new Debuff("High Divinity", "Passive Skill",
                                      t, hdS, hdA, -1);
    addDebuff(highDivinity);

    vector<Stat> dcS;
    dcS.push_back(MAXHP);
    dcS.push_back(MAXMP);
    vector<int> dcA;
    dcA.push_back(150);
    dcA.push_back(150);
    Debuff *drankCup = new Debuff("Drank From The Cup", "Passive Skill",
                                  t, dcS, dcA, -1);
    addDebuff(drankCup);

    currHP = getMaxHP();
    currMP = getMaxMP();

    actionList[0].push_back("2: NP: Chains of Enkidu");
    actionList[1].push_back("2: NP: Chains of Enkidu");
    actionList[1].push_back("3: NP: Gate of Babylon");
    actionList[2].push_back("2: NP: Chains of Enkidu");
    actionList[2].push_back("3: NP: Gate of Babylon");
    actionList[2].push_back("4: NP: Ea");

    actionListTypes[0].push_back(A);
    actionListTypes[1].push_back(A);
    actionListTypes[1].push_back(A);
    actionListTypes[2].push_back(A);
    actionListTypes[2].push_back(A);
    actionListTypes[2].push_back(A);

    actionMPCosts[0].push_back(50);
    actionMPCosts[1].push_back(50);
    actionMPCosts[1].push_back(100);
    actionMPCosts[2].push_back(50);
    actionMPCosts[2].push_back(100);
    actionMPCosts[2].push_back(150);

    actionDodgeable[0].push_back(false);
    actionDodgeable[1].push_back(false);
    actionDodgeable[1].push_back(true);
    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(true);
    actionDodgeable[2].push_back(false);

    actionCounterable[0].push_back(false);
    actionCounterable[1].push_back(false);
    actionCounterable[1].push_back(false);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);

    vector<string> np1;
    np1.push_back("Chains of Enkidu");
    np1.push_back("Immobilize the 4 highest-HP players on the other team, and reduce the movement of all other enemy players by half. Lasts two turns. Costs 50 MP but has a cooldown of three turns.");
    vector<string> np2;
    np2.push_back("Gate of Babylon");
    np2.push_back("The weapon version of bullet spam! Launch 100 weapons at everything in a 5-width column to the edge of the map, dealing damage to random enemy players for 10% damage each hit. Accuracy and critical checks apply. Costs 100 MP but has a cooldown of one turn.");
    vector<string> np3;
    np3.push_back("Ea");
    np3.push_back("If a Reality Marble is active, cancel it out completely and steal all of the owner’s MP. If a Reality Marble is not active, attack all enemy players for undodgeable 100 HP. Costs 150 MP but has a cooldown of four turns.");
    noblePhantasms.push_back(np1);
    noblePhantasms.push_back(np2);
    noblePhantasms.push_back(np3);
}

/***** Private Helper Functions *****/
bool BossGil::chainsCooldown()
{
    return hasDebuff("Chains of Enkidu Cooldown");
}

bool BossGil::gateCooldown()
{
    return hasDebuff("Gate of Babylon Cooldown");
}

bool BossGil::eaCooldown()
{
    return hasDebuff("Ea Cooldown");
}

/***** Function Re-definitions *****/
void BossGil::setPlayField(PlayField *f)
{
    field = f;

    // Chains of Enkidu range (whole playfield)
    npRanges.push_back(getLowToHighRange(1, field->getLargestDimension() * 2));

    // Gate of Babylon range
    vector<Coordinate> gobRange;
    for (int i = 0; i < field->getLargestDimension(); i++)
    {
        for (int j = -2; j <= 2; j++)
        {
            if (!(i == 0 && j == 0))
            {
                Coordinate c;
                c.x = j;
                c.y = i;
                gobRange.push_back(c);
            }
        }
    }
    npRanges.push_back(gobRange);

    // Ea range (whole playfield)
    npRanges.push_back(getLowToHighRange(1, field->getLargestDimension() * 2));
}

void BossGil::subHP(int hp, DamageType dt)
{
    bool applyDamage = true;
    if (currHP >= 0.1 * getMaxHP() && currHP - hp <= 1)
    {
        log->addToEventLog(getFullName() + "'s High Divinity allowed him to resist damage!");
        applyDamage = false;
    }
    else if (hp >= 0.05 * getMaxHP())
    {
        hp = 0.05 * getMaxHP();
        log->addToEventLog(getFullName() + "'s High Divinity capped the damage he takes at "
                           + to_string(hp) + "!");
    }

    if (applyDamage)
    {
        currHP -= hp;
        int mhp = getMaxHP();
        if (currHP > mhp)
            currHP = mhp;
    }

    if (currHP <= 0)
    {
        currHP = 0;
        remAllDebuffs(false);
    }
    else
    {
        // If The Facade is already added, remove the old one and add the new one
        for (int i = 0; i < (int) debuffs.size(); i++)
        {
            if (debuffs[i]->getDebuffName().compare("The Facade") == 0)
            {
                debuffs.erase(debuffs.begin() + i);
                i--;
            }
        }

        /** Skill: The Facade **/
        int malus = -1 * ((double) (getMaxHP() - currHP) / (double) getMaxHP()) * 10;
        int bonus = ((double) (getMaxHP() - currHP) / (double) getMaxHP()) * 20;
        vector<Stat> hdS;
        hdS.push_back(STR);
        hdS.push_back(DEF);
        hdS.push_back(MAG);
        hdS.push_back(RES);
        hdS.push_back(SPD);
        hdS.push_back(SKL);
        hdS.push_back(LUK);
        vector<int> hdA;
        hdA.push_back(bonus);
        hdA.push_back(malus);
        hdA.push_back(malus);
        hdA.push_back(malus);
        hdA.push_back(malus);
        hdA.push_back(malus);
        hdA.push_back(malus);
        Debuff *facade = new Debuff("The Facade", "Passive Skill",
                                    getTeam(), hdS, hdA, -1);
        addDebuff(facade);
    }
}

int BossGil::doAction(int actionNum, vector<Servant *> defenders)
{
    int ret = 0;
    switch (actionNum)
    {
        case 0:
            ret = attack(defenders, true);
            break;
        case 1:
            ret = activateNP1(defenders);
            break;
        case 2:
            ret = activateNP2(defenders);
            break;
        case 3:
            ret = activateNP3(defenders);
            break;
        default:
            return 2; // Not a valid choice
            break;
    }
    return ret;
}

int BossGil::isActionNP(int action)
{
    return (action - 1);
}

vector<Coordinate> BossGil::getActionRange(int action)
{
    vector<Coordinate> range;

    if (action == 0)
    {
        range = getLowToHighRange(getLowRange(), getHighRange());
    }
    else
    {
        range = getNPRange(action - 1);
    }

    return range;
}

/***** Noble Phantasms *****/
// Chains of Enkidu
int BossGil::activateNP1(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][1] > currMP)
        return 1; // Not enough MP to attack

    if (chainsCooldown())
    {
        log->addToErrorLog("Chains of Enkidu is still on Cooldown!");
        return 41;
    }

    subMP(actionMPCosts[ascension][1]);

    log->addToEventLog(getFullName() + " used the Chains of Enkidu!");

    // Remove allied players from defenders
    for (int i = 0; i < (int) defenders.size(); i++)
    {
        if (defenders[i]->getTeam() == getTeam())
        {
            defenders.erase(defenders.begin() + i);
            i--;
        }
    }

    // Find the four highest-HP players
    vector<Servant*> highest;
    vector<Servant*> rest;
    Servant *maxOne = NULL;
    Servant *maxTwo = NULL;
    Servant *maxThree = NULL;
    Servant *maxFour = NULL;
    int max1HP = 0;
    int max2HP = 0;
    int max3HP = 0;
    int max4HP = 0;
    for (unsigned int i = 0; i < defenders.size(); i++)
    {
        int cHP = defenders[i]->getCurrHP();
        if (cHP > max1HP)
        {
            if (maxFour != NULL)
                rest.push_back(maxFour);
            maxFour = maxThree;
            max4HP = max3HP;
            maxThree = maxTwo;
            max3HP = max2HP;
            maxTwo = maxOne;
            max2HP = max1HP;
            maxOne = defenders[i];
            max1HP = cHP;
        }
        else if (cHP > max2HP)
        {
            if (maxFour != NULL)
                rest.push_back(maxFour);
            maxFour = maxThree;
            max4HP = max3HP;
            maxThree = maxTwo;
            max3HP = max2HP;
            maxTwo = defenders[i];
            max2HP = cHP;
        }
        else if (cHP > max3HP)
        {
            if (maxFour != NULL)
                rest.push_back(maxFour);
            maxFour = maxThree;
            max4HP = max3HP;
            maxThree = defenders[i];
            max3HP = cHP;
        }
        else if (cHP > max4HP)
        {
            if (maxFour != NULL)
                rest.push_back(maxFour);
            maxFour = defenders[i];
            max4HP = cHP;
        }
        else
        {
            rest.push_back(defenders[i]);
        }
    }

    highest.push_back(maxOne);
    highest.push_back(maxTwo);
    highest.push_back(maxThree);
    highest.push_back(maxFour);

    // Immobilize the highest-HP players
    for (unsigned int i = 0; i < highest.size(); i++)
    {
        if (highest[i] != NULL)
        {
            vector<Stat> iS;
            iS.push_back(MOV);
            vector<int> iA;
            iA.push_back(-100);
            Debuff *chains = new Debuff("Chains of Enkidu",
                                        "You have been immobilized by the Chains of Enkidu!",
                                        highest[i]->getTeam(), iS, iA, 3);
            highest[i]->addDebuff(chains);
        }
    }

    // Half the movement of the rest
    for (unsigned int i = 0; i < rest.size(); i++)
    {
        vector<Stat> iS;
        iS.push_back(MOV);
        vector<int> iA;
        iA.push_back(-1 * (rest[i]->getMov() / 2));
        Debuff *chains = new Debuff("Chains of Enkidu",
                                    "You have been slowed by the Chains of Enkidu!",
                                    rest[i]->getTeam(), iS, iA, 3);
        rest[i]->addDebuff(chains);
    }

    // Instate the cooldown debuff
    vector<Stat> iS;
    iS.push_back(MOV);
    vector<int> iA;
    iA.push_back(0);
    Debuff *chains = new Debuff("Chains of Enkidu Cooldown",
                                "--", getTeam(), iS, iA, 4);
    addDebuff(chains);

    return 0;
}

// Gate of Babylon
int BossGil::activateNP2(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][2] > currMP)
        return 1; // Not enough MP to attack

    if (gateCooldown())
    {
        log->addToErrorLog("Gate of Babylon is still on Cooldown!");
        return 41;
    }

    subMP(actionMPCosts[ascension][2]);

    log->addToEventLog(getFullName() + " used Gate of Babylon!");

    // Remove all allies from defenders
    for (int i = 0; i < (int) defenders.size(); i++)
    {
        if (defenders[i]->getTeam() == getTeam())
        {
            defenders.erase(defenders.begin() + i);
            i--;
        }
    }

    // 100 times in a row, randomly pick a target and do damage
    bool allDead = false;
    for (int i = 0; i < 100 && !allDead && defenders.size() > 0; i++)
    {
        // Randomly pick a target
        Servant *target = defenders[0];
        int tInd = 0;
        do
        {
            tInd = rand() % defenders.size();
            target = defenders[tInd];
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
            double attackMult = 0.1;
            int critChance = capZero(getCriticalRate() -
                             target->getCriticalEvade());
            r = getRandNum();
            if (critChance >= r)
                attackMult *= 3;

            // Deal the damage
            dam = capOne((getStr() - target->getDef()) * attackMult);
            log->addToEventLog("Gate of Babylon dealt " +
                               to_string(dam) + " damage to " +
                               target->getFullName() + ".");
            target->subHP(dam, NP_STR);
        }
        else
        {
            log->addToEventLog("Gate of Babylon missed " +
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
        for (unsigned int j = 0; j < defenders.size() && allDead; j++)
        {
            if (defenders[j]->getCurrHP() > 0)
                allDead = false;
        }
    }

    // Instate Cooldown Debuff
    vector<Stat> iS;
    iS.push_back(MOV);
    vector<int> iA;
    iA.push_back(0);
    Debuff *chains = new Debuff("Gate of Babylon Cooldown",
                                "--", getTeam(), iS, iA, 3);
    addDebuff(chains);

    return 0;
}

// Ea
int BossGil::activateNP3(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][3] > currMP)
        return 1; // Not enough MP to attack

    if (eaCooldown())
    {
        log->addToErrorLog("Ea is still on Cooldown!");
        return 41;
    }

    subMP(actionMPCosts[ascension][3]);

    log->addToEventLog(getFullName() + " used Ea!!");

    // If a reality marble is active, cancel it and take all MP from the caster
    if (field->isRealityMarbleOn())
    {
        Servant* target = field->realityMarbleServant();
        field->endRealityMarble();

        int allMP = target->getCurrMP();
        target->setMP(0);
        addMP(allMP);
        log->addToEventLog(getFullName() + "'s Ea destroyed " +
                           target->getFullName() +
                           "'s reality marble, sapping " + to_string(allMP) +
                           " MP from them in the process!");
    }

    // If a reality marble is not active, deal 100 undodgeable damage to all
    // enemy units
    else
    {
        // Remove allied units from defenders
        for (int i = 0; i < (int) defenders.size(); i++)
        {
            if (defenders[i]->getTeam() == getTeam())
            {
                defenders.erase(defenders.begin() + i);
                i--;
            }
        }

        // Deal the 100 HP and 50 MP damage
        for (unsigned int i = 0; i < defenders.size(); i++)
        {
            defenders[i]->subHP(100, OMNI);
            defenders[i]->subMP(50);

            log->addToEventLog(getFullName() + "'s Ea attacked " +
                               defenders[i]->getFullName() +
                               " for 100 damage and destroyed 50 MP!");

            // Check to see if the defender is dead.
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


    // Instate Cooldown Debuff
    vector<Stat> iS;
    iS.push_back(MOV);
    vector<int> iA;
    iA.push_back(0);
    Debuff *chains = new Debuff("Ea Cooldown",
                                "--", getTeam(), iS, iA, 6);
    addDebuff(chains);

    return 0;
}
