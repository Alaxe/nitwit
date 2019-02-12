# 4. Expressions

## 4.1 Prefix Notation
Expressions in Nitwit are written using *prefix* notation, instead of the
traditional *infix* notation.
This choice has been made both because of its simplicity and its quirkiness.

The simplest expressions consist of a single value, either a *literal* or an
*identifier*.
Multiple expression can be combined using *operators*.
The general syntax is `<operator> <operand_1> <operand_2> ... <operand_n>`,
where every operand is also a valid expression.
As every operator accepts a predetermined number of operands, the parsing of
expressions is unambiguous.

Nitwit guarantees only that the arguments of each operator are calculated before
the result of the operator.
The arguments themselves may be evaluated in any order, with the exception of
the arguments of the `&` and `|` operators, as specified in 4.4.4.

Furthermore, if the order of evaluation of two operators is not specified and
they have any of the following *side effects*, the behavior of the program is
*undefined*:

-   If they both perform IO operations.
-   If they both modify the same variable.
-   If one of them modifies a variable, whose value is used by the other.

## 4.2 Result Categories
Every expression has as a result a single value, which falls in one of the
following categories:

-   **LValue**: These results can be used as the first or the second argument of
    an assignment operator (`=`).
    They further have a valid **type**.

-   **RValue**: These results can only be used only as the second argument of an
    assignment operator (`=`).
    They also have a **type** which may be any *primitive* type, any *RValue*
    type or any *flawed* type.

-   **Identifier**: This is the type of the *identifier value expressions*.
    Identifiers are the names of *functions*, *types*, *members*, or
    *variables*.

To simplify the next sections, the term *Value* will be used to mean either an
*LValue* or an *RValue*.
Also, when an operator requires any *Value* for one of its operands but receives
an *identifier*, then the *Identifier* is understood to be referencing a
variable and its result category is implicitly converted to an *LValue*.
In such situations the *Identifier* must be referencing a valid variable.

## 4.3 Value Expressions
These are the valid expression, consisting of a single token.
They come in two categories:

-   **Literals** are values, that are hard-coded in the source code.
    Their result category is *RValue*.
    The following are valid *literals*:
    -   Integer numbers (e.g. `436`) are literals with type `int64`.
        Values which can't fit in `int64` aren't valid integer literals.
    -   Fractional numbers (e.g. `3.60`) are literals with type `real`
    -   The *null literal* `.` is a literal with the *null* type.

-   **Identifiers** are names of *functions*, *types*, *members*, or
    *variables*.
    The context the indentifier is used in implies the type
    of construct it is refering to.

## 4.4 Operators on Primitives
In this section it is implied that all *Value* arguments must have a primitive
type, unless stated otherwise.

### 4.4.1 Arithmetic Conversion
Most of the operators in this section internally work with arguments of a common
type.
*Arithmetic conversion* is the process Nitwit uses to achieve that.

The *primitive* types are ranked `int8`, `uint8`, `int16`, `uint16`, `int`,
`int32`, `uint`, `uint32`, `float`, `real`.
In general, if one type precedes another type in this list, the set of values,
storable in the first, is a subset of the set of values, storable in the second.
Thus, converting a value from the first to the second type would usually keep it
the same.

*Arithmetic conversion* consists of taking the highest ranked argument of an
operator and converting all other arguments to that type.
The type to which all arguments are converted is refered to as the
*common type*.

A converted value could not fit in its new type only during a conversion from a
signed to an unsigned integer.
In these cases, the converted value is stored modulo the range of the
*common type*.

### 4.4.2 Arithmetic Operators
The arithmetic operators are *addition* (`+`), *subtraction* (`-`),
*multiplication* (`*`), *division* (`/`), and *remainder* (`//`).

All of them comply with following:

-   They take two *Value* arguments.
-   *Arithmetic conversion* is performed to their arguments
-   Their result is an *RValue* with the *common type*.
-   If the *common type* is a signed integer and the result does not fit in it,
    the behavior is undefined.
-   If the *common type* is an unsigned integer, all operations follow the rules
    of modular arithmetic.

The division (`/`) and remainder (`//`) operators to an integer *common type*
additionally comply with the following:

