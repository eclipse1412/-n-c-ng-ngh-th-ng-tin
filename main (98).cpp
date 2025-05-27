#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<functional>
#include<vector>
#include<ctime>
#include<iomanip>
using namespace std;
//Hàm băm mk: chuyển đổi chuỗi mật khẩu thành giá trị chuỗi để bảo mật
string bamMK(string& matkhau)
{
	hash<string> hashObj;
	size_t hashValue = hashObj(matkhau);
	return to_string(hashValue);
}
//Hàm sinh OTP ngẫu nhiên 
string sinhOTP() {
	int otp = rand() % 9000 + 1000; // Sinh số 4 chữ số
	return to_string(otp); // Chuyển số thành chuỗi
}
//Hàm sinh mk tự động (độ dài 8 kí tự gồm chữ và số)
string sinhMKtudong()
{
	string ktu = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	string MK = "";
	for (int i = 0; i < 8; i++)
	{
		MK += ktu[rand() % ktu.size()];
	}
	return MK;
}
//Struct đại diện cho 1 tài khoản người dùng
struct Taikhoan
{
	string ten, MK, Ngaysinh, cccd, sdt;
	int ViChinh, ViPhu;
	bool admin;
};

class Vi {
private:
	string Id, Owner;
	double TienHienCo;
	vector<string> LichSuGiaoDich;
public:
	Vi(string id, string owner) {
		Id = id;
		Owner = owner;
		TienHienCo = 0.0;
	}
	string getId() const {  //Hàm trả về Id của ví.
		return Id;
	}
	string getOwner() const {  //Hàm trả về tên chủ của ví.
		return Owner;
	}
	double getMoney() const {  //Hàm trả về số tiền hiện có trong tài khoản.
		return TienHienCo;
	}
	vector<string> getLSGD() const {  //Hàm để lưu trữ lịch sử giao dịch.
		return LichSuGiaoDich;
	}
	void AddHistory(string x) {  //Thêm vào lịch sử giao dịch khi nạp, chuyển hoặc nhận tiền.
		LichSuGiaoDich.push_back(x);
	}
	void HienLs() const {  //Chương trình giúp hiển thị toàn bộ lịch sử giao dịch.
		for (int i = 0; i < LichSuGiaoDich.size(); i++) {
			cout << "-" << LichSuGiaoDich[i] << endl;
		}
	}
	void Gui(double amount) {  //Chương trình dùng để cộng tiền vào tài khoản khi nạp tiền vào hoặc nhận được từ tk khác.
		TienHienCo += amount;
		AddHistory("Da Nhan: +" + to_string(amount));
	}
	bool Rut(double amount) {  //Chương trình dùng để rút tiền hoặc bị trừ khi chuyển tiền.
		if (TienHienCo >= amount) {
			TienHienCo -= amount;
			AddHistory("Da rut: -" + to_string(amount));
			return true;
		}
		return false;
	}
};
class QLVi {
private:
	vector<Vi> vi;
public:
	void TaoVi(const string& Owner) {
		string Id = "W" + to_string(vi.size() + 1000);
		vi.emplace_back(Id, Owner);
		cout << "Id cua vi ban la: " << Id << endl;
		cout << "Chu vi: " << Owner << endl;
		LuuVi();
	}

