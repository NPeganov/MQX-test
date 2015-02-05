#-----------------------------------------------------------
# libraries
#-----------------------------------------------------------
ifeq ($(CONFIG),debug)
LIBRARIES += $(MQX_ROOTDIR)/lib/twrk70f120m.iar/debug/bsp/bsp.a
LIBRARIES += $(MQX_ROOTDIR)/lib/twrk70f120m.iar/debug/psp/psp.a
LIBRARIES += $(MQX_ROOTDIR)/lib/twrk70f120m.iar/debug/rtcs/rtcs.a
LIBRARIES += $(MQX_ROOTDIR)/lib/twrk70f120m.iar/debug/usb/usbd.a
endif
ifeq ($(CONFIG),release)
LIBRARIES += $(MQX_ROOTDIR)/lib/twrk70f120m.iar/release/bsp/bsp.a
LIBRARIES += $(MQX_ROOTDIR)/lib/twrk70f120m.iar/release/psp/psp.a
LIBRARIES += $(MQX_ROOTDIR)/lib/twrk70f120m.iar/release/rtcs/rtcs.a
LIBRARIES += $(MQX_ROOTDIR)/lib/twrk70f120m.iar/release/usb/usbd.a
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
INCLUDE += $(MQX_ROOTDIR)/lib/twrk70f120m.iar/debug/bsp/Generated_Code
INCLUDE += $(MQX_ROOTDIR)/lib/twrk70f120m.iar/debug/bsp/Sources
INCLUDE += $(MQX_ROOTDIR)/lib/twrk70f120m.iar/debug
INCLUDE += $(MQX_ROOTDIR)/lib/twrk70f120m.iar/debug/bsp
INCLUDE += $(MQX_ROOTDIR)/lib/twrk70f120m.iar/debug/psp
INCLUDE += $(MQX_ROOTDIR)/lib/twrk70f120m.iar/debug/rtcs
INCLUDE += $(MQX_ROOTDIR)/lib/twrk70f120m.iar/debug/usb
endif
ifeq ($(CONFIG),release)
INCLUDE += $(MQX_ROOTDIR)/lib/twrk70f120m.iar/release/bsp/Generated_Code
INCLUDE += $(MQX_ROOTDIR)/lib/twrk70f120m.iar/release/bsp/Sources
INCLUDE += $(MQX_ROOTDIR)/lib/twrk70f120m.iar/release
INCLUDE += $(MQX_ROOTDIR)/lib/twrk70f120m.iar/release/bsp
INCLUDE += $(MQX_ROOTDIR)/lib/twrk70f120m.iar/release/psp
INCLUDE += $(MQX_ROOTDIR)/lib/twrk70f120m.iar/release/rtcs
INCLUDE += $(MQX_ROOTDIR)/lib/twrk70f120m.iar/release/usb
endif


#-----------------------------------------------------------
# runtime search paths
#-----------------------------------------------------------





