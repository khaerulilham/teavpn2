// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2021  Khaerul Ilham <khaerulilham163@gmail.com>
 */

#include <teavpn2/gui/common.h>
#include <teavpn2/gui/gui_app.h>


int gui_entry(int argc, char *argv[])
{
	return g_application_run(G_APPLICATION(gui_app_new()), argc, argv);
}
