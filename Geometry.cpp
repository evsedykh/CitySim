#include "stdafx.h"

void MakePathFromCoordinates(const std::vector<QPointF>& coords, QPainterPath& path )
{
    //std::cout << "MakePathFromCoordinates(): " << coords.size() << " points"<< std::endl;
    path.moveTo(coords[0].x(), coords[0].y());
    QLineF first_line(coords[0].x(), coords[0].y(), coords[1].x(), coords[1].y());
    qreal prev_angle = first_line.angle();
    for( size_t i = 1; i < coords.size(); ++i )
    {
        path.lineTo( coords[i].x(), coords[i].y());
        QLineF curr_line(coords[i-1].x(), coords[i-1].y(), coords[i].x(), coords[i].y());
        qreal angle = curr_line.angle();
        qreal delta = angle - prev_angle;
        NormalizeAngleDeg(delta);
        /*if(fabs(delta-180.)<20. )
        {
            std::cout << "dA = " << delta << std::endl;
        }*/
    }
}

void MakeDerivedPoint( const QPointF& xy0, const QPointF& xy1, Direction dir, qreal offset, QPointF& result )
{
    qreal sign = 1.;
    if(Direction::Left==dir) sign = -1.;
    qreal dx = xy1.x() - xy0.x();
    qreal dy = xy1.y() - xy0.y();
    qreal sx =0., sy = 0.;
    result = xy1;
    if( fabs(dx) < epsilon )
    {
        result.setY( result.y() - offset*sign);
    }else if(fabs(dy) < epsilon)
    {
        result.setX( result.x() - offset*sign);
    } else
    {
        qreal alpha = atan2(dx, dy);
        sx = offset*sin(alpha);
        sy = offset*cos(alpha);
        result.setX( result.x() - sx);
        result.setY( result.y() - sy);
    }
}

void MakeDerivedEndPoint( const QPointF& xy0, const QPointF& xy1, Direction dir, qreal offset, Segment segment, QPointF& result )
{
    qreal sign = 1.;
    if(Direction::Left==dir) sign = -1.;
    qreal dx = xy1.x() - xy0.x();
    qreal dy = xy1.y() - xy0.y();
    qreal alpha = M_PI / 2.;
    qreal sx =0., sy = 0.;
    if(segment == Begin) result = xy0; else result = xy1;
    if( fabs(dx) < epsilon )
    {
        result.setX( result.x() - offset*sign);
    }else if(fabs(dy) < epsilon)
    {
        result.setY( result.y() + offset*sign);
        alpha = 0.;
    } else
    {
        alpha = atan2(dx, dy);
        sx = offset*sin(alpha);
        sy = offset*cos(alpha);
        result.setX( result.x() - sx);
        result.setY( result.y() - sy);
    }
}

QPointF GetPointFrom( const QPainterPath& path, int num)
{
    auto elem = path.elementAt(num);
    return QPointF(elem.x, elem.y);
}

void GetLineFrom( const QPainterPath& path, int num, QLineF& result )
{
    auto elem0 = path.elementAt(num);
    auto elem1 = path.elementAt(num+1);
    result = QLineF(elem0.x, elem0.y, elem1.x, elem1.y);
}

void MakeNormal( const QLineF& origin, QLineF& result, Segment segment)
{
    QLineF line = origin;
    if( segment == End ) line = QLineF(origin.p2(), origin.p1());
    line.setLength( 1. );
    result = line.normalVector();
}

void GetNormalPoint( const QPainterPath& origin, qreal length, qreal distance, Direction dir, QPointF& point )
{
    qreal origin_length = origin.length();
    qreal percent = length/origin_length;
    if(percent > 1.)
    {
        percent = 1.;
    }
    if(percent < 0.)
    {
        percent = 0.;
    }
    QPointF curr_point( origin.pointAtPercent(percent));
    qreal angle = origin.angleAtPercent(percent);
    QLineF line(curr_point, QPointF(0., 0.));
    line.setAngle(angle+90);
    if(dir == Left)
    {
        line.setAngle(angle+270);
    }
    line.setLength(distance);
    point = line.p1();
}

