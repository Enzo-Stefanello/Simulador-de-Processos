.code
LOAD valor
loop: SUB #1
SYSCALL 1
BRPOS loop
SYSCALL 0
.endcode

.data
valor 3
.enddata