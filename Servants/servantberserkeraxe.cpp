#include "servantberserkeraxe.h"

ServantBerserkerAxe::ServantBerserkerAxe(int as, Team t, Logger *l) : ServantBerserker(as, t, l)
{
    name = "Axe Berserker";

    actionList[2].push_back("2: Extend Godmind");

    actionListTypes[2].push_back(N);

    actionMPCosts[2].push_back(70);

    actionDodgeable[2].push_back(false);

    actionCounterable[2].push_back(true);

    vector<string> np1;
    np1.push_back("Godmind");
    np1.push_back("Each turn you suffer no damage, there is an increased chance of regaining your sanity for three turns. Regaining your sanity means that you gain the ability to move where you want, move when you want, and attack who you want. The chance of this ability triggering is (# of damage-free turns * LUK / 2). Suffer all stats -10 due to the lessening of Mad Enhancement, but gain +10 STR due to the righteous (yet sane) rage you hold for having lost your sanity in the first place.");
    vector<string> np2;
    np2.push_back("Godhand");
    np2.push_back("Auto-revives on death three times. Each time you are revived you have less 25% less MAXHP but 5% more DEF. However, if you die while Godmind is active you lose all your lives and only a Caster will be able to revive you. Godhand blocks revival-denying kill moves, such as those from the Necromancer, causing them to take only one life like all other deaths.");
    vector<string> np3;
    np3.push_back("Godking");
    np3.push_back("In addition to the effects from Godhand, heal .025*MAXHP damage each turn. Attacks that do not deal at least .1*MAXHP of damage do not injure you unless they are part of a Noble Phantasm. If Godmind is active, you can spend 70 MP to extend Godmind by 5 turns and make you completely immune to all non-Noble Phantasms until the end of those 5 turns. Additionally, the chance of Godmind activating is increased to (# of damage-free turns * LUK).");
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

    deathCount = 0;
    deathCeiling = 3;
    numDamageFreeTurns = 0;

    /** Passive Skill modifiers **/
    vector<Stat> hdS;
    hdS.push_back(MOV);
    vector<int> hdA;
    hdA.push_back(0);
    Debuff *highDivinity = new Debuff("Bleedout", "Passive Skill",
                                      t, hdS, hdA, -1);
    addDebuff(highDivinity);
}

/***** Active Skills *****/
int ServantBerserkerAxe::attack(vector<Servant *> defenders, bool counter)
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
                defenders[i]->subHP(dam, D_STR);

                // Apply the Bleedout debuff
                vector<Stat> tStats;
                tStats.push_back(MAXHP);
                vector<int> tAmounts;
                tAmounts.push_back(-10);
                Debuff* bOut = new Debuff("Bleedout",
                                          "You are incapable of healing fully due to blood loss.",
                                          defenders[i]->getTeam(), tStats,
                                          tAmounts, 3);

                defenders[i]->addDebuff(bOut);
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

int ServantBerserkerAxe::extendGodmind()
{
    if (ascension != 2 || !isGodmindActive() || isGodmindExtended())
    {
        // Godmind is not active!
        if (!isGodmindActive())
            log->addToEventLog("Godmind is not currently active!");
        else if (isGodmindExtended())
            log->addToEventLog("Godmind has already been extended!");
        return 41;
    }

    if (actionMPCosts[ascension][2] > currMP)
        return 1; // Not enough MP to attack

    subMP(actionMPCosts[ascension][2]);

    int turnsLeft = 0;
    for (int i = 0; i < (int) debuffs.size(); i++)
    {
        if(debuffs[i]->getDebuffName().compare("Godmind") == 0)
        {
            turnsLeft = debuffs[i]->getTurnsRemaining();
            debuffs.erase(debuffs.begin() + i);
            i--;
        }
    }

    // Apply the "Godking" debuff
    vector<Stat> tStats;
    tStats.push_back(STR);
    tStats.push_back(MAG);
    tStats.push_back(DEF);
    tStats.push_back(RES);
    tStats.push_back(SPD);
    tStats.push_back(SKL);
    tStats.push_back(LUK);
    vector<int> tAmounts;
    tAmounts.push_back(10);
    tAmounts.push_back(-10);
    tAmounts.push_back(-10);
    tAmounts.push_back(-10);
    tAmounts.push_back(-10);
    tAmounts.push_back(-10);
    tAmounts.push_back(-10);
    Debuff* gEx = new Debuff("Godking",
                             "You have become godlike, rendering you invulnerable to all but the strongest attacks.",
                             getTeam(), tStats, tAmounts, 5 + turnsLeft);

    addDebuff(gEx);

    log->addToEventLog(getFullName() + " has extended Godmind into Godking.");

    return 0;
}

