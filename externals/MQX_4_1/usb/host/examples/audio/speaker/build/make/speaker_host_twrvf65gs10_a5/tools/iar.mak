#-----------------------------------------------------------
# libraries
#-----------------------------------------------------------
ifeq ($(CONFIG),debug)
LIBRARIES += $(MQX_ROOTDIR)/lib/twrvf65gs10_a5.iar/debug/bsp/bsp.a
LIBRARIES += $(MQX_ROOTDIR)/lib/twrvf65gs10_a5.iar/debug/psp/psp.a
LIBRARIES += $(MQX_ROOTDIR)/lib/twrvf65gs10_a5.iar/debug/mfs/mfs.a
LIBRARIES += $(MQX_ROOTDIR)/lib/twrvf65gs10_a5.iar/debug/shell/shell.a
LIBRARIES += $(MQX_ROOTDIR)/lib/twrvf65gs10_a5.iar/debug/usb/usbh.a
endif
ifeq ($(CONFIG),release)
LIBRARIES += $(MQX_ROOTDIR)/lib/twrvf65gs10_a5.iar/release/bsp/bsp.a
LIBRARIES += $(MQX_ROOTDIR)/lib/twrvf65gs10_a5.iar/release/psp/psp.a
LIBRARIES += $(MQX_ROOTDIR)/lib/twrvf65gs10_a5.iar/release/mfs/mfs.a
LIBRARIES += $(MQX_ROOTDIR)/lib/twrvf65gs10_a5.iar/release/shell/shell.a
LIBRARIES += $(MQX_ROOTDIR)/lib/twrvf65gs10_a5.iar/release/usb/usbh.a
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
INCLUDE += $(MQX_ROOTDIR)/lib/twrvf65gs10_a5.iar/debug/bsp/Generated_Code
INCLUDE += $(MQX_ROOTDIR)/lib/twrvf65gs10_a5.iar/debug/bsp/Sources
INCLUDE += $(MQX_ROOTDIR)/lib/twrvf65gs10_a5.iar/debug
INCLUDE += $(MQX_ROOTDIR)/lib/twrvf65gs10_a5.iar/debug/bsp
INCLUDE += $(MQX_ROOTDIR)/lib/twrvf65gs10_a5.iar/debug/psp
INCLUDE += $(MQX_ROOTDIR)/lib/twrvf65gs10_a5.iar/debug/mfs
INCLUDE += $(MQX_ROOTDIR)/lib/twrvf65gs10_a5.iar/debug/shell
INCLUDE += $(MQX_ROOTDIR)/lib/twrvf65gs10_a5.iar/debug/usb
endif
ifeq ($(CONFIG),release)
INCLUDE += $(MQX_ROOTDIR)/lib/twrvf65gs10_a5.iar/release/bsp/Generated_Code
INCLUDE += $(MQX_ROOTDIR)/lib/twrvf65gs10_a5.iar/release/bsp/Sources
INCLUDE += $(MQX_ROOTDIR)/lib/twrvf65gs10_a5.iar/release
INCLUDE += $(MQX_ROOTDIR)/lib/twrvf65gs10_a5.iar/release/bsp
INCLUDE += $(MQX_ROOTDIR)/lib/twrvf65gs10_a5.iar/release/psp
INCLUDE += $(MQX_ROOTDIR)/lib/twrvf65gs10_a5.iar/release/mfs
INCLUDE += $(MQX_ROOTDIR)/lib/twrvf65gs10_a5.iar/release/shell
INCLUDE += $(MQX_ROOTDIR)/lib/twrvf65gs10_a5.iar/release/usb
endif


#-----------------------------------------------------------
# runtime search paths
#-----------------------------------------------------------





