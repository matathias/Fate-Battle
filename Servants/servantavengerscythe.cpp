#include "servantavengerscythe.h"
#include "PlayField.h"

ServantAvengerScythe::ServantAvengerScythe(int as, Team t, Logger *l) : ServantAvenger(as, t, l)
{
    name = "Scythe Avenger";

    /** Stat modifiers **/
    /*int strMod = 5;
    int spdMod = -5;

    spd[0] += spdMod;
    spd[1] += spdMod;
    spd[2] += spdMod;
    str[0] += strMod;
    str[1] += strMod;
    str[2] += strMod;*/

    actionList[1].push_back("3: NP: Soul Harvester");
    actionList[2].push_back("3: NP: Soul Harvester");
    actionList[2].push_back("4: NP: Death Blow");

    actionListTypes[1].push_back(D);
    actionListTypes[2].push_back(D);
    actionListTypes[2].push_back(N);

    actionMPCosts[1].push_back(maxMP[1] * 0.3);
    actionMPCosts[2].push_back(maxMP[2] * 0.3);
    actionMPCosts[2].push_back(100);

    actionDodgeable[1].push_back(false);
    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(false);

    actionCounterable[1].push_back(false);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);

    vector<string> np1;
    np1.push_back("Lifetaker");
    np1.push_back("Passive Noble Phantasm. Have a LUK chance of healing for half the damage you deal.");
    vector<string> np2;
    np2.push_back("Soul Harvester");
    np2.push_back("For a cost of .3*MAXMP, harvest a dead unit -- be they friend or foe -- and add a quarter of all their stats to your own. The buffs last until the end of the battle. Any harvested units cannot be revived. The bonuses provided by Soul Harvester carry over if Carrying On the Torch is used.");
    vector<string> np3;
    np3.push_back("Death Blow");
    np3.push_back("Costs 100 MP. Your attacks are not guaranteed, but anyone you successfully attack (including counterattacks) within the next four turns suffers the “Doom” debuff. Anyone with the “Doom” debuff will die after two turns if they fail a LUK check (have a LUK chance of surviving), no matter how much HP they have, and they cannot be revived.");
    noblePhantasms.push_back(np1);
    noblePhantasms.push_back(np2);
    noblePhantasms.push_back(np3);

    // Lifetaker range (doesn't really have one)
    npRanges.push_back(getLowToHighRange(0,0));
    // Soul Harvester range (doesn't really have one)
    npRanges.push_back(getLowToHighRange(0,0));
    // Death Blow range (doesn't really have one)
    npRanges.push_back(getLowToHighRange(0,0));

    /** Passive Skill modifiers **/
    vector<Stat> hdS;
    hdS.push_back(SPD);
    hdS.push_back(STR);
    vector<int> hdA;
    hdA.push_back(-5);
    hdA.push_back(5);
    Debuff *highDivinity = new Debuff("Why Did You...", "Passive Skill",
                                      t, hdS, hdA, -1);
    addDebuff(highDivinity);

    hdS.clear();
    hdS.push_back(MOV);
    hdA.clear();
    hdA.push_back(0);
    Debuff *highDivinity2 = new Debuff("Sharp Side Inwards", "Passive Skill",
                                      t, hdS, hdA, -1);
    addDebuff(highDivinity2);

    hdS.clear();
    hdS.push_back(MOV);
    hdA.clear();
    hdA.push_back(0);
    Debuff *highDivinity3 = new Debuff("Lifetaker", "Passive Noble Phantasm",
                                      t, hdS, hdA, -1);
    addDebuff(highDivinity3);
}

/***** Private Helper Functions *****/
bool ServantAvengerScythe::deathBlowActive()
{
    bool active = false;
    for (int i = 0; i < (int) debuffs.size(); i++)
    {
        if(debuffs[i]->getDebuffName().compare("Death Blow") == 0)
        {
            active = true;
        }
    }

    return active;
}

/***** Function Re-definitions *****/
int ServantAvengerScythe::isActionNP(int action)
{
    int ret = -1;
    if (action >= 2)
    {
        ret = action - 1;
    }

    return ret;
}

vector<Coordinate> ServantAvengerScythe::getActionRange(int action)
{
    vector<Coordinate> range;

    if (action == 0)
        range = getLowToHighRange(getLowRange(), getHighRange());
    else if (action == 1)
        range = getLowToHighRange(0,0);
    else
    {
        int ind = isActionNP(action);
        if (ind != -1)
            range = getNPRange(ind);
    }

    return range;
}

int ServantAvengerScythe::doAction(int actionNum, vector<Servant *> defenders)
{
    int ret = 0;

    if (actionNum == 0)
        ret = attack(defenders, true);
    else if (actionNum == 1)
        ret = carryingOnTheTorch(defenders);
    else if (actionNum == 2)
        ret = activateNP1(defenders);
    else
        ret = activateNP2(defenders);

    return ret;
}

