#ifndef SERVANTSABERMINION_H
#define SERVANTSABERMINION_H

#pragma once

#include "Servants/servantsaber.h"

class ServantSaberMinion : public ServantSaber
{
public:
    ServantSaberMinion(int as, Team t, Logger *l);

    virtual int doAction(int actionNum, vector<Servant *> defenders);

    virtual int isActionNP(int action);
};

#endif // SERVANTSABERMINION_H