	void LuuVi() {
		ofstream Out("wallets.txt");
		for (auto& x : vi) {
			Out << x.getId() << "," << x.getOwner() << "," << x.getMoney();
		for (auto& y : x.getLSGD()) {
			Out << "," << y;
		}
		Out << endl;
	}
}

void TaiVi() {
    vi.clear();
    ifstream In("wallets.txt");
    
    if (!In.is_open()) {
        cout << "Không tìm thấy file wallets.txt hoặc file trống" << endl;
        return;
    }

    string line;
    while (getline(In, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string id, owner, money;
        
        if (!getline(ss, id, ',') || 
            !getline(ss, owner, ',') || 
            !getline(ss, money, ',')) {
            cerr << "Dòng không hợp lệ: " << line << endl;
            continue;
        }

        try {
            Vi v(id, owner);
            v.Gui(stod(money));
            
            string history;
            while (getline(ss, history, ',')) {
                v.AddHistory(history);
            }
            
            vi.push_back(v);
        } catch (const std::exception& e) {
            cerr << "Lỗi xử lý dòng: " << line << " - " << e.what() << endl;
        }
    }
}

bool ChuyenTien(const string& x, const string& y, double amount) {
	Vi* ViDau = nullptr;
	Vi* ViDich = nullptr;
	for (auto& w : vi) {
		if (w.getId() == x) ViDau = &w;
		if (w.getId() == y) ViDich = &w;
	}
	if (ViDau && ViDich) {
		if (ViDau->Rut(amount)) {
			ViDich->Gui(amount);
			ViDau->AddHistory("Da chuyen " + to_string(amount) + " den " + ViDich->getId());
			ViDich->AddHistory("Da nhan " + to_string(amount) + " tu " + ViDau->getId());
			LuuVi();
			return true;
		}
		cout << "Số dư không đủ!\n";
	}
	else {
		cout << "Không tìm thấy ví nguồn hoặc đích!\n";
	}
	return false;
}

void ThongTinVi(const string& id) {
	for (auto& x : vi) {
		if (x.getId() == id) {
			cout << "\n== Thông tin ví ==\n";
			cout << "ID: " << x.getId() << endl;
			cout << "Chủ sở hữu: " << x.getOwner() << endl;
			cout << "Số dư: " << x.getMoney() << endl;
			x.HienLs();
			return;
		}
	}
	cout << "Không tìm thấy ví!\n";
}

bool DaTaoVi(const string& Tk) {
	for (auto& x : vi) {
		if (x.getOwner() == Tk) return true;
	}
	return false;
}

bool Nap(const string& Id, double amount) {
	for (auto& x : vi) {
		if (x.getId() == Id) {
			x.Gui(amount);
			LuuVi();
			return true;
		}
	}
	return false;
}
string LayViCuaChu(const string& TenChu) {
	for (auto& x : vi) {
		if (x.getOwner() == TenChu) return x.getId();
	}
	return "";
}
};


class TaiKhoanNguoiDung
{
public:
	string getCurrentUser() const {
		return currentUser;
	}
	
	bool isAdminLoggedIn() const {
		return isAdmin;
	}
	
	void DangXuat() {
		currentUser = "";
		isAdmin = false;
	}
	
	QLVi& getQLVi() {
		return qlVi;
	}
	TaiKhoanNguoiDung(const string& fileTK, const string& fileGD)
		: FileQuanLyTK(fileTK), FileGD(fileGD), isAdmin(false), qlVi()
	{
		srand((unsigned)time(0));
	}

	//hàm tạo tài khoản mới
	void taoTK() {
		string TenTK, MK, Ngaysinh, cccd, sdt;
		int ViChinh = 0, ViPhu = 0;
		bool admin = false;
		cout << "Nhap tên tài khoản mới: ";
		cin >> TenTK;
		MK = sinhMKtudong();
		cout << "Mật khẩu được tạo tự động: " << MK << endl;
		cout << "Nhập ngày sinh (dd/mm/yyyy): ";
		cin >> Ngaysinh;
		if (Ngaysinh.size() >= 8) {
			if (Ngaysinh[1] == '/')
				Ngaysinh = '0' + Ngaysinh;
			if (Ngaysinh[4] == '/')
				Ngaysinh.insert(3, "0");
		}
		cout << "Nhập số điện thoại:";
		cin >> sdt;
		cout << "Nhập số căn cước công dân: ";
		cin >> cccd;
		
		if (KtTenDangNhap(TenTK)) {
			cout << "Tên tài khoản " << TenTK << " đã tồn tại." << endl;
			return;
		}
		
		if (TenTK == "admin")
			admin = true;

		ofstream xuatFile(FileQuanLyTK, ios::app);
		if (xuatFile.is_open()) {
			xuatFile << TenTK << "," << bamMK(MK) << "," << Ngaysinh << "," << cccd << "," 
					 << sdt << "," << ViChinh << "," << ViPhu << "," << (admin ? '1' : '2') << endl;
			
			qlVi.TaoVi(TenTK);
			
			xuatFile.close();
			cout << "Tài khoản: " << TenTK << " đã được tạo thành công." << endl;
		} else {
			cout << "Không thể mở tệp để tạo tài khoản." << endl;
		}
	}

