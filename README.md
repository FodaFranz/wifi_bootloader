# wifi_bootloader
hello

## Open questions:
* Being able to debug the application?
* Verify application works:
    * interrupts
    * data section present
    * bss is zero'd
* How to verify transmitted binary is not compromised?
    * Send hash of binary
    * (Signature???)
* Create secure channel
    * mbedTLS ???
        * use lwip tcp stack????
    * Use fuses to verify sender of binaries??
* Protect bootrom with MPU
* How to check wheter to fetch a new binary?
    * Always send a "Is a new bin available message with the current bin version" to the binary provider
* Verify every single message is correct
    * CRC????
    * HTTP resend mechanism???