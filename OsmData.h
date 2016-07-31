#ifndef OSMDATA_H
#define OSMDATA_H

#include <vector>
#include <map>
#include "RoadSegmentDataImpl.h"
#include "NodeDataImpl.h"

class OsmData
{
public:
    OsmData();
    void AddRoadSegment( const RoadSegmentDataImpl& road_segment );
    void AddNode( std::uint64_t id, const NodeDataImpl& node );

//private:
    std::vector< RoadSegmentDataImpl > m_RoadSegments;
    std::map< std::uint64_t, NodeDataImpl > m_Nodes;


};

#endif // OSMDATA_H
