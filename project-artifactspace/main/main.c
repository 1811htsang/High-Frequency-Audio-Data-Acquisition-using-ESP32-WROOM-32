// Khai báo các thư viện sử dụng

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
  #include "driver/i2s_std.h"
  #include "driver/i2s_common.h"
  #include "driver/gpio.h"

// Khai báo các hằng số và biến toàn cục nếu cần thiết

  #define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

  #define ui32 uint32_t
  #define ui16 uint16_t
  #define ui8 uint8_t

  #define I2S_SAMPLE_RATE_HZ 96000
  #define I2S_RX_BUFFER_SAMPLES 512
  #define I2S_LOG_SAMPLES_PER_CHUNK 8

// Khai báo các hàm sử dụng làm việc

  void check_reset_reason(void);
  void check_cpu_clock(void);

// Thực thi chương trình chính

  void app_main(void) {

    // Kiểm tra nguyên nhân khởi động
      
      check_reset_reason();

    // Kiểm tra xung clock nội bộ

      check_cpu_clock();

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
      chan_cfg.dma_desc_num = I2S_LOG_SAMPLES_PER_CHUNK;
      chan_cfg.dma_frame_num = I2S_RX_BUFFER_SAMPLES;
      ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, NULL, &rx_handle));

      // Cấu hình I2S định dạng chuẩn
      /**
       * Ghi chú:
       * Theo tài liệu Series datasheet của S3,
       * thì I2S được quyền sử dụng các chân từ GPIO4 đến GPIO7 cho chức năng I2S, 
       * ngoài ra trên thực tế, các chân này liền kề nhau 
       * cũng giúp việc routing tín hiệu dễ dàng hơn trên phần cứng,
       * do đó ta sẽ chọn các chân này để kết nối với PCM1808.
       * Cấu hình này sử dụng thạch anh rời để làm nguồn clock,
       * tuy nhiên do linh kiện chưa về kịp nên tạm thời sử dụng clock PLL nội bộ để test
       */
      
      /*
      i2s_std_config_t std_cfg = {
        .clk_cfg = {
          .sample_rate_hz = 96000,
          .clk_src = I2S_CLK_SRC_EXTERNAL,
          .mclk_multiple = I2S_MCLK_MULTIPLE_256,
          .ext_clk_freq_hz = 24576000,
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
      */

      i2s_std_config_t std_cfg = {
        .clk_cfg = {
          .sample_rate_hz = I2S_SAMPLE_RATE_HZ,
          .clk_src = I2S_CLK_SRC_DEFAULT, // Tạm thời sử dụng clock PLL nội bộ để test
          .mclk_multiple = I2S_MCLK_MULTIPLE_256,
          .ext_clk_freq_hz = 24576000, // Tần số của thạch anh rời dự kiến sẽ sử dụng, chỉ mang tính chất tham khảo khi test với clock nội bộ
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
      ESP_ERROR_CHECK(i2s_channel_init_std_mode(rx_handle, &std_cfg));

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


      // Cấp phát buffer nhận mẫu âm thanh và bắt đầu đọc liên tục
      int32_t *rx_buffer = (int32_t *)heap_caps_malloc(sizeof(int32_t) * I2S_RX_BUFFER_SAMPLES, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
      if (rx_buffer == NULL) {
          ESP_LOGE("main", "Failed to allocate RX buffer");
          return;
      }
      size_t bytes_read = 0;

      ESP_ERROR_CHECK(i2s_channel_enable(rx_handle));
      ESP_LOGI("main", "I2S RX started: sample_rate=%d Hz, dma_desc_num=%d, dma_frame_num=%d",
        I2S_SAMPLE_RATE_HZ,
        chan_cfg.dma_desc_num,
        chan_cfg.dma_frame_num
      );

      while (1) {

        // delay một khoảng thời gian để tránh log quá nhiều, đồng thời cũng giúp dễ quan sát hơn
        vTaskDelay(pdMS_TO_TICKS(100));

        esp_err_t read_err = i2s_channel_read(
          rx_handle,
          rx_buffer,
          sizeof(rx_buffer),
          &bytes_read,
          pdMS_TO_TICKS(1000)
        );

        if (read_err == ESP_OK && bytes_read > 0) {
          size_t samples_read = bytes_read / sizeof(rx_buffer[0]);
          size_t samples_to_log = (samples_read < I2S_LOG_SAMPLES_PER_CHUNK) ? samples_read : I2S_LOG_SAMPLES_PER_CHUNK;

          ESP_LOGI("main", "RX bytes=%u, samples=%u", (unsigned int)bytes_read, (unsigned int)samples_read);

          for (size_t i = 0; i < samples_to_log; i++) {
            // PCM1808 xuất 24-bit, dữ liệu đặt trong slot 32-bit nên dịch phải 8-bit để xem biên độ signed 24-bit.
            int32_t sample_24 = (rx_buffer[i] >> 8);
            ESP_LOGI("main", "sample[%u] raw=0x%08" PRIx32 " s24=%" PRId32,
              (unsigned int)i,
              (uint32_t)rx_buffer[i],
              sample_24
            );
          }
        }
        else if (read_err == ESP_ERR_TIMEOUT) {
          ESP_LOGW("main", "I2S read timeout: khong nhan duoc du lieu trong 1000 ms");
        }
        else {
          ESP_LOGE("main", "I2S read error: %s", esp_err_to_name(read_err));
          vTaskDelay(pdMS_TO_TICKS(100));
        }
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
