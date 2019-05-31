#include "HeatMap.h"

#include <QImage>
#include <QColor>

namespace Color
{

void createLegend(QImage &image, const float lowerbound,
                  const float upperbound, const ColorGradients cmap)
{

    const int height = image.height();
    const int width = image.width();

    for (int y = 0; y < height; ++y) {
        // get the color of each line of the image as the heatmap
        // color normalized to the lower and upper bound of the image
        const int value = height - y - 1;
        const float adjusted_value
                = STMath::linearConversion<float, float>(static_cast<float>(value),
                                                       0.0,
                                                       static_cast<float>(height),
                                                       lowerbound,
                                                       upperbound);
        const QColor color = Color::createCMapColor(adjusted_value, lowerbound, upperbound, cmap);
        const QRgb rgb_color = color.rgb();
        for (int x = 0; x < width; ++x) {
            image.setPixel(x, y, rgb_color);
        }
    }
}

// simple function that computes color from a min-max range
// using linear Interpolation
QColor createHeatMapLinearColor(const float value, const float min, const float max)
{
    const double halfmax = (min + max) / 2;
    const double blue = std::max(0.0, 255 * (1 - (value / halfmax)));
    const double red = std::max(0.0, 255 * ((value / halfmax) - 1));
    const double green = 255 - blue - red;
    return QColor::fromRgb(red, green, blue);
}

QColor createDynamicRangeColor(const float value, const float min,
                               const float max, const QColor color)
{
    const float adjusted_value
            = STMath::norm<float, float>(value, min, max);
    QColor newcolor(color);
    newcolor.setAlphaF(adjusted_value);
    return newcolor;
}

// simple function that computes color from a value
// using the human wave lenght spectra
QColor createHeatMapWaveLenghtColor(const float value)
{
    static const float gamma = 0.8f;

    // denorm value to range (380-780)
    const float cwavelength = STMath::denorm(value, 380.0, 780.0);

    // define colors according to wave lenght spectra
    float red = 0.0;
    float green = 0.0;
    float blue = 0.0;

    if (380.0 <= cwavelength && cwavelength < 440.0) {
        red = -(cwavelength - 440.0) / (440.0 - 380.0);
        green = 0.0;
        blue = 1.0;
    } else if (440.0 <= cwavelength && cwavelength < 490.0) {
        red = 0.0;
        green = (cwavelength - 440.0) / (490.0 - 440.0);
        blue = 1.0;
    } else if (490.0 <= cwavelength && cwavelength < 510.0) {
        red = 0.0;
        green = 1.0;
        blue = -(cwavelength - 510.0) / (510.0 - 490.0);
    } else if (510.0 <= cwavelength && cwavelength < 580.0) {
        red = (cwavelength - 510.0) / (580.0 - 510.0);
        green = 1.0;
        blue = 0.0;
    } else if (580.0 <= cwavelength && cwavelength < 645.0) {
        red = 1.0;
        green = -(cwavelength - 645.0) / (645.0 - 580.0);
        blue = 0.0;
    } else if (645.0 <= cwavelength && cwavelength <= 780.0) {
        red = 1.0;
        green = 0.0;
        blue = 0.0;
    }

    // Let the intensity fall off near the vision limits
    float factor = 0.3f;
    if (380.0 <= cwavelength && cwavelength < 420.0) {
        factor = 0.3 + 0.7 * (cwavelength - 380.0) / (420.0 - 380.0);
    } else if (420.0 <= cwavelength && cwavelength < 700.0) {
        factor = 1.0;
    } else if (700.0 <= cwavelength && cwavelength <= 780.0) {
        factor = 0.3 + 0.7 * (780.0 - cwavelength) / (780.0 - 700.0);
    }

    // Gamma adjustments (clamp to [0.0, 1.0])
    red = STMath::clamp(qPow(red * factor, gamma), 0.0, 1.0);
    green = STMath::clamp(qPow(green * factor, gamma), 0.0, 1.0);
    blue = STMath::clamp(qPow(blue * factor, gamma), 0.0, 1.0);

    // return color
    return QColor::fromRgbF(red, green, blue, 1.0);
}

// Functions to create a color mapped in the color range given
QColor createRangeColor(const float value, const float min, const float max,
                        QColor init, QColor end)
{
    const float norm_value = STMath::norm<float, float>(value, min, max);
    return STMath::lerp(norm_value, init, end);
}

QColor createCMapColorGpHot(const float value, const float min, const float max)
{
    const QCPRange range(min,max);
    QCPColorGradient cmap(QCPColorGradient::gpHot);
    return QColor(cmap.color(value, range));
}

QColor createCMapColor(const float value, const float min,
                       const float max, const ColorGradients cmap)
{
    const QCPRange range(min, max);
    QCPColorGradient cmapper(cmap);
    return QColor(cmapper.color(value, range));
}

QColor adjustVisualMode(const QColor merged_color,
                        const float &merged_value,
                        const float &min_reads,
                        const float &max_reads,
                        const SettingsWidget::VisualMode mode)
{
    QColor color = merged_color;
    switch (mode) {
    case (SettingsWidget::VisualMode::Normal): {
    } break;
    case (SettingsWidget::VisualMode::DynamicRange): {
        color = Color::createDynamicRangeColor(merged_value, min_reads,
                                               max_reads, merged_color);
    } break;
    case (SettingsWidget::VisualMode::HeatMap): {
        color = Color::createCMapColor(merged_value, min_reads,
                                       max_reads, Color::ColorGradients::gpSpectrum);
    } break;
    case (SettingsWidget::VisualMode::ColorRange): {
        color = Color::createCMapColor(merged_value, min_reads,
                                       max_reads, Color::ColorGradients::gpHot);
    } break;
    case (SettingsWidget::VisualMode::ColorRange2): {
        color = Color::createCMapColor(merged_value, min_reads,
                                       max_reads, Color::ColorGradients::gpPolar);
    }
    }
    return color;
}
}
