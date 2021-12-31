// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2021  Khaerul Ilham <khaerulilham163@gmail.com>
 */

#ifndef GUI__APP__WINDOW_H
#define GUI__APP__WINDOW_H

#include <gtk/gtk.h>
#include <teavpn2/gui/gui_app.h>

#define GUI_APP_WINDOW_TYPE (gui_app_window_get_type())
G_DECLARE_FINAL_TYPE(GuiAppWindow, gui_app_window, GUI, APP_WINDOW,
		     GtkApplicationWindow)


GuiAppWindow *gui_app_window_new(GuiApp *app);
GtkWidget *gui_app_window_get_lbl_cfg(GuiAppWindow *app);
GtkWidget *gui_app_window_get_btn_connect(GuiAppWindow *app);


#endif /* GUI__APP__WINDOW_H */
