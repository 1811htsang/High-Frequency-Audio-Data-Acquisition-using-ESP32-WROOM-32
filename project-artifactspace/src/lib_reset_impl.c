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
  #include "lib/lib_keyword_def.h"
  #include "lib/lib_condition_def.h"
  #include "lib/reset/lib_reset_def.h"
  #include "lib/reset/lib_reset_hal.h"

  #ifndef UNIT_TEST
    /**
     * Ghi chú:
     * Chỗ này sẽ bổ sung sau
     */
  #endif

// Định nghĩa các hàm thành phần

  void RST_SYS_SW_Reset(void) {

    if (__DEBUG_GET_MODE(ENABLE)) {
      printf("RST_SYS_SW_Reset, DBG1: Setup system software reset.\n");
    }

      RTC_CNTL_REGS_PTR->RTC_CNTL_OPTIONS0_REG.RTC_CNTL_SW_SYS_RST = RTC_CNTL_SW_SYS_RST_SET;

      while (1) {
        /**
         * Ghi chú:
         * Chờ đợi hệ thống được reset lại
         */
      }
  }

  void RST_CPU_SW_Reset(RST_Inp_Param *param) {

    if (__DEBUG_GET_MODE(ENABLE)) {
      printf("RST_CPU_SW_Reset, DBG1: Check Null pointer.\n");
    }

      if (__NULL_PTR_CHECK(param)) {
        return;
      }

    if (__DEBUG_GET_MODE(ENABLE)) {
      printf("RST_CPU_SW_Reset, DBG2: Assert parameter.\n");
    }

      assert_param((param->CPU_SEL == CPU_PRO) || (param->CPU_SEL == CPU_APP));

    if (__DEBUG_GET_MODE(ENABLE)) {
      printf("RST_CPU_SW_Reset, DBG3: Setup CPU software reset.\n");
    }

      if (param->CPU_SEL == PRO_CPU) {
        RTC_CNTL_REGS_PTR->RTC_CNTL_OPTIONS0_REG.RTC_CNTL_SW_PROCPU_RST = RTC_CNTL_SW_PROCPU_RST_SET;
      } else {
        RTC_CNTL_REGS_PTR->RTC_CNTL_OPTIONS0_REG.RTC_CNTL_SW_APPCPU_RST = RTC_CNTL_SW_APPCPU_RST_SET;
      }

      while (1) {
        /**
         * Ghi chú:
         * Chờ đợi CPU được reset lại
         */
      }
  }

  void RST_SRC_Capture(RST_FLG_Typdef *reset_set) {
    
    if (__DEBUG_GET_MODE(ENABLE)) {
      printf("RST_SRC_Capture, DBG1: Check Null pointer.\n");
    }

      if (__NULL_PTR_CHECK(reset_set)) {
        return;
      }

    if (__DEBUG_GET_MODE(ENABLE)) {
      printf("RST_SRC_Capture, DBG2: Capture reset source.\n");
    }

      ui pro_cpu_reset_cause = RTC_CNTL_REGS_PTR->RTC_CNTL_RESET_STATE_REG.RTC_CNTL_RESET_CAUSE_PROCPU;
      ui app_cpu_reset_cause = RTC_CNTL_REGS_PTR->RTC_CNTL_RESET_STATE_REG.RTC_CNTL_RESET_CAUSE_APPCPU;

      // Kiểm tra từng nguyên nhân reset và gán giá trị vào cấu trúc reset_set
      reset_set->IsPorReset = (
        __EQUAL_CHECK(pro_cpu_reset_cause, app_cpu_reset_cause) 
        && 
        __EQUAL_CHECK(pro_cpu_reset_cause, RST_POR)
      ) ? SET : RESET;
      reset_set->IsRwdtSysReset = (
        __EQUAL_CHECK(pro_cpu_reset_cause, app_cpu_reset_cause) 
        &&
        __EQUAL_CHECK(pro_cpu_reset_cause, RST_RWDT_SYS)
      ) ? SET : RESET;
      reset_set->IsBrownOutReset = (
        __EQUAL_CHECK(pro_cpu_reset_cause, app_cpu_reset_cause) 
        &&
        __EQUAL_CHECK(pro_cpu_reset_cause, RST_BROWNOUT)
      ) ? SET : RESET;
      reset_set->IsSwSysReset = (
        __EQUAL_CHECK(pro_cpu_reset_cause, app_cpu_reset_cause) 
        &&
        __EQUAL_CHECK(pro_cpu_reset_cause, RST_SW_SYS)
      ) ? SET : RESET;
      reset_set->IsDeepSleepReset = (
        __EQUAL_CHECK(pro_cpu_reset_cause, app_cpu_reset_cause) 
        &&
        __EQUAL_CHECK(pro_cpu_reset_cause, RST_DEEPSLEEP)
      ) ? SET : RESET;
      reset_set->IsSdioReset = (
        __EQUAL_CHECK(pro_cpu_reset_cause, app_cpu_reset_cause) 
        &&
        __EQUAL_CHECK(pro_cpu_reset_cause, RST_SDIO)
      ) ? SET : RESET;
      reset_set->IsMwdt0GloReset = (
        __EQUAL_CHECK(pro_cpu_reset_cause, app_cpu_reset_cause) 
        &&
        __EQUAL_CHECK(pro_cpu_reset_cause, RST_MWDT0_GLO)
      ) ? SET : RESET;
      reset_set->IsMwdt1GloReset = (
        __EQUAL_CHECK(pro_cpu_reset_cause, app_cpu_reset_cause) 
        &&
        __EQUAL_CHECK(pro_cpu_reset_cause, RST_MWDT1_GLO)
      ) ? SET : RESET;
      reset_set->IsRwdtCoreReset = (
        __EQUAL_CHECK(pro_cpu_reset_cause, app_cpu_reset_cause) 
        &&
        __EQUAL_CHECK(pro_cpu_reset_cause, RST_RWDT_CORE)
      ) ? SET : RESET;
      reset_set->IsRwdtCpuReset = (
        __EQUAL_CHECK(pro_cpu_reset_cause, app_cpu_reset_cause) 
        &&
        __EQUAL_CHECK(pro_cpu_reset_cause, RST_RWDT_CPU)
      ) ? SET : RESET;
      reset_set->IsProMwdt0CpuReset = (
        __EQUAL_CHECK(pro_cpu_reset_cause, RST_PRO_MWDT0_CPU)
      ) ? SET : RESET;
      reset_set->IsProSwCpuReset = (
        __EQUAL_CHECK(pro_cpu_reset_cause, RST_PRO_SW_CPU)
      ) ? SET : RESET;
      reset_set->IsAppMwdt1CpuReset = (
        __EQUAL_CHECK(app_cpu_reset_cause, RST_APP_MWDT1_CPU)
      ) ? SET : RESET;
      reset_set->IsAppSwCpuReset = (
        __EQUAL_CHECK(app_cpu_reset_cause, RST_APP_SW_CPU)
      ) ? SET : RESET;
      reset_set->IsAppCpuProCpuReset = (
        __EQUAL_CHECK(app_cpu_reset_cause, RST_APP_CPU_PRO_CPU)
      ) ? SET : RESET;

      /**
       * Ghi chú:
       * Do không tìm thấy thông tin về việc xóa các cờ reset trong RTC_CNTL_RESET_STATE_REG,
       * Nên hàm này sẽ không thực hiện xóa các cờ reset đã đọc
       */
  }