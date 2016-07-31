#ifndef NODEDATAIMPL_H
#define NODEDATAIMPL_H

#include <cstdint>

class NodeDataImpl
{
public:
    NodeDataImpl()
        : m_Longitude( 0. )
        , m_Latitude( 0. )
    { }
    NodeDataImpl( double lon, double lat )
        : m_Longitude( lon )
        , m_Latitude( lat )
    { }
    NodeDataImpl( const NodeDataImpl& other )
        : m_Longitude( other.m_Longitude )
        , m_Latitude( other.m_Latitude )
    { }

    virtual ~NodeDataImpl() { }
public:
    double m_Longitude;
    double m_Latitude;
};

#endif // NODEDATAIMPL_H
