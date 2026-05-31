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
			// Formula (i * n + j) maps flat 1D QML array to 2D coordinates
			mat[i][j] = flatMatrix[i * n + j].toDouble();
		}
	}

	double det = 1.0;
	for (int i = 0; i < n; ++i) {
		// Partial pivoting: find max value in column to avoid dividing by zero
		int pivot = i;
		for (int j = i + 1; j < n; ++j) {
			if (std::abs(mat[j][i]) > std::abs(mat[pivot][i])) pivot = j;
		}

			   // If the max pivot element is basically zero, matrix is singular (det = 0)
		if (std::abs(mat[pivot][i]) < 1e-9) return 0.0;

			   // Swap rows if needed and flip determinant sign
		if (pivot != i) {
			std::swap(mat[i], mat[pivot]);
			det *= -1.0;
		}

			   // Multiply the running determinant by the diagonal element
		det *= mat[i][i];

			   // Row elimination: zero out all elements below the current pivot
		for (int j = i + 1; j < n; ++j) {
			double factor = mat[j][i] / mat[i][i];
			for (int k = i; k < n; ++k) {
				mat[j][k] -= factor * mat[i][k]; // R_j = R_j - factor * R_i
			}
		}
	}
	return det;
}

// Section 2: Find Matrix Inverse using Gauss-Jordan on [A | I]
QVariantList MatrixSolver::findInverse(QVariantList flatMatrix, int n) {
	// Condition check: Matrix must have a non-zero determinant to be invertible
	if (std::abs(calculateDeterminant(flatMatrix, n)) < 1e-9) {
		return QVariantList(); // Return empty if not invertible
	}

		   // Create an augmented matrix [A | I] of size n x 2n (initialized to 0)
	std::vector<std::vector<double>> mat(n, std::vector<double>(2 * n, 0.0));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			// Map flat QML data into the left half (Matrix A)
			mat[i][j] = flatMatrix[i * n + j].toDouble();
		}
		// Place 1.0 on the diagonal of the right half (Identity Matrix I)
		mat[i][n + i] = 1.0;
	}

		   // Full Gauss-Jordan elimination loop
	for (int i = 0; i < n; ++i) {
		int pivot = i;
		for (int j = i + 1; j < n; ++j) {
			if (std::abs(mat[j][i]) > std::abs(mat[pivot][i])) pivot = j;
		}
		if (pivot != i) std::swap(mat[i], mat[pivot]);

			   // Normalize the pivot row so the main diagonal element becomes 1
		double pivotVal = mat[i][i];
		for (int k = 0; k < 2 * n; ++k) mat[i][k] /= pivotVal;

			   // Eliminate elements both above AND below the main diagonal
		for (int j = 0; j < n; ++j) {
			if (j != i) {
				double factor = mat[j][i];
				for (int k = 0; k < 2 * n; ++k) {
					mat[j][k] -= factor * mat[i][k];
				}
			}
		}
	}

		   // Extract the right side (which has now transformed into A^-1) and flatten for QML
	QVariantList result;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			result.append(mat[i][n + j]);
		}
	}
	return result;
}

// Section 3: Solve System Ax = B using Gauss-Jordan on [A | B]
QVariantList MatrixSolver::solveSystem(QVariantList flatA, QVariantList flatB, int n) {
	// Create an augmented matrix [A | B] of size n x (n + 1)
	std::vector<std::vector<double>> mat(n, std::vector<double>(n + 1, 0.0));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			// Fill left columns with coefficients from Matrix A
			mat[i][j] = flatA[i * n + j].toDouble();
		}
		// Fill the very last column with the constants from Vector B
		mat[i][n] = flatB[i].toDouble();
	}

		   // Gauss-Jordan elimination to reduce left side to Identity matrix
	for (int i = 0; i < n; ++i) {
		int pivot = i;
		for (int j = i + 1; j < n; ++j) {
			if (std::abs(mat[j][i]) > std::abs(mat[pivot][i])) pivot = j;
		}
		// If the pivot value is 0, the system does not have a unique solution
		if (std::abs(mat[pivot][i]) < 1e-9) return QVariantList();
		if (pivot != i) std::swap(mat[i], mat[pivot]);

		double pivotVal = mat[i][i];
		for (int k = 0; k < n + 1; ++k) mat[i][k] /= pivotVal;

		for (int j = 0; j < n; ++j) {
			if (j != i) {
				double factor = mat[j][i];
				for (int k = 0; k < n + 1; ++k) {
					mat[j][k] -= factor * mat[i][k];
				}
			}
		}
	}

		   // Left side is now Identity, meaning the last column contains the isolated answers for vector x
	QVariantList result;
	for (int i = 0; i < n; ++i) {
		result.append(mat[i][n]);
	}
	return result;
}
