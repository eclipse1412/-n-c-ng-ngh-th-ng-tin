## Hệ thống Quản lý Người dùng và Ví Điểm Thưởng (C++
)

## Giới thiệu dự án
Dự án xây dựng một hệ thống quản lý người dùng bằng ngôn ngữ lập trình **C++** với giao diện dòng lệnh (CLI). Hệ thống hỗ trợ:
- Đăng ký, đăng nhập tài khoản
- Phân quyền người dùng (admin và user)
- Xác thực hai lớp với OTP
- Quản lý thông tin cá nhân
- Hệ thống ví điểm thưởng cho phép chuyển điểm giữa các ví

Mục tiêu dự án là giúp sinh viên:
- Thực hành lập trình hướng đối tượng trong C++
- Ứng dụng kỹ thuật xử lý file, bảo mật đơn giản
- Học cách làm việc nhóm thông qua GitHub
- Tổ chức một dự án phần mềm thực tế theo mô hình phân công rõ ràng

---

## Thành viên nhóm

**1. Nguyễn Thanh Tú:**  
- Chức năng đăng ký, đăng nhập  
- Sinh mật khẩu tự động  
- Xác thực đăng nhập  

**2. Dương Gia Bảo:**  
- Ví điểm thưởng
- Quản lý lưu trữ file (`users.txt`, `wallets.txt`)
- Ghi và đọc lịch sử giao dịch (`transactions.log`) 

**3. Trần Anh Hào:**  
- Xây dựng phân quyền admin/user  
- OTP, xác minh khi thay đổi thông tin và thực hiện giao dịch  
- Quản lý xác thực hai lớp

**4. Trần Hoàng Hải Đăng:**  
- Thiết kế giao diện dòng lệnh  
- Viết tài liệu README  
- Hướng dẫn sử dụng chương trình và xử lý giao tiếp người dùng

---

## Chức năng chính

### 1. Đăng ký tài khoản
- Người dùng nhập tên đăng nhập, mật khẩu và thông tin cá nhân
- Quản lý (admin) có thể tạo tài khoản hộ người dùng
- Hệ thống sẽ tự sinh mật khẩu tự động cho lần đầu tiên đăng ký tài khoản

### 2. Đăng nhập
- Xác thực tên đăng nhập và mật khẩu đã lưu
- Nếu là mật khẩu tự động, hệ thống sẽ yêu cầu người dùng đổi mật khẩu sau lần đăng nhập đầu tiên

### 3. Phân quyền người dùng
- **Người dùng thường (User):**
  - Xem thông tin cá nhân
  - Cập nhật tên, email, đổi mật khẩu (có OTP xác minh)
  - Xem thông tin ví
  - Nạp điểm vào ví
  - Xem số dư ví
  - Thực hiện chuyển điểm
  - Xem lịch sử giao dịch

- **Quản trị viên (Admin):**
  - Xem toàn bộ danh sách người dùng
  - Tạo mới tài khoản
  - Chỉnh sửa thông tin tài khoản hộ người dùng (cần xác minh OTP)
  - Không được thay đổi tên tài khoản đăng nhập

### 4. OTP (One-Time Password)
- Hệ thống sẽ tự sinh OTP 4 chữ số ngẫu nhiên để xác minh:
  - Khi thay đổi thông tin cá nhân
  - Khi thực hiện giao dịch chuyển điểm
- OTP có thời gian hiệu lực giới hạn, tự sinh ngẫu nhiên và gửi qua hệ thống CLI

### 5. Ví điểm thưởng
- Mỗi người dùng có một ví riêng và có mã định danh duy nhất
- **Chuyển điểm**:
  - Nhập mã ví đích
  - Số điểm cần chuyển
  - Kiểm tra số dư ví nguồn
  - Xác minh OTP
  - Thực hiện giao dịch an toàn: nếu một trong các bước chuyển điểm thất bại (Ví dụ như số dư trong ví không đủ), toàn bộ giao dịch sẽ bị hủy và hoàn lại số dư cho người chuyển
