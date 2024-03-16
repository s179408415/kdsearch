#include "canvas.h"



Canvas::Canvas(QWidget* parent /*= nullptr*/) : QWidget(parent)
{

}

void Canvas::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setPen(QPen(Qt::black, 2)); // 设置画笔为黑色，宽度为2  
	painter.setBrush(Qt::red); // 设置画刷为红色  

	// 绘制一个矩形  
	painter.drawRect(10, 10, 100, 100); // 在(10,10)位置绘制一个100x100的矩形  

	// 绘制一段文本  
	painter.setFont(QFont("Arial", 12)); // 设置字体为Arial，大小为12  
	painter.drawText(60, 60, "Hello, QPainter!"); // 在(60,60)位置绘制文本  
}
