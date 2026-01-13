#ifndef LIB_CLOCK_HAL_H_
  #define LIB_CLOCK_HAL_H_

  // Khai báo các thư viện sử dụng chung

  #include "lib/lib_keyword_define.h"

  // Khai báo cấu trúc tham số hàm khởi tạo

  typedef enum
  {
    CPU_CLK_SRC_XTAL        = 0x00U,
    CPU_CLK_SRC_PLL         = 0x01U,
    CPU_CLK_SRC_RCF         = 0x02U,
    CPU_CLK_SRC_APLL        = 0x03U
  } CPU_CLK_Source;

  typedef enum
  {
    CPU_CLK_SRC_PLL_DRV_80MHZ   = 0x00U,
    CPU_CLK_SRC_PLL_DRV_160MHZ  = 0x01U,
    CPU_CLK_SRC_PLL_DRV_240MHZ  = 0x02U,
    CPU_CLK_SRC_APLL_DRV_DIV4   = 0x03U,
    CPU_CLK_SRC_APLL_DRV_DIV2   = 0x04U
  } CPU_CLK_Drive;

  typedef struct
  {
    CPU_CLK_Source Clk_Src; // Chọn nguồn clock hệ thống
    CPU_CLK_Drive Clk_Drv; // Chọn phân chia clock hệ thống
  } CPU_CLK_Init_Param;

  typedef struct 
  {
    ul Tick_Freq; // Chọn tần số tick tham chiếu
  } Ref_Tick_Init_Param;

  // Khai báo các kiểm tra tham số đầu vào nội bộ

  #define IS_CPU_CLK_SOURCE(SOURCE) (((SOURCE) == CPU_CLK_SRC_XTAL) || \
                                      ((SOURCE) == CPU_CLK_SRC_PLL) || \
                                      ((SOURCE) == CPU_CLK_SRC_RCF) || \
                                      ((SOURCE) == CPU_CLK_SRC_APLL))

  #define IS_CPU_CLK_DRIVE(DRIVE) (((DRIVE) == CPU_CLK_SRC_PLL_DRV_80MHZ) || \
                                   ((DRIVE) == CPU_CLK_SRC_PLL_DRV_160MHZ) || \
                                   ((DRIVE) == CPU_CLK_SRC_PLL_DRV_240MHZ) || \
                                   ((DRIVE) == CPU_CLK_SRC_APLL_DRV_DIV4) || \
                                   ((DRIVE) == CPU_CLK_SRC_APLL_DRV_DIV2))

  #define IS_REF_TICK_FREQ(FREQ) ((FREQ) <= 1000000ul) // Tối đa 1MHz

  // Khai báo các hàm thành phần

  // >> Hàm init clock hệ thống
  RETR_STAT CPU_CLK_Init(CPU_CLK_Init_Param *init_param);

  // >> Hàm de-initialize clock hệ thống
  RETR_STAT CPU_CLK_DeInit(CPU_CLK_Init_Param *init_param);

  // >> Hàm khởi tạo REF_TICK
  RETR_STAT Ref_Tick_Init(Ref_Tick_Init_Param *init_param);

#endif