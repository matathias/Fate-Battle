// Superclass to all of the specific servant instances

#pragma once

#include <vector>
#include <string>
#include <cstdlib>
#include <QMessageBox>

#include "Debuff.h"
#include "Skill.h"
//#include "PlayField.h"
#include "logger.h"
//#include "datatypes.h"

using namespace std;

class PlayField;

class Servant
{
    public:
        Servant(int as, Team t, Logger *l);

        virtual void setPlayField(PlayField *f);
                
        virtual void setHP(int hp);
        void setMP(int mp);
        void addHP(int hp);
        void addMP(int mp);
        virtual void subHP(int hp, DamageType dt);
        void subMP(int mp);
        
        string getName();
        string getTeamName();
        string getFullName();

        string getServantIcon();
        string getTeamIcon();

        // These are all virtual for the sake of the Avenger class
        int getMaxHP();
        int getCurrHP();
        int getMaxMP();
        int getCurrMP();
        int getMov();
        int getStr();
        int getMag();
        int getDef();
        int getRes();
        int getSpd();
        int getSkl();
        int getLuk();
        int getAscensionLvl();
        
        Class getClass();
        Team getTeam();

        vector<Debuff*> getDebuffs();
        virtual void addDebuff(Debuff* d);
        void decDebuffs();
        void remAllDebuffs(bool purgePermadeath);
        // The second version applies to a servant who is not dying
        void remAllDebuffs2(bool purgePermadeath, bool purgeDoom);
        void removeDoom();
        vector<Debuff*> getCastedDebuffs();
        void addCastedDebuff(Debuff* d);
        void decCastedDebuffs();
        void remCastedDebuffs();
        vector<Skill> getSkills();

        // Only relevant to casters
        bool isTerritoryActive();
        string removeTerritory();

        int getDebuffAmount(Stat s);

        Coordinate getCurrLoc();
        bool isAdjacent(Servant * s);
        bool isInRange(Servant *s);

        int getLowRange();
        int getHighRange();

        // Functions related to battle formulas and damage calculation
        int getHitRate();
        virtual vector<int> getEvade(); // This is a vector of ints to account
                                        // for Servants who have skills that
                                        // offer multiple evasion chances. Only
                                        // the first value is checked against
                                        // hit rate
        int getInitialEvade();
        int getCriticalRate();
        int getCriticalEvade();

        // Functions intended for use by the subclasses
        vector<string> getActionList();
        vector<ActionType> getActionListTypes();
        vector<int> getActionMPCosts();
        vector<bool> getActionsDodgeable();
        vector<bool> getActionsCounterable();

        ActionType getActionType(int action);
        int getActionMPCost(int action);
        bool getActionDodgeable(int action);
        bool getActionCounterable(int action);
        virtual string getActionName(int action);
        // If the action is a Noble Phantasm, returns the index of the NPvector
        // in which the NP is stored. If the action is not an NP, returns -1.
        virtual int isActionNP(int action);
        virtual bool isActionRealityMarble(int action);

        // Get the MP cost for continuing a reality marble or territory.
        virtual int getRealityMarbleMP();
        virtual int getTerritoryMP();
        virtual string getTerritoryName();

        // These two functions are specific to Casters and Assassins
        // respectively.
        virtual bool isHealAction(int action);
        virtual bool isPoisonAction(int action);
        virtual bool isKillDeadAction(int action);

        // Only need to define this for the berserkers
        virtual bool isGodmindActive();

        virtual bool isBerserk();

        virtual vector<Coordinate> getActionRange(int action);

        vector<vector<string>> getNoblePhantasms();
        vector<vector<Coordinate>> getNPRanges();
        vector<Coordinate> getNPRange(int np);

        void setLoc(Coordinate newLoc);

        bool isPermaDead();

        // This function is defined in the Servant class, but servants with
        // special skills attached to their attack should override the attack
        // method.
        // If counter is true, then the defending servants can counterattack.
        // Otherwise they cannot.
        // It returns 0 if it succeeds, and another value otherwise.
        virtual int attack(vector<Servant*> defenders, bool counter);

        // Like with attack, this function is defined in the Servant class but
        // will need to be overridden if a Servant has more options available to
        // them than just attacking and their noble phantasms.
        // The function returns 0 if it succeeds, and another value otherwise
        // (i.e. the choice made is not valid).
        virtual int doAction(int actionNum, vector<Servant*> defenders);

        // These functions actually need to be defined in the subclasses.
        // They return 0 if they succeed, and another value otherwise.
        virtual int activateNP1(vector<Servant*> defenders);
        virtual int activateNP2(vector<Servant*> defenders);
        virtual int activateNP3(vector<Servant*> defenders);

        // Generates a random number between 0 and 100 (inclusive) using fire
        // emblem's "true hit" technique
        int getRandNum();

        // If num is less than 0, returns 0. Otherwise, returns num.
        int capZero(int num);
        // If num is less than 1, returns 1. Otherwise, returns num.
        int capOne(int num);

        // Placeholder. Will only really be used for Avengers.
        virtual Debuff* finalRevenge();

        virtual int endRealityMarble();

        virtual void endTerritory();

        Coordinate getEndLocation(Coordinate c, int range);
        
    protected:
        string name; // Based on the Servant's class and weapon.

        // Stats are stored as vector<int>s in order to track the three different
        // values based on Ascension
        vector<int> maxHP;
        vector<int> maxMP;
        vector<int> mov;
        vector<int> str;
        vector<int> mag;
        vector<int> def;
        vector<int> res;
        vector<int> spd;
        vector<int> skl;
        vector<int> luk;
        int ascension; //0 is base ascension, 1 is first ascension, 2 is final
        
        int currHP;
        int currMP;

        int lowRange;
        int hiRange;
        
        Class clss;
        Team team;

        vector<Debuff*> debuffs;
        vector<Debuff*> castedDebuffs;
        vector<Skill> skills;

        bool territoryActive; // Only relevant to casters
        bool realityMarbleActive;

        Coordinate currLoc;
        PlayField *field;

        // Fields intended for use by subclasses
        vector<vector<string>> actionList;
        vector<vector<ActionType>> actionListTypes;
        vector<vector<int>> actionMPCosts;
        vector<vector<bool>> actionDodgeable;
        vector<vector<bool>> actionCounterable;

        vector<vector<string>> noblePhantasms; // Column 1: name, 2: description
        vector<vector<Coordinate>> npRanges; // Ranges for NPs

        Logger *log;
};
