# arpl - a random programming language
compiler

## build
```
make
```
the executable will be in bin

## features

### semantics
* scoping
* type checking
* type casting
* type coercion

### datatypes
* u8
* u16
* u32
* u64
* i8
* i16
* i32
* i64
* bool
* pointer

### misc
All datatypes are immutable by default. To make a datatype mutable, add the `mut` keyword before it.

Use `->` to perform type casting. For example: `n -> i32` is an expression that casts `n` to an `i32`.

## examples

### functions
```arpl
fn add (a i32, b i32) i32
{
    return a + b;
}

fn main () i32
{
    return add(2, 3);
}
```

### type casting
```arpl
fn f () i8
{
    var a mut i32 = 7;
    a = 6;
    var b mut i8 = a;
}
```
