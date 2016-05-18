#ifndef SERVANTARCHER_H
#define SERVANTARCHER_H

#pragma once

#include "Servant.h"

class ServantArcher : public Servant
{
public:
    ServantArcher(int as, Team t, Logger *l);

    virtual vector<int> getEvade();
};

#endif // SERVANTARCHER_H
