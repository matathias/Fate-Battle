#ifndef DATATYPES_H
#define DATATYPES_H

#endif // DATATYPES_H

enum Stat {MAXHP, HP, MAXMP, MP, MOV, STR, MAG, DEF, RES, SPD, SKL, LUK};
enum Class {Saber, Lancer, Archer, Caster, Berserker, Rider, Assassin, Avenger};
enum Team {Alpha, Omega, Boss};

//S = single target, A = AOE, T = territory creation, N = no target
enum ActionType {S, A, T, N};

struct Coordinate
{
    int x; int y;
};
