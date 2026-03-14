# Ghi chú về thiết kế hệ thống

## Giới thiệu

Ghi chú này tổng hợp các thiết kế hệ thống của dự án nhằm đảm bảo hoạt động ổn định và hiệu quả.

## Ý tưởng thiết kế

Thiết kế sử dụng sẽ lấy ý tưởng từ tài liệu [paper](../references/audio-data-acquisition-system-design-based-on-arm-and-dsp.pdf) nhằm đảm bảo tính ổn định và hiệu quả của hệ thống đã được kiểm chứng.

Đối với thiết kế hệ thống của tài liệu, nhóm tác giả sử dụng kết hợp giữa 1 vi điều khiển ARM với 1 DSP để xử lý âm thanh phức tạp.

Do đó, một câu hỏi đặt ra là ta có thể sử dụng một vi điều khiển duy nhất với 1 bộ thu thập dữ liệu sử dụng MAX9812, PCM1808, TL072IDT để thay thế cho thiết kế ban đầu hay không?

## Thay đổi thiết kế tương ứng

Từ thiết kế ban đầu, đã có một số thay đổi để phù hợp với yêu cầu của dự án:

- Thay thế cấu trúc thiết kế hệ thống của bài báo bằng một thiết kế mới sử dụng ESP32-S3-DevKitC N16R8 làm vi điều khiển chính, kết hợp với các bộ thu thập dữ liệu như MAX9812, PCM1808, TL072IDT để xử lý âm thanh.
- Vận dụng các giao thức như SPI, I2S, GPIO, RCC, IWDG, I2C và DMA để đảm bảo hiệu quả trong việc thu thập và xử lý dữ liệu âm thanh tương tự (96kHz/24-bit) như trong thiết kế ban đầu.
- Trong quá trình thiết kế tài liệu, API và bản thân phần cứng đã cho phép hỗ trợ DMA tự động với I2S trên ESP32, điều này giúp giảm tải cho CPU và cải thiện hiệu suất xử lý âm thanh. Do đó, bản thân đề tài cũng sẽ có sự mở rộng về nội dung hướng tới để kiểm chứng khả năng vận dụng của hệ thống này trong công nghiệp như bảo trì tiên đoán - predictive maintenance, một ứng dụng phổ biến của việc thu thập dữ liệu âm thanh trong công nghiệp.
