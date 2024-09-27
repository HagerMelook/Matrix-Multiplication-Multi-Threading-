# Matrix-Multiplication-Multi-Threading

This project performs matrix multiplication using multi-threading in C. The program takes input matrices, their number of rows and columns, and performs multiplication by element, row, or matrix, leveraging the power of multiple threads for efficient computation.

# Features

Element-wise Multiplication: Each element in the result matrix is computed in a separate thread.

Row-wise Multiplication: Each row in the result matrix is computed using a dedicated thread.

Full Matrix Multiplication: The entire matrix multiplication is performed using multi-threading.

# Technologies Used

C Programming Language

POSIX Threads (pthread.h)

# How It Works
The user provides the matrices, along with the number of rows and columns for each.

The program offers three types of multiplication:

Element-wise multiplication: A thread is created for each element.

Row-wise multiplication: A thread is created for each row.

Matrix multiplication: Multiple threads are used to perform the entire matrix multiplication efficiently.

The result is returned as a new matrix.
