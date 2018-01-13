/*
 * Copyright (C) 2017 SmokeTeam
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __PALMAS_TPS65913_H__
#define __PALMAS_TPS65913_H__

#include <asm/types.h>

struct udevice;

int tps65913_init_init(struct udevice **devp);
int tps65913_smps_init(struct udevice *pmic);

#endif /* __PALMAS_TPS65913_H__ */
