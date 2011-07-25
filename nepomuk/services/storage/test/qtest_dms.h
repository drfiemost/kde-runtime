/*
   This file is part of the Nepomuk KDE project.
   Copyright (C) 2011 Sebastian Trueg <trueg@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) version 3, or any
   later version accepted by the membership of KDE e.V. (or its
   successor approved by the membership of KDE e.V.), which shall
   act as a proxy defined in Section 6 of version 3 of the license.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef QTEST_DMS_H
#define QTEST_DMS_H

#include <QtTest>
#include <Soprano/Statement>
#include <Soprano/Node>
#include <Soprano/Model>

namespace QTest {
template<>
inline char* toString(const Soprano::Node& node) {
    return qstrdup( node.toN3().toLatin1().data() );
}

template<>
inline char* toString(const Soprano::Statement& s) {
    return qstrdup( (s.subject().toN3() + QLatin1String(" ") +
                     s.predicate().toN3() + QLatin1String(" ") +
                     s.object().toN3() + QLatin1String(" . ")).toLatin1().data() );
}

}

namespace Nepomuk {
    void insertOntologies( Soprano::Model *model, const QUrl &ontologyGraph );
}

#endif // QTEST_DMS_H
