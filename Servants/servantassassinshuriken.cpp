#include "servantassassinshuriken.h"
#include "PlayField.h"

ServantAssassinShuriken::ServantAssassinShuriken(int as, Team t, Logger *l) : ServantAssassin(as, t, l)
{
    name = "Shuriken Assassin";

    /** Stat modifiers **/
    int sklMod = 10;
    int spdMod = 10;

    spd[0] += spdMod;
    spd[1] += spdMod;
    spd[2] += spdMod;
    skl[0] += sklMod;
    skl[1] += sklMod;
    skl[2] += sklMod;

    actionList[0].push_back("4: NP: Shurikenfaire");
    actionList[1].push_back("4: NP: Shurikenfaire");
    actionList[1].push_back("5: NP: Death from the Shadows");
    actionList[2].push_back("4: NP: Shurikenfaire");
    actionList[2].push_back("5: NP: Death from the Shadows");
    actionList[2].push_back("6: NP: Shurikenworks");

    actionListTypes[0].push_back(S);
    actionListTypes[1].push_back(S);
    actionListTypes[1].push_back(S);
    actionListTypes[2].push_back(S);
    actionListTypes[2].push_back(S);
    actionListTypes[2].push_back(A);

    actionMPCosts[0].push_back(10);
    actionMPCosts[1].push_back(10);
    actionMPCosts[1].push_back(40);
    actionMPCosts[2].push_back(10);
    actionMPCosts[2].push_back(40);
    actionMPCosts[2].push_back(110);

    actionDodgeable[0].push_back(true);
    actionDodgeable[1].push_back(true);
    actionDodgeable[1].push_back(false);
    actionDodgeable[2].push_back(true);
    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(false);

    actionCounterable[0].push_back(false);
    actionCounterable[1].push_back(false);
    actionCounterable[1].push_back(false);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);

    vector<string> np1;
    np1.push_back("Shurikenfaire");
    np1.push_back("Spend 10 MP to attack at range +2 for unresisted normal damage. Subject to accuracy and critical checks.");
    vector<string> np2;
    np2.push_back("Death from the Shadows");
    np2.push_back("Spend 40 MP to launch a shuriken from the shadows, dealing undodgeable, unresisted twice normal damage and startling your opponent, leaving them with a “Disoriented” debuff that halves their SKL and SPD for three turns. Always hits but cannot crit.");
    vector<string> np3;
    np3.push_back("Shurikenworks");
    np3.push_back("Spend 110 MP to create 45 shuriken and utterly eviscerate all opponents within three spaces. Each shuriken targets a random opponent and does unresisted quarter normal damage. Caltrops applies to all hits and stacks. Any opponent hit four times will receive a “Disoriented” debuff. Always hits and can crit.");
    noblePhantasms.push_back(np1);
    noblePhantasms.push_back(np2);
    noblePhantasms.push_back(np3);

    // Shurikenfaire range
    npRanges.push_back(getLowToHighRange(1, getHighRange() + 2));
    // Death from the Shadows range
    npRanges.push_back(getLowToHighRange(1,2));
    // Shurikenworks range
    npRanges.push_back(getLowToHighRange(1,3));

    vector<Stat> tempS;
    tempS.push_back(SPD);
    vector<int> tempA;
    tempA.push_back(-5);
    classDebuff = new Debuff("Caltrops",
                             "You fell for the trap set by a Shuriken Assassin, reducing your speed.",
                             team, tempS, tempA, 3);
}

