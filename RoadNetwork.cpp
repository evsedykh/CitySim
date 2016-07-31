#include "stdafx.h"

RoadNetwork::RoadNetwork()
{
}


RoadNetwork::RoadNetwork(const RoadNetwork& other )
    : m_KeyPoints(other.m_KeyPoints ), m_RoadGraph( other.m_RoadGraph )
{
}

RoadNetwork& RoadNetwork::operator= (const RoadNetwork& other)
{
    m_KeyPoints = other.m_KeyPoints;
    m_RoadGraph = other.m_RoadGraph;
    return *this;
}

void RoadNetwork::AddKeyPoint(const RoadGraphId kp, std::pair< qreal, qreal > coord)
{
    auto fnd = m_KeyPoints.find( kp );
    if( fnd == m_KeyPoints.end() )
    {
        m_KeyPoints[ kp ] = coord;
    }
}

void RoadNetwork::RemoveKeyPoint( const RoadGraphId kp )
{
    auto fnd = m_KeyPoints.find( kp );
    if( fnd == m_KeyPoints.end())
    {
        m_KeyPoints.erase(fnd);
    }
}

void RoadNetwork::RemoveAllKeyPoints()
{
    m_KeyPoints.clear();
}

void RoadNetwork::AddRoadSegment( const RoadGraphId kp1, const RoadGraphId kp2 )
{
    if( kp1 != kp2 )
    {
        boost::add_edge( kp1, kp2, m_RoadGraph );
    }
}

RoadGraph RoadNetwork::GetRoadGraph() const
{
    return m_RoadGraph;
}

KeyPointsMap RoadNetwork::GetKeyPointsMap() const
{
    return m_KeyPoints;
}