	void ThaydoiMatkhau() {
		if (currentUser.empty()) {
			cout << "Vui lòng đăng nhập trước." << endl;
			return;
		}
		string MKCu, MKMoi, XacnhanMKMoi, NhapOTP;
		cout << "Nhập mật khẩu cũ: ";
		cin >> MKCu;
		if (!KtThongtinTK(currentUser, MKCu)) {
			cout << "Mật khẩu cũ không đúng." << endl;
			return;
		}
		// Gửi OTP
		string otp = sinhOTP();
		vector<Taikhoan> ds = docDanhSachTK(FileQuanLyTK);
		for (int i = 0; i < ds.size(); i++)
		{
			if (ds[i].ten == currentUser)
			{
				cout << "Mã OTP của bạn đã được gửi đến số điện thoại: " << ds[i].sdt << ": " << otp << endl;
				break;
			}
		}
cout << "Nhập OTP: ";
cin >> NhapOTP;
if (NhapOTP != otp) {
	cout << "OTP không đúng. Đổi mật khẩu thất bại." << endl;
	return;
}

// Đổi mật khẩu
cout << "Nhập mật khẩu mới: ";
cin >> MKMoi;
cout << "Xác nhận mật khẩu mới: ";
cin >> XacnhanMKMoi;

if (MKMoi != XacnhanMKMoi) {
	cout << "Mật khẩu mới và xác nhận không khớp." << endl;
	return;
}

vector<Taikhoan> danhSach = docDanhSachTK(FileQuanLyTK);
for (auto& tk : danhSach) {
	if (tk.ten == currentUser) {
		tk.MK = bamMK(MKMoi); // Hash mật khẩu mới
		break;
	}
}

LuulaiTaiKhoan(danhSach);
cout << "Đổi mật khẩu thành công." << endl;
}
void DangNhap() {
	string tenDN, MK;
	cout << "============= ĐĂNG NHẬP ================" << endl;
	cout << "Tên đăng nhập: ";
	cin >> tenDN;
	cout << "Mật khẩu: ";
	cin >> MK;

	// Kiểm tra tài khoản
	if (!KtThongtinTK(tenDN, MK)) {
		cout << "Sai tên đăng nhập hoặc mật khẩu." << endl;
		return;
	}
	// Sinh và hiển thị OTP
	string otp = sinhOTP();
	// Gửi OTP (giả lập)
	vector<Taikhoan> ds = docDanhSachTK(FileQuanLyTK);
	bool timThay = false;
	for (int i = 0; i < ds.size(); i++) {
		if (ds[i].ten == tenDN) {
			cout << "Mã OTP đã được gửi tới số điện thoại: " << ds[i].sdt << ": " << otp << endl;
			timThay = true;
			break;
		}
	}
	if (!timThay) {
		cout << "Không tìm thấy tài khoản để gửi OTP." << endl;
		return;
	}
	// Nhập và xác thực OTP
	string nhapOTP1;
	cout << "Nhập mã OTP: ";
	cin >> nhapOTP1;

	if (nhapOTP1 != otp) {
		cout << "Mã OTP không đúng. Đăng nhập thất bại." << endl;
		return;
	}
	// Thành công
	currentUser = tenDN;
	isAdmin = (tenDN == "admin");
	cout << "Đăng nhập thành công. Xin chào, " << currentUser << "!" << endl;
	string nhapOTP2;
	cout << "Nhập mã OTP: ";
	cin >> nhapOTP2;
	if (nhapOTP2 != otp) {
		cout << "Mã OTP không đúng. Đăng nhập thất bại." << endl;
		return;
	}

	// Xác thực thành công
	currentUser = tenDN;
	for (auto& tk : ds) {
		if (tk.ten == tenDN) {
			isAdmin = tk.admin;
			break;
		}
	}
	cout << "Đăng nhập thành công với vai trò " << (isAdmin ? "Quản trị viên." : "Người dùng.") << endl;
	qlVi.TaiVi(); // Load dữ liệu ví sau khi đăng nhập
}
void XemThongTinVi()
{
	if (currentUser.empty())
	{
		cout << "Vui lòng đăng nhập trước.\n";
		return;
	}
	string viId = qlVi.LayViCuaChu(currentUser);
	if (viId.empty())
	{
		cout << "Bạn chưa có ví.\n";
		return;
	}
	qlVi.ThongTinVi(viId);
}
void NapTienVaoVi()
{
	if (currentUser.empty())
	{
		cout << "Vui lòng đăng nhập trước.\n";
		return;
	}
	double amount;
	cout << "Nhập số tiền muốn nạp: ";
	cin >> amount;
	string viId = qlVi.LayViCuaChu(currentUser);
	if (qlVi.Nap(viId, amount))
	{
		cout << "Nạp tiền thành công.\n";
	}
	else
	{
		cout << "Nạp tiền thất bại.\n";
	}
}
void ChuyenTien()
{
	if (currentUser.empty())
	{
		cout << "Vui lòng đăng nhập trước.\n";
		return;
	}
	string viDich;
	double amount;
	cout << "Nhập ID ví nhận tiền: ";
	cin >> viDich;
	cout << "Nhập số tiền muốn chuyển: ";
	cin >> amount;
	string viGui = qlVi.LayViCuaChu(currentUser);
	if (qlVi.ChuyenTien(viGui, viDich, amount))
	{
		cout << "Chuyển tiền thành công.\n";
	}
	else
	{
		cout << "Chuyển tiền thất bại.\n";
	}
}


int GuiVaXacNhanOTP(const string& sdt) {
	string otp = sinhOTP();
	cout << "Mã OTP đã được gửi đến số điện thoại " << sdt << ": " << otp << endl;
	cout << "Nhập OTP để xác nhận: ";
	string nhapOTP;
	cin >> nhapOTP;
	return nhapOTP == otp;
}
// Hàm cập nhật thông tin cá nhân (dành cho user thường)
 void CapNhatThongTinCaNhan() {
	if (currentUser.empty()) {
		cout << "Vui lòng đăng nhập trước khi cập nhật thông tin.\n";
		return;
	}

	vector<Taikhoan> ds = docDanhSachTK(FileQuanLyTK);
	int idx = -1;
	for (int i = 0; i < (int)ds.size(); i++) {
		if (ds[i].ten == currentUser) {
			idx = i;
			break;
		}
	}
	
	if (idx == -1) {
		cout << "Không tìm thấy tài khoản hiện tại.\n";
		return;
	}

	Taikhoan& tk = ds[idx];
	cout << "== Thông tin hiện tại của bạn ==\n";
	cout << "Ngày sinh: " << tk.Ngaysinh << endl;
	cout << "CCCD: " << tk.cccd << endl;
	cout << "Số điện thoại: " << tk.sdt << endl;

	cout << "Bạn muốn thay đổi trường nào? (1: Ngày sinh, 2: CCCD, 3: SDT, 4: Đổi mật khẩu, 0: Hủy)\n";
	int chon;
	cin >> chon;
	cin.ignore();

	string noiDungThayDoi = "Bạn sẽ thay đổi:\n";
	bool thayDoi = false;

	switch (chon) {
		case 1: {
			cout << "Nhập ngày sinh mới (dd/mm/yyyy): ";
			string ns;
			getline(cin, ns);
			noiDungThayDoi += "Ngày sinh từ [" + tk.Ngaysinh + "] thành [" + ns + "]\n";
if (!GuiVaXacNhanOTP(tk.sdt)) {
    cout << "Xác nhận OTP không thành công, hủy cập nhật.\n";
    return;
}
tk.Ngaysinh = ns;
thayDoi = true;
break;
}
case 2: {
    cout << "Nhập CCCD mới: ";
    string cccdMoi;
    getline(cin, cccdMoi);
    noiDungThayDoi += "CCCD từ [" + tk.cccd + "] thành [" + cccdMoi + "]\n";
    if (!GuiVaXacNhanOTP(tk.sdt)) {
        cout << "Xác nhận OTP không thành công, hủy cập nhật.\n";
        return;
    }
    tk.cccd = cccdMoi;
    thayDoi = true;
    break;
}
case 3: {
    cout << "Nhập số điện thoại mới: ";
    string sdtMoi;
    getline(cin, sdtMoi);
    noiDungThayDoi += "SDT từ [" + tk.sdt + "] thành [" + sdtMoi + "]\n";
    if (!GuiVaXacNhanOTP(tk.sdt)) {
        cout << "Xác nhận OTP không thành công, hủy cập nhật.\n";
        return;
    }
    tk.sdt = sdtMoi;
    thayDoi = true;
    break;
}
case 4: {
    cout << "Nhập mật khẩu cũ: ";
    string mkCu;
    cin >> mkCu;
    if (bamMK(mkCu) != tk.MK) {
        cout << "Mật khẩu cũ không đúng.\n";
        return;
    }
    cout << "Nhập mật khẩu mới: ";
    string mkMoi1, mkMoi2;
    cin >> mkMoi1;
    cout << "Xác nhận mật khẩu mới: ";
    cin >> mkMoi2;
    if (mkMoi1 != mkMoi2) {
        cout << "Mật khẩu xác nhận không khớp.\n";
        return;
    }
    noiDungThayDoi += "Mật khẩu được thay đổi.\n";
    if (!GuiVaXacNhanOTP(tk.sdt)) {
        cout << "Xác nhận OTP không thành công, hủy cập nhật.\n";
        return;
    }
    tk.MK = bamMK(mkMoi1);
    thayDoi = true;
    break;
}
case 0:
    cout << "Hủy cập nhật.\n";
    return;
default:
    cout << "Lựa chọn không hợp lệ.\n";
    return;
}

if (thayDoi) {
    LuulaiTaiKhoan(ds);
    cout << "Cập nhật thành công.\n";
}
}
// Hàm admin chỉnh sửa thông tin tài khoản khác
void AdminChinhSuaTaiKhoan() {
    if (!isAdmin) {
        cout << "Chỉ admin mới có quyền chỉnh sửa thông tin tài khoản khác.\n";
        return;
    }

    cout << "Nhập tên tài khoản cần chỉnh sửa: ";
    string tkChinhSua;
    cin >> tkChinhSua;

    if (tkChinhSua == "admin") {
        cout << "Không được phép thay đổi tài khoản admin.\n";
        return;
    }

    vector<Taikhoan> ds = docDanhSachTK(FileQuanLyTK);
    int idx = -1;
    for (int i = 0; i < (int)ds.size(); i++) {
        if (ds[i].ten == tkChinhSua) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        cout << "Không tìm thấy tài khoản " << tkChinhSua << ".\n";
        return;
    }

    Taikhoan& tk = ds[idx];
    cout << "== Thông tin hiện tại của tài khoản " << tkChinhSua << " ==\n";
    cout << "Ngày sinh: " << tk.Ngaysinh << endl;
    cout << "CCCD: " << tk.cccd << endl;
    cout << "Số điện thoại: " << tk.sdt << endl;

    cout << "Bạn muốn thay đổi trường nào? (1: Ngày sinh, 2: CCCD, 3: SDT, 4: Reset mật khẩu, 0: Hủy)\n";
    int chon;
    cin >> chon;
    cin.ignore();

    bool thayDoi = false;
    switch (chon) {
    case 1: {
        cout << "Nhập ngày sinh mới (dd/mm/yyyy): ";
        string ns;
        getline(cin, ns);
        tk.Ngaysinh = ns;
        thayDoi = true;
        break;
    }
    case 2: {
        cout << "Nhập CCCD mới: ";
        string cccdMoi;
        getline(cin, cccdMoi);
        tk.cccd = cccdMoi;
        thayDoi = true;
        break;
    }
    case 3: {
        cout << "Nhập số điện thoại mới: ";
        string sdtMoi;
        getline(cin, sdtMoi);
        tk.sdt = sdtMoi;
        thayDoi = true;
        break;
    }
    case 4: {
        string mkMoi = sinhMKtudong();
        cout << "Mật khẩu mới được tạo tự động: " << mkMoi << endl;
        tk.MK = bamMK(mkMoi);
        thayDoi = true;
        break;
    }
    case 0:
        cout << "Hủy cập nhật.\n";
        return;
    default:
        cout << "Lựa chọn không hợp lệ.\n";
        return;
    }

    if (thayDoi) {
        LuulaiTaiKhoan(ds);
        cout << "Cập nhật thành công.\n";
    }
}
// Hàm cho phép tạo ví thông qua đối tượng qlVi bên trong class
void TaoViChoNguoiDung(const string& tenChuVi) {
    qlVi.TaoVi(tenChuVi);
}

private:
string FileQuanLyTK; // File lưu tt tài khoản
string FileGD;       // File lưu lịch sử giao dịch
string currentUser;  // tên tài khoản hiện tại
bool isAdmin;        // quyền admin
QLVi qlVi;           // Quản lí ví giá lập

bool KtTenDangNhap(string TenDN) {
    ifstream nhapfile(FileQuanLyTK);
    string tmp;
    while (getline(nhapfile, tmp)) {
        stringstream ss(tmp);
        string TenTkinFile;
        getline(ss, TenTkinFile, ',');
        if (TenTkinFile == TenDN) {
            nhapfile.close();
            return 1;
        }
    }
    nhapfile.close();
    return 0;
}
bool KtThongtinTK(string TenTK, string MK) {
    ifstream nhapFile(FileQuanLyTK);
    string tmp;
    while (getline(nhapFile, tmp)) {
        stringstream ss(tmp);
        string TenTKinFile, MKinFile;
        getline(ss, TenTKinFile, ',');
getline(ss, MKinFile, ',');
if (TenTKinFile == TenTK && MKinFile == bamMK(MK))
{
    nhapFile.close();
    return 1;
}
}
nhapFile.close();
return 0;
}
void LuulaiTaiKhoan(vector<Taikhoan>& danhsachTaiKhoan)
{
    ofstream xuatFile(FileQuanLyTK);
    if (!xuatFile.is_open()) {
        cout << "Không thể mở tập để ghi Tài Khoản" << endl;
        return;
    }
    for (size_t i = 0; i < danhsachTaiKhoan.size(); i++)
    {
        xuatFile << danhsachTaiKhoan[i].ten << ","
            << danhsachTaiKhoan[i].MK << ","
            << danhsachTaiKhoan[i].Ngaysinh << ","
            << danhsachTaiKhoan[i].cccd << ","
            << danhsachTaiKhoan[i].sdt << ","
            << danhsachTaiKhoan[i].ViChinh << ","
            << danhsachTaiKhoan[i].ViPhu << ","
            << (danhsachTaiKhoan[i].admin ? "1" : "0")
            << endl;
    }
    xuatFile.close();
}
vector<Taikhoan> docDanhSachTK(string& tenFile) {
    vector<Taikhoan> danhSach;
    ifstream nhapFile(tenFile);
    if (!nhapFile.is_open()) {
        cout << "Không thể mở file " << tenFile << endl;
        return danhSach;
    }

    string tmp;
    while (getline(nhapFile, tmp)) {
        stringstream ss(tmp);
        Taikhoan tk;
        string adminStr, ViChinhStr, ViPhuStr;

        // Đọc các trường theo đúng thứ tự và phân cách bằng dấu phẩy
        getline(ss, tk.ten, ',');
        getline(ss, tk.MK, ',');
        getline(ss, tk.Ngaysinh, ',');
        getline(ss, tk.cccd, ',');
        getline(ss, tk.sdt, ',');
        getline(ss, ViChinhStr, ',');
        getline(ss, ViPhuStr, ',');
        getline(ss, adminStr);

        // Chuyển đổi các trường số
        try {
            tk.ViChinh = ViChinhStr.empty() ? 0 : stoi(ViChinhStr);
            tk.ViPhu = ViPhuStr.empty() ? 0 : stoi(ViPhuStr);
            tk.admin = (adminStr == "1");
        } catch (const std::invalid_argument& e) {
            cerr << "Lỗi chuyển đổi số: " << e.what() << endl;
            continue; // Bỏ qua dòng lỗi
        }

        danhSach.push_back(tk);
    }
    nhapFile.close();
    return danhSach;
}
};

int main() {
    srand(time(0));
    TaiKhoanNguoiDung qlTK("accounts.txt", "transactions.txt");
    int choice;

    do {
        cout << "\n============= HỆ THỐNG QUẢN LÝ TÀI KHOẢN =============\n";
        cout << "1. Đăng nhập\n";
        cout << "2. Tạo tài khoản mới\n";
        cout << "3. Thoát\n";
        cout << "Lựa chọn của bạn: ";
        cin >> choice;

        switch(choice) {
            case 1: {
                qlTK.DangNhap();
                if (!qlTK.getCurrentUser().empty()) {
                    int userChoice;
                    do {
                        cout << "\n============= MENU NGƯỜI DÙNG =============\n";
                        cout << "1. Xem thông tin ví\n";
                        cout << "2. Nạp tiền vào ví\n";
                        cout << "3. Chuyển tiền\n";
                        cout << "4. Thay đổi mật khẩu\n";
                        cout << "5. Cập nhật thông tin cá nhân\n";

                        // Nếu là admin
                        if (qlTK.isAdminLoggedIn()) {
                            cout << "6. Quản lý tài khoản (Admin)\n";
                            cout << "7. Tạo ví mới\n";
                        }

                        cout << "0. Đăng xuất\n";
                        cout << "Lựa chọn của bạn: ";
                        cin >> userChoice;

                        switch(userChoice) {
                            case 1:
                                qlTK.XemThongTinVi();
                                break;
                            case 2:
                                qlTK.NapTienVaoVi();
                                break;
                            case 3:
                                qlTK.ChuyenTien();
                                break;
                            case 4:
                                qlTK.ThaydoiMatkhau();
                                break;
                            case 5:
                                qlTK.CapNhatThongTinCaNhan();
                                break;
                            case 6:
                                if (qlTK.isAdminLoggedIn()) {
                                    qlTK.AdminChinhSuaTaiKhoan();
                                }
                                break;
                            case 7:
                                if (qlTK.isAdminLoggedIn()) {
                                    string owner;
                                    cout << "Nhập tên chủ ví: ";
                                    cin >> owner;
                                    qlTK.TaoViChoNguoiDung(owner); // gọi hàm public mới

                                }
                                break;
                            case 0:
                                qlTK.DangXuat();
                                cout << "Đã đăng xuất thành công.\n";
                                break;
                            default:
                                cout << "Lựa chọn không hợp lệ.\n";
                        }
                    } while (userChoice != 0);
                }
                break;
            }
            case 2:
                qlTK.taoTK();
                break;
            case 3:
                cout << "Cảm ơn bạn đã sử dụng dịch vụ!\n";
                break;
            default:
                cout << "Lựa chọn không hợp lệ. Vui lòng chọn lại.\n";
        }
    } while (choice != 3);

    return 0;
}

