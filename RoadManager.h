#ifndef ROADMANAGER_H
#define ROADMANAGER_H

#include "stdafx.h"
#include "Formulaes.h"

class RoadSegment;
class RoadConnection;
class Intersection;

class RoadManager
{
public:
    static RoadManager& GetInstance()
    {
        static RoadManager theRoadManager;
        return theRoadManager;
    }
    void AddRoadSegment( const RoadSegment& segment );
    void AddRoadConnection( const RoadConnection& connection );
    bool SplitRoadSegment( size_t id, std::set<size_t> point );
    void AddIntersection( const Intersection& intersection );
    RoadSegment* GetRoadSegment( unsigned long id );
    RoadConnection* GetRoadConnection(unsigned long id);
    const Intersection* GetIntersection(unsigned long id) const;
    std::vector< RoadSegment > GetRoadSegments( const std::vector< unsigned long > ids ) const;
    const std::vector< RoadSegment >& GetAllRoadSegments() const;
    const std::vector< RoadConnection >& GetAllRoadConnections() const;
    const std::vector< Intersection >& GetAllIntersections() const;

    void SetEdges( const EarthCoordinates& min, const EarthCoordinates& max );
    void GetEdges( EarthCoordinates& min, EarthCoordinates& max ) const;
    const EarthCoordinates&  GetCenter();
    void GetEdgesXY( QPointF& min, QPointF& max ) const;
    void GetSizesXY( qreal& width, qreal& height ) const;
    const QPointF& GetCenterXY() const;

    typedef boost::geometry::model::point<qreal, 2, boost::geometry::cs::cartesian> point;
    typedef boost::geometry::model::linestring<point> roadsegment;
    typedef std::pair<point, unsigned> value;

    void PreconstructRoadSegments();
    void ConstructRoadSegments();
    void ConstructIntersections();
    void ConstructRoadConnections();

    void FindIntersections();

private:
    RoadManager();
    RoadManager(const RoadManager& );
    RoadManager& operator=(const RoadManager&);

    void PopulateTree( boost::geometry::index::rtree< value, boost::geometry::index::quadratic<16> >& rtree ) const;

    std::vector< RoadSegment > m_RoadSegments;
    std::vector< RoadConnection > m_RoadConnections;
    std::vector< Intersection > m_Intersections;
    EarthCoordinates m_MinLatLong;
    EarthCoordinates m_MaxLatLong;
    EarthCoordinates m_CenterLatLong;
    QPointF m_MinXY;
    QPointF m_MaxXY;
    QPointF m_CenterXY;

};

#endif // ROADMANAGER_H
