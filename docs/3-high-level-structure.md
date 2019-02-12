# 3. High Level Structure

## 3.1 **Top Level Constructs**
These are the constructs in Nitwit, which are written *without indentation*.
All of the top level constructs may be referenced from any part of the code,
thus their order doesn't matter.

### 3.1.1 Global Variables
These are variables, accessible from all functions.
Their declaration look like `$ <type> <name>`, where `<type>` is a valid
type and `<name>` is a valid identifier.
The starting value of global variables can't be specified and is `0` or `.`
for *primitive* and *non-primitive* types respectively
No two global variables may have the same name.

### 3.1.2 Structs
*Structs* are defined by their name and the list of their *members*, which
have both a **type** and a **name**.
They are declared the following way:
```
$: <struct name>
    $ <member type 1> <member name 1>
    $ <member type 2> <member name 2>
    ...
```
The member declarations have to be indented at least one space and the
struct declaration ends at the first non-indented **logical line** (or EOF).
Different lines may have different indentation, though there isn't any
semantic difference.

No two structs may have the same name.
Furthermore, no struct may have the same name as any of the primitive types
or `void`.

`<member type i>` can be any valid type and `<member name i>` can be any
valid identifier.
No two members of *the same* struct may have the same name.
It is allowed for different structs to have members with the same name.


### 3.1.3 Functions
Function declarations consist of a *prototype* and a *body*.
The *prototype* holds the information about the the function's *name*,
*return type*, and the names and types of its *arguments*.
The *body* is a sequence of *statements*, indented at least one space, and
ends at the first non-indented *logical line* (or EOF).

Function Declarations have the following syntax:
```
$@ <return type> <function name> <arg type 1> <arg name 1> ...
    <statement 1>
    <statement 2>
    ...
```

`<function name>` and `<arg name i>` have to be valid identifiers,
`<arg type i>` has to be a valid type.
`<return type>` may either be a valid type or `void`, the latter meaning the
function doesn't return a value.

No two functions may have the same name.
This implies that there is no function overloading.
(Though it wouldn't be possible anyway, as the prefix notation requires all
operations to take a predetermined number of arguments.)

The execution of the program starts from the *main* function, which has the
following prototype: `$@ int main`.
It should return `0` if the program has been executed successfully and
something else otherwise.

If any non-*void* function terminates without reaching a return statement,
the default value (`0` or `.`) for its return type is returned.

## 3.2 Variable Scope

### 3.2.1 Blocks
The statements in function bodies are grouped in *blocks*, depending on their
*indentation*.
For every level of indentation *i*, all *contiguous sequences* of logical lines,
*indented at least i*, are grouped in blocks.
Blocks are made up of the statements indented *exactly i*, as well as the
*nested* blocks with *greater indentation*.
As a line may be in multiple nested blocks, we define *the block* of a line or
statement to be *the most indented* of the blocks it belongs to.

### 3.2.2 Local Variables
Local variables may be declared *anywhere* within the body of a function.
They are created at their *declarations* and destroyed when the *block* they are
declared in is exited.
A variable is *alive* between its creation and its destruction.

The memory for each local variable is allocated on the stack at the beginning of
its block call, and freed when it is exited.
Note that if a block is *executed multiple times* (e.g. a loop), any variables
declared inside of it will be created and destroyed multiple times as well
(though they wouldn't use additional memory).

### 3.2.3 Shadowing
Two local variables may share a the same name only if they are declared in
different blocks.
If two such variables are declared in *nested* blocks, the variable declared in
the more indented block *shadows* the one declared in the less indented one.
While both variables are alive, their shared name is understood to refer to the
*shadowing* variable.
There is no way to access the *shadowed* variable before the *shadowing*
variable is destroyed.

A local and a global variable may share the same name, but the local variable
will *shadow* the global one.

Function arguments behave as local variables, declared in a *block without
indentation*.
Thus, two arguments of the same function can't have the same name, arguments
can be shadowed by local variables, and at the end of the function all arguments
are destroyed.
