1. Trước khi viết driver nên tìm hiểu bổ sung về vấn đề Big/Little Endian trên ESP32. Nếu không xác định đúng sẽ rất dễ gặp lỗi khi xử lý dữ liệu nhị phân hoặc trong quá trình cài đặt thanh ghi hoặc set/reset các bit trên thanh ghi sẽ bị sai lệch.
2. Nên thiết kế driver theo 3 lớp, trong đó:

   - Lớp thấp nhất: Giao tiếp trực tiếp với phần cứng (thanh ghi, bus giao tiếp,...)
   - Lớp trung gian: Xử lý các logic nghiệp vụ, chuyển đổi dữ liệu giữa lớp thấp và lớp cao.
   - Lớp cao nhất: Cung cấp API cho ứng dụng sử dụng driver.

    Phương pháp này giúp tách biệt rõ ràng các chức năng, dễ dàng bảo trì và mở rộng trong tương lai. Ngoài ra, hỗ trợ kiểm tra unit test cho từng lớp riêng biệt.

3. Sử dụng các macro và định nghĩa trong file lib_keyword_define.h để đảm bảo tính nhất quán và dễ bảo trì mã nguồn.