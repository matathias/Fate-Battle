
#pragma once

#include "Servant.h"
#include "Debuff.h"

using namespace std;

class PlayField
{
    public:
        // l, or length, corresponds to y, which is north-south.
        // w, or width, corresponds to x, which is west-east.
        PlayField(int l, int w, vector<Servant*> servantList,
                  vector<Coordinate> servantLocations);
        
        vector<vector<Servant*>> getServantLocations();
        //vector<vector<Debuff*>> getField();
        vector<vector<Debuff*>> getTerritories();

        bool isValidCoordinate(Coordinate c);
        Coordinate moveServant(Servant* s, Coordinate c); // returns the
                                                          // Servant's previous
                                                          // location

        bool isRealityMarbleOn();
        Servant* realityMarbleServant(); // returns null if there is no active
                                         // reality marble

        void startRealityMarble(Servant* owner, Debuff* rm);
        void endRealityMarble();
        void eraseTerritory(string n);
        
        bool doesSpaceHaveServant(Coordinate c);
        vector<Coordinate> pruneRange(vector<Coordinate> range, Servant* source);
        vector<Servant*> getAllInRange(Servant* s, vector<Coordinate> range);
        Servant* getServantOnSpace(Coordinate c);
        Debuff* getDebuffOnSpace(Coordinate c);

        int getFieldLength();
        int getFieldWidth();

        Coordinate getRandomCoord();
        
    protected:
        int length;
        int width;

        bool realityMarbleOn;
        Servant *rmServant;
        
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
};
