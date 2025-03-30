// Headers
#include "core.hpp"
#include "matrix.hpp"

// Constructors
Matrix::Matrix(unsigned num_rows, unsigned num_columns, double initial)
{
    if ((num_rows < 2 && num_columns < 2) || num_rows == 0 || num_columns == 0)
    {
        string error = Logger::error("MATRIX", "Matrix dimensions must be at least 1xn or nx1.");
        throw std::invalid_argument(error);
    }

    this->num_rows = num_rows;
    this->num_columns = num_columns;
    values = vector<vector<double>>(num_rows, vector<double>(num_columns, initial));
}

Matrix44::Matrix44(double initial) : Matrix(4, 4, initial) {}
Matrix33::Matrix33(double initial) : Matrix(3, 3, initial) {}
Matrix22::Matrix22(double initial) : Matrix(2, 2, initial) {}

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
    {
        string error = Logger::error("MATRIX", "Matrix must be square to compute determinant.");
        throw std::invalid_argument(error);
    }

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
    {
        string error = Logger::error("MATRIX", "Matrix must be square to compute inverse.");
        throw std::invalid_argument(error);
    }

    double det = determinant();
    if (det == 0)
    {
        string error = Logger::error("MATRIX", "Matrix is singular and cannot be inverted.");
        throw std::invalid_argument(error);
    }

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
    {
        string error = Logger::error("MATRIX", "Matrix must be square to compute trace.");
        throw std::invalid_argument(error);
    }

    double sum = 0.0;
    for (unsigned i = 0; i < num_rows; ++i) {
        sum += values[i][i];
    }
    return sum;
}

Matrix Matrix::power(unsigned exponent) const
{
    if (num_rows != num_columns)
    {
        string error = Logger::error("MATRIX", "Matrix must be square to be raised to a power.");
        throw std::invalid_argument(error);
    }

    if (exponent == 0)
    {
        string error = Logger::error("MATRIX", "Cannot power with exponent 0");
        throw std::invalid_argument(error);
    }

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
    {
        string error = Logger::error("MATRIX", "Row index out of range.");
        throw std::invalid_argument(error);
    }

    std::swap(values[row1], values[row2]);
}

void Matrix::swap_columns(unsigned col1, unsigned col2)
{
    if (col1 >= num_columns || col2 >= num_columns)
    {
        string error = Logger::error("MATRIX", "Column index out of range.");
        throw std::invalid_argument(error);
    }

    for (unsigned i = 0; i < num_rows; ++i)
        std::swap(values[i][col1], values[i][col2]);
}

void Matrix::scale_row(unsigned row, double scalar)
{
    if (row >= num_rows)
    {
        string error = Logger::error("MATRIX", "Row index out of range.");
        throw std::invalid_argument(error);
    }

    for (unsigned i = 0; i < num_columns; ++i)
        values[row][i] *= scalar;
}

void Matrix::scale_column(unsigned col, double scalar)
{
    if (col >= num_columns)
    {
        string error = Logger::error("MATRIX", "Column index out of range.");
        throw std::invalid_argument(error);
    }

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
    {
        string error = Logger::error("MATRIX", "Matrix dimensions must match for += operation.");
        throw std::invalid_argument(error);
    }

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
    {
        string error = Logger::error("MATRIX", "Matrix dimensions must match for -= operation.");
        throw std::invalid_argument(error);
    }

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
    {
        string error = Logger::error("MATRIX", "Division by zero is not allowed.");
        throw std::invalid_argument(error);
    }

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

