#ifndef SERVANTBERSERKER_H
#define SERVANTBERSERKER_H

#pragma once

#include "Servant.h"

class ServantBerserker : public Servant
{
public:
    ServantBerserker(int as, Team t, Logger *l);

    void madRoar(vector<Servant*> defenders);

    virtual void turnUpdate();
    virtual int isActionNP(int action);
};

#endif // SERVANTBERSERKER_H
