/*
 * (C) Copyright 2014
 * NVIDIA Corporation <www.nvidia.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <netdev.h>
#include <power/as3722.h>

#include <asm/arch/gpio.h>
#include <asm/arch/pinmux.h>

#include "pinmux-config-jetson-tk1.h"

DECLARE_GLOBAL_DATA_PTR;

/*
 * Routine: pinmux_init
 * Description: Do individual peripheral pinmux configs
 */
void pinmux_init(void)
{
	gpio_config_table(jetson_tk1_gpio_inits,
			  ARRAY_SIZE(jetson_tk1_gpio_inits));

	pinmux_config_pingrp_table(jetson_tk1_pingrps,
				   ARRAY_SIZE(jetson_tk1_pingrps));

	pinmux_config_drvgrp_table(jetson_tk1_drvgrps,
				   ARRAY_SIZE(jetson_tk1_drvgrps));
}
