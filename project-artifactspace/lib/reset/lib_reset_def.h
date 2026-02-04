#ifndef LIB_RESET_DEF_H
  #define LIB_RESET_DEF_H

  // Khai báo các thư viện sử dụng chung

    #include "lib/lib_keyword_def.h"

  // Khai báo địa chỉ ngoại vi

    #define RTC_CNTL_REGS_DPTBUS_BASEADDR 0x3FF48000ul
    #define RTC_CNTL_REGS_AHBBUS_BASEADDR 0x60008000ul
  
  // Khai báo địa chỉ thanh ghi cụ thể

    #define RTC_CNTL_OPTIONS0_REG_ADDR    (RTC_CNTL_REGS_DPTBUS_BASEADDR + 0x0000ul)
    #define RTC_CNTL_RESET_STATE_REG_ADDR (RTC_CNTL_REGS_DPTBUS_BASEADDR + 0x0034ul)

  // Khai báo cấu trúc thanh ghi

    tdf_strc RTC_CNTL_OPTIONS0_REG_Typedef {
      __vo ui RTC_CNTL_SW_STALL_APPCPU_C0 : 2;
      __vo ui RTC_CNTL_SW_STALL_PROCPU_C0 : 2;
      __vo ui RTC_CNTL_SW_APPCPU_RST : 1; 
      __vo ui RTC_CNTL_SW_PROCPU_RST : 1;
      __vo ui RTC_CNTL_BB_I2C_FORCE_PD : 1;
      __vo ui RTC_CNTL_BB_I2C_FORCE_PU : 1;
      __vo ui RTC_CNTL_BBPLL_I2C_FORCE_PD : 1;
      __vo ui RTC_CNTL_BBPLL_I2C_FORCE_PU : 1;
      __vo ui RTC_CNTL_BBPLL_FORCE_PD : 1;
      __vo ui RTC_CNTL_BBPLL_FORCE_PU : 1;
      __vo ui RTC_CNTL_XTL_FORCE_PD : 1;
      __vo ui RTC_CNTL_XTL_FORCE_PU : 1;
      __vo ui RTC_CNTL_BIAS_SLEEP_FOLW_8M : 1;
      __vo ui RTC_CNTL_BIAS_FORCE_SLEEP : 1;
      __vo ui RTC_CNTL_BIAS_FORCE_NOSLEEP : 1;
      __vo ui RTC_CNTL_BIAS_I2C_FOLW_8M : 1;
      __vo ui RTC_CNTL_BIAS_I2C_FORCE_PD : 1;
      __vo ui RTC_CNTL_BIAS_I2C_FORCE_PU : 1;
      __vo ui RTC_CNTL_BIAS_CORE_FOLW_8M : 1;
      __vo ui RTC_CNTL_BIAS_CORE_FORCE_PD : 1;
      __vo ui RTC_CNTL_BIAS_CORE_FORCE_PU : 1;
      __vo ui RESERVED0 : 6;
      __vo ui RTC_CNTL_DG_WRAP_FORCE_RST : 1;
      __vo ui RTC_CNTL_DG_WRAP_FORCE_NORST : 1;
      __vo ui RTC_CNTL_SW_SYS_RST : 1;
    } RTC_CNTL_OPTIONS0_REG_Typedef;

    tdf_strc RTC_CNTL_RESET_STATE_REG_Typedef {
      __vo ui RTC_CNTL_RESET_CAUSE_PROCPU : 6;
      __vo ui RTC_CNTL_RESET_CAUSE_APPCPU : 6;
      __vo ui RTC_CNTL_APPCPU_STAT_VECTOR_SEL : 1;
      __vo ui RTC_CNTL_PROCPU_STAT_VECTOR_SEL : 1;
      __vo ui RESERVED0 : 18;
    } RTC_CNTL_RESET_STATE_REG_Typedef;

  // Khai báo cấu trúc ngoại vi

    tdf_strc RTC_CNTL_REGS_PTRS {
      __vo RTC_CNTL_OPTIONS0_REG_Typedef RTC_CNTL_OPTIONS0_REG;
      __vo BLANK_REG RESERVED1[12]; // 0x3FF48004 - 0x3FF48030
      __vo RTC_CNTL_RESET_STATE_REG_Typedef RTC_CNTL_RESET_STATE_REG;
      __vo BLANK_REG RESERVED2[40]; // 0x3FF48038 - 0x3FF480D4
    } RTC_CNTL_REGS_Typedef;

  // >> Tạo con trỏ phần cứng thật và con trỏ phần cứng giả tới ngoại vi

    #ifndef UNIT_TEST
			#define RTC_CNTL_REGS_PTR ((RTC_CNTL_REGS_Typedef *)RTC_CNTL_REGS_DPTBUS_BASEADDR)
		#else
			extern RTC_CNTL_REGS_PTR MOCK_RTC_CNTL_REGS;
      #define RTC_CNTL_REGS_PTR (&MOCK_RTC_CNTL_REGS)
		#endif

  // Khai báo các định nghĩa bit cần sử dụng trên RTC_CNTL_OPTIONS0_REG

    #define RTC_CNTL_SW_APPCPU_RST_SET    0x01ul
    #define RTC_CNTL_SW_APPCPU_RST_CLEAR  0x00ul

    #define RTC_CNTL_SW_PROCPU_RST_SET    0x01ul
    #define RTC_CNTL_SW_PROCPU_RST_CLEAR  0x00ul

    #define RTC_CNTL_SW_SYS_RST_SET       0x01ul
    #define RTC_CNTL_SW_SYS_RST_CLEAR     0x00ul

  // Khai báo các định nghĩa bit cần sử dụng trên RTC_CNTL_RESET_STATE_REG

    #define RTC_CNTL_RESET_CAUSE_POR_RESET              0x01ul
    #define RTC_CNTL_RESET_CAUSE_RWDT_SYS_RESET         0x10ul
    #define RTC_CNTL_RESET_CAUSE_BRO_RESET              0x0Ful
    #define RTC_CNTL_RESET_CAUSE_SW_SYS_RESET           0x03ul
    #define RTC_CNTL_RESET_CAUSE_DS_RESET               0x05ul
    #define RTC_CNTL_RESET_CAUSE_SDIO_RESET             0x06ul
    #define RTC_CNTL_RESET_CAUSE_MWDT0_GLO_RESET        0x07ul
    #define RTC_CNTL_RESET_CAUSE_MWDT1_GLO_RESET        0x08ul
    #define RTC_CNTL_RESET_CAUSE_RWDT_CORE_RESET        0x09ul
    #define RTC_CNTL_RESET_CAUSE_RWDT_CPU_RESET         0x0Dul
    #define RTC_CNTL_RESET_CAUSE_PROCPU_MWDT0_CPU_RESET 0x0Bul
    #define RTC_CNTL_RESET_CAUSE_PROCPU_SW_CPU_RESET    0x0Cul
    #define RTC_CNTL_RESET_CAUSE_APPCPU_MWDT1_CPU_RESET 0x0Bul
    #define RTC_CNTL_RESET_CAUSE_APPCPU_SW_CPU_RESET    0x0Cul
    #define RTC_CNTL_RESET_CAUSE_APPCPU_PROCPU_CPU_RESET    0x0Eul

  // Khai báo các giá trị reset có thể catch

    /**
     * Ghi chú:
     * Các giá trị reset này phải đáp ứng khi cả 2 bên CPU sau khi reset đều fetch ra cùng 1 giá trị.
     */

    #define RST_POR RTC_CNTL_RESET_CAUSE_POR_RESET
    #define RST_RWDT_SYS RTC_CNTL_RESET_CAUSE_RWDT_SYS_RESET
    #define RST_BROWNOUT RTC_CNTL_RESET_CAUSE_BRO_RESET
    #define RST_SW_SYS RTC_CNTL_RESET_CAUSE_SW_SYS_RESET
    #define RST_DEEPSLEEP RTC_CNTL_RESET_CAUSE_DS_RESET
    #define RST_SDIO RTC_CNTL_RESET_CAUSE_SDIO_RESET
    #define RST_MWDT0_GLO RTC_CNTL_RESET_CAUSE_MWDT0_GLO_RESET
    #define RST_MWDT1_GLO RTC_CNTL_RESET_CAUSE_MWDT1_GLO_RESET
    #define RST_RWDT_CORE RTC_CNTL_RESET_CAUSE_RWDT_CORE_RESET
    #define RST_RWDT_CPU RTC_CNTL_RESET_CAUSE_RWDT_CPU_RESET

  // Khai báo các giá trị reset riêng biệt của PRO_CPU

    #define RST_PRO_MWDT0_CPU RTC_CNTL_RESET_CAUSE_PROCPU_MWDT0_CPU_RESET
    #define RST_PRO_SW_CPU RTC_CNTL_RESET_CAUSE_PROCPU_SW_CPU_RESET

  // Khai báo các giá trị reset riêng biệt của APP_CPU

    #define RST_APP_MWDT1_CPU RTC_CNTL_RESET_CAUSE_APPCPU_MWDT1_CPU_RESET
    #define RST_APP_SW_CPU RTC_CNTL_RESET_CAUSE_APPCPU_SW_CPU_RESET
    #define RST_APP_CPU_PRO_CPU RTC_CNTL_RESET_CAUSE_APPCPU_PROCPU_CPU_RESET

  // Khai báo nguồn CPU select

    #define PRO_CPU 0x0Ful
    #define APP_CPU 0xF0ul
    #define BOTH_CPU 0xFFul

#endif