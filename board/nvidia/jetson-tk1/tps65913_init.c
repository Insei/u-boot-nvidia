/*
 * (C) Copyright 2017
 * Insei <goodmobiledevices@gmail.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <i2c.h>
#include <asm/io.h>
#include <asm/arch-tegra/tegra_i2c.h>
#include "tps65913_init.h"


void tegra_i2c_ll_write_addr(uint addr, uint config)
{
	struct i2c_ctlr *reg = (struct i2c_ctlr *)TEGRA_DVC_BASE;

	writel(addr, &reg->cmd_addr0);
	writel(config, &reg->cnfg);
}

void tegra_i2c_ll_write_data(uint data, uint config)
{
	struct i2c_ctlr *reg = (struct i2c_ctlr *)TEGRA_DVC_BASE;

	writel(data, &reg->cmd_data1);
	writel(config, &reg->cnfg);
}

void pmic_enable_cpu_vdd(void)
{
	debug("%s entry\n", __func__);

	/* Don't need to set up VDD_CORE - already done - by OTP */

	debug("%s: Setting VDD_CPU to 1.0V via AS3722 reg 0/4D\n", __func__);
	/*
	 * Bring up VDD_CPU via the AS3722 PMIC on the PWR I2C bus.
	 * First set VDD to 1.0V, then enable the VDD regulator.
	 */
	tegra_i2c_ll_write_addr(TPS65913_I2C_ADDR, 2);
	tegra_i2c_ll_write_data(TPS65913_SMPS12_VOLTAGE_DATA, I2C_SEND_2_BYTES);
	udelay(1000);
	tegra_i2c_ll_write_data(TPS65913_SMPS12_CTRL_DATA, I2C_SEND_2_BYTES);
	/*
	 * Don't write SDCONTROL - it's already 0x7F, i.e. all SDs enabled.
	 * tegra_i2c_ll_write_data(AS3722_SD0CONTROL_DATA, I2C_SEND_2_BYTES);
	 */
	udelay(10 * 1000);

	debug("%s: Setting VDD_GPU to 1.0V via AS3722 reg 6/4D\n", __func__);
	/*
	 * Bring up VDD_GPU via the AS3722 PMIC on the PWR I2C bus.
	 * First set VDD to 1.0V, then enable the VDD regulator.
	 */
	tegra_i2c_ll_write_addr(TPS65913_I2C_ADDR, 2);
	tegra_i2c_ll_write_data(TPS65913_SMPS45_VOLTAGE_DATA, I2C_SEND_2_BYTES);
	udelay(1000);
	tegra_i2c_ll_write_data(TPS65913_SMPS45_CTRL_DATA, I2C_SEND_2_BYTES);
	/*
	 * Don't write SDCONTROL - it's already 0x7F, i.e. all SDs enabled.
	 * tegra_i2c_ll_write_data(AS3722_SD6CONTROL_DATA, I2C_SEND_2_BYTES);
	 */
	udelay(10 * 1000);

	//debug("%s: Set VDD_SDMMC to 3.3V via AS3722 reg 0x16/4E\n", __func__);
	/*
	 * Bring up VDD_SDMMC via the AS3722 PMIC on the PWR I2C bus.
	 * First set it to bypass 3.3V straight thru, then enable the regulator
	 *
	 * NOTE: We do this early because doing it later seems to hose the CPU
	 * power rail/partition startup. Need to debug.
	 */
	//tegra_i2c_ll_write_addr(AS3722_I2C_ADDR, 2);
	//tegra_i2c_ll_write_data(AS3722_LDO6VOLTAGE_DATA, I2C_SEND_2_BYTES);
	/*
	 * Don't write LDCONTROL - it's already 0xFF, i.e. all LDOs enabled.
	 * tegra_i2c_ll_write_data(AS3722_LDO6CONTROL_DATA, I2C_SEND_2_BYTES);
	 */
	//udelay(10 * 1000);
}
