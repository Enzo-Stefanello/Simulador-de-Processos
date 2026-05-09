.code
LOAD variable
loop: SUB #1
SYSCALL 1
BRPOS loop
SYSCALL 0
.endcode

.data
variable 3
.enddata