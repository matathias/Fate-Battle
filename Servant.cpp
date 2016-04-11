/* Class file for servants.
 * 
 * This file contains the implementation for a simple Servant. It tracks the
 * stats and Ascension level, as well as holding a pointer to the Servant's 
 * weapon.
 *
 */

#include "Servant.h"

/********** Function definitions **********/
// Constructor
Servant::Servant(vector<int> h, vector<int> m, vector<int> mv, vector<int> st,
                 vector<int> mg, vector<int> d, vector<int> r, vector<int> sp,
                 vector<int> sk, vector<int> l, int as, Class c, Team t, Weapon* w,
                 vector<*Debuff> dbf, vector<Skill> sks)
{
    maxHP = h;
    maxMP = m;
    mov = mv;
    str = st;
    mag = mg;
    def = d;
    res = r;
    spd = sp;
    skl = sk;
    luk = l;

    ascension = as;
    clss = c;
    team = t;
    weap = w;
    debuffs = dbf;
    skills = sks;

    setHP(getMaxHP());
    setMP(getMaxMP());
}

//***** Manipulators *****
void Servant::setHP(int hp)
{
    currHP = hp;
}

void Servant::setMP(int mp)
{
    currMP = mp;
}

//***** Retrievers *****
int Servant::getMaxHP()
{
    return maxHP[ascension];
}

int Servant::getCurrHP()
{
    return currHP;
}

int Servant::getMaxMP()
{
    return maxMP[ascension];
}

int Servant::getCurrMP()
{
    return currMP;
}

int Servant::getMov()
{
    return mov[ascension];
}

int Servant::getStr()
{
    return str[ascension];
}

int Servant::getMag()
{
    return mag[ascension];
}

int Servant::getDef()
{
    return def[ascension];
}

int Servant::getRes()
{
    return res[ascension];
}

int Servant::getSpd()
{
    return spd[ascension];
}

int Servant::getSkl()
{
    return skl[ascension];
}

int Servant::getLuk()
{
    return luk[ascension];
}

int Servant::getAscensionLvl()
{
    return ascension;
}

Class Servant::getClass()
{
    return clss;
}

Team Servant::getTeam()
{
    return team;
}

Weapon* Servant::getWeapon()
{
    return weap;
}

vector<Debuff*> Servant::getDebuffs()
{
    return debuffs;
}

vector<Skill> Servant::getSkills()
{
    return skills;
}

Coordinate Servant::getCurrLoc()
{
    return currLoc;
}
