# arpl - a random programming language

messy compiler for a small toy language that I built

## build

```
make
```

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
* user defined structs

### misc

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

### structs

```arpl
struct SomeStruct {
    a u8
    b i32
    c bool
}
```

