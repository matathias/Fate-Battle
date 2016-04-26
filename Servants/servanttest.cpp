#include "servanttest.h"

ServantTest::ServantTest(int as, Team t) : Servant(as, t)
{
    maxHP.push_back(100);
    maxHP.push_back(150);
    maxHP.push_back(200);
    maxMP.push_back(100);
    maxMP.push_back(150);
    maxMP.push_back(200);
    mov.push_back(5);
    mov.push_back(6);
    mov.push_back(7);
    str.push_back(20);
    str.push_back(30);
    str.push_back(40);
    mag.push_back(20);
    mag.push_back(30);
    mag.push_back(40);
    def.push_back(10);
    def.push_back(20);
    def.push_back(30);
    res.push_back(10);
    res.push_back(20);
    res.push_back(30);
    spd.push_back(20);
    spd.push_back(30);
    spd.push_back(40);
    skl.push_back(20);
    skl.push_back(30);
    skl.push_back(40);
    luk.push_back(10);
    luk.push_back(20);
    luk.push_back(30);

    currHP = maxHP[as];
    currMP = maxMP[as];

    lowRange = 1;
    hiRange = 1;

    clss = Saber;

    vector<string> actions;
    actions.push_back("1: Attack");
    actions.push_back("2: NP 1");
    actions.push_back("3: NP 2");
    actions.push_back("4: NP 3");
    actionList.push_back(actions);
    actionList.push_back(actions);
    actionList.push_back(actions);
    vector<ActionType> actionTypes;
    actionTypes.push_back(S);
    actionTypes.push_back(S);
    actionTypes.push_back(S);
    actionTypes.push_back(A);
    actionListTypes.push_back(actionTypes);
    actionListTypes.push_back(actionTypes);
    actionListTypes.push_back(actionTypes);
    vector<int> actionCosts;
    actionCosts.push_back(2);
    actionCosts.push_back(25);
    actionCosts.push_back(50);
    actionCosts.push_back(100);
    actionMPCosts.push_back(actionCosts);
    actionMPCosts.push_back(actionCosts);
    actionMPCosts.push_back(actionCosts);

    vector<string> np1;
    np1.push_back("Straight Slash");
    np1.push_back("Attack straight-on for thrice normal damage.");
    vector<string> np2;
    np2.push_back("Side Slash");
    np2.push_back("Attack from the side, catching your opponent off-guard and dealing undodgeable five times normal damage. No counterattacks or enemy skill activations.");
    vector<string> np3;
    np3.push_back("Omni Slash");
    np3.push_back("Attack all adjacent opponents for five times normal damage. No counterattacks or enemy skill activations.");
    noblePhantasms.push_back(np1);
    noblePhantasms.push_back(np2);
    noblePhantasms.push_back(np3);

    Coordinate c1;
    c1.x = 1; c1.y = 0;
    Coordinate c2;
    c2.x = 0; c2.y = 1;
    Coordinate c3;
    c3.x = -1; c3.y = 0;
    Coordinate c4;
    c4.x = 0; c4.y = -1;
    vector<Coordinate> npc1;
    npc1.push_back(c1);
    npc1.push_back(c2);
    npc1.push_back(c3);
    npc1.push_back(c4);
    vector<Coordinate> npc2;
    npc2.push_back(c1);
    npc2.push_back(c2);
    npc2.push_back(c3);
    npc2.push_back(c4);
    vector<Coordinate> npc3;
    npc3.push_back(c1);
    npc3.push_back(c2);
    npc3.push_back(c3);
    npc3.push_back(c4);
    npRanges.push_back(npc1);
    npRanges.push_back(npc2);
    npRanges.push_back(npc3);
}

int ServantTest::activateNP1(vector<Servant *> defenders)
{
    // TODO
    int ret = 0;
    return ret;
}

int ServantTest::activateNP2(vector<Servant *> defenders)
{
    // TODO
    int ret = 0;
    return ret;
}

int ServantTest::activateNP3(vector<Servant *> defenders)
{
    // TODO
    int ret = 0;
    return ret;
}
