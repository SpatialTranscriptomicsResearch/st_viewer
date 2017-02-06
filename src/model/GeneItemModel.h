#ifndef GENEFITEMMODEL_H
#define GENEFITEMMODEL_H

#include <QAbstractTableModel>
#include "dataModel/STData.h"

class QModelIndex;
class QStringList;
class QMimeData;
class QItemSelection;
class Gene;

// Wrapper model class for the gene data (specific to a dataset).
// Primarily used to enumerate the genes in the cell view (genes table)
// and allow the user to interact with individual genes.
class GeneItemModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_ENUMS(Column)

public:
    enum Column { Show = 0, Name = 1, CutOff = 2, Color = 3 };

    explicit GeneItemModel(QObject *parent = 0);
    virtual ~GeneItemModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    // allows the user to modify the cut off on the table
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // this function will set to visible the genes included in the selection
    // and emit a signal with the modified genes
    void setGeneVisibility(const QItemSelection &selection, bool visible);

    // this function will modify the color of the genes included in the selection
    // and emit a signal with the modified genes
    void setGeneColor(const QItemSelection &selection, const QColor &color);

    // reload the reference to the genes from DataProxy
    void loadGenes(const STData::gene_list &geneList);

    // clear and reset the model
    void clearGenes();

public slots:
    // TODO pass reference instead for genename
    // Used to sort the genes in the table
    bool geneName(const QModelIndex &index, QString *genename) const;

signals:
    // Signals to notify that any of the gene/s properties have changed
    void signalCutOffChanged(Gene gene);
    void signalSelectionChanged(const STData::gene_list &geneList);
    void signalColorChanged(const STData::gene_list &geneLis);

private:
    STData::gene_list m_genelist_reference;

    Q_DISABLE_COPY(GeneItemModel)
};

#endif // GENEFITEMMODEL_H