void MakeParallel( const QLineF& origin, QLineF& result, qreal distance, Direction dir)
{
    QLineF normal;
    MakeNormal( origin, normal );
    normal.setLength(distance);
    if( dir == Left ) normal.setAngle(normal.angle() + 180.);
    MakeNormal( normal, result, End);
    result.setLength(origin.length());
}

qreal Sqr(qreal x)
{
    return x*x;
}

void NormalizeAngleRad( qreal& angle )
{
    int full_rounds = angle / (2.*M_PI);
    angle -= 2. * M_PI * full_rounds;
    angle = (angle<0.)?(angle+2.*M_PI):angle;
}

void NormalizeAngleDeg( qreal& angle )
{
    int full_rounds = angle / (360.);
    angle -= 360. * full_rounds;
    angle = (angle<0.)?(angle+360.):angle;
}

qreal Angle( const QLineF& line1, const QLineF& line2 )
{
    qreal result = line1.angle() - line2.angle();
    NormalizeAngleDeg(result);
    return result;
}

void DumpQPainterPath( const QPainterPath& path )
{
    int elems = path.elementCount();
    std::cout << "Path contains " << elems << " elements:" << std::endl;
    int count = 1;
    for(int i = 0; i < elems; ++i)
    {
        std::cout << count << ". ";
        QPainterPath::Element elem = path.elementAt(i);

        if ( elem.isMoveTo() )
        {
            std::cout << "moveTo :";
        }
        if ( elem.isLineTo() )
        {
            std::cout << "lineTo :";
        }
        if ( elem.isCurveTo() )
        {
            std::cout << "curveTo";
            QPainterPath::Element elem_curve1 = path.elementAt(i+1);
            QPainterPath::Element elem_curve2 = path.elementAt(i+2);
            i+=2;
            std::cout << ": ( " << elem_curve1.x << " ; " << elem_curve1.y << " ), ( " << elem_curve2.x << " ; " << elem_curve2.y << " ), ";
        }
        std::cout << "( " << elem.x << " ; " << elem.y << " )" << std::endl;
        ++count;
    }
}

std::pair< qreal, qreal > GetBeginEndLengths( qreal width_begin, qreal width, qreal width_end, qreal length )
{
    const qreal ratio = 50.;
    qreal dwb = fabs(width-width_begin);
    qreal dwe = fabs(width-width_end);
    qreal len_begin = dwb*ratio;
    qreal len_end = dwe*ratio;
    qreal sum_len = len_begin+len_end;
    if(sum_len>length)
    {
        len_begin *= length/sum_len;
        len_end *= length/sum_len;
    }
    return std::make_pair( len_begin, len_end );
}

qreal GetWidth(qreal width_begin, qreal width, qreal width_end, qreal length, qreal curr_len)
{
    qreal result = width;
    qreal dwb = width-width_begin;
    qreal dwe = width-width_end;
    std::pair<qreal, qreal > len = GetBeginEndLengths(width_begin, width, width_end, length);
    qreal len_begin = len.first;
    qreal len_end = len.second;
    if(curr_len<=len_begin && len_begin > epsilon)
    {
        result += dwb*((curr_len-len_begin)/len_begin);
    }
    if(curr_len>=len_end)
    {
        result -= dwe*(curr_len-len_end)/(length-len_end);
    }
    //std::cout << "GetWidth: w = " << width << " len_b = " << len_begin << " len_e = " << len_end << " curr_len = " << curr_len << " dwb = " << dwb << " dwe = " << dwe
    //          << " w = " << result << std::endl;
    return result;
}

