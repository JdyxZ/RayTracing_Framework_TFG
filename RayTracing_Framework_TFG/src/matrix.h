#ifndef MATRIX_H
#define MATRIX_H

class Matrix
{
public:
    // Constructors & Destructor
    Matrix(unsigned num_rows, unsigned num_columns, double initial = 0.0);
    ~Matrix() = default;

    // Matrix Operations
    Matrix identity(unsigned size) const;
    Matrix transpose() const;
    double determinant() const;
    Matrix inverse() const;
    double trace() const;
    Matrix power(unsigned exponent) const;

    // Row & Column Operations
    void swap_rows(unsigned row1, unsigned row2);
    void swap_columns(unsigned col1, unsigned col2);
    void scale_row(unsigned row, double scalar);
    void scale_column(unsigned col, double scalar);

    // Operator Overloads
    vector<double>& operator[](unsigned row);
    const vector<double>& operator[](unsigned row) const;
    double operator()(unsigned row, unsigned col) const;

    Matrix operator-() const;
    Matrix& operator+=(const Matrix& M);
    Matrix& operator-=(const Matrix& M);
    Matrix& operator+=(const double scalar);
    Matrix& operator-=(const double scalar);
    Matrix& operator*=(const double scalar);
    Matrix& operator/=(const double scalar);

    // Helper Functions
    void print() const;
    Matrix copy() const;

    // Getters
    unsigned get_num_rows() const;
    unsigned get_num_columns() const;

private:
    unsigned num_rows;
    unsigned num_columns;
    vector<vector<double>> values;
};

class Matrix44 : public Matrix 
{
public:
    Matrix44(double initial) : Matrix(4, 4, initial) { }
};

class Matrix33 : public Matrix
{
public:
    Matrix33(double initial) : Matrix(3, 3, initial) {}
};

class Matrix22 : public Matrix
{
public:
    Matrix22(double initial) : Matrix(2, 2, initial) {}
};

// Non-member operator overloads
inline Matrix operator-(const Matrix& A, const Matrix& B);
inline Matrix operator*(const Matrix& A, const Matrix& B);
inline vector<double> operator*(const Matrix& M, const vector<double>& v);
inline vector<double> operator*(const vector<double>& v, const Matrix& M);
inline Matrix operator+(const Matrix& M, const double scalar);
inline Matrix operator-(const Matrix& M, const double scalar);
inline Matrix operator*(const Matrix& M, const double scalar);
inline Matrix operator/(const Matrix& M, const double scalar);
inline Matrix operator+(const double scalar, const Matrix& M);
inline Matrix operator-(const double scalar, const Matrix& M);
inline Matrix operator*(const double scalar, const Matrix& M);

#endif
