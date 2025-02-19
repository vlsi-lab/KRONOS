//////////////////////////////////////////////////////////////////////////////////////////
// Engineer:      Alessandra Dolmeta - alessandra.dolmeta@cea.fr                        //
//                                                                                      //
// Design Name:    Top level module                                                     //
// Project Name:   kronos:                                                               //
//                 Tightly-coupled Accelerator for RISC-V with Code-based Algorithms    //
// File name:      xif_controller.sv                                                    //
// Language:       SystemVerilog                                                        //
//                                                                                      //
// Description:    kronos xif_controller.                                                //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////

module xif_controller
  import cv32e40px_core_v_xif_pkg::*;
  import kronos_pkg::*;
(
  input clk_i,
  input rst_ni,

  // Compressed Interface
  input  logic x_compressed_valid_i,
  output logic x_compressed_ready_o,
  input  x_compressed_req_t x_compressed_req_i,
  output x_compressed_resp_t x_compressed_resp_o,

  // Issue Interface
  input  logic x_issue_valid_i,
  output logic x_issue_ready_o,
  input  x_issue_req_t x_issue_req_i,
  output x_issue_resp_t x_issue_resp_o,

  // Commit Interface
  input  logic x_commit_valid_i,
  input  x_commit_t x_commit_i,

  // Memory Eequest/Response Interface
  output logic x_mem_valid_o,
  input  logic x_mem_ready_i,
  output x_mem_req_t x_mem_req_o,
  input  x_mem_resp_t x_mem_resp_i,

  // Memory Result Interface
  input  logic x_mem_result_valid_i,
  input  x_mem_result_t x_mem_result_i,

  // Result Interface
  output logic x_result_valid_o,
  input  logic x_result_ready_i,
  output x_result_t x_result_o,

  //kronos interface
  output kronos_pkg::mode_t mode_o, 
  output kronos_pkg::in_t in_o,
  output kronos_pkg::funct7_t funct7_o,
  output kronos_pkg::funct2_t funct2_o,
  output kronos_pkg::immediate_t immediate_o,
  output logic [1:0] insr_o,
  input kronos_pkg::out_t res_register_i,

  //output control signals
  output logic keccak_start_o,
  output logic keccak_store_o,
  input logic keccak_done_i
);


logic [4:0] rd_saved;
logic save_rd;

kronos_pkg::instruction_u instruction; 
kronos_pkg::controller_state_t state, next_state;
logic keccak_start, keccak_store;
logic result_valid_reg;
logic issue_resp_accept_reg;
logic issue_resp_accept;

logic issue_ready, resp_writeback, result_valid, save_reg_rd; 
logic [X_ID_WIDTH      -1:0] id;
logic [4:0] rd;
kronos_pkg::out_t data;
logic [X_RFW_WIDTH/XLEN-1:0] we;
kronos_pkg::mode_t mode; 
kronos_pkg::in_t in;
kronos_pkg::funct7_t funct7;
kronos_pkg::funct2_t funct2;
kronos_pkg::immediate_t immediate;
logic [1:0] insr;

//*****COMPRESSED INTERFACE*********************************************
//not used
assign x_compressed_ready_o = '0;
assign x_compressed_resp_o = '0;
//*****ISSUE INTERFACE**************************************************
assign x_issue_resp_o.loadstore = '0;
assign x_issue_resp_o.ecswrite = '0;
assign x_issue_resp_o.exc = '0;
assign x_issue_resp_o.dualread = '0;
assign x_issue_resp_o.dualwrite = '0;
//*****MEMORY INTERFACE**************************************************
assign x_mem_valid_o = '0;
assign x_mem_req_o = '0;
//*****RESULT INTERFACE**************************************************
assign x_result_o.ecsdata = '0;
assign x_result_o.ecswe = '0;
assign x_result_o.exc = '0;
assign x_result_o.exccode = '0;
assign x_result_o.err = '0;
assign x_result_o.dbg = '0;
//**********************************************************************

//****EXTERNAL SIGNALS*************************************************
//assign keccak_start_o = keccak_start;
//assign keccak_store_o = keccak_store;
assign immediate_o  = '0; 


