# TCL File Generated by Component Editor 12.1sp1
# Tue Mar 21 10:47:27 MDT 2017
# DO NOT MODIFY


# 
# servo "servo" v1.0
# null 2017.03.21.10:47:27
# 
# 

# 
# request TCL package from ACDS 12.1
# 
package require -exact qsys 12.1


# 
# module servo
# 
set_module_property NAME servo
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property DISPLAY_NAME servo
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property ANALYZE_HDL AUTO
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false


# 
# file sets
# 
add_fileset QUARTUS_SYNTH QUARTUS_SYNTH "" ""
set_fileset_property QUARTUS_SYNTH TOP_LEVEL servo
set_fileset_property QUARTUS_SYNTH ENABLE_RELATIVE_INCLUDE_PATHS false
add_fileset_file servo.vhd VHDL PATH servo.vhd


# 
# parameters
# 


# 
# display items
# 


# 
# connection point clock
# 
add_interface clock clock end
set_interface_property clock clockRate 0
set_interface_property clock ENABLED true

add_interface_port clock clk clk Input 1


# 
# connection point conduit_end_0
# 
add_interface conduit_end_0 conduit end
set_interface_property conduit_end_0 associatedClock clock
set_interface_property conduit_end_0 associatedReset reset_n
set_interface_property conduit_end_0 ENABLED true

add_interface_port conduit_end_0 coe_gpio0 export Output 1


# 
# connection point s0
# 
add_interface s0 avalon end
set_interface_property s0 addressUnits WORDS
set_interface_property s0 associatedClock clock
set_interface_property s0 associatedReset reset_n
set_interface_property s0 bitsPerSymbol 8
set_interface_property s0 burstOnBurstBoundariesOnly false
set_interface_property s0 burstcountUnits WORDS
set_interface_property s0 explicitAddressSpan 0
set_interface_property s0 holdTime 0
set_interface_property s0 linewrapBursts false
set_interface_property s0 maximumPendingReadTransactions 0
set_interface_property s0 readLatency 0
set_interface_property s0 readWaitTime 1
set_interface_property s0 setupTime 0
set_interface_property s0 timingUnits Cycles
set_interface_property s0 writeWaitTime 0
set_interface_property s0 ENABLED true

add_interface_port s0 avs_s0_write_n write_n Input 1
add_interface_port s0 avs_s0_read_n read_n Input 1
add_interface_port s0 avs_s0_readdata readdata Output 32
add_interface_port s0 avs_s0_writedata writedata Input 32
set_interface_assignment s0 embeddedsw.configuration.isFlash 0
set_interface_assignment s0 embeddedsw.configuration.isMemoryDevice 0
set_interface_assignment s0 embeddedsw.configuration.isNonVolatileStorage 0
set_interface_assignment s0 embeddedsw.configuration.isPrintableDevice 0


# 
# connection point reset_n
# 
add_interface reset_n reset end
set_interface_property reset_n associatedClock clock
set_interface_property reset_n synchronousEdges DEASSERT
set_interface_property reset_n ENABLED true

add_interface_port reset_n reset_n reset_n Input 1