-   If second argument is equal to `0`, the behavior is undefined.
-   The result of division (`/`) is rounded towards zero.
-   The value of `+ // a b / a b b` is always equal to `a`.

The remainder (`//`) operator may only be applied to an integer *common type*.

### 4.4.3 Comparison Operators
The comparison operators are *equals* (`==`), *does not equal* (`~=`),
*less than* (`<`), *greater than* (`>`), *less than or equal* (`<=`),
and *greater than or equal* (`>=`).

All of them comply with the following:

-   They take two *Value* arguments.
-   *Arithmetic conversion* is performed to their arguments
-   Their result is an *RValue* with type `int`.
    It has value `1` if the comparison is true and `0` otherwise.

`==`, `~=`, `<=`, and `>=` may only be applied to an integer *common type*.
As floating point numbers suffer from rounding errors during arithmetic
opreations, they may not be directly compared directly for equality.

### 4.4.4 Logical Operators
The logical operators are *and* (`&`), *or* (`|`), *xor* (`^`), and *not* (`~`).

All of them comply with the following:

-   They take *Value* arguments with some integer type.
-   *Arithmetic conversion* is performed to to their arguments
-   Their result is an *RValue* with type `int`.

The operators *and* (`&`), and *or* (`|`) comply with the following:

-   They take two arguments.
-   The evaluation of the second argument may only start after the evaluation of
    the first has finished.
-   If the result of the operator in known, without evaluating its second
    argument, the second argument is not evaluated (so-called *short-circuit*
    evaluation).

The operator *xor* (`^`) also takes two argumens, while the operator *not* (`~`)
takes only a single argument.

The value of the operators is defined as follows:

-   *And* (`&`) has value `1` (true), if both of its arguments are not equal to
    `0`, and value `0` (false) otherwise.
-   *Or* (`|`) has value `1` (true), if *at least* one of its arguments is not
    equal to `0`, and value `0` (false) otherwise.
-   *Xor* (`^`) has value `1` (true), if *excatly* one of its arguments is not
    equal to `0`, and value `0` (false) otherwise.
-   *Not* (`~`) has value `1` (true), if its argument is equal to `0`, and value
    `0` (false) otherwise.

### 4.4.5 I/O Operators
The input / output (I/O) operators are *read* (`>>`) and *write* (`<<`).

The *read* operator (`>>`) takes a single *LValue* argument, while the *write*
operator takes a single *Value* argument.
Both of them have an *RValue* result with type `int`.
Its value is `1` if the operation has been successful, and `0` otherwise.

### 4.4.6 Assignment Operator
The *assignment operator* (`=`) takes two arguments:

-   An *LValue*, to which the second argument is assigned.
-   A *Value*, which is assigned to the first argument.

The *assignment operator* (`=`) return its first argument as an *LValue*.

If the arguments have different types, the value of the second argument
(the *converted value*) is converted to the type of the of the first argument
(the *target type*).
Depending on the types of the arguments, one of the following rules applies:

-   **Target higher ranked than converted value:** The value is converted
    following the rules of *arithmetic conversion*.

-   **Floating Point to Integer:** The *converted value* is rounded towards
    zero.
    If it does not afterwards fit in the *target type*, the behavior is
    undefined.

-   **Floating Point to Floating Point:** The *converted value* is rounded to
    the nearest value, representable by the target type.
    If the converted value falls outside of the range of the *target type*, the
    behavior is undefined.

-   **Integer to Integer:**
    -   If the *target type* is *unsigned*, than the *converted value* is stored
        modulo its range.
    -   If the *target type* is signed, the *converted value* is not modified,
        if it fits in the *target type*, otherwise the behavior is undefined.

### 4.4.7 Augmented Assignment Operators
The *agumented assignment operators* `+=`, `-=`, `*=`, `/=`, and `//=` combine
an *arithmetic operation* and assignment.
The expression `<opr>= a b` is equivalent to `= a <opr> a b` with the exception
that the first argument (`a`) is evaluated only once.

## 4.5 Operators on Non-Primitive types
In this section it is implied that all *Value* arguments have a *non-primitive*
type, unless stated otherwise.

### 4.5.1 RValue Conversion
This is the process of converting any *non-primitive* type to an equivalent
*RValue* type.
*RValue* and *primitive* types are not modified, while *weak references* are
changed to *strong* references.