assign instruction = x_issue_req_i.instr;
//Save the value of rd to perform write-back
always_ff @(posedge clk_i or negedge rst_ni) begin
  if (!rst_ni) begin
    rd_saved = '0;
  end else begin
    if (save_reg_rd) rd_saved = instruction.as_kronos_R.rd;
    else rd_saved = rd_saved;
  end
end


//State register
always_ff @(posedge clk_i or negedge rst_ni) begin
  if (!rst_ni) begin
    state <= RESET_S;
  end else begin 
    state <= next_state;
  end
end

always_ff @(posedge clk_i) begin
    x_issue_resp_o.accept    <= issue_resp_accept;
    x_issue_ready_o          <= issue_ready;
    x_issue_resp_o.writeback <= resp_writeback;
    x_result_valid_o         <= result_valid;
    x_result_o.id            <= id;
    x_result_o.rd            <= rd;
    x_result_o.data          <= res_register_i;
    x_result_o.we            <= we;
    mode_o       <= mode;
    in_o         <= in;
    save_rd      <= save_reg_rd;
    funct7_o     <= funct7;
    funct2_o     <= funct2;
    insr_o       <= insr;
    keccak_start_o <= keccak_start;
    keccak_store_o <= keccak_store;
end



always_comb begin : issue_result_interface
  next_state = WAIT_S;
  issue_ready = 1'b0;

  resp_writeback = 1'b0;
  issue_resp_accept = 1'b0;
  //x_issue_resp_o.accept = issue_resp_accept_reg;

  result_valid = '0;
  id = '0;
  rd = '0;
  data = '0;
  we    = '0;
  
  mode       = '0;
  in         = '0;
  save_reg_rd      = '0;
  funct7     = '0;
  funct2     = '0;
  insr       = '0;
  keccak_start = '0;
  keccak_store = '0;

  case (state)

    RESET_S:  begin 
      next_state = WAIT_S;
      issue_ready = 1'b0;

      resp_writeback = 1'b0;
      issue_resp_accept = '0;

      result_valid = '0;
      id = '0;
      rd = '0;
      data = '0;
      we    = '0;

      mode       = '0;
      in         = '0;
      save_reg_rd      = '0;
      funct7     = '0;
      funct2     = '0;
      insr       = '0;
      keccak_start = '0;
      keccak_store = '0;
    end 

    WAIT_S:  begin
      next_state = WAIT_S;
      issue_ready = 1'b0;

      resp_writeback = 1'b0;
      issue_resp_accept = '0;

      result_valid = '0;
      id = '0;
      rd = '0;
      data = '0;
      we    = '0;

      if (x_issue_valid_i == 1'b1) begin
        case (instruction.raw[6:0])
  
          //R-TYPE--------------------------------------------------------------
          kronos_pkg::kronos_R4: begin
            insr = 2'b11;
            funct7 = '0; //Independently on the instruction, funct7 is for R4

            case (instruction.as_kronos_R4.funct3)
              //OP_R4_R4-------------------------------------------------------
              kronos_pkg::OP_R4_R4: begin

                case (instruction.as_kronos_R4.funct2)
                  kronos_pkg::FUNCT2_0: begin
                    save_reg_rd     = '1;
                    issue_ready = 1'b1;

                    issue_resp_accept = 1'b1;
                    resp_writeback = 1'b1;
                    in.rs1_0 = x_issue_req_i.rs[0]; //rs1
                    in.rs2_0 = x_issue_req_i.rs[1]; //rs2
                    in.rs3_0 = x_issue_req_i.rs[2]; //rs3
                    mode.mode_kronos = instruction.as_kronos_R4.funct3;
                    funct2 = instruction.as_kronos_R4.funct2;
                    next_state = LOAD_S;

                    //x_result_o.rd = rd_saved;
                    //x_result_o.data = res_register_i;
                  end
                  kronos_pkg::FUNCT2_1: begin
                    save_reg_rd     = '0;
                    issue_ready = 1'b1;

                    issue_resp_accept = 1'b1;
                    resp_writeback = 1'b0;
                    in.rs1_0 = x_issue_req_i.rs[0]; //rs1
                    in.rs2_0 = x_issue_req_i.rs[1]; //rs2
                    in.rs3_0 = x_issue_req_i.rs[2]; //rs3
                    mode.mode_kronos = instruction.as_kronos_R4.funct3;
                    funct2 = instruction.as_kronos_R4.funct2;
                    next_state = KECCAK_S;

                    keccak_start      = 1'b1;
                    //x_result_o.rd = rd_saved;
                    //x_result_o.data = res_register_i;
                  end
                  kronos_pkg::FUNCT2_2: begin
                    save_reg_rd     = '1;
                    issue_ready = 1'b1;

                    issue_resp_accept = 1'b1;
                    resp_writeback = 1'b1;
                    in.rs1_0 = x_issue_req_i.rs[0]; //rs1
                    in.rs2_0 = x_issue_req_i.rs[1]; //rs2
                    in.rs3_0 = x_issue_req_i.rs[2]; //rs3
                    mode.mode_kronos = instruction.as_kronos_R4.funct3;
                    funct2 = instruction.as_kronos_R4.funct2;
                    next_state = COMPUTE_S;

                    keccak_store      = 1'b1; 
                  end
                  default: begin
                    issue_resp_accept = 1'b0;
                    next_state = WAIT_S;
                  end
                endcase
            
              end
            endcase
          end //tyca_R4

          default: begin
            issue_resp_accept = 1'b0;
            next_state = WAIT_S;
          end
        endcase
      end
    
    end

    LOAD_S: begin
      
      next_state = WAIT_S;
      issue_ready = 1'b0;
      resp_writeback = 1'b0;
      issue_resp_accept = '0;
      result_valid = '0;
      id = '0;
      rd = '0;
      data = '0;
      we    = '0;
      mode       = '0;
      in         = '0;
      save_reg_rd      = '0;
      funct7     = '0;
      funct2     = '0;
      insr       = '0;
      keccak_start = '0;
      keccak_store = '0;

      if (x_issue_valid_i == 1'b1) begin
        issue_ready = 1'b1;

        if (instruction.raw[6:0] == kronos_pkg::kronos_R4 && instruction.as_kronos_R4.funct3 == OP_R4_R4 && instruction.as_kronos_R4.funct2 == FUNCT2_0) begin
          next_state = LOAD_S;
          
          resp_writeback = 1'b0;
          issue_resp_accept = '1;

          in.rs1_0 = x_issue_req_i.rs[0]; //rs1
          in.rs2_0 = x_issue_req_i.rs[1]; //rs2
          in.rs3_0 = x_issue_req_i.rs[2]; //rs3
          mode.mode_kronos = instruction.as_kronos_R4.funct3;
          funct2 = instruction.as_kronos_R4.funct2;
          save_reg_rd      = '0;
          funct7     = '0;
          funct2     = '0;
          insr       = 2'b11;
          keccak_start = '0;
          keccak_store = '0;
        
        end else
          next_state = WAIT_S;
          issue_ready = 1'b0;
      end
    end

    KECCAK_S: begin

      next_state = KECCAK_S;
      issue_ready = 1'b0;
      resp_writeback = 1'b0;
      issue_resp_accept = '0;
      result_valid = '0;
      id = '0;
      rd = '0;
      data = '0;
      we    = '0;
      mode       = '0;
      in         = '0;
      save_reg_rd      = '0;
      funct7     = '0;
      funct2     = '0;
      insr       = '0;
      keccak_start = '0;
      keccak_store = '0;

      if (keccak_done_i == 1'b1) begin
        next_state = WAIT_S;
        issue_ready = 1'b1;

        resp_writeback = 1'b1;
        issue_resp_accept = '1;

        result_valid = '1;
        id = '0;
        rd = rd_saved;
        data = res_register_i;
        we    = '1;

        mode   = instruction.as_kronos_R4.funct3;
        funct2 = instruction.as_kronos_R4.funct2;
        save_reg_rd = '1;
        insr        = '0;
        keccak_store = '1;
      end

    end

    COMPUTE_S: begin
      next_state = WAIT_S;
      issue_ready = 1'b0;

      resp_writeback = 1'b0;
      issue_resp_accept = '0;

      result_valid = '1;
      id = '0;
      rd = rd_saved;
      data = res_register_i;
      we    = '1;

      mode       = '0;
      in         = '0;
      save_reg_rd      = '0;
      funct7     = '0;
      funct2     = '0;
      insr       = '0;
      keccak_start = '0;
      keccak_store = '0;

      if (x_issue_valid_i == 1'b1) begin
        if (instruction.raw[6:0] == kronos_pkg::kronos_R && instruction.as_kronos_R.funct3 == OP_R_R1 && instruction.as_kronos_R.funct7 == FUNCT7_3) begin 
          save_reg_rd = '1;
          issue_ready = 1'b1;
          
          insr = 2'b01;
          issue_resp_accept = 1'b1;
          resp_writeback = 1'b1;
          in.rs1_0 = x_issue_req_i.rs[0]; //rs1
          in.rs2_0 = x_issue_req_i.rs[1]; //rs2
          in.rs3_0 = x_issue_req_i.rs[2]; //rs3
          mode.mode_kronos = instruction.as_kronos_R.funct3;
          funct7 = instruction.as_kronos_R.funct7;
          next_state = COMPUTE_S;
          result_valid = '1;
          rd = rd_saved;
          data = res_register_i;
          we = '1;
        end
        else if (instruction.raw[6:0] == kronos_pkg::kronos_R4 && instruction.as_kronos_R4.funct3 == OP_R4_R1 && (instruction.as_kronos_R4.funct2 == FUNCT2_1 || instruction.as_kronos_R4.funct2 == FUNCT2_0)) begin 
          save_reg_rd = '1;
          issue_ready = 1'b1;
          
          insr = 2'b11;
          issue_resp_accept = 1'b1;
          resp_writeback = 1'b1;
          in.rs1_0 = x_issue_req_i.rs[0]; //rs1
          in.rs2_0 = x_issue_req_i.rs[1]; //rs2
          in.rs3_0 = x_issue_req_i.rs[2]; //rs3
          mode.mode_kronos = instruction.as_kronos_R4.funct3;
          funct2 = instruction.as_kronos_R4.funct2;
          next_state = COMPUTE_S;
          result_valid = '1;
          rd = rd_saved;
          data = res_register_i;
          we = '1;
        end
        else if (instruction.raw[6:0] == kronos_pkg::kronos_R && instruction.as_kronos_R.funct3 == OP_R_R2 && (instruction.as_kronos_R.funct7 == FUNCT7_2 || instruction.as_kronos_R.funct7 == FUNCT7_3 ||instruction.as_kronos_R.funct7 == FUNCT7_4)) begin 
          save_reg_rd = '1;
          issue_ready = 1'b1;
          
          insr = 2'b01;
          issue_resp_accept = 1'b1;
          resp_writeback = 1'b1;
          in.rs1_0 = x_issue_req_i.rs[0]; //rs1
          in.rs2_0 = x_issue_req_i.rs[1]; //rs2
          in.rs3_0 = x_issue_req_i.rs[2]; //rs3
          mode.mode_kronos = instruction.as_kronos_R.funct3;
          funct7 = instruction.as_kronos_R.funct7;
          next_state = COMPUTE_S;
          result_valid = '1;
          rd = rd_saved;
          data = res_register_i;
          we = '1;
        end
        else if (instruction.raw[6:0] == kronos_pkg::kronos_R4 && instruction.as_kronos_R4.funct3 == OP_R4_R3 && (instruction.as_kronos_R4.funct2 == FUNCT2_1 || instruction.as_kronos_R4.funct2 == FUNCT2_0)) begin 
          save_reg_rd = '1;
          issue_ready = 1'b1;
          
          insr = 2'b11;
          issue_resp_accept = 1'b1;
          resp_writeback = 1'b1;
          in.rs1_0 = x_issue_req_i.rs[0]; //rs1
          in.rs2_0 = x_issue_req_i.rs[1]; //rs2
          in.rs3_0 = x_issue_req_i.rs[2]; //rs3
          mode.mode_kronos = instruction.as_kronos_R4.funct3;
          funct2 = instruction.as_kronos_R4.funct2;
          next_state = COMPUTE_S;
          result_valid = '1;
          rd = rd_saved;
          data = res_register_i;
          we = '1;
        end
      end
    end
  endcase

end




endmodule
