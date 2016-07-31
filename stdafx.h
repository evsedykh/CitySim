#ifndef STDAFX_H
#define STDAFX_H

// STL headers
#include <vector>
#include <list>
#include <map>
#include <set>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <utility>
#include <initializer_list>
#include <algorithm>
#include <cmath>
#include <tuple>

// Boost headers
//#include <boost/graph/graph_traits.hpp>
//#include <boost/graph/adjacency_list.hpp>
#include <boost/logic/tribool.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/math/special_functions/sign.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/function_output_iterator.hpp>

// Qt
#define QT_NO_KEYWORDS
#include <QtGlobal>
#include <QApplication>
#include <QMainWindow>
#include <QWheelEvent>
#include <QPainter>
#include <QPen>
#include <QBrush>

//typedef boost::adjacency_list< boost::listS, boost::vecS, boost::undirectedS > RoadGraph;

struct EarthCoordinates
{
    EarthCoordinates()
        : longitude( 0. ), latitude( 0. ){ }
    EarthCoordinates( qreal lon, qreal lat )
        : longitude( lon ), latitude( lat ) { }
    qreal longitude;
    qreal latitude;
};

enum Direction{Left, Right};

enum Segment{Begin, End, None};

enum MarkingType{ line_1_1, line_1_2_1, line_1_2_2, line_1_3, line_1_4, line_1_5_city, line_1_5_rural,
                  line_1_6_city, line_1_6_rural, line_1_7, line_1_8, line_1_8_motorway, line_1_10,
                  line_1_11_city_left, line_1_11_city_right, line_1_11_rural_left, line_1_11_rural_right };

#include "Geometry.h"
#include "Drawable.h"
#include "CrossingDataImpl.h"
#include "BusStopDataImpl.h"
#include "TrafficSignalsDataImpl.h"
#include "GiveWaySignDataImpl.h"
#include "StopSignDataImpl.h"

#include "RoadSegmentDataImpl.h"
#include "RoadSegment.h"
#include "RoadManager.h"
// tests
#include "UnitRoadSegment.h"
#include "TestRoadSegments.h"

// own headers
#include "Singleton.h"
#include "DataReader.h"
//#include "RoadNetwork.h"
#include "MarkingLine.h"
#include "LaneLimits.h"
#include "RoadConnection.h"
#include "Intersection.h"
#include "mainwindow.h"
#include "Constants.h"
//#include "RoadNetworkGenerator.h"

#endif // STDAFX_H
