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
  #include "esp_clk_internal.h"

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
      printf("CLK_Init, DBG3: Setup clock selection.\n");
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

          switch (init_param->Clk_Drv) {
            case CPU_CLK_SRC_PLL_DRV_80MHZ:
              RTC_CNTL_REGS_PTR->RTC_CNTL_CLK_CONF_REG.RTC_CNTL_SOC_CLK_SEL = CPU_CLK_SRC_PLL;
              DPORT_REGS_PTR->DPORT_CPU_PER_CONF_REG.CPU_PERIOD_SEL = CPU_CLK_SRC_PLL_DRV_80MHZ;
              break;

            case CPU_CLK_SRC_PLL_DRV_160MHZ:
              RTC_CNTL_REGS_PTR->RTC_CNTL_CLK_CONF_REG.RTC_CNTL_SOC_CLK_SEL = CPU_CLK_SRC_PLL;
              DPORT_REGS_PTR->DPORT_CPU_PER_CONF_REG.CPU_PERIOD_SEL = CPU_CLK_SRC_PLL_DRV_160MHZ;
              break;

            case CPU_CLK_SRC_PLL_DRV_240MHZ:
              RTC_CNTL_REGS_PTR->RTC_CNTL_CLK_CONF_REG.RTC_CNTL_SOC_CLK_SEL = CPU_CLK_SRC_PLL;
              DPORT_REGS_PTR->DPORT_CPU_PER_CONF_REG.CPU_PERIOD_SEL = CPU_CLK_SRC_PLL_DRV_240MHZ;
              break;
            
            default:
              return STAT_ERROR;
              break;
          }

          if (DEBUG_MODE == ENABLE) {
            printf("CLK_Init, DBG4: Setup REF_TICK 1MHz.\n");
          }

            Ref_Tick_Init(init_param, 1000000ul); // Cấu hình tick tham chiếu 1MHz

          return STAT_OK;
          break;

        case CPU_CLK_SRC_APLL:

          /**
           * Ghi chú:
           * RTC_CNTL_CLK_CONF_REG_SOC_CLK_SEL = 11
           * DPORT_CPU_PER_CONF_REG_CPU_PERIOD_SEL = 00
           * Nếu không đúng cấu hình này thì báo lỗi
           */

          switch (init_param->Clk_Drv) {
            case CPU_CLK_SRC_APLL_DRV_DIV4:
              RTC_CNTL_REGS_PTR->RTC_CNTL_CLK_CONF_REG.RTC_CNTL_SOC_CLK_SEL = CPU_CLK_SRC_APLL;
              DPORT_REGS_PTR->DPORT_CPU_PER_CONF_REG.CPU_PERIOD_SEL = DPORT_CPU_PER_CONF_REG_CPU_PERIOD_SEL_80MHZ_APLL_DIV4;
              break;
            
            case CPU_CLK_SRC_APLL_DRV_DIV2:
              RTC_CNTL_REGS_PTR->RTC_CNTL_CLK_CONF_REG.RTC_CNTL_SOC_CLK_SEL = CPU_CLK_SRC_APLL;
              DPORT_REGS_PTR->DPORT_CPU_PER_CONF_REG.CPU_PERIOD_SEL = DPORT_CPU_PER_CONF_REG_CPU_PERIOD_SEL_160MHZ_APLL_DIV2;
              break;
            
            default:
              return STAT_ERROR;
              break;
          }

          if (DEBUG_MODE == ENABLE) {
            printf("CLK_Init, DBG4: Setup REF_TICK 1MHz.\n");
          }

            Ref_Tick_Init(init_param, 1000000ul); // Cấu hình tick tham chiếu 1MHz
          
          return STAT_OK;
          break;

        case RSC_CLK_SOURCE_RSC:

          /**
           * Ghi chú:
           * RTC_CNTL_CLK_CONF_REG_ANA_CLK_RTC_SEL = 00
           * Không cần cấu hình DPORT_CPU_PER_CONF_REG_CPU_PERIOD_SEL
           */

          RTC_CNTL_REGS_PTR->RTC_CNTL_CLK_CONF_REG.RTC_CNTL_ANA_CLK_RTC_SEL = RTC_SLOW_CLK_SOURCE_RSC;
          
          return STAT_OK;
          break;
        
        case APLL_CLK:

          /**
           * Ghi chú:
           * RTC_CNTL_ANA_CONF_REG_PLLA_FORCE_PU = 1
           * Không cần cấu hình RTC_CNTL_CLK_CONF_REG_SOC_CLK_SEL
           */

          RTC_CNTL_REGS_PTR->RTC_CNTL_ANA_CONF_REG.RTC_CNTL_PLLA_FORCE_PU = SET;
          
          return STAT_OK;
          break;

        default:
          return STAT_ERROR;
          break;
      }
    
    if (DEBUG_MODE == ENABLE) {
      printf("CLK_Init, DBG4: Setup procedure ends.\n");
    }

      return STAT_DONE;
  }

  RETR_STAT CLK_DeInit(CLK_Init_Param *init_param) {

    /**
     * Ghi chú:
     * Hàm này chỉ áp dụng đối với APLL
     * - nguồn clock CPU không nên bị tắt đi
     * - nguồn clock RTC_SLOW ở trạng thái reset state
     *   cũng đã ràng buộc 1 nguồn clock rồi.
     */

    if (DEBUG_MODE == ENABLE) {
      printf("CLK_DeInit, DBG1: Check Null pointer.\n");
    }

      if (init_param == NULL) {
        return STAT_ERROR;
      }

    if (DEBUG_MODE == ENABLE) {
      printf("CLK_DeInit, DBG2: Assert parameter.\n");
    }

      assert_param(IS_CPU_CLK_SOURCE(init_param->Clk_Src));

      /**
       * Ghi chú:
       * Đối với các nguồn clock khác,
       * Không có thao tác chia nguồn nên không cần xử lý
       */

    if (DEBUG_MODE == ENABLE) {
      printf("CLK_DeInit, DBG3: DeInit procedure starts.\n");
    }

      switch (init_param->Clk_Src) {        
        case APLL_CLK:

          /**
           * Ghi chú:
           * RTC_CNTL_ANA_CONF_REG_PLLA_FORCE_PD = 1
           * Không cần cấu hình RTC_CNTL_CLK_CONF_REG_SOC_CLK_SEL
           */

          RTC_CNTL_REGS_PTR->RTC_CNTL_ANA_CONF_REG.RTC_CNTL_PLLA_FORCE_PD = SET;
          
          break;

        default:
          return STAT_ERROR;
          break;
      }

    if (DEBUG_MODE == ENABLE) {
      printf("CLK_DeInit, DBG4: DeInit procedure ends.\n");
    }

      return STAT_DONE;
  }

  RETR_STAT Ref_Tick_Init(CLK_Init_Param *init_param, ul ref_tick_value) {

    /**
     * Ghi chú:
     * Hàm này sẽ cấu hình tần số tick tham chiếu
     * cho CPU_CLK dựa trên nguồn clock đã được cấu hình
     * trong hàm CLK_Init.
     * 
     * Các nguồn clock dùng cho REF_TICK bao gồm:
     * - PLL_CLK (480MHz)
     * - APLL_CLK (nếu được cấu hình)
     * - XTAL_CLK (40MHz) không sử dụng tới
     * - RCF_CLK (8MHz) không sử dụng tới
     * 
     * Tick tham chiếu tối đa là 1MHz.
     */
    
    if (DEBUG_MODE == ENABLE) {
      printf("Ref_Tick_Init, DBG1: Assert parameter.\n");
    }

      assert_param(IS_REF_TICK_FREQ(init_param->Tick_Freq));

      /**
       * Ghi chú:
       * Do hàm này chỉ phục vụ cho việc cấu hình
       * tần số tick tham chiếu nên đầu vào init_param
       * đã được cấu hình trong hàm CLK_Init.
       * Vì vậy, không cần kiểm tra nguồn clock ở đây.
       */

    if (DEBUG_MODE == ENABLE) {
      printf("Ref_Tick_Init, DBG2: Setup Ref_Tick value.\n");
    }

      switch (init_param->Clk_Src) {
        case CPU_CLK_SRC_PLL:
          
          ui tick_config_pll = (CPU_SEL_PLL / ref_tick_value) - 1ul;
          SYSCON_REGS_PTR->SYSCON_PLL_TICK_CONF_REG.SYSCON_PLL_TICK_NUM = tick_config_pll;

          return STAT_OK;
          break;

        case CPU_CLK_SRC_APLL:

          ui tick_config_apll = ((CPU_SEL_APLL / 2ul) / ref_tick_value) - 1ul;
          SYSCON_REGS_PTR->SYSCON_APLL_TICK_CONF_REG.SYSCON_APLL_TICK_NUM = tick_config_apll;
          
          return STAT_OK;
          break;
    

        default:
          return STAT_ERROR;
          break;
      }

    return STAT_DONE;
  }