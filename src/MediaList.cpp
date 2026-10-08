
#include <iostream>
#include <cstdio>        /* std::perror() */

#include <stdlib.h>
#include <dirent.h>
#include <string.h>      /* strerror() */
#include <errno.h>      /* errno */

#include "MediaList.hpp"

int filter_directory (const struct dirent *entry) {
  return (entry->d_type == DT_DIR) && strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..") ;
}

int filter_file_obj (const struct dirent *entry) {
  const char *extension = entry->d_name + strlen(entry->d_name) - 4 ;
  return (entry->d_type == DT_REG) && !strcmp(extension, ".obj") ;
}


Media::Media (const char *path, const char *dirname)
  : m_name (dirname),
    m_fullName ("")
{
  struct dirent **namelist;
  int n;

  m_fullName += path ;
  if (path[strlen(path)-1] != '/')
    m_fullName += "/" ;
  m_fullName += dirname ;

  n = scandir(m_fullName.c_str(), &namelist, filter_file_obj, alphasort);
  if (n < 0) {
    std::cerr << "Scandir error" << std::endl ;
    std::perror( strerror(errno) );
    exit (1) ;
  }
  else {
    while (n--) {
      //std::cout << " == " << namelist[n]->d_name << std::endl ;

      m_listName.push_back (namelist[n]->d_name) ;
      m_listFullName.push_back (m_fullName + "/" + namelist[n]->d_name) ;

      free(namelist[n]);
    }
    free(namelist);
  }

}

Media::~Media ()
{
  m_listFullName.clear();
  m_listName.clear();
}

const char * Media::getName () 
{
  return m_name.c_str () ;
}

const char * Media::getFullName () 
{
  return m_fullName.c_str () ;
}

int Media::size ()
{
  return m_listName.size() ;
}

const char * Media::getElementFullName (int i) 
{
  return m_listFullName[i].c_str() ;
}

const char * Media::getElementName (int i) 
{
  return m_listName[i].c_str() ;
}

MediaList::MediaList (const char *path)
{
  struct dirent **namelist;
  int n;

  n = scandir(path, &namelist, filter_directory, alphasort);
  if (n < 0) {
    std::cerr << "Scandir error" << std::endl ;
    std::perror( strerror(errno) );
    exit (1) ;
  }
  else {
    while (n--) {
      //std::cout << "** " << namelist[n]->d_name << std::endl ;

      m_list.push_back(Media (path, namelist[n]->d_name)) ;

      free(namelist[n]);
    }
    free(namelist);
  }

}

MediaList::~MediaList ()
{

}

int MediaList::size ()
{
  return m_list.size () ;
}

Media & MediaList::getElement (int i)
{
  return m_list[i] ;
}

void MediaList::display ()
{
  std::cout << this->size() << std::endl ;
  for (int i=0 ; i < this->size() ; i++) {
    std::cout << "== " << this->getElement(i).getName() << std::endl ;
    for (int j=0 ; j < this->getElement(i).size () ; j++) {
      std::cout << "** " << this->getElement(i).getElementFullName(j) << std::endl ;
    }
  }
}
