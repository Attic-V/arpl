# ARPL: A Random Programming Language

compiler for a language that I built

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

* i8
* bool
* pointer
* user defined structs

## examples

### functions

```arpl
fn add (a i8, b i8) i8
{
    return a + b;
}

fn main () i8
{
    return add(2, 3);
}
```

### structs

```arpl
struct SomeStruct {
    a i8
    b i8
    c bool
}
```

