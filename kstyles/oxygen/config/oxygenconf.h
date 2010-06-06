/*
Copyright (C) 2003 Sandro Giessl <ceebx@users.sourceforge.net>

based on the Keramik configuration dialog:
Copyright (c) 2003 Maksim Orlovich <maksim.orlovich@kdemail.net>

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

*/

#ifndef OXYGENCONF_H
#define OXYGENCONF_H

#include "ui_oxygenconf.h"

class OxygenStyleConfig: public QWidget, Ui::OxygenConfigDialog
{
    Q_OBJECT
public:
    OxygenStyleConfig(QWidget* parent);
    ~OxygenStyleConfig();

    //This signal and the next two slots are the plugin
    //page interface
Q_SIGNALS:
    void changed(bool);

public Q_SLOTS:
    void save();
    void defaults();

    //Everything below this is internal.
protected Q_SLOTS:
    void updateChanged();

protected:
    int menuMode() const;
    int tabStyle() const;
};

#endif // OXYGENCONF_H