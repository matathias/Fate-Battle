#include "servantassassin.h"
#include "PlayField.h"

ServantAssassin::ServantAssassin(int as, Team t, Logger *l) : Servant(as, t, l)
{
    maxHP.push_back(100);
    maxHP.push_back(125);
    maxHP.push_back(150);
    maxMP.push_back(75);
    maxMP.push_back(100);
    maxMP.push_back(125);
    mov.push_back(5);
    mov.push_back(6);
    mov.push_back(7);
    str.push_back(20);
    str.push_back(20);
    str.push_back(20);
    mag.push_back(10);
    mag.push_back(10);
    mag.push_back(10);
    def.push_back(20);
    def.push_back(20);
    def.push_back(20);
    res.push_back(10);
    res.push_back(10);
    res.push_back(10);
    spd.push_back(40);
    spd.push_back(50);
    spd.push_back(75);
    skl.push_back(40);
    skl.push_back(50);
    skl.push_back(75);
    luk.push_back(40);
    luk.push_back(50);
    luk.push_back(75);

    currHP = maxHP[as];
    currMP = maxMP[as];

    lowRange = 1;
    hiRange = 2;

    clss = Assassin;
    name = "Assassin";

    vector<string> basicActions;
    basicActions.push_back("1: Attack");
    basicActions.push_back("2: Skill: Poison Strike");
    basicActions.push_back("3: Skill: Presence Concealment");
    actionList.push_back(basicActions);
    actionList.push_back(basicActions);
    actionList.push_back(basicActions);
    vector<ActionType> actionTypes;
    actionTypes.push_back(S);
    actionTypes.push_back(S);
    actionTypes.push_back(N);
    actionListTypes.push_back(actionTypes);
    actionListTypes.push_back(actionTypes);
    actionListTypes.push_back(actionTypes);
    vector<int> actionCosts;
    actionCosts.push_back(2);
    actionCosts.push_back(8);
    actionCosts.push_back(15);
    actionMPCosts.push_back(actionCosts);
    actionMPCosts.push_back(actionCosts);
    actionMPCosts.push_back(actionCosts);

    vector<bool> acDodge;
    acDodge.push_back(true);
    acDodge.push_back(true);
    acDodge.push_back(false);
    actionDodgeable.push_back(acDodge);
    actionDodgeable.push_back(acDodge);
    actionDodgeable.push_back(acDodge);
    vector<bool> acCounter;
    acCounter.push_back(true);
    acCounter.push_back(false);
    acCounter.push_back(false);
    actionCounterable.push_back(acCounter);
    actionCounterable.push_back(acCounter);
    actionCounterable.push_back(acCounter);

    vector<Stat> tempS;
    tempS.push_back(MOV);
    vector<int> tempA;
    tempA.push_back(0);
    classDebuff = new Debuff("No Debuff", "No Effect", team, tempS, tempA, 1);
}

/***** Active Skills *****/
int ServantAssassin::poisonStrike(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][1] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][1]);
        for (unsigned int i = 0; i < defenders.size(); i++)
        {
            log->addToEventLog(getFullName() + " used Poison Strike against " +
                               defenders[i]->getFullName() + "!");

            // Check if you hit the targets
            vector<int> opEvade = defenders[i]->getEvade();
            bool hit = false;
            // Calculate accuracy
            int accuracy = capZero(getHitRate() - opEvade[0]);

            int r = getRandNum();
            if (accuracy >= r)
                hit = true;

            // Skill: Presence Detection skips all other evasion checks!

            // If you hit, apply the poison debuff
            if (hit)
            {
                vector<Stat> dStats;
                dStats.push_back(HP);
                vector<int> dAmount;
                dAmount.push_back(defenders[i]->getMaxHP() / 10);
                Debuff *poison = new Debuff("Poison",
                                            "Poison does damage over time.",
                                            defenders[i]->getTeam(),
                                            dStats, dAmount, 5);
                defenders[i]->addDebuff(poison);
                log->addToEventLog(getFullName() + " poisoned " +
                                   defenders[i]->getFullName() + "!");
            }
            else
            {
                log->addToEventLog(getFullName() + " missed " +
                                   defenders[i]->getFullName() + "!");
            }

            // Poison strike does no damage and thus there is no need to check
            // if the defender is dead.
        }
    }

    return 0;
}

