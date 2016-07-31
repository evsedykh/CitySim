#include "OsmData.h"

OsmData::OsmData()
{
}

void OsmData::AddRoadSegment( const RoadSegmentDataImpl& road_segment )
{
    m_RoadSegments.push_back( road_segment );
}

void OsmData::AddNode( std::uint64_t id, const NodeDataImpl& node )
{
    m_Nodes[ id ] = node;
}





