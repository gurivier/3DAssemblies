#ifndef MEDIALIST_HPP
#define MEDIALIST_HPP

#include <vector>
#include <string>

class Media {

public:

  Media (const char *path, const char *dirname) ;
  ~Media () ;

  const char * getName () ;
  const char * getFullName () ;
  int size () ;
  const char * getElementFullName (int i) ;
  const char * getElementName (int i) ;

private:

  std::string m_name ;
  std::string m_fullName ;

  std::vector<std::string> m_listFullName ;
  std::vector<std::string> m_listName ;

} ;


class MediaList {

public:

  MediaList (const char *path) ;

  ~MediaList () ;

  int size () ;

  Media & getElement (int i) ;

  void display () ;

private:

  std::vector<Media> m_list ;

} ;


#endif /* MEDIALIST_HPP */


