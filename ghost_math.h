#if !defined(HANDMADE_MATH_H)
//
union v2
{
    struct
    {
        real32 X, Y;
    };
    real32 E[2];
};

// TODO(): Consider v2 A = v2{5, 3}; ?
inline v2
V2(real32 X, real32 Y)
{
    v2 Result;

    Result.X = X;
    Result.Y = Y;

    return(Result);
}

inline v2
operator*(real32 A, v2 B)
{
    v2 Result;

    Result.X = A*B.X;
    Result.Y = A*B.Y;

    return(Result);
}

inline v2
operator*(v2 B, real32 A)
{
    v2 Result = A*B;

    return(Result);
}

inline v2 &
operator*=(v2 &B, real32 A)
{
    B = A * B;

    return(B);
}

inline v2
operator-(v2 A)
{
    v2 Result;

    Result.X = -A.X;
    Result.Y = -A.Y;

    return(Result);
}

inline v2
operator+(v2 A, v2 B)
{
    v2 Result;

    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;

    return(Result);
}

inline v2 &
operator+=(v2 &A, v2 B)
{
    A = A + B;

    return(A);
}

inline v2
operator-(v2 A, v2 B)
{
    v2 Result;

    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;

    return(Result);
}

inline real32
Square(real32 A)
{
    
    real32 Result = A * A;
    return(Result);
}

inline real32
Inner(v2 A, v2 B)
{
    real32 Result;

    Result = (A.X * B.X) + (A.Y * B.Y);
    
    return(Result);
}

inline real32
LengthSq(v2 A)
{
    real32 Result = Inner(A, A);

    return(Result);
}

inline real32
AbsValue(real32 A)
{
    if( A < 0)
    {
        A *= -1;
    }
    
    return(A);
}

inline real32
AbsDiff(real32 A, real32 B)
{
    real32 Result;
    
    if(A >= B)
    {
        Result = A - B;
    }
    else
    {
        Result = B - A;
    }

    return(Result);
}

#define HANDMADE_MATH_H
#endif
