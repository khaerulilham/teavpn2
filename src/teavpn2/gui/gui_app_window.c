// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2021  Khaerul Ilham <khaerulilham163@gmail.com>
 */

#include <teavpn2/gui/gui_app.h>
#include <teavpn2/gui/gui_app_window.h>
#include <teavpn2/client/common.h>


struct _GuiAppWindow {
	GtkApplicationWindow parent;

	GtkWidget *menu;
	GtkWidget *notebook;
	GtkWidget *lbl_cfg;
	GtkWidget *btn_connect;
};

G_DEFINE_TYPE(GuiAppWindow, gui_app_window, GTK_TYPE_APPLICATION_WINDOW);


/* Function prototypes */
static void *run_teavpn2(void *user_data);
static void btn_connect_callback(GtkWidget *self, void *user_data);


/* Global (static) variables */


/* Virtual functions */
static void gui_app_window_init(GuiAppWindow *win)
{
	GtkBuilder *builder;
	GMenuModel *menu;

	builder = gtk_builder_new_from_resource("/org/teainside/teavpn2/ui/menu.ui");
	menu = G_MENU_MODEL(gtk_builder_get_object(builder, "menu"));

	gtk_widget_init_template(GTK_WIDGET(win));
	gtk_menu_button_set_menu_model(GTK_MENU_BUTTON(win->menu), menu);

	g_object_unref(builder);

}

static void gui_app_window_constructed(GObject *obj)
{
	GuiAppWindow *win = GUI_APP_WINDOW(obj);

	gtk_label_set_label(GTK_LABEL(win->lbl_cfg), "/etc/teavpn2/client.ini");

	g_signal_connect(win->btn_connect, "clicked",
			 G_CALLBACK(btn_connect_callback), win);

	/* Always chain up to the parent constructed function */
	G_OBJECT_CLASS(gui_app_window_parent_class)->constructed(obj);

}

static void gui_app_window_class_init(GuiAppWindowClass *class)
{
	gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class),
						    "/org/teainside/teavpn2/ui/window.ui");
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class),
					     GuiAppWindow, menu);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class),
					     GuiAppWindow, notebook);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class),
					     GuiAppWindow, lbl_cfg);
	gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class),
					     GuiAppWindow, btn_connect);


	G_OBJECT_CLASS(class)->constructed = gui_app_window_constructed;
}


/* Private functions */
static void *run_teavpn2(void *user_data)
{
	GuiAppWindow *win = GUI_APP_WINDOW(user_data);
	struct cli_cfg cfg = {
		.sys.cfg_file = (char *)gtk_label_get_label(GTK_LABEL(win->lbl_cfg))
	};


	if (client_parse_cfg_file(cfg.sys.cfg_file, &cfg) < 0)
		goto ret0;

	switch (cfg.sock.type) {
	case SOCK_UDP:
		teavpn2_client_udp_run(&cfg);
		goto ret1;
	case SOCK_TCP:
	default:
		pr_error("cfg.sock.type: Not Supported");
	}

ret0:
	gtk_widget_set_sensitive(GTK_WIDGET(win->btn_connect), TRUE);

ret1:
	return NULL;
}


/* Global functions */
GuiAppWindow *gui_app_window_new(GuiApp *app)
{
	return g_object_new(GUI_APP_WINDOW_TYPE, "application", app, NULL);
}

GtkWidget *gui_app_window_get_lbl_cfg(GuiAppWindow *app)
{
	return app->lbl_cfg;
}

GtkWidget *gui_app_window_get_btn_connect(GuiAppWindow *app)
{
	return app->btn_connect;
}


/* Callback handlers */
static void btn_connect_callback(GtkWidget *self, void *user_data)
{
	(void) user_data;

	pthread_t thread;
	const char *btn_label;


	btn_label = gtk_button_get_label(GTK_BUTTON(self));

	gtk_widget_set_sensitive(self, FALSE);
	if (g_strcmp0(btn_label, "Connect") == 0) {
		if (pthread_create(&thread, NULL, run_teavpn2, user_data) != 0) {
			pr_error("pthread");
			return;
		}
		pthread_detach(thread);

	} else if (g_strcmp0(btn_label, "Disconnect") == 0) {
		teavpn2_client_udp_stop();
	}
}
