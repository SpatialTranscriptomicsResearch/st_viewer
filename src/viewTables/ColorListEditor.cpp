/*
    Copyright (C) 2012  Spatial Transcriptomics AB,
    read LICENSE for licensing terms.
    Contact : Jose Fernandez Navarro <jose.fernandez.navarro@scilifelab.se>

*/

#include "ColorListEditor.h"

#include <QVariant>
#include <QScopedPointer>

#include "color/ColorPalette.h"

static const int DEFAULT_SATURATION = 200;

ColorListEditor::ColorListEditor(QWidget* widget)
    : QComboBox(widget)
{
    HSVPalette palette(this);
    palette.setSaturation(DEFAULT_SATURATION); // lighter colors
    populateList(&palette);
}

ColorListEditor::~ColorListEditor()
{

}

const QColor ColorListEditor::color() const
{
    return qvariant_cast<QColor>(itemData(currentIndex(), Qt::DecorationRole));
}

void ColorListEditor::setColor(const QColor& color)
{
    setCurrentIndex(findData(color, int(Qt::DecorationRole)));
}

void ColorListEditor::populateList(const ColorPalette* palette)
{
    const ColorPalette::ColorList colorList = palette->colorList();

    for (int i = 0; i < colorList.size(); ++i) {
        const ColorPalette::ColorPair pair = colorList[i];
        insertItem(i, pair.second);
        setItemData(i, pair.first, Qt::DecorationRole);
    }
}