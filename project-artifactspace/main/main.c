// Khai báo các thư viện sử dụng

  #include <stdio.h>
  #include <stdint.h>
  #include "esp_system.h"
  #include "soc/clk_tree_defs.h"
  #include "esp_clk_tree.h"
  #include "esp_pm.h"
  #include "esp_log.h"
  #include "driver/i2s_std.h"
  #include "driver/i2s_common.h"
  #include "driver/gpio.h"

// Khai báo các hằng số và biến toàn cục nếu cần thiết

  #define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

  #define ui32 uint32_t
  #define ui16 uint16_t
  #define ui8 uint8_t

// Thực thi chương trình chính

  void app_main(void) {

    // Kiểm tra nguyên nhân khởi động
      
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

    // Kiểm tra xung clock nội bộ

      ui32 cpu_freq_mhz = 0;
      ESP_ERROR_CHECK(esp_clk_tree_src_get_freq_hz(
        SOC_MOD_CLK_CPU, 
        ESP_CLK_TREE_SRC_FREQ_PRECISION_EXACT, 
        &cpu_freq_mhz)
      );

      ESP_LOGI("main", "CPU frequency: %d MHz", cpu_freq_mhz);

    // Cấu hình quản lý năng lượng

      /**
       * Ghi chú:
       * Đã sử dụng cấu hình năng lượng thông qua
       * `menuconfig` nên không cần dùng API để cấu hình lại ở đây
       */

    // Cấu hình I2S

      // Khởi tạo I2S channel
      i2s_chan_handle_t rx_handle;
      i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
      i2s_new_channel(&chan_cfg, NULL, &rx_handle);

      // Cấu hình I2S định dạng chuẩn
      /**
       * Ghi chú:
       * Theo tài liệu Series datasheet của S3,
       * thì I2S được quyền sử dụng các chân từ GPIO4 đến GPIO7 cho chức năng I2S, 
       * ngoài ra trên thực tế, các chân này liền kề nhau 
       * cũng giúp việc routing tín hiệu dễ dàng hơn trên phần cứng,
       * do đó ta sẽ chọn các chân này để kết nối với PCM1808.
       */
      i2s_std_config_t std_cfg = {
        .clk_cfg = {
          .sample_rate_hz = 96000,
          .clk_src = I2S_CLK_SRC_EXTERNAL,
          .mclk_multiple = I2S_MCLK_MULTIPLE_256,
          
        },
        .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_32BIT, I2S_SLOT_MODE_STEREO),
        .gpio_cfg = {
          .mclk = GPIO_NUM_4, // Master clock (MCLK) được cung cấp từ nguồn clock ngoài 24.576MHz
          .bclk = GPIO_NUM_5, // Bit clock (BCLK) sẽ được tạo ra dựa trên MCLK, tần số sẽ là 6.144MHz (24.576MHz / 4)
          .ws = GPIO_NUM_6, // Word select (WS/LRCK) sẽ được tạo ra dựa trên BCLK, tần số sẽ là 96kHz (6.144MHz / 64, với 32-bit slot và stereo)
          .dout = I2S_GPIO_UNUSED, // Có thể sử dụng trong trường hợp mở rộng chức năng
          .din = GPIO_NUM_7,
          .invert_flags = {
              .mclk_inv = false,
              .bclk_inv = false,
              .ws_inv = false,
          },
        },
      };
      // i2s_channel_init_std_mode(rx_handle, &std_cfg);

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

      /**
       * Ghi chú:
       * Về phần truyền tải dữ liệu trong ESP32 đã được hỗ trợ cấu hình với DMA sẵn,
       * Do đó khi sử dụng API `i2s_channel_read()`, dữ liệu sẽ được tự động truyền qua DMA mà không cần phải cấu hình thêm gì cả,
       * Điều này giúp giảm tải cho CPU và tăng hiệu suất truyền dữ liệu.
       */


      /* Before reading data, start the RX channel first */
        // i2s_channel_enable(rx_handle);
        // i2s_channel_read(rx_handle, desc_buf, bytes_to_read, bytes_read, ticks_to_wait);

      /* Have to stop the channel before deleting it */
        // i2s_channel_disable(rx_handle);
      /* If the handle is not needed any more, delete it to release the channel resources */
        // i2s_del_channel(rx_handle);
  }
