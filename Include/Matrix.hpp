#ifndef ADVENTURE_MATRIX_HPP_
#define ADVENTURE_MATRIX_HPP_

namespace Adventure
{
	struct Vector3;
	
	struct Matrix
	{
		private:
			float matrix[16];
		
		public:
			Matrix();
			Matrix(const float& other[16]);
			Matrix(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);
			Matrix(const Matrix& other);
			
			// Getter and setter methods
			inline float Get(int row, int column) const
			{
				return matrix[(column << 2) + row];
			}
			
			void Set(int row, int column, float value)
			{
				matrix[(column << 2) + row] = value;
			}
			
			// Mathematical operations (for now, only multiplication is supported)
			Matrix& operator *=(const Matrix& other);
			
			// Determinant
			float CalculateDeterminant();
			
			// Transformation
			static Matrix Rotate(const Vector3& axis, float angle);
			static Matrix Translate(const Vector3& translation);
			static Matrix Scale(const Vector3& scale);
			
			// Inverse
			Matrix Invert(const Matrix& matrix);
	};
	
	Matrix operator *(const Matrix& left, const Matrix& right);
}

#endif
