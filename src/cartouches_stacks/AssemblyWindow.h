
#ifndef ASSEMBLYWINDOW_H
#define ASSEMBLYWINDOW_H

#include <QtGui/QImage>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QList>
#include <QtWidgets/QMainWindow>
#include <vector>

#include "AssemblyWindow.h"
#include "TargetManager.h"

class AssemblyWindow : public QMainWindow
{
  Q_OBJECT

 public:

  //=========================
  //== CONSTRUCTORS public
  //=========================

  AssemblyWindow() ;

 public:

  //=========================
  //== ACCESSORS public
  //=========================

  inline void setTargetManager(TargetManager *targetManager) { m_targetManager = targetManager ; }


 public:

  //=========================
  //== METHODS public
  //=========================

  bool loadFragment(const QString &fileName) ;

  void showFragment(const int i) ;

  void hideFragment(const int i) ;


 protected:

  //=========================
  //== METHODS protected
  //=========================

  void closeEvent (QCloseEvent *event) ;


 private:

  //=========================
  //== MEMBERS private
  //=========================

  QHBoxLayout *centralLayout ;

  std::vector<QLabel*> m_labels ;

  std::vector<QImage*> m_images ;

  TargetManager *m_targetManager ;

  int m_numFragment ;

};

#endif /* ASSEMBLYWINDOW_H */