void GetCornerPoints( const QPainterPath& origin, qreal width_begin, qreal width_end, SymmetricPoints& begin_points, SymmetricPoints& end_points )
{
    auto elems = origin.elementCount();
    QLineF line_begin, line_end, normal_b, normal_e, parallel_br, parallel_er, parallel_el, parallel_bl;
    QPointF intersection_point_br, intersection_point_er, intersection_point_el, intersection_point_bl;
    GetLineFrom(origin, 0, line_begin);
    GetLineFrom(origin, elems - 2, line_end);

    MakeNormal(line_begin, normal_b);
    MakeParallel(line_begin, parallel_br, width_begin);
    auto intersect_br = normal_b.intersect(parallel_br, &intersection_point_br);
    if( intersect_br == QLineF::NoIntersection )
    {
        std::cerr << "No intersection at Begin Right!" << std::endl;
    }

    MakeNormal(line_end, normal_e, End);
    MakeParallel(line_end, parallel_er, width_end);
    auto intersect_er = normal_e.intersect(parallel_er, &intersection_point_er);
    if( intersect_er == QLineF::NoIntersection )
    {
        std::cerr << "No intersection at End Right!" << std::endl;
    }

    MakeParallel(line_end, parallel_el, width_end, Left);
    auto intersect_el = normal_e.intersect(parallel_el, &intersection_point_el);
    if( intersect_el == QLineF::NoIntersection )
    {
        std::cerr << "No intersection at End Left!" << std::endl;
    }

    MakeParallel(line_begin, parallel_bl, width_begin, Left);
    auto intersect_bl = normal_b.intersect(parallel_bl, &intersection_point_bl);
    if( intersect_bl == QLineF::NoIntersection )
    {
        std::cerr << "No intersection at Begin Left!" << std::endl;
    }

    begin_points.left = intersection_point_bl;
    begin_points.right = intersection_point_br;
    end_points.left = intersection_point_el;
    end_points.right = intersection_point_er;
}

void SwapLine( const QLineF& line, QLineF& result )
{
    result = QLineF(line.p2(), line.p1());
}

void GetOutlinePoint( const QPainterPath& origin, int point, qreal width, Direction dir, QPointF& result )
{
    QLineF line1, line2, parallel1, parallel2;
    GetLineFrom(origin, point, line1);
    GetLineFrom(origin, point + (dir==Right?1:-1) , line2);
    MakeParallel(line1, parallel1, width, dir);
    MakeParallel(line2, parallel2, width, dir);
    auto intersect_type = parallel1.intersect(parallel2, &result);
    if( intersect_type == QLineF::NoIntersection )
    {
        std::cerr << "No intersection at Outline!" << std::endl;
    }
}

void MakeRoundCorner( const AnglePoints& angle_points, qreal width, ArcProperties& arc, QPointF& c1, QPointF& c2 )
{
    const QPointF& P1(angle_points.P1);
    const QPointF& P(angle_points.P);
    const QPointF& P2(angle_points.P2);

    qreal radius = 6.;
    qreal angle = atan2(P.y() - P1.y(), P.x() - P1.x()) - atan2(P.y() - P2.y(), P.x() - P2.x());
    NormalizeAngleRad(angle);
    if( fabs(angle) < M_PI )
    {
        radius += 2.*width;
    }
    //std::cout << "Corner: angle = " << angle << std::endl;

    qreal segment = radius / fabs(tan(angle/2.));
    qreal PC1 = segment;
    qreal PC2 = segment;

    qreal PP1 = sqrt( Sqr(P.x() - P1.x()) + Sqr(P.y() - P1.y()));
    qreal PP2 = sqrt( Sqr(P.x() - P2.x()) + Sqr(P.y() - P2.y()));

    if(segment > std::min(PP1, PP2) )
    {
        segment = std::min(PP1, PP2);
        radius = segment * fabs(tan(angle/2.));
    }

    qreal P0 = sqrt(segment * segment + radius * radius);

    qreal C1X = P.x() - (P.x() - P1.x()) * PC1 / PP1;
    qreal C1Y = P.y() - (P.y() - P1.y()) * PC1 / PP1;
    qreal C2X = P.x() - (P.x() - P2.x()) * PC2 / PP2;
    qreal C2Y = P.y() - (P.y() - P2.y()) * PC2 / PP2;

    qreal dx = P.x() * 2. - C1X - C2X;
    qreal dy = P.y() * 2. - C1Y - C2Y;
    qreal PC = sqrt(Sqr(dx) + Sqr(dy));

    qreal centerX = P.x() - dx * P0 / PC;
    qreal centerY = P.y() - dy * P0 / PC;

    arc.Center = QPointF( centerX, centerY);
    arc.Radius = radius;
    c1 = QPointF(C1X, C1Y);
    c2 = QPointF(C2X, C2Y);
    //std::cout << "r = " << radius << std::endl;
}

