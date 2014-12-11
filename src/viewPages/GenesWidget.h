/*
    Copyright (C) 2012  Spatial Transcriptomics AB,
    read LICENSE for licensing terms.
    Contact : Jose Fernandez Navarro <jose.fernandez.navarro@scilifelab.se>

*/

#ifndef GENESWIDGET_H
#define GENESWIDGET_H

#include <QWidget>
#include <QIcon>

#include "data/DataProxy.h"

class QPushButton;
class QLineEdit;
class GenesTableView;
class GeneFeatureItemModel;
class QSortFilterProxyModel;
class QColorDialog;

//This widget is part of the CellView,
//it is componsed of the Gene Table
//a search field and the select and action
//menus

class GenesWidget : public QWidget
{
    Q_OBJECT

public:

    explicit GenesWidget(QWidget *parent = 0);
    virtual ~GenesWidget();

    //clear focus/status and selections
    void clear();

signals:
    //signals emitted when the user selects or change colors of genes in the table
    void signalSelectionChanged(DataProxy::GeneList);
    void signalColorChanged(DataProxy::GeneList);

public slots:
    //updates the model of the table with the given objects
    void slotLoadModel(const DataProxy::GeneList &geneList);

private slots:
    //slots triggered by the show/color controls in the gene table
    void slotSetColorAllSelected(const QColor &color);
    void slotSetVisibilityForSelectedRows(bool visible);
    void slotHideAllSelected();
    void slotShowAllSelected();

private:

    //internal function to configure created buttons
    //to avoid code duplication
    //TODO better approach would be to have factories somewhere else
    void configureButton(QPushButton *button,
                         const QIcon icon = QIcon(), const QString tooltip = QString());

    //internal function to retrieve the model and the proxy model of the gene table
    QSortFilterProxyModel *getProxyModel();
    GeneFeatureItemModel *getModel();

    //some references needed to UI elements
    QPointer<QLineEdit> m_lineEdit;
    QPointer<GenesTableView> m_genes_tableview;
    QPointer<QColorDialog> m_colorList;

    Q_DISABLE_COPY(GenesWidget)
};

#endif // GENESWIDGET_H
