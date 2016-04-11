
#pragma once

#include "Servant.h"
#include "Debuff.h"
#include "GameState.h"

using namespace std;

struct Coordinate
{
    int x; int y;
};

class PlayField
{
    public:
        // l, or length, corresponds to y, which is north-south.
        // w, or width, corresponds to x, which is west-east.
        PlayField(int l, int w, vector<Servant*> servantList,
                  vector<Coordinate> servantLocations, GameState *GS);
        
        vector<vector<Servant*>> getServantLocations();
        //vector<vector<Debuff*>> getField();
        vector<vector<Debuff*>> getTerritories();
        
        vector<Coordinate> getValidMoves(Servant* s);
        bool isValidCoordinate(Coordinate c);
        Coordinate moveServant(Servant* s, Coordinate c); // returns the
                                                          // Servant's previous
                                                          // location
        
        vector<Servant*> getAllInRange(Servant* s, vector<Coordinate> range);
        
    protected:
        int length;
        int width;
        GameState *gs;
        
        vector<vector<Servant*>> field; // length x width playing field. If a 
                                        // Servant is occupying the spot, then
                                        // there will be a pointer to that
                                        // Servant. Otherwise there is a NULL
                                        // pointer.
                                        
        //vector<vector<Debuff*>> terrain; // Marks the terrain. Some terrain may
                                         // apply buffs or debuffs to servants
                                         // standing on it.
        
        vector<vector<Debuff*>> tempEffects; // Marks temporary effects like
                                             // caster territories.
}
