#include "stdafx.h"

Intersection::Intersection()
{
}


Intersection::Intersection( const Intersection& other )
    : Drawable(other),
      m_Center(other.m_Center),
      m_MaxWidth( other.m_MaxWidth ),
      m_RoadSegments( other.m_RoadSegments ),
      m_RoadSegmentsSorted(other.m_RoadSegmentsSorted),
      m_Sorted(other.m_Sorted)
{
}

Intersection& Intersection::operator=( const Intersection& other )
{
    m_Center = other.m_Center;
    m_MaxWidth = other.m_MaxWidth;
    m_RoadSegments = other.m_RoadSegments;
    m_RoadSegmentsSorted = other.m_RoadSegmentsSorted;
    m_Sorted = other.m_Sorted;
    return *this;
}

void Intersection::Draw()
{
    //m_Painter->setBrush(Qt::blue);
    m_Painter->setBrush(Qt::NoBrush);

    if(! m_Sorted)
    {
        m_Painter->setBrush(Qt::blue);
    }
    //m_Painter->setBrush(Qt::NoBrush);
    m_Painter->setPen(QPen(Qt::blue, 2.0));

    m_Painter->drawPath( *m_Path );
}

void Intersection::AddRoadSegment( size_t segment_id)
{
    RoadManager& theRoadManager( RoadManager::GetInstance() );
    const RoadSegment* segment = theRoadManager.GetRoadSegment( segment_id );

    m_RoadSegments.push_back( segment_id );

    qreal segment_width = segment->GetCentralWidth();
    if(m_MaxWidth < segment_width )
    {
        m_MaxWidth = segment_width;
    }
    m_Sorted = false;
}

qreal Intersection::GetIntersectionLength()
{
    return m_MaxWidth * 5.;
}

void Intersection::ConstructPath()
{
    SortSegmentsByAngle();
    if( ! m_Sorted)
    {
        m_Path->addEllipse(m_Center, 15, 15);
        return;
    }

    size_t segments = m_RoadSegmentsSorted.size();
    if( segments < 2 )
    {
        std::cerr << "Should be 2 or more RoadSegments during Intersection::ConstructPath calling." << std::endl;
        return;
    }

    //std::cout << "ConstructPath" << std::endl;
    RoadManager& theRoadManager( RoadManager::GetInstance() );

    for(size_t i = 0; i <= segments-1; ++i)
    {
        const RoadSegment* segment = theRoadManager.GetRoadSegment( m_RoadSegmentsSorted[ i ] );

        QLineF normal;
        Segment segm = segment->GetBeginEnd(m_Center);
        segment->GetNormal( m_Center, segm, GetIntersectionLength(), normal);

        size_t next_i = i+1;
        if(i==(segments-1)) next_i = 0;
        const RoadSegment* next_segment = theRoadManager.GetRoadSegment( m_RoadSegmentsSorted[ next_i ] );
        QLineF next_normal;
        Segment next_segm = next_segment->GetBeginEnd(m_Center);
        next_segment->GetNormal( m_Center, next_segm, GetIntersectionLength(), next_normal);

        // construct parallels
        QLineF parallel_curr(normal.p2(), normal.p1());
        parallel_curr.setAngle(parallel_curr.angle()+90.);
        QLineF parallel_next(next_normal.p1(), next_normal.p2());
        parallel_next.setAngle(parallel_next.angle()+90.);

        QPointF par_intersection;
        QLineF::IntersectType intersect_type = parallel_curr.intersect(parallel_next, &par_intersection);
        if(intersect_type == QLineF::NoIntersection)
        {
            std::cout << "Intersection: No intersection" << std::endl;
        }

        if(fabs(fabs(parallel_curr.angle() - parallel_next.angle())<5.) || fabs(fabs(parallel_curr.angle() - parallel_next.angle())-180.)<5. )
        {
            //std::cout << "almost parallel lines: curr: " << parallel_curr.angle() << " next: " << parallel_next.angle() << std::endl;
            par_intersection.setX(0.5*(normal.p2().x() + next_normal.p1().x()));
            par_intersection.setY(0.5*(normal.p2().y() + next_normal.p1().y()));
        }

        if(i == 0)
        {
            m_Path->moveTo(normal.p1());
        } else
        {
            m_Path->lineTo(normal.p1());
        }

        m_Path->lineTo(normal.p2());
        AnglePoints angle_points;
        angle_points.P1 = normal.p2();
        angle_points.P = par_intersection;
        angle_points.P2 = next_normal.p1();

        MakeAngleConnection( angle_points, 0.01, *m_Path );
        if( i == (segments - 1))
        {
           m_Path->lineTo(next_normal.p1());
        }
    }
}

void Intersection::SortSegmentsByAngle()
{
    RoadManager& theRoadManager( RoadManager::GetInstance() );
    std::map<qreal, size_t> angle_info;
    for(size_t i = 0; i < m_RoadSegments.size(); ++i)
    {
        const RoadSegment* segment = theRoadManager.GetRoadSegment( m_RoadSegments[ i ] );

        Segment segm = segment->GetBeginEnd( m_Center );
        std::pair<QPointF, QPointF> far_points;

        if(segm == Begin)
        {
            far_points = segment->GetBeginPoints( GetIntersectionLength() );
        }
        else if(segm == End)
        {
            far_points = segment->GetEndPoints( GetIntersectionLength() );
        } else
        {
            //std::cout << "None edge in SortSegmentsByAngle" << std::endl;
            return;
        }
        QLineF line(m_Center, far_points.first);
        qreal angle = line.angle();

        angle_info.insert(std::make_pair(angle, m_RoadSegments[ i ]));
    }

    m_RoadSegmentsSorted.clear();
    for(auto it: angle_info)
    {
        m_RoadSegmentsSorted.push_back(it.second);
    }
    m_Sorted = true;
}