void MakeAngleConnection( const AnglePoints& angle_points, qreal width, QPainterPath& result )
{
    const QPointF& P1(angle_points.P1);
    const QPointF& P(angle_points.P);
    const QPointF& P2(angle_points.P2);

    QLineF line_before(P1, P);
    QLineF line_after(P, P2);

    QLineF parallel_before, parallel_after;
    QPointF intersect;
    MakeParallel(line_before, parallel_before, width);
    MakeParallel(line_after, parallel_after, width);
    QLineF::IntersectType intersect_type = parallel_before.intersect(parallel_after, &intersect );
    qreal angle = Angle( parallel_before, parallel_after );

    //std::cout << "angle = " << angle << std::endl;
    if ( (angle>RoundAngle && angle<(180.-RoundAngle)) || (angle>(180.+RoundAngle) && angle < (360.-RoundAngle)) )
    {
        ArcProperties arc;
        QPointF c1, c2;
        AnglePoints angle_points;
        angle_points.P1 = parallel_before.p1();
        angle_points.P = intersect;
        QLineF normal_end;
        QPointF intersect_normal;
        MakeNormal(line_after, normal_end, End);
        intersect_type = parallel_after.intersect(normal_end, &intersect_normal );

        if(fabs(fabs(parallel_after.angle() - parallel_before.angle())<5.) || fabs(fabs(parallel_after.angle() - parallel_before.angle())-180.)<5. || intersect_type == QLineF::NoIntersection )
        {
            //std::cout << "almost parallel lines: curr: " << parallel_after.angle() << " next: " << parallel_before.angle() << std::endl;
            //intersect_normal.setX(0.5*(parallel_after.x1()+parallel_before.x1()));
            //intersect_normal.setY(0.5*(parallel_after.y1()+parallel_before.y1()));
        }
        angle_points.P2 = intersect_normal;

        MakeRoundCorner( angle_points, width, arc, c1, c2 );
        qreal start_angle, sweep_angle;
        if(angle>180.)
        {
            start_angle = parallel_before.angle()-90.;
            sweep_angle = 360.-angle;
        } else
        {
            start_angle = parallel_before.angle()+90.;
            sweep_angle = -angle;
        }

        QPointF top_left( arc.Center.x()-arc.Radius,arc.Center.y()-arc.Radius);
        QPointF bottom_right( arc.Center.x()+arc.Radius,arc.Center.y()+arc.Radius);
        QRectF arc_rect( top_left, bottom_right );

        result.arcTo(arc_rect.x(), arc_rect.y(), arc_rect.width(), arc_rect.height(), start_angle, sweep_angle);
    } else
    {
        QPointF intersect_normal;
        if(fabs(fabs(parallel_after.angle() - parallel_before.angle())<5.) || fabs(fabs(parallel_after.angle() - parallel_before.angle())-180.)<5. || intersect_type == QLineF::NoIntersection )
        {
            //std::cout << "almost parallel lines: curr: " << parallel_after.angle() << " next: " << parallel_before.angle() << std::endl;
            intersect_normal.setX(0.5*(parallel_after.x2()+parallel_before.x1()));
            intersect_normal.setY(0.5*(parallel_after.y2()+parallel_before.y1()));
        } else
        {
            intersect_type = parallel_before.intersect(parallel_after, &intersect_normal );

        }
        result.lineTo(intersect_normal);
    }
}

