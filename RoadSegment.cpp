#include "stdafx.h"
#include "Formulaes.h"

QPainter* Drawable::m_Painter = nullptr;
qreal Drawable::m_OffsetX = 0.f;
qreal Drawable::m_OffsetY = 0.f;
qreal Drawable::m_Scale = 1.6f;

RoadSegment::RoadSegment()
    : m_CentralWidth(1.),
      m_BeginWidth(1.),
      m_EndWidth(1.),
      m_Oneway( false ),
      m_Roundabout( false ),
      m_SpeedLimit(60)
{
    m_Tags[0]=0.;
    m_Tags[1]=0.;
}

RoadSegment::RoadSegment( const RoadSegmentDataImpl& impl, OsmData& osm_data )
    : RoadSegmentDataImpl( impl)
    , m_CentralWidth( impl.width * 0.5 )
    , m_BeginWidth( impl.width * 0.5 )
    , m_EndWidth( impl.width * 0.5 )
    , m_Oneway( impl.oneway)
    , m_Roundabout( false )
    , m_SpeedLimit( 60. )
{
    m_Tags[0]=0.;
    m_Tags[1]=0.;
    for( auto it: impl.nodes )
    {
        AddCoordinate( EarthCoordinates( osm_data.m_Nodes[ it ].m_Longitude, osm_data.m_Nodes[ it ].m_Latitude ));
    }
}


RoadSegment::RoadSegment( std::initializer_list< EarthCoordinates > coords )
{
    for( auto it: coords)
    {
        AddCoordinate( it );
    }
}

RoadSegment::~RoadSegment()
{
}

RoadSegment::RoadSegment( const RoadSegment& other )
    : Drawable(other),
      m_CentralWidth(other.m_CentralWidth),
      m_BeginWidth(other.m_BeginWidth),
      m_EndWidth(other.m_EndWidth),
      m_Name( other.m_Name ),
      m_Oneway( other.m_Oneway ),
      m_Roundabout( other.m_Roundabout ),
      m_SpeedLimit(other.m_SpeedLimit),
      m_Coordinates( other.m_Coordinates ),
      m_Lanes( other.m_Lanes ),
      m_Tags(other.m_Tags),
      m_MarkingLines(other.m_MarkingLines),
      m_CentralPath(other.m_CentralPath)
{
}

RoadSegment& RoadSegment::operator=( const RoadSegment& other )
{
    m_Coordinates = other.m_Coordinates;
    m_Name = other.m_Name;
    m_Lanes = other.m_Lanes;
    m_CentralWidth = other.m_CentralWidth;
    m_BeginWidth = other.m_BeginWidth;
    m_EndWidth = other.m_EndWidth;
    m_Oneway = other.m_Oneway;
    m_Roundabout = other.m_Roundabout;
    m_SpeedLimit = other.m_SpeedLimit;
    m_Tags = other.m_Tags;
    m_MarkingLines = other.m_MarkingLines;
    m_CentralPath = other.m_CentralPath;
    return *this;
}

void RoadSegment::AddCoordinate( const EarthCoordinates& coords )
{
    RoadManager& theRoadManager( RoadManager::GetInstance() );
    const EarthCoordinates& map_center = theRoadManager.GetCenter();
    const QPointF& center_xy = theRoadManager.GetCenterXY();

    QPointF xy = LongLat2Mercator( coords, map_center.longitude );
    xy.setY( xy.y() - center_xy.y());

    std::cout << "x = " << xy.x() << " y = " << xy.y() << std::endl;
    std::cout << "lon = " << coords.longitude << " lat = " << coords.latitude << std::endl;
    std::cout << "mc: lon = " << map_center.longitude << " lat = " << map_center.latitude << std::endl;


    m_Coordinates.push_back( xy );
}

void RoadSegment::AddXYCoordinate( const QPointF& coords )
{
    m_Coordinates.push_back( coords );
}

void RoadSegment::SetCoordinates( const std::vector< EarthCoordinates >& coords )
{
    RoadManager& theRoadManager( RoadManager::GetInstance() );
    m_Coordinates.reserve(coords.size());
    const QPointF& map_center = theRoadManager.GetCenterXY();
    QPointF xy;
    for(size_t i= 0; i < coords.size(); ++i )
    {
        xy = LongLat2Mercator(coords[i], theRoadManager.GetCenter().longitude);
        xy.setY( xy.y() - map_center.y());
        m_Coordinates.push_back(xy);
    }
}

void RoadSegment::SetCoordinates( const std::vector<QPointF >& coords )
{
    m_Coordinates = coords;
}

