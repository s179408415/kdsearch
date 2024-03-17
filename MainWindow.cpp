#include "MainWindow.h"
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qmenubar.h>
#include <QtWidgets/qmenu.h>
#include <QtCore/qstringlist.h>
#include <QtWidgets/qdialog.h>
#include <QtWidgets/qlineedit.h>
#include <QtGui/qevent.h>
#include <random>
#pragma execution_character_set("utf-8")
MainWindow::MainWindow()
	:actionNew(QIcon(":/icons/new.png"), "New新建", &toolBar),
	actionOpen(QIcon(":/icons/open.png"), "Open", &toolBar),
	actionSave(QIcon(":/icons/save.png"), "Save", &toolBar),
	dockWidget("Dockable", this),
	dockWidgetRight("right", this),
	lineItem(0, 0, 50, 50)
{
	QMenu *fileMenu = menuBar()->addMenu("&File");
	fileMenu->addAction(&actionNew);
	fileMenu->addAction(&actionOpen);
	fileMenu->addAction(&actionSave);
	setMinimumSize(1800, 1200);
	
	toolBar.addAction(&actionNew);
	toolBar.addAction(&actionOpen);
	toolBar.addAction(&actionSave);

	addToolBar(&toolBar);
	connect(&actionNew, &QAction::triggered, this, &MainWindow::onNewFile);
	connect(&actionOpen, &QAction::triggered, this, &MainWindow::onOpenFile);
	connect(&actionSave, &QAction::triggered, this, &MainWindow::onSaveFile);
	dockWidget.setWidget(&dockWidgetContents);
	addDockWidget(Qt::LeftDockWidgetArea, &dockWidget);
	addDockWidget(Qt::RightDockWidgetArea, &dockWidgetRight);
	dockWidgetRight.setWidget(&listView);
	QStringList list;
	list << "Item 1" << "Item 2" << "Item 3" << "Item 4";
	model.setStringList(list);
	listView.setModel(&model);
	
	setCentralWidget(&view);
	
	view.setScene(&scene);
	view.installEventFilter(this);
	

	scene.addItem(&lineItem);
	scene.addLine(-200, -200, 0, -200);
}
float MainWindow::getRandom()
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_real_distribution<float> dist(0, 1);
	return dist(rng);
}
void MainWindow::onNewFile()
{
	QList<QGraphicsItem*> itemList = scene.items();
	for (QGraphicsItem* item : itemList) {
		scene.removeItem(item);
	}
	photoMap.cleanUp();
	QDialog dialog;
	dialog.setWindowTitle("输入点数量");

	// 创建一个布局管理器
	QVBoxLayout layout(&dialog);

	// 创建一个输入框
	QLineEdit lineEdit(&dialog);
	layout.addWidget(&lineEdit);

	// 创建一个确认按钮
	QPushButton button("OK", &dialog);
	layout.addWidget(&button);

	// 连接按钮的点击事件到槽函数，关闭对话框
	QObject::connect(&button, &QPushButton::clicked, &dialog, &QDialog::accept);
	dialog.exec();
	QString text = lineEdit.text();
	int num = text.toInt();
	qreal radius = 1;
	const int width = 600;
	float step = (float)width / num;
	for (int i=0;i<num;i++)
	{
// 		for (int j=0;j<num;j++)
// 		{
			qreal x = getRandom()* width - width / 2;
			qreal y = getRandom() * width - width / 2;
// 			qreal x = i * step - width / 2;
// 			qreal y = j * step - width / 2;
			auto item = scene.addEllipse(x - radius, y - radius, radius*2, radius*2);
			item->setBrush(QBrush(QColor(0, 0, 0)));
			item->setPen(QPen(QColor(0, 0, 0)));
			Photon photon;
			photon.Pos = vec2(x, y);
			photon.item = item;
			photoMap.store(photon);
//		}
	}
	photoMap.balance();
}

void MainWindow::onOpenFile()
{
	QMessageBox::warning(nullptr, "标题", "这是一条警告消息");

}

void MainWindow::onSaveFile()
{
	QMessageBox::critical(nullptr, "标题", "这是一条错误消息");

}

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
	if (obj == &view && event->type() == QEvent::MouseButtonPress) {
		QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
		if (mouseEvent->button() == Qt::LeftButton) {
			const int N = 20;
			Nearestphotons np;
			QPointF pos = view.mapToScene(mouseEvent->pos());
			np.Pos = vec2(pos.x(),pos.y());
			np.max_photons = N;
			np.dist2 = new float[N + 1];
			np.photons = new Photon * [N + 1];
			np.dist2[0] = 100 * 100;
			photoMap.getNearestPhotons(&np, 1);
			auto items = view.items();
			for (auto item : items)
			{
				bool bFound = false;
				for (int i =1;i<=np.found; i++)
				{
					if (item == np.photons[i]->item)
					{
						bFound = true;
						break;
					}
				}
				if (bFound)
				{
					((QGraphicsEllipseItem*)item)->setBrush(QBrush(QColor(255, 0, 0)));
					((QGraphicsEllipseItem*)item)->setPen(QPen(QColor(255, 0, 0)));
				}
				else
				{
					((QGraphicsEllipseItem*)item)->setBrush(QBrush(QColor(0, 0, 0)));
					((QGraphicsEllipseItem*)item)->setPen(QPen(QColor(0, 0, 0)));
				}
			}
			
		}
	}
	return QObject::eventFilter(obj, event);
}
