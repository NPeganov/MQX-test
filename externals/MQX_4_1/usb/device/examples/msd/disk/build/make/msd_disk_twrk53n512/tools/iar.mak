#-----------------------------------------------------------
# libraries
#-----------------------------------------------------------
ifeq ($(CONFIG),debug)
LIBRARIES += $(MQX_ROOTDIR)/lib/twrk53n512.iar/debug/bsp/bsp.a
LIBRARIES += $(MQX_ROOTDIR)/lib/twrk53n512.iar/debug/psp/psp.a
LIBRARIES += $(MQX_ROOTDIR)/lib/twrk53n512.iar/debug/usb/usbd.a
endif
ifeq ($(CONFIG),release)
LIBRARIES += $(MQX_ROOTDIR)/lib/twrk53n512.iar/release/bsp/bsp.a
LIBRARIES += $(MQX_ROOTDIR)/lib/twrk53n512.iar/release/psp/psp.a
LIBRARIES += $(MQX_ROOTDIR)/lib/twrk53n512.iar/release/usb/usbd.a
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
INCLUDE += $(MQX_ROOTDIR)/lib/twrk53n512.iar/debug/bsp/Generated_Code
INCLUDE += $(MQX_ROOTDIR)/lib/twrk53n512.iar/debug/bsp/Sources
INCLUDE += $(MQX_ROOTDIR)/lib/twrk53n512.iar/debug
INCLUDE += $(MQX_ROOTDIR)/lib/twrk53n512.iar/debug/bsp
INCLUDE += $(MQX_ROOTDIR)/lib/twrk53n512.iar/debug/psp
INCLUDE += $(MQX_ROOTDIR)/lib/twrk53n512.iar/debug/usb
endif
ifeq ($(CONFIG),release)
INCLUDE += $(MQX_ROOTDIR)/lib/twrk53n512.iar/release/bsp/Generated_Code
INCLUDE += $(MQX_ROOTDIR)/lib/twrk53n512.iar/release/bsp/Sources
INCLUDE += $(MQX_ROOTDIR)/lib/twrk53n512.iar/release
INCLUDE += $(MQX_ROOTDIR)/lib/twrk53n512.iar/release/bsp
INCLUDE += $(MQX_ROOTDIR)/lib/twrk53n512.iar/release/psp
INCLUDE += $(MQX_ROOTDIR)/lib/twrk53n512.iar/release/usb
endif


#-----------------------------------------------------------
# runtime search paths
#-----------------------------------------------------------





