#ifndef __LINUX_ILATENCY_H
#define __LINUX_ILATENCY_H
/*
 * include/linux/ilatency.h
 *
 * Interrupt latency instrumentation
 *
 * Author: dsingleton <dsingleton@mvista.com>
 *
 * 2001-2004 (c) MontaVista Software, Inc. This file is licensed under
 * the terms of the GNU General Public License version 2. This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 */

#ifndef __ASSEMBLY__
#ifdef CONFIG_ILATENCY
extern void interrupt_overhead_start(void);
extern void interrupt_overhead_stop(void);
#else 
#define interrupt_overhead_start()	do { } while(0)
#define interrupt_overhead_stop()	do { } while(0)
#endif
extern void reset_ilat_data(void);
extern void inthoff_logentry(unsigned long);
extern void ilat_irq_enable(char *, unsigned, int);
extern void ilat_restore_flags(char *, unsigned, unsigned long);
extern void ilat_irq_disable(char *, unsigned);
extern void latency_check(void);
extern unsigned long ticks_per_usec;
extern struct ilatency_bucket ilat_data;
extern struct ilatency_log ilat_log;

#define	NUM_LOG_ENTRIES 24
#define ILAT_FILE_SIZE	80
#define ILAT_LINE_SIZE	12
#define ON 		1
#define OFF 		0
#define	BUCKETS		250 
#define BUCKET_SIZE	2
#define UPPER_LIMIT	BUCKETS * BUCKET_SIZE
#define LAST_BUCKET	BUCKETS - 1

struct ilatency_log {
	int logging;
	int sync;
	int which;
	unsigned long cli_ticks;
	unsigned long sti_ticks;
	unsigned int cli_line_no;
	char *cli_file;
	/* error accounting */
	unsigned skip_sti;
	unsigned skip_cli;
	unsigned sti_error;
	unsigned cli_error;
	unsigned sti_break_error;
	unsigned restore_sti;
	unsigned restore_cli;
	struct {
		/* worst blocking time */
		unsigned long interrupts_off;
		char *cli_file;
		unsigned cli_line;
		unsigned long cli_ticks;
		char *sti_file;
		unsigned sti_line;
		unsigned long sti_ticks;
		int multiples;
	} log[NUM_LOG_ENTRIES];
};

struct ilatency_bucket {
	int logging;
	int which;
	unsigned long ilatency_start;
	unsigned long ilatency_stop;
	unsigned long ilatency_delta;
	unsigned long total_samples;
	unsigned long maximum_off;
	unsigned long overhead_start;
	unsigned long overhead_stop;
	unsigned long bucket[BUCKETS];
};
#endif
#endif

