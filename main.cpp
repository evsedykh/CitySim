#include "stdafx.h"

#include "BaseDataReaderImpl.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::string input_data(argv[1]);
    std::cout << "Input data: " << input_data << std::endl;
    DataReader* reader = new DataReader("xml");
    Osm::OsmFilter filter;
    Osm::OsmFilterKeysValues roads, road_crossings, road_bus_stops, road_traffic_signals, road_give_way, road_stop;
    Osm::OsmFilterElement filter_roads, filter_road_crossings, filter_road_bus_stops, filter_road_traffic_signals, filter_road_give_way, filter_road_stop;

    // Roads
    roads.ObligatoryKey = "highway";
    roads.ObligatoryValues = { "residential", "living_street", "unclassified", "tertiary", "secondary", "primary", "trunk", "motorway",
                            "tertiary_link", "secondary_link", "primary_link", "trunk_link", "motorway_link" };
    roads.AcceptedValues = { "name", "surface", "side_road", "trolley_wire", "lit", "maxspeed", "maxspeed:practical", "oneway",
                          "lanes", "lanes:forward", "lanes:backward", "smoothness", "width", "junction:roundabout",
                          "turn:lanes", "turn:lanes:backward", "turn:lanes:forward", "bridge", "toll", "layer", "hgv", "living_street",
                          "overtaking" };
    filter_roads.Entity = Osm::Way;
    filter_roads.KeysValues = roads;
    filter.AddFilter( filter_roads );

    // Crossings
    road_crossings.ObligatoryKey = "highway";
    road_crossings.ObligatoryValues = { "crossing" };
    road_crossings.AcceptedValues = { "crossing", "crossing_ref", "button_operated" };
    filter_road_crossings.Entity = Osm::Node;
    filter_road_crossings.KeysValues = road_crossings;
    filter.AddFilter( filter_road_crossings );

    // Bus stops
    road_bus_stops.ObligatoryKey = "highway";
    road_bus_stops.ObligatoryValues = { "bus_stop" };
    road_bus_stops.AcceptedValues = { "ref", "local_ref", "name" };
    filter_road_bus_stops.Entity = Osm::Node;
    filter_road_bus_stops.KeysValues = road_bus_stops;
    filter.AddFilter( filter_road_bus_stops );

    // Traffic signals
    road_traffic_signals.ObligatoryKey = "highway";
    road_traffic_signals.ObligatoryValues = { "traffic_signals" };
    road_traffic_signals.AcceptedValues = { "crossing", "crossing_ref", "button_operated" };
    filter_road_traffic_signals.Entity = Osm::Node;
    filter_road_traffic_signals.KeysValues = road_traffic_signals;
    filter.AddFilter( filter_road_traffic_signals );

    // Give way signs
    road_give_way.ObligatoryKey = "highway";
    road_give_way.ObligatoryValues = { "give_way" };
    road_give_way.AcceptedValues = { };
    filter_road_give_way.Entity = Osm::Node;
    filter_road_give_way.KeysValues = road_give_way;
    filter.AddFilter( filter_road_give_way );

    // Stop signs
    road_stop.ObligatoryKey = "highway";
    road_stop.ObligatoryValues = { "stop" };
    road_stop.AcceptedValues = { };
    filter_road_stop.Entity = Osm::Node;
    filter_road_stop.KeysValues = road_stop;
    filter.AddFilter( filter_road_stop );

    reader->SetFilter( &filter );
    RoadManager& theRoadManager( RoadManager::GetInstance() );
    EarthCoordinates p1( 30.38, 59.99 );
    EarthCoordinates p2( 30.40, 60.01 );
    theRoadManager.SetEdges( p1, p2 );

    std::unique_ptr< OsmData > data = reader->ReadData( input_data );


    for( auto it: data->m_RoadSegments )
    {
        RoadSegment segment( it, *data );
        theRoadManager.AddRoadSegment( segment );
    }

    //std::cout << "sizeof = " << sizeof(RoadSegmentDataImpl< 16 >) << std::endl;

  /*  RoadNetworkGenerator generator;
    generator.Generate();
    RoadNetwork network = generator.GetRoadNetwork();*/

/*
    std::string input_data(argv[1]);
    std::cout << "Input data: " << input_data << std::endl;
    DataReader data_reader;
    data_reader.ReadRoadData(input_data);
    //data_reader.ReadRoadData("/home/eugene/Projects/RoadEditor/data/grazhdanka.geojson");
*/

/*
    EarthCoordinates p1( 60., 30.);
    EarthCoordinates p2( 60.01, 30. );
    EarthCoordinates p3( 60.02, 30. );
    theRoadManager.SetEdges( p1, p3 );

    RoadSegment segment{p1, p2, p3};

    segment.SetBeginWidth(150.);
    segment.SetCentralWidth(100.);
    segment.SetEndWidth(150.);

    theRoadManager.AddRoadSegment(segment);
*/
/*
    TestRoadSegments test;
    test.TestBends();
    //test.TestStretch();
    //test.TestConnections();
*/
    theRoadManager.PreconstructRoadSegments();
    theRoadManager.FindIntersections();
    theRoadManager.ConstructIntersections();
    theRoadManager.ConstructRoadConnections();
    theRoadManager.ConstructRoadSegments();

    MainWindow w;
    w.show();

    //w.PaintRoadNetwork(network);

    return a.exec();
}
