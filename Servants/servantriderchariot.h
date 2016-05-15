#ifndef SERVANTRIDERCHARIOT_H
#define SERVANTRIDERCHARIOT_H

#pragma once

#include "Servants/servantrider.h"

class ServantRiderChariot : public ServantRider
{
public:
    ServantRiderChariot(int as, Team t, Logger *l);

    int whipOfTheMaster();

    virtual int attack(vector<Servant *> defenders, bool counter);

    virtual int isActionNP(int action);
    virtual bool isActionRealityMarble(int action);
    virtual vector<Coordinate> getActionRange(int action);

    virtual int getRealityMarbleMP();

    virtual int doAction(int actionNum, vector<Servant *> defenders);

    // Charge of the Vanguard
    virtual int activateNP1(vector<Servant *> defenders);
    // Call in the Cavalry
    virtual int activateNP2(vector<Servant *> defenders);
    // Essence of Ionioi Hetairoi
    virtual int activateNP3(vector<Servant *> defenders);

private:
    bool callInTheCavalryActive();
    bool whipOfTheMasterActive();
};

#endif // SERVANTRIDERCHARIOT_H
