#include "servantcasternecromancy.h"
#include "PlayField.h"

ServantCasterNecromancy::ServantCasterNecromancy(int as, Team t, Logger *l) : ServantCaster(as, t, l)
{
    name = "Necromancer Caster";

    deathSealCreated = false;
    deathSealCost = 150;

    int baneOfLifeCost = 150;
    int harbingerOfDeathCost = 500;

    /** Stat modifiers **/
    int resMod = 5;
    int defMod = 10;
    int spdMod = -10;
    double hpMod = 1.1;

    res[0] += resMod;
    res[1] += resMod;
    res[2] += resMod;

    def[0] += defMod;
    def[1] += defMod;
    def[2] += defMod;

    spd[0] += spdMod;
    spd[1] += spdMod;
    spd[2] += spdMod;

    maxHP[0] *= hpMod;
    maxHP[1] *= hpMod;
    maxHP[2] *= hpMod;

    currHP = maxHP[as];

    // Action List
    actionList[0].push_back("5: Item Construction - Death Seal");
    actionList[0].push_back("6: Territory Creation - Essencesap");
    actionList[0].push_back("7: Necroally");
    actionList[0].push_back("8: NP: Bane of Life");

    actionList[1].push_back("5: Item Construction - Death Seal");
    actionList[1].push_back("6: Territory Creation - Essencesap");
    actionList[1].push_back("7: Necroally");
    actionList[1].push_back("8: NP: Bane of Life");
    actionList[1].push_back("9: NP: Harbinger of Death");

    actionList[2].push_back("6: Item Construction - Death Seal");
    actionList[2].push_back("7: Territory Creation - Essencesap");
    actionList[2].push_back("8: Necroally");
    actionList[2].push_back("9: NP: Bane of Life");
    actionList[2].push_back("10: NP: Harbinger of Death");
    actionList[2].push_back("11: NP: Commander of Life and Death");

    // Action List Types
    actionListTypes[0].push_back(N);
    actionListTypes[0].push_back(T);
    actionListTypes[0].push_back(D);
    actionListTypes[0].push_back(A);

    actionListTypes[1].push_back(N);
    actionListTypes[1].push_back(T);
    actionListTypes[1].push_back(D);
    actionListTypes[1].push_back(A);
    actionListTypes[1].push_back(A);

    actionListTypes[2].push_back(N);
    actionListTypes[2].push_back(T);
    actionListTypes[2].push_back(D);
    actionListTypes[2].push_back(A);
    actionListTypes[2].push_back(A);
    actionListTypes[2].push_back(T);

    // Action MP Costs
    actionMPCosts[0].push_back(deathSealCost);
    actionMPCosts[0].push_back(maxMP[0] / 5);
    actionMPCosts[0].push_back(25);
    actionMPCosts[0].push_back(baneOfLifeCost);

    actionMPCosts[1].push_back(deathSealCost);
    actionMPCosts[1].push_back(maxMP[1] / 5);
    actionMPCosts[1].push_back(25);
    actionMPCosts[1].push_back(baneOfLifeCost);
    actionMPCosts[1].push_back(harbingerOfDeathCost);

    actionMPCosts[2].push_back(deathSealCost);
    actionMPCosts[2].push_back(maxMP[2] / 5);
    actionMPCosts[2].push_back(25);
    actionMPCosts[2].push_back(baneOfLifeCost);
    actionMPCosts[2].push_back(harbingerOfDeathCost);
    actionMPCosts[2].push_back(900);

    // Action Dodgeable?
    actionDodgeable[0].push_back(false);
    actionDodgeable[0].push_back(false);
    actionDodgeable[0].push_back(false);
    actionDodgeable[0].push_back(false);

    actionDodgeable[1].push_back(false);
    actionDodgeable[1].push_back(false);
    actionDodgeable[1].push_back(false);
    actionDodgeable[1].push_back(false);
    actionDodgeable[1].push_back(false);

    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(false);

    // Action Counterable?
    actionCounterable[0].push_back(false);
    actionCounterable[0].push_back(false);
    actionCounterable[0].push_back(false);
    actionCounterable[0].push_back(false);

    actionCounterable[1].push_back(false);
    actionCounterable[1].push_back(false);
    actionCounterable[1].push_back(false);
    actionCounterable[1].push_back(false);
    actionCounterable[1].push_back(false);

    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);

    vector<string> np1;
    np1.push_back("Bane of Life");
    np1.push_back("At a cost of 150 MP, curse all enemy units within three spaces. If a cursed unit dies within three turns, then they cannot be revived.");
    vector<string> np2;
    np2.push_back("Harbinger of Death");
    np2.push_back("At a cost of 500 MP, curse all enemy units within four spaces. If the Caster does not die in the next three turns, all cursed units will be instantly killed if they do not pass a LUK check. Any units killed this way cannot be revived.");
    vector<string> np3;
    np3.push_back("Commander of Life and Death");
    np3.push_back("At a cost of 900 MP and 30 HP (25 MP per turn to maintain), cast a Reality Marble to summon the terrors of Hell itself. All enemy units lose .1*MAXHP per turn while friendly units lose .05*MAXHP per turn (this effect cannot kill). Any unit that dies while the Reality Marble is active cannot be revived, be they friend or foe. At the beginning of each turn the Reality Marble is active, the Caster can choose a target unit. Said unit must then pass a LUK check (LUK*2); if they fail, they are reduced to 1 HP. If they are already at 1 HP, they die. This effect surpasses the Lancer’s Battle Continuation skill.");
    noblePhantasms.push_back(np1);
    noblePhantasms.push_back(np2);
    noblePhantasms.push_back(np3);

    // Bane of Life Range
    npRanges.push_back(getLowToHighRange(1,3));

    // Harbinger of Death Range
    npRanges.push_back(getLowToHighRange(1, 4));

    // Commander of Life and Death
    // It's a reality marble. The range doesn't matter.
    Coordinate tc;
    tc.x = 0; tc.y = 0;
    vector<Coordinate> npr;
    npr.push_back(tc);
    npRanges.push_back(npr);
}

