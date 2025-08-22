#include <stdio.h>
#include <Python.h>

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

    return 0;
}
