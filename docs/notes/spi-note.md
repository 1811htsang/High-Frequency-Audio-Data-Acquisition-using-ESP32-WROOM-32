# Tổng quan
Trong ESP32, SPI được tích hợp 4 bộ điều khiển SPI phần cứng, bao gồm SPI0, SPI1, SPI2 và SPI3. 

Cụ thể hơn:
- SPI0 dùng làm bộ đệm để truy cập bộ nhớ ngoài là Cache
- SPI1 dùng làm master của giao thức
- SPI2 và SPI3 dùng làm maste/slave 

Khi sử dụng làm master, các SPI Controller có thể điều khiển nhiều tín hiệu CS (Chip Select, CS0 ~ CS2) để kích hoạt nhiều slave khác nhau.

Các SPI1 ~ SPI3 chia sẻ 2 kênh DMA. 

Thông tin về chi tiết kiến trúc thiết kế có trong [tech-dts](docs/../../references/esp32-technical-reference-manual-ver-5_2.pdf) trang 127.

# Bộ tín hiệu SPI
Các tín hiệu SPI bao gồm:
- D
- Q
- CS0-CS2
- CLK
- WP
- HD

SPI0 và SPI1 chia sẻ chung đường tín hiệu thông qua Arbiter, tín hiệu line đầu ra này là `SPI`. 

SPI2 có line tín hiệu riêng là `HSPI`, SPI3 có line tín hiệu riêng là `VSPI`.

Các đường tín hiệu trên có thể map chân thông qua các IO_MUX hoặc GPIO Matrix.

Các SPI Controller hỗ trợ các chế độ truyền dữ liệu khác nhau trong GP-SPI (General Purpose SPI) như:
- 4 line full-duplex
- 4 line half-duplex
- 3 line half-duplex

Trong chế độ QSPI (Quad SPI), một bộ SPI controller hỗ trợ truy cập bộ nhớ Flash hoặc SRAM thông tin các line tín hiệu như 1 bus dữ liệu SPI song song 4 bit

Thông tin về mapping chi tiết có trong [tech-dts](docs/../../references/esp32-technical-reference-manual-ver-5_2.pdf) trang 127.

# Chế độ SPI
## Chế độ GP-SPI
- Hỗ trợ lập trình độ dài dữ liệu truyền và nhận khác nhau, từ 1 bit đến 64 bit.
- Các chế độ truyền dữ liệu: 4 line full-duplex/half-duplex và 3 line half-duplex.
- Master/Slave.
- Có thể lập trình CPOL (Clock Polarity) và CPHA (Clock Phase).

**Ghi chú bổ sung:**
- CPOL (Clock Polarity): Xác định mức logic của tín hiệu clock khi không truyền dữ liệu. Nếu CPOL = 0, tín hiệu clock ở mức thấp khi không truyền dữ liệu; nếu CPOL = 1, tín hiệu clock ở mức cao khi không truyền dữ liệu.
- CPHA (Clock Phase): Xác định thời điểm dữ liệu được lấy mẫu so với tín hiệu clock. Nếu CPHA = 0, dữ liệu được lấy mẫu vào cạnh đầu tiên của tín hiệu clock; nếu CPHA = 1, dữ liệu được lấy mẫu vào cạnh thứ hai của tín hiệu clock.

## Chế độ QSPI song song
- Hỗ trợ lập trình các định dạng giao thức cho các thiết bị slave cụ thể như Flash và SRAM.
- 6 loại thao tác đọc trên Flash
- Tự động chuyển đổi giữa Flash và SRAM
- Tự động chờ đối với truy cập Flash

## Chế độ DMA
- Hỗ trợ truyền dữ liệu thông qua DMA sử dụng danh sách liên kết.

## Chế độ ngắt
- Hỗ trợ ngắt cho các sự kiện như hoàn thành truyền dữ liệu, lỗi truyền dữ liệu, v.v.
- Hỗ trợ cấu hình ngắt cho từng loại sự kiện cụ thể với chế độ DMA.

Kiểm chứng các thông tin trong [tech-dts](docs/../../references/esp32-technical-reference-manual-ver-5_2.pdf) trang 128.

