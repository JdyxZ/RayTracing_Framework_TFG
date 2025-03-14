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

/// ++++++++++++ IMPLEMENTATION ++++++++++++++++ ///

// Constructors
Matrix::Matrix(unsigned num_rows, unsigned num_columns, double initial)
{
    if ((num_rows < 2 && num_columns < 2) || num_rows == 0 || num_columns == 0)
        throw std::invalid_argument("Matrix dimensions must be at least 1xn or nx1.");

    this->num_rows = num_rows;
    this->num_columns = num_columns;
    values = vector<vector<double>>(num_rows, vector<double>(num_columns, initial));
}

// Matrix operations
Matrix Matrix::identity(unsigned size) const
{
    Matrix result(size, size, 0.0);

    // Set diagonal elements to 1
    for (unsigned i = 0; i < size; ++i) {
        result[i][i] = 1.0;
    }

    return result;
}

Matrix Matrix::transpose() const
{
    Matrix result(num_columns, num_rows, 0.0);

    for (unsigned i = 0; i < num_columns; i++)
    {
        for (unsigned j = 0; j < num_rows; j++) {
            result[i][j] = this->values[j][i];
        }
    }

    return result;
}

double Matrix::determinant() const
{
    if (num_rows != num_columns)
        throw std::invalid_argument("Matrix must be square to compute determinant.");

    // Base case for 2x2 matrix
    if (num_rows == 2) {
        return values[0][0] * values[1][1] - values[0][1] * values[1][0];
    }

    double det = 0;
    for (unsigned i = 0; i < num_columns; ++i)
    {
        // Create submatrix by excluding the current row and column
        Matrix submatrix(num_rows - 1, num_columns - 1);

        for (unsigned j = 1; j < num_rows; ++j)
        {
            unsigned sub_col = 0;
            for (unsigned k = 0; k < num_columns; ++k)
            {
                if (k == i) continue;
                submatrix[j - 1][sub_col++] = values[j][k];
            }
        }
        // Cofactor expansion
        det += (i % 2 == 0 ? 1 : -1) * values[0][i] * submatrix.determinant();
    }

    return det;
}

Matrix Matrix::inverse() const
{
    if (num_rows != num_columns)
        throw std::invalid_argument("Matrix must be square to compute inverse.");

    double det = determinant();
    if (det == 0)
        throw std::invalid_argument("Matrix is singular and cannot be inverted.");

    // Compute the adjugate matrix
    Matrix adjugate(num_rows, num_columns);
    for (unsigned i = 0; i < num_rows; ++i)
    {
        for (unsigned j = 0; j < num_columns; ++j)
        {
            // Create submatrix for cofactor
            Matrix submatrix(num_rows - 1, num_columns - 1);
            unsigned sub_row = 0;

            for (unsigned r = 0; r < num_rows; ++r)
            {
                if (r == i) continue;
                unsigned sub_col = 0;
                for (unsigned c = 0; c < num_columns; ++c)
                {
                    if (c == j) continue;
                    submatrix[sub_row][sub_col++] = values[r][c];
                }
                ++sub_row;
            }
            // Compute cofactor and adjugate matrix
            adjugate[j][i] = ((i + j) % 2 == 0 ? 1 : -1) * submatrix.determinant();
        }
    }

    // Divide adjugate matrix by the determinant
    adjugate *= (1.0 / det);
    return adjugate;
}

double Matrix::trace() const
{
    if (num_rows != num_columns)
        throw std::invalid_argument("Matrix must be square to compute trace.");

    double sum = 0.0;
    for (unsigned i = 0; i < num_rows; ++i) {
        sum += values[i][i];
    }
    return sum;
}

Matrix Matrix::power(unsigned exponent) const
{
    if (num_rows != num_columns)
        throw std::invalid_argument("Matrix must be square to be raised to a power.");

    if (exponent == 0)
        throw std::invalid_argument("Cannot power with exponent 0");

    Matrix base = this->copy();
    Matrix result = identity(num_rows);

    while (exponent > 0)
    {
        if (exponent % 2 == 1)
            result = result * base;

        base = base * base;
        exponent /= 2;
    }

    return result;
}

void Matrix::swap_rows(unsigned row1, unsigned row2)
{
    if (row1 >= num_rows || row2 >= num_rows)
        throw std::invalid_argument("Row index out of range.");

    std::swap(values[row1], values[row2]);
}

void Matrix::swap_columns(unsigned col1, unsigned col2)
{
    if (col1 >= num_columns || col2 >= num_columns)
        throw std::invalid_argument("Column index out of range.");

    for (unsigned i = 0; i < num_rows; ++i)
        std::swap(values[i][col1], values[i][col2]);
}

void Matrix::scale_row(unsigned row, double scalar)
{
    if (row >= num_rows)
        throw std::invalid_argument("Row index out of range.");

    for (unsigned i = 0; i < num_columns; ++i)
        values[row][i] *= scalar;
}

