#include "MainWindow.h"
#include <windows.h>

int main(int argc, char* argv[]) {
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
	QApplication app(argc, argv);

	MainWindow window;
	window.setWindowTitle("Qt Layout Example");

	window.show();

	return app.exec();
}