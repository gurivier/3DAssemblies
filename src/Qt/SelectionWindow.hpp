#ifndef SECONDWINDOW_HPP
#define SECONDWINDOW_HPP

//#include <QtGui>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QToolButton>
#include <QStandardItemModel>

#include <vector>

#include "MediaList.hpp"

#include "ui_SelectionWindowBase.h"

class Controller ;

class SelectionWindow : public QMainWindow, private Ui::SelectionWindowBase
{
  Q_OBJECT
 
public:

  SelectionWindow(QWidget *parent, Controller *controller, MediaList *medias);

  ~SelectionWindow();

  void loadDirectory (int i) ;

private slots:

  void objectClickedForLeftHand (int j) ;
  void objectClickedForRightHand (int j) ;

  void handleSelectionChanged(const QItemSelection& selection) ;

private:

  QListView *m_listView ;

  QStandardItemModel *m_model ;

  QGridLayout *m_gridFiles ;

  Controller *m_controller ;

  MediaList *m_medias ;

  std::vector<QFrame *> m_frames ;
  std::vector<QLabel *> m_labels ;
  std::vector<QLabel *> m_images ;

  int m_curDir ;

};
 
#endif /* SECONDWINDOW_HPP */
