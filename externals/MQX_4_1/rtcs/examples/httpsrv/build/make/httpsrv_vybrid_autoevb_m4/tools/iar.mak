#-----------------------------------------------------------
# libraries
#-----------------------------------------------------------
ifeq ($(CONFIG),debug)
LIBRARIES += $(MQX_ROOTDIR)/lib/vybrid_autoevb_m4.iar/debug/bsp/bsp.a
LIBRARIES += $(MQX_ROOTDIR)/lib/vybrid_autoevb_m4.iar/debug/psp/psp.a
LIBRARIES += $(MQX_ROOTDIR)/lib/vybrid_autoevb_m4.iar/debug/rtcs/rtcs.a
LIBRARIES += $(MQX_ROOTDIR)/lib/vybrid_autoevb_m4.iar/debug/shell/shell.a
endif
ifeq ($(CONFIG),release)
LIBRARIES += $(MQX_ROOTDIR)/lib/vybrid_autoevb_m4.iar/release/bsp/bsp.a
LIBRARIES += $(MQX_ROOTDIR)/lib/vybrid_autoevb_m4.iar/release/psp/psp.a
LIBRARIES += $(MQX_ROOTDIR)/lib/vybrid_autoevb_m4.iar/release/rtcs/rtcs.a
LIBRARIES += $(MQX_ROOTDIR)/lib/vybrid_autoevb_m4.iar/release/shell/shell.a
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
INCLUDE += $(MQX_ROOTDIR)/lib/vybrid_autoevb_m4.iar/debug/bsp/Generated_Code
INCLUDE += $(MQX_ROOTDIR)/lib/vybrid_autoevb_m4.iar/debug/bsp/Sources
INCLUDE += $(MQX_ROOTDIR)/lib/vybrid_autoevb_m4.iar/debug
INCLUDE += $(MQX_ROOTDIR)/lib/vybrid_autoevb_m4.iar/debug/bsp
INCLUDE += $(MQX_ROOTDIR)/lib/vybrid_autoevb_m4.iar/debug/psp
INCLUDE += $(MQX_ROOTDIR)/lib/vybrid_autoevb_m4.iar/debug/rtcs
INCLUDE += $(MQX_ROOTDIR)/lib/vybrid_autoevb_m4.iar/debug/shell
endif
ifeq ($(CONFIG),release)
INCLUDE += $(MQX_ROOTDIR)/lib/vybrid_autoevb_m4.iar/release/bsp/Generated_Code
INCLUDE += $(MQX_ROOTDIR)/lib/vybrid_autoevb_m4.iar/release/bsp/Sources
INCLUDE += $(MQX_ROOTDIR)/lib/vybrid_autoevb_m4.iar/release
INCLUDE += $(MQX_ROOTDIR)/lib/vybrid_autoevb_m4.iar/release/bsp
INCLUDE += $(MQX_ROOTDIR)/lib/vybrid_autoevb_m4.iar/release/psp
INCLUDE += $(MQX_ROOTDIR)/lib/vybrid_autoevb_m4.iar/release/rtcs
INCLUDE += $(MQX_ROOTDIR)/lib/vybrid_autoevb_m4.iar/release/shell
endif


#-----------------------------------------------------------
# runtime search paths
#-----------------------------------------------------------





