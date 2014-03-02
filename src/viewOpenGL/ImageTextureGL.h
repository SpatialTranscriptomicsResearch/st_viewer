/*
    Copyright (C) 2012  Spatial Transcriptomics AB,
    read LICENSE for licensing terms.
    Contact : Jose Fernandez Navarro <jose.fernandez.navarro@scilifelab.se>

*/
#ifndef IMAGETEXTUREGL_H
#define IMAGETEXTUREGL_H

#include "GraphicItemGL.h"

class QGLPainter;
class QImage;
class QGLTexture2D;

class ImageTextureGL : public GraphicItemGL
{
    Q_OBJECT

public:

    explicit ImageTextureGL(QObject *parent = 0);
    virtual ~ImageTextureGL();

    void createTexture(const QImage& image);

    void clear();

public slots:

    void setIntensity(qreal intensity);

protected:

    void draw(QGLPainter *painter);
    void drawGeometry (QGLPainter * painter);

    const QRectF boundingRect() const;

    void setSelectionArea(const SelectionEvent *){};

private:

    void createTiles(const QImage &image);
    void addTexture(const QImage &image, const int x = 0, const int y = 0);
    void clearTextures();
    void clearNodes();

    QList<QGLTexture2D *> m_textures;
    QList<QGLSceneNode *> m_nodes;
    qreal m_intensity = 1.0f;
    QRectF m_bounds;
};

#endif // IMAGETEXTUREGL_H
