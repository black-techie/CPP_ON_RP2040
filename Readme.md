1. Install cmake and install tool chain
2. sudo apt install -y gcc-arm-none-eabi
3. Download pico sdk from > https://github.com/raspberrypi/pico-sdk.git
4. Create build dir 
5. Inside build dir >export sdk path > export  PICO_SDK_PATH=$HOME/pico-sdk
6. Run cmake ..
7. Run make
8. Copy and paste the uf2 file > cp blink.uf2 /media/black/RPI-RP2/

There you go! the source codes can be changed from from main.c file
