#include "servantarcherprojectiles.h"
#include "PlayField.h"

ServantArcherProjectiles::ServantArcherProjectiles(int as, Team t, Logger *l) : ServantArcher(as, t, l)
{
    name = "Projectile Archer";

    /** Stat modifiers **/
    /*int fakMod = -3;
    int magMod = 15;
    int resMod = 10;

    mag[0] += magMod;
    mag[1] += magMod;
    mag[2] += magMod;

    res[0] += resMod;
    res[1] += resMod;
    res[2] += resMod;

    skl[0] += fakMod;
    skl[1] += fakMod;
    skl[2] += fakMod;
    str[0] += fakMod;
    str[1] += fakMod;
    str[2] += fakMod;
    def[0] += fakMod;
    def[1] += fakMod;
    def[2] += fakMod;
    spd[0] += fakMod;
    spd[1] += fakMod;
    spd[2] += fakMod;
    luk[0] += fakMod;
    luk[1] += fakMod;
    luk[2] += fakMod;*/

    actionList[0].push_back("2: Counterfeit Life (MP to HP)");
    actionList[0].push_back("3: Counterfeit Life (HP to MP)");
    actionList[0].push_back("4: NP: Ultralimited Blade Works");
    actionList[1].push_back("2: Counterfeit Life (MP to HP)");
    actionList[1].push_back("3: Counterfeit Life (HP to MP)");
    actionList[1].push_back("4: NP: Ultralimited Blade Works");
    actionList[1].push_back("5: NP: Limited Blade Works");
    actionList[2].push_back("2: Counterfeit Life (MP to HP)");
    actionList[2].push_back("3: Counterfeit Life (HP to MP)");
    actionList[2].push_back("4: NP: Ultralimited Blade Works");
    actionList[2].push_back("5: NP: Limited Blade Works");
    actionList[2].push_back("6: NP: Unlimited Blade Works");

    actionListTypes[0].push_back(N);
    actionListTypes[0].push_back(N);
    actionListTypes[0].push_back(S);
    actionListTypes[1].push_back(N);
    actionListTypes[1].push_back(N);
    actionListTypes[1].push_back(S);
    actionListTypes[1].push_back(A);
    actionListTypes[2].push_back(N);
    actionListTypes[2].push_back(N);
    actionListTypes[2].push_back(S);
    actionListTypes[2].push_back(A);
    actionListTypes[2].push_back(T);

    actionMPCosts[0].push_back(30);
    actionMPCosts[0].push_back(0);
    actionMPCosts[0].push_back(30);
    actionMPCosts[1].push_back(30);
    actionMPCosts[1].push_back(0);
    actionMPCosts[1].push_back(30);
    actionMPCosts[1].push_back(75);
    actionMPCosts[2].push_back(30);
    actionMPCosts[2].push_back(0);
    actionMPCosts[2].push_back(30);
    actionMPCosts[2].push_back(75);
    actionMPCosts[2].push_back(170);

    actionDodgeable[0].push_back(false);
    actionDodgeable[0].push_back(false);
    actionDodgeable[0].push_back(true);
    actionDodgeable[1].push_back(false);
    actionDodgeable[1].push_back(false);
    actionDodgeable[1].push_back(true);
    actionDodgeable[1].push_back(true);
    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(true);
    actionDodgeable[2].push_back(true);
    actionDodgeable[2].push_back(true);

    actionCounterable[0].push_back(false);
    actionCounterable[0].push_back(false);
    actionCounterable[0].push_back(false);
    actionCounterable[1].push_back(false);
    actionCounterable[1].push_back(false);
    actionCounterable[1].push_back(false);
    actionCounterable[1].push_back(false);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);

    vector<string> np1;
    np1.push_back("Ultralimited Blade Works");
    np1.push_back("At a cost of 30 MP, attack one opponent 5 times in a row. Each individual attack has its own accuracy and critical check. Enemy cannot counterattack but enemy skills can activate.");
    vector<string> np2;
    np2.push_back("Limited Blade Works");
    np2.push_back("At a cost of 75 MP, attack all opponents within 3 spaces 5 times in a row each. Each individual attack has its own accuracy and critical check. Enemies cannot counterattack and enemy skills cannot activate.");
    vector<string> np3;
    np3.push_back("Unlimited Blade Works");
    np3.push_back("At a cost of 170 MP, create a Reality Marble around the entire playing field. On the turn of activation, attack four random opponents 8 times in a row each. Each individual attack has its own accuracy and critical check. Enemies cannot counterattack and enemy skills cannot activate. Can maintain the Reality Marble at a cost of 10 MP per turn; each turn that the Marble is maintained, all enemies are attacked once for (STR + MAG) damage. This attack always hits and can kill. The higher of the Enemy’s DEF or RES is used to determine how much damage they take. Enemies cannot counterattack and enemy skills cannot activate. UBW cannot be cast if another Reality Marble is active.");
    noblePhantasms.push_back(np1);
    noblePhantasms.push_back(np2);
    noblePhantasms.push_back(np3);

    // Ultralimited Blade Works range
    npRanges.push_back(getLowToHighRange(getLowRange(), getHighRange()));

    // Limited Blade Works range
    npRanges.push_back(getLowToHighRange(getLowRange(), 3));

    // UBW is a Reality Marble, so doesn't really have a range...
    Coordinate c;
    c.x = 0; c.y = 0;
    vector<Coordinate> npc3;
    npc3.push_back(c);
    npRanges.push_back(npc3);

    /** Passive Skill modifiers **/
    int mainBonus = -3;
    vector<Stat> hdS;
    hdS.push_back(STR);
    hdS.push_back(DEF);
    hdS.push_back(MAG);
    hdS.push_back(RES);
    hdS.push_back(SPD);
    hdS.push_back(SKL);
    hdS.push_back(LUK);
    vector<int> hdA;
    hdA.push_back(mainBonus);
    hdA.push_back(mainBonus);
    hdA.push_back(30);
    hdA.push_back(10);
    hdA.push_back(mainBonus);
    hdA.push_back(mainBonus);
    hdA.push_back(mainBonus);
    Debuff *highDivinity = new Debuff("That's No Bow!", "Passive Skill",
                                      t, hdS, hdA, -1);
    addDebuff(highDivinity);
}

