// Copyright EPFL contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#ifndef kronos_X_HEEP_H_
#define kronos_X_HEEP_H_

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

  //#include "core_v_mini_mcu.h"

#define kronos_PERIPH_START_ADDRESS (EXT_PERIPHERAL_START_ADDRESS + 0x0000000)
#define kronos_PERIPH_SIZE 0x0001000
#define kronos_PERIPH_END_ADDRESS (kronos_PERIPH_START_ADDRESS + kronos_PERIPH_SIZE)

#define kronos_IO_START_ADDRESS (EXT_SLAVE_START_ADDRESS + 0x0000000)
#define kronos_IO_SIZE 0x0001000
#define kronos_IO_END_ADDRESS (kronos_IO_START_ADDRESS + kronos_IO_SIZE)


#define kronos_DIN_START_ADDR kronos_IO_START_ADDRESS
#define kronos_DOUT_START_ADDR (kronos_IO_START_ADDRESS+0x0000000c8)

#define kronos_CTRL_START_ADDR kronos_PERIPH_START_ADDRESS
#define kronos_STATUS_START_ADDR (kronos_PERIPH_START_ADDRESS+0x00000004)

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  // kronos_X_HEEP_H_
