#ifndef ROADSEGMENTDATAIMPL_H
#define ROADSEGMENTDATAIMPL_H

#include <cstdint>

namespace Overtaking {
enum Rule { Allowed = 0, Prohibited = 1, Backward = 2, Forward = 3 }; }

namespace Hgv {
enum Rule { Allowed = 0, Prohibited = 1, Destination = 2 }; }

namespace SideRoad {
enum Type { No = 0, Yes = 1, Rotary =2, Double = 3 }; }

namespace Surface {
enum Type { Asphalt = 0,     // default for paved
            Concrete = 1,    // also for concrete:lanes and concrete:paves
            PavingStones = 2, Wood = 3,
            Compacted = 4,   // default for unpaved
            Gravel = 5, Ground = 6, Sand = 7 }; }

namespace Smoothness {
enum Type { Excellent = 0, VeryGood = 1, Good = 2, Intermediate = 3, Bad = 4, VeryBad = 5, Horrible = 6, Impassable = 7 }; }

namespace Turn { // bits
enum Rule { Left = 1, SlightLeft = 2, SharpLeft = 4, Through = 8, Right = 16, SlightRight = 32, SharpRight = 64, Reverse = 128, MergeToLeft = 256, MergeToRight = 512, None = 1024 }; }

//template< int NLanes >
struct RoadSegmentDataImpl
{
    RoadSegmentDataImpl()
        : lanes_forward( 1 ),
          lanes_backward( 1 ),
          oneway( 0 ),
          layer( 0 ),
          width( 5 ),
          maxspeed( 0. ),
          maxspeed_practical( 0 ),
          overtaking( 0 ),
          restriction_hgv( 0 ),
          bridge( 0 ),
          side_road( 0 ),
          surface( 0 ),
          smoothness( 0 ),
          lit( 1 ),
          trolley_wire( 0 ),
          lane_turns( 0 )
    {
    }
    unsigned int lanes_forward : 3;
    unsigned int lanes_backward : 3;
    unsigned int oneway : 1;
    int layer : 4;
    unsigned int width : 8; // step 0.5 m
    unsigned int maxspeed : 5; // step 5kmh
    unsigned int maxspeed_practical : 5;
    unsigned int overtaking : 2;
    unsigned int restriction_hgv : 2;
    unsigned int bridge : 1;
    unsigned int side_road : 2;
    unsigned int surface : 3;
    unsigned int smoothness : 3;
    unsigned int lit : 1;
    unsigned int trolley_wire : 1;
    std::uint64_t lane_turns : 60; // max 6 lanes
    std::vector< std::uint64_t > nodes;
};

#endif // ROADSEGMENTDATAIMPL_H
