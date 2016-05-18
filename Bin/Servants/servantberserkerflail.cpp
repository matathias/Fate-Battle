#include "servantberserkerflail.h"
#include "PlayField.h"

ServantBerserkerFlail::ServantBerserkerFlail(int as, Team t, Logger *l) : ServantBerserker(as, t, l)
{
    name = "Flail Berserker";

    /*** Stat Modifiers ***/
    hiRange += 1;

    int spdMod = 3;
    spd[0] += spdMod;
    spd[1] += spdMod;
    spd[2] += spdMod;

    actionListTypes[2][0] = A;
    actionCounterable[2][0] = false;

    vector<string> np1;
    np1.push_back("Lashing Out");
    np1.push_back("Passive Noble Phantasm. With a (LUK) chance your flail will sometimes hit all adjacent opponents for full damage. No accuracy or critical check. All adjacent squares will also receive the “Cratered” debuff which gives +2 DEF but -5 SPD to any servant standing on the space. This debuff lasts until erased by a territory or Reality Marble.");
    vector<string> np2;
    np2.push_back("Spin To Win");
    np2.push_back("Passive Noble Phantasm. Your constant thrashing and brandishing of the flail produces a spin that is very difficult to break through. When attacked, have a (LUK) chance of taking no damage. Additionally, your crit chance is increased threefold -- as everyone knows that spinning is better than not spinning.");
    vector<string> np3;
    np3.push_back("Maelstrom of Death");
    np3.push_back("Passive Noble Phantasm. Your lashing and whirling turn you into a massive Top of Carnage; your basic attack transforms into an AOE attack that hits everyone within two squares of you for no extra MP. The chance of creating craters from Lashing Out is increased to (LUK * 1.5), and the craters cover every space with two squares of you. Any targets have a half chance of dodging and criticals are twice as likely (this stacks with Spin to Win). Additionally, the LUK check for Spin To Win is increased to (LUK*1.5).");
    noblePhantasms.push_back(np1);
    noblePhantasms.push_back(np2);
    noblePhantasms.push_back(np3);

    Coordinate c1;
    c1.x = 0; c1.y = 0;
    vector<Coordinate> npc1;
    npc1.push_back(c1);
    vector<Coordinate> npc2;
    npc2.push_back(c1);
    vector<Coordinate> npc3;
    npc3.push_back(c1);
    npRanges.push_back(npc1);
    npRanges.push_back(npc2);
    npRanges.push_back(npc3);
}

/***** Function Re-definitions *****/

int ServantBerserkerFlail::attack(vector<Servant *> defenders, bool counter)
{
    if (actionMPCosts[ascension][0] > currMP)
        return 1; // Not enough MP to attack
    else
    {
        subMP(actionMPCosts[ascension][0]);
        bool npActive = false;

        // Check if Lashing Out activates
        // "Hitting all adjacent opponents" only applies to the first two
        //  ascension levels, as Maelstrom of Death trumps Lashing Out
        int r = getRandNum();

        // Cratered Debuff
        vector<Stat> tS;
        tS.push_back(SPD);
        tS.push_back(DEF);
        vector<int> tA;
        tA.push_back(-5);
        tA.push_back(2);
        // Get the Team of the opposing team
        /*Team otherTeam = All;
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
        }*/
        Debuff* br = new Debuff("Cratered", "The ground has been cratered.",
                                All, tS, tA, -1);

        vector<Coordinate> crateredRange;

        if (ascension <= 1 && r < getLuk())
        {
            // Add all adjacent servants to the defenders vector (as long as
            //  they aren't already in it)
            vector<Servant*> adj = field->getAllInRange(this, getLowToHighRange(1,1));
            for (unsigned int i = 0; i < adj.size(); i++)
            {
                bool isIn = false;
                for (unsigned int j = 0; j < defenders.size() && !isIn; j++)
                {
                    if (adj[i] == defenders[j])
                        isIn = true;
                }

                if (!isIn)
                    defenders.push_back(adj[i]);
            }

            crateredRange = getAbsoluteRange(1,1,this);
        }
        else if (ascension == 2 && r < getLuk() * 1.5)
        {
            crateredRange = getAbsoluteRange(1,2,this);
        }

        // Apply the cratered debuff to the appropriate spaces
        field->addDebuffToRange(br, crateredRange);

        for (unsigned int i = 0; i < defenders.size(); i++)
        {
            int dam = 0;
            // Check if you hit the targets
            vector<int> opEvade = defenders[i]->getEvade();
            bool hit = false;
            // Calculate accuracy
            int accuracy = capZero(getHitRate() - opEvade[0]);

            r = getRandNum();
            if (accuracy >= r)
                hit = true;

            // Maelstrom of Death doubles accuracy and prevents other evasion
            //  skills from activating
            if (ascension == 2)
                accuracy *= 2;

            if (opEvade.size() > 1 && hit && ascension <= 1)
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

                // Spin To Win triples crit chance
                if (ascension >= 1)
                    critChance *= 3;

                // Maelstrom of Death also doubles crit chance
                if (ascension == 2)
                    critChance *= 2;

                r = getRandNum();
                if (critChance >= r)
                    attackMult *= 3;

                // Deal the damage
                dam = capZero(getStr() - defenders[i]->getDef()) * attackMult;
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
                                           "'s Mad Counter activated, dealing " +
                                           to_string(dam) + " damage back to " +
                                           getFullName() + ".");
                        subHP(dam, C_STR);
                    }
                }
                // Call "attack" on the defending servant for their
                // counterattack, if you are in their range and you are the
                // initiating servant.
                // However if the servant is at their final ascension, or if
                //  Lashing out activates, targets cannot counter
                if (!npActive && counter && defenders[i]->isInRange(this))
                {
                    vector<Servant *> you;
                    you.push_back(this);
                    defenders[i]->attack(you, false);
                }
            }
            else
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

void ServantBerserkerFlail::subHP(int hp, DamageType dt)
{
    bool noDamage = false;

    int r = getRandNum();
    if (((ascension == 1 && r <= getLuk()) ||
         (ascension == 2 && r <= getLuk() * 1.5)) &&
            (dt == D_STR || dt == D_MAG))
    {
        noDamage = true;
        log->addToEventLog(getFullName() + " deflected the attack and took no damage! Spin to Win!");
    }

    if (!noDamage)
    {
        currHP -= hp;
        int mhp = getMaxHP();
        if (currHP > mhp)
            currHP = mhp;
        else if (currHP <= 0)
        {
            currHP = 0;
            remAllDebuffs(false);
        }
    }
}

vector<Coordinate> ServantBerserkerFlail::getActionRange(int action)
{
    // The action will always be 0 so no need to actually check
    if (ascension <= 1)
        return getLowToHighRange(getLowRange(), getHighRange());
    else
        return getLowToHighRange(1, 2);
}
