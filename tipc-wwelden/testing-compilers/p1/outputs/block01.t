|index| opcode || op1 | op2 |dest |
|     |        ||  sr |  sr |  sr |  
|0    | loadI  || 128 | -   | r0  |
|1    | load   || r0  | -   | r1  |
|2    | loadI  || 132 | -   | r2  |
|3    | load   || r2  | -   | r3  |
|4    | loadI  || 136 | -   | r4  |
|5    | load   || r4  | -   | r5  |
|6    | mult   || r3  | r5  | r3  |
|7    | add    || r1  | r3  | r1  |
|8    | store  || r1  | r0  | -   |
|9    | output || 128 | -   | -   |