/***** Active Skills *****/
int ServantCasterNecromancy::necroally(vector<Servant *> defenders)
{
    if ((ascension == 2 && actionMPCosts[ascension][7] > currMP) ||
        (ascension != 2 && actionMPCosts[ascension][6] > currMP))
        return 1; // Not enough MP

    // Double the MP cost
    actionMPCosts[ascension][2] *= 2;
    ressurectCount++;

    if (ascension == 2)
    {
        subMP(actionMPCosts[ascension][7]);
        actionMPCosts[ascension][7] = actionMPCosts[ascension][2] / 2;
    }
    else
    {
        subMP(actionMPCosts[ascension][6]);
        actionMPCosts[ascension][6] = actionMPCosts[ascension][2] / 2;
    }

    for (unsigned int i = 0; i < defenders.size(); i++)
    {
        defenders[i]->setHP((int) (defenders[i]->getMaxHP() * 0.8));

        // Apply the necroally debuff to lower def and raise res
        vector<Stat> tStats;
        tStats.push_back(DEF);
        tStats.push_back(RES);
        vector<int> tAmounts;
        tAmounts.push_back(-1 * (defenders[i]->getDef() / 2));
        tAmounts.push_back(defenders[i]->getRes());

        Debuff *necro = new Debuff("Necroally",
                                   "You were revived through Necromancy.",
                                   defenders[i]->getTeam(), tStats, tAmounts,
                                   -1);

        defenders[i]->addDebuff(necro);

        log->addToEventLog(getFullName() + " used Necroally on " +
                           defenders[i]->getFullName() + "!");
    }

    return 0;
}

int ServantCasterNecromancy::itemConstruction()
{
    if ((ascension == 2 && actionMPCosts[ascension][5] > currMP) ||
        (ascension != 2 && actionMPCosts[ascension][4] > currMP))
        return 1; // Not enough MP

    if (deathSealCreated)
        return 41;

    if (ascension == 2)
        subMP(actionMPCosts[ascension][5]);
    else
        subMP(actionMPCosts[ascension][4]);

    deathSealCreated = true;
    deathSealCost *= 4;

    log->addToEventLog(getFullName() + " created a Death Seal!");

    // Modify the action lists
    if (ascension != 2)
    {
        actionList[ascension][4] = "5: Use Death Seal";
        actionListTypes[ascension][4] = D;
        actionMPCosts[ascension][4] = 0;
        actionDodgeable[ascension][4] = false;
        actionCounterable[ascension][4] = false;
    }
    else
    {
        actionList[ascension][5] = "6: Use Death Seal";
        actionListTypes[ascension][5] = D;
        actionMPCosts[ascension][5] = 0;
        actionDodgeable[ascension][5] = false;
        actionCounterable[ascension][5] = false;
    }

    return 0;
}

