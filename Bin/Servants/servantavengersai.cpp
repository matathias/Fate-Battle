#include "servantavengersai.h"
#include "PlayField.h"

ServantAvengerSai::ServantAvengerSai(int as, Team t, Logger *l) : ServantAvenger(as, t, l)
{
    name = "Sai Avenger";

    hasPermadead = false;
    lastDamage = 0;

    /** Stat modifiers **/
    int strMod = -5;
    int spdMod = 5;

    spd[0] += spdMod;
    spd[1] += spdMod;
    spd[2] += spdMod;
    str[0] += strMod;
    str[1] += strMod;
    str[2] += strMod;

    actionList[2].push_back("3: NP: Casual Causality");

    actionListTypes[2].push_back(N);

    actionMPCosts[2].push_back(85);

    actionDodgeable[2].push_back(false);

    actionCounterable[2].push_back(false);

    vector<string> np1;
    np1.push_back("Damage Forwarding");
    np1.push_back("Passive Noble Phantasm. Have a LUK chance of adding the last damage value you took to your attack.");
    vector<string> np2;
    np2.push_back("Essence of Fragarach");
    np2.push_back("Passive ability. If user has 40 MP and is the target of an enemy Noble Phantasm, enemy attack is completely stopped and enemy loses .8*MAXHP (this can kill). Does not stop Essence of Gae Bolg from activating (though Gae Bolg user will still sustain .8*MAXHP damage), and in fact Gae Bolg will always succeed if Essence of Fragarach activates. This ability activates automatically and cannot be stopped.");
    vector<string> np3;
    np3.push_back("Casual Causality");
    np3.push_back("Spend 85 MP to make your mark on causality. For the next five turns, your attacks always hit and your HP will not drop below 1. However if an attack would have killed you during this period, you gain a permanent Permadeath debuff (i.e. you cannot be revived once you die).");
    noblePhantasms.push_back(np1);
    noblePhantasms.push_back(np2);
    noblePhantasms.push_back(np3);

    // Damage Forwarding range (doesn't really have one)
    npRanges.push_back(getLowToHighRange(0,0));
    // Essence of Fragarach range (doesn't really have one)
    npRanges.push_back(getLowToHighRange(0,0));
    // Casual Causality range (doesn't really have one)
    npRanges.push_back(getLowToHighRange(0,0));
}

/***** Private Helper Functions *****/
bool ServantAvengerSai::casualCausalityActive()
{
    bool active = false;
    for (int i = 0; i < (int) debuffs.size(); i++)
    {
        if(debuffs[i]->getDebuffName().compare("Casual Causality") == 0)
        {
            active = true;
        }
    }

    return active;
}

/***** Function Re-definitions *****/
void ServantAvengerSai::subHP(int hp, DamageType dt)
{
    // Damage Forwarding: track the last damage value taken as damage. Does not
    //  count OMNI damage
    if (dt != OMNI && hp <= 0)
        lastDamage = hp;

    currHP -= hp;
    int mhp = getMaxHP();
    if (currHP > mhp)
        currHP = mhp;
    else if (currHP <= 0)
    {
        // Casual Causality does not protect against Gae Bolg
        if (casualCausalityActive() && dt != GAEBOLG)
        {
            currHP = 1;
            if (!hasPermadead)
            {
                vector<Stat> tDebStat;
                tDebStat.push_back(MOV);
                vector<int> tDebAm;
                tDebAm.push_back(0);
                Debuff *newDebuff = new Debuff("Permadeath",
                                               "If you die while this debuff is active, you cannot be ressurected.",
                                               getTeam(), tDebStat, tDebAm, -1);
                addDebuff(newDebuff);

                hasPermadead = true;
            }
        }
        else
        {
            currHP = 0;
            remAllDebuffs(false);
        }
    }
}

vector<int> ServantAvengerSai::getEvade()
{
    // Evasion = Speed * 2 + Luck
    vector<int> evade;
    evade.push_back(getInitialEvade());
    // Bladecatcher = Skl / 4
    evade.push_back(getSkl() / 4);
    return evade;
}

int ServantAvengerSai::isActionNP(int action)
{
    int ret = -1;
    if (action == 2)
        ret = 2;
    return ret;
}

vector<Coordinate> ServantAvengerSai::getActionRange(int action)
{
    vector<Coordinate> range;

    if (action == 0)
        range = getLowToHighRange(getLowRange(), getHighRange());
    else if (action == 1)
        range = getLowToHighRange(0,0);
    else
        range = getNPRange(2);

    return range;
}

int ServantAvengerSai::doAction(int actionNum, vector<Servant *> defenders)
{
    int ret = 0;

    if (actionNum == 0)
        ret = attack(defenders, true);
    else if (actionNum == 1)
        ret = carryingOnTheTorch(defenders);
    else
        ret = activateNP1(defenders);

    return ret;
}

int ServantAvengerSai::attack(vector<Servant *> defenders, bool counter)
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

        // If Casual Causality is active, your attacks always hit
        if (casualCausalityActive())
            hit = true;

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

            // Check for Damage Forwarding
            int damageForward = 0;
            r = getRandNum();
            if (r <= getLuk())
            {
                damageForward = lastDamage;
                log->addToEventLog(getFullName() + " activated Damage Forwarding!");
            }

            // Deal the damage
            dam = (capZero(getStr() - defenders[i]->getDef()) + addVengeance
                   + damageForward) * attackMult;
            log->addToEventLog(getFullName() + " dealt " +
                               to_string(dam) + " damage to " +
                               defenders[i]->getFullName() + ".");
            if (casualCausalityActive())
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
// Casual Causality
int ServantAvengerSai::activateNP1(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][2] > currMP)
        return 1; // Not enough MP to attack

    if (casualCausalityActive())
        return 41; // Can't use it if it's already active

    subMP(actionMPCosts[ascension][2]);

    log->addToEventLog(getFullName() + " activated Casual Causality!");

    // Add a Casual Causality buff
    vector<Stat> tDebStat;
    tDebStat.push_back(MOV);
    vector<int> tDebAm;
    tDebAm.push_back(0);
    Debuff *newDebuff = new Debuff("Casual Causality",
                                   "Causality is yours to command, causing you to always hit!",
                                   getTeam(), tDebStat, tDebAm, 5);
    addDebuff(newDebuff);

    return 0;
}
