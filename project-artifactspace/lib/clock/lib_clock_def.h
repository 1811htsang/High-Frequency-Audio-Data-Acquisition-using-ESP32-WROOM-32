#ifndef LIB_CLOCK_DEF_H_
  #define LIB_CLOCK_DEF_H_

  /**
	 * Ghi chú: Ở đây,
	 * ESP32 chỉ được cấu hình để sử dụng các nguồn clock sau 
	 * cho hệ thống và ngoại vi
	 * - PLL_CLK (480MHz) -> CPU_CLK, APB_CLK, REF_TICK
	 * - RTC_SLOW_CLK (150kHz) -> WDT
	 * - APLL_CLK (nếu config) -> I2S, DAC, ADC
	 * - PLL_160M_CLK (160MHz) -> I2S
	 */

  // Khai báo các thư viện sử dụng chung

    #include "lib_keyword_def.h"

  // Khai báo địa chỉ ngoại vi

    #define RTC_CNTL_REGS_DPTBUS_BASEADDR 0x3FF48000ul
    #define RTC_CNTL_REGS_AHBBUS_BASEADDR 0x60008000ul
    #define DPORT_REGS_BASEADDR           0x3FF00000ul
    #define SYSCON_REGS_BASEADDR          0x3FF66000ul

    /**
     * Ghi chú:
     * Các ngoại vi cần sử dụng sẽ được bổ sung thêm trong tương lai.
     */

  // Khai báo địa chỉ thanh ghi cụ thể

    #define RTC_CNTL_CLK_CONF_REG_ADDR     (RTC_CNTL_REGS_DPTBUS_BASEADDR + 0x0070ul)
    #define DPORT_CPU_PERI_CLK_EN_REG_ADDR (DPORT_REGS_BASEADDR + 0x003Cul)
    #define SYSCON_CLK_CONF_REG_ADDR       (SYSCON_REGS_BASEADDR + 0x0008ul)

    /**
     * Ghi chú:
     * Các thanh ghi cần sử dụng sẽ được bổ sung thêm trong tương lai.
     */

  // Khai báo cấu trúc thanh ghi

    tdf_strc RTC_CNTL_CLK_CONF_REG {
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
      __vo ui RTC_CNTL_SOC_CLK_SEL : 2; // For CPU_CLK
      __vo ui RTC_CNTL_RTC_FAST_CLK_SEL : 1;
      __vo ui RTC_CNTL_ANA_CLK_RTC_SEL : 2; // For RTC_SLOW_CLK
    } RTC_CNTL_CLK_CONF_REG_Typedef;

    tdf_strc RTC_CNTL_ANA_CONF_REG {
      __vo ui RESERVED0 : 23;
      __vo ui RTC_CNTL_PLLA_FORCE_PD : 1; // For APLL_CLK
      __vo ui RTC_CNTL_PLLA_FORCE_PU : 1; // For APLL_CLK
      __vo ui RESERVED1 : 1;
      __vo ui RTC_CNTL_PVTMON_PU : 1;
      __vo ui RTC_CNTL_TXRF_I2C_PU : 1;
      __vo ui RTC_CNTL_RFRX_PBUS_PU : 1;
      __vo ui RESERVED2 : 1;
      __vo ui RTC_CNTL_CKGEN_I2C_PU : 1;
      __vo ui RTC_CNTL_PLL_I2C_PU : 1;
    } RTC_CNTL_ANA_CONF_REG_Typedef;

    tdf_strc DPORT_CPU_PER_CONF_REG {
      __vo ui CPU_PERIOD_SEL : 2;
      __vo ui RESERVED0 : 30;
    } DPORT_CPU_PER_CONF_REG_Typedef;

    tdf_strc SYSCON_PLL_TICK_CONF_REG  {
      __vo ui SYSCON_PLL_TICK_NUM : 8;
      __vo ui RESERVED0 : 24;
    } SYSCON_PLL_TICK_CONF_REG_Typedef;

    tdf_strc SYSCON_APLL_TICK_CONF_REG {
      __vo ui SYSCON_APLL_TICK_NUM : 8;
      __vo ui RESERVED0 : 24;
    } SYSCON_APLL_TICK_CONF_REG_Typedef;

    /**
     * Ghi chú:
     * Các cấu trúc thanh ghi cần sử dụng sẽ được bổ sung thêm trong tương lai.
     */

  // Khai báo cấu trúc ngoại vi

    tdf_strc RTC_CNTL_REGS_PTRS {
      __vo BLANK_REG RESERVED0[12]; // 0x3FF48000 - 0x3FF4802C
      __vo RTC_CNTL_ANA_CONF_REG_Typedef RTC_CNTL_ANA_CONF_REG;
      __vo BLANK_REG RESERVED1[14]; // 0x3FF48034 - 0x3FF4806C
      __vo RTC_CNTL_CLK_CONF_REG_Typedef RTC_CNTL_CLK_CONF_REG;
      __vo BLANK_REG RESERVED2[25]; // 0x3FF48074 - 0x3FF480D4
    } RTC_CNTL_REGS_Typedef;

    tdf_strc DPORT_REGS_PTRS {
      __vo BLANK_REG RESERVED0[15]; // 0x3FF00000 - 0x3FF00038
      __vo DPORT_CPU_PER_CONF_REG_Typedef DPORT_CPU_PER_CONF_REG;
      __vo BLANK_REG RESERVED1[347]; // 0x3FF00040 - 0x3FF05A8
    } DPORT_REGS_Typedef;

    tdf_strc SYSCON_REGS_PTRS {
      __vo BLANK_REG RESERVED0[1]; // 0x3FF66000 - 0x3FF66004
      __vo SYSCON_PLL_TICK_CONF_REG_Typedef SYSCON_PLL_TICK_CONF_REG;
      __vo BLANK_REG RESERVED1[12]; // 0x3FF6600C - 0x3FF66038
      __vo SYSCON_APLL_TICK_CONF_REG_Typedef SYSCON_APLL_TICK_CONF_REG;
      __vo BLANK_REG RESERVED2[16]; // 0x3FF66040 - 0x3FF6607C
    } SYSCON_REGS_Typedef;  

    /**
     * Ghi chú:
     * Các cấu trúc ngoại vi cần sử dụng sẽ được bổ sung thêm trong tương lai.
     */

  // >> Tạo con trỏ phần cứng thật và con trỏ phần cứng giả tới ngoại vi

    #ifndef UNIT_TEST
			#define RTC_CNTL_REGS_PTR ((RTC_CNTL_REGS_Typedef *)RTC_CNTL_REGS_DPTBUS_BASEADDR)
      #define DPORT_REGS_PTR    ((DPORT_REGS_Typedef *)DPORT_REGS_BASEADDR)
      #define SYSCON_REGS_PTR   ((SYSCON_REGS_Typedef *)SYSCON_REGS_BASEADDR)
		#else
			extern RTC_CNTL_REGS_PTR MOCK_RTC_CNTL_REGS;
      #define RTC_CNTL_REGS_PTR (&MOCK_RTC_CNTL_REGS)
      
      extern DPORT_REGS_PTR MOCK_DPORT_REGS;
      #define DPORT_REGS_PTR (&MOCK_DPORT_REGS)

      extern SYSCON_REGS_PTR MOCK_SYSCON_REGS;
      #define SYSCON_REGS_PTR (&MOCK_SYSCON_REGS)
		#endif

  // Khai báo các định nghĩa bit cần sử dụng trên RTC_CNTL_CLK_CONF_REG

    #define RTC_CNTL_CLK_CONF_REG_ANA_CLK_RTC_SEL_RSC     0x00ul
    #define RTC_CNTL_CLK_CONF_REG_ANA_CLK_RTC_SEL_XTAL32K 0x01ul
    #define RTC_CNTL_CLK_CONF_REG_ANA_CLK_RTC_SEL_RCFD    0x02ul
    
    #define RTC_CNTL_CLK_CONF_REG_SOC_CLK_SEL_XTAL 0x00ul
    #define RTC_CNTL_CLK_CONF_REG_SOC_CLK_SEL_PLL  0x01ul
    #define RTC_CNTL_CLK_CONF_REG_SOC_CLK_SEL_CK8M 0x02ul
    #define RTC_CNTL_CLK_CONF_REG_SOC_CLK_SEL_APLL 0x03ul

  // Khai báo các định nghĩa bit cần sử dụng trên RTC_CNTL_ANA_CONF_REG
  
    #define RTC_CNTL_ANA_CONF_REG_APLL_ENABLE  0x01ul
    #define RTC_CNTL_ANA_CONF_REG_APLL_DISABLE 0x01ul

    /**
     * Ghi chú:
     * Quyền tắt được ưu tiên so với quyền bật.
     */

  // Khai báo các định nghĩa bit cần sử dụng trên DPORT_CPU_PER_CONF_REG
  
    #define DPORT_CPU_PER_CONF_REG_CPU_PERIOD_SEL_80MHZ_APLL_DIV4  0x00ul
    #define DPORT_CPU_PER_CONF_REG_CPU_PERIOD_SEL_160MHZ_APLL_DIV2 0x01ul
    #define DPORT_CPU_PER_CONF_REG_CPU_PERIOD_SEL_240MHZ           0x02ul

    /**
     * Ghi chú:
     * Các định nghĩa bit cần sử dụng sẽ được bổ sung thêm trong tương lai.
     */

  // Khai báo các định nghĩa bit cần sử dụng trên SYSCON_PLL_TICK_CONF_REG
  
    /**
     * Ghi chú:
     * Do nguồn xung clock cho CPU_CLK tùy thuộc vào cấu hình của người dùng,
     * nên giá trị tick tham chiếu cho CPU_CLK sẽ được tính toán bởi giá trị
     * người dùng cấu hình.
     */

  // Khai báo các định nghĩa bit cần sử dụng trên SYSCON_APLL_TICK_CONF_REG

    /**
     * Ghi chú:
     * Do nguồn xung clock cho CPU_CLK tùy thuộc vào cấu hình của người dùng,
     * nên giá trị tick tham chiếu cho CPU_CLK sẽ được tính toán bởi giá trị
     * người dùng cấu hình.
     */

  // Khai báo các nguồn clock khởi tạo

    #define RTC_SLOW_CLK_SOURCE_RSC      RTC_CNTL_CLK_CONF_REG_ANA_CLK_RTC_SEL_RSC
    #define RTC_SLOW_CLK_SOURCE_XTAL32K  RTC_CNTL_CLK_CONF_REG_ANA_CLK_RTC_SEL_XTAL32K
    #define RTC_SLOW_CLK_SOURCE_RCFD     RTC_CNTL_CLK_CONF_REG_ANA_CLK_RTC_SEL_RCFD

    #define APLL_CLK                     0x0Ful 

    /**
     * Ghi chú:
     * Do APLL_CLK không nằm trong cấu hình của CPU_CLK
     * mà nằm trong cấu hình cho I2S, DAC, ADC
     * Do đó khai báo này chỉ mang tính chất tham khảo.
     * Cấu hình bật thực tế sẽ do RTC_CNTL_ANA_CONF_REG_APLL_ENABLE đảm nhiệm.
     */

    #define CPU_CLK_SRC_XTAL             RTC_CNTL_CLK_CONF_REG_SOC_CLK_SEL_XTAL
    #define CPU_CLK_SRC_PLL              RTC_CNTL_CLK_CONF_REG_SOC_CLK_SEL_PLL
    #define CPU_CLK_SRC_RFC              RTC_CNTL_CLK_CONF_REG_SOC_CLK_SEL_CK8M
    #define CPU_CLK_SRC_APLL             RTC_CNTL_CLK_CONF_REG_SOC_CLK_SEL_APLL

    #define CPU_CLK_SRC_PLL_DRV_80MHZ    DPORT_CPU_PER_CONF_REG_CPU_PERIOD_SEL_80MHZ_APLL_DIV4
    #define CPU_CLK_SRC_PLL_DRV_160MHZ   DPORT_CPU_PER_CONF_REG_CPU_PERIOD_SEL_160MHZ_APLL_DIV2
    #define CPU_CLK_SRC_PLL_DRV_240MHZ   DPORT_CPU_PER_CONF_REG_CPU_PERIOD_SEL_240MHZ

    #define CPU_CLK_SRC_APLL_DRV_DIV4    DPORT_CPU_PER_CONF_REG_CPU_PERIOD_SEL_80MHZ_APLL_DIV4
    #define CPU_CLK_SRC_APLL_DRV_DIV2    DPORT_CPU_PER_CONF_REG_CPU_PERIOD_SEL_160MHZ_APLL_DIV2
  
  // Khai báo giá trị lựa chọn clock

    #define CPU_CLK_SOURCE_XTAL     CPU_CLK_SRC_XTAL       // 0x00U
    #define CPU_CLK_SOURCE_PLL      CPU_CLK_SRC_PLL        // 0x01U
    #define CPU_CLK_SOURCE_RCF      CPU_CLK_SRC_RFC        // 0x02U
    #define CPU_CLK_SOURCE_APLL     CPU_CLK_SRC_APLL       // 0x03U
    #define RSC_CLK_SOURCE_RSC      0x04ul                 // RTC_SLOW_CLK_SOURCE_RSC = 0x00U
    #define RSC_CLK_SOURCE_XTAL32K  0x05ul                 // RTC_SLOW_CLK_SOURCE_XTAL32K = 0x01U
    #define RSC_CLK_SOURCE_RCFD     0x06ul                 // RTC_SLOW_CLK_SOURCE_RCFD = 0x02U
    #define APLL_CLK_SOURCE         APLL_CLK               // 0x0FU

  // Khai báo giá trị chia clock

    #define NONE                            0x00ul
    #define CPU_CLK_SOURCE_PLL_DRV_80MHZ    0x01ul     // CPU_CLK_SRC_PLL_DRV_80MHZ = 0x00U
    #define CPU_CLK_SOURCE_PLL_DRV_160MHZ   0x02ul     // CPU_CLK_SRC_PLL_DRV_160MHZ = 0x01U
    #define CPU_CLK_SOURCE_PLL_DRV_240MHZ   0x03ul     // CPU_CLK_SRC_PLL_DRV_240MHZ = 0x02U
    #define CPU_CLK_SOURCE_APLL_DRV_DIV4    0x04ul     // CPU_CLK_SRC_APLL_DRV_DIV4 = 0x00U
    #define CPU_CLK_SOURCE_APLL_DRV_DIV2    0x05ul     // CPU_CLK_SRC_APLL_DRV_DIV2 = 0x01U

  // Khai báo tần số CPU
    
    #define CPU_SEL_PLL   80000000ul
    #define CPU_SEL_APLL  40000000ul // Giá trị lấy trong Audio PLL do không có thông tin cụ thể

#endif /* LIB_CLOCK_DEF_H_ */