int ServantCasterNecromancy::deathSeal(vector<Servant *> defenders)
{
    if (!deathSealCreated)
        return 41;

    log->addToEventLog(getFullName() + " used their Death Seal!");

    for (unsigned int i = 0; i < defenders.size(); i++)
    {
        // Get the target's stats and add them to the Necromancer in the form of
        //  a buff
        string nDebDes = "You used Death Seal on " + defenders[i]->getFullName();
        vector<Stat> nStats;
        nStats.push_back(MAXHP);
        nStats.push_back(MAXMP);
        nStats.push_back(STR);
        nStats.push_back(MAG);
        nStats.push_back(DEF);
        nStats.push_back(RES);
        nStats.push_back(SPD);
        nStats.push_back(SKL);
        nStats.push_back(LUK);
        vector<int> nAmounts;
        nAmounts.push_back(defenders[i]->getMaxHP() / 20);
        nAmounts.push_back(defenders[i]->getMaxMP() / 20);
        nAmounts.push_back(defenders[i]->getStr() / 20);
        nAmounts.push_back(defenders[i]->getMag() / 20);
        nAmounts.push_back(defenders[i]->getDef() / 20);
        nAmounts.push_back(defenders[i]->getRes() / 20);
        nAmounts.push_back(defenders[i]->getSpd() / 20);
        nAmounts.push_back(defenders[i]->getSkl() / 20);
        nAmounts.push_back(defenders[i]->getLuk() / 20);
        Debuff *nBonus = new Debuff("Death Seal Bonus", nDebDes, team, nStats,
                                    nAmounts, -1);
        addDebuff(nBonus);

        // add Permadeath to the target
        vector<Stat> pStats;
        pStats.push_back(MOV);
        vector<int> pAmounts;
        pAmounts.push_back(0);
        Debuff *perma = new Debuff("Permadeath",
                                   "You have been permanetly killed.",
                                   defenders[i]->getTeam(), pStats, pAmounts,
                                   -1);

        defenders[i]->addDebuff(perma);
        log->addToEventLog(defenders[i]->getFullName() + " is now permanetly dead!");
    }

    deathSealCreated = false;

    // Modify the action lists
    if (ascension != 2)
    {
        actionList[ascension][4] = "5: Item Construction - Death Seal";
        actionListTypes[ascension][4] = N;
        actionMPCosts[ascension][4] = deathSealCost;
        actionDodgeable[ascension][4] = false;
        actionCounterable[ascension][4] = false;
    }
    else
    {
        actionList[ascension][5] = "6: Item Construction - Death Seal";
        actionListTypes[ascension][5] = N;
        actionMPCosts[ascension][5] = deathSealCost;
        actionDodgeable[ascension][5] = false;
        actionCounterable[ascension][5] = false;
    }

    return 0;
}

int ServantCasterNecromancy::territoryCreation(vector<Servant *> defenders)
{
    if ((ascension == 2 && actionMPCosts[ascension][6] > currMP) ||
        (ascension != 2 && actionMPCosts[ascension][5] > currMP))
        return 1; // Not enough MP

    if (territoryActive)
        return 41;

    if (ascension == 2)
        subMP(actionMPCosts[ascension][6]);
    else
        subMP(actionMPCosts[ascension][5]);

    vector<Coordinate> range = getLowToHighRange(0, 2 + ascension);

    // Create the Debuff
    vector<Stat> tStats;
    tStats.push_back(MP);
    vector<int> tAmounts;
    tAmounts.push_back(-25);
    // Get the Team of the opposing team
    Team otherTeam = All;
    vector<vector<Servant*>> pField = field->getServantLocations();
    for (unsigned int i = 0; i < pField.size(); i++)
    {
        for(unsigned int j = 0; j < pField[i].size(); j++)
        {
            if(pField[i][j] != NULL && pField[i][j]->getTeam() != getTeam())
            {
                otherTeam = pField[i][j]->getTeam();
            }
        }
    }

    Debuff *tEssence = new Debuff("Territory", "Essencesap",
                                 otherTeam, tStats,
                                 tAmounts, -1);

    // Start the territory
    field->startTerritory(this, tEssence, range);

    territoryActive = true;

    log->addToEventLog(getFullName() + " created an Essencesap territory!");

    return 0;
}

