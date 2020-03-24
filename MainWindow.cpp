#include "stdafx.h"
#include "ui_MainWindow.h"
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui = new Ui::MainWindow();
	ui->setupUi(this);

	connect(ui->btnBuildPoint,&QPushButton::clicked , this, &MainWindow::buildPoint);
	connect(ui->btnBuildTree, &QPushButton::clicked, this, &MainWindow::buildTree);
	connect(ui->btnRefleshImage, &QPushButton::clicked, this, &MainWindow::draw);
	qsrand(QTime::currentTime().msec());

	buildPoint();
}

void MainWindow::buildPoint()
{
	_points.clear();

	size_t count = ui->edtPointCount->value();
	
	_points.resize(count);
	for (size_t i = 0; i < count ; ++i)
	{
		auto& p = _points[i];
		for (size_t j = 0,Length = 2; j < Length ; ++j)
		{
			auto r = (double)(qrand() % 10000);
			p[j] = (float)(r / 10000 - 0.5) * count;
		}
	}
}

void MainWindow::buildTree()
{
	_rootNode.swap(std::make_unique<tree::Node>());
	for (auto& p : _points)
	{
		_rootNode->addPoint(p);
	}
}

void MainWindow::draw()
{

}
