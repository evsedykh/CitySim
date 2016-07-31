#include "stdafx.h"
#include "OsmXmlReader.h"
#include "OsmGeojsonReader.h"
/*
DataReader::DataReader()
    : DataReader("xml")
{
}*/

DataReader::DataReader(const std::string& format)
{
    if ( format == "xml" )
    {
        m_Reader.reset( new OsmXmlReader );
    } else if ( format == "geojson" )
    {
        m_Reader.reset( new OsmGeojsonReader );
    } else
    {
        std::cerr << "Wrong format: " << format << std::endl;
    }
}

DataReader::~DataReader()
{
}

std::unique_ptr< OsmData > DataReader::ReadData(const std::string &filename)
{
    if( ! m_Reader )
    {
        return nullptr;
    }

    // two pass reading:
    // 1) ways and relations
    // 2) then nodes of interest
    m_Reader->ReadData( filename, false );
    m_Reader->ReadData( filename, true );

    return m_Reader->GetData();
}

void DataReader::SetFilter( Osm::OsmFilter* filter)
{
    if( m_Reader )
    {
        m_Reader->SetFilter( filter );
    }
}

/*
std::unique_ptr< OsmData > DataReader::ReadData( const std::string& filename )
{
    std::set<std::string> highway_tags;

    std::ifstream f(filename);
    if( ! f )
    {
        std::cerr << "Problem opening file with road data: " << filename << std::endl;
        return;
    }
    RoadManager& theRoadManager( RoadManager::GetInstance() );

    LaneLimits lane_central;
    LaneLimits lane_edge;
    LaneLimits lane_backward;
    LaneLimits lane_forward;

    lane_central.SetBothDirections();
    lane_edge.SetWidth(3.5);
    lane_backward.SetBackwardDirectionOnly();

    json_spirit::Value value;
    json_spirit::read( f, value );
    json_spirit::Object obj = value.get_obj();
    //std::cout << json_spirit::write(value) << std::endl;
    qreal min_lon = 1000.;
    qreal max_lon = -1000.;
    qreal min_lat = 1000.;
    qreal max_lat = -1000.;

    for( auto it_primary : obj )
    {
        //std::cout << it_primary.name_ << std::endl;

        if(it_primary.name_ == "bbox" )
        {
            json_spirit::Array bbox = it_primary.value_.get_array();
            EarthCoordinates min( bbox[0].get_real(), bbox[1].get_real() );
            EarthCoordinates max( bbox[2].get_real(), bbox[3].get_real() );
            theRoadManager.SetEdges( min, max );
            QPointF min_xy, max_xy;
            theRoadManager.GetEdgesXY(min_xy, max_xy);
            std::cout << std::endl << min.latitude << "; " << min.longitude << " - " << max.latitude << "; " << max.longitude << std::endl;
            std::cout << "[ " << min_xy.x() << "; " << min_xy.y() << " ] - [ " << max_xy.x() << "; " << max_xy.y() << " ]" << std::endl;
        }

        if(it_primary.name_ == "features")
        {
            json_spirit::Array streets = it_primary.value_.get_array();
            for( auto it_street : streets )
            {
                std::string hw_type;
                int lanes = -1;
                bool oneway = false;
                int lanes_fwd = -1;
                int lanes_back = -1;
                std::string primitive="point";
                RoadSegment segment;

                json_spirit::Object obj_street = it_street.get_obj();

                for(auto it_prop: obj_street)
                {
                    //std::cout << "prop: " <<  it_prop.name_ << std::endl;

                    if(it_prop.name_=="properties")
                    {
                        json_spirit::Value properties = it_prop.value_;
                        json_spirit::Object obj_properties = properties.get_obj();

                        for(auto it: obj_properties)
                        {
                            //std::cout << it.name_ << ": " << it.value_.get_str() << std::endl;

                            if(it.name_ == "highway")
                            {
                                hw_type = it.value_.get_str();
                                //std::cout << hw_type << std::endl;
                                highway_tags.insert(hw_type);
                            }
                            if(it.name_ == "lanes")
                            {
                                lanes = boost::lexical_cast<int>(it.value_.get_str());
                            }
                            if(it.name_ == "lanes:forward")
                            {
                                lanes_fwd = boost::lexical_cast<int>(it.value_.get_str());
                            }

                            if(it.name_ == "lanes:backward")
                            {
                                lanes_back = boost::lexical_cast<int>(it.value_.get_str());
                            }
                            if(it.name_ == "name")
                            {
                                segment.SetName(it.value_.get_str());
                            }
                            if(it.name_ == "oneway")
                            {
                                if(it.value_.get_str() == "yes")
                                {
                                    oneway = true;
                                }
                            }
                            if(it.name_ == "junction")
                            {
                                if(it.value_.get_str() == "roundabout")
                                {
                                    segment.SetRoundabout();
                                }
                            }
                            if(it.name_ == "maxspeed")
                            {
                                if(it.value_.get_str() == "RU:urban")
                                {
                                    segment.SetMaxSpeed(60);
                                } else if(it.value_.get_str() == "RU:rural")
                                {
                                    segment.SetMaxSpeed(90);
                                }
                            }
                            if(it.name_ == "turn:lanes" || it.name_ == "turn:lanes:forward")
                            {
                                segment.SetTurnRules(it.value_.get_str());
                            }
                            if(it.name_ == "turn:lanes:backward")
                            {
                                segment.SetTurnRulesBackward(it.value_.get_str());
                            }
                        }
                    }

                    // geography
                    if(it_prop.name_ == "geometry" &&
                            (
                                hw_type == "unclassified"
                                || hw_type == "residential"
                                || hw_type == "tertiary"
                                || hw_type == "secondary"
                                || hw_type == "primary"
                              ) )
                    {
                        json_spirit::Value geo = it_prop.value_;
                        json_spirit::Object obj_geo = geo.get_obj();

                        for(auto it: obj_geo)
                        {
                            if(it.name_ == "type" && it.value_.get_str()=="LineString") primitive = "linestring";
                            if(it.name_ == "type" && it.value_.get_str()=="Point") primitive = "point";
                        }
                        //std::cout << "primitive: " << primitive << std::endl;
                        for(auto it: obj_geo)
                        {
                            if(primitive=="linestring" && it.name_=="coordinates" )
                            {

                                json_spirit::Array coords = it.value_.get_array();
                                qreal latitude = 0.;
                                qreal longitude = 0.;

                                for( auto it_coords : coords )
                                {
                                    json_spirit::Array obj_coord = it_coords.get_array();


                                    int i = 0;

                                    for( auto it_c: obj_coord )
                                    {
                                        if( i == 0 ) longitude = it_c.get_real();
                                        if( i == 1 ) latitude = it_c.get_real();
                                        ++i;
                                    }
                                    EarthCoordinates c( longitude, latitude);
                                    segment.AddCoordinate( c );
                                    if(latitude<min_lat)min_lat=latitude;
                                    if(latitude>max_lat)max_lat=latitude;
                                    if(longitude<min_lon)min_lon=longitude;
                                    if(longitude>max_lon)max_lon=longitude;

                                }
                            }
                        }
                    }
                }

                if(oneway)
                {
                    segment.SetOneway();
                    if(lanes==-1)
                    {
                        //std::cout << "No lanes info!!!" << std::endl;
                        lanes = 2;
                    }
                    for(int i = 0; i < lanes ; ++i)
                    {
                        segment.AddLane(lane_forward);
                    }
                } else {
                    if(lanes_back!=-1 && lanes_fwd!=-1)
                    {
                        for(int i = 0; i < lanes_fwd ; ++i)
                        {
                            segment.AddLane(lane_forward);
                        }
                        for(int i = 0; i < lanes_back ; ++i)
                        {
                            segment.AddLane(lane_backward);
                        }
                    } else if (lanes_back==-1 && lanes_fwd==-1 )
                    {
                        if(lanes==-1)
                        {
                            //std::cout << "No lanes info!!!" << std::endl;
                            lanes=4;
                        }
                        switch(lanes){
                        case 1: segment.AddLane(lane_forward); break;
                        case 2: segment.AddLane(lane_forward); segment.AddLane(lane_backward); break;
                        case 3: segment.AddLane(lane_forward); segment.AddLane(lane_central); segment.AddLane(lane_backward); break;
                        case 4: segment.AddLane(lane_forward); segment.AddLane(lane_forward);
                                segment.AddLane(lane_backward); segment.AddLane(lane_backward); break;
                        case 5: if(!oneway)std::cout << "Should be oneway!!!" << std::endl; break;
                        case 6:  segment.AddLane(lane_forward); segment.AddLane(lane_forward);
                                 segment.AddLane(lane_forward); segment.AddLane(lane_forward);
                                 segment.AddLane(lane_backward); segment.AddLane(lane_backward); break;
                        }
                    }
                }

                if(segment.GetNumLanes()==0)std::cout << "Zero lanes! " << segment.GetName() << std::endl;
                if(primitive=="linestring")theRoadManager.AddRoadSegment(segment);
            }
        }
    }
}
*/



