#include "matrixsolver.h"
#include <vector>
#include <cmath>

MatrixSolver::MatrixSolver(QObject *parent) : QObject(parent) {}

// Section 1: Calculate Determinant using Upper Triangular Matrix
double MatrixSolver::calculateDeterminant(QVariantList flatMatrix, int n) {
	// Convert 1D QVariantList from QML into a 2D C++ vector
	std::vector<std::vector<double>> mat(n, std::vector<double>(n));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			mat[i][j] = flatMatrix[i * n + j].toDouble();
		}
	}

	double det = 1.0;
	for (int i = 0; i < n; ++i) {
		// Partial pivoting: find max value in column to avoid division by zero
		int pivot = i;
		for (int j = i + 1; j < n; ++j) {
			if (std::abs(mat[j][i]) > std::abs(mat[pivot][i])) pivot = j;
		}

		if (std::abs(mat[pivot][i]) < 1e-9) return 0.0;

			   // Swap rows if needed and flip determinant sign
		if (pivot != i) {
			std::swap(mat[i], mat[pivot]);
			det *= -1.0;
		}

		det *= mat[i][i];

			   // Row elimination to zero out elements below the pivot
		for (int j = i + 1; j < n; ++j) {
			double factor = mat[j][i] / mat[i][i];
			for (int k = i; k < n; ++k) {
				mat[j][k] -= factor * mat[i][k];
			}
		}
	}
	return det;
}

// Section 2: Find Matrix Inverse using Gauss-Jordan on [A | I]
QVariantList MatrixSolver::findInverse(QVariantList flatMatrix, int n) {
	// Matrix must have a non-zero determinant to be invertible
	if (std::abs(calculateDeterminant(flatMatrix, n)) < 1e-9) {
		return QVariantList();
	}

		   // Create an augmented matrix [A | I] of size n x 2n
	std::vector<std::vector<double>> mat(n, std::vector<double>(2 * n, 0.0));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) mat[i][j] = flatMatrix[i * n + j].toDouble();
		mat[i][n + i] = 1.0;
	}

		   // Full Gauss-Jordan elimination loop
	for (int i = 0; i < n; ++i) {
		int pivot = i;
		for (int j = i + 1; j < n; ++j) {
			if (std::abs(mat[j][i]) > std::abs(mat[pivot][i])) pivot = j;
		}
		if (pivot != i) std::swap(mat[i], mat[pivot]);

			   // Normalize the pivot row so the diagonal becomes 1
		double pivotVal = mat[i][i];
		for (int k = 0; k < 2 * n; ++k) mat[i][k] /= pivotVal;

			   // Eliminate elements both above and below the main diagonal
		for (int j = 0; j < n; ++j) {
			if (j != i) {
				double factor = mat[j][i];
				for (int k = 0; k < 2 * n; ++k) mat[j][k] -= factor * mat[i][k];
			}
		}
	}

		   // Extract the right side (the inverted matrix) and flatten it for QML
	QVariantList result;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) result.append(mat[i][n + j]);
	}
	return result;
}

// Section 3: Solve System Ax = B using Gauss-Jordan on [A | B]
QVariantList MatrixSolver::solveSystem(QVariantList flatA, QVariantList flatB, int n) {
	// Create an augmented matrix [A | B] of size n x (n + 1)
	std::vector<std::vector<double>> mat(n, std::vector<double>(n + 1, 0.0));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) mat[i][j] = flatA[i * n + j].toDouble();
		mat[i][n] = flatB[i].toDouble();
	}

		   // Standard Gauss-Jordan loop to reduce left side to Identity matrix
	for (int i = 0; i < n; ++i) {
		int pivot = i;
		for (int j = i + 1; j < n; ++j) {
			if (std::abs(mat[j][i]) > std::abs(mat[pivot][i])) pivot = j;
		}
		if (std::abs(mat[pivot][i]) < 1e-9) return QVariantList();
		if (pivot != i) std::swap(mat[i], mat[pivot]);

		double pivotVal = mat[i][i];
		for (int k = 0; k < n + 1; ++k) mat[i][k] /= pivotVal;

		for (int j = 0; j < n; ++j) {
			if (j != i) {
				double factor = mat[j][i];
				for (int k = 0; k < n + 1; ++k) mat[j][k] -= factor * mat[i][k];
			}
		}
	}

		   // The last column now contains the solution values for the vector x
	QVariantList result;
	for (int i = 0; i < n; ++i) result.append(mat[i][n]);
	return result;
}