1. Install cmake
2. Download pico sdk from > https://github.com/raspberrypi/pico-sdk.git
3. Create build dir 
4. Inside build dir >export sdk path > $export  PICO_SDK_PATH=/home/usr/....
5. Run cmake ..
7. Run make
8. Copy and paste the uf2 file > cp blink.uf2 /media/black/RPI-RP2/

There you go! the source codes can be changed from from blink.c file