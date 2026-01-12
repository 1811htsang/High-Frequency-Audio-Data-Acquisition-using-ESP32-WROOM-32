# Tổng quan
ESP32 được thiết kế với 3 WDT chính là 
- 2 WDT gọi là MWDT (Main Watchdog Timer) 
- 1 WDT gọi là RWDT (RTC Watchdog Timer).

Mỗi watch dog đều có 4 công đoạn chính, mỗi công đoạn có thể chọn 1 trong 3/4 action khi hết thời gian với mỗi công đoạn. Mỗi công đoạn có timeoute khác nhau có thể set.

Các action bao gồm:
- Ngắt
- CPU reset
- Core reset
- System reset

Trong đó, chỉ có mỗi RWDT được phép thực hiện System reset và reset toàn bộ chip bao gồm cả RTC.

# Chức năng
- 4 công đoạn với các cấu hình khác nhau có thể tắt riêng biệt
- Timeout có thể cấu hình riêng biệt cho từng công đoạn
- Bộ đếm 32 bit 
- Hỗ trợ write protection bảo vệ cho MWDT và RWDT
- Hỗ trợ flash boot protection

# Mô tả chức năng
## Clk đầu vào
RWDT sử dụng từ RTC_SLOW_CLK

MWDT sử dụng từ APB_CLK + prescaler

Với mỗi watchdog, nguồn clk sẽ là nguồn vào của bộ đếm 32-bit.

Khi bộ đếm đặt giá trị timeout của stage hiện tại, các action đã được cấu hình sẽ thực thi. Bộ đếm reset và stage kế tiếp được kích hoạt.

## Quy trình hoạt động
Khi bộ hẹn giờ giám sát được kích hoạt, nó sẽ hoạt động theo vòng lặp từ stage 0 đến stage 3, sau đó quay lại stage 0 và bắt đầu lại. 

Action timeout và khoảng thời gian cho mỗi stage có thể được cấu hình riêng lẻ. 

Mỗi stage có thể được cấu hình cho một trong các action sau khi bộ hẹn giờ timeout đạt đến giá trị thời gian chờ của stage: 
- Kích hoạt ngắt 

  Khi stage timeout, một ngắt sẽ được kích hoạt. 

- Reset lõi CPU 

  Khi stage timeout, lõi CPU được chỉ định sẽ được reset. 
  
  MWDT0 CPU reset chỉ reset PRO_CPU. 
  
  Reset CPU MWDT1 chỉ reset CPU APP. 
  
  Reset CPU RWDT có thể reset một trong hai, hoặc cả hai, hoặc không reset cái nào, tùy thuộc vào cấu hình.

- Reset main system 

  Khi stage timeout, hệ thống chính, bao gồm cả các MWDT, sẽ được reset. Trong bài viết này, hệ thống chính bao gồm CPU và tất cả các thiết bị ngoại vi. Bộ đếm thời gian thực (RTC) là một ngoại lệ và nó sẽ không được đặt lại. 

- Đặt lại hệ thống chính và RTC 

  Khi stage kết thúc, cả hệ thống chính và RTC sẽ được đặt lại. action này chỉ khả dụng trong RWDT.

- Vô hiệu hóa stage này sẽ không có tác động gì đến hệ thống. 

Khi phần mềm cung cấp dữ liệu cho bộ đếm thời gian giám sát, nó sẽ quay lại stage 0 và bộ đếm thời gian timeout của nó sẽ reset từ 0.

## Bảo vệ ghi
Cả MWDT và RWDT đều có thể được bảo vệ khỏi việc ghi dữ liệu ngẫu nhiên. Để thực hiện điều này, chúng có một thanh ghi khóa ghi (`TIMGn_WDTWPROTECT_REG` cho MWDT, `RTC_CNTL_WDTWPROTECT_REG` cho RWDT). 

Khi khởi động lại, các thanh ghi này được khởi tạo với giá trị `0x50D83AA1`. Khi giá trị trong thanh ghi này được thay đổi từ `0x50D83AA1`, chức năng bảo vệ ghi sẽ được kích hoạt. 

Các thao tác ghi vào bất kỳ thanh ghi WDT nào, bao gồm cả thanh ghi cấp dữ liệu (nhưng không bao gồm chính thanh ghi khóa ghi), đều bị bỏ qua. 

Quy trình được khuyến nghị để truy cập WDT là: 
1. Vô hiệu hóa chức năng bảo vệ ghi 
2. Thực hiện sửa đổi cần thiết hoặc cấp dữ liệu cho bộ giám sát 
3. Kích hoạt lại chức năng bảo vệ ghi

## Bảo vệ khởi động từ flash
Trong quá trình khởi động flash, MWDT trong nhóm hẹn giờ 0 (TIMG0), cũng như RWDT, được tự động kích hoạt. 

Giai đoạn 0 đối với MWDT được kích hoạt sẽ tự động được cấu hình để đặt lại hệ thống khi hết hạn; giai đoạn 0 đối với RWDT sẽ đặt lại RTC khi nó hết hạn. 

Sau khi khởi động, cần clear thanh ghi `TIMERS_WDT_FLASHBOOT_MOD_EN` để dừng quy trình bảo vệ khởi động flash cho MWDT, và cần clear thanh ghi `RTC_CNTL_WDT_FLASHBOOT_MOD_EN` để thực hiện điều tương tự cho RWDT. Sau đó, MWDT và RWDT có thể được cấu hình bằng phần mềm.

# Thanh ghi
Các thanh ghi MWDT là một phần của TIM và được mô tả trong phần TIM. Các thanh ghi RWDT là một phần của RTC và được mô tả trong phần RTC.

# Bổ sung
Đối với thiết kế phần cứng, ESP32 chỉ tồn tại 2 WDT là MWDT và RWDT. 
Tuy nhiên, với API của ESP32 sẽ xây dựng bổ sung 3 WDT như sau:
- IWDT (Interrupt Watchdog Timer) chịu trách nhiệm đảm bảo các chương trình xử lý ngắt (ISR) không bị chặn chạy trong thời gian dài và ngăn ISR bị kẹt trong quá trình thực thi.
- TWDT (Task Watchdog Timer) chịu trách nhiệm phát hiện các trường hợp tác vụ chạy mà không nhường quyền điều khiển quá lâu.
- RTC/LP-WDT (RTC/Low Power Watchdog Timer) có thể được sử dụng để theo dõi thời gian khởi động từ khi bật nguồn cho đến khi hàm chính của người dùng được thực thi. Nó cũng có thể được sử dụng trong quá trình xử lý lỗi hệ thống và trong môi trường năng lượng thấp.

# IWDT
IWDT sử dụng MWDT nhánh TIM1 và tận dụng ngắt tick FreeRTOS ở mỗi CPU để phục vụ việc feed IWDT.

Nếu ngắt tick trên một CPU cụ thể không được thực thi trong khoảng thời gian chờ của IWDT, điều đó cho thấy có điều gì đó đang ngăn chặn các ISR chạy trên CPU đó.