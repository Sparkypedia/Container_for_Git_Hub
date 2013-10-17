to generate hex file. edit make file to pick up correct sources then

make xyz.hex

cable from  http://elm-chan.org/works/avrx/avrx_lpt.png

1- MISO
2- VCC
3- SCK
4- MOSI
5- RESET
6- gnd

programming with avrdude
 set programmer type/part/filename and reset after prog status..

avrdude  -c stk200 -p m32 -U flash:w:xyz.hex -E noreset 


NOTE: if chip ID is broken add a -F to the command line to force write.