### 4.5.2 Assignment Operator
The assignment operator (`=`) takes two arguments:

-   An *LValue*, to which the second argument is assigned.
-   A *Value*, which is assigned to the first argument.
    If the second argument is a *weak* reference to a *destroyed* object, it is
    treated as if its value were *null* (`.`).

The *assignment operator* (`=`) return its first argument as an *LValue*.

Assignment is allowed if one of the following is true:

- The types of the arguments are the same after *RValue Conversion*.
- The type of the second argument is the *Null Type*.


### 4.5.3 Comparison Operators
The *comparison operators* *equals* (`==`) and *does not equal* (`~=`) take two
*Value* arguments, with one of the following true for their types:

-   At least one of the arguments has the *Null Type* (possibly both)
-   The types of the arguments are the the same after *RValue Conversion*.


The *equals* (`==`) has an *RValue* result with type `int` and value `1` if the
arguments point to the same object and `0` otherwise.
Using the *does not equal* operator `~= a b` is equivalent to `~ == a b`.

If any of the arguments is a *weak* reference to a destroyed object, it is
treated as if its value were *null* (`.`).

### 4.5.4 Allocation Operator
The *allocation operator* (`%`) is used for creating objects of specific type.
Depending on the type, its syntax is:

-   **Struct type:** The operator takes a single *Identifier* argument, the name
    of the struct.
-   **Array type:** The operator takes two arguments, an *Identifier*, denoting
    a valid *RValue array type*, and an integer, denoting the size of the array
    to be created.
    If the value of the second argument is non-positive, the behavior is
    undefined.

The *allocation operator* returns an *RValue*, a reference to the newly created
object.
All members of the struct / elements of the array are initialized with their
default value.

### 4.5.5 Indexing Operator
The *indexing operator* (`#`) is used for accessing elements of arrays.
It takes two *Value* arguments:

-   The array to be indexed, it must have an array type.
    If this argument is equal to *null* (`.`), the behavior is undefined.
-   The index to be accessed, it must have an integer type.
    If the value of this argument is greater or equal to the size of the array,
    the behavior is undefined.

The operator returns the element at the specified index in the provided array.
Depending on the *value category* of the first argument, the result has the
following category and type:

-   If the first arguments is an *LValue*, the result is an *LValue* with the
    same type as the elements of the array.
-   If the first argument is an *RValue*, the result is an *RValue* with the
    same type as the elements of the array, after *RValue conversion*.

### 4.5.6 Member Access Operator
The *member access operator* (`:`) is used for accessing members of instances of
structs.
It takes *two* arguments:

-   A *Value* with struct type, the object whose member is to be accessed.
    If this argument is equal to *null* (`.`), the behavior is undefined.
-   An *Identifier*, the name of the member to be accessed.
    It must match one of the members of the type of the first argument.

The operator returns the specified member of the provided type.
Depending on the *value category* of the first argument, the result has the
following category and type:

-   If the first arguments is an *LValue*, the result is an *LValue* with the
    same type as the member of the struct.
-   If the first argument is an *RValue*, the result is an *RValue* with the
    same type as the member of the struct, after *RValue conversion*.

### 4.5.7 Array Size Operator
The *array size operator* (`|#|`) complies with the following:

-   It takes a single *Value* argument with an array type.
    If the argument is equal to `.`, the behavior is undefined.

-   It returns the size of the array the argument references as *RValue* with
    type `int`.


## 4.6 Function-Call Operator
The *function-call operator* (`@`) complies with the following:

-   Its first argument is an *Identifier*, the name of the function to be
    called.
    It must reference a valid function.

-   A number of *Value* arguments follow, as many as specified in the
    *function prototype*.
    Additionally, assignment must be possible from the type of every *provided
    argument* to the type of its corresponding *function argument*, specified in
    the *function prototype*.
    When *weak* references to *destroyed* objects are passed, they are treated
    as if they were *null* (`.`).

-   Upon evaluation of the expression, the *function body* is executed,
    triggering all of its *side effects*.

-   The value is an *RValue*, having the return type specified in the *function
    prototype* and equal to the value returned by the function execution.
