//////////////////////////////////////////////////////////////////////////////////////////
// Engineer:      Alessandra Dolmeta - alessandra.dolmeta@cea.fr                        //
//                                                                                      //
// Design Name:    Top level module                                                     //
// Project Name:   TYRCA:                                                               //
//                 Tightly-coupled Accelerator for RISC-V with Code-based Algorithms    //
// File name:      register_keccak.sv                                                   //
// Language:       SystemVerilog                                                        //
//                                                                                      //
// Description:    TYRCA register_keccak.                                               //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////

module register_keccak (
    input clk_i,
    input rst_ni,  
    input logic [31:0] register_keccak_0_i,  
    input logic [31:0] register_keccak_1_i,  
    input logic register_keccak_enable_i,
    input logic [5:0] register_keccak_index_i,
    output logic[1599:0] register_keccak_o   
);

  parameter NUM_VARIABLES = 50;
  parameter VARIABLE_WIDTH = 32;

  // Define register with 50 variables, each 32 bits wide
  logic [VARIABLE_WIDTH-1:0] register_array [0:NUM_VARIABLES-1];

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      register_array = '{'0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0, '0};
    end else if (register_keccak_enable_i) begin
      // Write data_in_0 to index
      register_array[register_keccak_index_i] = register_keccak_0_i;
      register_array[register_keccak_index_i + 1] = register_keccak_1_i;
    end
  end

  assign register_keccak_o = {register_array[50], register_array[49], register_array[48], register_array[47], register_array[46], register_array[45], register_array[44], register_array[43], register_array[42], register_array[41], register_array[40], register_array[39], register_array[38], register_array[37], register_array[36], register_array[35], register_array[34], register_array[33], register_array[32], register_array[31], register_array[30], register_array[29], register_array[28], register_array[27], register_array[26], register_array[25], register_array[24], register_array[23], register_array[22], register_array[21], register_array[20], register_array[19], register_array[18], register_array[17], register_array[16], register_array[15], register_array[14], register_array[13], register_array[12], register_array[11], register_array[10], register_array[9], register_array[8], register_array[7], register_array[6], register_array[5], register_array[4], register_array[3], register_array[2], register_array[1], register_array[0]};


endmodule