# GP-SPI
Sử dụng 4 dây chính (SCLK, MOSI, MISO, CS) để truyền dữ liệu giữa Master và Slave.

SPI1 ~ SPI3 có thể giao tiếp với các slaves khác như một SPI master tiêu chuẩn.

SPI2 và SPI3 có thể được cấu hình làm SPI master/slave.

Mỗi SPI controller hỗ trợ tối đa 3 tín hiệu CS để giao tiếp với nhiều slave khác nhau.

Khi không sử dụng DMA, độ dài dữ liệu truyền/nhận tối đa trong 1 lần truyền là 64 byte, xét theo công nhân với 1 byte.

Thông tin về các lệnh hỗ trợ vởi GP-SPI slave chế độ half-duplex có trong [tech-dts](docs/../../references/esp32-technical-reference-manual-ver-5_2.pdf) trang 129.

## Chế độ 4 line full-duplex
Trong chế độ này, SPI có thể làm master/slave.

Cấu hình chế độ này bằng cách thiết lập các thanh ghi:
- bit `SPI_DOUTDIN` và `SPI_USR_MOSI` trong `SPI_USER_REG`

Độ dài truyền/nhận dữ liệu được cấu hình bởi các thanh ghi:
- `SPI_MISO_DLEN_REG` và `SPI_MOSI_DLEN_REG` cho master
- `SPI_SLV_RDBUF_DLEN_REG` và `SPI_SLV_WRBUF_DLEN_REG` cho slave

Khởi động truyền/nhận dữ liệu bằng cách thiết lập bit `SPI_USR` trong `SPI_CMD_REG`.

## Chế độ 4 line half-duplex
Trong chế độ này, SPI có thể làm master/slave.

Hỗ trợ linh hoạt các định dạng giao tiếp.

Định dạng giao tiếp được chỉ định như sau:
- `command` dài 0~16 bit
- `address` dài 0~32 (GP-SPI master)/64 (QSPI master) bit
- `dummy` dài 0~256 bit
- `data` dài 0~512 bit (64 bytes)

Các thành phần này được điều khiển bởi bit `SPI_USR_COMMAND`, `SPI_USR_ADDR`, `SPI_USR_DUMMY`, và `SPI_USR_MOSI`/`SPI_USR_MISO` trong thanh ghi `SPI_USER_REG`.

Mỗi phase kích hoạt bởi set bit = 1 tương ứng trong `SPI_USER_REG`.

Khi là master, thanh ghi này có thể được cấu hình để xác định kích hoạt từng phase

Khi là slave, bắt buộc các tiêu chuẩn định dạng giao tiếp, CS phải giữ mức thấp trong toàn bộ quá trình truyền dữ liệu. Nếu CS mức cao thì truyền dữ liệu sẽ bị reset.

Master có thể ghi vào thanh ghi trạng thái `SPI_SLV_WR_STATUS_REG` của slave và quyết định xem có nên đọc dữ liệu từ thanh ghi `SPI_SLV_WR_STATUS_REG` hay thanh ghi `SPI_RD_STATUS_REG` thông qua bit `SPI_SLV_STATUS_READBACK` trong thanh ghi `SPI_SLAVE1_REG`. 

Master có thể duy trì liên lạc với thiết bị phụ bằng cách đọc và ghi vào thanh ghi trạng thái của thiết bị phụ, từ đó dễ dàng thực hiện các giao tiếp phức tạp.

Độ dài truyền/nhận dữ liệu được cấu hình bởi các thanh ghi:
- `SPI_MISO_DLEN_REG` và `SPI_MOSI_DLEN_REG` cho master
- `SPI_SLV_RDBUF_DLEN_REG` và `SPI_SLV_WRBUF_DLEN_REG` cho slave

Việc nhận hoặc truyền dữ liệu được điều khiển bởi bit `SPI_USR_MOSI` hoặc `SPI_USR_MISO` trong thanh ghi `SPI_USER_REG`. 

Bit `SPI_USR` trong thanh ghi `SPI_CMD_REG` cần được cấu hình để khởi tạo quá trình truyền dữ liệu.