/***** Protected Functions *****/
bool ServantBerserkerAxe::isGodmindExtended()
{
    bool isExtended = false;
    for (int i = 0; i < (int) debuffs.size(); i++)
    {
        if(debuffs[i]->getDebuffName().compare("Godking") == 0)
        {
            isExtended = true;
        }
    }

    return isExtended;
}

int ServantBerserkerAxe::godmindTurnsLeft()
{
    int turnsLeft = 0;
    for (int i = 0; i < (int) debuffs.size(); i++)
    {
        if(debuffs[i]->getDebuffName().compare("Godmind") == 0 ||
                debuffs[i]->getDebuffName().compare("Godking") == 0)
        {
            turnsLeft = debuffs[i]->getTurnsRemaining();
        }
    }

    return turnsLeft;
}

/***** Function Re-definitions *****/
bool ServantBerserkerAxe::isGodmindActive()
{
    bool isActive = false;

    for (int i = 0; i < (int) debuffs.size(); i++)
    {
        if(debuffs[i]->getDebuffName().compare("Godmind") == 0 ||
                debuffs[i]->getDebuffName().compare("Godking") == 0)
        {
            isActive = true;
        }
    }

    return isActive;
}

void ServantBerserkerAxe::turnUpdate()
{
    int godmindChance = numDamageFreeTurns * getLuk();
    if (!isGodmindActive())
    {
        int r = getRandNum();
        if ((ascension != 2 && r <= godmindChance / 2) ||
                (ascension == 2 && r <= godmindChance))
        {
            log->addToEventLog("Godmind comes into effect! " + getFullName() +
                               " has temporarily regained their sanity!");
            // Apply Godmind
            vector<Stat> tStats;
            tStats.push_back(STR);
            tStats.push_back(MAG);
            tStats.push_back(DEF);
            tStats.push_back(RES);
            tStats.push_back(SPD);
            tStats.push_back(SKL);
            tStats.push_back(LUK);
            vector<int> tAmounts;
            tAmounts.push_back(10);
            tAmounts.push_back(-10);
            tAmounts.push_back(-10);
            tAmounts.push_back(-10);
            tAmounts.push_back(-10);
            tAmounts.push_back(-10);
            tAmounts.push_back(-10);
            Debuff* gEx = new Debuff("Godmind",
                                     "You have temporarily regained your sanity.",
                                     getTeam(), tStats, tAmounts, 3);
            addDebuff(gEx);
        }
    }

    if (ascension == 2)
    {
        // Heal .025 * MAXHP, as per Godking
        addHP(0.025 * getMaxHP());
    }

    if (!isGodmindActive())
        numDamageFreeTurns++;
}

int ServantBerserkerAxe::doAction(int actionNum, vector<Servant *> defenders)
{
    if (actionNum == 0)
        return attack(defenders, true);
    else if (actionNum == 1 && ascension == 2)
        return extendGodmind();
    else
        return 2;
}

void ServantBerserkerAxe::subHP(int hp, DamageType dt)
{
    bool noDamage = false;

    // Godking
    if (isGodmindExtended() && (dt != NP_STR && dt != NP_MAG && dt != OMNI && dt != GAEBOLG))
        noDamage = true;
    else if (ascension == 2 && dt != NP_STR && dt != NP_MAG && dt != GAEBOLG && dt != OMNI &&
             hp <= (0.1 * getMaxHP()))
        noDamage = true;

    if (!noDamage)
    {
        currHP -= hp;
        int mhp = getMaxHP();
        if (currHP > mhp)
            currHP = mhp;
        else if (currHP <= 0)
        {
            currHP = 0;
            // Godhand
            if (ascension >= 1 && deathCount < deathCeiling && !isGodmindActive())
            {
                deathCount++;
                maxHP[ascension] *= 0.75;
                def[ascension] *= 1.1;
                currHP = maxHP[ascension];
                remAllDebuffs(true);
                log->addToEventLog(getFullName() + "'s Godhand activated, reviving them!");
            }
            else if (isGodmindActive())
            {
                deathCount = deathCeiling;
                remAllDebuffs(false);
            }
            else
            {
                deathCount++;
                remAllDebuffs(false);
            }
        }
    }
    else
    {
        log->addToEventLog(getFullName() + " resisted taking damage!");
    }
}

void ServantBerserkerAxe::setHP(int hp)
{
    currHP = hp;
    int mhp = getMaxHP();
    if (currHP > mhp)
        currHP = mhp;
    else if (currHP <= 0)
    {
        currHP = 0;
        // Godhand
        if (ascension >= 1 && deathCount < deathCeiling && !isGodmindActive())
        {
            deathCount++;
            maxHP[ascension] *= 0.75;
            def[ascension] *= 1.1;
            currHP = maxHP[ascension];
            remAllDebuffs(true);
        }
        else if (isGodmindActive())
        {
            deathCount = deathCeiling;
            remAllDebuffs(false);
        }
        else
        {
            deathCount++;
            remAllDebuffs(false);
        }
    }
}
