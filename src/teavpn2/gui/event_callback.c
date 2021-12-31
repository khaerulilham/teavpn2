// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2021  Alviro Iskandar Setiawan <alviro.iskandar@gmail.com>
 */

#undef NDEBUG
#include <gtk/gtk.h>
#include <teavpn2/gui/event_callback.h>

void *client_udata;
struct tmutex event_mutex;
enum event_type client_event_type;

static inline void set_button_connect_prop(const gchar *label, gboolean state)
{
	gtk_button_set_label(GTK_BUTTON(client_udata), label);
	gtk_widget_set_sensitive(GTK_WIDGET(client_udata), state);
}

static void callback_client_on_connect(void)
{
	set_button_connect_prop("Disconnect", TRUE);
}

static void callback_client_on_disconnect(void)
{
	set_button_connect_prop("Connect", TRUE);
}

static void callback_client_on_error(int code)
{
	(void) code;
	set_button_connect_prop("Connect", TRUE);
}


void init_client_callbacks(void *data)
{
	client_udata = data;
	mutex_init(&event_mutex, NULL);
}

gboolean client_callback_event_loop(void *data)
{
	cpu_relax();
	if (mutex_trylock(&event_mutex)) {
		/* Failed to acquire the lock. */
		return FALSE;
	}

	switch (client_event_type) {
	case CLIENT_ON_CONNECT:
		pr_debug("CLIENT_ON_CONNECT");
		callback_client_on_connect();
		break;
	case CLIENT_ON_ERROR:
		pr_debug("CLIENT_ON_ERROR");
		callback_client_on_error(*(int *) data);
		break;
	case CLIENT_ON_DISCONNECT:
		pr_debug("CLIENT_ON_DISCONNECT");
		callback_client_on_disconnect();
		break;
	}

	mutex_unlock(&event_mutex);
	return FALSE;
}
