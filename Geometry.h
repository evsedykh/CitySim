#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "stdafx.h"

struct AnglePoints
{
    AnglePoints(){}
    AnglePoints( const AnglePoints& other )
        : P1(other.P1), P(other.P), P2(other.P2) {}
    QPointF P1;
    QPointF P;
    QPointF P2;
};

struct ArcProperties
{
    ArcProperties(){}
    ArcProperties( const ArcProperties& other )
        : Center( other.Center ), Radius( other.Radius ) { }
    QPointF Center;
    qreal Radius;
};

struct SymmetricPoints
{
    QPointF left;
    QPointF right;
};

void MakePathFromCoordinates(const std::vector<QPointF>& coords, QPainterPath& path );

void GetLineFrom( const QPainterPath& path, int num, QLineF& result );

void DumpQPainterPath( const QPainterPath& path );

void MakeNormal( const QLineF& origin, QLineF& result, Segment segment = Begin);

void MakeParallel( const QLineF& origin, QLineF& result, qreal distance, Direction dir = Right);

void MakeAngleConnection( const AnglePoints& angle_points, qreal width, QPainterPath& result );

void MakeRoundedOutline(const QPainterPath& origin, qreal width_begin, qreal width, qreal width_end, QPainterPath& result);

void MakeWideRoad( const QPainterPath& origin, qreal width_begin, qreal width, qreal width_end, QPainterPath& result );

void MakeDerivedPath( const QPainterPath& original, Direction dir, qreal offset, qreal begin, qreal end, QPainterPath& derived );

void NormalizeAngleDeg( qreal& angle );

void NormalizeAngleRad( qreal& angle );

qreal Length(const QPointF& p1, const QPointF& p2);

qreal GetWidth(qreal width_begin, qreal width, qreal width_end, qreal length, qreal curr_len);

void GetCornerPoints( const QPainterPath& origin, qreal width_begin, qreal width_end, SymmetricPoints& begin_points, SymmetricPoints& end_points );

#endif // GEOMETRY_H
