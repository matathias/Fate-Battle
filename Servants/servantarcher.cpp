#include "servantarcher.h"

ServantArcher::ServantArcher(int as, Team t, Logger *l) : Servant(as, t, l)
{
    maxHP.push_back(100);
    maxHP.push_back(150);
    maxHP.push_back(200);
    maxMP.push_back(100);
    maxMP.push_back(150);
    maxMP.push_back(200);
    mov.push_back(5);
    mov.push_back(5);
    mov.push_back(6);
    str.push_back(35);
    str.push_back(45);
    str.push_back(55);
    mag.push_back(10);
    mag.push_back(10);
    mag.push_back(10);
    def.push_back(20);
    def.push_back(20);
    def.push_back(20);
    res.push_back(10);
    res.push_back(10);
    res.push_back(10);
    spd.push_back(30);
    spd.push_back(35);
    spd.push_back(40);
    skl.push_back(35);
    skl.push_back(45);
    skl.push_back(55);
    luk.push_back(20);
    luk.push_back(20);
    luk.push_back(20);

    currHP = maxHP[as];
    currMP = maxMP[as];

    lowRange = 1;
    hiRange = 4;

    clss = Archer;
    name = "Archer";

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
    Debuff *highDivinity = new Debuff("Independent Action", "Passive Skill",
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

vector<int> ServantArcher::getEvade()
{
    // Evasion = Speed * 2 + Luck
    vector<int> evade;
    evade.push_back(getInitialEvade());
    // Eye of the Mind: (SKL / 4) chance of taking no damage.
    evade.push_back(getSkl() / 4);
    return evade;
}
