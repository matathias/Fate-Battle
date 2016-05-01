#ifndef SERVANTTEST_H
#define SERVANTTEST_H

#pragma once
#include "Servant.h"

class ServantTest : public Servant
{
    public:
        ServantTest(int as, Team t);

        // Straight Slash
        virtual int activateNP1(vector<Servant *> defenders);
        // Side Slash
        virtual int activateNP2(vector<Servant *> defenders);
        // Omni Slash
        virtual int activateNP3(vector<Servant *> defenders);
};

#endif // SERVANTTEST_H
