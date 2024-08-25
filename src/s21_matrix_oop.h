#ifndef S21_MATRIX_OOP_H
#define S21_MATRIX_OOP_H

#include <cmath>
#include <iostream>

#define EPS 1.0e-7
#define FOR(x) for (int i = 0; i < x; i++)
#define FORJ(x, y) FOR(x) for (int j = 0; j < y; j++)
#define FORJK(x, y, z) FORJ(x, y) for (int k = 0; k < z; k++)

class S21Matrix {
 public:
  //=================  CONSTRUCTORS   ======================
  S21Matrix();
  S21Matrix(int rows, int cols);
  S21Matrix(const S21Matrix& other);
  S21Matrix(S21Matrix&& other);
  ~S21Matrix();

  //=================   GET/SET   ======================
  int GetRows() const;
  int GetCols() const;
  void SetRows(int rows);
  void SetCols(int cols);

  //=================   ARITHMETIC   ======================
  bool EqMatrix(const S21Matrix& other) const;
  void SumMatrix(const S21Matrix& other);
  void SubMatrix(const S21Matrix& other);
  void MulNumber(const double num);
  void MulMatrix(const S21Matrix& other);

  //=================   OPERATIONS   ======================
  double Determinant() const;
  S21Matrix Transpose() const;
  S21Matrix CalcComplements() const;
  S21Matrix InverseMatrix();

  //=================   OPERATOR OVERLOAD   ======================
  S21Matrix operator+(const S21Matrix& other);
  S21Matrix operator-(const S21Matrix& other);
  S21Matrix operator*(const S21Matrix& other);
  S21Matrix operator*(const double mul);
  bool operator==(const S21Matrix& other) const;
  S21Matrix& operator=(const S21Matrix& other);
  S21Matrix operator+=(const S21Matrix& other);
  S21Matrix operator-=(const S21Matrix& other);
  S21Matrix operator*=(const S21Matrix& other);
  S21Matrix operator*=(const double mul);
  double& operator()(int row, int col);

  //=================   BASIC METHODS   ======================
  void InitMatrix();
  void ClearMatrix();
  void CopyMatrix(const S21Matrix& other);
  void FillMatrix(S21Matrix& newMatrix, int rows, int cols);

  //=================   SUPPLEMENTARY   ======================
  void CheckSizes(const S21Matrix& other) const;
  void CheckSquare() const;
  void CheckBounds(int row, int col) const;
  void FindMinor(S21Matrix& minor, int row, int col) const;
  void FindComplements(S21Matrix& complements) const;

 private:
  int rows_, cols_;
  double** matrix_;
};

#endif  // S21_MATRIX_OOP_H