int ServantAvengerScythe::attack(vector<Servant *> defenders, bool counter)
{
    if (actionMPCosts[ascension][0] > currMP)
        return 1; // Not enough MP to attack

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

        // Skill: The Sharp Side Is Pointing Toward You
        r = getRandNum();
        if (r <= 60 - (getSkl() + getLuk()))
        {
            subHP(getStr() / 10, D_STR);
            log->addToEventLog(getFullName() + " accidentally grazed themself with their scythe, taking " +
                               to_string(getStr() / 10) + " damage.");
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
            if (deathBlowActive())
            {
                defenders[i]->subHP(dam, NP_STR);
                // Add the Doom debuff
                vector<Stat> tDebStat;
                tDebStat.push_back(MOV);
                vector<int> tDebAm;
                tDebAm.push_back(0);
                Debuff *newDebuff = new Debuff("Doom",
                                               "Your days are numbered...",
                                               defenders[i]->getTeam(),
                                               tDebStat, tDebAm, 4);

                defenders[i]->addDebuff(newDebuff);
                log->addToEventLog(getFullName() + " doomed " +
                                   defenders[i]->getFullName() + "!");
            }
            else
            {
                defenders[i]->subHP(dam, D_STR);
            }

            // See if Lifetaker activates
            r = getRandNum();
            if (r <= getLuk())
            {
                addHP(dam / 2);
                log->addToEventLog(getFullName() + "'s Lifetaker skill activated, healing them for " +
                                   to_string(dam / 2) + " damage.");
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

    return 0;
}

/***** Noble Phantasms *****/
// Soul Harvester
int ServantAvengerScythe::activateNP1(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][2] > currMP)
        return 1; // Not enough MP to attack

    if (carriedTheTorch)
        return 41; // Can't use Soul Harvester after the Torch has been Carried

    subMP(actionMPCosts[ascension][2]);

    // Add a buff stat to yourself
    vector<Stat> tStats;
    tStats.push_back(MAXHP);
    tStats.push_back(MAXMP);
    tStats.push_back(STR);
    tStats.push_back(MAG);
    tStats.push_back(DEF);
    tStats.push_back(RES);
    tStats.push_back(SPD);
    tStats.push_back(SKL);
    tStats.push_back(LUK);
    vector<int> tAmounts;
    tAmounts.push_back(0.25 * defenders[0]->getMaxHP());
    tAmounts.push_back(0.25 * defenders[0]->getMaxMP());
    tAmounts.push_back(0.25 * defenders[0]->getStr());
    tAmounts.push_back(0.25 * defenders[0]->getMag());
    tAmounts.push_back(0.25 * defenders[0]->getDef());
    tAmounts.push_back(0.25 * defenders[0]->getRes());
    tAmounts.push_back(0.25 * defenders[0]->getSpd());
    tAmounts.push_back(0.25 * defenders[0]->getSkl());
    tAmounts.push_back(0.25 * defenders[0]->getLuk());
    string descrip = "You harvested " + defenders[0]->getFullName() + "'s soul.";
    Debuff* soulHarvest = new Debuff("Soul Harvested", descrip, getTeam(),
                                     tStats, tAmounts, -1);

    // Apply the debuff to the avenger
    addDebuff(soulHarvest);

    // Add a Permadeath debuff to the defender
    vector<Stat> tDebStat;
    tDebStat.push_back(MOV);
    vector<int> tDebAm;
    tDebAm.push_back(0);
    Debuff *newDebuff = new Debuff("Permadeath",
                                   "You are permanently dead.",
                                   defenders[0]->getTeam(), tDebStat, tDebAm,
                                   -1);
    defenders[0]->addDebuff(newDebuff);

    log->addToEventLog(getFullName() + " harvested " +
                       defenders[0]->getFullName() + "'s soul.");

    return 0;
}

// Death Blow
int ServantAvengerScythe::activateNP2(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][3] > currMP)
        return 1; // Not enough MP to attack

    if (deathBlowActive())
        return 41; // Can't use it if it's already active

    subMP(actionMPCosts[ascension][3]);

    log->addToEventLog(getFullName() + " activated Death Blow!");

    // Add a Casual Causality buff
    vector<Stat> tDebStat;
    tDebStat.push_back(MOV);
    vector<int> tDebAm;
    tDebAm.push_back(0);
    Debuff *newDebuff = new Debuff("Death Blow",
                                   "Your scythe brings death upon all it strikes!",
                                   getTeam(), tDebStat, tDebAm, 4);
    addDebuff(newDebuff);

    return 0;
}