/***** Noble Phantasms *****/
// Shurikenfaire
int ServantAssassinShuriken::activateNP1(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][3] > currMP)
        return 1; // Not enough MP to attack

    subMP(actionMPCosts[ascension][3]);

    log->addToEventLog(getFullName() + " used Shurikenfaire!");

    // Check if you hit the target
    vector<int> opEvade = defenders[0]->getEvade();
    bool hit = false;
    // Calculate accuracy
    int accuracy = capZero(getHitRate() - opEvade[0]);
    int dam = 0;

    int r = getRandNum();
    if (accuracy >= r)
        hit = true;

    // If you hit, do damage
    if (hit)
    {
        // Calculate crit chance
        int attackMult = 1;
        int critChance = capZero(getCriticalRate() -
                         defenders[0]->getCriticalEvade());
        int r = getRandNum();
        if (critChance >= r)
            attackMult *= 3;

        dam = getStr() * attackMult;
        log->addToEventLog(getFullName() + " dealt " +
                           to_string(dam) + " damage to " +
                           defenders[0]->getFullName() + ".");
        defenders[0]->subHP(dam, NP_STR);

        // Add the weapon-specific debuff to the target
        Debuff *deb = new Debuff(classDebuff->getDebuffName(),
                                 classDebuff->getDebuffDescrip(),
                                 defenders[0]->getTeam(),
                                 classDebuff->getDebuffStats(),
                                 classDebuff->getDebuffAmounts(),
                                 classDebuff->getTurnsRemaining());

        defenders[0]->addDebuff(deb);
    }
    else
    {
        log->addToEventLog(getFullName() + " missed " +
                           defenders[0]->getFullName() + "!");
    }

    // Check to see if the defender is dead.
    if(defenders[0]->getCurrHP() <= 0)
    {
        if (defenders[0]->getClass() == Avenger)
        {
            // Activate Final Revenge
            Debuff *finRev = defenders[0]->finalRevenge();
            addDebuff(finRev);
            if (defenders[0]->getAscensionLvl() == 2)
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

    return 0;
}

// Death from the Shadows
int ServantAssassinShuriken::activateNP2(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][4] > currMP)
        return 1; // Not enough MP to attack

    subMP(actionMPCosts[ascension][4]);

    log->addToEventLog(getFullName() + " used Death from the Shadows!");

    // Always hits
    int dam = getStr() * 2;
    log->addToEventLog(getFullName() + " dealt " +
                       to_string(dam) + " damage to " +
                       defenders[0]->getFullName() + ".");
    defenders[0]->subHP(dam, NP_STR);

    // Add the weapon-specific debuff to the target
    Debuff *deb = new Debuff(classDebuff->getDebuffName(),
                             classDebuff->getDebuffDescrip(),
                             defenders[0]->getTeam(),
                             classDebuff->getDebuffStats(),
                             classDebuff->getDebuffAmounts(),
                             classDebuff->getTurnsRemaining());

    defenders[0]->addDebuff(deb);

    // Add the Disoriented debuff to the target
    vector<Stat> tStats;
    tStats.push_back(SPD);
    tStats.push_back(SKL);
    vector<int> tAmounts;
    tAmounts.push_back(-1 * (defenders[0]->getSpd() / 2));
    tAmounts.push_back(-1 * (defenders[0]->getSkl() / 2));
    Debuff *terrified = new Debuff("Disoriented",
                                   "You have been disoriented by an enemy Assassin.",
                                   defenders[0]->getTeam(), tStats, tAmounts,
                                   4);
    defenders[0]->addDebuff(terrified);

    // Check to see if the defender is dead.
    if(defenders[0]->getCurrHP() <= 0)
    {
        if (defenders[0]->getClass() == Avenger)
        {
            // Activate Final Revenge
            Debuff *finRev = defenders[0]->finalRevenge();
            addDebuff(finRev);
            if (defenders[0]->getAscensionLvl() == 2)
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

    return 0;
}

// Shurikenworks
int ServantAssassinShuriken::activateNP3(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][5] > currMP)
        return 1; // Not enough MP to attack

    subMP(actionMPCosts[ascension][5]);

    // Move to be adjacent to the target servant
    log->addToEventLog(getFullName() + " used Shurikenworks!");

    // Remove teammates from the defenders vector
    for (int i = 0; i < (int) defenders.size(); i++)
    {
        if (defenders[i]->getTeam() == getTeam())
        {
            defenders.erase(defenders.begin()+i);
            i--;
        }
    }

    // Create an int vector to track how many times each Servant gets hit
    vector<int> timesHit;
    for (unsigned int i = 0; i < defenders.size(); i++)
    {
        timesHit.push_back(0);
    }

    // 30 times in a row, randomly pick a target and do damage
    bool allDead = false;
    for (int i = 0; i < 45 && !allDead && defenders.size() > 0; i++)
    {
        // Randomly pick a target
        Servant *target = defenders[0];
        int tInd = 0;
        do
        {
            tInd = rand() % defenders.size();
            target = defenders[tInd];
        } while (target->getCurrHP() <= 0);

        // Always hits; track that we have hit this Servant
        timesHit[tInd]++;
        // Check to see if it crits
        int attackMult = 0.25;
        int critChance = capZero(getCriticalRate() -
                                 target->getCriticalEvade());
        int r = getRandNum();
        if (critChance >= r)
            attackMult *= 3;

        int dam = getStr() * attackMult;
        log->addToEventLog(getFullName() + " dealt " +
                           to_string(dam) + " damage to " +
                           target->getFullName() + ".");
        target->subHP(dam, NP_STR);

        // Add the weapon-specific debuff to the target
        Debuff *deb = new Debuff(classDebuff->getDebuffName(),
                                 classDebuff->getDebuffDescrip(),
                                 target->getTeam(),
                                 classDebuff->getDebuffStats(),
                                 classDebuff->getDebuffAmounts(),
                                 classDebuff->getTurnsRemaining());

        target->addDebuff(deb);

        // If the target has been hit three times, apply the Terrified debuff
        if (timesHit[tInd] == 3)
        {
            // Add the Terrified debuff to the target
            vector<Stat> tStats;
            tStats.push_back(SPD);
            tStats.push_back(SKL);
            vector<int> tAmounts;
            tAmounts.push_back(-1 * (target->getSpd() / 2));
            tAmounts.push_back(-1 * (target->getSkl() / 2));
            Debuff *terrified = new Debuff("Disoriented",
                                           "You have been disoriented by an enemy Assassin.",
                                           target->getTeam(), tStats,
                                           tAmounts, 4);
            target->addDebuff(terrified);
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
