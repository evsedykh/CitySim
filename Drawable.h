#ifndef DRAWABLE_H
#define DRAWABLE_H

class Drawable
{
public:
    Drawable()
        : m_Path(new QPainterPath)
        , m_Pen(new QPen)
        , m_Brush(new QBrush)
    { }
    Drawable( const Drawable& other )
        : m_Path(other.m_Path)
        , m_Pen(other.m_Pen)
        , m_Brush(other.m_Brush)
    { }
    Drawable( QPainterPath* path )
        : m_Path(path)
        , m_Pen(new QPen)
        , m_Brush(new QBrush)
    { }
    Drawable& operator=( const Drawable& other )
    {
          m_Path  = other.m_Path;
          m_Pen   =  other.m_Pen;
          m_Brush = other.m_Brush;
          return *this;
    }
    virtual ~Drawable()
    {
        m_Path  = nullptr;
        m_Pen   = nullptr;
        m_Brush = nullptr;
    }
    virtual void Draw() = 0;
    void SetPainter( QPainter* painter )
    {
        m_Painter = painter;
    }
    void SetOffsets(qreal x, qreal y)
    {
        m_OffsetX = x;
        m_OffsetY = y;
    }
    void SetScale(qreal scale)
    {
        m_Scale = scale;
    }
protected:
    void SetPath( std::shared_ptr< QPainterPath > path)
    {
        m_Path = path;
    }
    void SetBrush( std::shared_ptr< QBrush > brush)
    {
        m_Brush = brush;
    }
    void SetPen( std::shared_ptr< QPen > pen)
    {
        m_Pen = pen;
    }

    std::shared_ptr< QPainterPath > m_Path;
    std::shared_ptr< QPen > m_Pen;
    std::shared_ptr< QBrush> m_Brush;

    static QPainter* m_Painter;
    static qreal m_OffsetX;
    static qreal m_OffsetY;
    static qreal m_Scale;
};

#endif // DRAWABLE_H
