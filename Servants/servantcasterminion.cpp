#include "servantcasterminion.h"
#include "PlayField.h"

ServantCasterMinion::ServantCasterMinion(int as, Team t, Logger *l) : ServantCaster(as, t, l)
{
    name = "Caster Minion";

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

int ServantCasterMinion::isActionNP(int action)
{
    return -1;
}

vector<Coordinate> ServantCasterMinion::getActionRange(int action)
{
    vector<Coordinate> range;
    Coordinate servLoc;
    servLoc.x = 0; servLoc.y = 0;

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
        range.push_back(servLoc);
    }
    else if (action == 3) // Cast From Life
    {
        range.push_back(servLoc);
    }
    else // Heal All
    {
        range.push_back(servLoc);
    }

    return range;
}

int ServantCasterMinion::doAction(int actionNum, vector<Servant *> defenders)
{
    int ret = 0;

    switch (actionNum)
    {
        case 0:
            ret = attack(defenders, true);
            break;
        case 1:
            ret = heal(defenders);
            break;
        case 2:
            ret = resurrect(defenders);
            break;
        case 3:
            ret = castFromLife();
            break;
        case 4:
            ret = healAll();
            break;
        default:
            return 2; // Not a valid choice
            break;
    }

    return ret;
}
