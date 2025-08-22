#include <stdio.h>
#include <Python.h>

// Declare the assembly functions
extern int is_prime(int n);
extern int find_primes_up_to_n(int n);

// Function to call Python's print_hello function
void call_python_print_hello()
{
    // Initialize Python interpreter
    Py_Initialize();

    if (!Py_IsInitialized())
    {
        printf("Failed to initialize Python\n");
        return;
    }

    // Add current directory to Python path so it can find our module
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('.')");

    // Import the printhello module
    PyObject *pModule = PyImport_ImportModule("printhello");
    if (pModule == NULL)
    {
        printf("Failed to import printhello module\n");
        PyErr_Print();
        Py_Finalize();
        return;
    }

    // Get the print_hello function
    PyObject *pFunc = PyObject_GetAttrString(pModule, "print_hello");
    if (pFunc == NULL || !PyCallable_Check(pFunc))
    {
        printf("Failed to find print_hello function\n");
        PyErr_Print();
        Py_DECREF(pModule);
        Py_Finalize();
        return;
    }

    // Call the function
    PyObject *pResult = PyObject_CallObject(pFunc, NULL);
    if (pResult == NULL)
    {
        printf("Failed to call print_hello function\n");
        PyErr_Print();
    }

    // Clean up
    Py_XDECREF(pResult);
    Py_DECREF(pFunc);
    Py_DECREF(pModule);
    Py_Finalize();
}

int main()
{
#ifdef _WIN32
    printf("Hello, Windows!\n");
#elif __linux__
    printf("Hello, Linux!\n");
#elif __APPLE__
    printf("Hello, macOS!\n");
#else
    printf("Hello, Unknown OS!\n");
#endif

    // Call Python function
    printf("\nCalling Python function:\n");
    call_python_print_hello();

    // Test the is_prime function
    int test_numbers[] = {2, 3, 4, 5, 17, 25, 29, 30};
    int num_tests = sizeof(test_numbers) / sizeof(test_numbers[0]);

    printf("\nTesting individual numbers for primality:\n");
    for (int i = 0; i < num_tests; i++)
    {
        int num = test_numbers[i];
        int result = is_prime(num);
        printf("%d is %s\n", num, result ? "prime" : "not prime");
    }

    // Test the find_primes_up_to_n function
    int limit = 30;
    printf("\nFinding primes up to %d:\n", limit);
    int prime_count = find_primes_up_to_n(limit);
    printf("Total primes found: %d\n", prime_count);

    return 0;
}