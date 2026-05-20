/**
 * @file server_main.c
 * @author Shang Huang 
 * @brief Source code for interacting with server 
 * @version 0.1
 * @date 2026-05-20
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "esp_system.h"
#include "soc/clk_tree_defs.h"
#include "esp_clk_tree.h"
#include "esp_pm.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lib_keyword_def.h"
#include "lib_condition_def.h"

/**
 * @brief Override log level của ESP-IDF
 */

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

/**
 * @brief Khai báo các declaration của các hàm
 */

  void check_reset_reason(void);
  void check_cpu_clock(void);

void app_main(void) {
    
  check_reset_reason();

  check_cpu_clock();

  while (1) {
    vTaskDelay(pdMS_TO_TICKS(10000));
    printf("Loop\n");
  }
}

void check_reset_reason(void) {
  esp_reset_reason_t reset_reason = esp_reset_reason();
  ESP_LOGI("main", "Reset reason: %d", reset_reason);

  switch (reset_reason) {
    case ESP_RST_UNKNOWN:
      ESP_LOGI("main", "Unknown reset reason.");
      break;
    case ESP_RST_POWERON:
      ESP_LOGI("main", "Power-on reset detected.");
      break;
    case ESP_RST_EXT:
      ESP_LOGI("main", "External pin reset detected.");
      break;
    case ESP_RST_SW:
      ESP_LOGI("main", "Software reset detected.");
      break;
    case ESP_RST_PANIC:
      ESP_LOGI("main", "Software reset due to exception/panic detected.");
      break;
    case ESP_RST_INT_WDT:
      ESP_LOGI("main", "Interrupt watchdog reset detected.");
      break;
    case ESP_RST_TASK_WDT:
      ESP_LOGI("main", "Task watchdog reset detected.");
      break;
    case ESP_RST_WDT:
      ESP_LOGI("main", "Other watchdog reset detected.");
      break;
    case ESP_RST_DEEPSLEEP:
      ESP_LOGI("main", "Woke up from deep sleep.");
      break;
    case ESP_RST_BROWNOUT:
      ESP_LOGI("main", "Brownout reset detected.");
      break;
    case ESP_RST_SDIO:
      ESP_LOGI("main", "SDIO reset detected.");
      break;
    case ESP_RST_USB:
      ESP_LOGI("main", "USB peripheral reset detected.");
      break;
    case ESP_RST_JTAG:
      ESP_LOGI("main", "JTAG reset detected.");
      break;
    case ESP_RST_EFUSE:
      ESP_LOGI("main", "Efuse error reset detected.");
      break;
    case ESP_RST_PWR_GLITCH:
      ESP_LOGI("main", "Power glitch reset detected.");
      break;
    case ESP_RST_CPU_LOCKUP:
      ESP_LOGI("main", "CPU lock up reset detected.");
      break;
    default:
      ESP_LOGI("main", "Unhandled reset reason: %d", reset_reason);
      break;
  }
}

void check_cpu_clock(void) {
  ui32 cpu_freq_mhz = 0;
  ESP_ERROR_CHECK(esp_clk_tree_src_get_freq_hz(
    SOC_MOD_CLK_CPU, 
    ESP_CLK_TREE_SRC_FREQ_PRECISION_EXACT, 
    &cpu_freq_mhz)
  );
  ESP_LOGI("main", "CPU frequency: %u Hz (~%u MHz)", cpu_freq_mhz, cpu_freq_mhz / 1000000U);
}
