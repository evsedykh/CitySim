#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_Scale = 1.6; // meters
    m_OffsetX = 0.;
    m_OffsetY = 0.;
    m_MouseButtonPressed = false;
}
/*
void MainWindow::PaintRoadNetwork( RoadNetwork network )
{
    m_RoadNetwork = network;
}*/

void MainWindow::paintEvent( QPaintEvent* )
{
    //RoadGraph graph = m_RoadNetwork.GetRoadGraph();
    //KeyPointsMap key_points = m_RoadNetwork.GetKeyPointsMap();

    // draw key points

    std::shared_ptr< QPainter > painter( new QPainter( this ) );

    RoadManager& theRoadManager( RoadManager::GetInstance() );

    qreal map_width, map_height;
    theRoadManager.GetSizesXY(map_width, map_height);

    m_YScaleFactor = 1.;// / cos(theRoadManager.GetCenter().latitude * DEG_TO_RAD);

/*
    min_xy.x /= m_Scale;
    min_xy.y /= m_Scale;
    max_xy.x /= m_Scale;
    max_xy.y /= m_Scale;
*/
    //QSize win_sz = ui->centralWidget->size();

    //std::cout << "[ " << min_xy.x << "; " << min_xy.y << " ] - [ " << max_xy.x << "; " << max_xy.y << " ]";
    //std::cout << " win_sz: " << win_sz.width() << "; " << win_sz.height() << " map_width: " << map_width << "; " << map_height << std::endl;


/*    std::pair<vertex_iter, vertex_iter> vp;
    for (vp = boost::vertices(graph); vp.first != vp.second; ++vp.first)
    {
        auto& kp = key_points[ *vp.first ];
        painter.drawEllipse( QRectF( QPointF(kp.first-point_size, kp.second-point_size), QPointF(kp.first+point_size, kp.second+point_size)));
    }
*/
    //painter.drawLine(100, 100, 200, 200 );



    auto segments = theRoadManager.GetAllRoadSegments();
    if(!segments.empty())
    {
        auto& first_segment(*segments.begin());
        first_segment.SetPainter( painter.get() );
        first_segment.SetScale( m_Scale );
        first_segment.SetOffsets( m_OffsetX, m_OffsetY );
    }
    for( auto it: segments)
    {
        it.Draw();
    }

    auto intersections = theRoadManager.GetAllIntersections();
    for( auto it: intersections)
    {
        it.Draw();
    }

    auto connections = theRoadManager.GetAllRoadConnections();
    for( auto it: connections)
    {
        it.Draw();
    }

/*
    typedef boost::graph_traits<RoadGraph>::edge_iterator edge_iter;
    std::pair<edge_iter, edge_iter> ep;
    //edge_iter ei, ei_end;
    for (ep = boost::edges(graph); ep.first != ep.second; ++ep.first)
    {
        auto u=boost::source(*ep.first,graph);
        auto v=boost::target(*ep.first,graph);
        //painter.drawLine(QPointF(key_points[u].first, key_points[u].second), QPointF(key_points[v].first, key_points[v].second));
        road_painter.Draw2LanesRoad( key_points[u].first, key_points[u].second, key_points[v].first, key_points[v].second);
        //std::cout << u << "; " << v << std::endl;
    }
       //
*/
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    int numSteps = event->delta()/120;

    if(numSteps>0)
    {
        while (numSteps && m_Scale / 2. > 0.04 )
        {
            m_Scale /= 2.;
            numSteps-=1;
        }
    } else
    {
        while (numSteps && m_Scale * 2. < 50. )
        {
            m_Scale *= 2.;
            numSteps+=1;
        }
    }

    update();
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if(event->button()==Qt::LeftButton)
    {
        m_MouseButtonPressed = true;
        QPointF pos = event->screenPos();
        m_MouseMovedX = pos.x();
        m_MouseMovedY = pos.y();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    if(event->button()==Qt::LeftButton)
    {
        m_MouseButtonPressed = false;
        QPointF pos = event->screenPos();
        qreal offset_x = pos.x()-m_MouseMovedX;
        qreal offset_y = pos.y()-m_MouseMovedY;
        if(offset_x!=0)m_OffsetX += offset_x;
        if(offset_y!=0)m_OffsetY += offset_y;
        update();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{

    if(m_MouseButtonPressed)
    {
        QPointF pos = event->screenPos();
        qreal offset_x = pos.x()-m_MouseMovedX;
        qreal offset_y = pos.y()-m_MouseMovedY;
        if(offset_x!=0)m_OffsetX += offset_x;
        if(offset_y!=0)m_OffsetY += offset_y;
        update();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
