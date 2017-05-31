#ifndef GENE_H
#define GENE_H

#include <QString>
#include <QColor>

// Data model class to store gene data.
// The genes are part of the ST Data, they are modeled in a class
// to encapculate their status and attributes
class Gene
{

public:
    Gene();
    explicit Gene(const Gene &other);
    explicit Gene(const QString &name,
                  bool visible = false,
                  const QColor &color = Qt::red,
                  const int cutoff = 1);
    ~Gene();

    Gene &operator=(const Gene &other);
    bool operator==(const Gene &other) const;

    // the name of the gene
    const QString name() const;
    // true if gene is visible
    bool visible() const;
    // the color of the gene
    const QColor color() const;
    // the threshold (reads)
    // the gene cut-off is used to discard genes whose count is below the cut off
    int cut_off() const;

    // Setters
    void name(const QString &name);
    void visible(bool visible);
    void color(const QColor &color);
    void cut_off(const int cutoff);

private:
    QString m_name;
    QColor m_color;
    bool m_visible;
    int m_cutoff;
};

#endif // GENE_H //
