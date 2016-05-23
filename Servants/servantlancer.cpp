#include "servantlancer.h"

ServantLancer::ServantLancer(int as, Team t, Logger *l) : Servant(as, t, l)
{
    maxHP.push_back(200);
    maxHP.push_back(225);
    maxHP.push_back(250);
    maxMP.push_back(75);
    maxMP.push_back(100);
    maxMP.push_back(125);
    mov.push_back(5);
    mov.push_back(6);
    mov.push_back(7);
    str.push_back(40);
    str.push_back(50);
    str.push_back(60);
    mag.push_back(20);
    mag.push_back(20);
    mag.push_back(20);
    def.push_back(30);
    def.push_back(30);
    def.push_back(40);
    res.push_back(20);
    res.push_back(20);
    res.push_back(20);
    spd.push_back(50);
    spd.push_back(40);
    spd.push_back(60);
    skl.push_back(75);
    skl.push_back(40);
    skl.push_back(50);
    luk.push_back(5);
    luk.push_back(5);
    luk.push_back(5);

    currHP = maxHP[as];
    currMP = maxMP[as];

    lowRange = 1;
    hiRange = 1;

    clss = Lancer;
    name = "Lancer";

    vector<string> basicActions;
    basicActions.push_back("1: Attack");
    basicActions.push_back("2: Skill: Mana Burst");
    actionList.push_back(basicActions);
    actionList.push_back(basicActions);
    actionList.push_back(basicActions);
    vector<ActionType> actionTypes;
    actionTypes.push_back(S);
    actionTypes.push_back(N);
    actionListTypes.push_back(actionTypes);
    actionListTypes.push_back(actionTypes);
    actionListTypes.push_back(actionTypes);
    vector<int> actionCosts;
    actionCosts.push_back(2);
    actionCosts.push_back(20);
    actionMPCosts.push_back(actionCosts);
    actionMPCosts.push_back(actionCosts);
    actionMPCosts.push_back(actionCosts);

    vector<bool> acDodge;
    acDodge.push_back(true);
    acDodge.push_back(true);
    actionDodgeable.push_back(acDodge);
    actionDodgeable.push_back(acDodge);
    actionDodgeable.push_back(acDodge);
    vector<bool> acCounter;
    acCounter.push_back(true);
    acCounter.push_back(true);
    actionCounterable.push_back(acCounter);
    actionCounterable.push_back(acCounter);
    actionCounterable.push_back(acCounter);

    /** Passive Skill modifiers **/
    vector<Stat> hdS;
    hdS.push_back(MOV);
    vector<int> hdA;
    hdA.push_back(0);
    Debuff *highDivinity = new Debuff("Battle Continuation", "Passive Skill",
                                      t, hdS, hdA, -1);
    addDebuff(highDivinity);

    hdS.clear();
    hdS.push_back(MOV);
    hdA.clear();
    hdA.push_back(0);
    Debuff *highDivinity2 = new Debuff("Eye of the Mind", "Passive Skill",
                                      t, hdS, hdA, -1);
    addDebuff(highDivinity2);
}

/***** Active Skills *****/
int ServantLancer::manaBurst(vector<Servant *> defenders)
{
    // Check to make sure that Mana Burst isn't already active.
    if (manaBurstActive())
    {
        log->addToErrorLog("Mana Burst is already active!");

        QMessageBox messageBox;
        messageBox.setWindowTitle("Final Fate");
        messageBox.setText("Mana Burst is already active!");
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.setDefaultButton(QMessageBox::Ok);
        messageBox.exec();

        return 41;
    }

    // Add buff to this Lancer
    vector<Stat> gvs;
    gvs.push_back(STR);
    gvs.push_back(DEF);
    vector<int> gvi;
    gvi.push_back((int) (getStr() * 0.75));
    gvi.push_back((int) (-1 * (getDef() * 0.75)));
    Debuff *guardian = new Debuff("Mana Burst",
                                  "You sacrifice defense to gain incredible power.",
                                  team, gvs, gvi, 3);
    addDebuff(guardian);

    log->addToEventLog(getFullName() + " activated Mana Burst!");

    return 0;
}

/***** Retrievers and Helpers *****/
bool ServantLancer::manaBurstActive()
{
    bool ret = false;
    for (unsigned int i = 0; i < debuffs.size(); i++)
    {
        if(debuffs[i]->getDebuffName().compare("Mana Burst") == 0)
            ret = true;
    }

    return ret;
}

/***** Overridden functions from Servant class *****/
void ServantLancer::subHP(int hp, DamageType dt)
{
    // Skill: Battle Continuation
    //  Does not activate against "Omni" or "Gae Bolg" damage
    if (currHP == 1 && dt != OMNI && dt != GAEBOLG)
    {
        if (getRandNum() <= getLuk())
        {
            log->addToEventLog(getFullName() + "'s Battle Continuation prevented them from dying!");
            return;
        }
    }
    else if (currHP - hp <= 0 && dt != OMNI && dt != GAEBOLG)
    {
        if (getRandNum() <= getLuk() * 2)
        {
            log->addToEventLog(getFullName() + "'s Battle Continuation prevented them from dying!");
            currHP = 1;
            return;
        }
    }

    currHP -= hp;
    int mhp = getMaxHP();
    if (currHP > mhp)
        currHP = mhp;

    if (currHP <= 0)
    {
        currHP = 0;
        remAllDebuffs(false);
    }
}

int ServantLancer::doAction(int actionNum, vector<Servant *> defenders)
{
    int ret = 0;
    switch (actionNum)
    {
        case 0:
            ret = attack(defenders, true);
            break;
        case 1:
            ret = manaBurst(defenders);
            break;
        case 2:
            ret = activateNP1(defenders);
            break;
        case 3:
            ret = activateNP2(defenders);
            break;
        case 4:
            ret = activateNP3(defenders);
            break;
        default:
            return 2; // Not a valid choice
            break;
    }
    return ret;
}

vector<int> ServantLancer::getEvade()
{
    // Evasion = Speed * 2 + Luck
    vector<int> evade;
    evade.push_back(getInitialEvade());
    // Eye of the Mind: (SKL / 4) chance of taking no damage.
    evade.push_back(getSkl() / 4);
    return evade;
}

int ServantLancer::isActionNP(int action)
{
    int ret = -1;
    switch(action)
    {
        case 2:
            ret = 0;
            break;
        case 3:
            ret = 1;
            break;
        case 4:
            ret = 2;
            break;
        default:
            break;
    }

    return ret;
}

vector<Coordinate> ServantLancer::getActionRange(int action)
{
    // Figure out what action this is and return the appropriate range
    vector<Coordinate> range;

    if (action == 0) // Regular attack range
    {
        for (int i = -1*getHighRange(); i <= getHighRange(); i++)
        {
            for (int j = -1*getHighRange(); j <= getHighRange(); j++)
            {
                if (abs(i) + abs(j) >= getLowRange() && abs(i) + abs(j) <= getHighRange())
                {
                    Coordinate inRange;
                    inRange.x = i;
                    inRange.y = j;
                    range.push_back(inRange);
                }
            }
        }
    }
    else if (action == 1) // Mana Burst Range
    {
        Coordinate inRange;
        inRange.x = 0;
        inRange.y = 0;
        range.push_back(inRange);
    }
    else
    {
        range = getNPRange(action-2);
    }

    return range;
}
