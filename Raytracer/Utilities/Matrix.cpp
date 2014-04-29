#include "Matrix.h"

Matrix::Matrix() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j)
				m[i][j] = 1.0;
			else
				m[i][j] = 0.0;
		}
	}
}

Matrix::Matrix (const Matrix& matrix) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = matrix.m[i][j];
		}
	}
}

Matrix::~Matrix()
{}

Matrix& Matrix::operator= (const Matrix& rhs) {
	if (this == &rhs)
		return *this;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = rhs.m[i][j];
		}
	}
	return *this;
}

Matrix Matrix::operator* (const Matrix& matrix) const {
	Matrix result;

	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			float sum = 0.0;
			for (int k = 0; k < 4; k++) {
				sum += m[i][k] * matrix.m[k][j];
			}
			result.m[i][j] = sum;
		}
	}
	return result;
}

Matrix Matrix::operator/ (const float d) { 
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = m[i][j] / d;
		}
	}
	return *this;
}

void Matrix::make_identity() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j)
				m[i][j] = 1.0;
			else 
				m[i][j] = 0.0;
		}
	}
}