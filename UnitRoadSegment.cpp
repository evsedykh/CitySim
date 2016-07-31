#include "stdafx.h"

UnitRoadSegment::UnitRoadSegment()
{
    EarthCoordinates p0(30.0, 60.0);
    EarthCoordinates p1(30.001, 60.0);
    m_Segment.AddCoordinate(p0);
    m_Segment.AddCoordinate(p1);
    m_Segment.SetCentralWidth(10.);
    m_Segment.SetBeginWidth(10.);
    m_Segment.SetEndWidth(10.);
}

UnitRoadSegment::UnitRoadSegment( const UnitRoadSegment& other)
    : m_Segment( other.m_Segment )
{
}

void UnitRoadSegment::StretchLength( qreal factor )
{
    auto coords = m_Segment.GetCoordinates();
    std::vector< QPointF > empty;
    m_Segment.SetCoordinates( empty );
    size_t points = coords.size();
    EarthCoordinates p(coords[ 0 ].x(), coords[ 0 ].y() );
    m_Segment.AddCoordinate( p );
    for( size_t i = 1; i < points; ++i)
    {
        QLineF line( coords[ i - 1 ].x(), coords[ i - 1 ].y(), coords[ i ].x(), coords[ i ].y() );
        line.setLength( line.length() * factor );
        EarthCoordinates p( line.p2().x(), line.p2().y() );
        m_Segment.AddCoordinate( p );
    }
    Construct();
}

void UnitRoadSegment::StretchWidth( qreal factor )
{
    qreal curr_width = m_Segment.GetCentralWidth();
    m_Segment.SetBeginWidth( curr_width * factor );
    m_Segment.SetCentralWidth( curr_width * factor );
    m_Segment.SetEndWidth( curr_width * factor );
}

void UnitRoadSegment::StretchBeginWidth( qreal factor )
{
    m_Segment.SetBeginWidth( m_Segment.GetBeginWidth() * factor );
}

void UnitRoadSegment::StretchCentralWidth( qreal factor )
{
    m_Segment.SetCentralWidth( m_Segment.GetCentralWidth() * factor );
}

void UnitRoadSegment::StretchEndWidth( qreal factor )
{
    m_Segment.SetEndWidth( m_Segment.GetEndWidth() * factor );
}

void UnitRoadSegment::Rotate( qreal angle )
{
    auto coords = m_Segment.GetCoordinates();
    for( size_t i = 1; i < coords.size(); ++i)
    {
        QLineF line( coords[ i - 1 ], coords[ i ] );
        line.setAngle( line.angle() + angle );
        coords[ i - 1] = line.p1();
        coords[ i ] = line.p2();
    }
    m_Segment.SetCoordinates( coords );
}

void UnitRoadSegment::Translate( const QPointF& relative )
{
    auto coords = m_Segment.GetCoordinates();
    for( size_t i = 0; i < coords.size(); ++i)
    {
        coords[ i ] += relative;
    }
    m_Segment.SetCoordinates( coords );
}

void UnitRoadSegment::AddUnitRoadSegment( const UnitRoadSegment& other )
{
    auto coords = other.m_Segment.GetCoordinates();
    size_t points = coords.size();
    auto own_coords = m_Segment.GetCoordinates();
    m_Segment.SetEndWidth( other.m_Segment.GetBeginWidth());
    qreal own_last_x = own_coords[ own_coords.size()-1 ].x();
    qreal own_last_y = own_coords[own_coords.size()-1 ].y();
    for( size_t i = 1; i < points; ++i)
    {
        QPointF p(coords[ i ].x() + own_last_x - coords[0].x(), coords[ i ].y() + own_last_y - coords[0].y());
        m_Segment.AddXYCoordinate( p );
    }
    //m_Segment.Print();
    Construct();
}

void UnitRoadSegment::ChangeDirection()
{
    auto coords = m_Segment.GetCoordinates();
    std::vector< QPointF > empty;
    m_Segment.SetCoordinates( empty );
    size_t points = coords.size();
    for( long i = points - 1 ; i >= 0; --i )
    {
        EarthCoordinates p(coords[ i ].x(), coords[ i ].y() );
        m_Segment.AddCoordinate( p );
    }
    Construct();
}

void UnitRoadSegment::Draw()
{
    m_Segment.Draw();
}

void UnitRoadSegment::Add()
{
    RoadManager& theRoadManager( RoadManager::GetInstance() );
    theRoadManager.AddRoadSegment( m_Segment );
}

void UnitRoadSegment::Construct()
{
    m_Segment.ConstructCentralPath();
    m_Segment.ConstructPath();
}

void UnitRoadSegment::Print() const
{
    m_Segment.Print();
}

