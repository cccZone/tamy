#pragma once

/// @file   core\Serializer.h
/// @brief  tool for saving/loading serializables to external storage

#include "core\UniqueObject.h"
#include <string>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * This class gives the serializables abilities to save and restore 
 * their state from an external device.
 */
class Serializer
{
public:
   virtual ~Serializer() {}

   /**
    * The method saves an integer.
    *
    * @param val     value to save
    */
   virtual void saveInt(int val) = 0;

   /**
    * Restores the next integer from the underlying stream of data.
    *
    * @return        serialized integer value
    */
   virtual int loadInt() = 0;

   /**
    * The method saves an float.
    *
    * @param val     value to save
    */
   virtual void saveFloat(float val) = 0;

   /**
    * Restores the next float from the underlying stream of data.
    *
    * @return        serialized float value
    */
   virtual float loadFloat() = 0;

   /**
    * The method saves a boolean value.
    *
    * @param val     value to save
    */
   virtual void saveBool(bool val) = 0;

   /**
    * Restores the next bool from the underlying stream of data.
    *
    * @return        serialized bool value
    */
   virtual bool loadBool() = 0;

   /**
    * The method saves a character.
    *
    * @param val     value to save
    */
   virtual void saveChar(char val) = 0;

   /**
    * Restores the next char from the underlying stream of data.
    *
    * @return        serialized char value
    */
   virtual char loadChar() = 0;

   /**
    * The method saves a long value.
    *
    * @param val     value to save
    */
   virtual void saveLong(long val) = 0;

   /**
    * Restores the next long value from the underlying stream of data.
    *
    * @return        serialized long value
    */
   virtual long loadLong() = 0;

   /**
    * The method saves a string.
    *
    * @param str     string to save
    */
   virtual void saveString(const std::string& str) = 0;

   /**
    * Restores a string from the underlying stream of data.
    *
    * @return        serialized string
    */
   virtual void loadString(std::string& str) = 0;

   /**
    * The method saves a matrix.
    *
    * @param mtx     matrix to save
    */
   virtual void saveMatrix(const D3DXMATRIX& mtx) = 0;

   /**
    * Restores a matrix from the underlying stream of data.
    *
    * @return        serialized matrix
    */
   virtual void loadMatrix(D3DXMATRIX& mtx) = 0;

   /**
    * The method saves a vector.
    *
    * @param mtx     vector to save
    */
   virtual void saveVec3(const D3DXVECTOR3& vec) = 0;

   /**
    * Restores a vector from the underlying stream of data.
    *
    * @return        serialized vector
    */
   virtual void loadVec3(D3DXVECTOR3& vec) = 0;
};

///////////////////////////////////////////////////////////////////////////////
