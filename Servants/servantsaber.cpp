#include "servantsaber.h"

ServantSaber::ServantSaber(int as, Team t, Logger *l) : Servant(as, t, l)
{
    maxHP.push_back(250);
    maxHP.push_back(300);
    maxHP.push_back(350);
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
    def.push_back(30);
    def.push_back(40);
    def.push_back(40);
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
    name = "Saber";

    vector<string> basicActions;
    basicActions.push_back("1: Attack");
    basicActions.push_back("2: Guardian Knight");
    basicActions.push_back("3: Mana Burst");
    actionList.push_back(basicActions);
    actionList.push_back(basicActions);
    actionList.push_back(basicActions);
    vector<ActionType> actionTypes;
    actionTypes.push_back(S);
    actionTypes.push_back(A);
    actionTypes.push_back(N);
    actionListTypes.push_back(actionTypes);
    actionListTypes.push_back(actionTypes);
    actionListTypes.push_back(actionTypes);
    vector<int> actionCosts;
    actionCosts.push_back(2);
    actionCosts.push_back(20);
    actionCosts.push_back(20);
    actionMPCosts.push_back(actionCosts);
    actionMPCosts.push_back(actionCosts);
    actionMPCosts.push_back(actionCosts);

    vector<bool> acDodge;
    acDodge.push_back(true);
    acDodge.push_back(true);
    acDodge.push_back(true);
    actionDodgeable.push_back(acDodge);
    actionDodgeable.push_back(acDodge);
    actionDodgeable.push_back(acDodge);
    vector<bool> acCounter;
    acCounter.push_back(true);
    acCounter.push_back(false);
    acCounter.push_back(true);
    actionCounterable.push_back(acCounter);
    actionCounterable.push_back(acCounter);
    actionCounterable.push_back(acCounter);

    /** Passive Skill modifiers **/
    vector<Stat> hdS;
    hdS.push_back(MOV);
    vector<int> hdA;
    hdA.push_back(0);
    Debuff *highDivinity = new Debuff("Charisma", "Passive Skill",
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
int ServantSaber::guardianKnight(vector<Servant *> defenders)
{
    // Check to make sure Guardian Knight isn't already active, or that Mana
    //  Burst isn't currently active.
    if (guardianKnightActive())
    {
        log->addToErrorLog("Guardian Knight is already active!");

        QMessageBox messageBox;
        messageBox.setWindowTitle("Final Fate");
        messageBox.setText("Guardian Knight is already active!");
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.setDefaultButton(QMessageBox::Ok);
        messageBox.exec();

        return 41;
    }
    else if (manaBurstActive())
    {
        log->addToErrorLog("Cannot use Guardian Knight while Mana Burst is active!");

        QMessageBox messageBox;
        messageBox.setWindowTitle("Final Fate");
        messageBox.setText("Cannot use Guardian Knight while Mana Burst is active!");
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.setDefaultButton(QMessageBox::Ok);
        messageBox.exec();

        return 41;
    }

    // Add buff to this Saber
    vector<Stat> gvs;
    gvs.push_back(MOV);
    gvs.push_back(SPD);
    gvs.push_back(DEF);
    vector<int> gvi;
    gvi.push_back(-3);
    gvi.push_back(-1 * (getSpd() / 2));
    gvi.push_back((getDef() * 3) / 4);
    Debuff *guardian = new Debuff("Guardian Knight",
                                  "You guard your allies with your life.",
                                  team, gvs, gvi, 3);
    addDebuff(guardian);

    // See which of the defenders resist provoke. If they fail, add a "Provoked"
    //  debuff.
    for (unsigned int i = 0; i < defenders.size(); i++)
    {
        // Don't provoke members of your own team!
        if (defenders[i]->getTeam() == getTeam())
            continue;

        // Check against defender's (SKL / 2 + LUK / 2)
        int resistChance = (defenders[i]->getSkl() / 2) +
                           (defenders[i]->getLuk() / 2);
        if (resistChance < defenders[i]->getRandNum())
        {
            vector<Stat> vs;
            vs.push_back(MOV);
            vector<int> vi;
            vi.push_back(0);
            Debuff *provokedDebuff = new Debuff("Provoked",
                                                "You have been provoked by an enemy Saber!",
                                                defenders[i]->getTeam(),
                                                vs, vi, 3);
            defenders[i]->addDebuff(provokedDebuff);
            log->addToEventLog(defenders[i]->getFullName() + " was provoked by "
                               + getFullName());
        }
        else
            log->addToEventLog(defenders[i]->getFullName() + " resisted provoke.");
    }

    log->addToEventLog(getFullName() + " activated Guardian Knight!");

    return 0;
}

int ServantSaber::manaBurst(vector<Servant *> defenders)
{
    // Check to make sure that Mana Burst isn't already active, or that Guardian
    //  Knight isn't currently active.
    if (guardianKnightActive())
    {
        log->addToErrorLog("Cannot use Mana Burst while Guardian Knight is active!");

        QMessageBox messageBox;
        messageBox.setWindowTitle("Final Fate");
        messageBox.setText("Cannot use Mana Burst while Guardian Knight is active!");
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.setDefaultButton(QMessageBox::Ok);
        messageBox.exec();

        return 41;
    }
    else if (manaBurstActive())
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

    // Add buff to this Saber
    vector<Stat> gvs;
    gvs.push_back(STR);
    gvs.push_back(SPD);
    gvs.push_back(DEF);
    vector<int> gvi;
    gvi.push_back(getStr() / 2);
    gvi.push_back(getSpd() / 10);
    gvi.push_back(-1 * ((getDef() * 6) / 10));
    Debuff *guardian = new Debuff("Mana Burst",
                                  "You sacrifice defense to gain incredible power.",
                                  team, gvs, gvi, 3);
    addDebuff(guardian);

    log->addToEventLog(getFullName() + " activated Mana Burst!");

    return 0;
}

/***** Passive Skills *****/
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

/***** Retrievers and Helpers *****/
bool ServantSaber::guardianKnightActive()
{
    bool ret = false;
    for (unsigned int i = 0; i < debuffs.size(); i++)
    {
        if(debuffs[i]->getDebuffName().compare("Guardian Knight") == 0)
            ret = true;
    }

    return ret;
}

bool ServantSaber::manaBurstActive()
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
vector<int> ServantSaber::getEvade()
{
    // Evasion = Speed * 2 + Luck
    vector<int> evade;
    evade.push_back(getInitialEvade());
    // Eye of the Mind: (SKL / 4) chance of taking no damage.
    evade.push_back(getSkl() / 4);
    return evade;
}

int ServantSaber::isActionNP(int action)
{
    int ret = -1;
    switch(action)
    {
        case 3:
            ret = 0;
            break;
        case 4:
            ret = 1;
            break;
        case 5:
            ret = 2;
            break;
        default:
            break;
    }

    return ret;
}

vector<Coordinate> ServantSaber::getActionRange(int action)
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
    else if (action == 1) // Guardian Knight range
    {
        for (int i = -3; i <= 3; i++)
        {
            for (int j = -3; j <= 3; j++)
            {
                if (abs(i) + abs(j) >= 1 && abs(i) + abs(j) <= 3)
                {
                    Coordinate inRange;
                    inRange.x = i;
                    inRange.y = j;
                    range.push_back(inRange);
                }
            }
        }
    }
    else if (action == 2) // Mana Burst Range
    {
        Coordinate inRange;
        inRange.x = 0;
        inRange.y = 0;
        range.push_back(inRange);
    }
    else
    {
        range = getNPRange(action-3);
    }

    return range;
}
