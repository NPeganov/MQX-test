#-----------------------------------------------------------
# libraries
#-----------------------------------------------------------
ifeq ($(CONFIG),debug)
LIBRARIES += $(MQX_ROOTDIR)/lib/twrk21f120m.iar/debug/bsp/bsp.a
LIBRARIES += $(MQX_ROOTDIR)/lib/twrk21f120m.iar/debug/psp/psp.a
endif
ifeq ($(CONFIG),release)
LIBRARIES += $(MQX_ROOTDIR)/lib/twrk21f120m.iar/release/bsp/bsp.a
LIBRARIES += $(MQX_ROOTDIR)/lib/twrk21f120m.iar/release/psp/psp.a
endif


#-----------------------------------------------------------
# runtime libraries
#-----------------------------------------------------------


#-----------------------------------------------------------
# runtime library paths
#-----------------------------------------------------------


#-----------------------------------------------------------
# search paths
#-----------------------------------------------------------
ifeq ($(CONFIG),debug)
INCLUDE += $(MQX_ROOTDIR)/lib/twrk21f120m.iar/debug/bsp/Generated_Code
INCLUDE += $(MQX_ROOTDIR)/lib/twrk21f120m.iar/debug/bsp/Sources
INCLUDE += $(MQX_ROOTDIR)/lib/twrk21f120m.iar/debug
INCLUDE += $(MQX_ROOTDIR)/lib/twrk21f120m.iar/debug/bsp
INCLUDE += $(MQX_ROOTDIR)/lib/twrk21f120m.iar/debug/psp
endif
ifeq ($(CONFIG),release)
INCLUDE += $(MQX_ROOTDIR)/lib/twrk21f120m.iar/release/bsp/Generated_Code
INCLUDE += $(MQX_ROOTDIR)/lib/twrk21f120m.iar/release/bsp/Sources
INCLUDE += $(MQX_ROOTDIR)/lib/twrk21f120m.iar/release
INCLUDE += $(MQX_ROOTDIR)/lib/twrk21f120m.iar/release/bsp
INCLUDE += $(MQX_ROOTDIR)/lib/twrk21f120m.iar/release/psp
endif


#-----------------------------------------------------------
# runtime search paths
#-----------------------------------------------------------





