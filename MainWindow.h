#pragma once

#include <QtWidgets/QMainWindow>
#include "Node.h"
#include "QRect"

namespace Ui{ class MainWindow; }

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);

	void buildPoint();

	void buildTree();

	void draw(QPaintDevice * device);
	void drawPicture();

	QRectF computeRect(tree::Node* node);
	void drawPoint(QPainter& painter, tree::Node* node);
	void drawLine(QPainter& painter, tree::Node* node, QRectF rect);


	void searchReport();

protected:
	QVector<QVector3D> _points;
	std::unique_ptr<tree::Node> _rootNode;
	QRectF _rect;

	virtual void paintEvent(QPaintEvent *event) override;

	int currdepth = 0;

private:
	Ui::MainWindow* ui = nullptr;
};
