#pragma once
#include <QtWidgets/qwidget.h>
#include <QtGUI/qpainter.h>

class Canvas : public QWidget
{
	Q_OBJECT
public:
	Canvas(QWidget* parent = nullptr);

protected:
	void paintEvent(QPaintEvent* event) override;
};