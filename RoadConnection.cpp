#include "stdafx.h"

RoadConnection::RoadConnection()
{
}

RoadConnection::RoadConnection( const RoadConnection& other )
    : Drawable(other)
    , m_Center(other.m_Center)
    , m_MaxWidth(other.m_MaxWidth)
    , m_RoadSegments(other.m_RoadSegments)
{
}

RoadConnection& RoadConnection::operator=( const RoadConnection& other )
{
    m_Center = other.m_Center;
    m_MaxWidth = other.m_MaxWidth;
    m_RoadSegments = other.m_RoadSegments;
    return *this;
}

void RoadConnection::Draw()
{
    m_Painter->setBrush(Qt::NoBrush);
    m_Painter->setPen(QPen(Qt::red, 1.));
    m_Painter->drawPath( *m_Path );
}

void RoadConnection::AddRoadSegments( size_t segment1, size_t segment2 )
{
    m_RoadSegments.first = segment1;
    m_RoadSegments.second = segment2;
    RoadManager& theRoadManager( RoadManager::GetInstance() );

    RoadSegment* segm1 = theRoadManager.GetRoadSegment( m_RoadSegments.first );
    RoadSegment* segm2 = theRoadManager.GetRoadSegment( m_RoadSegments.second );
    segm1->SetBeginWidth( segm2->GetCentralWidth() );
    segm2->SetEndWidth( segm1->GetCentralWidth() );

}

qreal RoadConnection::GetIntersectionLength()
{
    RoadManager& theRoadManager( RoadManager::GetInstance() );

    const RoadSegment* segment1 = theRoadManager.GetRoadSegment( m_RoadSegments.first );
    const RoadSegment* segment2 = theRoadManager.GetRoadSegment( m_RoadSegments.second );
    qreal w1 = segment1->GetCentralWidth();
    qreal w2 = segment2->GetCentralWidth();
    qreal dw = fabs(w1-w2);
    qreal lenw = dw * 20. /2.;
    if(dw < 0.1) lenw = w1*2.;

    qreal len1 = segment1->GetPath().length();
    qreal len2 = segment2->GetPath().length();
    qreal len_max = std::min(len1, len2);

    qreal result = std::min( lenw, len_max);
    Segment segm1 = segment1->GetBeginEnd(m_Center);
    Segment segm2 = segment1->GetBeginEnd(m_Center);
    qreal percent1 = (segm1==Begin)?0.:1.;
    qreal percent2 = (segm2==Begin)?0.:1.;
    qreal angle1 = segment1->GetPath().angleAtPercent( percent1 );
    qreal angle2 = segment2->GetPath().angleAtPercent( percent2 );
    if( fabs(angle1-angle2) > 0.1 && fabs(angle1-angle2) < 35. )
    {
        result /= sin(fabs(angle1-angle2)*M_PI/180.);
        result = std::min(result, len_max );
    }
    //std::cout << "w1 = " << w1 << " w2 = " << w2 << " dw = " << dw << " lenw = " << lenw <<
    //              " ; len1 = " << len1 << " len2 = " << len2 << " len_max = " << len_max << " ; result = " << result << std::endl;
    return result;
}

void RoadConnection::ConstructPath()
{
    m_Path.reset( new QPainterPath );

    RoadManager& theRoadManager( RoadManager::GetInstance() );

    const RoadSegment* segment1 = theRoadManager.GetRoadSegment( m_RoadSegments.first );
    const RoadSegment* segment2 = theRoadManager.GetRoadSegment( m_RoadSegments.second );

    QLineF normal1;
    Segment segm1 = segment1->GetBeginEnd(m_Center);
    segment1->GetNormal( m_Center, segm1, GetIntersectionLength(), normal1);

    QLineF normal2;
    Segment segm2 = segment2->GetBeginEnd(m_Center);
    segment2->GetNormal( m_Center, segm2, GetIntersectionLength(), normal2);

    QLineF parallel1(normal1.p2(), normal1.p1());
    parallel1.setAngle(parallel1.angle()+90.);
    QLineF parallel2(normal2.p1(), normal2.p2());
    parallel2.setAngle(parallel2.angle()+90.);

    QPointF par_intersection;
    QLineF::IntersectType intersect_type = parallel1.intersect(parallel2, &par_intersection);
    if(intersect_type == QLineF::NoIntersection)
    {
        std::cout << "RoadConnection::No intersection 1" << std::endl;
    }

    if(fabs(fabs(parallel1.angle() - parallel2.angle())<5.) || fabs(fabs(parallel1.angle() - parallel2.angle())-180.)<5. )
    {
        //std::cout << "almost parallel lines: curr: " << parallel_curr.angle() << " next: " << parallel_next.angle() << std::endl;
        std::pair< QPointF, QPointF > p1, p2;
        if( segm1 == Begin ) p1 = segment1->GetBeginPoints();
        if( segm1 == End ) p1 = segment1->GetEndPoints();
        if( segm2 == Begin ) p1 = segment2->GetBeginPoints();
        if( segm2 == End ) p1 = segment2->GetEndPoints();
        par_intersection.setX(0.5*(p1.first.x() + p2.first.x()));
        par_intersection.setY(0.5*(p1.second.y() + p2.second.y()));
    }

    m_Path->moveTo(normal1.p1());
    m_Path->lineTo(normal1.p2());
    AnglePoints angle_points;
    angle_points.P1 = normal1.p2();
    angle_points.P = par_intersection;
    angle_points.P2 = normal2.p1();

    MakeAngleConnection( angle_points, 0.01, *m_Path );

    m_Path->lineTo(normal2.p1());
    m_Path->lineTo(normal2.p2());

    QLineF parallel3(normal2.p2(), normal2.p1());
    parallel3.setAngle(parallel3.angle()+90.);

    QLineF parallel4(normal1.p1(), normal1.p2());
    parallel4.setAngle(parallel4.angle()+90.);

    intersect_type = parallel3.intersect(parallel4, &par_intersection);
    if(intersect_type == QLineF::NoIntersection)
    {
        std::cout << "RoadConnection::No intersection 2" << std::endl;
    }

    if(fabs(fabs(parallel3.angle() - parallel4.angle())<5.) || fabs(fabs(parallel3.angle() - parallel4.angle())-180.)<5. )
    {
        //std::cout << "almost parallel lines: curr: " << parallel_curr.angle() << " next: " << parallel_next.angle() << std::endl;
        std::pair< QPointF, QPointF > p1, p2;
        if( segm1 == Begin ) p1 = segment1->GetBeginPoints();
        if( segm1 == End ) p1 = segment1->GetEndPoints();
        if( segm2 == Begin ) p1 = segment2->GetBeginPoints();
        if( segm2 == End ) p1 = segment2->GetEndPoints();
        par_intersection.setX(0.5*(p1.first.x() + p2.first.x()));
        par_intersection.setY(0.5*(p1.second.y() + p2.second.y()));
    }
    angle_points.P1 = normal2.p2();
    angle_points.P = par_intersection;
    angle_points.P2 = normal1.p1();

    MakeAngleConnection( angle_points, 0.01, *m_Path );

    m_Path->lineTo(normal1.p1());
}
