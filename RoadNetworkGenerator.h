#ifndef ROADNETWORKGENERATOR_H
#define ROADNETWORKGENERATOR_H

#include "stdafx.h"

class RoadNetworkGenerator
{
public:
    RoadNetworkGenerator();
    void Generate();
    const RoadNetwork& GetRoadNetwork();
private:
    RoadNetwork m_Network;
};

#endif // ROADNETWORKGENERATOR_H
