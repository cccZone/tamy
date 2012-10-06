#include "core\Matrix.h"
#include "core\Vector.h"
#include "core\EulerAngles.h"
#include "core\Quaternion.h"
#include "core\Assert.h"
#include "core\OutStream.h"
#include "core\InStream.h"
#include "core\MathDefs.h"


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

Matrix::Matrix( float _11, float _12, float _13, float _14, 
                float _21, float _22, float _23, float _24, 
                float _31, float _32, float _33, float _34,
                float _41, float _42, float _43, float _44 )
{
   m[0][0] = _11;
   m[0][1] = _12;
   m[0][2] = _13;
   m[0][3] = _14;
   
   m[1][0] = _21;
   m[1][1] = _22;
   m[1][2] = _23;
   m[1][3] = _24;
   
   m[2][0] = _31;
   m[2][1] = _32;
   m[2][2] = _33;
   m[2][3] = _34;

   m[3][0] = _41;
   m[3][1] = _42;
   m[3][2] = _43;
   m[3][3] = _44;
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

Matrix& Matrix::operator=( const Matrix& rhs )
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

Matrix& Matrix::setTranslation( const Vector& translationVec )
{
   memset( &m, 0, sizeof(float) * 16 );
   m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1;
   m[3][0] = translationVec.x;
   m[3][1] = translationVec.y;
   m[3][2] = translationVec.z;

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Matrix& Matrix::setRotation( const Quaternion& q )
{
   Vector quatVec;
   quatVec.load( q.m_quad );

   Vector q2; q2.setAdd( quatVec, quatVec );
   Vector xq2; xq2.setMul( q2, quatVec.x );
   Vector yq2; yq2.setMul( q2, quatVec.y );
   Vector zq2; zq2.setMul( q2, quatVec.z );
   Vector wq2; wq2.setMul( q2, quatVec.w );

   Vector c0; c0.set( 1.0f - ( yq2.y + zq2.z ), xq2.y + wq2.z , xq2.z - wq2.y, 0.0f );
   Vector c1; c1.set( xq2.y - wq2.z, 1.0f - ( xq2.x + zq2.z), yq2.z + wq2.x, 0.0f );
   Vector c2; c2.set( xq2.z + wq2.y , yq2.z - wq2.x, 1.0f - ( xq2.x + yq2.y ), 0.0f );

   setRows( c0, c1, c2 );
   
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

void Matrix::getRotation( Quaternion& q ) const
{
   float trace = m[0][0] + m[1][1] + m[2][2];

   if( trace > -1.0f ) 
   {
      float s = sqrt( trace + 1.0f ) * 2.0f;
      q[3] = 0.25f * s;
      q[0] = ( m[1][2] - m[2][1] ) / s;
      q[1] = ( m[2][0] - m[0][2] ) / s;
      q[2] = ( m[0][1] - m[1][0] ) / s;
   } 
   else if ( m[0][0] > m[1][1] && m[0][0] > m[2][2] ) 
   {
      float s = 2.0f * sqrt( 1.0f + m[0][0] - m[1][1] - m[2][2]);
      q[3] = (m[1][2] - m[2][1] ) / s;
      q[0] = 0.25f * s;
      q[1] = (m[0][1] + m[1][0] ) / s;
      q[2] = (m[0][2] + m[2][0] ) / s;
   } 
   else if ( m[1][1] > m[2][2] ) 
   {
      float s = 2.0f * sqrt( 1.0f + m[1][1] - m[0][0] - m[2][2]);
      q[3] = (m[0][2] - m[2][0] ) / s;
      q[0] = (m[0][1] + m[1][0] ) / s;
      q[1] = 0.25f * s;
      q[2] = (m[1][2] + m[2][1] ) / s;
   } 
   else 
   {
      float s = 2.0f * sqrt( 1.0f + m[2][2] - m[0][0] - m[1][1] );
      q[3] = (m[1][0] - m[0][1] ) / s;
      q[0] = (m[0][2] + m[2][0] ) / s;
      q[1] = (m[1][2] + m[2][1] ) / s;
      q[2] = 0.25f * s;
   }
   
   q.normalize();
}

///////////////////////////////////////////////////////////////////////////////

Matrix& Matrix::setRotation( const EulerAngles& angles )
{
   // Assuming the angles are in radians.
   double cy = cos( DEG2RAD( angles[ EulerAngles::Yaw ] ) );
   double sy = sin( DEG2RAD( angles[ EulerAngles::Yaw ] ) );
   double cr = cos( DEG2RAD( angles[ EulerAngles::Roll ] ) );
   double sr = sin( DEG2RAD( angles[ EulerAngles::Roll ] ) );
   double cp = cos( DEG2RAD( angles[ EulerAngles::Pitch ] ) );
   double sp = sin( DEG2RAD( angles[ EulerAngles::Pitch ] ) );

  // Mtx(Roll) * Mtx(Pitch) * Mtx(Yaw) - to be in sync with the counterpart method Quaternion::setFromEulerAngles( const EulerAngles& angles )

   m[0][0] = (float)( cr*cy + sr*sp*sy );
   m[0][1] = (float)( sr*cp );
   m[0][2] = (float)( sr*sp*cy - sy*cr );
   m[0][3] = 0.0f;

   m[1][0] = (float)( cr*sp*sy - sr*cy );
   m[1][1] = (float)( cr*cp );
   m[1][2] = (float)( sr*sy + cr*sp*cy );
   m[1][3] = 0.0f;

   m[2][0] = (float)( cp*sy );
   m[2][1] = (float)( -sp );
   m[2][2] = (float)( cp*cy );
   m[2][3] = 0.0f; 

   m[3][0] = m[3][1] = m[3][2] = 0.0f;
   m[3][3] = 1.0f;

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

void Matrix::getRotation( EulerAngles& outAngles ) const
{
   if ( m[2][1] < -0.998f ) 
   { 
      // singularity at north pole
      outAngles[ EulerAngles::Yaw ] = -atan2( m[0][2], m[1][2] );
      outAngles[ EulerAngles::Pitch ] = (float)M_PI_2;
      outAngles[ EulerAngles::Roll ] = 0;
   }
   else if ( m[2][1] > 0.998f ) 
   { 
      // singularity at south pole
      outAngles[ EulerAngles::Yaw ] = -atan2( m[0][2], -m[1][2] );
      outAngles[ EulerAngles::Pitch ] = (float)-M_PI_2;
      outAngles[ EulerAngles::Roll ] = 0;
   }
   else
   {
      outAngles[ EulerAngles::Yaw ] = atan2( m[2][0], m[2][2] );
      outAngles[ EulerAngles::Roll ] = atan2( m[0][1], m[1][1] );
      outAngles[ EulerAngles::Pitch ] = asin( -m[2][1] );
   }

   outAngles[ EulerAngles::Yaw ] = RAD2DEG( outAngles[ EulerAngles::Yaw ] );
   outAngles[ EulerAngles::Pitch ] = RAD2DEG( outAngles[ EulerAngles::Pitch ] );
   outAngles[ EulerAngles::Roll ] = RAD2DEG( outAngles[ EulerAngles::Roll ] );
}

///////////////////////////////////////////////////////////////////////////////

Matrix& Matrix::setMul( const Matrix& a, const Matrix& b )
{
   ASSERT_MSG( &a != this, "Passing itself as one of this method parameters will yield unpredicted results" );
   ASSERT_MSG( &b != this, "Passing itself as one of this method parameters will yield unpredicted results" );

   for ( int row = 0; row < 4; ++row )
   {
      for ( int col = 0; col < 4; ++col )
      {
         m[row][col] = 0;

         for ( int i = 0; i < 4; ++i )
         {
            m[row][col] += a.m[row][i] * b.m[i][col];
         }
      }
   }

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Matrix& Matrix::mul( const Matrix& a )
{
   Matrix tmpMtx = *this;
   setMul( tmpMtx, a );

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Matrix& Matrix::preMul( const Matrix& a )
{
   Matrix tmpMtx = *this;
   setMul( a, tmpMtx );

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Matrix& Matrix::setMul( const Matrix& a, float t )
{
   for ( int row = 0; row < 4; ++row )
   {
      for ( int col = 0; col < 4; ++col )
      {
         m[row][col] = a.m[row][col] * t;
      }
   }

   return *this;
}

/////////////////////////////////////////////////////////////////

Matrix& Matrix::mul( float t )
{
   for ( int row = 0; row < 4; ++row )
   {
      for ( int col = 0; col < 4; ++col )
      {
         m[row][col] *= t;
      }
   }

   return *this;
}

/////////////////////////////////////////////////////////////////

const Vector& Matrix::sideVec() const
{
   return ( const Vector& )( m[0][0] );
}

///////////////////////////////////////////////////////////////////////////////

const Vector& Matrix::upVec() const
{
   return ( const Vector& )( m[1][0] );
}

///////////////////////////////////////////////////////////////////////////////

const Vector& Matrix::forwardVec() const
{
   return ( const Vector& )( m[2][0] );
}

///////////////////////////////////////////////////////////////////////////////

const Vector& Matrix::position() const
{
   return ( const Vector& )( m[3][0] );
}

///////////////////////////////////////////////////////////////////////////////

const Vector& Matrix::getRow( byte rowIdx ) const
{
   ASSERT_MSG( rowIdx >= 0 && rowIdx <= 3, "Matrix row index out of bounds" );
   return ( const Vector& )( m[rowIdx][0] );
}

///////////////////////////////////////////////////////////////////////////////

void Matrix::setSideVec( const Vector& vec )
{
   ( Vector& )( m[0][0] ) = vec;
}

///////////////////////////////////////////////////////////////////////////////

void Matrix::setUpVec( const Vector& vec )
{
   ( Vector& )( m[1][0] ) = vec;
}

///////////////////////////////////////////////////////////////////////////////

void Matrix::setForwardVec( const Vector& vec )
{
   ( Vector& )( m[2][0] ) = vec;
}

///////////////////////////////////////////////////////////////////////////////

void Matrix::setPosition( const Vector& pos )
{
   ( Vector& )( m[3][0] ) = pos;
   m44 = 1;
}

///////////////////////////////////////////////////////////////////////////////

void Matrix::getScale( Vector& outScaleVec ) const
{
   outScaleVec.setNormalized( m[0][0], m[1][1], m[2][2] );
}

///////////////////////////////////////////////////////////////////////////////

void Matrix::scaleUniform( float scale )
{
   m[0][0] *= scale;
   m[1][1] *= scale;
   m[2][2] *= scale;
}

///////////////////////////////////////////////////////////////////////////////

void Matrix::scale( const Vector& scaleVec )
{
   m[0][0] *= scaleVec.x;
   m[1][1] *= scaleVec.y;
   m[2][2] *= scaleVec.z;
}

///////////////////////////////////////////////////////////////////////////////

Matrix& Matrix::setInverse( const Matrix& rhs )
{
   // implementation of the algorithm taken from here: ftp://download.intel.com/design/PentiumIII/sml/24504301.pdf
   // <simd.todo> implement the simd version using the aforementioned document

   float tmp[12]; // temp array of pairs
   float src[16]; // array of transpose source matrix
   float det; // determinant

   const float* mat = &rhs.m[0][0];

   // transpose matrix
   for ( int i = 0; i < 4; ++i ) 
   {
      src[i] = mat[i*4];
      src[i + 4] = mat[i*4 + 1];
      src[i + 8] = mat[i*4 + 2];
      src[i + 12] = mat[i*4 + 3];
   }

   // calculate pairs for first 8 elements (cofactors)
   tmp[0] = src[10] * src[15];
   tmp[1] = src[11] * src[14];
   tmp[2] = src[9] * src[15];
   tmp[3] = src[11] * src[13];
   tmp[4] = src[9] * src[14];
   tmp[5] = src[10] * src[13];
   tmp[6] = src[8] * src[15];
   tmp[7] = src[11] * src[12];
   tmp[8] = src[8] * src[14];
   tmp[9] = src[10] * src[12];
   tmp[10] = src[8] * src[13];
   tmp[11] = src[9] * src[12];

   // calculate first 8 elements (cofactors)
   float* dst = &m[0][0];
   dst[0] = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
   dst[0] -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];
   dst[1] = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
   dst[1] -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
   dst[2] = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
   dst[2] -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
   dst[3] = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
   dst[3] -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
   dst[4] = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
   dst[4] -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
   dst[5] = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
   dst[5] -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
   dst[6] = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
   dst[6] -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
   dst[7] = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
   dst[7] -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];

   // calculate pairs for second 8 elements (cofactors)
   tmp[0] = src[2]*src[7];
   tmp[1] = src[3]*src[6];
   tmp[2] = src[1]*src[7];
   tmp[3] = src[3]*src[5];
   tmp[4] = src[1]*src[6];
   tmp[5] = src[2]*src[5];
   tmp[6] = src[0]*src[7];
   tmp[7] = src[3]*src[4];
   tmp[8] = src[0]*src[6];
   tmp[9] = src[2]*src[4];
   tmp[10] = src[0]*src[5];
   tmp[11] = src[1]*src[4];

   // calculate second 8 elements (cofactors)
   dst[8] = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
   dst[8] -= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
   dst[9] = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
   dst[9] -= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
   dst[10] = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
   dst[10]-= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
   dst[11] = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
   dst[11]-= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
   dst[12] = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
   dst[12]-= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
   dst[13] = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
   dst[13]-= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
   dst[14] = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
   dst[14]-= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
   dst[15] = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
   dst[15]-= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];

   // calculate determinant
   det=src[0]*dst[0]+src[1]*dst[1]+src[2]*dst[2]+src[3]*dst[3];

   // calculate matrix inverse
   det = 1/det;
   for ( int j = 0; j < 16; ++j )
   {
      dst[j] *= det;
   }

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Matrix& Matrix::invert()
{
   Matrix tmpMtx;
   tmpMtx.setInverse( *this );

   memcpy( &m[0][0], &tmpMtx.m[0][0], sizeof( float ) * 16 );
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Matrix& Matrix::setTransposed( const Matrix& rhs )
{
   for( int row = 0; row < 4; ++row )
   {
      for( int col = 0; col < 4; ++col )
      {
         m[col][row] = rhs.m[row][col];
      }
   }
   
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Matrix& Matrix::transpose()
{
   Matrix tmpMtx;
   tmpMtx.setTransposed( *this );

   memcpy( &m[0][0], &tmpMtx.m[0][0], sizeof( float ) * 16 );
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

Matrix& Matrix::setInverseTranspose( const Matrix& rhs )
{
   Matrix tmpMtx;
   tmpMtx.setInverse( rhs );
   tmpMtx.transpose();

   memcpy( &m[0][0], &tmpMtx.m[0][0], sizeof( float ) * 16 );

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

void Matrix::getVectors( Vector& outSideVec, Vector& outUpVec, Vector& outForwardVec, Vector& outPos ) const
{
   outSideVec.x = m[0][0];
   outSideVec.y = m[0][1];
   outSideVec.z = m[0][2];

   outUpVec.x = m[1][0];
   outUpVec.y = m[1][1];
   outUpVec.z = m[1][2];

   outForwardVec.x = m[2][0];
   outForwardVec.y = m[2][1];
   outForwardVec.z = m[2][2];

   outPos.x = m[3][0];
   outPos.y = m[3][1];
   outPos.z = m[3][2];
}

///////////////////////////////////////////////////////////////////////////////

void Matrix::setRows( const Vector& sideVec, const Vector& upVec, const Vector& forwardVec, const Vector& pos )
{
   m[0][0] = sideVec.x;
   m[0][1] = sideVec.y;
   m[0][2] = sideVec.z;
   m[0][3] = 0.0f;

   m[1][0] = upVec.x;
   m[1][1] = upVec.y;
   m[1][2] = upVec.z;
   m[1][3] = 0.0f;

   m[2][0] = forwardVec.x;
   m[2][1] = forwardVec.y;
   m[2][2] = forwardVec.z;
   m[2][3] = 0.0f;

   m[3][0] = pos.x;
   m[3][1] = pos.y;
   m[3][2] = pos.z;
   m[3][3] = 1.0f;
}

///////////////////////////////////////////////////////////////////////////////

void Matrix::transform( const Vector& rhs, Vector& outVec ) const
{
   float inV[] = {rhs.x, rhs.y, rhs.z, 1};
   for ( int col = 0; col < 4; ++col )
   {
      outVec[col] = 0.0f;
      for ( int row = 0; row < 4; ++row )
      {
         outVec[col] += m[row][col] * inV[row];
      }
   }

   for ( int i = 0; i < 4; ++i )
   {
      outVec[i] /= outVec[3];
   }
}

///////////////////////////////////////////////////////////////////////////////

void Matrix::transformNorm( const Vector& rhs, Vector& outNormal ) const
{
   float inV[] = { rhs.x, rhs.y, rhs.z };
   for ( int col = 0; col < 3; ++col )
   {
      outNormal[col] = 0.0f;
      for ( int row = 0; row < 3; ++row )
      {
         outNormal[col] += m[row][col] * inV[row];
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void Matrix::transform4( const Vector& rhs, Vector& outVec ) const
{
   for ( int col = 0; col < 4; ++col )
   {
      outVec[col] = 0.0f;
      for ( int row = 0; row < 4; ++row )
      {
         outVec[col] += m[row][col] * rhs[row];
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void Matrix::transform( const Plane& inPlane, Plane& outPlane ) const
{
   Matrix tmpInvMtx;
   tmpInvMtx.setInverseTranspose( *this );

   tmpInvMtx.transform4( ( const Vector& )inPlane, ( Vector& )outPlane );
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

OutStream& operator<<( OutStream& serializer, const Matrix& rhs )
{
   for (int row = 0; row < 4; ++row)
   {
      for (int col = 0; col < 4; ++col )
      {
         serializer << rhs.m[row][col];
      }
   }

   return serializer;
}

///////////////////////////////////////////////////////////////////////////////

InStream& operator>>( InStream& serializer, Matrix& rhs )
{
   for (int row = 0; row < 4; ++row)
   {
      for (int col = 0; col < 4; ++col )
      {
         serializer >> rhs.m[row][col];
      }
   }

   return serializer;
}

///////////////////////////////////////////////////////////////////////////////