/***** Re-defined Functions *****/
int ServantCasterNecromancy::getTerritoryMP()
{
    return (getMaxMP() / 20);
}

string ServantCasterNecromancy::getTerritoryName()
{
    return "Essencesap";
}

int ServantCasterNecromancy::getRealityMarbleMP()
{
    return 25;
}

int ServantCasterNecromancy::isActionNP(int action)
{
    int ret = -1;
    if (ascension != 2)
    {
        if (action >= 7)
            ret = action - 7;
    }
    else
    {
        if (action >= 8)
            ret = action - 8;
    }

    return ret;
}

bool ServantCasterNecromancy::isActionRealityMarble(int action)
{
    return (ascension == 2 && action == 10);
}

vector<Coordinate> ServantCasterNecromancy::getActionRange(int action)
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
    else if (ascension == 2) // Final Ascension
    {
        if (action == 4) // Heal All
        {
            range.push_back(servLoc);
        }
        else if (action == 5) // Item Construction and Death Seal
        {
            range.push_back(servLoc);
        }
        else if (action == 6) // Territory Creation
        {
            range = getLowToHighRange(0, 2 + ascension);
        }
        else if (action == 7) // Necroally
        {
            range.push_back(servLoc);
        }
        else // Noble Phantasms
        {
            range = getNPRange(action-8);
        }
    }
    else // First or Second Ascension
    {
        if (action == 4) // Item Construction and Death Seal
        {
            range.push_back(servLoc);
        }
        else if (action == 5) // Territory Creation
        {
            range = getLowToHighRange(0, 2 + ascension);
        }
        else if (action == 6) // Necroally
        {
            range.push_back(servLoc);
        }
        else // Noble Phantasms
        {
            range = getNPRange(action-7);
        }
    }

    return range;
}

int ServantCasterNecromancy::doAction(int actionNum, vector<Servant *> defenders)
{
    int ret = 0;
    if (ascension != 2)
    {
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
                if (deathSealCreated)
                    ret = deathSeal(defenders);
                else
                    ret = itemConstruction();
                break;
            case 5:
                ret = territoryCreation(defenders);
                break;
            case 6:
                ret = necroally(defenders);
                break;
            case 7:
                ret = activateNP1(defenders);
                break;
            case 8:
                ret = activateNP2(defenders);
                break;
            default:
                return 2; // Not a valid choice
                break;
        }
    }
    else
    {
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
            case 5:
                if (deathSealCreated)
                    ret = deathSeal(defenders);
                else
                    ret = itemConstruction();
                break;
            case 6:
                ret = territoryCreation(defenders);
                break;
            case 7:
                ret = necroally(defenders);
                break;
            case 8:
                ret = activateNP1(defenders);
                break;
            case 9:
                ret = activateNP2(defenders);
                break;
            case 10:
                ret = activateNP3(defenders);
                break;
                return 2; // Not a valid choice
                break;
        }
    }
    return ret;
}

/***** Noble Phantasms *****/
// Bane of Life
int ServantCasterNecromancy::activateNP1(vector<Servant *> defenders)
{
    if ((ascension == 2 && actionMPCosts[ascension][8] > currMP) ||
        (ascension != 2 && actionMPCosts[ascension][7] > currMP))
        return 1; // Not enough MP

    if (ascension == 2)
        subMP(actionMPCosts[ascension][8]);
    else
        subMP(actionMPCosts[ascension][7]);

    log->addToEventLog(getFullName() + " cast Bane of Life!");

    for (unsigned int i = 0; i < defenders.size(); i++)
    {
        // curse the defender
        vector<Stat> tDebStat;
        tDebStat.push_back(MOV);
        vector<int> tDebAm;
        tDebAm.push_back(0);
        Debuff *newDebuff = new Debuff("Cursed",
                                       "If you die while this debuff is active, you cannot be ressurected.",
                                       defenders[i]->getTeam(),
                                       tDebStat, tDebAm, 3);

        defenders[i]->addDebuff(newDebuff);

        log->addToEventLog(defenders[i]->getFullName() + " was cursed!");
    }

    return 0;
}

