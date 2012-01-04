#include "matrix.h"


const Matrix Matrix::Identity(	Vector4( 1.0f, 0.0f, 0.0f, 0.0f ),
							  Vector4( 0.0f, 1.0f, 0.0f, 0.0f ),
							  Vector4( 0.0f, 0.0f, 1.0f, 0.0f ),
							  Vector4( 0.0f, 0.0f, 0.0f, 1.0f ) );
Matrix::Matrix()
{
	setZero();
}

Matrix::Matrix( const Vector4& v0, const Vector4& v1, const Vector4& v2, const Vector4& v3 )
{
	row( 0, v0);
	row( 1, v1);
	row( 2, v2);
	row( 3, v3);
}
