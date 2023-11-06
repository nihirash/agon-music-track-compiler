MAX_ARGS: EQU 1
    include "crt.inc"

TRACK_INIT:     EQU $90000

show_banner:
    ld hl, banner
    jp printZ

_main:
    ld a, (argc)
    and a
    jr z, show_banner

    ;; Copy player
    ld hl, resident
    ld de, $90000
    ld bc, player_len
    ldir

    ld hl, (argv)
    ld de, track
    ld bc, $ffff-player_len
    MOSCALL mos_load

    call.lil TRACK_INIT

    ret

resident:
    incbin "resident-part.bin"
end_of_resident:

player_len: EQU end_of_resident-resident

track: EQU $90000+player_len

banner:
    db 13,10
    db "Usage: ", 13, 10
    db "bgplay <track.bin>", 13, 10, 0