void Matrix::scale_column(unsigned col, double scalar)
{
    if (col >= num_columns)
        throw std::invalid_argument("Column index out of range.");

    for (unsigned i = 0; i < num_rows; ++i)
        values[i][col] *= scalar;
}


// Access operators
vector<double>& Matrix::operator[](const unsigned row)
{
    return values[row];
}

const vector<double>& Matrix::operator[](unsigned row) const // Const version (needed for read-only access)
{
    return values[row];
}

double Matrix::operator()(const unsigned row, const unsigned col) const
{
    return values[row][col];
}

// Math operators
Matrix Matrix::operator-() const // Unary minus operator
{
    Matrix result(num_rows, num_columns);

    for (unsigned i = 0; i < num_rows; i++)
    {
        for (unsigned j = 0; j < num_columns; j++)
        {
            result[i][j] = -values[i][j];
        }
    }

    return result;
}

Matrix& Matrix::operator+=(const Matrix& M)
{
    if (num_rows != M.num_rows || num_columns != M.num_columns)
        throw std::invalid_argument("Matrix dimensions must match for += operation.");

    for (unsigned i = 0; i < num_rows; i++)
    {
        for (unsigned j = 0; j < num_columns; j++)
        {
            values[i][j] += M[i][j];
        }
    }

    return *this;
}

Matrix& Matrix::operator-=(const Matrix& M)
{
    if (num_rows != M.num_rows || num_columns != M.num_columns)
        throw std::invalid_argument("Matrix dimensions must match for -= operation.");

    for (unsigned i = 0; i < num_rows; i++)
    {
        for (unsigned j = 0; j < num_columns; j++)
        {
            values[i][j] -= M[i][j];
        }
    }

    return *this;
}

Matrix& Matrix::operator+=(const double scalar)
{
    for (unsigned i = 0; i < num_rows; i++)
    {
        for (unsigned j = 0; j < num_columns; j++)
        {
            values[i][j] += scalar;
        }
    }
    return *this;
}

Matrix& Matrix::operator-=(const double scalar)
{
    for (unsigned i = 0; i < num_rows; i++)
    {
        for (unsigned j = 0; j < num_columns; j++)
        {
            values[i][j] -= scalar;
        }
    }
    return *this;
}

Matrix& Matrix::operator*=(const double scalar)
{
    for (unsigned i = 0; i < num_rows; i++)
    {
        for (unsigned j = 0; j < num_columns; j++)
        {
            values[i][j] *= scalar;
        }
    }
    return *this;
}

Matrix& Matrix::operator/=(const double scalar)
{
    if (scalar == 0.0)
        throw std::invalid_argument("Division by zero is not allowed.");

    for (unsigned i = 0; i < num_rows; i++)
    {
        for (unsigned j = 0; j < num_columns; j++)
        {
            values[i][j] /= scalar;
        }
    }
    return *this;
}

// Help methods
void Matrix::print() const
{
    std::cout << "Matrix: " << std::endl;
    for (unsigned i = 0; i < num_rows; i++)
    {
        for (unsigned j = 0; j < num_columns; j++)
        {
            std::cout << "[" << values[i][j] << "] ";
        }
        std::cout << std::endl;
    }
}

Matrix Matrix::copy() const
{
    Matrix result(num_rows, num_columns);

    for (unsigned i = 0; i < num_rows; i++)
    {
        for (unsigned j = 0; j < num_columns; j++)
        {
            result[i][j] = values[i][j];
        }
    }
    return result;
}

// Attribute accessors
unsigned Matrix::get_num_rows() const
{
    return num_rows;
}

unsigned Matrix::get_num_columns() const
{
    return num_columns;
}

// Matrix Operators
inline Matrix operator+(const Matrix& A, const Matrix& B)
{
    if (A.get_num_rows() != B.get_num_rows() || A.get_num_columns() != B.get_num_columns())
        throw std::invalid_argument("A:rows != B:rows or A:cols != B:cols");

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
        throw std::invalid_argument("A:rows != B:rows or A:cols != B:cols");

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
        throw std::invalid_argument("A:cols != B:rows");

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
        throw std::invalid_argument("Number of matrix columns must match vector size.");

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
        throw std::invalid_argument("Number of vector elements must match matrix row count.");

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
        throw std::invalid_argument("Matrix columns must be 3 for multiplication with vec3");

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
        throw std::invalid_argument("Matrix rows must be 3 for multiplication with vec3");

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
        throw std::invalid_argument("Division by zero is not allowed.");
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
Matrix operator+(double scalar, const Matrix& M) { return M + scalar; }
Matrix operator-(double scalar, const Matrix& M) { return (-M) + scalar; }
Matrix operator*(double scalar, const Matrix& M) { return M * scalar; }

#endif
