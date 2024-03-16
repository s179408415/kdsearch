#include "canvas.h"



Canvas::Canvas(QWidget* parent /*= nullptr*/) : QWidget(parent)
{

}

void Canvas::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setPen(QPen(Qt::black, 2)); // ���û���Ϊ��ɫ�����Ϊ2  
	painter.setBrush(Qt::red); // ���û�ˢΪ��ɫ  

	// ����һ������  
	painter.drawRect(10, 10, 100, 100); // ��(10,10)λ�û���һ��100x100�ľ���  

	// ����һ���ı�  
	painter.setFont(QFont("Arial", 12)); // ��������ΪArial����СΪ12  
	painter.drawText(60, 60, "Hello, QPainter!"); // ��(60,60)λ�û����ı�  
}
