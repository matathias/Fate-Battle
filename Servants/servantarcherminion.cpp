#include "servantarcherminion.h"
#include "PlayField.h"

ServantArcherMinion::ServantArcherMinion(int as, Team t, Logger *l) : ServantArcher(as, t, l)
{
    name = "Archer Minion";

    /** Stat modifiers **/
    int minionMod = -5;

    spd[0] += minionMod;
    spd[1] += minionMod;
    spd[2] += minionMod;
    str[0] += minionMod;
    str[1] += minionMod;
    str[2] += minionMod;
    mag[0] += minionMod;
    mag[1] += minionMod;
    mag[2] += minionMod;
    def[0] += minionMod;
    def[1] += minionMod;
    def[2] += minionMod;
    res[0] += minionMod;
    res[1] += minionMod;
    res[2] += minionMod;
    skl[0] += minionMod;
    skl[1] += minionMod;
    skl[2] += minionMod;
    luk[0] += minionMod;
    luk[1] += minionMod;
    luk[2] += minionMod;

    maxHP[0] = 0.9 * maxHP[0];
    maxHP[1] = 0.9 * maxHP[1];
    maxHP[2] = 0.9 * maxHP[2];

    maxMP[0] = 0.9 * maxMP[0];
    maxMP[1] = 0.9 * maxMP[1];
    maxMP[2] = 0.9 * maxMP[2];

    currHP = maxHP[ascension];
    currMP = maxMP[ascension];

    npRanges.push_back(getLowToHighRange(0,0));
    npRanges.push_back(getLowToHighRange(0,0));
    npRanges.push_back(getLowToHighRange(0,0));
}

int ServantArcherMinion::doAction(int actionNum, vector<Servant *> defenders)
{
    int ret = 0;
    switch (actionNum)
    {
        case 0:
            ret = attack(defenders, true);
            break;
        default:
            return 2; // Not a valid choice
            break;
    }
    return ret;
}

int ServantArcherMinion::isActionNP(int action)
{
    return -1;
}

vector<Coordinate> ServantArcherMinion::getActionRange(int action)
{
    return getLowToHighRange(getLowRange(), getHighRange());
}
