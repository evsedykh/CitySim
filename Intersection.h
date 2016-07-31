#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "stdafx.h"

class Intersection : public Drawable
{
public:
    Intersection();
    Intersection( const Intersection& other );
    Intersection& operator=( const Intersection& other );
    void SetCenter( const QPointF& xy ){m_Center = xy;}
    void Draw();
    void AddRoadSegment( size_t segment_id );
    qreal GetIntersectionLength();
    void ConstructPath();
    void SortSegmentsByAngle();

private:
    QPointF m_Center;
    qreal m_MaxWidth;
    std::vector< size_t > m_RoadSegments;
    std::vector< size_t > m_RoadSegmentsSorted;
    bool m_Sorted;

};

#endif // INTERSECTION_H
