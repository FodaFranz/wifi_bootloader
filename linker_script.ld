ENTRY(reset_handler)

_estack = ORIGIN(SRAM) + LENGTH(SRAM);

_Min_Heap_Size = 0x200;
_Min_Stack_Size = 0x400;

MEMORY
{
  SRAM              (xrw)   : ORIGIN = 0x20000000,   LENGTH = 128K
  BOOTROM           (rx)    : ORIGIN = 0x8000000,    LENGTH = 16K
  APPLICATION       (rx)    : ORIGIN = 0x8004000,    LENGTH = 496k
}

__bootrom_start__ = ORIGIN(BOOTROM);
__bootrom_size__ = LENGTH(BOOTROM);
__approm_start__ = ORIGIN(APPLICATION);
__approm_size__ = LENGTH(APPLICATION);

SECTIONS
{
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector))
    . = ALIGN(4);
  } >BOOTROM

  .text :
  {
    . = ALIGN(4);

    *(.text)          
    *(.rodata)      

    . = ALIGN(4);
    _etext = .;        
  } >BOOTROM

  .data :
  {
    . = ALIGN(4);

    _sdata = .;
    *(.data)

    . = ALIGN(4);
    _edata = .;
  } >SRAM AT> BOOTROM

  .bss :
  {
    . = ALIGN(4);

    _sbss = .;
    *(.bss)

    . = ALIGN(4);
    _ebss = .;
  } >SRAM

  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >SRAM
}
