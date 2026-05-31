# AUSMT Linear Algebra Project

A lightweight, high-performance matrix operations calculator built using **C++** for the math backend and **Qt Quick (QML)** for a mobile-responsive UI. 

**Author:** Rahman Taleshi  
**Course:** Linear Algebra — AUSMT  

---

## 📱 Features & Core Functionality

This application handles core linear algebra operations dynamically for matrix sizes ($n \times n$) ranging from 2x2 to 5x5:
1. **Solve Linear Systems ($Ax = B$):** Computes exact solutions using Gauss-Jordan elimination.
2. **Matrix Inversion ($A^{-1}$):** Finds the inverse of a square matrix via row operations on an augmented matrix, with automated invertibility checks.
3. **Determinant Calculation ($\det(A)$):** Converts the matrix into an upper triangular form to efficiently calculate the determinant.

---

## 🛠️ Algorithms Used

The backend completely avoids heavy external libraries and processes all math natively using **Elementary Row Operations**:

### 1. Determinant Calculation
* **Method:** Upper Triangular Matrix Conversion.
* **How it works:** The algorithm loops through columns using partial pivoting (swapping rows to put the largest absolute value on top to prevent division by zero). Each row swap flips the sign of the determinant ($\times -1$). It then uses row replacement ($R_j \leftarrow R_j - c \cdot R_i$) to clear out all numbers below the main diagonal. The final determinant is computed by multiplying all elements along the main diagonal.

### 2. Matrix Inverse
* **Method:** Gauss-Jordan Elimination on an Augmented Matrix $[A \mid I]$.
* **How it works:** First, it verifies invertibility by ensuring $\det(A) \neq 0$. It then augments the original matrix $A$ with an Identity Matrix $I$ of the same size. By scaling rows to 1 and eliminating elements both above and below the pivots, it transforms the left side into the Identity Matrix. The right side naturally transforms into $A^{-1}$, which is then parsed back to the UI.

### 3. Solving Systems of Equations
* **Method:** Gauss-Jordan Elimination on an Augmented Matrix $[A \mid B]$.
* **How it works:** Augments the coefficient matrix $A$ with the constant vector $B$ (forming an $n \times (n+1)$ grid). It runs full elimination to reduce the left side to a reduced row echelon form (Identity Matrix). Once complete, the final column automatically contains the exact solution vector $x$.

---

## 📂 Project Structure

```text
MatrixMaster/
├── CMakeLists.txt          # Build configuration (or .pro file for qmake)
├── main.cpp                # App entry point & registers C++ backend to QML
├── matrixsolver.h          # Header file containing the class interface
├── matrixsolver.cpp        # Backend implementation of the algorithms
└── main.qml                # Mobile-friendly UI layout and frontend logic
```
---
## 📸 Screeshots
<img width="452" height="977" alt="image" src="https://github.com/user-attachments/assets/8fecd046-92f4-4037-8fe9-e8a81aedbbe3" />
<img width="452" height="977" alt="image" src="https://github.com/user-attachments/assets/7ff4f3ca-0963-473b-880a-3ed6310121a2" />
---
## 🚀 How to Run
- Open the project in Qt Creator.
- Ensure the Qt Quick / Qml modules are installed.
- Build and Run using your preferred kit (Desktop or Mobile).
