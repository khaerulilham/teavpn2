// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2021  Alviro Iskandar Setiawan <alviro.iskandar@gmail.com>
 */

#ifndef TEAVPN2__GUI__EVENT_CALLBACKS_H
#define TEAVPN2__GUI__EVENT_CALLBACKS_H

#include <gtk/gtk.h>
#include <teavpn2/mutex.h>

enum event_type {
	CLIENT_ON_CONNECT,
	CLIENT_ON_ERROR,
	CLIENT_ON_DISCONNECT
};

extern void *client_udata;
extern void (*client_on_connect)(void);
extern void (*client_on_error)(int code);
extern void (*client_on_disconnect)(void);
extern void init_client_callbacks(void *data);
extern gboolean client_callback_event_loop(void *data);
extern enum event_type client_event_type;
extern struct tmutex event_mutex;

#ifdef CONFIG_GUI

static inline void invoke_client_on_connect(void)
{
	mutex_lock(&event_mutex);
	client_event_type = CLIENT_ON_CONNECT;
	gdk_threads_add_idle(client_callback_event_loop, NULL);
	mutex_unlock(&event_mutex);
}

static inline void invoke_client_on_disconnect(void)
{
	mutex_lock(&event_mutex);
	client_event_type = CLIENT_ON_DISCONNECT;
	gdk_threads_add_idle(client_callback_event_loop, NULL);
	mutex_unlock(&event_mutex);
}

static inline void invoke_client_on_error(int code)
{
	mutex_lock(&event_mutex);
	client_event_type = CLIENT_ON_ERROR;
	gdk_threads_add_idle(client_callback_event_loop, &code);
	mutex_unlock(&event_mutex);
}

#else

static inline void invoke_client_on_connect(void)
{
}

static inline void invoke_client_on_disconnect(void)
{
}

static inline void invoke_client_on_error(int code)
{
	(void) code;
}

#endif

#endif /* #ifndef TEAVPN2__GUI__EVENT_CALLBACKS_H */
