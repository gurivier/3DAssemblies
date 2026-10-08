#include <QtGui>
#include <iostream>

#include "CameraArea.h"

//=========================
//== CONSTRUCTORS public
//=========================

CameraArea::CameraArea(QWidget *parent, TargetManager *targetManager)
  : QWidget(parent),
    m_targetManager (targetManager),
    m_currentTargetNum (0),
    m_mouseTargetPosition (0, 0),
    m_drawingMouseTarget (false),
    m_targetSize (targetManager->getTargetSize()),
    m_imageWidth (targetManager->getImageWidth()),
    m_imageHeight (targetManager->getImageHeight())
{
  setAttribute(Qt::WA_StaticContents);
  m_penWidth = 1 ;
}

//=========================
//== METHODS public
//=========================

bool CameraArea::openImage(const QString &fileName)
{
  QImage loadedImage;
  if (!loadedImage.load(fileName))
    return false;

  QSize newSize (m_imageWidth, m_imageHeight);
  resizeImage(&loadedImage, newSize);
  m_image = loadedImage;
  update();
  return true;
}

void CameraArea::openImage(const uchar *data, int width, int height)
{
  QImage loadedImage(data, width, height, QImage::Format_RGB888);
  QSize newSize (width, height);
  resizeImage(&loadedImage, newSize);
  m_image = loadedImage;
  update();
}

bool CameraArea::saveImage(const QString &fileName, const char *fileFormat)
{
  QImage visibleImage = m_image;
  resizeImage(&visibleImage, size());
  return visibleImage.save(fileName, fileFormat) ;
}

void CameraArea::drawTargets ()
{
  for (int i=0 ; i<m_targetManager->size() ; i++)
    drawTarget (m_targetManager->getTargetPosition(i), i+1, m_targetManager->getTargetState(i), false);

  if (m_drawingMouseTarget)
    drawTarget (m_mouseTargetPosition, m_currentTargetNum+1, false, true);
}

//=========================
//== SLOTS public
//=========================

void CameraArea::clearImage()
{
  m_image.fill(qRgb(255, 255, 255));
  update();
}

//=========================
//== METHODS protected
//=========================

void CameraArea::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton) {
    m_mouseTargetPosition = event->pos();
    m_drawingMouseTarget = true ;

    // Check if clicked in target
    for (int i=0 ; i<m_targetManager->size() ; i++) {
      QPoint pos = m_targetManager->getTargetPosition(i);

      int Xmin = pos.x() - m_targetSize*0.5;
      int Xmax = pos.x() + m_targetSize*0.5;
      int Ymin = pos.y() - m_targetSize*0.5;
      int Ymax = pos.y() + m_targetSize*0.5;

      if (m_mouseTargetPosition.x() >= Xmin && m_mouseTargetPosition.x() <= Xmax
	  && m_mouseTargetPosition.y() >= Ymin && m_mouseTargetPosition.y() <= Ymax)
	m_currentTargetNum = i;
    }
  }
}

void CameraArea::mouseMoveEvent(QMouseEvent *event)
{
  if ((event->buttons() & Qt::LeftButton) && m_drawingMouseTarget)
    m_mouseTargetPosition = event->pos();
}

void CameraArea::mouseReleaseEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton) {
    m_drawingMouseTarget = false ;
    m_targetManager->setTargetPosition(m_currentTargetNum, event->pos());
    nextTarget();
  }
}

void CameraArea::paintEvent(QPaintEvent * /* event */)
{
  QPainter painter(this);
  painter.drawImage(QPoint(0, 0), m_image);
}

void CameraArea::resizeEvent(QResizeEvent *event)
{
  if (width() > m_image.width() || height() > m_image.height()) {
    resizeImage(&m_image, QSize(m_imageWidth, m_imageHeight));
    update();
  }
  QWidget::resizeEvent(event);
}

//=========================
//== METHODS private
//=========================

void CameraArea::drawTarget(const QPoint &point, const int targetNum, const bool targetIsActive, const bool move)
{
  QPainter painter(&m_image);

  QPoint p1(point.x()-m_targetSize*0.5, point.y()-m_targetSize*0.5);
  QPoint p2(point.x()+m_targetSize*0.5, point.y()+m_targetSize*0.5);

  QRect rect(p1, p2) ;

  // draw target
  if (move)
    painter.setPen(QPen(Qt::blue, m_penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  else if (targetIsActive)
    painter.setPen(QPen(Qt::green, m_penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  else
    painter.setPen(QPen(Qt::red, m_penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));      

  painter.drawRect(rect);

  // draw number
  if (move)
    painter.setPen(QPen(Qt::blue, m_penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  else
    painter.setPen(QPen(Qt::black, m_penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  painter.drawText(p2, QString::number(targetNum));

  int rad = (m_penWidth / 2) + 2;
  update(rect.normalized().adjusted(-rad, -rad, +rad, +rad));
}

void CameraArea::resizeImage(QImage *image, const QSize &newSize)
{
  if (image->size() == newSize)
    return;

  QImage newImage(newSize, QImage::Format_RGB32);
  newImage.fill(qRgb(255, 255, 255));
  QPainter painter(&newImage);
  painter.drawImage(QPoint(0, 0), *image);
  *image = newImage;
}
