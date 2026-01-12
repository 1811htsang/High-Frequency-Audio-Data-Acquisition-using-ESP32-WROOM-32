#ifndef LIB_CLOCK_DEFINITION_H_
  #define LIB_CLOCK_DEFINITION_H_

  // Khai báo các thư viện sử dụng chung

  #include "lib/lib_keyword_define.h"

  // Khai báo địa chỉ ngoại vi

  #define RTC_CNTL_REGS_DPTBUS_BASEADDR 0x3FF48000ul
  #define RTC_CNTL_REGS_AHBBUS_BASEADDR 0x60008000ul
  #define DPORT_REGS_BASEADDR 0x3FF00000ul
  #define SYSCON_REGS_BASEADDR 0x3FF66000ul

  /**
   * Ghi chú:
   * Các ngoại vi cần sử dụng sẽ được bổ sung thêm trong tương lai.
   */

  // Khai báo địa chỉ thanh ghi cụ thể

  #define RTC_CNTL_CLK_CONF_REG_ADDR (RTC_CNTL_REGS_DPTBUS_BASEADDR + 0x0070ul)
  #define DPORT_CPU_PERI_CLK_EN_REG_ADDR (DPORT_REGS_BASEADDR + 0x003Cul)
  #define SYSCON_CLK_CONF_REG_ADDR (SYSCON_REGS_BASEADDR + 0x0008ul)

  /**
   * Ghi chú:
   * Các thanh ghi cần sử dụng sẽ được bổ sung thêm trong tương lai.
   */

  // Khai báo cấu trúc thanh ghi

  typedef __vo struct RTC_CNTL_CLK_CONF_REG {
    __vo ui RESERVED2 : 4;
    __vo ui RTC_CNTL_CK8M_DIV : 2;
    __vo ui RTC_CNTL_ENB_CK8M : 1;
    __vo ui RTC_CNTL_ENB_CK8M_DIV : 1;
    __vo ui RTC_CNTL_DIG_XTAL32K_EN : 1;
    __vo ui RTC_CNTL_DIG_CLK8M_D256_EN : 1;
    __vo ui RTC_CNTL_DIG_CLK8M_EN : 1;
    __vo ui RESERVED1 : 1;
    __vo ui RTC_CNTL_CK8M_DIV_SEL : 3;
    __vo ui RESERVED0 : 2;
    __vo ui RTC_CNTL_CK8M_DFREQ : 8;
    __vo ui RTC_CNTL_CK8M_FORCE_PD : 1;
    __vo ui RTC_CNTL_CK8M_FORCE_PU : 1;
    __vo ui RTC_CNTL_SOC_CLK_SEL : 2;
    __vo ui RTC_CNTL_RTC_FAST_CLK_SEL : 1;
    __vo ui RTC_CNTL_ANA_CLK_RTC_SEL : 2;
  } RTC_CNTL_CLK_CONF_REG_Typedef;

  typedef __vo struct DPORT_CPU_PER_CONF_REG {
    __vo ui CPU_PERIOD_SEL : 2;
    __vo ui RESERVED0 : 30;
  } DPORT_CPU_PER_CONF_REG_Typedef;

  typedef __vo struct SYSCON_PLL_TICK_CONF_REG  {
    __vo ui SYSCON_PLL_TICK_NUM : 8;
    __vo ui RESERVED0 : 24;
  } SYSCON_PLL_TICK_CONF_REG_Typedef;

  /**
   * Ghi chú:
   * Các cấu trúc thanh ghi cần sử dụng sẽ được bổ sung thêm trong tương lai.
   */

  // Khai báo con trỏ tới thanh ghi

  typedef struct RTC_CNTL_REGS_PTRS {
    __vo BLANK_REG RESERVED0[42];
    __vo RTC_CNTL_CLK_CONF_REG_Typedef RTC_CNTL_CLK_CONF;
    __vo BLANK_REG RESERVED1[5];
  } RTC_CNTL_REGS_Typedef;

  typedef struct DPORT_REGS_PTRS {
    __vo BLANK_REG RESERVED0[15];
    __vo DPORT_CPU_PER_CONF_REG_Typedef DPORT_CPU_PERI_CLK_EN;
    __vo BLANK_REG RESERVED1[1]; 

    /**
     * Ghi chú:
     * Phần này kiểm tra lại trong tương lai
     */

  } DPORT_REGS_Typedef;

  // Khai báo các định nghĩa bit cần sử dụng 

#endif /* LIB_CLOCK_DEFINITION_H_ */