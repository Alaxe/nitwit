# 2. Data Model

## 2.1 Primitive Types
The *primitive types* are:

-   **Integer types**:
    -   Signed: `int8`, `int16`, `int32`, `int`, and `int64`
    -   Unsigned: `uint8`, `uint16`, `uint32`, `uint`, and `uint64`

    The number in the suffix denotes the size (in bits) of the type.
    `int` and `uint` are aliases to `int32` and `uint32` respectively.

-   **Floating point**: `real` and `float`, equivalent to `double` and `float`
    in C.

## 2.2 Non-Primitive Types
This category consists of *arrays* and *structs*.
The instances of *non-primitive* types are called collectively *objects*.
Similarly to Java, objects are *pass by reference*, meaning that when objects
are passed to functions or are assigned to one another, *only the reference*
to the object is copied, instead of the whole object.

-   *Structs* are denoted by their name.
-   *Arrays* of `<type>` are denoted by `#<type>`.
    `<type>` may be any valid type.
-   *Weak references* to `<type>` are denoted by `~<type>`.
    `<type>` can be any *non-primtive* type that is not itself a *weak
    reference*.

*Strcuts* and *arrays* (without *weak references*) are collectively called
*RValue* types, as they are the *non-primtive* types an *RValue* may posses.

## 2.3 Flawed Types
*Flawed types* are types which can't be the types of any variables.
These are:

-   The *null* type, the type of the *null* literal `.`.
-   The *void* type, the type returned by *void* functions.

## 2.4 Strong and Weak References
Nitwit uses *reference counting* garbage collection, meaning that objects are
deleted once there aren't any references to them.
To deal with memory leaks due to reference cycles, Nitwit also has *weak*
references (denoted with `~`).
These are references, which don't count towards the object's reference count and
thus don't prohibit its *destruction*.

It's the job of the user to break dependency cycles through usage of *weak*
references.
*Weak* references add more performance overhead than *strong* references,
so when both types of references would do the job, prefer the *strong* option.
*Weak* references to *destroyed* objects are equivalent to *null* (`.`).

There are also *RValue* references, which are created for example when an object
is returned from a function.
To avoid premature deletion of an object, they count towards the reference count
as well.

## 2.5 Freeing of Memory
An object is *destroyed* when there aren't any strong references to it, though
the memory it occupies may not be immediately *freed*.
This is done so to prevent *weak* references, pointing to a *destroyed* object
(*dangling references*).
This way *weak* references can check whether they point to a *destroyed* object
and if so, behave like *null* `.`.
Each object is *freed* after it is both *destroyed* and no more *weak*
references point to it.
