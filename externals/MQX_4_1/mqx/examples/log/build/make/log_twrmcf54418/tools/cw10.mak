#-----------------------------------------------------------
# libraries
#-----------------------------------------------------------
ifeq ($(CONFIG),debug)
LIBRARIES += $(MQX_ROOTDIR)/lib/twrmcf54418.cw10/debug/bsp/bsp.a
LIBRARIES += $(MQX_ROOTDIR)/lib/twrmcf54418.cw10/debug/psp/psp.a
endif
ifeq ($(CONFIG),release)
LIBRARIES += $(MQX_ROOTDIR)/lib/twrmcf54418.cw10/release/bsp/bsp.a
LIBRARIES += $(MQX_ROOTDIR)/lib/twrmcf54418.cw10/release/psp/psp.a
endif


#-----------------------------------------------------------
# runtime libraries
#-----------------------------------------------------------


#-----------------------------------------------------------
# runtime library paths
#-----------------------------------------------------------
ifeq ($(CONFIG),debug)
RT_PATHS += $(TOOLCHAIN_ROOTDIR)/MCU/ColdFire_Support/ewl/lib/
endif
ifeq ($(CONFIG),release)
RT_PATHS += $(TOOLCHAIN_ROOTDIR)/MCU/ColdFire_Support/ewl/lib/
endif


#-----------------------------------------------------------
# search paths
#-----------------------------------------------------------
ifeq ($(CONFIG),debug)
INCLUDE += $(MQX_ROOTDIR)/lib/twrmcf54418.cw10/debug/bsp/Generated_Code
INCLUDE += $(MQX_ROOTDIR)/lib/twrmcf54418.cw10/debug/bsp/Sources
INCLUDE += $(MQX_ROOTDIR)/lib/twrmcf54418.cw10/debug
INCLUDE += $(MQX_ROOTDIR)/lib/twrmcf54418.cw10/debug/bsp
INCLUDE += $(MQX_ROOTDIR)/lib/twrmcf54418.cw10/debug/psp
endif
ifeq ($(CONFIG),release)
INCLUDE += $(MQX_ROOTDIR)/lib/twrmcf54418.cw10/release/bsp/Generated_Code
INCLUDE += $(MQX_ROOTDIR)/lib/twrmcf54418.cw10/release/bsp/Sources
INCLUDE += $(MQX_ROOTDIR)/lib/twrmcf54418.cw10/release
INCLUDE += $(MQX_ROOTDIR)/lib/twrmcf54418.cw10/release/bsp
INCLUDE += $(MQX_ROOTDIR)/lib/twrmcf54418.cw10/release/psp
endif


#-----------------------------------------------------------
# runtime search paths
#-----------------------------------------------------------
ifeq ($(CONFIG),debug)
RT_INCLUDE += $(TOOLCHAIN_ROOTDIR)/MCU/ColdFire_Support/ewl/EWL_C/include
RT_INCLUDE += $(TOOLCHAIN_ROOTDIR)/MCU/ColdFire_Support/ewl/EWL_C/include/sys
RT_INCLUDE += $(TOOLCHAIN_ROOTDIR)/MCU/ColdFire_Support/ewl/EWL_Runtime/Runtime_ColdFire/Source
endif
ifeq ($(CONFIG),release)
RT_INCLUDE += $(TOOLCHAIN_ROOTDIR)/MCU/ColdFire_Support/ewl/EWL_C/include
RT_INCLUDE += $(TOOLCHAIN_ROOTDIR)/MCU/ColdFire_Support/ewl/EWL_C/include/sys
RT_INCLUDE += $(TOOLCHAIN_ROOTDIR)/MCU/ColdFire_Support/ewl/EWL_Runtime/Runtime_ColdFire/Source
endif





