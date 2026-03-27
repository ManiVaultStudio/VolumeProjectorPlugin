#pragma once
#include <QColor>
#include <QPainter>
#include <QRectF>
#include <QPixmap>
#include <QBitmap>

enum class SelectedSide {
    None,
    Left,
    Right,
    Top,
    Bottom
};

struct gradientData {
    bool gradient;
    int textureID;
    float xOffset;
    float yOffset;
    float width;
    float height;
	int rotation;
};

namespace mv::gui
{
    class PointRenderer;
}

class InteractiveShape {
public:
    InteractiveShape(const QPixmap& pixmap, const QRectF& rect, const QRect& bounds, const QColor& pixmapColor, int globalAlphaValue, mv::gui::PointRenderer* pointRenderer, qreal threshold = 10.0);

    void draw(QPainter& painter, bool drawBorder, bool useGlobalAlpha, bool normalizeWindow, QColor borderColor = Qt::black, int scaleTo = 0) const;
	void drawID(QPainter& painter, bool normalizeWindow, int id, int scaleTo = 0) const;
    bool contains(const QPointF& point) const;
    void moveBy(const QPointF& delta);
    void resizeBy(const QPointF& delta, SelectedSide& side);

    bool isNearSide(const QPointF& point, SelectedSide& side) const;
    bool isNearTopRightCorner(const QPointF& point) const;

    void setSelected(bool selected);
    bool getSelected() const;

    void setColor(const QColor& color);
    QColor getColor() const;

    void setThreshold(qreal threshold);
    void setBounds(const QRect& bounds);

	void updateGradient(const gradientData& data);
	QImage getGradientImage() const;
	gradientData getGradientData() const;

	void setGlobalAlphaValue(int globalAlphaValue);
    QRectF getRelativeRect() const;
    QRectF getWorldRect() const;
    QRectF getAdjustedWorldRect(int scaleTo = 0) const;

private:

    QRectF getAbsoluteRect() const;
	void updatePixmap();

private:
    QPixmap _pixmap;
    QPixmap _globalAlphaPixmap;
    QPixmap _colormap;
    QPixmap _globalAlphaColormap;

    QRectF _rect;
    QRect _bounds;
    bool _isSelected;
    qreal _threshold;
    QColor _pixmapColor;
    QBitmap _mask;

	int _globalAlphaValue = 100;

	QImage _gradient1D;
	QImage _gradient2D;
    QImage _usedGradient;

	gradientData _gradientData;

    mv::gui::PointRenderer* _pointRenderer = nullptr;
};