void RoadSegment::ClearCoordinates()
{
    m_Coordinates.clear();
}

void RoadSegment::AddLegthTag( int tag, qreal len )
{
    m_Tags[tag]=len;
}

void RoadSegment::AddLane( const LaneLimits& limits )
{
    m_Lanes.push_back( limits );
    m_CentralWidth += limits.GetWidth();
    m_BeginWidth = m_CentralWidth;
    m_EndWidth = m_CentralWidth;
}

void RoadSegment::RemoveLane( size_t lane )
{
    if (lane <= ( m_Lanes.size() - 1 ) )
    {

        m_CentralWidth -= m_Lanes[lane].GetWidth();
        m_Lanes.erase( m_Lanes.begin() + lane);
    }
    m_BeginWidth = m_CentralWidth;
    m_EndWidth = m_CentralWidth;
}

void RoadSegment::RemoveAllLanes()
{
    m_Lanes.clear();
    m_CentralWidth = 1.;
    m_BeginWidth = m_CentralWidth;
    m_EndWidth = m_CentralWidth;
}

void RoadSegment::AddMarkingLine(Direction dir, qreal offset, int begin, int end, MarkingType type)
{
    QPainterPath stroke_central;
    qreal begin_len = m_Tags[begin];
    qreal end_len = m_Tags[end];

    MakeDerivedPath(*m_CentralPath, dir, offset, begin_len, end_len, stroke_central);
    MarkingLine central(&stroke_central);
    central.SetType(type);
    MarkingLineSegment next_element=std::make_pair(central, type);
    m_MarkingLines.push_back(next_element);
}

void RoadSegment::Draw()
{

    m_Painter->setRenderHint(QPainter::Antialiasing, true);

    QPen pen_road(Qt::darkGreen, 1.5, Qt::SolidLine);
    QPen pen_cosmetic(Qt::darkGray, 0.1, Qt::SolidLine);
    QPen pen_line(Qt::white, 0.3, Qt::SolidLine);
    QBrush brush_road(Qt::darkGray);
    QBrush brush_no(Qt::NoBrush);

    m_Painter->setBrush(brush_road);
    m_Painter->setPen(pen_cosmetic);

    QMatrix matrix;
    matrix.translate( m_Painter->device()->width() / 2. + m_OffsetX*m_Scale, m_Painter->device()->height() / 2. + m_OffsetY*m_Scale );
    matrix.scale( m_Scale, -m_Scale );
    m_Painter->setMatrix( matrix );

    m_Painter->drawPath( *m_Path );

    m_Painter->setPen(pen_line);
    m_Painter->setBrush(brush_no);

    m_Painter->drawPath(*m_CentralPath);

    //m_Painter->setPen(pen_line);
    //m_Painter->setBrush(Qt::cyan);
    //for(int i = 0; i <  m_CentralPath->elementCount(); ++i)
    //{
    //    m_Painter->drawEllipse(QPointF(m_CentralPath->elementAt(i).x, m_CentralPath->elementAt(i).y), 2., 2.);
    //}

    //for(auto it: m_MarkingLines)
    //{
    //    it.first.Draw();
    //}

    /*QPainterPath net;
    for( int i = 1; i < 5; ++i)
    {
        MakeDerivedPath(*m_CentralPath, Left, i*3., 0, 1, net);
        std::cout << "n=" << net.elementCount() << std::endl;
    }

    m_Painter->drawPath(net);
*/

    /*m_Painter->setPen(pen_road);
    auto begin = GetBeginPoints(0.);
    auto end = GetEndPoints(0.);
    m_Painter->setBrush(QBrush(Qt::green));
    m_Painter->drawEllipse(QPoint(begin.first.x, begin.first.y), 3, 3);
    m_Painter->drawEllipse(QPoint(begin.second.x, begin.second.y), 3, 3);
    m_Painter->setBrush(QBrush(Qt::blue));
    m_Painter->drawEllipse(QPoint(end.first.x, end.first.y), 3, 3);
    m_Painter->drawEllipse(QPoint(end.second.x, end.second.y), 3, 3);*/
}

void RoadSegment::SetName( const std::string& name )
{
    m_Name = name;
}

const std::string& RoadSegment::GetName() const
{
    return m_Name;
}

void RoadSegment::SetMaxSpeed( unsigned int speed )
{
    m_SpeedLimit = speed;
}

void RoadSegment::SetTurnRules(const std::string& )
{
    //std::cout << "turn rules: " << rules << std::endl;
}

void RoadSegment::SetTurnRulesBackward(const std::string& )
{
    //std::cout << "backward turn rules: " << rules << std::endl;
}

