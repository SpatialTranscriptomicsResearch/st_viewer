#ifndef STDATA_H
#define STDATA_H

#include <QSharedPointer>
#include <QList>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QColor>

#include "data/Gene.h"
#include "data/Spot.h"
#include "viewPages/SettingsWidget.h"

#include <armadillo>
//As crazy as it sounds RcppArmadillo must be included before RInside
#include "RcppArmadillo.h"
#include "RInside.h"

using namespace arma;

// TODO the values type can be templated
class STData
{

public:
    typedef Mat<float> Matrix;
    typedef Row<float> rowvec;
    typedef Col<float> colvec;
    typedef QString GeneType;
    typedef QSharedPointer<Spot> SpotObjectType;
    typedef QSharedPointer<Gene> GeneObjectType;
    typedef QList<SpotObjectType> SpotListType;
    typedef QList<GeneObjectType> GeneListType;

    STData();
    ~STData();

    // import/export matrix
    void read(const QString &filename);
    void save(const QString &filename) const;

    Matrix slice_matrix_counts() const;
    Matrix matrix_counts() const;

    size_t number_spots() const;
    size_t number_genes() const;

    GeneListType genes();
    SpotListType spots();

    void setRenderingSettings(SettingsWidget::Rendering *rendering_settings);

    void computeRenderingData();

    const QVector<unsigned> &renderingIndexes() const;
    const QVector<QVector3D> &renderingVertices() const;
    const QVector<QVector2D> &renderingTextures() const;
    const QVector<QVector4D> &renderingColors() const;

    void updateSize(const float size);

    void initRenderingData();

    bool parseSpotsMap(const QString &spots_file);

    static inline rowvec computeNonZeroColumns(Matrix matrix);
    inline Matrix normalizeCounts() const;

private:

    void computeGenesCutoff();
    inline void updateColor(const int index, const QColor &color);
    void computeDESeqFactors();
    void computeScranFactors();
    inline QColor adjustVisualMode(const QColor merged_color, const float &merged_value,
                                   const float &min_reads, const float &max_reads) const;

    Matrix m_counts_matrix;
    // cache the size factors to save computational time
    rowvec m_deseq_size_factors;
    rowvec m_scran_size_factors;
    // store gene/spots objects and indexes in matrix
    SpotListType m_spots;
    GeneListType m_genes;
    QVector<GeneType> m_matrix_genes;
    QVector<Spot::SpotType> m_matrix_spots;
    // rendering settings
    SettingsWidget::Rendering *m_rendering_settings;
    // rendering data
    QVector<QVector3D> m_vertices;
    QVector<QVector2D> m_textures;
    QVector<QVector4D> m_colors;
    QVector<unsigned> m_indexes;
    // R terminal
    RInside R;
};

#endif // STDATA_H
