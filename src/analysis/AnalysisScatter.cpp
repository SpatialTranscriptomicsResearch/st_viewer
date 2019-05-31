#include "AnalysisScatter.h"

#include <QChartView>
#include <QValueAxis>
#include <QScatterSeries>

#include "color/HeatMap.h"

#include "ui_analysisScatter.h"

AnalysisScatter::AnalysisScatter(const STData::STDataFrame &data,
                                 QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f)
    , m_ui(new Ui::analysisScatter)
{

    // setup UI
    m_ui->setupUi(this);

    const unsigned num_spots = data.spots.size();
    const colvec spot_reads = sum(data.counts, 1);
    const ucolvec spot_genes = STData::computeNonZeroRows(data.counts);
    const float min_reads = spot_reads.min();
    const float max_reads = spot_reads.max();
    const float min_genes = spot_genes.min();
    const float max_genes = spot_genes.max();
    for (unsigned i = 0; i < num_spots; ++i) {
        QScatterSeries *series_reads = new QScatterSeries(this);
        QScatterSeries *series_genes = new QScatterSeries(this);
        series_reads->setMarkerShape(QScatterSeries::MarkerShapeCircle);
        series_reads->setMarkerSize(10.0);
        series_reads->setUseOpenGL(false);

        series_genes->setMarkerShape(QScatterSeries::MarkerShapeCircle);
        series_genes->setMarkerSize(10.0);
        series_genes->setUseOpenGL(false);

        const auto &spot = Spot::getCoordinates(data.spots.at(i));
        const float value_reads = spot_reads.at(i);
        const float value_genes = spot_genes.at(i);
        const QColor color_reads = Color::createCMapColor(value_reads, min_reads,
                                                          max_reads, Color::ColorGradients::gpHot);
        const QColor color_genes = Color::createCMapColor(value_genes, min_genes,
                                                          max_genes, Color::ColorGradients::gpHot);
        series_reads->setColor(color_reads);
        series_reads->append(spot.first, spot.second * -1);
        m_ui->plotReads->chart()->addSeries(series_reads);

        series_genes->setColor(color_genes);
        series_genes->append(spot.first, spot.second * -1);
        m_ui->plotGenes->chart()->addSeries(series_genes);
    }

    m_ui->plotReads->chart()->setTitle(tr("Spots colored by expression (transcripts)"));
    m_ui->plotReads->chart()->setDropShadowEnabled(false);
    m_ui->plotReads->chart()->legend()->hide();
    m_ui->plotReads->chart()->createDefaultAxes();
    m_ui->plotReads->chart()->axes(Qt::Horizontal).back()->setGridLineVisible(false);
    m_ui->plotReads->chart()->axes(Qt::Horizontal).back()->setLabelsVisible(true);
    m_ui->plotReads->chart()->axes(Qt::Horizontal).back()->setTitleText(tr("Spot(X)"));
    m_ui->plotReads->chart()->axes(Qt::Vertical).back()->setGridLineVisible(false);
    m_ui->plotReads->chart()->axes(Qt::Vertical).back()->setLabelsVisible(true);
    m_ui->plotReads->chart()->axes(Qt::Vertical).back()->setTitleText(tr("Spot(Y)"));

    m_ui->plotGenes->chart()->setTitle(tr("Spots colored by expression (genes)"));
    m_ui->plotGenes->chart()->setDropShadowEnabled(false);
    m_ui->plotGenes->chart()->legend()->hide();
    m_ui->plotGenes->chart()->createDefaultAxes();
    m_ui->plotGenes->chart()->axes(Qt::Horizontal).back()->setGridLineVisible(false);
    m_ui->plotGenes->chart()->axes(Qt::Horizontal).back()->setLabelsVisible(true);
    m_ui->plotGenes->chart()->axes(Qt::Horizontal).back()->setTitleText(tr("Spot(X)"));
    m_ui->plotGenes->chart()->axes(Qt::Vertical).back()->setGridLineVisible(false);
    m_ui->plotGenes->chart()->axes(Qt::Vertical).back()->setLabelsVisible(true);
    m_ui->plotGenes->chart()->axes(Qt::Vertical).back()->setTitleText(tr("Spot(Y)"));

    connect(m_ui->exportReads, &QPushButton::clicked, this, &AnalysisScatter::slotExportPlotReads);
    connect(m_ui->exportGenes, &QPushButton::clicked, this, &AnalysisScatter::slotExportPlotGenes);
}

AnalysisScatter::~AnalysisScatter()
{

}

void AnalysisScatter::slotExportPlotReads()
{
    m_ui->plotReads->slotExportPlot(tr("Save Scatter Transcripts Plot"));
}

void AnalysisScatter::slotExportPlotGenes()
{
    m_ui->plotGenes->slotExportPlot(tr("Save Scatter Genes Plot"));
}
