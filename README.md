# Hệ thống Quản lý Người dùng và Ví Điểm Thưởng (C++)

## Giới thiệu dự án

Dự án xây dựng một hệ thống quản lý người dùng bằng ngôn ngữ lập trình C++ với giao diện dòng lệnh (CLI). Hệ thống hỗ trợ:
- Đăng ký, đăng nhập tài khoản
- Phân quyền người dùng (admin và user)
- Xác thực hai lớp với OTP
- Quản lý thông tin cá nhân
- Hệ thống ví điểm thưởng cho phép chuyển điểm giữa các ví

Mục tiêu dự án là giúp sinh viên thực hành kỹ năng lập trình hướng đối tượng trong C++, xử lý file, bảo mật đơn giản, làm việc nhóm qua Git và tổ chức một dự án phần mềm thực tế.

---

## Thành viên nhóm

 
**1. Nguyễn Thanh Tú:**   Chức năng đăng ký, đăng nhập, sinh mật khẩu tự động, xác thực đăng nhập             
**2. Dương Gia Bảo:**    Phân quyền admin/user, OTP, xác minh thay đổi thông tin và giao dịch                
**3. Trần Anh Hào:**      Quản lý lưu trữ file `users.txt`, `wallets.txt`, sao lưu/phục hồi dữ liệu           
**4. Trần Hoàng Hải Đăng:** Thiết kế giao diện dòng lệnh, tài liệu README, hướng dẫn sử dụng chương trình   

---

## Chức năng chính

### 1. Đăng ký tài khoản
- Nhập tên đăng nhập, mật khẩu, thông tin cá nhân
- Có thể tạo tài khoản hộ qua quản lý (admin)
- Nếu không nhập mật khẩu, hệ thống sẽ sinh tự động

### 2. Đăng nhập
- Xác thực tên đăng nhập và mật khẩu đã lưu
- Nếu mật khẩu được sinh tự động, hệ thống yêu cầu đổi mật khẩu sau lần đăng nhập đầu tiên

### 3. Phân quyền người dùng
- **User**: Xem và chỉnh sửa thông tin cá nhân, chuyển điểm, xem số dư ví
- **Admin**: Toàn quyền tạo/sửa tài khoản, xem danh sách người dùng, chỉnh sửa hộ, xác minh qua OTP

### 4. OTP (One-Time Password)
- OTP được sử dụng để xác nhận các thay đổi thông tin quan trọng và giao dịch chuyển điểm
- Hệ thống sinh OTP ngẫu nhiên 6 chữ số và yêu cầu nhập lại để xác nhận

### 5. Ví điểm thưởng
- Mỗi người dùng có 1 ví chứa điểm
- Ví có mã định danh duy nhất
- **Chuyển điểm**: giữa hai ví cần xác thực OTP và kiểm tra đủ số dư
- **Giao dịch an toàn (atomic)**: Nếu một bước trong chuyển điểm thất bại, toàn bộ thao tác bị hủy và hoàn lại số dư
