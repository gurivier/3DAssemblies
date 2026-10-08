
/* function template maximum */

template < class T > // or template< typename T >
T maximum( T value1, T value2, T value3 )
{
  T maximumValue = (value1 > value2) ? value1 : value2 ;
  if (value3 > maximumValue)
    maximumValue = value3 ;
  return maximumValue ;
}

template < class T > // or template< typename T >
T maximum( T value1, T value2)
{
  return (value1 > value2) ? value1 : value2 ;
}