float round2( float f )
{
    return floor((f * 10 ) + 0.5) / 10;
}

//#define DRAW_NORMALS
void MakeRoundedOutline(const QPainterPath& origin, qreal width_begin, qreal width, qreal width_end, QPainterPath& result)
{
    //std::cout << "full len = " << origin.length() << std::endl;
    SymmetricPoints begin_points, end_points;
    GetCornerPoints(origin, width_begin/2., width_end/2., begin_points, end_points);
    int elems = origin.elementCount();
    result.moveTo(begin_points.right);
    qreal curr_len = 0.;
    std::vector< qreal > tmp_points, points;
    for(int point = 1; point < elems; ++point)
    {
        QPointF P1 = GetPointFrom(origin, point-1);
        QPointF P = GetPointFrom(origin, point);
        curr_len += Length(P1, P);
        tmp_points.push_back( curr_len );
    }
    qreal length = origin.length();
    std::pair<qreal, qreal > len = GetBeginEndLengths(width_begin, width, width_end, length);
    qreal len_begin = len.first;
    qreal len_end = len.second;
    //std::cout << "len_begin = " << len_begin << " len_end = " << len_end << std::endl;
    tmp_points.push_back(0.);
    tmp_points.push_back( len_begin );
    tmp_points.push_back( length - len_end );
    tmp_points.push_back(length);
    std::sort(tmp_points.begin(), tmp_points.end() );
    points.push_back(tmp_points.front());
    for( size_t i = 1; i <= tmp_points.size()-1; ++i )
    {
        if( fabs(tmp_points[i]-tmp_points[i-1])>epsilon ) points.push_back(tmp_points[i]);
    }
    //for( size_t i = 0; i <= points.size()-1; ++i )
    //{
    //    std::cout << "dist = " << points[ i ] << std::endl;
    //}
#ifdef DRAW_NORMALS
    QFont font("Lucida", 5, QFont::Normal);
#endif
    //std::cout << "points.size() = " << points.size() << " and size - 2 = " << (size_t)(points.size()-2) << std::endl;
    for( size_t i = 1; i <= points.size()-2; ++i )
    {
        AnglePoints ap;
        qreal prev_len = points[ i - 1 ];
        qreal prev_width = GetWidth(width_begin/2., width/2., width_end/2., length, prev_len);
        QPointF prev_point, point, next_point;
        GetNormalPoint(origin, prev_len, prev_width, Right, prev_point );
        ap.P1 = prev_point;
#ifdef DRAW_NORMALS
        result.addEllipse(prev_line.p1(), 2, 2);
        result.moveTo(prev_line.p1());
        result.lineTo(prev_line.p2());
        result.addText(prev_line.p2()+=QPointF(5., 20.), font, QString( std::to_string(round2(prev_line.length())).c_str()));
#endif
        qreal len = points[ i ];
        qreal curr_width = GetWidth(width_begin/2., width/2., width_end/2., length, len);
        GetNormalPoint(origin, len, curr_width, Right, point );
        ap.P = point;
#ifdef DRAW_NORMALS
        result.addEllipse(line.p1(), 2, 2);
        result.moveTo(line.p1());
        result.lineTo(line.p2());
        result.addText(line.p2()+=QPointF(5., 20.), font, QString( std::to_string(round2(line.length())).c_str()));
#endif
        qreal next_len = points[ i + 1 ];
        qreal next_width = GetWidth(width_begin/2., width/2., width_end/2., length, next_len);
        GetNormalPoint(origin, next_len, next_width, Right, next_point );
        ap.P2 = next_point;
#ifdef DRAW_NORMALS
        result.addEllipse(next_line.p1(), 2, 2);
        result.moveTo(next_line.p1());
        result.lineTo(next_line.p2());
        result.addText(next_line.p2()+=QPointF(5., 20.), font, QString( std::to_string(round2(next_line.length())).c_str()));
#else
        MakeAngleConnection(ap, curr_width, result);
#endif
    }

#ifndef DRAW_NORMALS
    result.lineTo(end_points.right);
    result.lineTo(end_points.left);
#endif
    for( size_t i = points.size()-2; i >= 1; --i )
    {
        AnglePoints ap;
        //QLineF prev_line, line, next_line;
        QPointF prev_point, point, next_point;
        qreal prev_len = points[ i + 1 ];
        qreal prev_width = GetWidth(width_begin/2., width/2., width_end/2., length, prev_len);
        GetNormalPoint(origin, prev_len, prev_width , Left, prev_point );
        ap.P1 = prev_point;
#ifdef DRAW_NORMALS
        result.addEllipse(prev_line.p1(), 2, 2);
        result.moveTo(prev_line.p1());
        result.lineTo(prev_line.p2());
        result.addText(prev_line.p2()+=QPointF(5., -20.), font, QString( std::to_string(round2(prev_line.length())).c_str()));
#endif
        qreal len = points[ i ];
        qreal curr_width = GetWidth(width_begin/2., width/2., width_end/2., length, len);
        GetNormalPoint(origin, len, curr_width, Left, point );
        ap.P = point;
#ifdef DRAW_NORMALS
        result.addEllipse(line.p1(), 2, 2);
        result.moveTo(line.p1());
        result.lineTo(line.p2());
        result.addText(line.p2()+=QPointF(5., -20.), font, QString( std::to_string(round2(line.length())).c_str()));
#endif
        qreal next_len = points[ i - 1 ];
        qreal next_width = GetWidth(width_begin/2., width/2., width_end/2., length, next_len);
        GetNormalPoint(origin, next_len, next_width, Left, next_point );
        ap.P2 = next_point;
#ifdef DRAW_NORMALS
        result.addEllipse(next_line.p1(), 2, 2);
        result.moveTo(next_line.p1());
        result.lineTo(next_line.p2());
        result.addText(next_line.p2()+=QPointF(5., -20.), font, QString( std::to_string(round2(next_line.length())).c_str()));
#else
        MakeAngleConnection(ap, curr_width, result);
#endif
    }
#ifndef DRAW_NORMALS
    result.lineTo(begin_points.left);
    result.lineTo(begin_points.right);
#endif

#undef DRAW_NORMALS
}

