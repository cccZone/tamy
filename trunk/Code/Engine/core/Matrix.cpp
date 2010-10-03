#include "core\Matrix.h"
#include "core\Vector.h"
#include "core\EulerAngles.h"
#include "core\Assert.h"

#define _USE_MATH_DEFINES
#include <math.h>

///////////////////////////////////////////////////////////////////////////////

BEGIN_RTTI(Matrix)
END_RTTI

///////////////////////////////////////////////////////////////////////////////

Matrix Matrix::IDENTITY;

///////////////////////////////////////////////////////////////////////////////

Matrix::Matrix()
{
   memset( &m, 0, sizeof(float) * 16 );
   m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1;
}

///////////////////////////////////////////////////////////////////////////////

Matrix::Matrix(const float* values)
{
   memcpy( &m, values, sizeof(float) * 16 );
}

///////////////////////////////////////////////////////////////////////////////

Matrix::Matrix(const D3DXMATRIX& mtx)
{
   for ( int row = 0; row < 4; ++row )
   {
      for ( int col = 0; col < 4; ++col )
      {
         m[row][col] = mtx.m[col][row];
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

Matrix::Matrix(const EulerAngles& angles)
{
   D3DXMATRIX mtx;
   D3DXMatrixRotationYawPitchRoll( &mtx, DEG2RAD( angles.yaw ), DEG2RAD( angles.pitch ), DEG2RAD( angles.roll ) );

   for ( int row = 0; row < 4; ++row )
   {
      for ( int col = 0; col < 4; ++col )
      {
         m[row][col] = mtx.m[col][row];
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

Matrix::Matrix( const Vector& axis, float angle )
{
   D3DXVECTOR3 dxAxis = axis;

   D3DXMATRIX mtx;
   D3DXMatrixRotationAxis(&mtx, &dxAxis, angle);
   *this = mtx;
}

///////////////////////////////////////////////////////////////////////////////

Matrix::Matrix( const Vector& translation )
{
   memset( &m, 0, sizeof(float) * 16 );
   m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1;
   m[0][3] = translation.x;
   m[1][3] = translation.y;
   m[2][3] = translation.z;
}

///////////////////////////////////////////////////////////////////////////////

bool Matrix::operator==(const Matrix& rhs) const
{
   for ( int row = 0; row < 4; ++row )
   {
      for ( int col = 0; col < 4; ++col )
      {
         if ( m[row][col] != rhs.m[row][col] )
         {
            return false;
         }
      }
   }

   return true;
}

///////////////////////////////////////////////////////////////////////////////

bool Matrix::operator!=(const Matrix& rhs) const
{
   return !(*this == rhs);
}

///////////////////////////////////////////////////////////////////////////////

Matrix& Matrix::operator=(const Matrix& rhs)
{
   for ( int row = 0; row < 4; ++row )
   {
      for ( int col = 0; col < 4; ++col )
      {
         m[row][col] = rhs.m[row][col];
      }
   }

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Matrix Matrix::operator+(const Matrix& rhs) const
{
   Matrix newMtx( *this );

   for ( int row = 0; row < 4; ++row )
   {
      for ( int col = 0; col < 4; ++col )
      {
         newMtx.m[row][col] += rhs.m[row][col];
      }
   }

   return newMtx;
}

///////////////////////////////////////////////////////////////////////////////

Matrix& Matrix::operator+=(const Matrix& rhs)
{
   for ( int row = 0; row < 4; ++row )
   {
      for ( int col = 0; col < 4; ++col )
      {
         m[row][col] += rhs.m[row][col];
      }
   }

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Matrix Matrix::operator-(const Matrix& rhs) const
{
   Matrix newMtx( *this );

   for ( int row = 0; row < 4; ++row )
   {
      for ( int col = 0; col < 4; ++col )
      {
         newMtx.m[row][col] -= rhs.m[row][col];
      }
   }

   return newMtx;
}

///////////////////////////////////////////////////////////////////////////////

Matrix& Matrix::operator-=(const Matrix& rhs)
{
   for ( int row = 0; row < 4; ++row )
   {
      for ( int col = 0; col < 4; ++col )
      {
         m[row][col] -= rhs.m[row][col];
      }
   }

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Matrix Matrix::operator*(const Matrix& rhs) const
{
   Matrix newMtx;

   for ( int row = 0; row < 4; ++row )
   {
      for ( int col = 0; col < 4; ++col )
      {
         newMtx.m[row][col] = 0;

         for ( int i = 0; i < 4; ++i )
         {
            newMtx.m[row][col] += m[row][i] * rhs.m[i][col];
         }
      }
   }


   return newMtx;
}

///////////////////////////////////////////////////////////////////////////////

Matrix& Matrix::operator*=(const Matrix& rhs)
{
   *this = *this * rhs;

   return *this;
}

///////////////////////////////////////////////////////////////////////////////
   
Matrix Matrix::operator+(float val) const
{
   Matrix newMtx( *this );

   for ( int row = 0; row < 4; ++row )
   {
      for ( int col = 0; col < 4; ++col )
      {
         newMtx.m[row][col] += val;
      }
   }

   return newMtx;
}

///////////////////////////////////////////////////////////////////////////////
   
Matrix& Matrix::operator+=(float val)
{
   for ( int row = 0; row < 4; ++row )
   {
      for ( int col = 0; col < 4; ++col )
      {
         m[row][col] += val;
      }
   }

   return *this;
}

///////////////////////////////////////////////////////////////////////////////
   
Matrix Matrix::operator-(float val) const
{
   Matrix newMtx( *this );

   for ( int row = 0; row < 4; ++row )
   {
      for ( int col = 0; col < 4; ++col )
      {
         newMtx.m[row][col] -= val;
      }
   }

   return newMtx;
}

///////////////////////////////////////////////////////////////////////////////

Matrix& Matrix::operator-=(float val)
{
   for ( int row = 0; row < 4; ++row )
   {
      for ( int col = 0; col < 4; ++col )
      {
         m[row][col] -= val;
      }
   }

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Matrix Matrix::operator*(float val) const
{
   Matrix newMtx( *this );

   for ( int row = 0; row < 4; ++row )
   {
      for ( int col = 0; col < 4; ++col )
      {
         newMtx.m[row][col] *= val;
      }
   }

   return newMtx;
}

///////////////////////////////////////////////////////////////////////////////

Matrix& Matrix::operator*=(float val)
{
   for ( int row = 0; row < 4; ++row )
   {
      for ( int col = 0; col < 4; ++col )
      {
         m[row][col] *= val;
      }
   }

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Matrix Matrix::operator/(float val) const
{
   ASSERT_MSG( val != 0, "Division by 0 not supported" );

   Matrix newMtx( *this );

   for ( int row = 0; row < 4; ++row )
   {
      for ( int col = 0; col < 4; ++col )
      {
         newMtx.m[row][col] /= val;
      }
   }

   return newMtx;
}

///////////////////////////////////////////////////////////////////////////////

Matrix& Matrix::operator/=(float val)
{
   ASSERT_MSG( val != 0, "Division by 0 not supported" );

   for ( int row = 0; row < 4; ++row )
   {
      for ( int col = 0; col < 4; ++col )
      {
         m[row][col] /= val;
      }
   }

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Matrix::operator D3DXMATRIX() const
{
   D3DXMATRIX mtx;

   for ( int row = 0; row < 4; ++row )
   {
      for ( int col = 0; col < 4; ++col )
      {
         mtx.m[col][row] = m[row][col];
      }
   }

   return mtx;
}

///////////////////////////////////////////////////////////////////////////////

Vector Matrix::rightVec() const
{
   return Vector( m[0][0], m[1][0], m[2][0] );
}

///////////////////////////////////////////////////////////////////////////////

Vector Matrix::upVec() const
{
   return Vector( m[0][1], m[1][1], m[2][1] );
}

///////////////////////////////////////////////////////////////////////////////

Vector Matrix::lookVec() const
{
   return Vector( m[0][2], m[1][2], m[2][2] );
}

///////////////////////////////////////////////////////////////////////////////

Vector Matrix::position() const
{
   return Vector( m[0][3], m[1][3], m[2][3] );
}

///////////////////////////////////////////////////////////////////////////////

Vector Matrix::scale() const
{
   return Vector( m[0][0], m[1][1], m[2][2] ).normalized();
}

///////////////////////////////////////////////////////////////////////////////

Matrix::operator EulerAngles() const
{
   float yaw, pitch, roll;

   if ( m[1][0] > 0.998f ) 
   { 
      // singularity at north pole
      yaw = atan2( m[0][2], m[2][2] );
      pitch = (float)M_PI_2;
      roll = 0;
   }
   if ( m[1][0] < -0.998f ) 
   { 
      // singularity at south pole
      yaw = atan2( m[0][2], m[2][2] );
      pitch = (float)-M_PI_2;
      roll = 0;
   }
   else
   {
      yaw = atan2( -m[2][0], m[0][0] );
      pitch = atan2( -m[1][2], m[1][1] );
      roll = asin( m[1][0] );
   }

   return EulerAngles( RAD2DEG( yaw ), RAD2DEG( pitch ), RAD2DEG( roll ) );
}

///////////////////////////////////////////////////////////////////////////////

Vector Matrix::transform(const Vector& rhs) const
{
   float inV[] = {rhs.x, rhs.y, rhs.z, 1};
   float v[] = {0, 0, 0, 0};
   for ( int i = 0; i < 4; ++i )
   {
      for ( int j = 0; j < 4; ++j )
      {
         v[i] += m[i][j] * inV[j];
      }
   }

   return Vector( v[0] / v[3], v[1] / v[3], v[2] / v[3] );
}

///////////////////////////////////////////////////////////////////////////////

Vector Matrix::transformNorm(const Vector& rhs) const
{
   return transform(rhs).normalized();
}

///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<( std::ostream& stream, const Matrix& rhs )
{
   stream << "[ ";
   for (int row = 0; row < 4; ++row)
   {
      stream << "[";
      for (int col = 0; col < 4; ++col )
      {
         stream << rhs.m[row][col];
         if ( col < 3 )
         {
            stream << ", ";
         }
      }
      stream << "] ";
   }
   stream << "]";

   return stream;
}

///////////////////////////////////////////////////////////////////////////////
