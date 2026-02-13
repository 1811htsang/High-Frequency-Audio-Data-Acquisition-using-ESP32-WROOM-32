# Ghi chú khi viết driver cho ESP32

## Các lưu ý quan trọng

1. Trước khi viết driver nên tìm hiểu bổ sung về vấn đề Big/Little Endian trên ESP32. Nếu không xác định đúng sẽ rất dễ gặp lỗi khi xử lý dữ liệu nhị phân hoặc trong quá trình cài đặt thanh ghi hoặc set/reset các bit trên thanh ghi sẽ bị sai lệch.
2. Nên thiết kế driver theo 3 lớp, trong đó:

   - Lớp thấp nhất: Giao tiếp trực tiếp với phần cứng (thanh ghi, bus giao tiếp,...)
   - Lớp trung gian: Xử lý các logic nghiệp vụ, chuyển đổi dữ liệu giữa lớp thấp và lớp cao.
   - Lớp cao nhất: Cung cấp API cho ứng dụng sử dụng driver.

    Phương pháp này giúp tách biệt rõ ràng các chức năng, dễ dàng bảo trì và mở rộng trong tương lai. Ngoài ra, hỗ trợ kiểm tra unit test cho từng lớp riêng biệt.

3. Sử dụng các macro và định nghĩa trong file lib_keyword_define.h để đảm bảo tính nhất quán và dễ bảo trì mã nguồn.

## Ghi chú ngày 12-2-2026

Dựa trên các đánh giá về việc khai thác hướng xây dựng driver bare-metal cho ESP32, đối với dự án này sẽ thực hiện việc sử dụng ESP-IDF framework để tận dụng các thư viện và công cụ hỗ trợ sẵn có. Điều này giúp giảm thiểu thời gian phát triển và tăng tính ổn định của hệ thống.

Nguyên nhân được đưa ra như sau:

- Kiến trúc của ESP32 theo hướng SoC với nhiều thành phần tích hợp sẵn, việc viết driver từ đầu sẽ rất phức tạp và dễ gặp lỗi so với STM32 là 1 MCU.
- ESP-IDF cung cấp nhiều driver và thư viện đã được tối ưu hóa, giúp tiết kiệm thời gian phát triển.
- Việc sử dụng ESP-IDF giúp tận dụng cộng đồng hỗ trợ rộng lớn và tài liệu phong phú, giúp giải quyết các vấn đề nhanh hơn.
