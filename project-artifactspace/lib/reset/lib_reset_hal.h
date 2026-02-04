#ifndef LIB_RESET_HAL_H_
  #define LIB_RESET_HAL_H_

  // Khai báo các thư viện sử dụng chung

    #include "lib_keyword_def.h"
    #include "reset/lib_reset_def.h"

  // Khai báo các cấu trúc tham số hàm

    tdf_strc {
      ui CPU_SEL; // Chọn CPU cần reset
    } RST_Inp_Param;
  
  // Khai báo cấu trúc catch reset

    tdf_strc RST_FLG_Typdef {
			ui IsPorReset;          // Cờ đánh dấu system reset bởi nguồn điện (POR)
			ui IsRwdtSysReset;      // Cờ đánh dấu system reset bởi RWDT (RWDT_SYS)
			ui IsBrownOutReset;     // Cờ đánh dấu system reset bởi brownout (BROWNOUT)
      ui IsSwSysReset;        // Cờ đánh dấu core reset bởi phần mềm (SW_SYS)
			ui IsDeepSleepReset;    // Cờ đánh dấu core reset bởi chế độ ngủ sâu (DEEP SLEEP)
      ui IsSdioReset;         // Cờ đánh dấu core reset bởi SDIO (SDIO)
      ui IsMwdt0GloReset;     // Cờ đánh dấu system reset bởi MWDT0 toàn cục (MWDT0_GLO)
      ui IsMwdt1GloReset;     // Cờ đánh dấu system reset bởi MWDT1 toàn cục (MWDT1_GLO)
      ui IsRwdtCoreReset;     // Cờ đánh dấu core reset bởi RWDT core (RWDT_CORE)
      ui IsRwdtCpuReset;      // Cờ đánh dấu CPU reset bởi RWDT CPU (RWDT_CPU)
      ui IsProMwdt0CpuReset;  // Cờ đánh dấu PRO CPU reset bởi MWDT0 CPU (PRO_MWDT0_CPU)
      ui IsProSwCpuReset;     // Cờ đánh dấu PRO CPU reset bởi SW CPU (PRO_SW_CPU)
      ui IsAppMwdt1CpuReset;  // Cờ đánh dấu APP CPU reset bởi MWDT1 CPU (APP_MWDT1_CPU)
      ui IsAppSwCpuReset;     // Cờ đánh dấu APP CPU reset bởi SW CPU (APP_SW_CPU)
      ui IsAppCpuProCpuReset; // Cờ đánh dấu APP CPU reset bởi PRO CPU (APP_CPU_PRO_CPU)
		} RST_FLG_Typdef;

  // Khai báo các hàm thành phần

    // >> Hàm thực hiện system reset bởi phần mềm
    void RST_SYS_SW_Reset(void);

    // >> Hàm thực hiện cpu reset bởi phần mềm
    void RST_CPU_SW_Reset(RST_Inp_Param *param);

    // >> Hàm catch reset event sau khi khởi động lại hệ thống
    void RST_SRC_Capture(RST_FLG_Typdef *reset_source);

#endif