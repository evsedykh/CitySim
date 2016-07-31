#include "stdafx.h"

TestRoadSegments::TestRoadSegments()
{
    RoadManager& theRoadManager( RoadManager::GetInstance() );
    EarthCoordinates p1( 29.99, 59.99);
    EarthCoordinates p2( 30.01, 60.01 );
    theRoadManager.SetEdges( p1, p2 );
}

void TestRoadSegments::TestStretch()
{
    const int n_changes = 10;
    const qreal delta_w = 0.2;
    const qreal delta_x = 150.;
    UnitRoadSegment segment;
    for(int i = 0; i < n_changes; ++i )
    {
        UnitRoadSegment another( segment );
        QPointF translate_to( i * delta_x, 0.);
        another.Translate( translate_to );
        another.StretchWidth( 1. + i * delta_w);
        another.Add();
    }
    for(int i = 0; i < n_changes; ++i )
    {
        UnitRoadSegment another( segment );
        QPointF translate_to( i * delta_x, -100.);
        another.Translate( translate_to );
        qreal factor = 1. + i * delta_w;
        another.StretchBeginWidth( factor );
        //std::cout << "i = " << i << "w: " << 10. * factor << " -> 10 -> 10" << std::endl;
        another.Add();
    }
    for(int i = 0; i < n_changes; ++i )
    {
        UnitRoadSegment another( segment );
        QPointF translate_to( i * delta_x, -200.);
        another.Translate( translate_to );
        another.StretchCentralWidth( 1. + i * delta_w);
        another.Add();
    }
    for(int i = 0; i < n_changes; ++i )
    {
        UnitRoadSegment another( segment );
        QPointF translate_to( i * delta_x, -300.);
        another.Translate( translate_to );
        another.StretchEndWidth( 1. + i * delta_w);
        another.Add();
    }
}

void TestRoadSegments::TestRotation()
{
    const int n_changes = 10;
    const qreal delta_a = 10.;
    const qreal delta_x = 150.;

    UnitRoadSegment segment;
    for(int i = 0; i < n_changes; ++i )
    {
        UnitRoadSegment another( segment );
        QPointF translate_to( i * delta_x, 0.);
        another.Translate( translate_to );
        another.Rotate( 1. + i * delta_a);
        another.Add();
    }
}

void TestRoadSegments::TestBends()
{
    const qreal min_angle = 0;
    const qreal max_angle = 360;
    const qreal delta_angle = 15;
    const qreal delta_x = 300.;

    UnitRoadSegment segment;

    // uniform width
    int i = 0;
/*    for(qreal angle = min_angle; angle <= max_angle; angle += delta_angle )
    {
        UnitRoadSegment first_part( segment );
        UnitRoadSegment second_part( segment );
        QPointF translate_to( i * delta_x - 3.*sqrt(delta_x) * i , 0.);
        first_part.Translate( translate_to );
        second_part.Rotate( angle );
        first_part.AddUnitRoadSegment( second_part );
        first_part.Add();
        ++i;
    }
*/
    // variable widths
    i = 0;
    qreal max_widening = 10.;
    for(qreal angle = min_angle; angle <= max_angle; angle += delta_angle )
    {
        UnitRoadSegment first_part( segment );
        UnitRoadSegment second_part( segment );
        QPointF translate_to( i * delta_x - 3.*sqrt(delta_x) * i , 200.);
        first_part.Translate( translate_to );
        qreal widening = 1. + max_widening*angle/(max_angle-min_angle);
        first_part.StretchBeginWidth(widening);
        second_part.Rotate( angle );
        first_part.AddUnitRoadSegment( second_part );
        first_part.Add();
        ++i;
    }
}

void TestRoadSegments::TestConnections()
{
    const qreal min_angle = 0.;
    const qreal max_angle = 180.;
    const qreal delta_angle = 5.;
    const qreal delta_x = 300.;

    int i = 0;
    for(qreal angle = min_angle; angle <= max_angle; angle += delta_angle)
    {
        UnitRoadSegment road1;
        QPointF translate_to(i * delta_x, 0.);
        road1.Translate(translate_to);
        //road1.StretchLength(2.);
        road1.Rotate(45.);
        road1.Add();
        road1.Rotate(angle);
        road1.Add();
        ++i;
    }
}



void TestRoadSegments::TestIntersection3()
{
    const qreal min_angle = 10.;
    const qreal max_angle = 170.;
    const qreal delta_angle = 10.;
    const qreal delta_x = 300.;

    int i = 0;
    for(qreal angle = min_angle; angle <= max_angle; angle += delta_angle)
    {
        UnitRoadSegment road1;
        QPointF translate_to(i * delta_x, 0.);
        road1.Translate(translate_to);
        //road1.StretchLength(2.);
        road1.Add();
        road1.Rotate(180.);
        road1.Add();
        road1.Rotate(angle);
        road1.Add();
        ++i;
    }
}

