/*
 * test_functions.c - Test file to verify C functions work correctly
 * This is for verification purposes only - not included in kernel build
 */

#include <stdio.h>

/* Copy of the C functions from kernel.c for testing */
int sum_of_three(int arg1, int arg2, int arg3) {
    return arg1 + arg2 + arg3;
}

int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    int result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

int fibonacci(int n) {
    if (n <= 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }
    
    int a = 0, b = 1, result = 0;
    for (int i = 2; i <= n; i++) {
        result = a + b;
        a = b;
        b = result;
    }
    return result;
}

int power(int base, int exponent) {
    if (exponent == 0) {
        return 1;
    }
    
    int result = 1;
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }
    return result;
}

int main() {
    printf("Testing C functions that will be called from kernel:\n");
    printf("sum_of_three(1, 2, 3) = %d (expected: 6)\n", sum_of_three(1, 2, 3));
    printf("factorial(5) = %d (expected: 120)\n", factorial(5));
    printf("fibonacci(10) = %d (expected: 55)\n", fibonacci(10));
    printf("power(2, 8) = %d (expected: 256)\n", power(2, 8));
    
    if (sum_of_three(1, 2, 3) == 6 && factorial(5) == 120 && 
        fibonacci(10) == 55 && power(2, 8) == 256) {
        printf("✅ All functions return correct values!\n");
        return 0;
    } else {
        printf("❌ Some functions return incorrect values!\n");
        return 1;
    }
}
