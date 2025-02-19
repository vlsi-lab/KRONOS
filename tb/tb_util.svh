// Copyright 2022 OpenHW Group
// Solderpad Hardware License, Version 2.1, see LICENSE.md for details.
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1

`ifndef SYNTHESIS
// Task for loading 'mem' with SystemVerilog system task $readmemh()
export "DPI-C" task tb_readHEX;
export "DPI-C" task tb_loadHEX;
export "DPI-C" task tb_writetoSram0;
export "DPI-C" task tb_writetoSram1;
export "DPI-C" task tb_writetoSram2;
export "DPI-C" task tb_writetoSram3;
export "DPI-C" task tb_writetoSram4;
export "DPI-C" task tb_writetoSram5;
export "DPI-C" task tb_writetoSram6;
export "DPI-C" task tb_writetoSram7;
export "DPI-C" task tb_writetoSram8;
export "DPI-C" task tb_writetoSram9;
export "DPI-C" task tb_getMemSize;
export "DPI-C" task tb_set_exit_loop;

import core_v_mini_mcu_pkg::*;

task tb_getMemSize;
  output int mem_size;
  output int num_banks;
  mem_size  = core_v_mini_mcu_pkg::MEM_SIZE;
  num_banks = core_v_mini_mcu_pkg::NUM_BANKS;
endtask

task tb_readHEX;
  input string file;
  output logic [7:0] stimuli[core_v_mini_mcu_pkg::MEM_SIZE];
  $readmemh(file, stimuli);
endtask

task tb_loadHEX;
  input string file;
  //whether to use debug to write to memories
  logic [7:0] stimuli[core_v_mini_mcu_pkg::MEM_SIZE];
  int i, stimuli_counter, bank, NumBytes, NumBanks;
  logic [31:0] addr;

  tb_readHEX(file, stimuli);
  tb_getMemSize(NumBytes, NumBanks);

`ifndef VERILATOR
  for (i = 0; i < NumBytes; i = i + 4) begin

    @(posedge kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.clk_i);
    addr = i;
    #1;
    // write to memory
    force kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.debug_subsystem_i.dm_obi_top_i.master_req_o = 1'b1;
    force kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.debug_subsystem_i.dm_obi_top_i.master_addr_o = addr;
    force kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.debug_subsystem_i.dm_obi_top_i.master_we_o = 1'b1;
    force kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.debug_subsystem_i.dm_obi_top_i.master_be_o = 4'b1111;
    force kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.debug_subsystem_i.dm_obi_top_i.master_wdata_o = {
      stimuli[i+3], stimuli[i+2], stimuli[i+1], stimuli[i]
    };

    while (!kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.debug_subsystem_i.dm_obi_top_i.master_gnt_i)
      @(posedge kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.clk_i);

    #1;
    force kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.debug_subsystem_i.dm_obi_top_i.master_req_o = 1'b0;

    wait (kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.debug_subsystem_i.dm_obi_top_i.master_rvalid_i);

    #1;

  end

  release kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.debug_subsystem_i.dm_obi_top_i.master_req_o;
  release kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.debug_subsystem_i.dm_obi_top_i.master_addr_o;
  release kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.debug_subsystem_i.dm_obi_top_i.master_we_o;
  release kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.debug_subsystem_i.dm_obi_top_i.master_be_o;
  release kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.debug_subsystem_i.dm_obi_top_i.master_wdata_o;

