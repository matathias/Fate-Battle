#include "servantarcherpistols.h"
#include "PlayField.h"

ServantArcherPistols::ServantArcherPistols(int as, Team t, Logger *l) : ServantArcher(as, t, l)
{
    name = "Pistol Archer";

    /** Stat modifiers **/
    int sklMod = 10;
    int strMod = 10;

    skl[0] += sklMod;
    skl[1] += sklMod;
    skl[2] += sklMod;
    str[0] += strMod;
    str[1] += strMod;
    str[2] += strMod;

    lowRange += 1;
    hiRange += 1;

    actionList[0].push_back("2: NP: Sweeping Blows");
    actionList[1].push_back("2: NP: Sweeping Blows");
    actionList[1].push_back("3: NP: 360 NO SCOPE");
    actionList[2].push_back("2: NP: Sweeping Blows");
    actionList[2].push_back("3: NP: 360 NO SCOPE");
    actionList[2].push_back("4: NP: Castle Doctrine");

    actionListTypes[0].push_back(A);
    actionListTypes[1].push_back(A);
    actionListTypes[1].push_back(S);
    actionListTypes[2].push_back(A);
    actionListTypes[2].push_back(S);
    actionListTypes[2].push_back(T);

    actionMPCosts[0].push_back(25);
    actionMPCosts[1].push_back(25);
    actionMPCosts[1].push_back(75);
    actionMPCosts[2].push_back(25);
    actionMPCosts[2].push_back(75);
    actionMPCosts[2].push_back(150);

    actionDodgeable[0].push_back(true);
    actionDodgeable[1].push_back(true);
    actionDodgeable[1].push_back(false);
    actionDodgeable[2].push_back(true);
    actionDodgeable[2].push_back(false);
    actionDodgeable[2].push_back(false);

    actionCounterable[0].push_back(false);
    actionCounterable[1].push_back(false);
    actionCounterable[1].push_back(false);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);
    actionCounterable[2].push_back(false);

    vector<string> np1;
    np1.push_back("Sweeping Blows");
    np1.push_back("Costs 25 MP. Attack everyone in a 3-radius semicircle for normal damage. Accuracy and critical checks apply.");
    vector<string> np2;
    np2.push_back("360 NO SCOPE");
    np2.push_back("Costs 75 MP. Spin around and fire at a target within 6 spaces for 5 times normal damage. The absolute badass(TM) factor makes up for you not using your scope, causing this attack to always hit and making criticals twice as likely.");
    vector<string> np3;
    np3.push_back("Castle Doctrine");
    np3.push_back("At a cost of 150 MP, create a Reality Marble around the entire playing field. This field is now your home -- and any trespassers within your home deserve to be shot! All enemy units take normal damage at the beginning of their turn, and normal damage again at the end of their turn if they move. Both attacks are subject to accuracy and critical checks. Neither attack can kill. Costs 15 MP per turn to maintain.");
    noblePhantasms.push_back(np1);
    noblePhantasms.push_back(np2);
    noblePhantasms.push_back(np3);

    // Sweeping Blows range
    vector<Coordinate> npc1;
    for (int i = -3; i <= 3; i++)
    {
        for (int j = 0; j <= 3; j++)
        {
            if (abs(i) + abs(j) <= 3)
            {
                Coordinate tc;
                tc.x = i; tc.y = j;
                npc1.push_back(tc);
            }
        }
    }

    npRanges.push_back(npc1);

    // 360 NO SCOPE range
    npRanges.push_back(getLowToHighRange(getLowRange(), 6));

    // Castle Doctrine is a Reality Marble, so doesn't really have a range...
    Coordinate c;
    c.x = 0; c.y = 0;
    vector<Coordinate> npc3;
    npc3.push_back(c);
    npRanges.push_back(npc3);
}

int ServantArcherPistols::doAction(int actionNum, vector<Servant *> defenders)
{
    int ret = 0;
    switch (actionNum)
    {
        case 0:
            ret = attack(defenders, true);
            break;
        case 1:
            ret = activateNP1(defenders);
            break;
        case 2:
            ret = activateNP2(defenders);
            break;
        case 3:
            ret = activateNP3(defenders);
            break;
        default:
            return 2; // Not a valid choice
            break;
    }
    return ret;
}

int ServantArcherPistols::isActionNP(int action)
{
    return (action - 1);
}

bool ServantArcherPistols::isActionRealityMarble(int action)
{
    return (action == 3); // Castle Doctrine is action 3
}