void MakeWideRoad( const QPainterPath& origin, qreal width_begin, qreal width, qreal width_end, QPainterPath& result )
{
    int elems = origin.elementCount();
    // need at least 2 elements
    if(elems < 2 )
    {
        std::cerr << "MakeWideRoad: need at least 2 elements in origin path!" << std::endl;
        return;
    }

    QPainterPath::Element elem0 = origin.elementAt(0); // moveTo p1
    QPainterPath::Element elem1 = origin.elementAt(1); // lineTo next point
    QPainterPath::Element elem_last = origin.elementAt(elems - 1);
    QPainterPath::Element elem_last_but1 = origin.elementAt(elems - 2);

    if ( !elem0.isMoveTo() || !elem1.isLineTo() )
    {
        std::cerr << "MakeWideRoad: wrong 2 first elements: should be moveTo and lineTo!" << std::endl;
    }
    if ( elems != 2 && (!elem_last.isLineTo() || !elem_last_but1.isLineTo()) )
    {
        std::cerr << "MakeWideRoad: wrong 2 last elements: should be lineTo and lineTo!" << std::endl;
    }

    // widths
    qreal dw1 = width - width_begin;
    qreal dw2 = width - width_end;
    qreal len_begin = fabs(dw1) * 50. / 2.;
    qreal len_end = fabs(dw2) * 50. / 2.;

    //std::cout << "w_b = " << width_begin << " w_c = " << width << " w_e = " << width_end << " dw1 = " << dw1 << " dw2 = " << dw2 << std::endl;
    //std::cout << "len_b = " << len_begin << " len_e = " << len_end << std::endl;

    QPointF p1( elem0.x, elem0.y);
    QPointF p2( elem1.x, elem1.y);
    QPointF p_last( elem_last.x, elem_last.y );
    QPointF p_last_but1( elem_last_but1.x, elem_last_but1.y );

    qreal len12 = Length(p1, p2);
    qreal lenlast = Length(p_last_but1, p_last);
    //std::cout << "len12 = " << len12 << " lenlast = " << lenlast << std::endl;

    // intersection of normal at begin and parallel
    QLineF line_begin(p1, p2);
    QLineF line_widthchange(p1, p2);
    line_widthchange.setLength(std::max(len_begin, 1.));
    QLineF normal_begin, parallel_begin, parallel_widthchange_begin, normal_widthchange_begin;
    QPointF begin_normal_right_intersect, begin_normal_left_intersect;
    MakeNormal(line_begin, normal_begin);
    MakeNormal(line_widthchange, normal_widthchange_begin, End);
    MakeParallel(line_begin, parallel_begin, width-dw1*0.5 );

    //std::cout << "width_b = " << width-dw1*0.5 << std::endl;

    MakeParallel(line_begin, parallel_widthchange_begin, width);
    QLineF::IntersectType intersect_type = normal_begin.intersect(parallel_begin, &begin_normal_right_intersect );
    if( intersect_type == QLineF::NoIntersection )
    {
        std::cout << "Right X Begin normal: no intersection!" << std::endl;
    }
    result.moveTo(line_begin.p1());
    result.lineTo(begin_normal_right_intersect);
    if( len_begin>epsilon && len12>len_begin )
    {
        //std::cout << "lineTo widthchange begin" << std::endl;
        normal_widthchange_begin.setLength( width_end );
        normal_widthchange_begin.setAngle(normal_widthchange_begin.angle()+180.);
        result.lineTo( normal_widthchange_begin.p2() );
    }

    // main part of the path - right side
    qreal curr_len = 0.;
    for(int i = 1; i <= elems-2; ++i)
    {
        AnglePoints angle_points;
        angle_points.P1 = QPointF(origin.elementAt(i-1).x, origin.elementAt(i-1).y);
        angle_points.P = QPointF(origin.elementAt(i).x, origin.elementAt(i).y);
        angle_points.P2 = QPointF(origin.elementAt(i+1).x, origin.elementAt(i+1).y);
        qreal len = Length(angle_points.P1, angle_points.P);
        //qreal len2 = Length(angle_points.P, angle_points.P2);
        //std::cout << "len1 = " << len << " len2 = " << len2 << std::endl;
        curr_len += len;
        qreal curr_width = width;
        if( curr_width < len_begin ) curr_width -= dw1 * len / ( 50. * len_begin );
        if( curr_width > len_end ) curr_width -= dw2 * len / ( 50. * len_end );

        MakeAngleConnection( angle_points, width/2., result);
    }

    // intersection of normal at end and parallel
    QLineF line_end(p_last, p_last_but1);
    QLineF line_widthchange_end(p_last, p_last_but1);
    line_widthchange_end.setLength(std::max(len_end, 1.));

    QLineF normal_end, parallel_end, parallel_widthchange_end, normal_widthchange_end;
    QPointF end_normal_right_intersect, end_widthchange_intersect;
    MakeParallel(line_end, parallel_end, width-dw2*0.5, Left );
    MakeParallel(line_end, parallel_widthchange_end, width, Left);
    //std::cout << "width_e = " << width-dw2*0.5 << std::endl;

    MakeNormal(line_end, normal_end);
    MakeNormal(line_widthchange_end, normal_widthchange_end, End);
    intersect_type = normal_end.intersect(parallel_end, &end_normal_right_intersect );
    if( intersect_type == QLineF::NoIntersection )
    {
        std::cout << "Right X End normal: no intersection!" << std::endl;
    }
    if( len_end>epsilon && lenlast > len_end )
    {
        std::cout << "lineTo widthchange end" << std::endl;
        normal_widthchange_end.setLength(width_begin);
        result.lineTo( normal_widthchange_end.p2() );
    }
    result.lineTo(end_normal_right_intersect);


    /*
    MakeParallel(line_begin, parallel_begin, width-dw1*0.5, Left );
    intersect_type = normal_begin.intersect(parallel_begin, &begin_normal_left_intersect );
    if( intersect_type == QLineF::NoIntersection )
    {
        std::cout << "Left X Begin normal: no intersection!" << std::endl;
    }

    result.lineTo(end_normal_left_intersect);*/

    for(int i = elems-2; i >= 1; --i)
    {
        AnglePoints angle_points;
        angle_points.P1 = QPointF(origin.elementAt(i+1).x, origin.elementAt(i+1).y);
        angle_points.P = QPointF(origin.elementAt(i).x, origin.elementAt(i).y);
        angle_points.P2 = QPointF(origin.elementAt(i-1).x, origin.elementAt(i-1).y);

        MakeAngleConnection( angle_points, width/2., result);
    }
/*
    normal_begin.setAngle(normal_begin.angle()+180.);
    QLineF parallel_begin_left;
    MakeParallel(line_begin, parallel_begin_left, width/2., Left);
    intersect_type = normal_begin.intersect(parallel_begin_left, &begin_normal_left_intersect );
    if( intersect_type == QLineF::NoIntersection )
    {
        std::cout << "Left X Begin normal: no intersection!" << std::endl;
        //return;
    }
*/
    result.lineTo(begin_normal_left_intersect);
    result.lineTo(line_begin.p1());
    result.setFillRule(Qt::WindingFill);
}

