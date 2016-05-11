#include "servantavenger.h"
#include "PlayField.h"

ServantAvenger::ServantAvenger(int as, Team t, Logger *l) : Servant(as, t, l)
{
    maxHP.push_back(100);
    maxHP.push_back(150);
    maxHP.push_back(200);
    maxMP.push_back(75);
    maxMP.push_back(100);
    maxMP.push_back(125);
    mov.push_back(5);
    mov.push_back(5);
    mov.push_back(6);
    str.push_back(20);
    str.push_back(20);
    str.push_back(20);
    mag.push_back(20);
    mag.push_back(20);
    mag.push_back(20);
    def.push_back(20);
    def.push_back(30);
    def.push_back(40);
    res.push_back(20);
    res.push_back(20);
    res.push_back(20);
    spd.push_back(20);
    spd.push_back(20);
    spd.push_back(20);
    skl.push_back(20);
    skl.push_back(30);
    skl.push_back(40);
    luk.push_back(20);
    luk.push_back(30);
    luk.push_back(40);

    currHP = maxHP[as];
    currMP = maxMP[as];

    lowRange = 1;
    hiRange = 1;

    clss = Avenger;
    name = "Avenger";

    vector<string> basicActions;
    basicActions.push_back("1: Attack");
    basicActions.push_back("2: Carry On The Torch");
    actionList.push_back(basicActions);
    actionList.push_back(basicActions);
    actionList.push_back(basicActions);
    vector<ActionType> actionTypes;
    actionTypes.push_back(S);
    actionTypes.push_back(D);
    actionListTypes.push_back(actionTypes);
    actionListTypes.push_back(actionTypes);
    actionListTypes.push_back(actionTypes);
    vector<int> actionCosts;
    actionCosts.push_back(2);
    actionCosts.push_back(50);
    actionMPCosts.push_back(actionCosts);
    actionMPCosts.push_back(actionCosts);
    actionMPCosts.push_back(actionCosts);

    vector<bool> acDodge;
    acDodge.push_back(true);
    acDodge.push_back(false);
    actionDodgeable.push_back(acDodge);
    actionDodgeable.push_back(acDodge);
    actionDodgeable.push_back(acDodge);
    vector<bool> acCounter;
    acCounter.push_back(true);
    acCounter.push_back(false);
    actionCounterable.push_back(acCounter);
    actionCounterable.push_back(acCounter);
    actionCounterable.push_back(acCounter);

    carriedTheTorch = false;
}

void ServantAvenger::setTeam(vector<Servant *> ts)
{
    teamServants = ts;
}

/***** Active Skills *****/
int ServantAvenger::carryingOnTheTorch(vector<Servant *> defenders)
{
    if (carriedTheTorch)
    {
        // This action was already taken! Can't do it again!
        return 41;
    }

    // We only care about the first servant (since there should only be one)
    Servant *defender = defenders[0];
    carriedTheTorch = true;

    // Set the Avenger's stats to the defender stats
    maxHP[ascension] = defender->getMaxHP();
    maxMP[ascension] = defender->getMaxMP();
    mov[ascension] = defender->getMov();
    str[ascension] = defender->getStr();
    mag[ascension] = defender->getMag();
    def[ascension] = defender->getDef();
    res[ascension] = defender->getRes();
    spd[ascension] = defender->getSpd();
    skl[ascension] = defender->getSkl();
    luk[ascension] = defender->getLuk();

    // Add a Permadeath debuff to the defender servant
    vector<Stat> tss;
    tss.push_back(MOV);
    vector<int> tas;
    tas.push_back(0);
    Debuff* permaD = new Debuff("Permadeath",
                                "You are permanently dead.",
                                defender->getTeam(), tss, tas, -1);

    defender->addDebuff(permaD);

    return 0;
}

/***** Passive Skills *****/
Debuff* ServantAvenger::finalRevenge()
{
    // Magnitude of debuff
    int statBonus = -10;

    vector<Stat> tStats;
    //tStats.push_back(MAXHP);
    //tStats.push_back(MAXMP);
    tStats.push_back(MOV);
    tStats.push_back(STR);
    tStats.push_back(MAG);
    tStats.push_back(DEF);
    tStats.push_back(RES);
    tStats.push_back(SPD);
    tStats.push_back(SKL);
    tStats.push_back(LUK);
    vector<int> tAmounts;
    //tAmounts.push_back(statBonus);
    //tAmounts.push_back(statBonus);
    tAmounts.push_back(statBonus);
    tAmounts.push_back(statBonus);
    tAmounts.push_back(statBonus);
    tAmounts.push_back(statBonus);
    tAmounts.push_back(statBonus);
    tAmounts.push_back(statBonus);
    tAmounts.push_back(statBonus);
    tAmounts.push_back(statBonus);
    Debuff* avengersRage = new Debuff("Final Revenge",
                                      "The Avenger you killed still haunts you...",
                                      team, tStats, tAmounts, 3);

    return avengersRage;
}

void ServantAvenger::updateAvengersRage()
{
    // Only update so long as the Avenger has not Carried the Torch
    if (!carriedTheTorch)
    {
        // Remove the previous buff if it exists
        for (int i = 0; i < (int) debuffs.size(); i++)
        {
            if (debuffs[i]->getDebuffName().compare("Avenger's Rage") == 0)
            {
                debuffs.erase(debuffs.begin() + i);
                i--;
            }
        }

        //*** Get the total HP loss for the team (in percentage) ***
        int totalHPLoss = 0;
        for (unsigned int i = 0; i < teamServants.size(); i++)
        {
            int loss = teamServants[i]->getMaxHP() - teamServants[i]->getCurrHP();
            double lossProp = ((double) loss) / ((double) teamServants[i]->getMaxHP());
            totalHPLoss += (int) (lossProp * 100);
        }

        // Get the average HP Loss
        totalHPLoss = totalHPLoss / teamServants.size();

        // Get the stat bonus from this loss
        int statBonus = totalHPLoss / 5;

        // Make a new debuff with statBonus
        vector<Stat> tStats;
        tStats.push_back(MAXHP);
        tStats.push_back(MAXMP);
        tStats.push_back(MOV);
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
        tAmounts.push_back(statBonus);
        tAmounts.push_back(statBonus);
        tAmounts.push_back(statBonus);
        Debuff* avengersRage = new Debuff("Avenger's Rage",
                                          "Your teammates' loss becomes your strength!",
                                          team, tStats, tAmounts, -1);

        // Apply the debuff to the avenger
        addDebuff(avengersRage);
    }
}

/***** Servant Function Re-definitions *****/
// Re-define attack to account for the Vengeance skill
int ServantAvenger::attack(vector<Servant *> defenders, bool counter)
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
            // Also check if Skill: Vengeance activates
            if (hit)
            {
                int attackMult = 1;
                int critChance = capZero(getCriticalRate() -
                                 defenders[i]->getCriticalEvade());
                r = getRandNum();
                if (critChance >= r)
                    attackMult *= 3;

                // Check for Vengeance
                int addVengeance = 0;
                r = getRandNum();
                if (getSkl() * 2 >= r)
                {
                    addVengeance = (getMaxHP() - getCurrHP()) * 2;
                    log->addToEventLog(getFullName() + " activated Vengeance!");
                }

                // Deal the damage
                dam = (capZero(getStr() - defenders[i]->getDef()) + addVengeance)
                        * attackMult;
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
