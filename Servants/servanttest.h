#ifndef SERVANTTEST_H
#define SERVANTTEST_H

#pragma once
#include "Servant.h"

class ServantTest : public Servant
{
    public:
        ServantTest(int as, Team t);

        int activateNP1(vector<Servant *> defenders);
        int activateNP2(vector<Servant *> defenders);
        int activateNP3(vector<Servant *> defenders);
};

#endif // SERVANTTEST_H
