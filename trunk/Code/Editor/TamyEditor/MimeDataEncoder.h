#pragma once

/// @file   TamyEditor\MimeDataEncoder.h
/// @brief  Common interface describing a mime data encoder 
///         used by the DropArea widget


///////////////////////////////////////////////////////////////////////////////

class QMimeData;

///////////////////////////////////////////////////////////////////////////////

class MimeDataEncoder
{
public:
   virtual ~MimeDataEncoder() {}

   /**
    * Saves the specified paths to the underlying mime data stream.
    *
    * @param data
    */
   virtual void save( QMimeData& data ) = 0;

   /**
   * Restores the specified paths from the underlying mime data stream.
    *
    * @param data
    */
   virtual void load( const QMimeData& data ) = 0;
};

///////////////////////////////////////////////////////////////////////////////
