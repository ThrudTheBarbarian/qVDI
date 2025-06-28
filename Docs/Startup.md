# Startup 
- Need test app (GemBoot) to emulate GEM in the ROMs starting up
    - Open Physical screen workstation when v_openwk called
    - then wait for other apps to start, and they use v_opnvwk 
    - if v_openwk hasn't been called when v_opnvwk is called, we should call v_openwk first

AUTO program (because we expect GEM to boot using our VDI)
    - or possibly a c: program
    - Insert our own VDI handler, see A_HANDLR.S in XAAES by Craig Graham (in ~/src/atari/ST/Sources/C/Craig Graham) as well as HANDLER.C

Client apps
    - expect start with appl_init(), then graf_handle(), then opn_vwk()


