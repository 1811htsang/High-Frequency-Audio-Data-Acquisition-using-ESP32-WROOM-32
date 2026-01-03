#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "esp_clk_tree.h"

// Khai báo từ khóa 
#define __vo volatile
#define HIGH 1
#define LOW 0
#define SET HIGH
#define RESET LOW
#define ul unsigned long

// Khai báo địa chỉ ngoại vi
/*
  Trong khai báo địa chỉ nên specify cụ thể UL (unsigned long) để tránh lỗi ép kiểu
  khi sử dụng trên các nền tảng khác nhau (32-bit và 64-bit).

  Nhóm ngoại vi được sử dụng trong CPU thông qua khoảng `0x3FF40000`-`0x3FF7FFFF`
  có thể được truy cập thông qua khoảng địa chỉ khác là `0x60000000`-`0x6003FFFF` (khoảng địa chỉ của AHB).
  Nghĩa là (`0x3FF40000+N`) tương đương với (`0x60000000+N`).
*/
// > Nhóm địa chỉ nền

#define DPort_REGS_BASEADDR 0x3FF00000ul 
#define RTC_REGS_BASEADDR 0x3FF48000ul

// >> Nhóm địa chỉ thanh ghi cụ thể

#define DPORT_CPU_PER_CONF_REG_ADDR (DPort_REGS_BASEADDR + 0x003Cul)
#define RTC_CNTL_CLK_CONF_REG_ADDR (RTC_REGS_BASEADDR + 0x0070ul)

// Khai báo cấu trúc thanh ghi

typedef __vo struct DPORT_CPU_PER_CONF_REG
{
  __vo unsigned int CPU_PERIOD_SEL : 2;
  __vo unsigned int RESERVED0 : 30;
} DPORT_CPU_PER_CONF_REG;

typedef __vo struct RTC_CNTL_CLK_CONF_REG
{
  __vo unsigned int RESERVED2 : 4;
  __vo unsigned int RTC_CNTL_CK8M_DIV : 2;
  __vo unsigned int RTC_CNTL_ENB_CK8M : 1;
  __vo unsigned int RTC_CNTL_ENB_CK8M_DIV : 1;
  __vo unsigned int RTC_CNTL_DIG_XTAL32K_EN : 1;
  __vo unsigned int RTC_CNTL_DIG_CLK8M_D256_EN : 1;
  __vo unsigned int RTC_CNTL_DIG_CLK8M_EN : 1;
  __vo unsigned int RESERVED1 : 1;
  __vo unsigned int RTC_CNTL_CK8M_DIV_SEL : 3;
  __vo unsigned int RESERVED0 : 2;
  __vo uint8_t RTC_CNTL_CK8M_DFREQ : 8;
  __vo unsigned int RTC_CNTL_CK8M_FORCE_PD : 1;
  __vo unsigned int RTC_CNTL_CK8M_FORCE_PU : 1;
  __vo unsigned int RTC_CNTL_SOC_CLK_SEL : 2;
  __vo unsigned int RTC_CNTL_RTC_FAST_CLK_SEL : 1;
  __vo unsigned int RTC_CNTL_ANA_CLK_RTC_SEL : 2;
} RTC_CNTL_CLK_CONF_REG;

// Tạo con trỏ tới thanh ghi
#define DPORT_CPU_PER_CONF_REG_PTR ((DPORT_CPU_PER_CONF_REG *)DPORT_CPU_PER_CONF_REG_ADDR)
#define RTC_CNTL_CLK_CONF_REG_PTR ((RTC_CNTL_CLK_CONF_REG *)RTC_CNTL_CLK_CONF_REG_ADDR)

void app_main(void) {
  // Setup clock cho CPU
  RTC_CNTL_CLK_CONF_REG_PTR->RTC_CNTL_SOC_CLK_SEL = 1; // Chọn nguồn clock SOC là PLL_CLK
  printf("SOC Clock Select set to: %u\n", RTC_CNTL_CLK_CONF_REG_PTR->RTC_CNTL_SOC_CLK_SEL);
  DPORT_CPU_PER_CONF_REG_PTR->CPU_PERIOD_SEL = 2; // Chọn tần số CPU là 240MHz
  printf("CPU Period Select set to: %u\n", DPORT_CPU_PER_CONF_REG_PTR->CPU_PERIOD_SEL);

  // Kiểm tra lại giá trị đã thiết lập sử dụng hàm esp_clk_tree_get_cpu_freq
  uint32_t result = 0;
  soc_module_clk_t clk_src = SOC_MOD_CLK_CPU;
  esp_clk_tree_src_freq_precision_t precision = ESP_CLK_TREE_SRC_FREQ_PRECISION_EXACT;
  esp_err_t err = esp_clk_tree_src_get_freq_hz(
    clk_src,
    precision,
    &result
  );

  switch (err) {
    case ESP_OK:
      printf("ESP_OK: Frequency retrieved successfully.\n");
      printf("CPU Frequency: %"PRIu32" Hz\n", result);
      break;
    case ESP_ERR_INVALID_ARG:
      printf("ESP_ERR_INVALID_ARG: Invalid argument provided.\n");
      break;
    case ESP_FAIL:
      printf("ESP_FAIL: Frequency calibration failed.\n");
      break;
    default:
      break;
  }
}