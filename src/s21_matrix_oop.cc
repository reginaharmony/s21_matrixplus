#include "s21_matrix_oop.h"

//=================   CONSTRUCTORS   ======================

S21Matrix::S21Matrix() : rows_{}, cols_{}, matrix_{} {}

S21Matrix::S21Matrix(int rows, int cols) {
  if (rows < 1 || cols < 1) throw std::invalid_argument("Can't be less than 1");
  rows_ = rows, cols_ = cols, InitMatrix();
}

S21Matrix::S21Matrix(const S21Matrix &other)
    : rows_(other.rows_), cols_(other.cols_) {
  if (&other == this) throw std::logic_error("Can't copy into itself");
  CopyMatrix(other);
}

S21Matrix::S21Matrix(S21Matrix &&other) {
  matrix_ = other.matrix_, rows_ = other.rows_, cols_ = other.cols_;
  other.matrix_ = nullptr, other.cols_ = 0, other.rows_ = 0;
}

S21Matrix::~S21Matrix() { ClearMatrix(); }

//=================   GET/SET   ======================

int S21Matrix::GetRows() const { return rows_; }
int S21Matrix::GetCols() const { return cols_; }

void S21Matrix::SetRows(int rows) {
  S21Matrix newMatrix(rows, cols_);
  FillMatrix(newMatrix, (rows < rows_) ? rows : rows_, cols_);
  *this = newMatrix;
}

void S21Matrix::SetCols(int cols) {
  S21Matrix newMatrix(rows_, cols);
  FillMatrix(newMatrix, rows_, (cols < cols_) ? cols : cols_);
  *this = newMatrix;
}

//=================   BASIC METHODS   ======================

void S21Matrix::InitMatrix() {
  matrix_ = new double *[rows_]();
  FOR(rows_) matrix_[i] = new double[cols_]();
}

void S21Matrix::CopyMatrix(const S21Matrix &other) {
  InitMatrix();
  FORJ(rows_, cols_) matrix_[i][j] = other.matrix_[i][j];
}

void S21Matrix::FillMatrix(S21Matrix &newMatrix, int rows, int cols) {
  FORJ(rows, cols) newMatrix.matrix_[i][j] = matrix_[i][j];
}

void S21Matrix::ClearMatrix() {
  if (matrix_) FOR(rows_) delete[] matrix_[i];
  delete[] matrix_, matrix_ = nullptr, rows_ = 0, cols_ = 0;
}

void S21Matrix::CheckSizes(const S21Matrix &other) const {
  if (rows_ != other.rows_ || cols_ != other.cols_)
    throw std::invalid_argument("Unequal size of matrices");
}

//=================   ARITHMETIC   ======================

bool S21Matrix::EqMatrix(const S21Matrix &other) const {
  if (rows_ != other.rows_ || cols_ != other.cols_) return false;
  FORJ(rows_, cols_)
  if (fabs(matrix_[i][j] - other.matrix_[i][j]) >= EPS) return false;

  return true;
}

#define SUMSUB(s)    \
  CheckSizes(other); \
  FORJ(rows_, cols_) matrix_[i][j] s other.matrix_[i][j];

void S21Matrix::SumMatrix(const S21Matrix &other) { SUMSUB(+=) }
void S21Matrix::SubMatrix(const S21Matrix &other) { SUMSUB(-=) }

void S21Matrix::MulNumber(const double num) {
  FORJ(rows_, cols_) matrix_[i][j] *= num;
}

void S21Matrix::MulMatrix(const S21Matrix &other) {
  if (cols_ != other.rows_) throw std::invalid_argument("Invalid sizes");

  S21Matrix result(rows_, other.cols_);
  FORJK(rows_, cols_, other.cols_)
  result(i, k) += matrix_[i][j] * other.matrix_[j][k];
  *this = result;
}

//=================   OPERATIONS   ======================

S21Matrix S21Matrix::Transpose() const {
  S21Matrix result(cols_, rows_);
  FORJ(result.rows_, result.cols_) result.matrix_[i][j] = matrix_[j][i];
  return result;
}

S21Matrix S21Matrix::CalcComplements() const {
  CheckSquare();
  if (rows_ == 1) throw std::logic_error("Size can not be 1");
  S21Matrix result(rows_, cols_);
  FindComplements(result);
  return result;
}

double S21Matrix::Determinant() const {
  CheckSquare();
  if (rows_ == 1) return matrix_[0][0];
  double result = 0;
  FOR(cols_) {
    S21Matrix minor(rows_ - 1, cols_ - 1);
    FindMinor(minor, 0, i);
    result += matrix_[0][i] * pow(-1, i) * minor.Determinant();
    minor.ClearMatrix();
  }
  return result;
}

S21Matrix S21Matrix::InverseMatrix() {
  if (fabs(Determinant()) <= EPS)
    throw std::logic_error("Determinant cannot be 0");
  return CalcComplements().Transpose() *= (1.0 / Determinant());
}

//=================   OPERATOR OVERLOAD   ======================

#define OP S21Matrix S21Matrix::operator

#define OPCOMMON(METHOD)   \
  S21Matrix result(*this); \
  result.METHOD;           \
  return result

OP + (const S21Matrix &other) { OPCOMMON(SumMatrix(other)); }
OP - (const S21Matrix &other) { OPCOMMON(SubMatrix(other)); }
OP *(const S21Matrix &other) { OPCOMMON(MulMatrix(other)); }
OP *(const double mul) { OPCOMMON(MulNumber(mul)); }

OP += (const S21Matrix &other) { return SumMatrix(other), *this; }
OP -= (const S21Matrix &other) { return SubMatrix(other), *this; }
OP *= (const S21Matrix &other) { return MulMatrix(other), *this; }
OP *= (const double mul) { return MulNumber(mul), *this; }

S21Matrix &S21Matrix::operator=(const S21Matrix &other) {
  if (this == &other) return *this;
  ClearMatrix(), rows_ = other.rows_, cols_ = other.cols_, CopyMatrix(other);
  return *this;
}

bool S21Matrix::operator==(const S21Matrix &other) const {
  return EqMatrix(other);
}

double &S21Matrix::operator()(int row, int col) {
  return CheckBounds(row, col), matrix_[row][col];
}

//=================   SUPPLEMENTARY   ======================

void S21Matrix::CheckSquare() const {
  if (rows_ != cols_) throw std::logic_error("Matrix is not square");
}

void S21Matrix::CheckBounds(int row, int col) const {
  if (row < 0 || col < 0)
    throw std::out_of_range("Less than 0 exception");
  else if (row >= rows_ || col >= cols_)
    throw std::out_of_range("Out of bounds exception");
}

void S21Matrix::FindMinor(S21Matrix &minor, int row, int col) const {
  int minorRow = 0, minorCol = 0;
  FORJ(rows_, cols_)
  if (i != row && j != col) {
    minor.matrix_[minorRow][minorCol++] = matrix_[i][j];
    if (minorCol == cols_ - 1) minorCol = 0, ++minorRow;
  }
}

void S21Matrix::FindComplements(S21Matrix &complements) const {
  FORJ(rows_, cols_) {
    S21Matrix minor(rows_ - 1, cols_ - 1);
    FindMinor(minor, i, j);
    complements.matrix_[i][j] = pow(-1, (i + j)) * minor.Determinant();
    minor.ClearMatrix();
  }
}