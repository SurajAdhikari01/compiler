// Assembly function to check if a number is prime
// Compatible with C calling convention on Apple Silicon (ARM64)

.text
.align 2
.global _is_prime

// Function: is_prime
// Input: x0 = number to check
// Output: x0 = 1 if prime, 0 if not prime
_is_prime:
    stp     x29, x30, [sp, #-16]!
    mov     x29, sp
    
    // Check if number <= 1
    cmp     x0, #1
    b.le    not_prime_return
    
    // Check if number == 2
    cmp     x0, #2
    b.eq    prime_return
    
    // Check if number is even
    tst     x0, #1
    b.eq    not_prime_return
    
    // Check odd divisors from 3 to sqrt(n)
    mov     x1, #3        // divisor counter
    mov     x2, x0        // save original number
    
check_loop:
    // Calculate x1 * x1
    mul     x3, x1, x1
    
    // If x1^2 > n, number is prime
    cmp     x3, x2
    b.gt    prime_return
    
    // Check if n % x1 == 0
    udiv    x3, x2, x1
    msub    x4, x3, x1, x2  // x4 = x2 - (x3 * x1) = remainder
    cbz     x4, not_prime_return
    
    // Increment divisor by 2 (check only odd numbers)
    add     x1, x1, #2
    b       check_loop
    
prime_return:
    mov     x0, #1
    ldp     x29, x30, [sp], #16
    ret
    
not_prime_return:
    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

// Function: find_primes_up_to_n
// Input: x0 = upper limit
// Output: x0 = count of primes found
.align 2
.global _find_primes_up_to_n
_find_primes_up_to_n:
    stp     x29, x30, [sp, #-32]!
    mov     x29, sp
    stp     x19, x20, [sp, #16]
    
    mov     x19, x0       // save upper limit
    mov     x20, #2       // start from 2
    mov     x0, #0        // prime counter
    
find_loop:
    cmp     x20, x19      // compare with upper limit
    b.gt    find_done
    
    str     x0, [sp, #-16]!   // save prime counter
    mov     x0, x20           // pass current number to is_prime
    bl      _is_prime
    
    ldr     x1, [sp], #16     // restore prime counter
    cmp     x0, #1            // check if prime
    b.ne    continue_find
    
    // Number is prime, increment counter
    add     x0, x1, #1
    b       next_number
    
continue_find:
    mov     x0, x1            // restore counter
    
next_number:
    add     x20, x20, #1      // next number
    b       find_loop
    
find_done:
    ldp     x19, x20, [sp, #16]
    ldp     x29, x30, [sp], #32
    ret