void MakeDerivedPath( const QPainterPath& original, Direction dir, qreal offset, qreal begin, qreal end, QPainterPath& derived )
{
    if(original.elementCount() < 2) return;

    QPointF xy;
    QPainterPath temp_path;
    qreal len = original.length();
    qreal begin_percent = begin / len;
    qreal end_percent = end / len;

    qreal curr_len = 0.;
    QPointF prev_xy(original.elementAt(0).x, original.elementAt(0).y);
    temp_path.moveTo(prev_xy);
    int begin_elem = -1, end_elem = -1;

    for( int i = 1; i < original.elementCount(); ++i )
    {
        xy = QPointF( original.elementAt(i).x, original.elementAt(i).y);

        curr_len += Length(prev_xy, xy);
        prev_xy = xy;
        if(curr_len>begin && curr_len < end && begin_elem == -1)
        {
            begin_elem = i-1;
            temp_path.moveTo(original.pointAtPercent(begin_percent));
        }else if( fabs(curr_len - begin)< epsilon)
        {
            begin_elem = i-1;
            temp_path.lineTo(original.pointAtPercent(end_percent));
        }
        else if(curr_len>=end && end_elem==-1)
        {
            end_elem = i;
            temp_path.lineTo(original.pointAtPercent(end_percent));
        }
        if(curr_len >= begin && curr_len <= end)
        {
            temp_path.lineTo(original.elementAt(i));
        }
    }

    QPointF xy0( temp_path.elementAt(0).x, temp_path.elementAt(0).y );
    QPointF xy1( temp_path.elementAt(1).x, temp_path.elementAt(1).y );

    QPointF xy_off;
    MakeDerivedEndPoint( xy0, xy1, dir, offset, Begin, xy_off );

    prev_xy = xy0;
    derived.moveTo(xy_off);

    for( int i = 1; i < temp_path.elementCount()-1; ++i )
    {
        xy = QPointF( temp_path.elementAt(i).x, temp_path.elementAt(i).y);

        MakeDerivedPoint( prev_xy, xy, dir, offset, xy_off);
        prev_xy = xy;

        derived.lineTo( xy_off);

    }
    MakeDerivedEndPoint( xy, prev_xy, dir, offset, End, xy_off );

    derived.lineTo(xy_off);
}

qreal Length(const QPointF& p1, const QPointF& p2)
{
    return sqrt((p2.x()-p1.x())*(p2.x()-p1.x())+(p2.y()-p1.y())*(p2.y()-p1.y()));
}

