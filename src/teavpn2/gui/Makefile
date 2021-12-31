#
# SPDX-License-Identifier: GPL-2.0-only
#
# @author Alviro Iskandar Setiawan <alviro.iskandar@gmail.com>
# @license GPL-2.0-only
#
# Copyright (C) 2021  Alviro Iskandar Setiawan
#

DEP_DIRS += $(BASE_DEP_DIR)/src/teavpn2/gui
GUI_DIR := $(BASE_DIR)/src/teavpn2/gui

GUI_RESOURCES := \
	$(GUI_DIR)/gui_app.gresources.xml \
	$(GUI_DIR)/ui/window.ui \
	$(GUI_DIR)/ui/menu.ui

OBJ_TMP_CC := \
	$(GUI_DIR)/entry.o \
	$(GUI_DIR)/event_callback.o \
	$(GUI_DIR)/gui_app.o \
	$(GUI_DIR)/gui_app_window.o \
	$(GUI_DIR)/gui_resources.o

OBJ_PRE_CC += $(OBJ_TMP_CC)

$(GUI_DIR)/gui_resources.c: $(GUI_RESOURCES)
	$(shell pkg-config --variable glib_compile_resources gio-2.0) \
	$(GUI_DIR)/gui_app.gresources.xml --target $(@) \
	--sourcedir $(GUI_DIR) --generate-source

$(OBJ_TMP_CC): $(GUI_DIR)/gui_resources.c
	$(CC_PRINT)
	$(Q)$(CC) $(PIE_FLAGS) $(DEPFLAGS) $(CFLAGS) -c $(O_TO_C) -o $(@)

