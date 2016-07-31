#include "stdafx.h"

RoadManager::RoadManager()
{
}

void RoadManager::AddRoadSegment( const RoadSegment& segment )
{
    //std::cout  << "Add segment with " << segment.GetNumLanes() << " lanes" << std::endl;

    /*
    for(size_t i = 0; i < segment.GetNumLanes(); ++i)
    {
        std::string dir;
        if(segment.GetLane(i)->GetDirections() == true) dir = "forward";
        if(segment.GetLane(i)->GetDirections() == false) dir = "backward";
        if(boost::indeterminate( segment.GetLane(i)->GetDirections() )) dir = "both";

        std::cout << "Lane " << i << ": width:" << segment.GetLane(i)->GetWidth() << " direction: " << dir << std::endl;
    }
*/

    m_RoadSegments.push_back( segment );
}

bool RoadManager::SplitRoadSegment( size_t id, std::set<size_t> points )
{
    if( id >= m_RoadSegments.size() )
    {
        std::cerr << "Wrong id!" << std::endl;
        return false;
    }
    bool result = false;

    RoadSegment orig_segment = m_RoadSegments[ id ];
    std::vector< QPointF > original = orig_segment.GetCoordinates();
    if(original.size()<=2) return false;
    size_t begin = 0;

    size_t i = 1;
    for(auto point: points)
    {
        //std::cout << "In Split: point = " << point << " from " << original.size() << " name = " << orig_segment.GetName() << " id = " << id << std::endl;
        if(point > original.size() )
        {
            std::cerr << "Wrong point number!" << std::endl;
            return false;
        }
        if(point == 0 || point == (original.size() - 1) ) continue;
        std::vector< QPointF > segm1, segm2;
        std::copy(original.begin()+begin, original.begin()+point+1, std::back_inserter(segm1));
        std::copy(original.begin()+point, original.end(), std::back_inserter(segm2));
        RoadSegment new_segment = orig_segment;
        new_segment.SetCoordinates(segm1);
        AddRoadSegment(new_segment);
        m_RoadSegments[ id ].SetCoordinates(segm2);
        result = true;
        begin=point;
        ++i;
    }
    return result;
}

RoadSegment* RoadManager::GetRoadSegment( unsigned long id )
{
    if( id < m_RoadSegments.size() )
        return &m_RoadSegments[id];
    else
        return nullptr;
}

std::vector< RoadSegment > RoadManager::GetRoadSegments( const std::vector< unsigned long > ids ) const
{
    std::vector< RoadSegment > result;
    for( auto it: ids )
    {
        if( it < m_RoadSegments.size() )
        {
            result.push_back(m_RoadSegments[it]);
        }
    }
    return result;
}

void RoadManager::AddRoadConnection( const RoadConnection& connection )
{
    m_RoadConnections.push_back(connection);
}

void RoadManager::AddIntersection( const Intersection& intersection )
{
    m_Intersections.push_back(intersection);
}

RoadConnection* RoadManager::GetRoadConnection(unsigned long id)
{
    if( id < m_RoadConnections.size() )
        return &m_RoadConnections[id];
    else
        return nullptr;
}

const Intersection* RoadManager::GetIntersection(unsigned long id) const
{
    if( id < m_Intersections.size() )
        return &m_Intersections[id];
    else
        return nullptr;
}

const std::vector< RoadSegment >& RoadManager::GetAllRoadSegments() const
{
    //std::cout << "RoadManager::GetAllRoadSegments(): " << m_RoadSegments.size() << " segments." << std::endl;
    return m_RoadSegments;
}

const std::vector< RoadConnection >& RoadManager::GetAllRoadConnections() const
{
    return m_RoadConnections;
}

const std::vector< Intersection >& RoadManager::GetAllIntersections() const
{
    return m_Intersections;
}

