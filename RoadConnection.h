#ifndef ROADCONNECTION_H
#define ROADCONNECTION_H

#include "stdafx.h"

class RoadConnection : public Drawable
{
public:
    RoadConnection();

    RoadConnection( const RoadConnection& other );
    RoadConnection& operator=( const RoadConnection& other );

    void SetCenter( const QPointF& xy ){m_Center = xy;}
    void Draw();

    void AddRoadSegments( size_t segment1, size_t segment2 );
    qreal GetIntersectionLength();
    void ConstructPath();

private:
    QPointF m_Center;
    qreal m_MaxWidth;
    std::pair< size_t, size_t > m_RoadSegments;
};

#endif // ROADCONNECTION_H
