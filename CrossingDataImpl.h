#ifndef CROSSINGDATAIMPL_H
#define CROSSINGDATAIMPL_H

#include <cstdint>

namespace CrossingType {
enum Type { Unmarked = 0, Zebra = 1, Pelican = 2 }; }

struct CrossingDataImpl
{
    CrossingDataImpl( double _x, double _y)
        : x( _x ), y( _y )
    { }
    double x;
    double y;
    unsigned int type : 2; // zebra, pelican etc.
    std::uint64_t traffic_light_ref;
};

#endif // CROSSINGDATAIMPL_H
