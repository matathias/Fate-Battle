// Superclass to all of the specific servant instances

#pragma once

#include <vector>

#include "PlayField.h"

using namespace std;

enum Stat {HP, MP, MOV, STR, MAG, DEF, RES, SPD, SKL, LUK};
enum Class {Saber, Lancer, Archer, Caster, Berserker, Rider, Assassin, Avenger};
enum Team {Alpha, Omega, Boss};
 
class Servant
{
    public:
        Servant(vector<int> h, vector<int> m, vector<int> mv, vector<int> st,
                vector<int> mg, vector<int> d, vector<int> r, vector<int> sp,
                vector<int> sk, vector<int> l, int as, Team t);
                
        void setHP(int hp);
        void setMP(int mp);
        
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
        vector<Skill> getSkills();

        Coordinate getCurrLoc();
        
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
        
        Class clss;
        Team team;

        vector<Debuff*> debuffs;
        vector<Skill> skills;

        Coordinate currLoc;
}
