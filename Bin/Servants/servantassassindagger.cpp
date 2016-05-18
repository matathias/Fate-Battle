#include "servantassassindagger.h"
#include "PlayField.h"

ServantAssassinDagger::ServantAssassinDagger(int as, Team t, Logger *l) : ServantAssassin(as, t, l)
{
    name = "Dagger Assassin";

    /** Stat modifiers **/
    int strMod = 15;

    str[0] += strMod;
    str[1] += strMod;
    str[2] += strMod;

    actionList[0].push_back("4: NP: Backslash!");
    actionList[1].push_back("4: NP: Backslash!");
    actionList[1].push_back("5: NP: Reach of Darkness");
    actionList[2].push_back("4: NP: Backslash!");
    actionList[2].push_back("5: NP: Reach of Darkness");
    actionList[2].push_back("6: NP: Daggerworks");

    actionListTypes[0].push_back(S);
    actionListTypes[1].push_back(S);
    actionListTypes[1].push_back(S);
    actionListTypes[2].push_back(S);
    actionListTypes[2].push_back(S);
    actionListTypes[2].push_back(A);

    actionMPCosts[0].push_back(8);
    actionMPCosts[1].push_back(8);
    actionMPCosts[1].push_back(40);
    actionMPCosts[2].push_back(8);
    actionMPCosts[2].push_back(40);
    actionMPCosts[2].push_back(100);

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
    np1.push_back("Backslash!");
    np1.push_back("Spend 8 MP to lunge forward a space and stab an enemy in the back. Deals twice normal damage, but can be dodged. Cannot crit.");
    vector<string> np2;
    np2.push_back("Reach of Darkness");
    np2.push_back("Spend 40 MP to lunge forward and drag an enemy into the darkness, dealing undodgeable thrice normal damage and leaving them with a \"Terrified\" debuff that halves their SKL and SPD for three turns. Always hits but cannot crit.");
    vector<string> np3;
    np3.push_back("Daggerworks");
    np3.push_back("Spend 100 MP to create 30 daggers and utterly eviscerate all opponents within two spaces. Each dagger targets a random opponent and does half normal damage. Armor Scratcher applies to all hits and stacks. Any opponent hit three times or more will receive a \"Terrified\" debuff. Always hits and can crit.");
    noblePhantasms.push_back(np1);
    noblePhantasms.push_back(np2);
    noblePhantasms.push_back(np3);

    // Backslash! range
    npRanges.push_back(getLowToHighRange(1,2));
    // Reach of Darkness range
    npRanges.push_back(getLowToHighRange(1,2));
    // Daggerworks range
    npRanges.push_back(getLowToHighRange(1,2));

    vector<Stat> tempS;
    tempS.push_back(DEF);
    vector<int> tempA;
    tempA.push_back(-5);
    classDebuff = new Debuff("Armor Scratcher",
                             "You took a hit from a Dagger Assassin, reducing the effectiveness of your armor.",
                             team, tempS, tempA, 3);
}

/***** Noble Phantasms *****/
// Backslash!
int ServantAssassinDagger::activateNP1(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][3] > currMP)
        return 1; // Not enough MP to attack

    // Get the landing location
    //  If there is no valid landing location next to the target servant,
    //  stop processing and return 41.
    Coordinate landingCoord = getEndLocation(defenders[0]->getCurrLoc(), 1);
    if(landingCoord.x == -1 && landingCoord.y == -1)
    {
        // No valid adjacent space
        log->addToErrorLog("No valid adjacent space to target.");
        return 41;
    }

    subMP(actionMPCosts[ascension][3]);

    // Move to be adjacent to the target servant
    log->addToEventLog(getFullName() + " used Backslash!");
    log->addToEventLog(getFullName() + " lunged to space (" +
                       to_string(landingCoord.x) + "," +
                       to_string(landingCoord.y) + ") from (" +
                       to_string(currLoc.x) + "," + to_string(currLoc.y) +
                       ") and attacked " + defenders[0]->getFullName() +
                       ".");
    field->moveServant(this, landingCoord);

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
        dam = capZero(getStr() - defenders[0]->getDef()) * 2;
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

// Reach of Darkness
int ServantAssassinDagger::activateNP2(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][4] > currMP)
        return 1; // Not enough MP to attack

    // Get the landing location
    //  If there is no valid landing location next to the target servant,
    //  stop processing and return 41.
    Coordinate landingCoord = getEndLocation(defenders[0]->getCurrLoc(), 1);
    if(landingCoord.x == -1 && landingCoord.y == -1)
    {
        // No valid adjacent space
        log->addToErrorLog("No valid adjacent space to target.");
        return 41;
    }

    subMP(actionMPCosts[ascension][4]);

    // Move to be adjacent to the target servant
    log->addToEventLog(getFullName() + " used Reach of Darkness!");
    log->addToEventLog(getFullName() + " lunged to space (" +
                       to_string(landingCoord.x) + "," +
                       to_string(landingCoord.y) + ") from (" +
                       to_string(currLoc.x) + "," + to_string(currLoc.y) +
                       ") and attacked " + defenders[0]->getFullName() +
                       ".");
    field->moveServant(this, landingCoord);

    // Always hits
    int dam = capZero(getStr() - defenders[0]->getDef()) * 3;
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

    // Add the Terrified debuff to the target
    vector<Stat> tStats;
    tStats.push_back(SPD);
    tStats.push_back(SKL);
    vector<int> tAmounts;
    tAmounts.push_back(-1 * (defenders[0]->getSpd() / 2));
    tAmounts.push_back(-1 * (defenders[0]->getSkl() / 2));
    Debuff *terrified = new Debuff("Terrified",
                                   "You have been terrified by an enemy Assassin.",
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

// Daggerworks
int ServantAssassinDagger::activateNP3(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][5] > currMP)
        return 1; // Not enough MP to attack

    subMP(actionMPCosts[ascension][5]);

    // Move to be adjacent to the target servant
    log->addToEventLog(getFullName() + " used Daggerworks!");

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
    for (int i = 0; i < 30 && !allDead; i++)
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
        int attackMult = 0.5;
        int critChance = capZero(getCriticalRate() -
                                 target->getCriticalEvade());
        int r = getRandNum();
        if (critChance >= r)
            attackMult *= 3;

        int dam = capZero(getStr() - target->getDef()) * attackMult;
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
            Debuff *terrified = new Debuff("Terrified",
                                           "You have been terrified by an enemy Assassin.",
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
