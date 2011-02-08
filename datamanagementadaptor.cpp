/*
   This file is part of the Nepomuk KDE project.
   Copyright (C) 2010-2011 Sebastian Trueg <trueg@kde.org>

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

   The basis for this file was generated by qdbusxml2cpp version 0.7
   Command line was: qdbusxml2cpp -a datamanagementadaptor -c DataManagementAdaptor -m org.kde.nepomuk.DataManagement.xml

   qdbusxml2cpp is Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 */

#include "datamanagementadaptor.h"
#include "datamanagementmodel.h"
#include "datamanagementcommand.h"

#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtCore/QThreadPool>

namespace {
QVariantList convertVariantList(const QList<QDBusVariant>& values) {
    QVariantList vl;
    Q_FOREACH(const QDBusVariant& v, values)
        vl << v.variant();
    return vl;
}
}

Nepomuk::DataManagementAdaptor::DataManagementAdaptor(Nepomuk::DataManagementModel *parent)
    : QObject(parent),
      m_model(parent)
{
    m_threadPool = new QThreadPool(this);

    // never let go of our threads - that is just pointless apu cycles wasted
    m_threadPool->setExpiryTimeout(-1);

    // N threads means N connections to Virtuoso
    m_threadPool->setMaxThreadCount(10);
}

Nepomuk::DataManagementAdaptor::~DataManagementAdaptor()
{
}

void Nepomuk::DataManagementAdaptor::addProperty(const QStringList &resources, const QString &property, const QList<QDBusVariant> &values, const QString &app)
{
    Q_ASSERT(calledFromDBus());
    setDelayedReply(true);
    enqueueCommand(new AddPropertyCommand(decodeUrls(resources), decodeUrl(property), convertVariantList(values), app, m_model, message()));
}

void Nepomuk::DataManagementAdaptor::addProperty(const QString &resource, const QString &property, const QDBusVariant &value, const QString &app)
{
    addProperty(QStringList() << resource, property, QList<QDBusVariant>() << value, app);
}

QString Nepomuk::DataManagementAdaptor::createResource(const QStringList &types, const QString &label, const QString &description, const QString &app)
{
    Q_ASSERT(calledFromDBus());
    setDelayedReply(true);
    enqueueCommand(new CreateResourceCommand(decodeUrls(types), label, description, app, m_model, message()));
    // QtDBus will ignore this return value
    return QString();
}

QString Nepomuk::DataManagementAdaptor::createResource(const QString &type, const QString &label, const QString &description, const QString &app)
{
    return createResource(QStringList() << type, label, description, app);
}

Nepomuk::SimpleResourceGraph Nepomuk::DataManagementAdaptor::describeResources(const QStringList &resources, bool includeSubResources)
{
    Q_ASSERT(calledFromDBus());
    setDelayedReply(true);
    enqueueCommand(new DescribeResourcesCommand(decodeUrls(resources), includeSubResources, m_model, message()));
    // QtDBus will ignore this return value
    return SimpleResourceGraph();
}

void Nepomuk::DataManagementAdaptor::mergeResources(Nepomuk::SimpleResourceGraph resources, const QString &app, const QHash<QString, QDBusVariant> &additionalMetadata)
{
    Q_ASSERT(calledFromDBus());
    setDelayedReply(true);
    QHash<QUrl, QVariant> decodedMetaData;
    for(QHash<QString, QDBusVariant>::const_iterator it = additionalMetadata.constBegin();
        it != additionalMetadata.constEnd(); ++it) {
        decodedMetaData.insert(decodeUrl(it.key()), it.value().variant());
    }
    enqueueCommand(new MergeResourcesCommand(resources, app, decodedMetaData, m_model, message()));
}

void Nepomuk::DataManagementAdaptor::removeDataByApplication(const QString &app, int flags)
{
    Q_ASSERT(calledFromDBus());
    setDelayedReply(true);
    enqueueCommand(new RemoveDataByApplicationCommand(QList<QUrl>(), app, flags, m_model, message()));
}

void Nepomuk::DataManagementAdaptor::removeDataByApplication(const QStringList &resources, const QString &app, int flags)
{
    Q_ASSERT(calledFromDBus());
    setDelayedReply(true);
    enqueueCommand(new RemoveDataByApplicationCommand(decodeUrls(resources), app, flags, m_model, message()));
}

void Nepomuk::DataManagementAdaptor::removeProperties(const QStringList &resources, const QStringList &properties, const QString &app)
{
    Q_ASSERT(calledFromDBus());
    setDelayedReply(true);
    enqueueCommand(new RemovePropertiesCommand(decodeUrls(resources), decodeUrls(properties), app, m_model, message()));
}

void Nepomuk::DataManagementAdaptor::removeProperties(const QString &resource, const QString &property, const QString &app)
{
    removeProperties(QStringList() << resource, QStringList() << property, app);
}

void Nepomuk::DataManagementAdaptor::removePropertiesByApplication(const QStringList &resources, const QStringList &properties, const QString &app)
{
    Q_ASSERT(calledFromDBus());
    setDelayedReply(true);
    enqueueCommand(new RemovePropertiesByApplicationCommand(decodeUrls(resources), decodeUrls(properties), app, m_model, message()));
}

void Nepomuk::DataManagementAdaptor::removeProperty(const QStringList &resources, const QString &property, const QList<QDBusVariant> &values, const QString &app)
{
    Q_ASSERT(calledFromDBus());
    setDelayedReply(true);
    enqueueCommand(new RemovePropertyCommand(decodeUrls(resources), decodeUrl(property), convertVariantList(values), app, m_model, message()));
}

void Nepomuk::DataManagementAdaptor::removeProperty(const QString &resource, const QString &property, const QDBusVariant &value, const QString &app)
{
    removeProperty(QStringList() << resource, property, QList<QDBusVariant>() << value, app);
}

void Nepomuk::DataManagementAdaptor::removeResources(const QStringList &resources, const QString &app, int flags)
{
    Q_ASSERT(calledFromDBus());
    setDelayedReply(true);
    enqueueCommand(new RemoveResourcesCommand(decodeUrls(resources), app, flags, m_model, message()));
}

void Nepomuk::DataManagementAdaptor::removeResources(const QString &resource, const QString &app, int flags)
{
    removeResources(QStringList() << resource, app, flags);
}

void Nepomuk::DataManagementAdaptor::setProperty(const QStringList &resources, const QString &property, const QList<QDBusVariant> &values, const QString &app)
{
    Q_ASSERT(calledFromDBus());
    setDelayedReply(true);
    enqueueCommand(new SetPropertyCommand(decodeUrls(resources), decodeUrl(property), convertVariantList(values), app, m_model, message()));
}

void Nepomuk::DataManagementAdaptor::setProperty(const QString &resource, const QString &property, const QDBusVariant &value, const QString &app)
{
    setProperty(QStringList() << resource, property, QList<QDBusVariant>() << value, app);
}

void Nepomuk::DataManagementAdaptor::enqueueCommand(DataManagementCommand *cmd)
{
    m_threadPool->start(cmd);
}

#include "datamanagementadaptor.moc"
