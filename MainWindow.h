#pragma once
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qtoolbar.h>
#include <QtWidgets/qgraphicsscene.h>
#include <QtWidgets/qgraphicsview.h>
#include <QtWidgets/qgraphicsitem.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qaction.h>
#include <QtGui/qicon.h>
#include <QtWidgets/qdockwidget.h>
#include <QtWidgets/qtextedit.h>
#include <QtWidgets/qlistview.h>
#include <QtCore/qstringlistmodel.h>
#include <QtGui/qwindow.h>
#include "canvas.h"
#include "PhotonMap.h"
class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow();

	float getRandom();
	Q_SLOT
		void onNewFile();
	void onOpenFile();
	void onSaveFile();
	bool eventFilter(QObject* obj, QEvent* event) override;
private:
	QVBoxLayout MainWindowLayout;

	QToolBar toolBar;
	QAction actionNew;
	QAction actionOpen;
	QAction actionSave;
	QDockWidget dockWidget;
	QDockWidget dockWidgetRight;
	QWidget centralWidget;
	QGraphicsScene scene;
	QGraphicsView view;
	QListView listView;
	QStringListModel model;
	QGraphicsLineItem lineItem;
	Canvas canvas;
	QWindow viewWnd;
	QTextEdit dockWidgetContents;
	PhotonMap photoMap;
};