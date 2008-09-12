#ifndef _TSLIB_H_
#define _TSLIB_H_
/*
 *  tslib/src/tslib.h
 *
 *  Copyright (C) 2001 Russell King.
 *
 * This file is placed under the LGPL.
 *
 * $Id: tslib.h,v 1.2 2002/06/17 17:21:43 dlowder Exp $
 *
 * Touch screen library interface definitions.
 */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include <stdarg.h>
#include <sys/time.h>

struct tsdev;

struct ts_sample {
	int		x;
	int		y;
	unsigned int	pressure;
	struct timeval	tv;
};

/*
 * Close the touchscreen device, free all resources.
 */
int ts_close(struct tsdev *);

/*
 * Configure the touchscreen device.
 */
int ts_config(struct tsdev *);

/*
 * Change this hook to point to your custom error handling function.
 */
extern int (*ts_error_fn)(const char *fmt, va_list ap);

/*
 * Returns the file descriptor in use for the touchscreen device.
 * (not currently implemented)
 */
int ts_fd(struct tsdev *);

/*
 * Load a filter/scaling module
 */
int ts_load_module(struct tsdev *, const char *mod, const char *params);

/*
 * Open the touchscreen device.
 */
struct tsdev *ts_open(const char *dev_name, int nonblock);

/*
 * Open the touchscreen device with event type specified.
 */
struct tsdev *
ts_open_type(const char *dev_name, int nonblock, const char *eventtype);

/*
 * Return a scaled touchscreen sample.
 */
int ts_read(struct tsdev *, struct ts_sample *, int);

/*
 * Returns a raw, unscaled sample from the touchscreen.
 */
int ts_read_raw(struct tsdev *, struct ts_sample *, int);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _TSLIB_H_ */
