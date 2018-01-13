/*
 * Copyright (C) 2014 NVIDIA Corporation
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#define pr_fmt(fmt) "tps65913: " fmt

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <fdtdec.h>
#include <i2c.h>

#include <power/as3722.h>

#define TPS65913_SMPS12_VOLTAGE_REG		0x53	/* CPU */
#define TPS65913_SMPS12_CTRL_REG		0x50	/* CPU CTRL */
#define TPS65913_SMPS45_VOLTAGE_REG		0x5B	/* GPU */
#define TPS65913_SMPS45_CTRL_REG 		0x58 	/* GPU CTRL*/

int tps65913_smps_init(struct udevice *pmic)
{
	/*
	 * Bring up VDD_CPU via the TPS65913 PMIC on the PWR I2C bus.
	 * First set VDD to 1.2V, then enable the VDD regulator.
	 */
	// SET
	data_buffer[0] = 0x90;
	reg = 0x53;
	dm_i2c_write(dev, reg, data_buffer, 1);

	
	// ENABLE
	data_buffer[0] = 0x01;
	reg = 0x50;
	dm_i2c_write(dev, reg, data_buffer, 1);

	// WAIT
	udelay(10 * 1000);

	debug("%s: Setting VDD_GPU to 1.2V via TPS65913\n", __func__);
	/*
	 * Bring up VDD_GPU via the TPS65913 PMIC on the PWR I2C bus.
	 * First set VDD to 1.2V, then enable the VDD regulator.
	 */
	// SET
	data_buffer[0] = 0x90;
	reg = 0x5B;
	dm_i2c_write(dev, reg, data_buffer, 1);
	// ENABLE
	data_buffer[0] = 0x01;
	reg = 0x58;
	dm_i2c_write(dev, reg, data_buffer, 1);
	// WAIT
	udelay(10 * 1000);

	debug("%s: Set VDD_SDMMC to 3.3V via TPS65913\n", __func__);
	/*
	 * Bring up VDD_SDMMC via the TPS65913 PMIC on the PWR I2C bus.
	 * First set it to bypass 3.3V straight thru, then enable the regulator
	 *
	 * NOTE: We do this early because doing it later seems to hose the CPU
	 * power rail/partition startup. Need to debug.
	 */
	/* TPS65913: LDO9_VOLTAGE = 3.3V */
	data_buffer[0] = 0x31;
	reg = 0x61;
	dm_i2c_write(dev, reg, data_buffer, 1);
	/* TPS65913: LDO9_CTRL = Active */
	data_buffer[0] = 0x01;
	reg = 0x60;
	dm_i2c_write(dev, reg, data_buffer, 1);

	// WAIT
	udelay(10 * 1000);
}

int tps65913_init(struct udevice **devp)
{
	struct udevice *pmic;
	const unsigned int bus = 4;
	const unsigned int address = 0x58;
	int err;

	err = i2c_get_chip_for_busnum(bus, address, 1, &pmic);
	if (err)
		return err;
	      
	*devp = pmic;

	return 0;
}
