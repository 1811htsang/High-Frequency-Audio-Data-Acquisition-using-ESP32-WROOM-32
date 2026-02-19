// Khai báo các thư viện sử dụng

  #include <stdio.h>
  #include <stdint.h>
  #include "esp_system.h"
  #include "esp_pm.h"
  #include "driver/i2s_std.h"
  #include "driver/i2s_common.h"
  #include "driver/gpio.h"

// Khai báo các hằng số và biến toàn cục nếu cần thiết

// Khai báo các hàm phụ trợ

  void reset_catch(void) {
    esp_reset_reason_t reset_reason = esp_reset_reason();
    printf("Reset reason: %d\n", reset_reason);

    switch (reset_reason) {
      case ESP_RST_UNKNOWN:
        printf("Unknown reset reason.\n");
        break;
      case ESP_RST_POWERON:
        printf("Power-on reset detected.\n");
        break;
      case ESP_RST_EXT:
        printf("External pin reset detected.\n");
        break;
      case ESP_RST_SW:
        printf("Software reset detected.\n");
        break;
      case ESP_RST_PANIC:
        printf("Software reset due to exception/panic detected.\n");
        break;
      case ESP_RST_INT_WDT:
        printf("Interrupt watchdog reset detected.\n");
        break;
      case ESP_RST_TASK_WDT:
        printf("Task watchdog reset detected.\n");
        break;
      case ESP_RST_WDT:
        printf("Other watchdog reset detected.\n");
        break;
      case ESP_RST_DEEPSLEEP:
        printf("Woke up from deep sleep.\n");
        break;
      case ESP_RST_BROWNOUT:
        printf("Brownout reset detected.\n");
        break;
      case ESP_RST_SDIO:
        printf("SDIO reset detected.\n");
        break;
      case ESP_RST_USB:
        printf("USB peripheral reset detected.\n");
        break;
      case ESP_RST_JTAG:
        printf("JTAG reset detected.\n");
        break;
      case ESP_RST_EFUSE:
        printf("Efuse error reset detected.\n");
        break;
      case ESP_RST_PWR_GLITCH:
        printf("Power glitch reset detected.\n");
        break;
      case ESP_RST_CPU_LOCKUP:
        printf("CPU lock up reset detected.\n");
        break;
      default:
        printf("Unhandled reset reason: %d\n", reset_reason);
        break;
    }
  }

  esp_err_t power_management_setup(void) {
    esp_pm_config_t pm_config = {
      .max_freq_mhz = 240,
      .min_freq_mhz = 80,
      .light_sleep_enable = true
    };

    esp_err_t ret = esp_pm_configure(&pm_config);
    return ret;
  }

// Thực thi chương trình chính

  void app_main(void) {

    // Kiểm tra nguyên nhân khởi động
      reset_catch();

    // Cấu hình quản lý năng lượng
      if (power_management_setup() != ESP_OK) {
        printf("Failed to configure power management.\n");
        return -1;
      } else {
        printf("Power management configured successfully.\n");
      }

    // Cấu hình I2S

      i2s_chan_handle_t rx_handle;

      i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);

  }