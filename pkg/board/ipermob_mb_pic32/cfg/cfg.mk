ifeq ($(findstring __IPERMOB_MB_PIC32__,$(EEOPT)) , __IPERMOB_MB_PIC32__)

ifeq ($(findstring __USE_BUTTONS__,$(EEOPT)) , __USE_BUTTONS__)
#EE_SRCS += pkg/board/ipermob_mb_pic32/src/ee_board.c
endif

endif # __IPERMOB_MB_PIC32__