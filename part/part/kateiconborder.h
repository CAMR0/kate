/* This file is part of the KDE libraries
   Copyright (C) 2001 Anders Lund <anders@alweb.dk>
   Copyright (C) 2001 Christoph Cullmann <cullmann@kde.org>
   Copyright (C) 1999 Jochen Wilhelmy <digisnap@cs.tu-berlin.de>
   
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#ifndef _KateIconBorder_H_
#define _KateIconBorder_H_

#include <qwidget.h>

class KateIconBorder : public QWidget
{
  Q_OBJECT      
  
  public:
    KateIconBorder( class KateViewInternal* internalView );
    ~KateIconBorder() {};

    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;
    
    // TODO: Standardize naming
    void setIconBorder(       bool enable );
    void setLineNumbersOn(    bool enable );
    void setFoldingMarkersOn( bool enable );
    void toggleIconBorder()    { setIconBorder(    !iconBorder() );    }
    void toggleLineNumbersOn() { setLineNumbersOn( !lineNumbersOn() ); }
    bool iconBorder()    const { return m_iconBorderOn;  }
    bool lineNumbersOn() const { return m_lineNumbersOn; }

    // When border options change, updateGeometry() is called.
    // Normally the layout would handle it automatically, but
    // KateViewInternal doesn't use a layout, so emit a signal instead.
    virtual void updateGeometry() { emit sizeHintChanged(); }
    
  signals:
    void sizeHintChanged();
    void toggleRegionVisibility( uint );

  protected:
    void paintEvent( QPaintEvent* );
    void mousePressEvent( QMouseEvent* );
                  
  private:        
    void createMarkMenu();
                      
    class KateView *myView;   
    class KateDocument *myDoc;
    class KateViewInternal *myInternalView;
    class QPopupMenu *markMenu;
                    
    bool m_iconBorderOn:1;
    bool m_lineNumbersOn:1;
    bool m_foldingMarkersOn:1;
    
    bool lmbSetsBreakpoints;
    uint oldEditableMarks;
};
#endif
