#ifndef MATRIXSOLVER_H
#define MATRIXSOLVER_H

#include <QObject>
#include <QVariantList>

class MatrixSolver : public QObject {
	Q_OBJECT

public:
	explicit MatrixSolver(QObject *parent = nullptr);

	Q_INVOKABLE double calculateDeterminant(QVariantList flatMatrix, int n);
	Q_INVOKABLE QVariantList findInverse(QVariantList flatMatrix, int n);
	Q_INVOKABLE QVariantList solveSystem(QVariantList flatA, QVariantList flatB, int n);
};

#endif // MATRIXSOLVER_H