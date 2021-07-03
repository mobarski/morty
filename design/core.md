# Core vs Extension

The question is - should Morty aim for the "true" core or rather more practical one?

True core is just being Turing-complete - it's too close to the eso-langs.

The core should be based on practical minimalistic instruction sets: UXN, AnotherWorld VM, etc

In practice it's not about instruction count. All ALU instructions are trivial to implement, so ALU
should not be minimalistic. It's better to provide expressive power similar to C operators with small additions
to handle some low-cost CPU architectures (ie mulshr for CPUs without divide operation).


## Current core:

```
branch:  4 + 3 (loops)
stack:   10
mem:     8 
alu-ari: 7 + 2?
alu-bit: 6 + 1?
alu-cmp: 9 + 4?
io:      2
halt:    1  
nop:     1  

TOTAL:   51 + 7?
```

## Feature importance

| rank |      feature    | reason |
| ---- | --------------- | ------ |
|  1.1 | conditionals    | required for Turing completnes |
|  1.2 | loops           | required for Turing completnes BUT since single, global loop is enough conditionals are more important OTOH conditional execution is a special case of 0 or 1 times loop |
|  2.1 | arrays          | the most primitive collection |
|  2.2 | structures      |  |
|  3.1 | multitasking    |  |
|  3.2 | multiprocessing |  |
|    4 | lambdas         | without closures they are not that usefull |


# Compiler:

The compiler is separated into several layers:
- language (morty -> vm-asm)
- peephole optimizer (vm-asm -> vm-asm)
- assembler (vm-asm -> p-code)
- linker?
