#include "servantsaber.h"

ServantSaber::ServantSaber(int as, Team t, Logger *l) : Servant(as, t, l)
{
    maxHP.push_back(200);
    maxHP.push_back(250);
    maxHP.push_back(300);
    maxMP.push_back(100);
    maxMP.push_back(125);
    maxMP.push_back(150);
    mov.push_back(5);
    mov.push_back(5);
    mov.push_back(6);
    str.push_back(50);
    str.push_back(50);
    str.push_back(100);
    mag.push_back(30);
    mag.push_back(30);
    mag.push_back(30);
    def.push_back(40);
    def.push_back(50);
    def.push_back(50);
    res.push_back(30);
    res.push_back(30);
    res.push_back(30);
    spd.push_back(40);
    spd.push_back(40);
    spd.push_back(50);
    skl.push_back(30);
    skl.push_back(30);
    skl.push_back(30);
    luk.push_back(20);
    luk.push_back(30);
    luk.push_back(30);

    currHP = maxHP[as];
    currMP = maxMP[as];

    lowRange = 1;
    hiRange = 1;

    clss = Saber;

    // TODO: finish class declaration
}

int ServantSaber::guardianKnight(vector<Servant *> defenders)
{
    // TODO
}

int ServantSaber::manaBurst(vector<Servant *> defenders)
{
    // TODO
}

int ServantSaber::getCharismaRange()
{
    return ascension + 2;
}

// The magnitude of the Charisma buff is equal to 5% of the Saber's STR and DEF.
Debuff* ServantSaber::getCharisma()
{
    vector<Stat> buffedStats;
    vector<int> buffedAmounts;

    buffedStats.push_back(STR);
    buffedStats.push_back(DEF);
    buffedAmounts.push_back(getStr() / 20);
    buffedAmounts.push_back(getDef() / 20);

    Debuff *charBuff = new Debuff("Charisma",
                                  "Standing close to a friendly Saber gives you a boost to STR and DEF!",
                                  team, buffedStats, buffedAmounts, 1);

    return charBuff;
}
