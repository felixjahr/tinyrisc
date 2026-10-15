// Top-level CPU integrating the datapath, memory, bus, and terminal.

#ifndef TINYRISC_HPP
#define TINYRISC_HPP

#include <systemc>
#include <systemc.h>
#include <map>
using namespace sc_core;

#include "cu.hpp"
#include "main_memory.hpp"
#include "register_file.hpp"
#include "alu.hpp"
#include "multiplexer.hpp"
#include "pc.hpp"
#include "terminal.hpp"
#include "bus.hpp"
#include "adder.hpp"
 
SC_MODULE(TINY_RISC) {
  sc_in<bool> reset;
  sc_clock clk;

  // Cycle counter
  uint32_t cycles;

  std::map<uint32_t, uint32_t> initialMemory;

  PC pc;
  REGISTER_FILE rf;
  ALU alu;
  MAIN_MEMORY mm;
  CU cu;
  TERMINAL ter;
  BUS bus;
  ADDER pcAdder;

  MULTIPLEXER_I32 muxReg1;
  MULTIPLEXER_I32 muxReg2;
  MULTIPLEXER_I32 muxRegW;
  MULTIPLEXER_I32 muxAluRo;

  // Bus multiplexers
  MULTIPLEXER_I32 muxBusAddrin;
  MULTIPLEXER_I32 muxBusRdatain;
  MULTIPLEXER_I32 muxBusAddr;
  MULTIPLEXER_I32 muxBusRdata;
  MULTIPLEXER_I32 muxBusWdata;
  MULTIPLEXER_I8 muxBusIosource;
  MULTIPLEXER_BOOLEAN muxReady;

  // JALR multiplexers
  MULTIPLEXER_I32 muxPcPcout;
  MULTIPLEXER_I32 muxPcAdder;

  sc_signal<uint32_t> rf_r1_muxReg1;
  sc_signal<uint32_t> muxReg1_r1_alu;
  sc_signal<uint32_t> muxReg1_next_pc;
  sc_signal<uint32_t> rf_r2_muxReg2;
  sc_signal<uint32_t> muxReg2_r2_alu;
  sc_signal<uint32_t> muxRegW_wdata_rf;
  sc_signal<uint32_t> alu_ro_muxAluRo;
  sc_signal<uint32_t> muxAluRo_r0_muxRegW;
  sc_signal<uint32_t> muxAluRo_r0_cu;

  sc_signal<bool> alu_overflow_cu;
  sc_signal<sc_bv<3>> cu_op_alu;

  sc_signal<bool> pc_enable_cu;
  sc_signal<sc_bv<2>> pc_j_cu;
  sc_signal<sc_bv<12>> cu_offset_pc;

  sc_signal<sc_bv<5>> cu_a1_rf;
  sc_signal<sc_bv<5>> cu_a2_rf;
  sc_signal<sc_bv<5>> cu_a3_rf;
  sc_signal<bool> cu_we_rf;

  sc_signal<uint8_t> muxReg1_select;
  sc_signal<uint8_t> muxReg2_select;
  sc_signal<uint8_t> muxAluRo_select;
  sc_signal<uint8_t> cu_select_muxRegW;

  // Bus signals
  sc_signal<bool> cu_rin_bus;
  sc_signal<bool> cu_win_bus;
  sc_signal<uint32_t> muxReg2_wdatain_bus;
  sc_signal<bool> bus_r_mm;
  sc_signal<bool> bus_r_ter;
  sc_signal<bool> bus_w_mm;
  sc_signal<bool> bus_w_ter;

  // MuxBusAddrin signals
  sc_signal<uint32_t> muxReg1_muxBusAddrin;
  sc_signal<uint32_t> muxBusAddrin_addrin_bus;
  sc_signal<uint8_t> cu_select_muxBusAddrin;

  // MuxBusRdatain signals
  sc_signal<uint32_t> mm_rdata_muxBusRdatain;
  sc_signal<uint32_t> ter_rdata_muxBusRdatain;
  sc_signal<uint32_t> muxBusRdatain_rdatain_bus;
  sc_signal<uint8_t> muxBusIosource_select_muxBusRdatain;

  // MuxBusAddr signals
  sc_signal<uint32_t> bus_addr_muxBusAddr;
  sc_signal<uint32_t> muxBusAddr_addr_mm;
  sc_signal<uint32_t> muxBusAddr_addr_ter;
  sc_signal<uint8_t> muxBusAddr_select;

  // MuxBusRdata signals
  sc_signal<uint32_t> bus_rdata_muxBusRdata;
  sc_signal<uint32_t> muxBusRdata_datain_cu;
  sc_signal<uint32_t> muxBusRdata_muxRegW;
  sc_signal<uint8_t> muxBusRdata_select;

  // MuxBusWdata signals
  sc_signal<uint32_t> bus_wdata_muxBusWdata;
  sc_signal<uint32_t> muxBusWdata_wdata_mm;
  sc_signal<uint32_t> muxBusWdata_wdata_ter;
  sc_signal<uint8_t> muxBusWdata_select;

  // MuxBusIosource signals
  sc_signal<uint8_t> bus_iosource_muxBusIosource;
  sc_signal<uint8_t> muxBusIosource_select;

  // MuxReady signals
  sc_signal<bool> mm_ready_muxReady;
  sc_signal<bool> ter_ready_muxReady;
  sc_signal<bool> muxReady_ready_cu;
  sc_signal<uint8_t> muxBusIosource_select_muxReady;

  // MuxPcPcout signals
  sc_signal<uint32_t> pc_pcout_muxPcPcout;
  sc_signal<uint32_t> muxPcPcout_muxBusAddrin;
  sc_signal<uint32_t> muxPcPcout_pcAdder;
  sc_signal<uint8_t> muxPcPcout_select;

  // MuxPcAdder signals
  sc_signal<uint32_t> pcAdder_muxPcAdder;
  sc_signal<uint32_t> muxPcAdder_pcin_pc;
  sc_signal<uint32_t> muxPcAdder_muxRegW;
  sc_signal<uint8_t> muxPcAdder_select;

  // PcAdder signal
  sc_signal<uint32_t> pcAdder_inB;

  // LI signal
  sc_signal<uint32_t> cu_muxRegW;

  // Terminal end signal
  sc_signal<bool> ter_end;


  SC_CTOR(TINY_RISC);
  TINY_RISC(sc_module_name name, std::map<uint32_t, uint32_t> initialMemory, uint32_t period, sc_trace_file* tracefile, char terminalToStdout, uint32_t latency) : 
      sc_module(name), clk("clk", period, SC_NS), initialMemory(initialMemory),
      pc("pc"), rf("rf"), alu("alu"), mm("mm"), cu("cu"), ter("ter", terminalToStdout, latency), bus("bus"), pcAdder("pcAdder"),
      muxReg1("muxReg1", 1, 3), muxReg2("muxReg2", 1, 2), muxRegW("muxRegW", 4, 1), muxAluRo("muxAluRo", 1, 2), 
      muxBusAddrin("muxBusAddrin", 2, 1), muxBusRdatain("muxBusRdatain", 2, 1), muxBusAddr("muxBusAddr", 1, 2), muxBusRdata("muxBusRdata", 1, 2), muxBusWdata("muxBusWdata", 1, 2), 
      muxBusIosource("muxBusIosource", 1, 2), muxReady("muxReady", 2, 1), muxPcPcout("muxPcPcout", 1, 2), muxPcAdder("muxPcAdder", 1, 2) {
    rf.clk(clk);
    mm.clk(clk);
    cu.clk(clk);
    pc.clk(clk);
    // Bind terminal clock
    ter.clk(clk);
    
    rf.r1.bind(rf_r1_muxReg1);
    muxReg1.in[0](rf_r1_muxReg1);
    alu.r1.bind(muxReg1_r1_alu);
    muxReg1.out[0](muxReg1_r1_alu);
    pc.next.bind(muxReg1_next_pc);
    muxReg1.out[1](muxReg1_next_pc);
    rf.r2.bind(rf_r2_muxReg2);
    muxReg2.in[0](rf_r2_muxReg2);
    alu.r2.bind(muxReg2_r2_alu);
    muxReg2.out[0](muxReg2_r2_alu);
    rf.wdata.bind(muxRegW_wdata_rf);
    muxRegW.out[0](muxRegW_wdata_rf);
    alu.ro.bind(alu_ro_muxAluRo);
    muxAluRo.in[0](alu_ro_muxAluRo);
    muxRegW.in[0](muxAluRo_r0_muxRegW);
    muxAluRo.out[0](muxAluRo_r0_muxRegW);
   
    alu.overflow.bind(alu_overflow_cu);
    cu.overflow.bind(alu_overflow_cu);
    alu.op.bind(cu_op_alu);
    cu.op.bind(cu_op_alu);
    muxAluRo.out[1](muxAluRo_r0_cu);
    cu.ro.bind(muxAluRo_r0_cu);
    
    pc.enable.bind(pc_enable_cu);
    cu.pcenable.bind(pc_enable_cu);
    pc.j.bind(pc_j_cu);
    cu.pcj.bind(pc_j_cu);
    pc.offset.bind(cu_offset_pc);
    cu.pcoffset.bind(cu_offset_pc);
    
    rf.a1.bind(cu_a1_rf);
    rf.a2.bind(cu_a2_rf);
    rf.a3.bind(cu_a3_rf);
    rf.we.bind(cu_we_rf);
    cu.a1.bind(cu_a1_rf);
    cu.a2.bind(cu_a2_rf);
    cu.a3.bind(cu_a3_rf);
    cu.we.bind(cu_we_rf);
    
    muxReg1.select.bind(muxReg1_select);
    muxReg2.select.bind(muxReg2_select);
    muxAluRo.select.bind(muxAluRo_select);
    muxReg1_select.write(0);
    muxReg2_select.write(0);
    muxAluRo_select.write(0);
    cu.rsource.bind(cu_select_muxRegW);
    muxRegW.select.bind(cu_select_muxRegW);

    // Bus signals
    cu.r.bind(cu_rin_bus);
    bus.rin.bind(cu_rin_bus);
    cu.w.bind(cu_win_bus);
    bus.win.bind(cu_win_bus);
    muxReg2.out[1](muxReg2_wdatain_bus);
    bus.wdatain.bind(muxReg2_wdatain_bus);
    bus.rmm.bind(bus_r_mm);
    mm.r.bind(bus_r_mm);
    bus.rter.bind(bus_r_ter);
    ter.r.bind(bus_r_ter);
    bus.wmm.bind(bus_w_mm);
    mm.w.bind(bus_w_mm);
    bus.wter.bind(bus_w_ter);
    ter.w.bind(bus_w_ter);

    // MuxBusAddrin signals
    muxReg1.out[2](muxReg1_muxBusAddrin);
    muxBusAddrin.in[1](muxReg1_muxBusAddrin);
    muxBusAddrin.out[0](muxBusAddrin_addrin_bus);
    bus.addrin.bind(muxBusAddrin_addrin_bus);
    cu.asource.bind(cu_select_muxBusAddrin);
    muxBusAddrin.select.bind(cu_select_muxBusAddrin);

    // MuxBusRdatain signals
    mm.rdata.bind(mm_rdata_muxBusRdatain);
    muxBusRdatain.in[0](mm_rdata_muxBusRdatain);
    ter.rdata.bind(ter_rdata_muxBusRdatain);
    muxBusRdatain.in[1](ter_rdata_muxBusRdatain);
    muxBusRdatain.out[0](muxBusRdatain_rdatain_bus);
    bus.rdatain.bind(muxBusRdatain_rdatain_bus);
    muxBusIosource.out[1](muxBusIosource_select_muxBusRdatain);
    muxBusRdatain.select.bind(muxBusIosource_select_muxBusRdatain);

    // MuxBusAddr signals
    bus.addr.bind(bus_addr_muxBusAddr);
    muxBusAddr.in[0](bus_addr_muxBusAddr);
    muxBusAddr.out[0](muxBusAddr_addr_mm);
    mm.addr.bind(muxBusAddr_addr_mm);
    muxBusAddr.out[1](muxBusAddr_addr_ter);
    ter.addr.bind(muxBusAddr_addr_ter);
    muxBusAddr.select.bind(muxBusAddr_select);
    muxBusAddr_select.write(0);

    // MuxBusRdata signals
    bus.rdata.bind(bus_rdata_muxBusRdata);
    muxBusRdata.in[0](bus_rdata_muxBusRdata);
    muxBusRdata.out[0](muxBusRdata_datain_cu);
    cu.datain.bind(muxBusRdata_datain_cu);
    muxBusRdata.out[1](muxBusRdata_muxRegW);
    muxRegW.in[1](muxBusRdata_muxRegW);
    muxBusRdata.select.bind(muxBusRdata_select);
    muxBusRdata_select.write(0);

    // MuxBusWdata signals
    bus.wdata.bind(bus_wdata_muxBusWdata);
    muxBusWdata.in[0](bus_wdata_muxBusWdata);
    muxBusWdata.out[0](muxBusWdata_wdata_mm);
    mm.wdata.bind(muxBusWdata_wdata_mm);
    muxBusWdata.out[1](muxBusWdata_wdata_ter);
    ter.wdata.bind(muxBusWdata_wdata_ter);
    muxBusWdata.select.bind(muxBusWdata_select);
    muxBusWdata_select.write(0);

    // MuxBusIosource signals
    bus.iosource.bind(bus_iosource_muxBusIosource);
    muxBusIosource.in[0](bus_iosource_muxBusIosource);
    muxBusIosource.select.bind(muxBusIosource_select);
    muxBusIosource_select.write(0);

    // MuxReady signals
    mm.ready.bind(mm_ready_muxReady);
    muxReady.in[0](mm_ready_muxReady);
    ter.ready.bind(ter_ready_muxReady);
    muxReady.in[1](ter_ready_muxReady);
    muxReady.out[0](muxReady_ready_cu);
    cu.ready.bind(muxReady_ready_cu);
    muxBusIosource.out[0](muxBusIosource_select_muxReady);
    muxReady.select.bind(muxBusIosource_select_muxReady);

    // MuxPcPcout signals
    pc.pc.bind(pc_pcout_muxPcPcout);
    muxPcPcout.in[0](pc_pcout_muxPcPcout);
    muxPcPcout.out[0](muxPcPcout_muxBusAddrin);
    muxBusAddrin.in[0](muxPcPcout_muxBusAddrin);
    muxPcPcout.out[1](muxPcPcout_pcAdder);
    pcAdder.inA.bind(muxPcPcout_pcAdder);
    muxPcPcout.select.bind(muxPcPcout_select);
    muxPcPcout_select.write(0);

    // MuxPcAdder signals
    pcAdder.out.bind(pcAdder_muxPcAdder);
    muxPcAdder.in[0](pcAdder_muxPcAdder);
    muxPcAdder.out[0](muxPcAdder_pcin_pc);
    pc.pcin.bind(muxPcAdder_pcin_pc);
    muxPcAdder.out[1](muxPcAdder_muxRegW);
    muxRegW.in[2](muxPcAdder_muxRegW);
    muxPcAdder.select.bind(muxPcAdder_select);
    muxPcAdder_select.write(0);

    // PcAdder signal
    pcAdder.inB.bind(pcAdder_inB);
    pcAdder_inB.write(4);

    // LI signal
    cu.immout.bind(cu_muxRegW);
    muxRegW.in[3](cu_muxRegW);

    // Terminal end signal
    ter.end.bind(ter_end);

    // Trace file signals
    if (tracefile != nullptr) {
      sc_trace(tracefile, clk, "clk");
      sc_trace(tracefile, pc_pcout_muxPcPcout, "pcout");
      sc_trace(tracefile, ter_end, "ter_end");
      sc_trace(tracefile, bus_iosource_muxBusIosource, "iosource");
      sc_trace(tracefile, cu_win_bus, "win");
      sc_trace(tracefile, cu_rin_bus, "rin");
      sc_trace(tracefile, muxBusRdatain_rdatain_bus, "rdatain");
      sc_trace(tracefile, muxReg2_wdatain_bus, "wdatain");
    }
    
    doReset();
    SC_THREAD(behaviour);
    sensitive << clk.posedge_event();
  }

  void behaviour() {
    while(true) {
      wait();
      if(reset.read()) {
        doReset();
      }
      // Count cycle
      cycles++;
    }
  }

  void doReset() {
    pc.pcValue = 0x00001000;
    for(int i = 0; i < 32; i++) {
      rf.registers_data[i] = 0;
    }
    mm.memory.clear();
    for(auto it = initialMemory.begin(); it != initialMemory.end(); it++) {
      mm.set(it->first, it->second);
    }
  }

  uint32_t getMemoryValue(uint32_t address) {
    return mm.get(address);
  }
};


#endif // TINYRISC_HPP