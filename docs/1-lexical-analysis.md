# 1. Lexical Analysis

## 1.1 Logical Lines
The source code is split into *logical lines*, which roughly resemble a single
statement.
They differ in a few ways from real lines:

-   Multiple lines can be joined together using `\` before a `\n`.
-   Tabs are replaced with four spaces.
-   Everything from a semicolon `;` to the end of the real line is ignored
    (incl. joining of real lines). The text after `;` is a comment.

After this processing, we define as *indentation* for each line the number of
spaces before the first non whitespace character.
Also, empty logical lines are removed.

## 1.2 Tokenization
Each *logical line* is further split into *tokens*.
There are three main types of tokens:

-   **Identifiers** are used for the names of variables, functions or types.
    They have to begin with an *alphabetic* character and consist of
    *alphanumeric* characters and underscores `_`.

-   **Literals** represent values.
    They have to consist of digits and at most one `.` for floating
    point literals.
    The `.` literal represents the *null* value.

-   **Special sequences** have to be made of `~!@#$%^&*(){}[]?+\/|`-:<=>`.
    They are used to represent operators or identify statements.

Tokens have to be separated by whitespace, with the ecxeption of a
*special sequence* followed by an *identifier* or a **literal**,
as this doesn't cause any ambiguities.

These general rules come with two exceptions:
1.  Names of non-primitive types can begin with a sequence of `#` and `~`.
    To deal with this, in situations where a type is expected but a
    *special sequence** followed by an *identifier* is provided, the two are
    combined into a single **identifier**.

    These situations are:

    -   After a `%` or `%` operator (used for memory allocation)
    -   In variable, member or function declarations on the positions where
        types are expected.

2.  If a *Special sequence* token begins with `#` or `:`, its first character is
    separated into a different token.
    This allows multiple `#` and `:` operators to be chained without being
    separated by whitespace.