int ServantArcherPistols::getRealityMarbleMP()
{
    return 15;
}

vector<Coordinate> ServantArcherPistols::getActionRange(int action)
{
    vector<Coordinate> range;

    if (action == 0)
    {
        range = getLowToHighRange(getLowRange(), getHighRange());
    }
    else
    {
        range = getNPRange(action - 1);
    }

    return range;
}

int ServantArcherPistols::attack(vector<Servant *> defenders, bool counter)
{
    if (actionMPCosts[ascension][0] > currMP && !realityMarbleActive)
        return 1; // Not enough MP to attack
    else
    {
        // If Castle Doctrine is active, the normal attack does not cost MP
        if (!realityMarbleActive)
            subMP(actionMPCosts[ascension][0]);

        for (unsigned int i = 0; i < defenders.size(); i++)
        {
            // Attack three times at 40% damage each
            bool enemyDead = false;
            for (int j = 0; j < 3 && !enemyDead; j++)
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
                    double attackMult = 0.4;
                    int critChance = capZero(getCriticalRate() -
                                     defenders[i]->getCriticalEvade());
                    r = getRandNum();
                    if (critChance >= r)
                        attackMult *= 3;

                    // Deal the damage
                    dam = (int) (capZero(getStr() - defenders[i]->getDef()) * attackMult);
                    log->addToEventLog(getFullName() + " dealt " +
                                       to_string(dam) + " damage to " +
                                       defenders[i]->getFullName() + ".");
                    defenders[i]->subHP(dam, D_STR);
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
                            subHP(dam, C_STR);
                        }
                    }
                    // Call "attack" on the defending servant for their
                    // counterattack, if you are in their range and you are the
                    // initiating servant.
                    if (defenders[i]->isInRange(this) && counter && j == 2)
                    {
                        vector<Servant *> you;
                        you.push_back(this);
                        defenders[i]->attack(you, false);
                    }
                }
                else
                {
                    enemyDead = true;
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
    }

    return 0;
}

// Sweeping Blows
int ServantArcherPistols::activateNP1(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][1] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][1]);
        log->addToEventLog(getFullName() + " used Sweeping Blows!");

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
                dam = capZero(getStr() - defenders[i]->getDef()) * attackMult;
                log->addToEventLog(getFullName() + " dealt " +
                                   to_string(dam) + " damage to " +
                                   defenders[i]->getFullName() + ".");
                defenders[i]->subHP(dam, NP_STR);
            }
            else
            {
                log->addToEventLog(getFullName() + " missed " +
                                   defenders[i]->getFullName() + "!");
            }

            // Check to see if the defender is dead. If they are and they are an
            // Avenger, activate Final Revenge.
            if(defenders[i]->getCurrHP() <= 0)
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

// 360 NO SCOPE
int ServantArcherPistols::activateNP2(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][2] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][2]);
        log->addToEventLog(getFullName() + " used 360 NO SCOPE!");

        for (unsigned int i = 0; i < defenders.size(); i++)
        {
            int dam = 0;
            // 360 NO SCOPE always hits, so no accuracy check
            int attackMult = 5;
            int critChance = capZero(getCriticalRate() -
                             defenders[i]->getCriticalEvade());
            int r = getRandNum();
            if ((critChance * 2) >= r)
                attackMult *= 3;

            // Deal the damage
            dam = capZero(getStr() - defenders[i]->getDef()) * attackMult;
            log->addToEventLog(getFullName() + " dealt " +
                               to_string(dam) + " damage to " +
                               defenders[i]->getFullName() + ".");
            defenders[i]->subHP(dam, NP_STR);

            // Check to see if the defender is dead. If they are and they are an
            // Avenger, activate Final Revenge.
            if(defenders[i]->getCurrHP() <= 0)
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

// Castle Doctrine
int ServantArcherPistols::activateNP3(vector<Servant *> defenders)
{
    if (actionMPCosts[ascension][3] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][3]);
        realityMarbleActive = true;

        // Castle Doctrine "Debuff":
        vector<Stat> tstats;
        tstats.push_back(MOV);
        vector<int> tAmounts;
        tAmounts.push_back(0);
        Debuff *castleDoctrine = new Debuff("Reality Marble", "Castle Doctrine",
                                            team, tstats, tAmounts, -1);

        field->startRealityMarble(this, castleDoctrine);

        log->addToEventLog(getFullName() + " activated a Reality Marble! Castle Doctrine is now in effect!");
    }

    return 0;
}
