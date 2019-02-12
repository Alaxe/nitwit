# Nitwit
Nitwit is an imperative programming language that uses prefix notation for
amusement purposes.
It is strongly typed, gargabe collected, and compiled by being transcribed to C.

## How to use
To build the compiler, run `make stable`.
In case you don't have `make`, you'd have to compile all the files from `/src/`
manually.

Programs are compiled using `./nitwit <input file> <output file>`, where
`<input file>` is the nitwit source and `<output file>` is the compiled C
file.
To create an executable program, you'd then compile the C code using something
like `cc <output file> -o <program>`.

## Docs
Nitwit's syntax is documented in the [docs](./docs/spec.md) folder of this repo.
Additionally, some example code is provided under [sample-code](./sample-code).
