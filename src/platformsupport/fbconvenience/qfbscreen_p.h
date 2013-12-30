/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the plugins of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QFBSCREEN_P_H
#define QFBSCREEN_P_H

#include <qpa/qplatformscreen.h>
#include <QtCore/QTimer>
#include <QtCore/QSize>
#include "qfbcursor_p.h"

QT_BEGIN_NAMESPACE

class QFbWindow;
class QFbCursor;
class QPainter;
class QFbBackingStore;

class QFbScreen : public QObject, public QPlatformScreen
{
    Q_OBJECT
public:
    QFbScreen();
    ~QFbScreen();

    QRect geometry() const Q_DECL_OVERRIDE { return mGeometry; }
    int depth() const Q_DECL_OVERRIDE { return mDepth; }
    QImage::Format format() const Q_DECL_OVERRIDE { return mFormat; }
    QSizeF physicalSize() const Q_DECL_OVERRIDE { return mPhysicalSize; }
    QPlatformCursor *cursor() const Q_DECL_OVERRIDE { return mCursor; }

    QWindow *topWindow() const;
    QWindow *topLevelAt(const QPoint & p) const Q_DECL_OVERRIDE;

    // compositor api
    virtual void addWindow(QFbWindow *window);
    virtual void removeWindow(QFbWindow *window);
    virtual void raise(QFbWindow *window);
    virtual void lower(QFbWindow *window);
    virtual void topWindowChanged(QWindow *) {}

    void addBackingStore(QFbBackingStore *bs) {mBackingStores << bs;}

    void scheduleUpdate();

public slots:
    virtual void setDirty(const QRect &rect);
    void setPhysicalSize(const QSize &size);
    void setGeometry(const QRect &rect);

protected slots:
    virtual QRegion doRedraw();

protected:
    virtual void initializeCompositor();
    virtual void initializeScreenImage();

    QList<QFbWindow *> mWindowStack;
    QRegion mRepaintRegion;
    QTimer mRedrawTimer;

    QFbCursor *mCursor;
    QRect mGeometry;
    int mDepth;
    QImage::Format mFormat;
    QSizeF mPhysicalSize;
    QImage *mScreenImage;
    QPainter *mCompositePainter;

private:
    void invalidateRectCache() { mIsUpToDate = false; }
    void generateRects();

    QRect mapToDevice(const QRect &rect) const;

    QList<QPair<QRect, int> > mCachedRects;
    QList<QFbBackingStore*> mBackingStores;

    friend class QFbWindow;
    bool mIsUpToDate;
};

QT_END_NAMESPACE

#endif // QFBSCREEN_P_H
