# Features:
* On reset execute bootloader, check if you receive.
* Rust webserver running on device.
* Select device and binary to flash.
* Device verifies binary somehow before actually writing it into program memory.
* Be fail-safe: in case of error, use old program.

# Learnings:
* Linker file
* CMake
* GDB
* Secure communication between microcontroller and webserver


# How do I check whether to run the bootloader?
Always run on reset?
# How do I get the linker variables when the binary arrives byte by byte via wifi and I simply copy those bytes into the flash memory???
Can I simply access them after written into the rom????? Are they then just there?????
So the bootrom variables will all be __bootrom_edata, ... ????
# ISR's in bootloader memory section???

# Knowledge:
Smallest possible eraseable element in flash is a sector, would be great if my bootload would fit in one sector.


# TODOs:
* Create CMake build environment - DONE
    * Build basic executable and be able to flash the binary - DONE
    * Include CMSIS - DONE
* Test writing flash sections - DONE

* Test out erasing flash sections
* Write a fake binary to flash, see if the linker variables are there...
* WiFi communication proof of concept
* Send binary byte by byte and write it into the flash
* Add security: TLS via http magic (open-ssl, mbed-tls?????, certificates????, sign binary????)
* Add safety: crc's, resend (happens anyway via TCP (http) ???)
* Write unit tests