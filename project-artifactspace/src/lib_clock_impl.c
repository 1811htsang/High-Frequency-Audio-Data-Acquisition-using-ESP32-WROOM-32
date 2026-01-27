// Khai báo các thư viện sử dụng chung

  #ifdef UNIT_TEST
    /**
     * Ghi chú:
     * Chỗ này sẽ bổ sung sau
     */
  #endif

  #include <stdint.h>
  #include <stdio.h>
  #include <string.h>
  #include "lib_keyword_def.h"
  #include "clock/lib_clock_def.h"
  #include "clock/lib_clock_hal.h"

  #ifndef UNIT_TEST
    /**
     * Ghi chú:
     * Chỗ này sẽ bổ sung sau
     */
  #endif
  

// Định nghĩa các hàm thành phần

  RETR_STAT CLK_Init(CLK_Init_Param *init_param) {

    if (DEBUG_MODE == ENABLE) {
      printf("CLK_Init, DBG1: Check Null pointer.\n");
    }

      if (init_param == NULL) {
        return STAT_ERROR;
      }
    
    if (DEBUG_MODE == ENABLE) {
      printf("CLK_Init, DBG2: Assert parameter.\n");
    }

      assert_param(IS_CPU_CLK_SOURCE(init_param->Clk_Src));
      assert_param(IS_CPU_CLK_DRIVE(init_param->Clk_Drv));

    if (DEBUG_MODE == ENABLE) {
      printf("CLK_Init, DBG3: Setup procedure starts.\n");
    }

      /**
       * Ghi chú:
       * Trong bảng cấu hình trang 42,
       * Có 2 tham số lựa chọn chính là 
       * - RTC_CNTL_CLK_CONF_REG_SOC_CLK_SEL
       * - DPORT_CPU_PER_CONF_REG_CPU_PERIOD_SEL
       * Phụ thuộc vào từng lựa chọn của người dùng
       * Các tham số trong bảng sẽ được thiết lập tương ứng.
       * Các tham số dấu (-) nghĩa là không quan tâm.
       */

      switch (init_param->Clk_Src) {
        case CPU_CLK_SRC_PLL:

          /**
           * Ghi chú:
           * RTC_CNTL_CLK_CONF_REG_SOC_CLK_SEL = 01
           * DPORT_CPU_PER_CONF_REG_CPU_PERIOD_SEL = 02
           * Nếu không đúng cấu hình này thì báo lỗi
           */

          if (init_param->Clk_Drv != CPU_CLK_SRC_PLL_DRV_240MHZ) {
            return STAT_ERROR;
          }

          RTC_CNTL_REGS_PTR->RTC_CNTL_CLK_CONF_REG.RTC_CNTL_SOC_CLK_SEL = CPU_CLK_SRC_PLL;
          DPORT_REGS_PTR->DPORT_CPU_PER_CONF_REG.CPU_PERIOD_SEL = DPORT_CPU_PER_CONF_REG_CPU_PERIOD_SEL_240MHZ;

          return STAT_OK;
          break;

        case CPU_CLK_SRC_APLL:
          
          return STAT_OK;
          break;

        case RSC_CLK_SOURCE_RSC:
          
          return STAT_OK;
          break;
        
        case APLL_CLK:
          
          return STAT_OK;
          break;

        default:
          return STAT_ERROR;
          break;
      }

    return STAT_DONE;
  }

  RETR_STAT CLK_DeInit(CLK_Init_Param *init_param) {
    return STAT_DONE;
  }

  RETR_STAT Ref_Tick_Init(Ref_Tick_Init_Param *init_param) {
    return STAT_DONE;
  }