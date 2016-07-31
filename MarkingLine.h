#ifndef MARKINGLINE_H
#define MARKINGLINE_H

#include "stdafx.h"

class MarkingLine :
        public Drawable
{
public:
    MarkingLine();
    MarkingLine( QPainterPath* path );
    MarkingLine( const MarkingLine& other );
    MarkingLine& operator=( const MarkingLine& other );

    virtual void Draw();

    void SetType(MarkingType type);
private:
    MarkingType m_Type;

};

#endif // MARKINGLINE_H
