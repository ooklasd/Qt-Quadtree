#pragma once

#include <QtWidgets/QMainWindow>
#include "Node.h"

namespace Ui{ class MainWindow; }

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);

	void buildPoint();

	void buildTree();

	void draw();

protected:
	QVector<QVector3D> _points;
	std::unique_ptr<tree::Node> _rootNode;

private:
	Ui::MainWindow* ui = nullptr;
};
