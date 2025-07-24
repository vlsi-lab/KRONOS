#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "keccak_x_heep.h"
#include "core_v_mini_mcu.h"
#include "keccak_driver.h"
#include "keccak_auto.h"

#include "stats.h"

// To manage interrupt
#include "csr.h"
#include "rv_plic.h"
#include "rv_plic_regs.h"
#include "rv_plic_structs.h"
#include "hart.h"

// To manage DMA
#include "dma.h"

#define KECCAK_BUSY 0
#define DATA_SIZE 50

// Interrupt controller variables
plic_result_t plic_res;

// Own defined ext interrupt handler
void handler_irq_ext(uint32_t id){
  //printf("D\n");
}
  
void KeccakF1600_StatePermute(uint32_t* Din, uint32_t* Dout)
{

  uint32_t volatile *Din_reg_start = (uint32_t*)KECCAK_PERIPH_START_ADDRESS;
  uint32_t volatile *ctrl_reg = (uint32_t*)(KECCAK_PERIPH_START_ADDRESS + 0xc8);
  uint32_t volatile *status_reg = (uint32_t*)(KECCAK_PERIPH_START_ADDRESS + 0xcc);
  uint32_t current_status;


  
  uint32_t* ext_addr_4B_PTR = (uint32_t*)KECCAK_PERIPH_START_ADDRESS;
 
  // Keccak accelerator send interrupt on ext_intr line 0
  plic_res = plic_Init();
  if (plic_res != kPlicOk) {return -1;}
  
  // Set Keccak priority to 1 (target threshold is by default 0) to trigger an interrupt to the target (the processor)
    plic_res = plic_irq_set_priority(EXT_INTR_0, 1);
    if (plic_res == kPlicOk) {
    } else {}

  // Enable the interrupt in reg 0 
  plic_res = plic_irq_set_enabled(EXT_INTR_0, kPlicToggleEnabled);
  if (plic_res == kPlicOk) {
  } else {}

  // Enable interrupt on processor side
  CSR_SET_BITS(CSR_REG_MSTATUS, 0x8);
  // Set mie.MEIE bit to one to enable machine-level external interrupts
  const uint32_t mask = 1 << 11;//IRQ_EXT_ENABLE_OFFSET;
  CSR_SET_BITS(CSR_REG_MIE, mask);


  // The DMA is initialized (i.e. Any current transaction is cleaned.)
  dma_init(NULL);
    
  dma_config_flags_t res;
  // First DMA transaction consist on loading Din in Keccak register file
  dma_target_t tgt_src = {
                              .ptr        = Din,
                              .inc_du     = 1,
                              .size_du    = DATA_SIZE,
                              .trig       = DMA_TRIG_MEMORY,
                              .type       = DMA_DATA_TYPE_WORD,
                              };
  dma_target_t tgt_dst = {
                              .ptr        = ext_addr_4B_PTR,
                              .inc_du     = 1,
                              .size_du    = DATA_SIZE,
                              .trig       = DMA_TRIG_MEMORY,
                              };

  dma_target_t tgt_addr = {
                              .ptr        = ext_addr_4B_PTR,
                              .inc_du     = 1,
                              .size_du    = DATA_SIZE,
                              .trig       = DMA_TRIG_MEMORY,
                              };

  dma_trans_t trans = {
                              .src        = &tgt_src,
                              .dst        = &tgt_dst,
                              .src_addr   = &tgt_addr,
                              .mode       = DMA_TRANS_MODE_SINGLE,
                              .win_du     = 0,
                              .end        = DMA_TRANS_END_INTR,
                              };

  res = dma_validate_transaction( &trans, DMA_ENABLE_REALIGN, DMA_PERFORM_CHECKS_INTEGRITY );
  res = dma_load_transaction(&trans);
  res = dma_launch(&trans);

  while( ! dma_is_ready()) {
      // disable_interrupts
      // this does not prevent waking up the core as this is controlled by the MIP register
      CSR_SET_BITS(CSR_REG_MSTATUS, 0x0);
      if ( dma_is_ready() == 0 ) {
          wait_for_interrupt();
          //from here we wake up even if we did not jump to the ISR
      }
      CSR_SET_BITS(CSR_REG_MSTATUS, 0x8);
  }

  
  asm volatile ("": : : "memory");
  *ctrl_reg = 1 << KECCAK_CTRL_START;
  asm volatile ("": : : "memory");
  *ctrl_reg = 0 << KECCAK_CTRL_START;

  // Wait till keccak is done
  while(plic_intr_flag==0) {
      wait_for_interrupt();
  }

  ext_addr_4B_PTR = (uint32_t*)KECCAK_PERIPH_START_ADDRESS;
  tgt_src.ptr = ext_addr_4B_PTR;
  tgt_dst.ptr = Dout;

  // Second DMA transaction consist on reading Dout from Keccak register file
  res = dma_validate_transaction( &trans, DMA_ENABLE_REALIGN, DMA_PERFORM_CHECKS_INTEGRITY );
  res = dma_load_transaction(&trans);
  res = dma_launch(&trans);

  while( ! dma_is_ready()) {
      // disable_interrupts
      // this does not prevent waking up the core as this is controlled by the MIP register
      CSR_SET_BITS(CSR_REG_MSTATUS, 0x0);
      if ( dma_is_ready() == 0 ) {
          wait_for_interrupt();
      }
      CSR_SET_BITS(CSR_REG_MSTATUS, 0x8);
  }
  
}

