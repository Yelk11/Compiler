# addition

popq %eax
add (%esp), %eax
add $4, %esp
pushq %eax
movb (%esp), %cl

# new
movl $0, -4(%rbp)
movl -4(%rbp), %ecx
addl $1, %ecx
movl %ecx, -4(%rbp)