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
	setRow( 0, v0);
	setRow( 1, v1);
	setRow( 2, v2);
	setRow( 3, v3);
}

ApiRender_ Matrix operator*( const Matrix& a, const Matrix& b )
{
	Matrix r(a);
	r.postMultiply(b);
	return r;
}
