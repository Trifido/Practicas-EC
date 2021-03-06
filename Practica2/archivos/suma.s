.section .data

      .macro linea
	 	.int 1,1,1,1
	#	.int 2,2,2,2
	#	.int 1,2,3,4
	#	.int -1,-1,-1,-1
	#	.int 0xfffffffff,0xfffffffff,0xfffffffff,0xfffffffff 
	#	.int 0x08000000,0x08000000,0x08000000,0x08000000 
	#	.int 0x10000000,0x20000000,0x40000000,0x80000000
        .endm
lista: .irpc i,12345678
		linea
	.endr
longlista:	.int (.-lista)/4 	# Numero de enteros que se suman

resultado:	.quad-1				# Obtiene los registros que tienen el resultado en direcciones consecutivas


.section .text
_start:	.global _start

	mov $lista, %ebx		# Almacenamos en ebx el contenido lista
	mov longlista, %ecx		# Almacenamos en ecx longlista
	call suma				# Llamamos a la función suma
	mov %eax, resultado		# Movemos la primera parte de la suma a resultado
        mov %edx, resultado+4		# Movemos el resto de la suma a resultado a la parte alta ya que es little-endian

	mov $1, %eax			# Le introducimos al registro eax el valor 1
	mov $0, %ebx			# Le introducimos al registro ebx el valor 0
	int $0x80				# Llamamos a exit

suma:
	push %esi				# Guardamos el contenido anterior de esi
	mov $0, %eax			# Y le asignamos a los tres registros el valor 0
	mov $0, %edx			# Acumula el acarreo
 	mov $0, %esi			# El registro contador
bucle:
	add (%ebx,%esi,4), %eax		# Vamos sumando el registro que toque a eax
    jnc no_hay_acarreo			# Si no hay acarreo llamamos a no_hay_acarreo
	inc    %edx				# Incrementamos el registro edx si hubiera acarreo
no_hay_acarreo:
    inc    %esi				# Incrementamos el registro contador esi
	cmp  %esi,%ecx			# Comprobamos si el contado ha llegado al final
	jne bucle				# Si esi y ecx no son iguales significa que no hemos llegado al 
							# final de bucle asi que volvemos a llamar a bucle
	pop %esi				# Finalmente si no llama al bucle significa que ha terminado el
							# el bucle por lo que se restaura esi
	ret
