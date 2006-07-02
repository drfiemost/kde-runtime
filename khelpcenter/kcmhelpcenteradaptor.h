/*
 * This file was generated by dbusidl2cpp version 0.5
 * when processing input file org.kde.khelpcenter.kcmhelpcenter.xml
 *
 * dbusidl2cpp is Copyright (C) 2006 Trolltech AS. All rights reserved.
 *
 * This is an auto-generated file.
 */

#ifndef KCMHELPCENTERADAPTOR_H_213271150278513
#define KCMHELPCENTERADAPTOR_H_213271150278513

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;

/*
 * Adaptor class for interface org.kde.khelpcenter.kcmhelpcenter
 */
class KcmhelpcenterAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.khelpcenter.kcmhelpcenter")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"org.kde.khelpcenter.kcmhelpcenter\" >\n"
"    <method name=\"slotIndexProgress\" />\n"
"    <method name=\"slotIndexError\" >\n"
"      <arg direction=\"in\" type=\"s\" name=\"error\" />\n"
"    </method>\n"
"  </interface>\n"
        "")
public:
    KcmhelpcenterAdaptor(QObject *parent);
    virtual ~KcmhelpcenterAdaptor();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    void slotIndexError(const QString &error);
    void slotIndexProgress();
Q_SIGNALS: // SIGNALS
};

#endif
