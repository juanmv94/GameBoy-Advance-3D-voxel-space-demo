path=C:\Softwares\devkitARM\bin
REM arm-none-eabi-gcc -Ofast -mcpu=arm7tdmi -mtune=arm7tdmi -marm -c color.c
REM arm-none-eabi-gcc -Ofast -mcpu=arm7tdmi -mtune=arm7tdmi -marm -c prof.c
REM arm-none-eabi-gcc -Ofast -mcpu=arm7tdmi -mtune=arm7tdmi -marm -c matrizalturas.c
REM arm-none-eabi-gcc -Ofast -mcpu=arm7tdmi -mtune=arm7tdmi -marm -c matrizx.c
arm-none-eabi-gcc -Ofast -mcpu=arm7tdmi -mtune=arm7tdmi -marm -c ex.c
REM arm-none-eabi-gcc -mcpu=arm7tdmi -mtune=arm7tdmi -marm -mthumb -mthumb-interwork -c ex.c
arm-none-eabi-gcc -specs=gba.specs ex.o color.o prof.o matrizalturas.o matrizx.o -o juanmv94demo.elf
arm-none-eabi-objcopy -O binary juanmv94demo.elf juanmv94demo.gba
gbafix juanmv94demo.gba