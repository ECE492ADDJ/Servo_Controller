#!/bin/sh
#
# This file was automatically generated.
#
# It can be overwritten by nios2-flash-programmer-generate or nios2-flash-programmer-gui.
#

#
# Converting ELF File: /afs/ualberta.ca/home/d/j/djr1/ece492_projects/ece492_projects/Servo_Controller/software/TiltMazeController_nonVolatile/TiltMazeController_nonVolatile.elf to: "../flash/TiltMazeController_nonVolatile_generic_tristate_controller_0.flash"
#
elf2flash --input="/afs/ualberta.ca/home/d/j/djr1/ece492_projects/ece492_projects/Servo_Controller/software/TiltMazeController_nonVolatile/TiltMazeController_nonVolatile.elf" --output="../flash/TiltMazeController_nonVolatile_generic_tristate_controller_0.flash" --boot="$SOPC_KIT_NIOS2/components/altera_nios2/boot_loader_cfi.srec" --base=0x1400000 --end=0x1800000 --reset=0x1400000 --verbose 

#
# Programming File: "../flash/TiltMazeController_nonVolatile_generic_tristate_controller_0.flash" To Device: generic_tristate_controller_0
#
nios2-flash-programmer "../flash/TiltMazeController_nonVolatile_generic_tristate_controller_0.flash" --base=0x1400000 --sidp=0x19090B8 --id=0x0 --timestamp=1491155961 --device=1 --instance=0 '--cable=USB-Blaster on localhost [2-1.6]' --program --verbose 

