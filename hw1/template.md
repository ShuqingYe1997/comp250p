### Function invocation

Report your measured cycles number instead of X:

```
func invocation cycles:13
```

### Addition instruction

Provide a short explanation for assembly code generated for both loop and op measurements
```
 mov     DWORD PTR [rbp-4], 0
        jmp     .L2
.L3:
        mov     edx, DWORD PTR [rbp-8]
        mov     eax, DWORD PTR [rbp-4]
        add     eax, edx
        mov     DWORD PTR [rbp-8], eax
        add     DWORD PTR [rbp-4], 1
.L2:
        mov     eax, DWORD PTR [rbp-4]
        cdqe
        cmp     rax, 99999999
        jbe     .L3

In loop, addition is measured to take 3 cycles. add the value in edx to eax, increment i and compare i to 99999999.  
In op, execute the add instruction 10000 times and it involves zero overhead.
```

Report your measured cycles number instead of X:

```
add loop cycles: 6
add op cycles: 5
```

### Division

Provide a short explanation for assembly code generated for both loop and op measurements
```
 mov     DWORD PTR [rbp-4], 0
        jmp     .L2
.L3:
        mov     eax, DWORD PTR [rbp-8]
        mov     ecx, DWORD PTR [rbp-8]
        cdq
        idiv    ecx
        mov     DWORD PTR [rbp-8], eax
        add     DWORD PTR [rbp-4], 1
.L2:
        mov     eax, DWORD PTR [rbp-4]
        cdqe
        cmp     rax, 99999999

In loop, division is measured to take 3 cycles: divide value in ecx, increment i and compare i to 99999999.    
In op, execute the divide instruction 10000 times and it involves zero overhead.
```
Report your measured cycles number instead of X:

```
div loop cycles: 28 
div op cycles: 23
```

### Multiplication

Provide a short explanation for assembly code generated for both loop and op measurements  
```
.L3:
        mov     edx, DWORD PTR [rbp-8]
        mov     eax, DWORD PTR [rbp-8]
        imul    eax, edx
        mov     DWORD PTR [rbp-8], eax
        add     DWORD PTR [rbp-4], 1
.L2:
        mov     eax, DWORD PTR [rbp-4]
        cdqe
        cmp     rax, 99999999
        jbe     .L3

In loop, multiplication is measured to take 3 cycles: multiply valus in eax and edx, increment i and compare i to 99999999.   
In op, execute the multiply instruction 10000 times and it involves zero overhead.
```
Report your measured cycles number instead of X:

```
mul loop cycles: 9
mul op cycles: 6
```

### Malloc

Provide a short explanation for assembly code generated for the loop measurement
```
        mov     DWORD PTR [rbp-4], 0
        jmp     .L2
.L3:
        mov     edi, 4
        call    malloc
        mov     rdx, rax
        mov     eax, DWORD PTR [rbp-4]
        cdqe
        mov     QWORD PTR [rbp-80016+rax*8], rdx
        add     DWORD PTR [rbp-4], 1
.L2:
        mov     eax, DWORD PTR [rbp-4]
        cmp     eax, 9999
        jbe     .L3

In loop, malloc is measured to take 3 cycles: move the value in rdx to PTR [rbp-80016+rax*8], increment i and compare i to 9999  
```

Report your measured cycles number instead of X:

```
malloc loop cycles: 142
```

### Copy 

Provide a short explanation for assembly code generated for the loop measurement
```
        mov     DWORD PTR [rbp-8], 0
        jmp     .L4
.L7:
        mov     DWORD PTR [rbp-4], 0
        jmp     .L5
.L6:
        mov     eax, DWORD PTR [rbp-4]
        cdqe
        movzx   edx, BYTE PTR [rbp-272+rax]
        mov     eax, DWORD PTR [rbp-4]
        cdqe
        mov     BYTE PTR [rbp-528+rax], dl
        add     DWORD PTR [rbp-4], 1
.L5:
        mov     eax, DWORD PTR [rbp-4]
        cmp     eax, 255
        jbe     .L6
        add     DWORD PTR [rbp-8], 1
.L4:
        mov     eax, DWORD PTR [rbp-8]
        cdqe
        cmp     rax, 99999999
        jbe     .L7

In loop, copying is measured to take 3 cycles, each cycle taks 3 steps: copy from source to destination, increment i and compare i to 99999999.  
```
Report your measured cycles number instead of X:

```
copy loop cycles: 1989
```

### memcpy

Provide a short explanation for assembly code generated for the loop measurement
```
In loop, memcpy is measured to take 3 cycles.
```

Report your measured cycles number instead of X:

```
memcpy loop cycles: 30
```