void RoadSegment::SetOneway()
{
    m_Oneway = true;
}

bool RoadSegment::IsOneway() const
{
    return m_Oneway;
}

void RoadSegment::SetRoundabout()
{
    m_Roundabout = true;
}

bool RoadSegment::IsRoundabout() const
{
    return m_Roundabout;
}

const std::vector< QPointF > RoadSegment::GetCoordinates() const
{
    return m_Coordinates;
}

size_t RoadSegment::GetNumLanes() const
{
    return m_Lanes.size();
}

void RoadSegment::SetCentralWidth( qreal width )
{
    m_CentralWidth = width;
}

qreal RoadSegment::GetBeginWidth() const
{
    return m_BeginWidth;
}

qreal RoadSegment::GetCentralWidth() const
{
    return m_CentralWidth;
}

qreal RoadSegment::GetEndWidth() const
{
    return m_EndWidth;
}

void RoadSegment::SetBeginWidth( qreal width)
{
    m_BeginWidth = width;
}

void RoadSegment::SetEndWidth( qreal width)
{
    m_EndWidth = width;
}

qreal RoadSegment::GetWidth( qreal percent ) const
{
    return ::GetWidth( m_BeginWidth, m_CentralWidth, m_EndWidth, m_CentralPath->length(), m_CentralPath->length() * percent );
}

const LaneLimits* RoadSegment::GetLane( size_t lane ) const
{
    if (lane <= ( m_Lanes.size() - 1 ) )
    {
        return &m_Lanes[ lane ];
    } else
    {
        return nullptr;
    }
}

const QPainterPath& RoadSegment::GetPath() const
{
    try{
        return *m_CentralPath;
    }
    catch (...)
    {
        std::cerr << "Bad pointer to path" << std::endl;
    }
}

std::pair<QPointF, QPointF> RoadSegment::GetBeginPoints( qreal offset ) const
{
    qreal percent = m_CentralPath->percentAtLength( offset );
    qreal delta = 0.001;
    if(percent >=(1.-delta) ) delta = -0.001;
    QPointF central_point = m_CentralPath->pointAtPercent( percent );
    QPointF central_point_1 = m_CentralPath->pointAtPercent( percent + delta );
    qreal angle = m_CentralPath->angleAtPercent( percent );
    QLineF normal( central_point, central_point_1 );
    normal.setLength( GetWidth( percent ) / 2. );
    normal.setAngle( angle - 90.);
    QPointF result_right( normal.p2());
    normal.setAngle( angle + 90.);
    QPointF result_left( normal.p2());
    return std::make_pair(result_right, result_left);
}

std::pair<QPointF, QPointF> RoadSegment::GetEndPoints( qreal offset ) const
{
    qreal length = m_CentralPath->length();
    qreal percent = m_CentralPath->percentAtLength( length - offset );
    qreal delta = 0.001;
    if(percent >=(1.-delta) )delta = -0.001;
    QPointF central_point = m_CentralPath->pointAtPercent( percent);
    QPointF central_point_1 = m_CentralPath->pointAtPercent( percent + delta );
    qreal angle = m_CentralPath->angleAtPercent( percent );
    QLineF normal( central_point, central_point_1 );
    normal.setLength( GetWidth( percent ) / 2. );
    normal.setAngle( angle - 90.);
    QPointF result_right( normal.p2());
    normal.setAngle( angle + 90.);
    QPointF result_left( normal.p2());
    return std::make_pair(result_right, result_left);
}

void RoadSegment::GetNormal( const QPointF& relative_to, Segment segm, qreal offset, QLineF& result ) const
{
    std::pair<QPointF, QPointF> far_points;
    if(segm == Begin)
    {
        far_points = GetBeginPoints(offset);
    }
    else if(segm == End)
    {
        far_points = GetEndPoints(offset);
    }
    QLineF right_line( relative_to, far_points.first );
    QLineF left_line( relative_to, far_points.second );
    if((right_line.angle() > left_line.angle()))
    {
        std::swap(far_points.first, far_points.second);
    }
    if((fabs(right_line.angle() - left_line.angle())>180.) )
    {
        std::swap(far_points.first, far_points.second);
    }

    result = QLineF( far_points.first, far_points.second );
}

Segment RoadSegment::GetBeginEnd( const QPointF& point ) const
{
    QPointF begin( m_Coordinates.front().x(), m_Coordinates.front().y() );
    QPointF end( m_Coordinates.back().x(), m_Coordinates.back().y() );
    Segment result = None;
    if(Length(point, begin) < 0.001)
    {
        result = Begin;
    }
    else if(Length(point, end) < 0.001)
    {
        result = End;
    }
    if(result == None)
    {
        //std::cout << "None edge in RoadSegment::GetBeginEnd." << std::endl;
        //std::cout << "Length to begin: " << Length(point, begin) << " length to end: " << Length(point, end) << std::endl;
    }
    return result;
}

