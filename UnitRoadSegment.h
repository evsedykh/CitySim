#ifndef UNITROADSEGMENT_H
#define UNITROADSEGMENT_H

#include "stdafx.h"

class UnitRoadSegment
{
public:
    UnitRoadSegment();
    UnitRoadSegment( const UnitRoadSegment& other);
    void StretchLength( qreal factor );
    void StretchWidth( qreal factor );
    void StretchBeginWidth( qreal factor );
    void StretchCentralWidth( qreal factor );
    void StretchEndWidth( qreal factor );
    void Rotate( qreal angle );
    void Translate( const QPointF& relative );
    void AddUnitRoadSegment( const UnitRoadSegment& other );
    void ChangeDirection();
    void Draw();
    void Add();
    void Print() const;

private:
    void Construct();
    RoadSegment m_Segment;
};

#endif // UNITROADSEGMENT_H
