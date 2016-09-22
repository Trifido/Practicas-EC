.data                  # sección de datos
	msg: .string "hola, mundo!\n"
	tam: .int . - msg

.text                  # sección de código
	.globl _start      # punto de entrada

_start:                # etiqueta de entrada
	movl    $4, %eax   # write
	movl    $1, %ebx   # salida estandar
	movl    $msg, %ecx # cadena
	movl    tam, %edx  # longitud
	int     $0x80      # llamada al sistema

	movl    $1, %eax   # exit
	xorl    %ebx, %ebx # 0
	int     $0x80      # llamada al sistema

