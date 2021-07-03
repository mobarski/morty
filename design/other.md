# Threading

To benchmark new Threading Model following instructions must be implemented:
- **loops.mrh**: push, stor, vget, times, add, loop, call, ret, vminfo, (addi)
- **fibo.mrh**: stor, vget, push, lt, jz, ret, ((qret)), sub, call, add, vminfo, (lti), (reti), (subi), (subcall), (addret)

Following threading models are implemented:
- switch based threading
- indirect threading
- replicated switch threading

# Other

## File Extensions

| extension | alternative | info                       |
| --------- | ----------- | -------------------------- |
| .mra      | .moa        | Morty assembly             |
| .mrb      | .mob        | Morty binary executable    |
| .mrh      | .moh        | Morty high-level assembly  |
| .mro      | .moo        | Morty optimized executable |
| .mrp      | .mop        | Morty portable executable  |
| .mrt      | .mo         | Morty source code          |