`else

  stimuli_counter = 0;
  for (i = 0; i < NumBytes / NumBanks; i = i + 4) begin
    tb_writetoSram0(i / 4, stimuli[stimuli_counter+3], stimuli[stimuli_counter+2],
                    stimuli[stimuli_counter+1], stimuli[stimuli_counter]);
    stimuli_counter = stimuli_counter + 4;
  end
  for (i = 0; i < NumBytes / NumBanks; i = i + 4) begin
    tb_writetoSram1(i / 4, stimuli[stimuli_counter+3], stimuli[stimuli_counter+2],
                    stimuli[stimuli_counter+1], stimuli[stimuli_counter]);
    stimuli_counter = stimuli_counter + 4;
  end
  for (i = 0; i < NumBytes / NumBanks; i = i + 4) begin
    tb_writetoSram2(i / 4, stimuli[stimuli_counter+3], stimuli[stimuli_counter+2],
                    stimuli[stimuli_counter+1], stimuli[stimuli_counter]);
    stimuli_counter = stimuli_counter + 4;
  end
  for (i = 0; i < NumBytes / NumBanks; i = i + 4) begin
    tb_writetoSram3(i / 4, stimuli[stimuli_counter+3], stimuli[stimuli_counter+2],
                    stimuli[stimuli_counter+1], stimuli[stimuli_counter]);
    stimuli_counter = stimuli_counter + 4;
  end
  for (i = 0; i < NumBytes / NumBanks; i = i + 4) begin
    tb_writetoSram4(i / 4, stimuli[stimuli_counter+3], stimuli[stimuli_counter+2],
                    stimuli[stimuli_counter+1], stimuli[stimuli_counter]);
    stimuli_counter = stimuli_counter + 4;
  end
  for (i = 0; i < NumBytes / NumBanks; i = i + 4) begin
    tb_writetoSram5(i / 4, stimuli[stimuli_counter+3], stimuli[stimuli_counter+2],
                    stimuli[stimuli_counter+1], stimuli[stimuli_counter]);
    stimuli_counter = stimuli_counter + 4;
  end
  for (i = 0; i < NumBytes / NumBanks; i = i + 4) begin
    tb_writetoSram6(i / 4, stimuli[stimuli_counter+3], stimuli[stimuli_counter+2],
                    stimuli[stimuli_counter+1], stimuli[stimuli_counter]);
    stimuli_counter = stimuli_counter + 4;
  end
  for (i = 0; i < NumBytes / NumBanks; i = i + 4) begin
    tb_writetoSram7(i / 4, stimuli[stimuli_counter+3], stimuli[stimuli_counter+2],
                    stimuli[stimuli_counter+1], stimuli[stimuli_counter]);
    stimuli_counter = stimuli_counter + 4;
  end
  for (i = 0; i < NumBytes / NumBanks; i = i + 4) begin
    tb_writetoSram8(i / 4, stimuli[stimuli_counter+3], stimuli[stimuli_counter+2],
                    stimuli[stimuli_counter+1], stimuli[stimuli_counter]);
    stimuli_counter = stimuli_counter + 4;
  end
  for (i = 0; i < NumBytes / NumBanks; i = i + 4) begin
    tb_writetoSram9(i / 4, stimuli[stimuli_counter+3], stimuli[stimuli_counter+2],
                    stimuli[stimuli_counter+1], stimuli[stimuli_counter]);
    stimuli_counter = stimuli_counter + 4;
  end

`endif

endtask

task tb_writetoSram0;
  input integer addr;
  input [7:0] val3;
  input [7:0] val2;
  input [7:0] val1;
  input [7:0] val0;
`ifdef VCS
  force kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_i.ram0_i.tc_ram_i.sram[addr] = {
    val3, val2, val1, val0
  };
  release kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_i.ram0_i.tc_ram_i.sram[addr];
`else
  kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_i.ram0_i.tc_ram_i.sram[addr] = {
    val3, val2, val1, val0
  };
`endif
endtask

task tb_writetoSram1;
  input integer addr;
  input [7:0] val3;
  input [7:0] val2;
  input [7:0] val1;
  input [7:0] val0;
`ifdef VCS
  force kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_i.ram1_i.tc_ram_i.sram[addr] = {
    val3, val2, val1, val0
  };
  release kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_i.ram1_i.tc_ram_i.sram[addr];
`else
  kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_i.ram1_i.tc_ram_i.sram[addr] = {
    val3, val2, val1, val0
  };
`endif
endtask

task tb_writetoSram2;
  input integer addr;
  input [7:0] val3;
  input [7:0] val2;
  input [7:0] val1;
  input [7:0] val0;
`ifdef VCS
  force kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_i.ram2_i.tc_ram_i.sram[addr] = {
    val3, val2, val1, val0
  };
  release kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_i.ram2_i.tc_ram_i.sram[addr];
`else
  kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_i.ram2_i.tc_ram_i.sram[addr] = {
    val3, val2, val1, val0
  };
