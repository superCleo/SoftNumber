#include <stdint.h>
#include <stdio.h>

typedef int32_t  I32;
typedef uint32_t U32;
typedef uint8_t  U8;

// 32-bit unsigned integer
struct SU32
{
    SU32()
    :v(0)
    {}
    
    SU32(U32 value)
    :v(value)
    {}
    
    void operator=(U32 value)
    {
        v = value;
    }
    
    U32 v;
};

// 32-bit signed integer
struct SI32
{
    void operator=(I32 value)
    {
        v = value;
    }
    
    U32 v;
};

// 32-bit floating point
struct SF32
{
    void operator=(float value)
    {
        v = value;
    }
    
    U32 v;
};

U32 SU32ToUint32(SU32 x)
{
    return x.v;
}

I32 S32ToInt32(SU32 x)
{
    // regardless of the Endianness.
    I32 result = 0;
    
    return result;
}

/*
 Radix Complement, r: radix, n: number of digits, x: positive number
 dc(x) = (r^n) - x
 
 Diminished Radix Complement 
 drc(x) = dc(x) - 1 
        = (r ^ n) - 1 - x
 
 (r ^ n) - 1 = ... 
             = (r - 1) * (r ^ (n-1)) + (r - 1) * (r ^ (n-2)) + ... + (r - 1) * (r ^ 0)
 
 x = d_(n-1) * (r ^ (n-1)) + d_(n-2) * (r ^ (n-2)) + ... + d_0 * (r ^ 0)
 
 d_n is the number on the n-th digit
 d_n is guaranteed to be smaller than or equal to (r - 1)
 
 drc(x) = ((r ^ n) - 1) - x 
        = ((r - 1) - d_(n-1)) * (r ^ (n-1)) + ((r - 1) - d_(n-2)) * (r ^ (n-2)) + ... + ((r - 1) - d_0) * (r ^ 0)
 
 Binary case:
 drc(x) = [11...1] - x = negate every bit of x
*/
SU32 DiminishedTwoComplement(SU32 x)
{
    U32 result = x.v ^ 0xffffffff;
    return SU32(result);
}

SU32 operator+(SU32 a, SU32 b)
{
    U32 sum = 0;
    U32 carry = 0;
    U32 mask = 1;
    for (;;)
    {
        U32 bit_a = a.v & mask;
        U32 bit_b = b.v & mask;
        sum |= (bit_a ^ bit_b) ^ carry;
        
        mask = mask << 1;
        if (!mask) break;
        
        carry = (bit_a & bit_b) | ((bit_a ^ bit_b) & carry);
        carry = carry << 1;
    }
    
    return SU32(sum);
}

/*
 To calculate x - y,
 method 1: drc(drc(x) + y)
 method 2: (x + drc(y)) - (r ^ n) + 1
*/
SU32 operator-(SU32 a, SU32 b)
{
    // use method 2
    
    SU32 dtc_b = DiminishedTwoComplement(b);
    
    // automatically subtract (r ^ n) by overflowing
    SU32 st = a + SU32(dtc_b);
    
    SU32 result = st + SU32(1);
    
    return result;
}
