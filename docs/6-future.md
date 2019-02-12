# 6. Future
This section includes ideas for further development of nitwit, in no particular
order:

-   Module system
    -   A way of importing functionality from different source files.
    -   A way of exporting functions and types
    -   A way of using c functions
    -   A standard library with math functions and casts
-   Variadic functions
    -   Functions that may take a variable number of arguments, deno
    -   `$@() <type> <function name> <arg list type> <list_name>`
    -   `$@() int sum #int args`
-   Text handling
    -   A character type
    -   Input / output operators on `char` and `#char`.
    -   Text and character literals
-   Better support for doubles
    -   Scientific notation literals
    -   Ways to check for NaN and infinity
-   More loops
    -   For loops
        -   `[<, >, >=, or <=] <variable_name> [start value] <end_value> [step]`
        -   `[<] <start expression> <condition> <increment>`
        -   `[<] i 5`
        -   `[>=] i 5`
        -   `[<] div * i i num i`
        -   `[#] i arr`
            -   `i` is a strong reference
            -   The loop holds a strong reference to `arr` so it can't be deleted
    -   Break statement `!!`
    -   Continue statement `!~`.
-   More operators
    -   Bit wise operators `&&`, `||`, `^^`, `~~`, `>>>`, `<<<`,
        -   Issues with negatives and signed numbers
    - Increment decrement operators
    - Casting operator e.g `$$ <type> <value>`

