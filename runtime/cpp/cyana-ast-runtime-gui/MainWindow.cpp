#include "MainWindow.h"

#include <QFileDialog>

#include "ui_MainWindow.h"

MainWindow::MainWindow(const StringTree *stringTree, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  this->setWindowTitle(tr("csyan-ast-gui"));

  ui->scaleSlider->setRange(-1000, 1000);
  ui->scaleSlider->setValue(0);
  connect(ui->scaleSlider, &QSlider::valueChanged, this,
          &MainWindow::on_treeViewScaleSliderValueChanged);

  sence = new QGraphicsScene();
  stringTreeGraphicsItem = new StringTreeGraphicsItem(stringTree);

  sence->sceneRect();

  sence->addItem(stringTreeGraphicsItem);
  ui->graphicsView->setScene(sence);
}

MainWindow::~MainWindow() {
  delete stringTreeGraphicsItem;
  stringTreeGraphicsItem = 0;
  delete sence;
  sence = 0;
  delete ui;
  ui = 0;
}

void MainWindow::closeEvent(QCloseEvent *event) { app->exit(0); }

void MainWindow::on_exportImgBtn_clicked() {
  QString imgFilePath = QFileDialog::getSaveFileName(
      this, tr("Save Picture"), "/", "PNG(*.png);;JPG(*.jpg);;BMP(*.bmp)");
  if (imgFilePath.isEmpty()) {
    return;
  } else {
    QPixmap stringTreeQPixmap(stringTreeGraphicsItem->boundingRect().width(),
                              stringTreeGraphicsItem->boundingRect().height());
    stringTreeQPixmap.fill(Qt::white);
    QPainter painter(&stringTreeQPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    QStyleOptionGraphicsItem opt;
    painter.translate(-1 * (stringTreeGraphicsItem->boundingRect().topLeft()));
    stringTreeGraphicsItem->paint(&painter, &opt, 0);
    stringTreeQPixmap.save(imgFilePath);
  }
}

void MainWindow::on_treeViewScaleSliderValueChanged(int value) {
  // 还原原始大小
  ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
  QMatrix originalQMatrix;
  originalQMatrix.setMatrix(
      1, ui->graphicsView->matrix().m12(), ui->graphicsView->matrix().m21(), 1,
      ui->graphicsView->matrix().dx(), ui->graphicsView->matrix().dy());
  ui->graphicsView->setMatrix(originalQMatrix, false);
  //缩放
  qreal scale = value / 1000.0 + 1.0;
  ui->graphicsView->scale(scale, scale);
}
