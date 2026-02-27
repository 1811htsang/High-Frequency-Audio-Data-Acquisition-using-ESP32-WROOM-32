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

      // Khởi tạo I2S channel
      i2s_chan_handle_t rx_handle;
      i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
      i2s_new_channel(&chan_cfg, NULL, &rx_handle);

      // Cấu hình I2S định dạng chuẩn
      i2s_std_config_t std_cfg = {
        .clk_cfg = {
          .sample_rate_hz = 96000,
          .clk_src = I2S_CLK_SRC_EXTERNAL,
          .mclk_multiple = I2S_MCLK_MULTIPLE_256,
          .ext_clk_freq_hz = 24576000
        },
        .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_32BIT, I2S_SLOT_MODE_STEREO),
        .gpio_cfg = {
          .mclk = GPIO_NUM_0,
          .bclk = GPIO_NUM_4,
          .ws = GPIO_NUM_5,
          .dout = I2S_GPIO_UNUSED,
          .din = GPIO_NUM_19,
          .invert_flags = {
              .mclk_inv = false,
              .bclk_inv = false,
              .ws_inv = false,
          },
        },
      };
      i2s_channel_init_std_mode(rx_handle, &std_cfg);

      /**
       * Ghi chú:
       * Thiết kế đảm bảo theo tài liệu của PCM1808, ta sẽ đạt được
       * - Tốc độ lấy mẫu 96kHz (dùng trong đầu ra của pin WS/LRCK)
       * - Độ phân giải 24-bit (sử dụng 32-bit slot để chứa dữ liệu 24-bit theo định dạng MSB-first & Left-justified)
       * - Sử dụng nguồn clock ngoài 24.576MHz (được cung cấp qua pin MCLK) để đảm bảo độ chính xác của tốc độ lấy mẫu, 
       * tránh sai lệch do clock nội bộ có thể không ổn định hoặc bị ảnh hưởng bởi các yếu tố khác.
       * - BCK đạt tần số 6.144MHz (tính toán từ MCLK 24.576MHz chia cho 4, phù hợp với tốc độ lấy mẫu 96kHz và độ phân giải 24-bit)
       * - Cấu hình GPIO phù hợp với sơ đồ kết nối phần cứng đã thiết kế, đảm bảo tín hiệu được truyền đúng cách giữa ESP32 và PCM1808.
       */


      /* Before reading data, start the RX channel first */
        // i2s_channel_enable(rx_handle);
        // i2s_channel_read(rx_handle, desc_buf, bytes_to_read, bytes_read, ticks_to_wait);

      /* Have to stop the channel before deleting it */
        // i2s_channel_disable(rx_handle);
      /* If the handle is not needed any more, delete it to release the channel resources */
        // i2s_del_channel(rx_handle);
  }