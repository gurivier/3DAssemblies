
#include <iostream>

#include "Cartouche.h"
#include "AssemblyWindow.h"

using namespace std;

//=========================
//== CONSTRUCTORS public
//=========================

Cartouche::Cartouche (int number, QString filename, AssemblyWindow *assemblyWindow)
  : m_number (number),
    m_name (filename),
    m_isPresent (false),
    m_assemblyWindow (assemblyWindow)
{
  cout << "Create Cartouche " << m_number << endl ;
}

Cartouche::Cartouche (const Cartouche& src)
  : m_number (src.m_number),
    m_name (src.m_name),
    m_isPresent (src.m_isPresent)
{
  cout << "Create Cartouche " << m_number << endl ;
}

//=========================
//== METHODS public
//=========================

void Cartouche::appear ()
{
  m_isPresent = true ;

  m_assemblyWindow->showFragment(m_number) ;

  cout << "Cartouche " << m_number << " appears" << endl ;
}

void Cartouche::disappear ()
{
  m_isPresent = false ;

  m_assemblyWindow->hideFragment(m_number) ;

  cout << "Cartouche " << m_number << " disappears" << endl ;
}
