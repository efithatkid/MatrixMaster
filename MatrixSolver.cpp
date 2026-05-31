#include "matrixsolver.h"
#include <vector>
#include <cmath>

MatrixSolver::MatrixSolver(QObject *parent) : QObject(parent) {}

// Section 1: Calculate Determinant using Upper Triangular Matrix
double MatrixSolver::calculateDeterminant(QVariantList flatMatrix, int n) {
	std::vector<std::vector<double>> mat(n, std::vector<double>(n));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			mat[i][j] = flatMatrix[i * n + j].toDouble();
		}
	}

	double det = 1.0;
	for (int i = 0; i < n; ++i) {
		int pivot = i;
		for (int j = i + 1; j < n; ++j) {
			if (std::abs(mat[j][i]) > std::abs(mat[pivot][i])) pivot = j;
		}

		if (std::abs(mat[pivot][i]) < 1e-9) return 0.0;

		if (pivot != i) {
			std::swap(mat[i], mat[pivot]);
			det *= -1.0;
		}

		det *= mat[i][i];

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
	if (std::abs(calculateDeterminant(flatMatrix, n)) < 1e-9) {
		return QVariantList();
	}

	std::vector<std::vector<double>> mat(n, std::vector<double>(2 * n, 0.0));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			mat[i][j] = flatMatrix[i * n + j].toDouble();
		}
		mat[i][n + i] = 1.0;
	}

	for (int i = 0; i < n; ++i) {
		int pivot = i;
		for (int j = i + 1; j < n; ++j) {
			if (std::abs(mat[j][i]) > std::abs(mat[pivot][i])) pivot = j;
		}
		if (pivot != i) std::swap(mat[i], mat[pivot]);

		double pivotVal = mat[i][i];
		for (int k = 0; k < 2 * n; ++k) mat[i][k] /= pivotVal;

		for (int j = 0; j < n; ++j) {
			if (j != i) {
				double factor = mat[j][i];
				for (int k = 0; k < 2 * n; ++k) {
					mat[j][k] -= factor * mat[i][k];
				}
			}
		}
	}

		   // Extracting inverse matrix and cleaning up floating-point precision artifacts
	QVariantList result;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			double val = mat[i][n + j];
			if (std::abs(val - std::round(val)) < 1e-9) {
				val = std::round(val);
			}
			result.append(val);
		}
	}
	return result;
}

// Section 3: Solve System Ax = B using Gauss-Jordan on [A | B]
QVariantList MatrixSolver::solveSystem(QVariantList flatA, QVariantList flatB, int n) {
	std::vector<std::vector<double>> mat(n, std::vector<double>(n + 1, 0.0));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			mat[i][j] = flatA[i * n + j].toDouble();
		}
		mat[i][n] = flatB[i].toDouble();
	}

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
				for (int k = 0; k < n + 1; ++k) {
					mat[j][k] -= factor * mat[i][k];
				}
			}
		}
	}

		   // Extracting results vector and cleaning up floating-point precision artifacts
	QVariantList result;
	for (int i = 0; i < n; ++i) {
		double val = mat[i][n];
		if (std::abs(val - std::round(val)) < 1e-9) {
			val = std::round(val);
		}
		result.append(val);
	}
	return result;
}
