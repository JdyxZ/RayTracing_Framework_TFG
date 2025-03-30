#pragma once

// Headers
#include "core.hpp"
#include "vec3.hpp"
#include "logger.hpp"

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
    Matrix44(double initial);
};

class Matrix33 : public Matrix
{
public:
    Matrix33(double initial);
};

class Matrix22 : public Matrix
{
public:
    Matrix22(double initial);
};

// ************ Non-member operator overloads ************ //

// Matrix Operators
inline Matrix operator+(const Matrix& A, const Matrix& B)
{
    if (A.get_num_rows() != B.get_num_rows() || A.get_num_columns() != B.get_num_columns())
    {
        string error = Logger::error("MATRIX", "A:rows != B:rows or A:cols != B:cols");
        throw std::invalid_argument(error);
    }

    Matrix result(A.get_num_rows(), A.get_num_columns());

    for (unsigned i = 0; i < A.get_num_rows(); i++)
    {
        for (unsigned j = 0; j < A.get_num_columns(); j++)
        {
            result[i][j] = A[i][j] + B[i][j];
        }
    }
    return result;
}

inline Matrix operator-(const Matrix& A, const Matrix& B)
{
    if (A.get_num_rows() != B.get_num_rows() || A.get_num_columns() != B.get_num_columns())
    {
        string error = Logger::error("MATRIX", "A:rows != B:rows or A:cols != B:cols");
        throw std::invalid_argument(error);
    }

    Matrix result(A.get_num_rows(), A.get_num_columns());

    for (unsigned i = 0; i < A.get_num_rows(); i++)
    {
        for (unsigned j = 0; j < A.get_num_columns(); j++)
        {
            result[i][j] = A[i][j] - B[i][j];
        }
    }

    return result;
}

inline Matrix operator*(const Matrix& A, const Matrix& B)
{
    if (A.get_num_columns() != B.get_num_rows())
    {
        string error = Logger::error("MATRIX", "A:cols != B:rows");
        throw std::invalid_argument(error);
    }

    Matrix result(A.get_num_rows(), B.get_num_columns(), 0.0);

    for (unsigned i = 0; i < A.get_num_rows(); i++)
    {
        for (unsigned j = 0; j < B.get_num_columns(); j++)
        {
            double acc = 0.0;

            for (unsigned k = 0; k < A.get_num_columns(); k++)
            {
                acc += A[i][k] * B[k][j];
            }

            result[i][j] = acc;
        }
    }
    return result;
}

// Vector operators
inline vector<double> operator*(const Matrix& M, const vector<double>& v)
{
    if (M.get_num_columns() != v.size())
    {
        string error = Logger::error("MATRIX", "Number of matrix columns must match vector size.");
        throw std::invalid_argument(error);
    }

    vector<double> result(M.get_num_rows(), 0.0);

    for (unsigned i = 0; i < M.get_num_rows(); i++)
    {
        for (unsigned j = 0; j < M.get_num_columns(); j++)
        {
            result[i] += M[i][j] * v[j];
        }
    }

    return result;
}

inline vector<double> operator*(const vector<double>& v, const Matrix& M)
{
    if (v.size() != M.get_num_rows())
    {
        string error = Logger::error("MATRIX", "Number of vector elements must match matrix row count.");
        throw std::invalid_argument(error);
    }

    vector<double> result(M.get_num_columns(), 0.0);

    for (unsigned j = 0; j < M.get_num_columns(); j++)
    {
        for (unsigned i = 0; i < M.get_num_rows(); i++)
        {
            result[j] += v[i] * M[i][j];
        }
    }

    return result;
}

inline vector<double> operator*(const Matrix& M, const vec3& v)
{
    if (M.get_num_columns() != 3)
    {
        string error = Logger::error("MATRIX", "Matrix columns must be 3 for multiplication with vec3");
        throw std::invalid_argument(error);
    }

    vector<double> result(M.get_num_rows(), 0.0);

    for (unsigned i = 0; i < M.get_num_rows(); ++i)
    {
        result[i] = 0.0;
        for (unsigned j = 0; j < M.get_num_columns(); ++j)
        {
            result[i] += M[i][j] * v[j];
        }
    }

    return result;
}

inline vector<double> operator*(const vec3& v, const Matrix& M)
{
    if (M.get_num_rows() != 3)
    {
        string error = Logger::error("MATRIX", "Matrix rows must be 3 for multiplication with vec3");
        throw std::invalid_argument(error);
    }

    vector<double> result(M.get_num_columns(), 0.0);

    for (unsigned j = 0; j < M.get_num_columns(); ++j)
    {
        result[j] = 0.0;
        for (unsigned i = 0; i < M.get_num_rows(); ++i)
        {
            result[j] += v[i] * M[i][j];
        }
    }

    return result;
}

// Scalar operators
inline Matrix operator+(const Matrix& M, const double scalar)
{
    Matrix result(M.get_num_rows(), M.get_num_columns());

    for (unsigned i = 0; i < M.get_num_rows(); i++)
    {
        for (unsigned j = 0; j < M.get_num_columns(); j++)
        {
            result[i][j] = M[i][j] + scalar;
        }
    }
    return result;
}

inline Matrix operator-(const Matrix& M, const double scalar)
{
    Matrix result(M.get_num_rows(), M.get_num_columns());

    for (unsigned i = 0; i < M.get_num_rows(); i++)
    {
        for (unsigned j = 0; j < M.get_num_columns(); j++)
        {
            result[i][j] = M[i][j] - scalar;
        }
    }
    return result;
}

inline Matrix operator*(const Matrix& M, const double scalar)
{
    Matrix result(M.get_num_rows(), M.get_num_columns());
    for (unsigned i = 0; i < M.get_num_rows(); i++)
    {
        for (unsigned j = 0; j < M.get_num_columns(); j++)
        {
            result[i][j] = M[i][j] * scalar;
        }
    }
    return result;
}

inline Matrix operator/(const Matrix& M, const double scalar)
{
    if (scalar == 0.0)
    {
        string error = Logger::error("MATRIX", "Division by zero is not allowed.");
        throw std::invalid_argument(error);
    }

    Matrix result(M.get_num_rows(), M.get_num_columns());

    for (unsigned i = 0; i < M.get_num_rows(); i++)
    {
        for (unsigned j = 0; j < M.get_num_columns(); j++)
        {
            result[i][j] = M[i][j] / scalar;
        }
    }
    return result;
}

// Reversed scalar operators
inline Matrix operator+(double scalar, const Matrix& M) { return M + scalar; }
inline Matrix operator-(double scalar, const Matrix& M) { return (-M) + scalar; }
inline Matrix operator*(double scalar, const Matrix& M) { return M * scalar; }

