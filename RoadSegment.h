#ifndef ROADSEGMENT_H
#define ROADSEGMENT_H

//#include "stdafx.h"
#include "OsmData.h"

class LaneLimits;
class MarkingLine;

typedef std::pair<MarkingLine, MarkingType> MarkingLineSegment;

struct RoadSegmentAngleInfo
{
    size_t SegmentId;
    qreal SegmentAngle;
    bool operator<(const RoadSegmentAngleInfo& other) const { return SegmentAngle < other.SegmentAngle; }
};

class RoadSegment
        : public Drawable
        , public RoadSegmentDataImpl
{
public:
    RoadSegment();
    RoadSegment( const RoadSegmentDataImpl& impl, OsmData& osm_data );
    RoadSegment( std::initializer_list< EarthCoordinates > coords );
    ~RoadSegment();
    RoadSegment( const RoadSegment& other );
    RoadSegment& operator=( const RoadSegment& other );

    void AddCoordinate( const EarthCoordinates& coords );
    void AddXYCoordinate( const QPointF& coords );
    void SetCoordinates( const std::vector<EarthCoordinates >& coords );
    void SetCoordinates( const std::vector<QPointF >& coords );
    void ClearCoordinates();
    void AddLane( const LaneLimits& limits );
    void RemoveLane( size_t lane );
    void RemoveAllLanes();

    void AddMarkingLine(Direction dir, qreal offset, int begin, int end, MarkingType type);
    void AddDoubleLine( qreal begin, qreal end );

    void SetMaxSpeed( unsigned int speed );
    void SetTurnRules(const std::string& rules);
    void SetTurnRulesBackward(const std::string& rules);

    void SetName( const std::string& name );
    const std::string& GetName() const;

    void SetOneway();
    bool IsOneway() const;

    void SetRoundabout();
    bool IsRoundabout() const;
    void Draw();

    const std::vector< QPointF > GetCoordinates() const;
    size_t GetNumLanes() const;
    const LaneLimits* GetLane( size_t lane ) const;

    void SetCentralWidth( qreal width);
    qreal GetCentralWidth() const;
    void SetBeginWidth( qreal width);
    qreal GetBeginWidth() const;
    void SetEndWidth( qreal width);
    qreal GetEndWidth() const;
    qreal GetWidth( qreal percent ) const;

    const QPainterPath& GetPath() const;
    std::pair<QPointF, QPointF> GetBeginPoints( qreal offset = 0. ) const;
    std::pair<QPointF, QPointF> GetEndPoints( qreal offset = 0. ) const;

    void GetNormal( const QPointF& relative_to, Segment segm, qreal offset, QLineF& result ) const;
    Segment GetBeginEnd( const QPointF& point ) const;
    void ConstructCentralPath();
    void ConstructPath();

    void AddLegthTag( int tag, qreal len );
    void Print() const;

private:

    qreal m_CentralWidth;
    mutable qreal m_BeginWidth;
    mutable qreal m_EndWidth;
    int m_LeftBeginElement;
    int m_RightEndElement;

    std::string m_Name;
    bool m_Oneway;
    bool m_Roundabout;
    unsigned int m_SpeedLimit;

    std::vector< QPointF > m_Coordinates;
    std::vector< LaneLimits > m_Lanes;
    std::map< int, qreal > m_Tags;
    std::vector< MarkingLineSegment > m_MarkingLines;
    std::shared_ptr< QPainterPath > m_CentralPath;
};

#endif // ROADSEGMENT_H
