#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "stdafx.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //void PaintRoadNetwork( RoadNetwork graph );
    void paintEvent(QPaintEvent *);
    void wheelEvent(QWheelEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

private:
    Ui::MainWindow *ui;
    //RoadNetwork m_RoadNetwork;
    bool m_MouseButtonPressed;
    qreal m_Scale; // meters in pixel
    qreal m_YScaleFactor; // meters in pixel
    qreal m_OffsetX;
    qreal m_OffsetY;
    int m_MouseMovedX;
    int m_MouseMovedY;
};

#endif // MAINWINDOW_H
