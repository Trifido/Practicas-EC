.section .data
	.macro linea
	 	.int 1,1,1,1
	 # 	.int 2,2,2,2
	 # 	.int 1,2,3,4
	 # 	.int -1,-1,-1,-1
	 # 	.int 0xffffffff,0xffffffff,0xffffffff,0xffffffff
	 # 	.int 0x08000000,0x08000000,0x08000000,0x08000000
	 #	.int 0x10000000,0x20000000,0x40000000,0x80000000
	.endm
lista:	.irpc i,12345678
		linea
	.endr

longlista:	.int (.-lista)/4	# Numero de enteros que se suman
resultado:	.int -1 			# El resultado es un int porque al hacer la media no hace falta más bits


.section .text
_start:	.global _start

	mov $lista, %ebx			# Almacenamos en ebx el contenido lista
	mov longlista, %ecx			# Almacenamos en ecx longlista
	call suma					# Llamamos a la función suma

	mov %eax, resultado	 		# Movemos la primera parte de la suma a resultado
	mov %edx, resultado+4  		# Movemos el resto de la suma a resultado a la parte alta ya que es little-endian
	


	mov $1, %eax				# Le introducimos al registro eax el valor 1
	mov $0, %ebx				# Le introducimos al registro ebx el valor 0
	int $0x80					# Llamamos a exit(0)

suma: 
	push %esi     				# Guardamos el posible contenido anterior de %esi, %ebp y %edi
	push %ebp	
	push %edi	
	mov $0, %eax  				#Iniciamos a 0 todos los registros
	mov $0, %edx
	mov $0, %esi
	mov $0, %ebp
	mov $0, %edi
bucle:		
	add (%ebx,%esi,4), %eax  	# De la lista vamos añadiendo en eax el número correspondiente
	cdq							# Extension de signo
	add %eax, %ebp	       		# Sumamos la parte baja del número en %ebp	
	adc %edx, %edi		 		# Sumamos la parte alta del número y el acarreo en %edi
	mov $0, %eax		 		# Limpiamos %eax y %edx para una posible siguiente iteracion
	mov $0, %edx             	
	inc %esi					# Incrementamos el contador

	cmp %esi,%ecx				# Comparamos para ver si ha llegado al final el bucle
	jne bucle					# Si no ha llegado al final saltamos de nuevo al inicio del bucle
	
	mov %ebp, %eax				# Si se terminase el bucle, movemos el resultado guardado en %ebp a %eax
	mov %edi, %edx		 		# y el %edi a %edx
	cdq			 				# Extensión de signo
	idiv %ecx		 			# Hacemos idiv de %ecx, que es la longitud de la lista
	pop %ebp					# Restauramos el valor de %ebp
	pop %edi 					# Restauramos el valor de %edi
	pop %esi					# Restauramos el valor del registro contador
	ret
