#include "servantberserkerclub.h"

ServantBerserkerClub::ServantBerserkerClub(int as, Team t, Logger *l) : ServantBerserker(as, t, l)
{
    name = "Club Berserker";

    vector<string> np1;
    np1.push_back("Barbarian's Might");
    np1.push_back("Passive Noble Phantasm. Every time you attack, there is a (LUK*2) chance of you reducing your target’s DEF by 5. Stacks with Crushing Blow.");
    vector<string> np2;
    np2.push_back("Barbarian's Rage");
    np2.push_back("Passive Noble Phantasm. Every time you take damage, your STR increases by 5. Every turn you do not take damage your STR decreases by 10. Will not decrease below your base STR.");
    vector<string> np3;
    np3.push_back("Barbarian's Wrath");
    np3.push_back("Passive Noble Phantasm. If facing someone who you have already attacked, deal thrice as much damage and decrease their DEF by 10. Stacks with Barbarian’s Might and Crushing Blow.");
    noblePhantasms.push_back(np1);
    noblePhantasms.push_back(np2);
    noblePhantasms.push_back(np3);

    Coordinate c1;
    c1.x = 0; c1.y = 0;
    vector<Coordinate> npc1;
    npc1.push_back(c1);
    vector<Coordinate> npc2;
    npc2.push_back(c1);
    vector<Coordinate> npc3;
    npc3.push_back(c1);
    npRanges.push_back(npc1);
    npRanges.push_back(npc2);
    npRanges.push_back(npc3);

    damageFreeTurn = true;
    strIncreases = 0;
    strIncrement = 5;
}

/***** Function Re-definitions *****/
void ServantBerserkerClub::turnUpdate()
{
    if (damageFreeTurn)
    {
        strIncreases -= 2;
    }

    damageFreeTurn = true;

    if (strIncreases < 0)
        strIncreases = 0;
    else if (strIncreases > 0)
    {
        int totalStrIncrease = strIncreases * strIncrement;

        vector<Stat> tS;
        tS.push_back(STR);
        vector<int> tA;
        tA.push_back(totalStrIncrease);
        Debuff* br = new Debuff("Barbarian's Rage",
                                "Taking damage fuels your rage, greatly increasing your strength.",
                                getTeam(), tS, tA, 1);
        addDebuff(br);
    }
}

int ServantBerserkerClub::doAction(int actionNum, vector<Servant *> defenders)
{
    if (actionNum == 0)
        return attack(defenders, true);
    else
        return 2;
}

int ServantBerserkerClub::attack(vector<Servant *> defenders, bool counter)
{
    if (actionMPCosts[ascension][0] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][0]);
        for (unsigned int i = 0; i < defenders.size(); i++)
        {
            // Add the target to the list of previous targets (if they are not
            //  already on it)
            if (!previouslyTargeted(defenders[i]))
                previousTargets.push_back(defenders[i]);

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

                // If you're at the Final Ascension and have already faced this
                //  target, deal thrice as much damage and decrease DEF by 10
                //  (Barbarian's Wrath)
                if(ascension == 2 && previouslyTargeted(defenders[i]))
                {
                    attackMult *= 3;
                    vector<Stat> tS;
                    tS.push_back(DEF);
                    vector<int> tA;
                    tA.push_back(-10);
                    Debuff* bw = new Debuff("Barbarian's Wrath",
                                            "You tried to fight a Club Berserker twice, and paid the price.",
                                            defenders[i]->getTeam(), tS, tA,
                                            -1);
                    defenders[i]->addDebuff(bw);
                    log->addToEventLog(defenders[i]->getFullName() + " felt " +
                                       getFullName() + "'s Wrath!");
                }

                // Deal the damage
                dam = capZero(getStr() - defenders[i]->getDef()) * attackMult;
                log->addToEventLog(getFullName() + " dealt " +
                                   to_string(dam) + " damage to " +
                                   defenders[i]->getFullName() + ".");
                defenders[i]->subHP(dam, D_STR);

                // Apply the Crushing Blow debuff
                vector<Stat> tStats;
                tStats.push_back(DEF);
                vector<int> tAmounts;
                tAmounts.push_back(-2);
                Debuff* bOut = new Debuff("Crushing Blow",
                                          "You took a blow from a Club Berserker, permanetly denting your armor.",
                                          defenders[i]->getTeam(), tStats,
                                          tAmounts, -1);

                defenders[i]->addDebuff(bOut);

                // Check to see if Barbarian's Might activates
                r = getRandNum();
                if (r <= getLuk() * 2)
                {
                    vector<Stat> tS;
                    tS.push_back(DEF);
                    vector<int> tA;
                    tA.push_back(-5);
                    Debuff* bm = new Debuff("Barbarian's Might",
                                            "You took an incredible blow from a Club Berserker, permanetly denting your armor.",
                                            defenders[i]->getTeam(), tS, tA,
                                            -1);
                    defenders[i]->addDebuff(bm);
                    log->addToEventLog(defenders[i]->getFullName() + " felt " +
                                       getFullName() + "'s Might!");
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

void ServantBerserkerClub::subHP(int hp, DamageType dt)
{
    damageFreeTurn = false;
    strIncreases++;

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

/***** Protected Functions *****/
bool ServantBerserkerClub::previouslyTargeted(Servant *target)
{
    bool found = false;
    for (unsigned int i = 0; i < previousTargets.size() && !found; i++)
    {
        if (previousTargets[i] == target)
        {
            found = true;
        }
    }

    return found;
}