/***** Active Skills *****/
int ServantArcherProjectiles::counterfeitLifeHPGain()
{
    if (actionMPCosts[ascension][1] > currMP)
        return 1; // Not enough MP
    else
    {
        subMP(actionMPCosts[ascension][1]);
        addHP(50);

        log->addToEventLog(getFullName() + " has used Counterfeit Life (HP to MP)!");
    }

    return 0;
}

int ServantArcherProjectiles::counterfeitLifeMPGain()
{
    if (getCurrHP() <= 25)
        return 1; // Not enough HP
    else
    {
        subHP(25, OMNI);
        addMP(15);

        log->addToEventLog(getFullName() + " has used Counterfeit Life (MP to HP)!");
    }

    return 0;
}


/***** Re-defined superclass functions *****/
int ServantArcherProjectiles::doAction(int actionNum, vector<Servant *> defenders)
{
    int ret = 0;
    switch (actionNum)
    {
        case 0:
            ret = attack(defenders, true);
            break;
        case 1:
            ret = counterfeitLifeHPGain();
            break;
        case 2:
            ret = counterfeitLifeMPGain();
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

int ServantArcherProjectiles::isActionNP(int action)
{
    int ret = -1;

    if (action >= 3)
        ret = action - 3;

    return ret;
}

bool ServantArcherProjectiles::isActionRealityMarble(int action)
{
    return (action == 5);
}

int ServantArcherProjectiles::getRealityMarbleMP()
{
    return 10;
}

vector<Coordinate> ServantArcherProjectiles::getActionRange(int action)
{
    vector<Coordinate> range;

    if (action == 0)
    {
        range = getLowToHighRange(getLowRange(), getHighRange());
    }
    else if (action == 1 || action == 2)
    {
        Coordinate tc;
        tc.x = 0; tc.y = 0;
        range.push_back(tc);
    }
    else
    {
        range = getNPRange(action - 3);
    }

    return range;
}

/***** Noble Phantasms *****/
// Ultralimited Blade Works
int ServantArcherProjectiles::activateNP1(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][3] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][3]);
        for (unsigned int i = 0; i < defenders.size(); i++)
        {
            // Attack the target 5 times in a row
            bool enemyDead = false;
            for (int j = 0; j < 5 && !enemyDead; j++)
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
                    defenders[i]->subHP(dam, NP_MAG);
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
    }

    return 0;
}

// Limited Blade Works
int ServantArcherProjectiles::activateNP2(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][4] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][4]);
        for (unsigned int i = 0; i < defenders.size(); i++)
        {
            if (defenders[i]->getTeam() == getTeam())
                continue;

            // Attack the target 5 times in a row
            bool enemyDead = false;
            for (int j = 0; j < 5 && !enemyDead; j++)
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

                // Check to see if the defender is dead.
                if(defenders[i]->getCurrHP() <= 0)
                {
                    enemyDead = true;
                }
            }
        }
    }

    return 0;
}

// Unlimited Blade Works
int ServantArcherProjectiles::activateNP3(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][5] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][5]);

        // Remove all teammates from defenders
        for (int i = 0; i < (int) defenders.size(); i++)
        {
            if (defenders[i]->getTeam() == getTeam())
            {
                defenders.erase(defenders.begin() + i);
                i--;
            }
        }

        // Randomly remove servants from the defenders vector until there are
        //  only four left
        while (defenders.size() > 4)
        {
            int r = rand() % ((int) defenders.size());
            defenders.erase(defenders.begin() + r);
        }

        for (unsigned int i = 0; i < defenders.size(); i++)
        {
            // Attack the target 8 times in a row
            bool enemyDead = false;
            for (int j = 0; j < 8 && !enemyDead; j++)
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

                // Check to see if the defender is dead.
                if(defenders[i]->getCurrHP() <= 0)
                {
                    enemyDead = true;
                }
            }
        }
    }

    // Do the actual reality marble stuff
    realityMarbleActive = true;

    // UBW "Debuff":
    vector<Stat> tstats;
    tstats.push_back(MOV);
    vector<int> tAmounts;
    tAmounts.push_back(0);
    Debuff *ubw = new Debuff("Reality Marble", "Unlimited Blade Works",
                             team, tstats, tAmounts, -1);

    field->startRealityMarble(this, ubw);

    log->addToEventLog(getFullName() + " activated a Reality Marble! Unlimited Blade Works is now in effect!");

    return 0;
}
