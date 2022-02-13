org 0x00
jmp main_work
org 0x1B
jmp INTT1

status data 20h
timer data 21h
sred data 30h
outcnt data 31h
intcnt data 32h
sred2 data 33h

INTT1:
	push ACC
POMT1:
	jnb TF1, POMT1
	clr TF1
	djnz outcnt, POMT1
	mov outcnt, #0FAh
	djnz intcnt, POMT1
	mov intcnt, #14h
	setb timer.0
ENDINTT1:
	pop ACC
reti

INITT1:
	setb EA ;enable all
	setb ET1 ;enable za timer1 prekid
	mov TMOD, #20h ;8-bitni auto reload mod
	mov timer, #00h	;pomocna promenljiva za brojac
	mov TH1, #48h ;podesavanje TH1 za auto reload, TL1 posle prvog kraja preuzima automatski vrednost sa TH1
	mov TL1, #48h ;72 -> 256 je 184, tj 200uS
	setb TR1 ;pokrece timer1 da radi
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
	mov intcnt, #14h
	mov outcnt, #0FAh
    mov sred2, #09h
	lcall INITT1
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
	sjmp main

pali:
	jnb timer.0, main ;timer prekid
	clr timer.0
   	mov P2, #0FFh
	sjmp gasi
gasi:
	jnb timer.0, gasi ;timer prekid
	clr timer.0
	mov P2, #00h
	sjmp main
init_levo:
	jb status.1, pom_levo
	mov P2, #01h
	setb status.1
	sjmp main
pom_levo:
	jnb timer.0, main ;timer prekid
	clr timer.0
	mov A, P2
	rl A
	mov P2, A
	sjmp main
pom_sred:
	jnb timer.0, main ;timer prekid
	clr timer.0
	jnb status.2, init_sred
	mov DPTR, #state
	mov A, sred
	movc A, @A+DPTR
	mov P2, A
	mov A, sred
	jz reset_sred
	dec sred
	sjmp main
init_sred:
	setb status.2
	mov sred, #06h
	sjmp main
reset_sred:
	clr status.2
	sjmp main
pom_sred2:
	jnb timer.0, main ;timer prekid
	clr timer.0
	jnb status.2, init_sred2
	mov DPTR, #state
	mov A, sred2
	movc A, @A+DPTR
	mov P2, A
	mov A, sred2
	jz reset_sred2
	dec sred2
	sjmp main
init_sred2:
	setb status.2
	mov sred2, #09h
	sjmp main
reset_sred2:
	clr status.2
	sjmp main
end