int ServantAssassin::presenceConcealment(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][2] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][2]);

        vector<Stat> pcStat;
        pcStat.push_back(MOV);
        vector<int> pcAmount;
        pcAmount.push_back(-1 * (getMov() / 4));

        Debuff *presConceal = new Debuff("Presence Concealment",
                                         "You conceal your presence, making yourself harder to hit. Evasion is increased.",
                                         team, pcStat, pcAmount, 4);
        addDebuff(presConceal);

        log->addToEventLog(getFullName() + " activated Presence Concealment!");
    }

    return 0;
}

// Returns true if this assassin has a "Presence Concealment" buff, and false
//  otherwise.
bool ServantAssassin::presenceConcealed()
{
    bool ret = false;
    for (unsigned int i = 0; i < debuffs.size(); i++)
    {
        if(debuffs[i]->getDebuffName().compare("Presence Concealment") == 0)
            ret = true;
    }

    return ret;
}

/***** Function re-definitions *****/
int ServantAssassin::attack(vector<Servant *> defenders, bool counter)
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

            // Skill: Presence Detection skips all other evasion checks!

            // If you hit, calculate crit chance
            if (hit)
            {
                int attackMult = 1;
                int critChance = capZero(getCriticalRate() -
                                 defenders[i]->getCriticalEvade());
                r = getRandNum();
                if (critChance >= r)
                    attackMult *= 3;

                // Add the weapon-specific debuff to the target
                Debuff *deb = new Debuff(classDebuff->getDebuffName(),
                                         classDebuff->getDebuffDescrip(),
                                         defenders[i]->getTeam(),
                                         classDebuff->getDebuffStats(),
                                         classDebuff->getDebuffAmounts(),
                                         classDebuff->getTurnsRemaining());

                defenders[i]->addDebuff(deb);

                // Calculate the chance of Lethality
                r = getRandNum();
                if (getSkl() / 8 > r)
                {
                    dam = defenders[i]->getMaxHP() * attackMult;

                    log->addToEventLog(getFullName() +
                                       " activated Lethality against " +
                                       defenders[i]->getFullName() + "!" +
                                       to_string(dam) + " damage dealt!");
                    defenders[i]->subHP(dam, D_STR);
                }
                else
                {
                    // Deal the damage
                    dam = capZero(getStr() - defenders[i]->getDef()) * attackMult;

                    log->addToEventLog(getFullName() + " dealt " +
                                       to_string(dam) + " damage to " +
                                       defenders[i]->getFullName() + ".");
                    defenders[i]->subHP(dam, D_STR);
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
    }

    return 0;
}

vector<int> ServantAssassin::getEvade()
{
    // Evasion = Speed * 2 + Luck
    // If presence concealment is active, add 25 to evade.
    int ev = (getSpd() * 2) + getLuk();
    vector<int> evade;
    if (presenceConcealed())
        evade.push_back(ev + 25);
    else
        evade.push_back(ev);

    // Clairvoyance: (LUK / 4) chance of taking no damage.
    evade.push_back(getLuk() / 4);
    return evade;
}

int ServantAssassin::isActionNP(int action)
{
    int ret = -1;
    if (action >= 3)
        ret = action - 3;

    return ret;
}

vector<Coordinate> ServantAssassin::getActionRange(int action)
{
    vector<Coordinate> range;

    if (action == 0) // Regular attack range
    {
        range = getLowToHighRange(getLowRange(), getHighRange());
    }
    else if (action == 1) // Poison Strike
    {
        range = getLowToHighRange(getLowRange(), getHighRange());
    }
    else if (action == 2) // Presence concealment
    {
        Coordinate inRange;
        inRange.x = 0;
        inRange.y = 0;
        range.push_back(inRange);
    }
    else
    {
        int index = isActionNP(action);
        if (index != -1)
            range = getNPRange(index);
    }

    return range;
}

bool ServantAssassin::isPoisonAction(int action)
{
    return (action == 1);
}

int ServantAssassin::doAction(int actionNum, vector<Servant *> defenders)
{
    int ret = 0;
    switch (actionNum)
    {
        case 0:
            ret = attack(defenders, true);
            break;
        case 1:
            ret = poisonStrike(defenders);
            break;
        case 2:
            ret = presenceConcealment(defenders);
            break;
        case 3:
            ret = activateNP1(defenders);
            break;
        case 4:
            ret = activateNP2(defenders);
            break;
        case 5:
            ret = activateNP3(defenders);
        default:
            return 2; // Not a valid choice
            break;
    }
    return ret;
}
