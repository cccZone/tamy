#ifndef _STREAM_BUFFER_H
#error "This file can only be included from StreamBuffer.h"
#else

#include "core\File.h"


///////////////////////////////////////////////////////////////////////////////

template<typename T>
StreamBuffer<T>::StreamBuffer(File* file)
: m_buffer(NULL)
, m_size(0)
{
   if (file == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a File instance");
   }

   file->seek(0, std::ios_base::end);
   unsigned int size = file->tell();
   file->seek(0, std::ios_base::beg);

   if (size < sizeof(T))
   {
      m_size = 0;
      m_buffer = new T[1];
   }
   else
   {
      m_size = (size + (sizeof(T) - 1)) / sizeof(T);
      m_buffer = new T[m_size];
      memset(m_buffer, 0, m_size * sizeof(T));
      file->read((byte*)(m_buffer), size);
   }

   delete file;
}

///////////////////////////////////////////////////////////////////////////////

template<typename T>
StreamBuffer<T>::~StreamBuffer()
{
   delete [] m_buffer; m_buffer = NULL;
   m_size = 0;
}

///////////////////////////////////////////////////////////////////////////////

template<typename T>
T* StreamBuffer<T>::getBuffer() 
{
   return m_buffer;
}

///////////////////////////////////////////////////////////////////////////////

template<typename T>
T& StreamBuffer<T>::operator[] (unsigned int idx) 
{
   return m_buffer[idx];
}

///////////////////////////////////////////////////////////////////////////////

template<typename T>
const T& StreamBuffer<T>::operator[] (unsigned int idx) const 
{
   return m_buffer[idx];
}

///////////////////////////////////////////////////////////////////////////////

template<typename T>
std::size_t StreamBuffer<T>::size() 
{
   return m_size;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _STREAM_BUFFER_H