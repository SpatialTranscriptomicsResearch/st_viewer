#ifndef GRAPHICITEMGL_H
#define GRAPHICITEMGL_H
/*
    Copyright (C) 2012  Spatial Transcriptomics AB,
    read LICENSE for licensing terms.
    Contact : Jose Fernandez Navarro <jose.fernandez.navarro@scilifelab.se>

*/

#include <QGLSceneNode>
#include <QTransform>
#include "utils/Utils.h"

class QGLPainter;
class QRectF;
class QMouseEvent;
class SelectionEvent;

class GraphicItemGL : public QObject
{

    Q_OBJECT
    Q_FLAGS(VisualOptions)

public:

    enum VisualOption
    {
        Visible = 1,
        Selectable = 2,
        Transformable = 4,
        Yinverted = 8,
        Xinverted = 16,
        RubberBandable = 32
    };
    Q_DECLARE_FLAGS(VisualOptions, VisualOption)

    explicit GraphicItemGL(QObject *parent = 0);
    virtual ~GraphicItemGL();

    Globals::Anchor anchor() const;
    void setAnchor(Globals::Anchor anchor);

    // transformation matrix of the element
    const QTransform transform() const;
    void setTransform(const QTransform& transform);

    bool visible() const;
    bool selectable() const;
    bool transformable() const;
    bool invertedY() const;
    bool invertedX() const;
    bool rubberBandable() const;

    GraphicItemGL::VisualOptions visualOptions() const;
    void setVisualOptions(GraphicItemGL::VisualOptions visualOptions);
    void setVisualOption(GraphicItemGL::VisualOption visualOption, bool value);

    virtual void draw(QGLPainter *painter) = 0;
    virtual void drawGeometry (QGLPainter * painter) = 0;

    // geometry of the graphic element
    virtual const QRectF boundingRect() const = 0;
    virtual bool contains(const QPointF& point) const;
    virtual bool contains(const QRectF& point) const;

    // graphic elements can be sent mouse events
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);

    virtual void setSelectionArea(const SelectionEvent *event) = 0;

public slots:

    void setVisible(bool);

signals:

    void updated();

protected:

    const QTransform adjustForAnchor(const QTransform& transform) const;

    QTransform m_transform;
    Globals::Anchor m_anchor = Globals::Anchor::NorthWest;
    GraphicItemGL::VisualOptions m_visualOptions;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(GraphicItemGL::VisualOptions)

#endif // GRAPHICITEMGL_H
