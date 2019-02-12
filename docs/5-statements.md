# 5. Statements
Statements are what the bodies of functions in Nitwit are made of.
All statements must be on separate logical lines, unless stated otherwise.

## 5.1 Expressions as Statements
Expressions with a result category different from *Identifier* are valid
statements in Nitwit.
Moreover, multiple expressions can be written on a single line and they are
evaluated from left to right.

## 5.2 Variable Declarations
*Variable declarations* in functions have tho following syntax:
`$ <type> <name> [expression]`.
The `<type>` must be a valid *type* and the `<name>` must be valid *Identifier*.

Optionally an `[expression]` may be provided for the initial value of the
variable.
It must have a *Value* result with a type that is assignable to the type of the
variable.
If the expression is omitted, the variable is initialized with the default value
for its type.

## 5.3 The Return Statement
The *return statement* marks the end of the execution of a function.
Its syntax depends on the return type of the function the *return statement* is
in:

-   `=>` for functions with return type *void*
-   `=> <expression>` for functions with *non-void* return type.
    The expression must have as a result a *Value* with a type, that can be
    assigned to the return type of the function.

If the execution of a function with *non-void* return type concludes without
reaching a *return statement*, the default value for that type is returned.

## 5.4 The Conditional Statement
The *conditional statement* has the following syntax:
```
? <expression>
    <block>
!? <expression>
    <block>
...
!
    <block>
```

-   It begins with an *if* line, consisting of the `?` token and an expression.
-   Optionally, a number of *else-if* lines may follow, consisting of the `!?`
    token and an expression.
-   Optionally, it may end with an *else* line, consisting only of the `!`
    token.
-   The expressions in the *if* and *else-if* lines must have a *Value* result
    with an integer type.
-   Every line is followed by a (potentially empty) indented block.

The expressions in the *if* and *else-if* lines are evaluated sequentially until
one of them has a *non-zero* value.
The Indented block following that expression is executed and no further
*else-if* expressions are evaluated.
If no *if* or *else-if* expression has a *non-zero* value and an *else* line is
present, the block following the *else* line is executed.

## 5.5 The While Loop
The *while loop* has the following syntax:
```
[] <expression>
    <block>
```

-   The `<expression>` must have a *Value* result with an integer type.
-   If the `<expression>` is omitted, it takes the default value of `1`.
-   The *while* line is followed by a (potentially empty) indented block.

The `<expression>` is evaluated multiple times and each time it has a
*non-zero* value, the indented block is executed.
The first time the `<expression>` is evaluated and it has a value, equal to
zero, the loop ends.

The compiler may assume for optimization purposes that every loop either
terminates, or has *side-effects*.