void RoadManager::SetEdges( const EarthCoordinates& min, const EarthCoordinates& max )
{
    m_MinLatLong = min;
    m_MaxLatLong = max;
    m_CenterLatLong.latitude = (max.latitude + min.latitude)/2.;
    m_CenterLatLong.longitude = (max.longitude + min.longitude)/2.;
    m_MinXY = LongLat2Mercator( m_MinLatLong, m_CenterLatLong.longitude );
    m_MaxXY = LongLat2Mercator( m_MaxLatLong, m_CenterLatLong.longitude );
    m_CenterXY = LongLat2Mercator( m_CenterLatLong, m_CenterLatLong.longitude );
    m_MinXY.setY( m_MinXY.y()- m_CenterXY.y());
    m_MaxXY.setY( m_MaxXY.y() - m_CenterXY.y());
}

void RoadManager::GetEdges( EarthCoordinates& min, EarthCoordinates& max ) const
{
    min = m_MinLatLong;
    max = m_MaxLatLong;
}

const EarthCoordinates&  RoadManager::GetCenter()
{
    return m_CenterLatLong;
}

void RoadManager::GetEdgesXY( QPointF& min, QPointF& max ) const
{
    min = m_MinXY;
    max = m_MaxXY;
}

void RoadManager::GetSizesXY( qreal& width, qreal& height ) const
{
    width = fabs( m_MaxXY.x() - m_MinXY.x() );
    height = fabs( m_MaxXY.y() - m_MinXY.y() ) / cos(m_CenterLatLong.latitude * DEG_TO_RAD);
}

const QPointF& RoadManager::GetCenterXY() const
{
    return m_CenterXY;
}

void RoadManager::PreconstructRoadSegments()
{
    for( size_t i = 0; i < m_RoadSegments.size(); ++i )
    {
        m_RoadSegments[ i ].ConstructCentralPath();
    }
}

void RoadManager::ConstructRoadSegments()
{
    for( size_t i = 0; i < m_RoadSegments.size(); ++i )
    {
        m_RoadSegments[ i ].ConstructPath();
    }
}

void RoadManager::ConstructIntersections()
{
    for( size_t i = 0; i < m_Intersections.size(); ++i )
    {
        m_Intersections[ i ].ConstructPath();
    }
}

void RoadManager::ConstructRoadConnections()
{
    for( size_t i = 0; i < m_RoadConnections.size(); ++i )
    {
        m_RoadConnections[ i ].ConstructPath();
    }
}

void RoadManager::PopulateTree( boost::geometry::index::rtree< value, boost::geometry::index::quadratic<16> >& rtree ) const
{
    rtree.clear();
    size_t segments_before = m_RoadSegments.size();
    for(size_t is=0; is < segments_before ; ++is)
    {
        std::vector<QPointF> coords = m_RoadSegments[is].GetCoordinates();
        for(size_t ip=0; ip<coords.size(); ++ip)
        {
            point p(coords[ip].x(), coords[ip].y());
            value v(p, is);
            rtree.insert(v);
        }
    }
}