// Harbinger of Death
int ServantCasterNecromancy::activateNP2(vector<Servant *> defenders)
{
    if ((ascension == 2 && actionMPCosts[ascension][9] > currMP) ||
        (ascension != 2 && actionMPCosts[ascension][8] > currMP))
        return 1; // Not enough MP

    if (ascension == 2)
        subMP(actionMPCosts[ascension][9]);
    else
        subMP(actionMPCosts[ascension][8]);

    log->addToEventLog(getFullName() + " cast Harbinger of Death!");

    for (unsigned int i = 0; i < defenders.size(); i++)
    {
        // curse the defender
        vector<Stat> tDebStat;
        tDebStat.push_back(MOV);
        vector<int> tDebAm;
        tDebAm.push_back(0);
        Debuff *newDebuff = new Debuff("Doom",
                                       "Your days are numbered...",
                                       defenders[i]->getTeam(),
                                       tDebStat, tDebAm, 4);

        defenders[i]->addDebuff(newDebuff);
        addCastedDebuff(newDebuff);

        log->addToEventLog(defenders[i]->getFullName() + " was cursed with Doom!");
    }

    return 0;
}

// Commander of Life and Death
int ServantCasterNecromancy::activateNP3(vector<Servant *> defenders)
{
    if ((ascension == 2 && actionMPCosts[ascension][10] > currMP) ||
        (ascension != 2 && actionMPCosts[ascension][9] > currMP))
        return 1; // Not enough MP

    if (ascension == 2)
        subMP(actionMPCosts[ascension][10]);
    else
        subMP(actionMPCosts[ascension][9]);

    // Also has an HP cost!
    int hpCost = getMaxHP() / 5;
    if (getCurrHP() - hpCost <= 0)
        setHP(1);
    else
        subHP(hpCost, OMNI);

    // Ignore the defenders list, we want to process ALL servants
    // Also use this opportunity to get the opposing team
    Team otherTeam = All;
    vector<vector<Servant*>> allServ = field->getServantLocations();
    for(unsigned int i = 0; i < field->getFieldWidth(); i++)
    {
        for (unsigned int j = 0; j < field->getFieldLength(); j++)
        {
            if (allServ[i][j] != NULL)
            {
                int hpLoss = 0;
                if (allServ[i][j]->getTeam() == getTeam())
                    hpLoss = allServ[i][j]->getMaxHP() * 0.2;
                else
                {
                    hpLoss = allServ[i][j]->getMaxHP() * 0.4;
                    otherTeam = allServ[i][j]->getTeam();
                }

                if (hpLoss >= allServ[i][j]->getCurrHP())
                    hpLoss = allServ[i][j]->getCurrHP() - 1;

                allServ[i][j]->subHP(hpLoss, OMNI);

                // Add a permadeath debuff to the servant
                vector<Stat> tDebStat;
                tDebStat.push_back(MOV);
                vector<int> tDebAm;
                tDebAm.push_back(0);
                Debuff *newDebuff = new Debuff("Cursed",
                                               "If you die while this debuff is active, you cannot be ressurected.",
                                               allServ[i][j]->getTeam(),
                                               tDebStat, tDebAm, 1);
                allServ[i][j]->addDebuff(newDebuff);
            }
        }
    }

    // Activate the actual Reality Marble
    realityMarbleActive = true;

    // Commander of Life and Death "Debuff":
    vector<Stat> tstats;
    tstats.push_back(MOV);
    vector<int> tAmounts;
    tAmounts.push_back(0);
    Debuff *commander = new Debuff("Reality Marble",
                                   "Commander of Life and Death",
                                   otherTeam, tstats, tAmounts, -1);

    field->startRealityMarble(this, commander);

    log->addToEventLog(getFullName() + " activated a Reality Marble! Commander of Life and Death is now in effect!");

    return 0;
}
