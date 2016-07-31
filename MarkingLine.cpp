#include "MarkingLine.h"

MarkingLine::MarkingLine()
{
}

MarkingLine::MarkingLine( QPainterPath* path )
    : Drawable(new QPainterPath(*path))
{
}

MarkingLine::MarkingLine( const MarkingLine& other )
    : Drawable(other)
    , m_Type(other.m_Type)
{
}

MarkingLine& MarkingLine::operator=( const MarkingLine& other )
{
    m_Type=other.m_Type;
    return *this;
}

void MarkingLine::Draw()
{
    QPen pen_1_1(Qt::white, 0.15, Qt::SolidLine);
    QPen pen_1_2_1(Qt::white, 0.2, Qt::SolidLine);
    QPen pen_1_3(Qt::black, 0.15, Qt::SolidLine);
    QPen pen_1_4(Qt::yellow, 0.2, Qt::SolidLine);

    QPen pen_1_5_city(Qt::black, 0.15); // 0.2?
    QVector<qreal> dashes_1_5_city; dashes_1_5_city << 3 << 9;
    pen_1_5_city.setDashPattern(dashes_1_5_city);
    QPen pen_1_5_road(Qt::white, 0.15); // 0.2?
    QVector<qreal> dashes_1_5_road; dashes_1_5_road << 4 << 12;
    pen_1_5_road.setDashPattern(dashes_1_5_road);

    QPen pen_1_6_city(Qt::white, 0.15); // 0.2?
    QVector<qreal> dashes_1_6_city; dashes_1_6_city << 6 << 2;
    pen_1_6_city.setDashPattern(dashes_1_6_city);
    QPen pen_1_6_road(Qt::white, 0.15); // 0.2?
    QVector<qreal> dashes_1_6_road; dashes_1_6_road << 9 << 3;
    pen_1_6_road.setDashPattern(dashes_1_6_road);

    QPen pen_1_7(Qt::white, 0.15); // 0.2?
    QVector<qreal> dashes_1_7; dashes_1_7 << 0.5 << 0.5;
    pen_1_7.setDashPattern(dashes_1_7);

    QPen pen_1_8(Qt::white, 0.2); // 0.2?
    QVector<qreal> dashes_1_8; dashes_1_8 << 1. << 3;
    pen_1_8.setDashPattern(dashes_1_8);
    QPen pen_1_8_freeway(Qt::white, 0.4); // 0.2?
    pen_1_8_freeway.setDashPattern(dashes_1_8);

    QPen pen_1_10(Qt::yellow, 0.2); // 0.2?
    QVector<qreal> dashes_1_10; dashes_1_10 << 1. << 1;
    pen_1_10.setDashPattern(dashes_1_10);

    QPen pen;
    switch(m_Type){
    case line_1_3 : pen=pen_1_3; break;
    case line_1_5_city : pen=pen_1_5_city; break;
    case line_1_6_city : pen=pen_1_6_city; break;
    default : pen=pen_1_5_city;
    }

    m_Painter->setPen(pen_1_1);
    //m_Painter->setBrush(Qt::NoBrush);
    m_Painter->drawPath(*m_Path);
}


void MarkingLine::SetType(MarkingType type)
{
    m_Type = type;
}
