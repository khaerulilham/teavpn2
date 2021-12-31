// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2021  Khaerul Ilham <khaerulilham163@gmail.com>
 */

#include <teavpn2/gui/gui_app.h>
#include <teavpn2/gui/gui_app_window.h>
#include <teavpn2/gui/event_callback.h>
#include <teavpn2/common.h>
#include <teavpn2/client/common.h>


struct _GuiApp {
	GtkApplication parent;

	GuiAppWindow *window;
	GtkWidget *btn_open;
	GtkWidget *btn_about;
};

G_DEFINE_TYPE(GuiApp, gui_app, GTK_TYPE_APPLICATION);


/* Function prototypes */
static void btn_open_callback(GSimpleAction *act, GVariant *pr, void *app);
static void btn_about_callback(GSimpleAction *act, GVariant *pr, void *app);


/* Global (static) variables */
static GActionEntry menu_entries[] = {
	{ .name = "open", .activate = btn_open_callback },
	{ .name = "about", .activate = btn_about_callback },
};


/* Virtual functions */
static void gui_app_init(GuiApp *app)
{
	(void) app;
}

static void gui_app_startup(GApplication *app)
{
	G_APPLICATION_CLASS(gui_app_parent_class)->startup(app);

	g_action_map_add_action_entries(G_ACTION_MAP(app), menu_entries,
					G_N_ELEMENTS(menu_entries), app);

}

static void gui_app_activate(GApplication *app)
{
	GuiApp *gui_app = GUI_APP(app);

	gui_app->window = gui_app_window_new(gui_app);

	init_client_callbacks(gui_app_window_get_btn_connect(gui_app->window));

	gtk_window_present(GTK_WINDOW(gui_app->window));
}

static void gui_app_class_init(GuiAppClass *class)
{
	G_APPLICATION_CLASS(class)->startup = gui_app_startup;
	G_APPLICATION_CLASS(class)->activate = gui_app_activate;
}


/* Global functions */
GuiApp *gui_app_new(void)
{
	return g_object_new(GUI_APP_TYPE,
			    "application-id", "org.teainside.teavpn2",
			    "flags", G_APPLICATION_FLAGS_NONE, NULL);
}


/* Private functions */


/* Callback handlers */
static void btn_open_callback(GSimpleAction *act, GVariant *pr, void *app)
{
	(void) act;
	(void) pr;

	GtkWindow *win;
	char *f_name;
	GtkFileChooserAction f_action;
	GtkWidget *f_dialog;
	GtkFileChooser *f_chooser;
	GtkFileFilter *f_filter;

	win = GTK_WINDOW(GUI_APP(app)->window);
	f_action = GTK_FILE_CHOOSER_ACTION_OPEN;
	f_filter = gtk_file_filter_new();
	f_dialog = gtk_file_chooser_dialog_new("Open Configuration File",
						win, f_action, "_Cancel",
						GTK_RESPONSE_CANCEL, "_Open",
						GTK_RESPONSE_ACCEPT, NULL);
	f_chooser = GTK_FILE_CHOOSER(f_dialog);

	gtk_file_filter_set_name(f_filter, "Configuration file");
	gtk_file_filter_add_pattern(f_filter, "*.ini");
	gtk_file_chooser_add_filter(f_chooser, f_filter);

	if (gtk_dialog_run(GTK_DIALOG(f_dialog)) == GTK_RESPONSE_ACCEPT) {
		f_name = gtk_file_chooser_get_filename(f_chooser);
		gtk_label_set_label(
			GTK_LABEL(gui_app_window_get_lbl_cfg(GUI_APP_WINDOW(win))),
			f_name
		);

		g_free(f_name);
	}

	gtk_widget_destroy(f_dialog);
}

static void btn_about_callback(GSimpleAction *act, GVariant *pr, void *app)
{
	(void) act;
	(void) pr;

	gtk_show_about_dialog(GTK_WINDOW(GUI_APP(app)->window),
			      "program-name", "TeaVPN2 Client",
			      "version", TEAVPN2_VERSION,
			      "license-type", GTK_LICENSE_GPL_2_0,
			      "website", "https://github.com/teainside/teavpn2",
			      NULL);

}
