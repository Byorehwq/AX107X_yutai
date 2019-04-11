#这是 for IAR 7.2 C51 的 Make处理流程, Date: 2018/06/16 Merlin Ho

# 需要编译的程式来源目录
SRC_DIR_LIST = . api common config fat inc key mem module rtc task user spi
SRC_DIR_LIST += display display/led display/theme_default
# 此处已移除 display/lcd

# 不列入编译的程式清单
SKIP_LIST = fm/sp3777.c fm/cl6017s.c fm/cl6017g.c fm/ar1019.c fm/ar1010.c
SKIP_LIST += display/theme_default/theme_alarm.c
SKIP_LIST += key/ir.c key/kedir.c key/kedir_all.c key/kedir_ir_led.c key/kedir_key.c key/kedir_key_ir.c key/kedir_key_led.c
SKIP_LIST += mem/mem_device.c mem/mem_eeprom.c mem/mem_internal.c
SKIP_LIST += task/task_alarm.c task/task_aux.c task/task_fm.c task/task_fmam_detect.c task/task_speaker.c
SKIP_LIST += display/theme_default/theme_fm.c key/touch_key.c

# 检查使用的MCU类型, 并设定编译环境与使用的库文件
# MCU_Item的设定, 请在 SetPath.bat 内进行设定
ifeq ($(MCU_Item),AX107X_TAG)
C51_OPTION = -D$(MCU_Item)
XLINK_LIB = lib_ax107x.r51
endif
ifeq ($(MCU_Item),AX207X_TAG)
C51_OPTION = -D$(MCU_Item) --debug
XLINK_LIB = lib_ax207x.r51
endif
ifeq ($(MCU_Item),AX207XB_TAG)
C51_OPTION = -D$(MCU_Item) --debug
XLINK_LIB = lib_ax207xb.r51
endif


# Project输出的目录指定
ifndef BUILD_DIR
# 如果在系统环境没有设定 BUILD_DIR时, 才使用下面的目录进行操作
# 此程式在调用前, 需先运行 SetPath.bat 并在里面用 SET BUILD_DIR=DebugBC
#BUILD_DIR = DebugBC
endif

OBJ_DIR = $(BUILD_DIR)/Obj
EXE_DIR = $(BUILD_DIR)\Exe
LST_DIR = $(BUILD_DIR)/List
DEPS_DIR = $(BUILD_DIR)/Deps

#个别指定 *.c 与 *.h 的搜索来源目录
vpath %.c $(SRC_DIR_LIST)
vpath %.h $(SRC_DIR_LIST)

#建立编译的 .c 清单
SOURCE = $(sort $(filter-out $(SKIP_LIST),$(foreach dir, $(SRC_DIR_LIST), $(wildcard $(dir)/*.c))))

# 建立 .obj 与 .d 的清单
OBJS   = $(addprefix $(OBJ_DIR)/,$(patsubst %.c,%.r51,$(notdir $(SOURCE))))
DEPS   = $(addprefix $(DEPS_DIR)/,$(patsubst %.c,%.d,$(notdir $(SOURCE))))

all: $(EXE_DIR)/app.bin

$(EXE_DIR)/app.bin: $(DEPS) $(OBJS)
#	$(EXE_DIR)\pre.bat
	rm -f $(EXE_DIR)/app.*
	@echo -A $(OBJS) > $(LST_DIR)/link_List.xcl
	xlink $(XLINK_LIB) -f $(LST_DIR)/link_List.xcl -f option_xlink51.cfg -I$(OBJ_DIR) -o $(EXE_DIR)/app.dbg -l $(LST_DIR)/app.map
#	rm $(LST_DIR)/link_List.xcl
	$(EXE_DIR)\make.bat
	xcopy /Y "$(EXE_DIR)\app.hex" "$(TOOLS_PATH)\..\AXIDE V1.1.3\"
	@echo app.hex Copy from "$(ROOT_PATH)$(EXE_DIR)\app.hex" > "$(TOOLS_PATH)\..\AXIDE V1.1.3\app_source.txt"

$(OBJS): $(OBJ_DIR)/%.r51 : %.c $(DEPS_DIR)/%.d
	icc8051 -f option_c51.cfg $(C51_OPTION) -lC $(LST_DIR) -o $@ $<

$(DEPS): $(DEPS_DIR)/%.d: %.c
	icc8051 -f option_c51.cfg $(C51_OPTION) --dependencies=m $@ -lC $(LST_DIR) -o $(OBJ_DIR)/$(basename $(notdir $@)).r51  $<
#	使用 RemovePath.pl 目的是将 IAR产生的完整路径进行当前路径的去除
#	perl $(TOOLS_PATH)/RemoveIAR.d_Path.pl '$@' '$(ROOT_PATH)'
	RemoveIAR.d_Path.exe '$@' '$(ROOT_PATH)'

# 将所有的 .d文件引入，作用为了更新~
-include $(DEPS)

clean:
	rm -f $(DEPS)/*.* $(LST_DIR)/*.* $(OBJS)/*.* $(EXE_DIR)/app.*

.PHONY: clean all
