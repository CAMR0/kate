/* This file is part of the KDE project
   Copyright 2008-2014 Dominik Haumann <dhaumann kde org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef BTPARSER_H
#define BTPARSER_H

#include <QList>
#include <QString>

class BtInfo
{
public:
    enum Type { Source = 0, Lib, Unknown, Invalid };

    /**
     * Default constructor => invalid element
     */
    BtInfo() = default;

public:
    QString original;
    QString filename;
    QString function;
    QString address;
    int step = -1;
    int line = -1;

    Type type = Invalid;
};

namespace KateBtParser
{
QList<BtInfo> parseBacktrace(const QString &bt);

}

#endif // BTPARSER_H

// kate: space-indent on; indent-width 4; replace-tabs on;
