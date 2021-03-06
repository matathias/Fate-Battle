
#pragma once

#include "Servant.h"
#include "Debuff.h"

using namespace std;

bool isInVector(vector<Coordinate> vec, Coordinate c);
Direction getDirectionAtoB(Coordinate a, Coordinate b);
vector<Coordinate> getLowToHighRange(int low, int high);
vector<Coordinate> getAbsoluteRange(int low, int high, Servant* s);
int getDistance(Coordinate a, Coordinate b);

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

        // Returns true if the Coordinate is in bounds AND there is no servant
        // on the space.
        bool isValidCoordinate(Coordinate c);

        // Returns true if the Coordinate is in bounds. Does not care if space
        // is occupied or not.
        bool isCoordinateInBounds(Coordinate c);

        // Returns the Servant's previous location
        Coordinate moveServant(Servant* s, Coordinate c);

        // The servant has died. Remove them from the playing field.
        void servantDead(Servant* s);

        // The servant has been revived. Add them back to the playing field.
        void servantRevived(Servant* s);

        bool isRealityMarbleOn();
        Servant* realityMarbleServant(); // returns null if there is no active
                                         // reality marble

        void startRealityMarble(Servant* owner, Debuff* rm);
        void endRealityMarble();
        void startTerritory(Servant* owner, Debuff* terr, vector<Coordinate> range);
        void eraseTerritory(string n);

        void addDebuffToSpace(Debuff* d, Coordinate c);
        void addDebuffToRange(Debuff* d, vector<Coordinate> range);
        
        bool doesSpaceHaveServant(Coordinate c);
        vector<Coordinate> pruneRange(vector<Coordinate> range, Servant* source);
        vector<Servant*> getAllInRange(Servant* s, vector<Coordinate> range);
        vector<Servant*> getAllInRange2(vector<Coordinate> range);
        Servant* getServantOnSpace(Coordinate c);
        Debuff* getDebuffOnSpace(Coordinate c);

        int getFieldLength();
        int getFieldWidth();
        int getLargestDimension();

        Coordinate getRandomCoord();
        Coordinate getNearestValidCoord(Coordinate c);
        Coordinate getNearestValidCoord2(Coordinate c);
        Coordinate getFarthestValidFrom(vector<Servant*> s);
        vector<Coordinate> getAdjacentSpaces(Coordinate c);
        vector<Coordinate> getAdjacentToRange(vector<Coordinate> range);

        // Functions to find a straight line from start to end
        vector<Coordinate> pathFind(Coordinate start, Coordinate end);
        
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
