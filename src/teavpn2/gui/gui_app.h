// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2021  Khaerul Ilham <khaerulilham163@gmail.com>
 */

#ifndef GUI__APP_H
#define GUI__APP_H

#include <gtk/gtk.h>

#define GUI_APP_TYPE (gui_app_get_type())
G_DECLARE_FINAL_TYPE(GuiApp, gui_app, GUI, APP, GtkApplication);

GuiApp *gui_app_new(void);

#endif /* GUI__APP_H */
