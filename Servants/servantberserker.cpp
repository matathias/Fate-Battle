#include "servantberserker.h"

ServantBerserker::ServantBerserker(int as, Team t, Logger *l) : Servant(as, t, l)
{
    maxHP.push_back(175);
    maxHP.push_back(300);
    maxHP.push_back(400);
    maxMP.push_back(50);
    maxMP.push_back(75);
    maxMP.push_back(100);
    mov.push_back(5);
    mov.push_back(5);
    mov.push_back(6);
    str.push_back(50);
    str.push_back(75);
    str.push_back(125);
    mag.push_back(10);
    mag.push_back(10);
    mag.push_back(20);
    def.push_back(25);
    def.push_back(35);
    def.push_back(45);
    res.push_back(10);
    res.push_back(10);
    res.push_back(20);
    spd.push_back(20);
    spd.push_back(25);
    spd.push_back(30);
    skl.push_back(40);
    skl.push_back(50);
    skl.push_back(60);
    luk.push_back(15);
    luk.push_back(20);
    luk.push_back(25);

    currHP = maxHP[as];
    currMP = maxMP[as];

    lowRange = 1;
    hiRange = 1;

    clss = Berserker;
    name = "Berserker";

    vector<string> basicActions;
    basicActions.push_back("1: Attack");
    actionList.push_back(basicActions);
    actionList.push_back(basicActions);
    actionList.push_back(basicActions);
    vector<ActionType> actionTypes;
    actionTypes.push_back(S);
    actionListTypes.push_back(actionTypes);
    actionListTypes.push_back(actionTypes);
    actionListTypes.push_back(actionTypes);
    vector<int> actionCosts;
    actionCosts.push_back(2);
    actionMPCosts.push_back(actionCosts);
    actionMPCosts.push_back(actionCosts);
    actionMPCosts.push_back(actionCosts);

    vector<bool> acDodge;
    acDodge.push_back(true);
    actionDodgeable.push_back(acDodge);
    actionDodgeable.push_back(acDodge);
    actionDodgeable.push_back(acDodge);
    vector<bool> acCounter;
    acCounter.push_back(true);
    actionCounterable.push_back(acCounter);
    actionCounterable.push_back(acCounter);
    actionCounterable.push_back(acCounter);

    /** Passive Skill modifiers **/
    vector<Stat> hdS;
    hdS.push_back(MOV);
    vector<int> hdA;
    hdA.push_back(0);
    Debuff *highDivinity = new Debuff("Mad Roar", "Passive Skill",
                                      t, hdS, hdA, -1);
    addDebuff(highDivinity);

    hdS.clear();
    hdS.push_back(MOV);
    hdA.clear();
    hdA.push_back(0);
    Debuff *highDivinity2 = new Debuff("Mad Counter", "Passive Skill",
                                      t, hdS, hdA, -1);
    addDebuff(highDivinity2);
}

void ServantBerserker::madRoar(vector<Servant *> defenders)
{
    // Figure out which of the defenders are within three spaces of the berserker
    // If they are, do the check and if they fail, apply the immobilized debuff
    for (unsigned int i = 0; i < defenders.size(); i++)
    {
        if(abs(defenders[i]->getCurrLoc().x - currLoc.x) +
                abs(defenders[i]->getCurrLoc().y - currLoc.y) <= 3)
        {
            // They are in range. See if they are affected.
            int r = getRandNum();
            if (r <= capZero((getLuk() * 4) - (defenders[i]->getLuk() + defenders[i]->getSkl())))
            {
                // They failed the check. Apply the immobilized debuff.
                vector<Stat> tStats;
                tStats.push_back(MOV);
                vector<int> tAmounts;
                tAmounts.push_back(-1 * defenders[i]->getMov());
                Debuff* roar = new Debuff("Immobilized",
                                          "An enemy Berserker terrified you with Mad Roar!",
                                          defenders[i]->getTeam(), tStats, tAmounts, 2);

                defenders[i]->addDebuff(roar);
            }
        }
    }
}


int ServantBerserker::doAction(int actionNum, vector<Servant *> defenders)
{
    if (actionNum == 0)
        return attack(defenders, true);
    else
        return 2;
}

void ServantBerserker::turnUpdate()
{
    return;
}

int ServantBerserker::isActionNP(int action)
{
    return -1;
}
