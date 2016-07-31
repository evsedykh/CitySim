#ifndef BUSSTOPDATAIMPL_H
#define BUSSTOPDATAIMPL_H

#include <cstdint>
#include <string>
#include <list>

struct BusStopDataImpl
{
    BusStopDataImpl( double _x, double _y)
        : x( _x ), y( _y )
    { }
    double x;
    double y;
    std::string name;
    std::list< std::uint16_t > bus_numbers;
};


#endif // BUSSTOPDATAIMPL_H
