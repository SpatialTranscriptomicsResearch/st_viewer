#include "GeneItemModel.h"
#include <set>
#include <QDebug>
#include <QModelIndex>
#include <QMimeData>
#include <QStringList>
#include <QItemSelection>

#include "data/Gene.h"
#include "data/Dataset.h"

static const int COLUMN_NUMBER = 4;

GeneItemModel::GeneItemModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

GeneItemModel::~GeneItemModel()
{
}

QVariant GeneItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || m_items_reference.empty()) {
        return QVariant(QVariant::Invalid);
    }

    const auto item = m_items_reference.at(index.row());

    if (role == Qt::DisplayRole && index.column() == Name) {
        return item->name();
    }

    if (role == Qt::ForegroundRole && index.column() == Name) {
        return QColor(0, 155, 60);
    }

    if (role == Qt::CheckStateRole && index.column() == Show) {
        return item->selected() ? Qt::Checked : Qt::Unchecked;
    }

    if (role == Qt::DecorationRole && index.column() == Color) {
        return item->color();
    }

    if (role == Qt::DisplayRole && index.column() == CutOff) {
        return item->cut_off();
    }

    if (role == Qt::TextAlignmentRole) {
        switch (index.column()) {
        case Show:
            return Qt::AlignCenter;
        case Color:
            return Qt::AlignCenter;
        case Name:
            return Qt::AlignLeft;
        case CutOff:
            return Qt::AlignCenter;
        default:
            return QVariant(QVariant::Invalid);
        }
    }

    return QVariant(QVariant::Invalid);
}

bool GeneItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole && index.column() == CutOff) {
        auto item = m_items_reference.at(index.row());
        const int new_cutoff = value.toUInt();
        if (item->cut_off() != new_cutoff && new_cutoff > 0) {
            item->cut_off(new_cutoff);
            emit dataChanged(index, index);
            emit signalGeneCutOffChanged();
            return true;
        }
        return false;
    }

    return false;
}

QVariant GeneItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case Name:
            return tr("Gene");
        case Show:
            return tr("Show");
        case Color:
            return tr("Color");
        case CutOff:
            return tr("Cut-off");
        default:
            return QVariant(QVariant::Invalid);
        }
    }

    if (orientation == Qt::Horizontal && role == Qt::ToolTipRole) {
        switch (section) {
        case Name:
            return tr("The name of the gene");
        case Show:
            return tr("Indicates if the genes is visible on the screen");
        case Color:
            return tr("Indicates the color of the gene on the screen");
        case CutOff:
            return tr("Numbers of reads from which this gene will be included");
        default:
            return QVariant(QVariant::Invalid);
        }
    }

    if (role == Qt::TextAlignmentRole) {
        switch (section) {
        case Show:
            return Qt::AlignCenter;
        case Color:
            return Qt::AlignLeft;
        case Name:
            return Qt::AlignLeft;
        case CutOff:
            return Qt::AlignCenter;
        default:
            return QVariant(QVariant::Invalid);
        }
    }

    // return invalid value
    return QVariant(QVariant::Invalid);
}

int GeneItemModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_items_reference.size();
}

int GeneItemModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : COLUMN_NUMBER;
}

Qt::ItemFlags GeneItemModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);

    if (!index.isValid()) {
        return defaultFlags;
    }

    switch (index.column()) {
    case Name:
        return defaultFlags;
    case Show:
        return Qt::ItemIsUserCheckable | defaultFlags;
    case Color:
        return defaultFlags;
    case CutOff:
        return Qt::ItemIsEditable | defaultFlags;
    }

    return defaultFlags;
}

void GeneItemModel::loadDataset(const Dataset &dataset)
{
    Q_UNUSED(dataset)
    beginResetModel();
    m_items_reference = dataset.data()->genes();
    endResetModel();
}

void GeneItemModel::clear()
{
    beginResetModel();
    m_items_reference.clear();
    endResetModel();
}

bool GeneItemModel::getName(const QModelIndex &index, QString &name) const
{
    if (!index.isValid() || m_items_reference.empty()) {
        return false;
    }

    const auto item = m_items_reference.at(index.row());
    if (index.column() == Name) {
        name = item->name();
        return true;
    }

    return false;
}

void GeneItemModel::setVisibility(const QItemSelection &selection, bool visible)
{
    if (m_items_reference.empty()) {
        return;
    }
    // get unique indexes from the user selection
    QSet<int> rows;
    for (const auto &index : selection.indexes()) {
        rows.insert(index.row());
    }
    // create a list of genes that are changing the selected state
    STData::GeneListType geneList;
    for (const auto &row : rows) {
        auto gene = m_items_reference.at(row);
        if (gene->selected() != visible) {
            gene->selected(visible);
            geneList.push_back(gene);
        }
    }
    //NOTE do not seem the changed genes for now
    if (!geneList.empty()) {
        // notify with the new list
        emit signalGeneSelectionChanged();
    }
}

void GeneItemModel::setColor(const QItemSelection &selection, const QColor &color)
{
    if (m_items_reference.empty()) {
        return;
    }
    // get unique indexes from the user selection
    QSet<int> rows;
    for (const auto &index : selection.indexes()) {
        rows.insert(index.row());
    }
    // create a list of genes that are changing the color
    STData::GeneListType geneList;
    for (const auto &row : rows) {
        auto gene = m_items_reference.at(row);
        if (color.isValid() && gene->color() != color) {
            gene->color(color);
            geneList.push_back(gene);
        }
    }
    //NOTE do not seem the changed genes for now
    if (!geneList.empty()) {
        // notify with the new list
        emit signalGeneColorChanged();
    }
}