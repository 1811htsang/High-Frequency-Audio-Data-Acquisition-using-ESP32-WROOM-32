#ifndef LIB_CLOCK_HAL_H_
  #define LIB_CLOCK_HAL_H_

  // Khai báo các thư viện sử dụng chung

    #include "lib_keyword_def.h"
    #include "clock/lib_clock_def.h"

  // Khai báo cấu trúc tham số hàm khởi tạo

    tdf_strc {
      ul Clk_Src; // Chọn nguồn clock
      ul Clk_Drv; // Chọn phân chia clock 
    } CLK_Init_Param;

    tdf_strc {
      ul Tick_Freq; // Chọn tần số tick tham chiếu
    } Ref_Tick_Init_Param;

  // Khai báo các kiểm tra tham số đầu vào nội bộ

  #define IS_CPU_CLK_SOURCE(SOURCE) (((SOURCE) == CPU_CLK_SRC_XTAL) || \
                                      ((SOURCE) == CPU_CLK_SRC_PLL) || \
                                      ((SOURCE) == CPU_CLK_SRC_RCF) || \
                                      ((SOURCE) == CPU_CLK_SRC_APLL) || \
                                      ((SOURCE) == RCS_CLK_SOURCE_RSC) || \
                                      ((SOURCE) == RCS_CLK_SOURCE_XTAL32K) || \
                                      ((SOURCE) == RCS_CLK_SOURCE_RCFD) || \
                                      ((SOURCE) == APLL_CLK))

  #define IS_CPU_CLK_DRIVE(DRIVE) (((DRIVE) == NONE) || \
                                    ((DRIVE) == CPU_CLK_SRC_PLL_DRV_80MHZ) || \
                                    ((DRIVE) == CPU_CLK_SRC_PLL_DRV_160MHZ) || \
                                    ((DRIVE) == CPU_CLK_SRC_PLL_DRV_240MHZ) || \
                                    ((DRIVE) == CPU_CLK_SRC_APLL_DRV_DIV4) || \
                                    ((DRIVE) == CPU_CLK_SRC_APLL_DRV_DIV2))

  #define IS_REF_TICK_FREQ(FREQ) ((FREQ) <= 1000000ul) // Tối đa 1MHz

  // Khai báo các hàm thành phần

    // >> Hàm init clock 
    RETR_STAT CLK_Init(CLK_Init_Param *init_param);

    // >> Hàm de-init clock
    RETR_STAT CLK_DeInit(CLK_Init_Param *init_param);

    // >> Hàm khởi tạo REF_TICK
    RETR_STAT Ref_Tick_Init(CLK_Init_Param *init_param, ul ref_tick_value);

#endif