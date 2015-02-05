#-----------------------------------------------------------
# search paths
#-----------------------------------------------------------
ifeq ($(CONFIG),debug)
INCLUDE += $(MQX_ROOTDIR)/mqx/source/psp/cortex_a/compiler/iar
endif
ifeq ($(CONFIG),release)
INCLUDE += $(MQX_ROOTDIR)/mqx/source/psp/cortex_a/compiler/iar
endif




#-----------------------------------------------------------
# iar sources
#-----------------------------------------------------------
SOURCES += $(MQX_ROOTDIR)/mqx/source/psp/cortex_a/compiler/iar/comp.c
SOURCES += $(MQX_ROOTDIR)/mqx/source/psp/cortex_a/compiler/iar/kernel_symbols.S


#-----------------------------------------------------------
# 'debug' configuration settings
#-----------------------------------------------------------
ifeq ($(CONFIG),debug)
AS_DEFINE += PSP_HAS_SUPPORT_STRUCT=1 
CC_DEFINE += MQX_DISABLE_CONFIG_CHECK=1 PSP_HAS_SUPPORT_STRUCT=1 
CX_DEFINE += MQX_DISABLE_CONFIG_CHECK=1 PSP_HAS_SUPPORT_STRUCT=1 
endif


#-----------------------------------------------------------
# 'release' configuration settings
#-----------------------------------------------------------
ifeq ($(CONFIG),release)
AS_DEFINE += PSP_HAS_SUPPORT_STRUCT=1 
CC_DEFINE += MQX_DISABLE_CONFIG_CHECK=1 PSP_HAS_SUPPORT_STRUCT=1 
CX_DEFINE += MQX_DISABLE_CONFIG_CHECK=1 PSP_HAS_SUPPORT_STRUCT=1 
endif

