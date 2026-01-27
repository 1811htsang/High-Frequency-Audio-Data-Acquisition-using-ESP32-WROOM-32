#ifndef LIB_CLOCK_HAL_H_
  #define LIB_CLOCK_HAL_H_

  // Khai báo các thư viện sử dụng chung

    #include "lib_keyword_def.h"
    #include "clock/lib_clock_def.h"

  // Khai báo cấu trúc tham số hàm khởi tạo

    /**
     * Ghi chú:
     * ở đây có sinh ra lỗi khi hiển thị,
     * tuy nhiên khi biên dịch thì không có lỗi gì xảy ra.
     * Nên tạm thời bỏ qua lỗi này.
     */

    tdf_enm {
      CPU_CLK_SRC_XTAL        = 0x00U,
      CPU_CLK_SRC_PLL         = 0x01U,
      CPU_CLK_SRC_RFC         = 0x02U,
      CPU_CLK_SRC_APLL        = 0x03U,
      RSC_CLK_SOURCE_RSC      = 0x04U,
      RSC_CLK_SOURCE_XTAL32K  = 0x05U,
      RSC_CLK_SOURCE_RCFD     = 0x06U,
      APLL_CLK                = 0x07U
    } CLK_Source;

    tdf_enm {
      NONE                        = 0x00U, 
      CPU_CLK_SRC_PLL_DRV_80MHZ   = 0x01U,
      CPU_CLK_SRC_PLL_DRV_160MHZ  = 0x02U,
      CPU_CLK_SRC_PLL_DRV_240MHZ  = 0x03U,
      CPU_CLK_SRC_APLL_DRV_DIV4   = 0x04U,
      CPU_CLK_SRC_APLL_DRV_DIV2   = 0x05U

      /**
       * Ghi chú:
       * Đối với RSC và APLL thì không có phân chia clock.
       */

    } CLK_Drive;

    tdf_strc {
      CLK_Source Clk_Src; // Chọn nguồn clock 
      CLK_Drive Clk_Drv; // Chọn phân chia clock 
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
    RETR_STAT Ref_Tick_Init(Ref_Tick_Init_Param *init_param);

#endif