`endif
endtask

task tb_writetoSram3;
  input integer addr;
  input [7:0] val3;
  input [7:0] val2;
  input [7:0] val1;
  input [7:0] val0;
`ifdef VCS
  force kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_i.ram3_i.tc_ram_i.sram[addr] = {
    val3, val2, val1, val0
  };
  release kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_i.ram3_i.tc_ram_i.sram[addr];
`else
  kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_i.ram3_i.tc_ram_i.sram[addr] = {
    val3, val2, val1, val0
  };
`endif
endtask

task tb_writetoSram4;
  input integer addr;
  input [7:0] val3;
  input [7:0] val2;
  input [7:0] val1;
  input [7:0] val0;
`ifdef VCS
  force kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_i.ram4_i.tc_ram_i.sram[addr] = {
    val3, val2, val1, val0
  };
  release kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_i.ram4_i.tc_ram_i.sram[addr];
`else
  kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_i.ram4_i.tc_ram_i.sram[addr] = {
    val3, val2, val1, val0
  };
`endif
endtask

task tb_writetoSram5;
  input integer addr;
  input [7:0] val3;
  input [7:0] val2;
  input [7:0] val1;
  input [7:0] val0;
`ifdef VCS
  force kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_i.ram5_i.tc_ram_i.sram[addr] = {
    val3, val2, val1, val0
  };
  release kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_i.ram5_i.tc_ram_i.sram[addr];
`else
  kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_i.ram5_i.tc_ram_i.sram[addr] = {
    val3, val2, val1, val0
  };
`endif
endtask

task tb_writetoSram6;
  input integer addr;
  input [7:0] val3;
  input [7:0] val2;
  input [7:0] val1;
  input [7:0] val0;
`ifdef VCS
  force kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_i.ram6_i.tc_ram_i.sram[addr] = {
    val3, val2, val1, val0
  };
  release kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_i.ram6_i.tc_ram_i.sram[addr];
`else
  kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_i.ram6_i.tc_ram_i.sram[addr] = {
    val3, val2, val1, val0
  };
`endif
endtask

task tb_writetoSram7;
  input integer addr;
  input [7:0] val3;
  input [7:0] val2;
  input [7:0] val1;
  input [7:0] val0;
`ifdef VCS
  force kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_i.ram7_i.tc_ram_i.sram[addr] = {
    val3, val2, val1, val0
  };
  release kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_i.ram7_i.tc_ram_i.sram[addr];
`else
  kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_i.ram7_i.tc_ram_i.sram[addr] = {
    val3, val2, val1, val0
  };
`endif
endtask

task tb_writetoSram8;
  input integer addr;
  input [7:0] val3;
  input [7:0] val2;
  input [7:0] val1;
  input [7:0] val0;
`ifdef VCS
  force kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_i.ram8_i.tc_ram_i.sram[addr] = {
    val3, val2, val1, val0
  };
  release kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_i.ram8_i.tc_ram_i.sram[addr];
`else
  kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_i.ram8_i.tc_ram_i.sram[addr] = {
    val3, val2, val1, val0
  };
`endif
endtask

task tb_writetoSram9;
  input integer addr;
  input [7:0] val3;
  input [7:0] val2;
  input [7:0] val1;
  input [7:0] val0;
`ifdef VCS
  force kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_i.ram9_i.tc_ram_i.sram[addr] = {
    val3, val2, val1, val0
  };
  release kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_i.ram9_i.tc_ram_i.sram[addr];
`else
  kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_i.ram9_i.tc_ram_i.sram[addr] = {
    val3, val2, val1, val0
  };
`endif
endtask


task tb_set_exit_loop;
`ifdef VCS
  force kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.ao_peripheral_subsystem_i.soc_ctrl_i.testbench_set_exit_loop[0] = 1'b1;
  release kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.ao_peripheral_subsystem_i.soc_ctrl_i.testbench_set_exit_loop[0];
`else
  kronos_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.ao_peripheral_subsystem_i.soc_ctrl_i.testbench_set_exit_loop[0] = 1'b1;
`endif
endtask
`endif
