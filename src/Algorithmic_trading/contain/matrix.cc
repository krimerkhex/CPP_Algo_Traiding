#include "matrix.h"

s21::Matrix::Matrix() {
  row_ = 0;
  col_ = 0;
  matrix_ = nullptr;
}

s21::Matrix::Matrix(const Matrix& other) {
  row_ = other.row_;
  col_ = other.col_;
  matrix_ = newData();
  for (unsigned int i = 0; i < row_; i++) {
    for (unsigned int j = 0; j < col_; j++) {
      matrix_[i][j] = other.matrix_[i][j];
    }
  }
}

s21::Matrix::Matrix(const Matrix&& other) {
  row_ = other.row_;
  col_ = other.col_;
  matrix_ = other.matrix_;
}

s21::Matrix::Matrix(Matrix&& other) {
  row_ = other.row_;
  col_ = other.col_;
  matrix_ = other.matrix_;
  other.row_ = 0;
  other.col_ = 0;
  other.matrix_ = nullptr;
}

s21::Matrix::Matrix(const unsigned int row, const unsigned int col)
    : row_(row), col_(col), matrix_(newData()) {}

s21::Matrix::Matrix(const unsigned int col)
    : row_(1), col_(col), matrix_(newData()) {}

s21::Matrix::~Matrix() { deleteData(); }

s21::Matrix& s21::Matrix::operator=(const Matrix& other) {
  if (this != &other) {
    deleteData();
    copy(other);
  }
  return *this;
}

s21::Matrix& s21::Matrix::operator=(Matrix&& other) {
  if (this != &other) {
    move(other);
  }
  other.row_ = 0;
  other.col_ = 0;
  other.matrix_ = nullptr;
  return *this;
}

s21::Matrix& s21::Matrix::operator=(const Matrix&& other) {
  if (this != &other) {
    move(other);
  }
  return *this;
}

void s21::Matrix::copy(const Matrix& other) {
  row_ = other.row_;
  col_ = other.col_;
  matrix_ = newData();
  for (unsigned int i = 0; i < row_; i++) {
    for (unsigned int j = 0; j < col_; j++) {
      matrix_[i][j] = other.matrix_[i][j];
    }
  }
}

void s21::Matrix::move(const Matrix& other) {
  deleteData();
  row_ = other.row_;
  col_ = other.col_;
  matrix_ = other.matrix_;
}

s21::Matrix& s21::Matrix::operator=(const double num) {
  for (unsigned int i = 0; i < row_; i++) {
    for (unsigned int j = 0; j < col_; j++) {
      matrix_[i][j] = num;
    }
  }
  return *this;
}

bool s21::Matrix::operator==(const Matrix& other) const {
  bool res = false;
  if (row_ != other.row_ || col_ != other.col_) {
    res = !(std::memcmp(matrix_, other.matrix_, this->row_ * this->col_));
  }
  return res;
}

bool s21::Matrix::operator!=(const Matrix& other) const {
  return !(*this == other);
}

s21::Matrix s21::Matrix::operator+(const Matrix& other) const {
  checkEquaLe(other);
  Matrix res(row_, col_);
  for (unsigned int i = 0; i < row_; i++) {
    for (unsigned int j = 0; j < col_; j++) {
      res.matrix_[i][j] = matrix_[i][j] + other.matrix_[i][j];
    }
  }
  return res;
}

s21::Matrix s21::Matrix::operator+(const double num) const {
  Matrix res(row_, col_);
  for (unsigned int i = 0; i < row_; i++) {
    for (unsigned int j = 0; j < col_; j++) {
      res.matrix_[i][j] = matrix_[i][j] + num;
    }
  }
  return res;
}

s21::Matrix s21::Matrix::operator-(const Matrix& other) const {
  checkEquaLe(other);
  Matrix res(row_, col_);
  for (unsigned int i = 0; i < row_; i++) {
    for (unsigned int j = 0; j < col_; j++) {
      res.matrix_[i][j] = matrix_[i][j] - other.matrix_[i][j];
    }
  }
  return res;
}

s21::Matrix s21::Matrix::operator-(const double num) const {
  Matrix res(row_, col_);
  for (unsigned int i = 0; i < row_; i++) {
    for (unsigned int j = 0; j < col_; j++) {
      res.matrix_[i][j] = matrix_[i][j] - num;
    }
  }
  return res;
}