void RoadManager::FindIntersections()
{

    namespace bg = boost::geometry;
    namespace bgi = boost::geometry::index;

    bgi::rtree<value, bgi::quadratic<16> > rtree;
    size_t segments_before = m_RoadSegments.size();
    std::vector<value> returned_values;
    qreal dist = 0.0001;

    PopulateTree(rtree);

    std::set<std::pair<qreal, qreal> > points;
    rtree.query(bgi::satisfies([](value const&){ return true; }),
                boost::make_function_output_iterator([&points](value const& v){
                //std::cout << bg::get<0>(v.first) << " ; " <<  bg::get<1>(v.first) << std::endl;
                points.insert(std::make_pair(bg::get<0>(v.first), bg::get<1>(v.first)));
              }));

    //std::cout << "Found: " << points.size() << " points of intersection" << std::endl;

    std::map<size_t, std::set<size_t> > splits;
    for(auto it: points )
    {
        rtree.query(bgi::satisfies([&](value const& v) {return bg::distance(v.first, point(it.first, it.second)) < dist;}),
                std::back_inserter(returned_values));
        //if(returned_values.size() != 1)
        //{
            //std::cout << "returned_values.size() = " << returned_values.size() << " : ";
            //for(auto iret: returned_values)
            //{
            //    std::cout << " " << iret.second << "; ";
            //}
            //std::cout << std::endl;
        //}

        if(returned_values.size()>=2)
        {
            for(auto iret: returned_values)
            {
                qreal x=bg::get<0>(iret.first);
                qreal y=bg::get<1>(iret.first);
                QPointF center(x, y);           // intersection point
                size_t segment_num = iret.second;
                for(auto iret1: returned_values)
                {
                    if(segment_num == iret1.second) continue;
                    std::vector<QPointF> coords = m_RoadSegments[iret1.second].GetCoordinates();
                    for(size_t ipoint = 0; ipoint < coords.size(); ++ipoint)
                    {
                        if(Length(center, coords[ipoint]) < dist)
                        {
                            //std::cout << "point is at segment id = " << iret1.second  << " point = " << ipoint << std::endl;
                            splits[ iret1.second ].insert( ipoint );
                        }
                    }
                }
            }
        }
        returned_values.clear();
    }
    std::set<size_t> segments_to_update;
    for(auto old_segm: splits)
    {
        if( SplitRoadSegment(old_segm.first, old_segm.second) )
        {
            segments_to_update.insert(old_segm.first);
        }
    }

    for(auto it: segments_to_update)
    {
        //std::cout << "updated segment: ConstructPath: " << it << std::endl;
        m_RoadSegments[ it ].ConstructPath();
    }
    for(size_t new_segm = segments_before; new_segm < m_RoadSegments.size(); ++new_segm)
    {
        //std::cout << "new segment: ConstructPath: " << new_segm << std::endl;
        m_RoadSegments[ new_segm ].ConstructPath();
    }

    points.clear();
    returned_values.clear();

    //std::cout << "tree size: " << rtree.size()<< std::endl;
    PopulateTree(rtree);

    rtree.query(bgi::satisfies([](value const&){ return true; }),
                boost::make_function_output_iterator([&points](value const& v){
                //std::cout << bg::get<0>(v.first) << " ; " <<  bg::get<1>(v.first) << std::endl;
                points.insert(std::make_pair(bg::get<0>(v.first), bg::get<1>(v.first)));
              }));

    //std::cout << "Found: " << points.size() << " points of intersection" << std::endl;
    for(auto it: points )
    {
        rtree.query(bgi::satisfies([&](value const& v) {return bg::distance(v.first, point(it.first, it.second)) < dist;}),
                std::back_inserter(returned_values));
        if(returned_values.size()<2)
        {
            returned_values.clear();
            continue;
        }
        if(returned_values.size()==2)
        {
            qreal x = bg::get<0>(returned_values[1].first);
            qreal y = bg::get<1>(returned_values[1].first);
            RoadConnection connection;
            connection.SetCenter(QPointF(x, y));
            connection.AddRoadSegments(returned_values[0].second, returned_values[1].second);
            m_RoadConnections.push_back(connection);
            //std::cout << "Connection: " << returned_values[0].second << " and " << returned_values[1].second << std::endl;
            //std::cout << x << " ; " << y << std::endl;
        } else
        {
            qreal x, y;
            Intersection intersection;
            //std::cout << "Intersection: ";
            for(auto itr: returned_values)
            {
                x = bg::get<0>(itr.first);
                y = bg::get<1>(itr.first);
                intersection.SetCenter(QPointF(x, y));
                intersection.AddRoadSegment( itr.second);
                //std::cout << " add RS " << itr.second;
            }
            m_Intersections.push_back(intersection);
            //std::cout << std::endl;
        }
        returned_values.clear();
    }

}
