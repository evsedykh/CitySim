#ifndef ROADNETWORK_H
#define ROADNETWORK_H

#include "stdafx.h"

typedef unsigned long int RoadGraphId;
typedef std::pair< qreal, qreal > Coordinates;
//typedef boost::property< boost::vertex_name_t, std::wstring > VertexProperty;
typedef std::map< RoadGraphId, std::pair< qreal, qreal > > KeyPointsMap;
class RoadNetwork
{
public:
    RoadNetwork();
    RoadNetwork(const RoadNetwork& );
    RoadNetwork& operator= (const RoadNetwork& other);

    void AddKeyPoint(const RoadGraphId kp, Coordinates coord);
    void RemoveKeyPoint( const RoadGraphId kp );
    void RemoveAllKeyPoints();
    void AddRoadSegment( const RoadGraphId kp1, const RoadGraphId kp2 );

    RoadGraph GetRoadGraph() const;
    KeyPointsMap GetKeyPointsMap() const;

private:
    KeyPointsMap m_KeyPoints;
    RoadGraph m_RoadGraph;
};

#endif // ROADNETWORK_H
