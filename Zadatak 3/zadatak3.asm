org 0x00
jmp main_work

status data 20h
prom1 data 30h
prom2 data 31h
prom3 data 32h
sred data 33h
brzina data 34h
sred2 data 35h

delay:
	mov prom1, #0FFh
	for1:
		mov prom2, #0FFh
	for2:
		mov prom3, brzina
	for3:
		djnz prom3, for3		
		djnz prom2, for2
		djnz prom1, for1
ret

state:
db 81h
db 42h
db 24h
db 18h
db 24h
db 42h
db 81h 

state2:
db 81h
db C0h
db 60h
db 30h
db 0Ch
db 06h
db 03h
db 81h 



main_work:
	mov P2, #00h
	mov status, #00h
	mov sred, #06h
    mov sred2, #08h
	mov brzina, #05h
main:
	jb P0.0, stop
	setb status.0
stop:
	jb P0.1, start
	mov status, #00h
	mov P2, #00h
start:
	jnb status.0, main
	jnb P0.2, pali
	jnb P0.3, init_levo
	jnb P0.4, pom_sred
	jnb P0.5, pom_sred2
	jnb P0.6, brzina_pola
	jnb P0.7, brzina_cetvrt
	sjmp main

pali:
   	mov P2, #0FFh
	lcall delay
	sjmp gasi
gasi:
	mov P2, #00h
	lcall delay
	sjmp main
init_levo:
	jb status.1, pom_levo
	mov P2, #01h
	setb status.1
	lcall delay
	sjmp main
pom_levo:
	mov A, P2
	rl A
	mov P2, A
	lcall delay
	sjmp main
pom_sred:
	jnb status.2, init_sred
	mov DPTR, #state
	mov A, sred
	movc A, @A+DPTR
	mov P2, A
	mov A, sred
	jz reset_sred
	dec sred
	lcall delay
	sjmp main
init_sred:
	setb status.2
	mov sred, #06h
	sjmp main
reset_sred:
	clr status.2
	sjmp main
pom_sred2:
	jnb status.2, init_sred2
	mov DPTR, #state2
	mov A, sred2
	movc A, @A+DPTR
	mov P2, A
	mov A, sred2
	jz reset_sred2
	dec sred2
	lcall delay
	sjmp main
init_sred2:
	setb status.2
	mov sred, #08h
	sjmp main
reset_sred2:
	clr status.2
	sjmp main
brzina_pola:
	mov brzina, #0Ah
	ljmp main
brzina_cetvrt:
	mov brzina, #14h
	ljmp main
end