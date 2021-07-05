# Kanban

## Active
- local labels (visibility limited to single function)

## Next
- while-do-loop (`do` in then vs `do` in loop) vs begin-again loop
- structs: usage
- types: methods
- modules

## To do
- peephole optimizer as a separate tool
- separate stacks, code and mem ?
- memory slices (addr -> slice,offset) ?
- all addrs are indirect ?
- debug stacks depth
- check stacks overflows
- trace
- stand-alone executable
- transpilation to C
- cooperative multitasking
- docs: all vm instructions (including turbo mode)
- macros / inline ?
- tail call optimization
- fix globals_cnt
- constants
- iterators
- else ?
- heapsort.morty (requires else for easy porting)
- array operations (similar to numpy / APL) ?
- encode word in ASM
- special stack shuffling syntax -> ie abcd|cadb or |cabd // ab|aba or |aba
- vm text-based mem dump
- strings
- quote / unquote 
- ASM - token to line number (for error reporting)
- case
- little vs big endian
- literate programming (compile from markdown)
- multiprocessing
- add Morty to linguist (https://github.com/github/linguist) so it can be detected by GitHub

## Done
- fix: loops.asm (comments, wrong for order)
- fix: sieve.asm / sieve.morty (for loop order)
- replace -i option with -f (more standard)
- forward-declarations or other technique for top-down programming
- separate linker
- { ... } ( -- len addr ) push array length on the stack before addr
- `a[x 1 add] .get` array notation (remove lambdas for now, introduce [ and ] as visual separators)
- array notation ( .get .set )
- more intuitive for ( lo hi incr )
- qsort
- ackerman.morty
- structs: declaration
- fix random.asm -O (muli, divi, modi vs OPFREQ)
- vm: load cell code from stdin
- remove HLA (lang.py prototype + asm is enough)
- capture variables in lambdas (lambdas will use call frames)
- rewrite benchmarks from HLA to ASM
- lang: lambdas
- fix: random.morty
- fix: sieve.morty
- fix: array.morty
- rewrite benchmarks from HLA to Morty
- compiler CLI (morty -> asm, -i -o)
- global references -> globals pointer
- static arrays
- vm cleanup, separate core from (turbo) extension
- primitive output
- dynamic statistics (op freq) -> 25% performance penalty
- vm text-based portable executable format
- conditionals
- replicated switch threading
- sieve.hla
- parametric cell size (16, 32, 64 bits)
- loop changes (NO - new word for counting down on existing loops)
- define and document file extensions
- indefinitive loops
- definitive loops
- asm text-based compiled format
- stack based label resolution
- high-level assembler
- vm cli
- vm - load from binary file
- primitive output
- vm boot sequence
- asm output to binary file
- asm character literals
- lang - structs
- lang - macros

