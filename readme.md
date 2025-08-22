# Compile assembly separately

as -arch arm64 primecalculate.s -o primecalculate.o

# Compile C code to object file

clang -arch arm64 $(python3-config --cflags) -c main.c -o main.o

# Link everything together

clang -arch arm64 main.o primecalculate.o $(python3-config --ldflags) -lpython3.11 -o primenumber_with_assembly
