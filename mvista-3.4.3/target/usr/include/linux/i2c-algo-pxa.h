#ifndef _LINUX_I2C_ALGO_PXA_H
#define _LINUX_I2C_ALGO_PXA_H

struct i2c_eeprom_emu;
struct i2c_eeprom_emu_byte;

struct i2c_eeprom_emu_watcher {
	int           (*read)(struct i2c_eeprom_emu *, int addr);
	int           (*write)(struct i2c_eeprom_emu *, int addr, int newval);
};

struct i2c_eeprom_emu_byte {
	unsigned long                  last_modified;
	struct i2c_eeprom_emu_watcher *watcher;
	unsigned char                  val;
};

#define I2C_EEPROM_EMU_SIZE (256)

struct i2c_eeprom_emu {
	int                          size;
	int                          ptr;
	int                          seen_start;

	struct i2c_eeprom_emu_byte   bytes[I2C_EEPROM_EMU_SIZE];
};

typedef enum i2c_slave_event_e {
	I2C_SLAVE_EVENT_NONE,
	I2C_SLAVE_EVENT_START,         /* start */
	I2C_SLAVE_EVENT_STOP
} i2c_slave_event_t;

typedef enum i2c_slave_mode_e {
	I2C_SLAVE_START_READ,
	I2C_SLAVE_START_WRITE
} i2c_slave_mode_t;

struct i2c_slave_client {
	int (*event)(void *pw, i2c_slave_event_t event, int value);
	int (*read) (void *pw);
	int (*write)(void *pw, int val);
};

extern int i2c_eeprom_emu_addwatcher(struct i2c_eeprom_emu *,
				     int addr, int size,
				     struct i2c_eeprom_emu_watcher *);


extern struct i2c_eeprom_emu *i2c_pxa_get_eeprom(void);

#endif /* _LINUX_I2C_ALGO_PXA_H */
