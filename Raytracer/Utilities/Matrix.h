#ifndef __MATRIX__
#define __MATRIX__

class Matrix {
	public: 
		float m[4][4];

		Matrix();
		Matrix(const Matrix& matrix);
		~Matrix();

		Matrix& operator= (const Matrix& rhs);
		Matrix operator* (const Matrix& matrix) const;
		Matrix operator/ (const float d);

		void make_identity();
};

#endif