s21::Matrix s21::Matrix::operator*(const Matrix& other) const {
  checkMulti(other);
  Matrix res(row_, other.col_);
  for (unsigned int i = 0; i < row_; i++) {
    for (unsigned int j = 0; j < other.col_; j++) {
      for (unsigned int k = 0; k < col_; k++) {
        res.matrix_[i][j] += matrix_[i][k] * other.matrix_[k][j];
      }
    }
  }
  return res;
}

s21::Matrix s21::Matrix::operator*(const double num) const {
  Matrix res(row_, col_);
  for (unsigned int i = 0; i < row_; i++) {
    for (unsigned int j = 0; j < col_; j++) {
      res.matrix_[i][j] = matrix_[i][j] * num;
    }
  }
  return res;
}

void s21::Matrix::operator+=(const double num) {
  for (unsigned int i = 0; i < row_; i++) {
    for (unsigned int j = 0; j < col_; j++) {
      matrix_[i][j] += num;
    }
  }
}

void s21::Matrix::operator+=(const Matrix& other) {
  checkEquaLe(other);
  for (unsigned int i = 0; i < row_; i++) {
    for (unsigned int j = 0; j < col_; j++) {
      matrix_[i][j] += other.matrix_[i][j];
    }
  }
}

void s21::Matrix::operator-=(const Matrix& other) {
  checkEquaLe(other);
  for (unsigned int i = 0; i < row_; i++) {
    for (unsigned int j = 0; j < col_; j++) {
      matrix_[i][j] -= other.matrix_[i][j];
    }
  }
}

void s21::Matrix::operator-=(const double num) {
  for (unsigned int i = 0; i < row_; i++) {
    for (unsigned int j = 0; j < col_; j++) {
      matrix_[i][j] -= num;
    }
  }
}

void s21::Matrix::operator*=(const double num) {
  for (unsigned int i = 0; i < row_; i++) {
    for (unsigned int j = 0; j < col_; j++) {
      matrix_[i][j] *= num;
    }
  }
}

void s21::Matrix::operator*=(const Matrix& other) {
  checkMulti(other);
  Matrix res(row_, other.col_);
  for (unsigned int i = 0; i < row_; i++) {
    for (unsigned int j = 0; j < other.col_; j++) {
      for (unsigned int k = 0; k < col_; k++) {
        res.matrix_[i][j] += matrix_[i][k] * other.matrix_[k][j];
      }
    }
  }
  *this = res;
}

double** s21::Matrix::newData() {
  double** matrix = new double*[row_];
  for (unsigned int i = 0; i < row_; i++) {
    matrix[i] = new double[col_]();
  }
  return matrix;
}

void s21::Matrix::deleteData() {
  if (col_ == 0 || row_ == 0) {
    col_ = 0;
    row_ = 0;
    matrix_ = nullptr;
  } else {
    for (unsigned int i = 0; i < row_; i++) {
      delete[] matrix_[i];
    }
    delete[] matrix_;
    col_ = 0;
    row_ = 0;
  }
}

void s21::Matrix::reSize(const unsigned int row, const unsigned int col) {
  deleteData();
  row_ = row;
  col_ = col;
  matrix_ = newData();
}

void s21::Matrix::print() const {
  for (unsigned int i = 0; i < row_; i++) {
    for (unsigned int j = 0; j < col_; j++) {
      std::cout << matrix_[i][j] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

double s21::Matrix::getMaxVal() {
  double max = matrix_[0][0];
  for (unsigned int i = 0; i < row_; i++) {
    for (unsigned int j = 0; j < col_; j++) {
      if (matrix_[i][j] > max) {
        max = matrix_[i][j];
      }
    }
  }
  return max;
}

double s21::Matrix::getMinVal() {
  double min = matrix_[0][0];
  for (unsigned int i = 0; i < row_; i++) {
    for (unsigned int j = 0; j < col_; j++) {
      if (matrix_[i][j] < min) {
        min = matrix_[i][j];
      }
    }
  }
  return min;
}

void s21::Matrix::checkEquaLe(const Matrix& other) const {
  if (row_ != other.row_ || col_ != other.col_) {
    throw std::invalid_argument("Matrices have different sizes");
  }
}

void s21::Matrix::checkMulti(const Matrix& other) const {
  if (col_ != other.row_) {
    throw std::invalid_argument("Matrices have different sizes");
  }
}

double& s21::Matrix::operator()(const unsigned int row,
                                const unsigned int col) {
  return matrix_[row][col];
}
double* s21::Matrix::operator()(const unsigned int row) { return matrix_[row]; }

unsigned int s21::Matrix::getRow() const { return row_; }
unsigned int s21::Matrix::getCol() const { return col_; }
unsigned int s21::Matrix::getSize() const { return row_ * col_; }
