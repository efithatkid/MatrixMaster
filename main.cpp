#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <MatrixSolver.h>

int main(int argc, char* argv[]) {
	QGuiApplication app(argc, argv);


	QQmlApplicationEngine engine;
	MatrixSolver solver;
	engine.rootContext()->setContextProperty("MatrixSolver", &solver);

	QObject::connect(
			&engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() { QCoreApplication::exit(-1); },
			Qt::QueuedConnection);
	engine.loadFromModule("MatrixMaster", "Main");

	return QGuiApplication::exec();
}
