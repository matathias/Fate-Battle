#ifndef DATATYPES_H
#define DATATYPES_H

#endif // DATATYPES_H

enum Stat {MAXHP, HP, MAXMP, MP, MOV, STR, MAG, DEF, RES, SPD, SKL, LUK};
enum Class {Saber, Lancer, Archer, Caster, Berserker, Rider, Assassin, Avenger};
enum Team {Alpha, Omega, Boss};
enum DamageType {D_STR, D_MAG, NP_STR, NP_MAG, OMNI, C_STR, C_MAG};

//S = single target, A = AOE, T = territory creation, N = no target, D = target
// dead
enum ActionType {S, A, T, N, D};

struct Coordinate
{
    int x; int y;
};
