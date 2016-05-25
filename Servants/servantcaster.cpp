#include "servantcaster.h"
#include "PlayField.h"

ServantCaster::ServantCaster(int as, Team t, Logger *l) : Servant(as, t, l)
{
    maxHP.push_back(100);
    maxHP.push_back(125);
    maxHP.push_back(150);
    maxMP.push_back(500);
    maxMP.push_back(750);
    maxMP.push_back(1000);
    mov.push_back(5);
    mov.push_back(5);
    mov.push_back(6);
    str.push_back(10);
    str.push_back(10);
    str.push_back(10);
    mag.push_back(40);
    mag.push_back(50);
    mag.push_back(60);
    def.push_back(10);
    def.push_back(10);
    def.push_back(10);
    res.push_back(30);
    res.push_back(40);
    res.push_back(50);
    spd.push_back(20);
    spd.push_back(20);
    spd.push_back(20);
    skl.push_back(30);
    skl.push_back(40);
    skl.push_back(50);
    luk.push_back(40);
    luk.push_back(50);
    luk.push_back(50);

    currHP = maxHP[as];
    currMP = maxMP[as];

    lowRange = 1;
    hiRange = 3;

    clss = Caster;
    name = "Caster";
    ressurectCount = 0;
    territoryActive = false;

    vector<string> basicActions;
    basicActions.push_back("1: Attack");
    basicActions.push_back("2: Heal");
    basicActions.push_back("3: Resurrect");
    basicActions.push_back("4: Cast From Life");
    actionList.push_back(basicActions);
    actionList.push_back(basicActions);
    actionList.push_back(basicActions);
    vector<ActionType> actionTypes;
    actionTypes.push_back(S);
    actionTypes.push_back(S);
    actionTypes.push_back(D);
    actionTypes.push_back(N);
    actionListTypes.push_back(actionTypes);
    actionListTypes.push_back(actionTypes);
    actionListTypes.push_back(actionTypes);
    vector<int> actionCosts;
    actionCosts.push_back(5);
    actionCosts.push_back(25);
    actionCosts.push_back(50);
    actionCosts.push_back(0);
    actionMPCosts.push_back(actionCosts);
    actionMPCosts.push_back(actionCosts);
    actionMPCosts.push_back(actionCosts);

    // Heal All
    actionList[2].push_back("5: Heal All");
    actionListTypes[2].push_back(N);
    actionMPCosts[2].push_back(50);

    vector<bool> acDodge;
    acDodge.push_back(true);
    acDodge.push_back(false);
    acDodge.push_back(false);
    acDodge.push_back(false);
    actionDodgeable.push_back(acDodge);
    actionDodgeable.push_back(acDodge);
    actionDodgeable.push_back(acDodge);
    vector<bool> acCounter;
    acCounter.push_back(true);
    acCounter.push_back(false);
    acCounter.push_back(false);
    acCounter.push_back(false);
    actionCounterable.push_back(acCounter);
    actionCounterable.push_back(acCounter);
    actionCounterable.push_back(acCounter);

    // Heal All
    actionDodgeable[2].push_back(false);
    actionCounterable[2].push_back(false);
}

/***** Active Skills *****/
int ServantCaster::heal(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][1] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][1]);
        for (unsigned int i = 0; i < defenders.size(); i++)
        {
            defenders[i]->addHP(getMag());
            log->addToEventLog(getFullName() + " healed " +
                               defenders[i]->getFullName() + " " +
                               to_string(getMag()) + " HP!");
        }
    }

    return 0;
}

int ServantCaster::healAll()
{
    if (actionMPCosts[ascension][4] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][4]);

        vector<vector<Servant*>> fi = field->getServantLocations();

        for (unsigned int i = 0; i < fi.size(); i++)
        {
            for (unsigned int j = 0; j < fi[i].size(); j++)
            {
                if (fi[i][j] != NULL && fi[i][j]->getTeam() == getTeam())
                    fi[i][j]->addHP(getMag() / 2);
            }
        }

        log->addToEventLog(getFullName() + " healed their whole team for " +
                           to_string(getMag() / 2) + " HP!");
    }

    return 0;
}

int ServantCaster::resurrect(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][2] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][2]);
        // Double the MP cost if the ascension is less than 2
        if (ascension < 2)
        {
            actionMPCosts[ascension][2] *= 2;
            ressurectCount++;
        }

        for (unsigned int i = 0; i < defenders.size(); i++)
        {
            defenders[i]->setHP(defenders[i]->getMaxHP() / 2);

            log->addToEventLog(getFullName() + " resurrected " +
                               defenders[i]->getFullName() + "!");
        }
    }

    return 0;
}

int ServantCaster::castFromLife()
{
    // The cost for castFromLife is actually HP. Drain HP accordingly and
    // get the amount drained, so that the amount of MP restored can be
    // accurately proportional.
    double prop = 1;
    int difference = getMaxHP() / 5;

    if(getCurrHP() < (getMaxHP() / 5) + 1)
    {
        difference = getCurrHP() - 1;
        prop = (difference * 1.0) / (getMaxHP() / 5.0);
        setHP(1);
    }
    else
    {
        subHP(getMaxHP() / 5, OMNI);
    }

    // Get all servants in range
    vector<Coordinate> range = getLowToHighRange(0, 2);
    vector<Servant*> defenders = field->getAllInRange(this, range);

    // Replenish .4*MAXMP * prop for all nearby allies
    for (unsigned int i = 0; i < defenders.size(); i++)
    {
        if (defenders[i]->getTeam() == getTeam())
        {
            defenders[i]->addMP((int) (defenders[i]->getMaxMP() * 0.4 * prop));
        }
    }

    log->addToEventLog(getFullName() + " sacrificed " +
                       to_string(difference) +
                       " HP to replenish MP to their whole team!");

    return 0;
}

/***** Re-defining superclass functions *****/
int ServantCaster::attack(vector<Servant *> defenders, bool counter)
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
                dam = capZero(getMag() - defenders[i]->getRes()) * attackMult;
                log->addToEventLog(getFullName() + " dealt " +
                                   to_string(dam) + " damage to " +
                                   defenders[i]->getFullName() + ".");
                defenders[i]->subHP(dam, D_MAG);
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

int ServantCaster::isActionNP(int action)
{
    // Remember that Heal All is only a performable action in the
    //  Final Ascension!
    int ret = -1;
    if (ascension == 2)
    {
        if (action >= 5)
            ret = action - 5;
    }
    else
    {
        if (action >= 4)
            ret = action - 4;
    }

    return ret;
}

vector<Coordinate> ServantCaster::getActionRange(int action)
{
    vector<Coordinate> range;

    if (action == 0) // Regular attack range
    {
        range = getLowToHighRange(getLowRange(), getHighRange());
    }
    else if (action == 1) // Heal
    {
        range = getLowToHighRange(0, field->getFieldLength() + field->getFieldWidth());
    }
    else if (action == 2) // Resurrect
    {
        Coordinate inRange;
        inRange.x = 0;
        inRange.y = 0;
        range.push_back(inRange);
    }
    else if (action == 3) // Cast From Life
    {
        Coordinate inRange;
        inRange.x = 0;
        inRange.y = 0;
        range.push_back(inRange);
    }
    else if (ascension == 2 && action == 4) // Heal All
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

bool ServantCaster::isHealAction(int action)
{
    return (action == 1 || (ascension == 2 && action == 4));
}
