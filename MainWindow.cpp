#include "stdafx.h"
#include "ui_MainWindow.h"
#include "MainWindow.h"
#include "QPainter"
#include "QImage"
#include "QImage"
#include "QTime"
#include "QElapsedTimer"
#include <assert.h>
#include <future>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui = new Ui::MainWindow();
	ui->setupUi(this);

	connect(ui->btnBuildPoint,&QPushButton::clicked , this, &MainWindow::buildPoint);
	connect(ui->btnBuildTree, &QPushButton::clicked, this, &MainWindow::buildTree);
	connect(ui->btnRefleshImage, &QPushButton::clicked, this, &MainWindow::drawPicture);
	connect(ui->btnSearch, &QPushButton::clicked, this, &MainWindow::searchReport);
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
	_rootNode.swap(std::make_unique<tree::Node>(tree::Seperator(QVector3D())));
	for (auto& p : _points)
	{
		_rootNode->addPoint(&p);
	}

	_rect = computeRect(_rootNode.get());
}

void MainWindow::draw(QPaintDevice * device)
{
	currdepth = 0;
	if (_rootNode)
	{
		const int margin = 10;
		auto rectMargin = _rect;

		QPainter painter(device);
		
		auto paintsize = QSizeF(device->width(), device->height());
		double scale = std::min(paintsize.width() / rectMargin.width(), paintsize.height() / rectMargin.height());
		painter.translate(-rectMargin.topLeft()*scale);
		painter.scale(scale, -scale);


		//基础颜色
		auto pen = painter.pen();
		pen.setColor(QColor(Qt::black));
		pen.setWidthF(2 / scale);
		painter.setPen(pen);
		drawPoint(painter, _rootNode.get());

		pen.setWidthF(1 / scale);
		painter.setPen(pen);
		drawLine(painter, _rootNode.get(), rectMargin);
	}
}

void MainWindow::drawPicture()
{
	auto size = ui->labPaint->size();
	QPixmap imageSave(size);
	imageSave.fill(Qt::white);
	draw(&imageSave);
	imageSave.save("temp.png", "png");
	ui->labPaint->setPixmap(imageSave);
}

QRectF MainWindow::computeRect(tree::Node* node)
{
	if (node == nullptr) return QRectF();
	QRectF ret;

	for (auto& it : node->getValues())
	{
		ret |= QRectF(it->toPointF(), QSizeF());
	}
	
	for (auto& child : node->getNodes())
	{
		ret |= computeRect(child.get());
	}

	return ret;
}


void MainWindow::paintEvent(QPaintEvent *event)
{
	__super::paintEvent(event);
}

void MainWindow::drawPoint(QPainter& painter, tree::Node* node)
{
	if (node == nullptr) return ;
	QRectF ret;


	for (auto& it : node->getValues())
	{
		painter.drawPoint(it->toPointF());
	}

	int index = 0;
	for (auto& child : node->getNodes())
	{
		drawPoint(painter,child.get());
		index++;
	}
}

void MainWindow::drawLine(QPainter& painter, tree::Node* node, QRectF rect)
{
	if (node == nullptr) return;
	//if (currdepth >= 2) return;
	currdepth++;

	auto minPoint = rect.topLeft();
	auto maxPoint = rect.bottomRight();
	auto centerPoint = node->getSeperator()._point.toPointF();

	assert(rect.contains(centerPoint));

	painter.save();
	auto pen = painter.pen();
	pen.setWidthF(pen.widthF()*0.9);
	painter.setPen(pen);

	if (minPoint.y() != maxPoint.y())
		painter.drawLine(QPointF(centerPoint.x(), minPoint.y()), QPointF(centerPoint.x(), maxPoint.y()));
	if (maxPoint.x() != minPoint.x())
		painter.drawLine(QPointF(minPoint.x(), centerPoint.y()), QPointF(maxPoint.x(), centerPoint.y()));

	{
		//画分割点
		painter.save();
		pen.setColor(Qt::green);
		pen.setWidthF(pen.widthF() * 3);
		painter.setPen(pen);
		painter.drawPoint(centerPoint);
		painter.restore();
	}

	auto& childs = node->getNodes();
	std::vector<QRectF> rects = {
		QRectF(centerPoint, maxPoint)
		, QRectF(QPointF(minPoint.x(), centerPoint.y()), QPointF(centerPoint.x(), maxPoint.y()))
		, QRectF(QPointF(centerPoint.x(), minPoint.y()), QPointF(maxPoint.x(), centerPoint.y()))
		, QRectF(minPoint, centerPoint)
	};
	for (size_t i = 0, Length = childs.size(); i < Length; ++i)
	{
		drawLine(painter, childs[i].get(), rects[i]);
	}

	painter.restore();

	currdepth--;

}

void MainWindow::searchReport()
{
	auto times = ui->edtSearchTimes->value();
	std::future<QString> normalTask = std::async(std::launch::async, [this, times](){
			QElapsedTimer time;
			time.start();
			for (size_t Index = 0, Length = times; Index < Length; ++Index)
			{
				auto i = qrand() % _points.size();
				auto pointFind = _points[i];
				for (auto& it : _points)
				{
					if (it == pointFind)
						break;
				}
			}
			return QString("%1").arg(QTime::fromMSecsSinceStartOfDay(time.elapsed()).toString("mm:ss.zzz"));
	});

	std::future<QString> treeTask = std::async(std::launch::async, [this, times](){
		QElapsedTimer time;
		time.start();
		for (size_t Index = 0, Length = times; Index < Length; ++Index)
		{
			auto i = qrand() % _points.size();
			auto pointFind = _points[i];
			if (_rootNode->findNode(pointFind) == nullptr)
			{
				return QString::fromLocal8Bit("查找错误");
			}
		}
		return QString("%1").arg(QTime::fromMSecsSinceStartOfDay(time.elapsed()).toString("mm:ss.zzz"));
	});

	normalTask.wait();
	treeTask.wait();

	ui->labForeachTime->setText(normalTask.get());
	ui->labTreeTime->setText(treeTask.get());
}
