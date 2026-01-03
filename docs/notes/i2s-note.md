# Tổng quan
I2S xuất phát từ I2C nhưng có sự thiết kế chuyên biệt cho truyền tải dữ liệu âm thanh số.

I2S cung cấp 2 ngoại vi chính là I2S0 và I2S1.

Một bus I2S tiêu chuẩn đề ra 3 tín hiệu chính:
- Tín hiệu CLK
- Tín hiệu chọn kênh
- Tín hiệu dữ liệu tuần tự

Và 2 vai trò là:
- Master: tạo xung clock và tín hiệu chọn kênh
- Slave: nhận xung clock và tín hiệu chọn kênh từ Master

Từ vai trò trên, mỗi ESP32 đều sẽ có 2 đơn vị thực hiện ghi nhận truyền tải (Transmit - TX) và ghi nhận (Receive - RX) dữ liệu âm thanh số.

Các tín hiệu đường truyền clock và chọn kênh của cả RX & TX unit đều có thể được config trở thành master chế độ truyền tải hoặc slave chế độ ghi nhận dữ liệu. (*Nghĩa là trên một ESP32, cả RX và TX đều có thể trở thành cùng một lúc master hoặc slave*)

2 đơn vị TX và RX đều được thiết kế sử dụng bộ FIFO 32-bit với sizeof 64.

Ngoài ra, chỉ có I2S0 mới hỗ trợ DAC/ADC nội bộ trên chip cũng như hỗ trợ quá trình truyền nhận tín hiệu PDM (Pulse Density Modulation).

**Thông tin bổ sung:**
PDM hay Pulse Density Modulation là phương pháp chuyển đổi tín hiệu âm thanh analog thành các chuỗi pulse, trong đó mật độ của các pulse phản ánh cường độ âm thanh.

Kiểm chứng thông tin và bổ sung thông tin về tổng quan kiến trúc I2S bên trong ESP32 được cập chi tiết tại [tech-dts](/docs/references/esp32-technical-reference-manual-ver-5_2.pdf) trang 313.

Các thông tin tín hiệu chi tiết phía đầu vào I/O của I2S được mô tả chi tiết trong [tech-dts](/docs/references/esp32-technical-reference-manual-ver-5_2.pdf) trang 314.

Chú ý, ngoại trừ tín hiệu CLK của I2S, tất cả các tín hiệu còn lại được mapping vào chip thông qua ma trận GPIO và IO_MUX. Tín hiệu CLK được mapping vào IO_MUX cố định tại các vị trí GPIO0, GPIO1 (UOTXD), GPIO3 (UORXD) thông qua select hàm GPIO CLK_OUT1, CLK_OUT2, CLK_OUT3.

Thông tin chi tiết hơn về mapping nằm tại [tech-dts](/docs/references/esp32-technical-reference-manual-ver-5_2.pdf) trang 61, 62.

# Thiết kế Clock của I2S
Nguồn clock của I2S lấy từ 2 nguồn chính:
- `APLL_CLK` (Nguồn mặc định được sử dụng)
- `PLL_F160M_CLK`

Thông tin bổ sung về 2 nguồn clock này được thu thập trong tài liệu [clk-note](/docs/notes/rcc-note.md).

`PLL_F160M_CLK` là nguồn clock general-purpose, nghĩa là có thể đa dụng với các yêu cầu custom / đòi hỏi sự chính xác không cần quá cao. 

Lưu ý thêm, khi sử dụng `PLL_F160M_CLK` làm nguồn clock thì tránh việc sử dụng các tần số chia không nguyên (fractional dividers).

`APLL_CLK` ngược lại là nguồn clock chuyên dụng do độ phân giải cao và độ ổn định tốt hơn. 

Sự lựa chọn 2 nguồn clock được xác định bởi bit `I2S_CLKA_ENA` của thanh ghi `I2S_CLKM_CONF_REG`.

Sau khi đã lựa chọn nguồn clock, tín hiệu clock sẽ được đưa qua bộ chia tần số (divider) để tạo ra tần số clock phù hợp với yêu cầu truyền tải dữ liệu.

Bộ chia tần số này bao gồm 2 phần:
- Bộ chia thứ nhất theo công thức 
  $$f_{\text{I2S\_CLK}} = \frac{f_\text{CLK\_SRC}}{N+\frac{a}{b}}$$
  Trong đó:
  - N là giá trị được lưu trữ trong `REG_CLKM_DIV_NUM[7:0]`
  - b là giá trị được lưu trữ trong `I2S_CLKM_DIV_B[5:0]`
  - a là giá trị được lưu trữ trong `I2S_CLKM_DIV_A[5:0]`
  
  Tất cả giá trị này đều lưu trong thanh ghi `I2S_CLKM_CONF_REG`.
- Bộ chia thứ hai theo công thức
  $$f_{\text{I2S\_O\_BCK}} = \frac{f_{\text{I2S}}}{M}$$
  Trong đó
  - Nếu ở chế độ truyền tải master, M là giá trị được lưu trữ trong `I2S_TX_BCK_DIV_NUM[7:0]`
  - Nếu ở chế độ ghi nhận master, M là giá trị được lưu trữ trong `I2S_RX_BCK_DIV_NUM[7:0]`

  Cả hai giá trị này đều lưu trong thanh ghi `I2S_SAMPLE_RATE_CONF_REG`.

Ngoài ra một lưu ý cần bổ sung rằng khi hoạt động ở chế độ slave thì thiết bị master phải sử dụng `I2S_CLK` làm master clock và thỏa mãn yêu cầu 
$$f_{\text{I2S\_CLK}} \geq 8\times f_{\text{I2S\_O\_BCK}}$$


