#ifndef STOPSIGNDATAIMPL_H
#define STOPSIGNDATAIMPL_H

struct StopSignDataImpl
{
    StopSignDataImpl( double _x, double _y)
        : x( _x ), y( _y )
    { }
    double x;
    double y;
};

#endif // STOPSIGNDATAIMPL_H