void RoadSegment::ConstructCentralPath()
{
    //std::cout << "ConstructCentralPath()" << std::endl;
    m_CentralPath.reset( new QPainterPath );
    MakePathFromCoordinates(m_Coordinates, *m_CentralPath);
    //std::cout << "Central path:" << std::endl;
    //DumpQPainterPath(*m_CentralPath);
}

void RoadSegment::ConstructPath()
{

    m_Tags[1] = m_CentralPath->length();
    //std::cout << "ConstructPath() len = " << m_CentralPath->length()
    //          << " width = " << m_BeginWidth << " -> " << m_CentralWidth << " -> " << m_EndWidth << std::endl;

    m_Path.reset( new QPainterPath );

    MakeRoundedOutline(*m_CentralPath, m_BeginWidth, m_CentralWidth, m_EndWidth, *m_Path);
    //std::cout << "Outline path:" << std::endl;
    //DumpQPainterPath(*m_Path);

    /*for( int i = 1; i < 10; ++i )
    {
        AddMarkingLine(Direction::Left, 2.*i, 0, 1, MarkingType::line_1_5_city);
        AddMarkingLine(Direction::Right, 2.*i, 0, 1, MarkingType::line_1_5_city);
    }*/

    //MakeWideRoad(*m_CentralPath, m_BeginWidth, m_CentralWidth, m_EndWidth, *m_Path );

    //qreal len = m_CentralPath->length();
    //AddLegthTag(2, 0.1*len);
    //AddLegthTag(3, 0.9*len);

    // double line
/*    size_t lanes(m_Lanes.size());
    if(lanes>=4 && ! m_Oneway ) AddDoubleLine(m_Tags[0], m_Tags[1]);
    if(lanes==2)
    {
        AddMarkingLine( Direction::Left, 0., 0, 1, MarkingType::line_1_5_city);
    }
    if(lanes==3)
    {
        AddMarkingLine( Direction::Left, 1.5, 0, 1, MarkingType::line_1_5_city);
        AddMarkingLine( Direction::Right, 1.5, 0, 1, MarkingType::line_1_5_city);
    }

    if(lanes==4)
    {
        AddMarkingLine( Direction::Left, 3., 0, 1, MarkingType::line_1_5_city);
        AddMarkingLine( Direction::Right, 3., 0, 1, MarkingType::line_1_5_city);
    }
    if(lanes==6)
    {
        AddMarkingLine( Direction::Left, 3., 0, 1, MarkingType::line_1_5_city);
        AddMarkingLine( Direction::Right, 3., 0, 1, MarkingType::line_1_5_city);
        AddMarkingLine( Direction::Left, 6., 0, 1, MarkingType::line_1_5_city);
        AddMarkingLine( Direction::Right, 6., 0, 1, MarkingType::line_1_5_city);
    }

    if(lanes==6)
    {
        AddMarkingLine( Direction::Left, 3., 0, 1, MarkingType::line_1_5_city);
        AddMarkingLine( Direction::Right, 3., 0, 1, MarkingType::line_1_5_city);
        AddMarkingLine( Direction::Left, 6., 0, 1, MarkingType::line_1_5_city);
        AddMarkingLine( Direction::Right, 6., 0, 1, MarkingType::line_1_5_city);
    }
*/

}

void RoadSegment::AddDoubleLine( qreal begin, qreal end )
{
    QPainterPath left, right, line;
    MakeDerivedPath(*m_CentralPath, Direction::Left, 0.19, begin, end, left);
    MakeDerivedPath(*m_CentralPath, Direction::Right, 0.19, begin, end, right);

    line.addPath(right);
    auto left_rev = left.toReversed();
    line.moveTo( left_rev.elementAt(0) );
    line.addPath(left_rev);
    line.moveTo( right.elementAt(0) );
    MarkingLine central(&line);
    central.SetType(line_1_3);
    m_MarkingLines.push_back(std::make_pair(central, line_1_3));
}

void RoadSegment::Print() const
{
    std::cout << "Segment from " << m_Coordinates.size() << " points. Width formula: "
              << m_BeginWidth << " - " << m_CentralWidth << " - " << m_EndWidth << std::endl;
    for( auto it: m_Coordinates )
    {
        std::cout << " ( " << it.x() << " , " << it.y() << " ) - " << std::endl;
    }
}
