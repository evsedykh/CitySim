#ifndef TRAFFICSIGNALSDATAIMPL_H
#define TRAFFICSIGNALSDATAIMPL_H

struct TrafficSignalsDataImpl
{
    TrafficSignalsDataImpl( double _x, double _y)
        : x( _x ), y( _y )
    { }
    double x;
    double y;
};

#endif // TRAFFICSIGNALSDATAIMPL_H
