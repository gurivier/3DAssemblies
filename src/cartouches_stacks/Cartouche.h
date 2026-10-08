#ifndef CARTOUCHE_H
#define CARTOUCHE_H

#include <QString>

class AssemblyWindow ;

class Cartouche {

 public:

  //=========================
  //== CONSTRUCTORS public
  //=========================

  Cartouche (int number, QString filename, AssemblyWindow *assemblyWindow) ;

  Cartouche (const Cartouche& src) ;


 public:

  //=========================
  //== ACCESSORS public
  //=========================

  inline bool isPresent () { return m_isPresent ; }

  inline int getNumber () { return m_number ; }

  inline QString getFilename () { return m_name ; }


 public:

  //=========================
  //== METHODS public
  //=========================

  void appear () ;

  void disappear () ;


  //=========================
  //== MEMBERS private
  //=========================

 private:

  int     m_number ;

  QString m_name ;

  bool    m_isPresent ;

  AssemblyWindow *m_assemblyWindow ;

};

#endif /* CARTOUCHE_H */
