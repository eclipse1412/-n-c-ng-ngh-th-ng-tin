#include<iostream>//Thư viện nhập xuâts
#include<string>//Thư viện sử dụng kiểu dữ liệu string
#include<fstream>//Thư viện thao tác với tệp đọc ghi
#include<sstream>//Thư viện thao luồng chuỗi
#include<functional>//Thư viện hàm hash
#include<vector>//Thư viện sử dụng vector
#include<ctime>//Thư viện cho các hàm thời gian
#include<iomanip>//Thư viện định dạng đầu ra
using namespace std;
//Hàm băm mk chuyển mk thành chuỗi
string bamMK(string& matkhau)
{
	hash<string>hashObj;
	size_t hashValue = hashObj(matkhau);
	return to_string(hashValue);//chuyển thành chuỗi
}
//Hàm sinh OTP ngẫu nhiên 
string sinhOTP() {
	int otp = rand() % 9000 + 1000; // Sinh 4 chữ  số
	return to_string(otp); // Chuyển số thành chữ
}
//Hàm sinh mk tự động gồm 8 kí tự
string sinhMKtudong()
{
	string ktu = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	string MK = "";
	for (int i = 0;i < 8;i++)
	{
		MK += ktu[rand() % ktu.size()];
	}
	return MK;
}
//Struct đại diện cho 1 tài khoản người dùng
struct Taikhoan
{
	string ten, MK, Ngaysinh, cccd,sdt;
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
	string getId()const {  //Hàm trả về Id của ví
		return Id;
	}
	string getOwner()const {  //hàm trả về tên chủ cái ví.
		return Owner;
	}
	double getMoney()const {  //hàm trả về số tiền hiện có trong tài khoản.
		return TienHienCo;
	}
	vector<string> getLSGD()const {  //Hàm lưu lịch sử giao dịch.
		return LichSuGiaoDich;
	}
	void AddHistory(string x) {  //Thêm vào lịch sử giao dịch khi nạp, chuyển hoặc nhậnn tiền.
		LichSuGiaoDich.push_back(x);
	}
	void HienLs()const {  //hiển thị lịch sử giao dịch
		for (int i = 0; i < LichSuGiaoDich.size(); i++) {
			cout << "-" << LichSuGiaoDich[i] << endl;
		}
	}
	void Gui(double amount) {  
		TienHienCo += amount;
		AddHistory("Da Nhan: +" + to_string(amount));
	}
	bool Rut(double amount) {  
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
		TaiVi();
		string Id = "W" + to_string(vi.size() + 1000);
		vi.emplace_back(Id, Owner);
		cout << "Id cua vi ban la: " << Id << endl;
		cout << "Chu vi: " << Owner << endl;
		LuuVi();
	}

	void LuuVi() {
		ofstream Out("wallets.txt", ios::trunc);
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
        cout << "Khong tim thay file wallets.txt ." << endl;
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
            cerr << "D?ng không h?p l?: " << line << endl;
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
            cerr << "L?i x? l? d?ng: " << line << " - " << e.what() << endl;
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
			cout << "So du khong du!\n";
		}
		else {
			cout << "Khong tim thay vi nguon hoac dich!\n";
		}
		return false;
	}

	void ThongTinVi(const string& id) {
		for (auto& x : vi) {
			if (x.getId() == id) {
				cout << "\n== Thong tin vi ==\n";
				cout << "ID: " << x.getId() << endl;
				cout << "Chu so huu: " << x.getOwner() << endl;
				cout << "So du: " << x.getMoney() << endl;
				x.HienLs();
				return;
			}
		}
		cout << "Khong tim thay vi!\n";
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

	//hàm t?o tài kho?n m?i
	void taoTK() {
    string TenTK, MK, Ngaysinh, cccd, sdt;
    int ViChinh = 0, ViPhu = 0;
    bool admin = false;
    cout << "Nhap tên tài khoan moi: ";
    cin >> TenTK;
    MK = sinhMKtudong();
    cout << "Mat khau duoc tao tu dong: " << MK << endl;
    cout << "Nhap ngày sinh (dd/mm/yyyy): ";
    cin >> Ngaysinh;
    if (Ngaysinh.size() >= 8) {
        if (Ngaysinh[1] == '/')
            Ngaysinh = '0' + Ngaysinh;
        if (Ngaysinh[4] == '/')
            Ngaysinh.insert(3, "0");
    }
    cout << "Nhap so dien thoai:";
    cin >> sdt;
    cout << "Nhâp so can cuoc công dân: ";
    cin >> cccd;
    
    if (KtTenDangNhap(TenTK)) {
        cout << "Tên Tài Khoan " << TenTK << " da ton tai." << endl;
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
        cout << "Tài Khoan: " << TenTK << " da duoc tao thành công." << endl;
    } else {
        cout << "Không the mo tep de tao Tài Khoan." << endl;
    }
}

	void ThaydoiMatkhau() {
		if (currentUser.empty()) {
			cout << "Vui long dang nhap truoc." << endl;
			return;
		}
		string MKCu, MKMoi, XacnhanMKMoi, NhapOTP;
		cout << "Nhap mat khau cu: ";
		cin >> MKCu;
		if (!KtThongtinTK(currentUser, MKCu)) {
			cout << "Mat khau cu khong dung." << endl;
			return;
		}
		// Gui OTP
		string otp = sinhOTP();
		vector<Taikhoan> ds = docDanhSachTK(FileQuanLyTK);
		for (int i = 0;i < ds.size();i++)
		{
			if (ds[i].ten == currentUser)
			{
				cout << "Ma OTP cua ban da duoc gui den so dien thoai:" << ds[i].sdt << ":" << otp << endl;
				break;
			}
		}
		cout << "Nhap OTP: ";
		cin >> NhapOTP;
		if (NhapOTP != otp) {
			cout << "OTP khong dung. Ðoi mat khau that bai." << endl;
			return;
		}

		// đổi MK
		cout << "Nhap mat khau moi: ";
		cin >> MKMoi;
		cout << "Xác nhan mat khau moi: ";
		cin >> XacnhanMKMoi;

		if (MKMoi != XacnhanMKMoi) {
			cout << "Mat khau moi và xác nhan không khop." << endl;
			return;
		}

		vector<Taikhoan> danhSach = docDanhSachTK(FileQuanLyTK);
		for (auto& tk : danhSach) {
			if (tk.ten == currentUser) {
				tk.MK = bamMK(MKMoi); // Hash m?t kh?u m?i
				break;
			}
		}

		LuulaiTaiKhoan(danhSach);
		cout << "Ð?i m?t kh?u thành công." << endl;
	}
	void DangNhap() {
		string tenDN, MK;
		cout << "============= ÐANG NHAP ================" << endl;
		cout << "Ten dang nhap: ";
		cin >> tenDN;
		cout << "Mat khau: ";
		cin >> MK;

		// Kiem tra tai khoan
		if (!KtThongtinTK(tenDN, MK)) {
			cout << "Sai ten dang nhap hoac mat khau." << endl;
			return;
		}
		// Sinh và hiển thị OTP
		string otp = sinhOTP();
		// Gửi OTP (giả lậpp)
		vector<Taikhoan> ds = docDanhSachTK(FileQuanLyTK);
		bool timThay = false;
		for (int i = 0; i < ds.size(); i++) {
			if (ds[i].ten == tenDN) {
				cout << "Ma OTP da duoc gui toi so dien thoai: " << ds[i].sdt << ": " << otp << endl;
				timThay = true;
				break;
			}
		}
		if (!timThay) {
			cout << "Khong tim thay tai khoan de gui OTP." << endl;
			return;
		}
		// Nhập xác thực bằng otp
		string nhapOTP1;
		cout << "Nhap ma OTP: ";
		cin >> nhapOTP1;

		if (nhapOTP1 != otp) {
			cout << "Ma OTP khong dung. Ðãng nhap that bai." << endl;
			return;
		}
		// Thành công
		currentUser = tenDN;
		isAdmin = (tenDN == "admin");
		cout << "Ðang nhap thanh cong, xin chao " << currentUser << "!" << endl;
		string nhapOTP2;
		cout << "Nhap ma OTP: ";
		cin >> nhapOTP2;
		if (nhapOTP2 != otp) {
			cout << "Ma OTP khong dung. Ðang nhap that bai." << endl;
			return;
		}

		// Xác th?c thành công
		currentUser = tenDN;
		for (auto& tk : ds) {
			if (tk.ten == tenDN) {
				isAdmin = tk.admin;
				break;
			}
		}
		cout << "Ðãng nhập thành công vai vai tro " << (isAdmin ? "Qu?n tri viên." : "Nguoi dung.") << endl;
		qlVi.TaiVi(); //  lưu ví sau khi ðăng nhập
	}
	void XemThongTinVi()
	{
		if (currentUser.empty())
		{
			cout << "Vui l?ng ðãng nh?p trý?c.\n";
			return;
		}
		string viId = qlVi.LayViCuaChu(currentUser);
		if (viId.empty())
		{
			cout << "B?n chýa có ví.\n";
			return;
		}
		qlVi.ThongTinVi(viId);
	}
	void NapTienVaoVi()
	{
		if (currentUser.empty())
		{
			cout << "Vui l?ng ðãng nh?p trý?c.\n";
			return;
		}
		double amount;
		cout << "Nh?p s? ti?n mu?n n?p: ";
		cin >> amount;
		string viId = qlVi.LayViCuaChu(currentUser);
		if (qlVi.Nap(viId, amount))
		{
			cout << "N?p ti?n thành công.\n";
		}
		else
		{
			cout << "N?p ti?n th?t b?i.\n";
		}
	}
	void ChuyenTien()
	{
		if (currentUser.empty())
		{
			cout << "Vui l?ng ðãng nh?p trý?c.\n";
			return;
		}
		string viDich;
		double amount;
		cout << "Nhap ID vi nhan tien: ";
		cin >> viDich;
		cout << "Nhap so tien muon chuyen: ";
		cin >> amount;
		string viGui = qlVi.LayViCuaChu(currentUser);
		if (qlVi.ChuyenTien(viGui, viDich, amount))
		{
			cout << "Chuyen tien thanh cong.\n";
		}
		else
		{
			cout << "Chuyen tien thet bai.\n";
		}
	}


	int GuiVaXacNhanOTP(const string& sdt) {
		string otp = sinhOTP();
		cout << "Ma OTP da duoc gui den so dien thoai " << sdt << ": " << otp << endl;
		cout << "Nhap OTP de xac nhan: ";
		string nhapOTP;
		cin >> nhapOTP;
		return nhapOTP == otp;
	}
	// Hàm cập nhập thông tin cá nhân (dành cho user)
	 void CapNhatThongTinCaNhan() {
        if (currentUser.empty()) {
            cout << "Vui long dang nhap truoc khi cap nhat thong tin.\n";
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
            cout << "Khong tim thay tai khoan hien tai.\n";
            return;
        }

        Taikhoan& tk = ds[idx];
        cout << "== Thong tin hien tai cua ban ==\n";
        cout << "Ngay sinh: " << tk.Ngaysinh << endl;
        cout << "CCCD: " << tk.cccd << endl;
        cout << "So dien thoai: " << tk.sdt << endl;

        cout << "Ban muon thay doi truong nao? (1: Ngay sinh, 2: CCCD, 3: SDT, 4: Doi mat khau, 0: Huy)\n";
        int chon;
        cin >> chon;
        cin.ignore();

        string noiDungThayDoi = "Ban se thay doi:\n";
        bool thayDoi = false;

        switch (chon) {
            case 1: {
                cout << "Nhap ngay sinh moi (dd/mm/yyyy): ";
                string ns;
                getline(cin, ns);
                noiDungThayDoi += "Ngay sinh tu [" + tk.Ngaysinh + "] thanh [" + ns + "]\n";
                if (!GuiVaXacNhanOTP(tk.sdt)) {
                    cout << "Xac nhan OTP khong thanh cong, huy cap nhat.\n";
                    return;
                }
                tk.Ngaysinh = ns;
                thayDoi = true;
                break;
            }
            case 2: {
                cout << "Nhap CCCD moi: ";
                string cccdMoi;
                getline(cin, cccdMoi);
                noiDungThayDoi += "CCCD tu [" + tk.cccd + "] thanh [" + cccdMoi + "]\n";
                if (!GuiVaXacNhanOTP(tk.sdt)) {
                    cout << "Xac nhan OTP khong thanh cong, huy cap nhat.\n";
                    return;
                }
                tk.cccd = cccdMoi;
                thayDoi = true;
                break;
            }
            case 3: {
                cout << "Nhap so dien thoai moi: ";
                string sdtMoi;
                getline(cin, sdtMoi);
                noiDungThayDoi += "SDT tu [" + tk.sdt + "] thanh [" + sdtMoi + "]\n";
                if (!GuiVaXacNhanOTP(tk.sdt)) {
                    cout << "Xac nhan OTP khong thanh cong, huy cap nhat.\n";
                    return;
                }
                tk.sdt = sdtMoi;
                thayDoi = true;
                break;
            }
            case 4: {
                cout << "Nhap mat khau cu: ";
                string mkCu;
                cin >> mkCu;
                if (bamMK(mkCu) != tk.MK) {
                    cout << "Mat khau cu khong dung.\n";
                    return;
                }
                cout << "Nhap mat khau moi: ";
                string mkMoi1, mkMoi2;
                cin >> mkMoi1;
                cout << "Xac nhan mat khau moi: ";
                cin >> mkMoi2;
                if (mkMoi1 != mkMoi2) {
                    cout << "Mat khau xac nhan khong khop.\n";
                    return;
                }
                noiDungThayDoi += "Mat khau duoc thay doi.\n";
                if (!GuiVaXacNhanOTP(tk.sdt)) {
                    cout << "Xac nhan OTP khong thanh cong, huy cap nhat.\n";
                    return;
                }
                tk.MK = bamMK(mkMoi1);
                thayDoi = true;
                break;
            }
            case 0:
                cout << "Huy cap nhat.\n";
                return;
            default:
                cout << "Lua chon khong hop le.\n";
                return;
        }

        if (thayDoi) {
            LuulaiTaiKhoan(ds);
            cout << "Cap nhat thanh cong.\n";
        }
    }
	// Hàm admin chỉnnh sửa thông tin tài khoản khác
	void AdminChinhSuaTaiKhoan() {
		if (!isAdmin) {
			cout << "Chi admin moi co quyen chinh sua thong tin tai khoan khac.\n";
			return;
		}

		cout << "Nhap ten tai khoan can chinh sua: ";
		string tkChinhSua;
		cin >> tkChinhSua;

		if (tkChinhSua == "admin") {
			cout << "Khong duoc phep thay doi tai khoan admin.\n";
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
			cout << "Khong tim thay tai khoan " << tkChinhSua << ".\n";
			return;
		}

		Taikhoan& tk = ds[idx];
		cout << "== Thong tin hien tai cua tai khoan " << tkChinhSua << " ==\n";
		cout << "Ngay sinh: " << tk.Ngaysinh << endl;
		cout << "CCCD: " << tk.cccd << endl;
		cout << "So dien thoai: " << tk.sdt << endl;

		cout << "Ban muon thay doi truong nao? (1: Ngay sinh, 2: CCCD, 3: SDT, 4: Reset mat khau, 0: Huy)\n";
		int chon;
		cin >> chon;
		cin.ignore();

		bool thayDoi = false;
		switch (chon) {
		case 1: {
			cout << "Nhap ngay sinh moi (dd/mm/yyyy): ";
			string ns;
			getline(cin, ns);
			tk.Ngaysinh = ns;
			thayDoi = true;
			break;
		}
		case 2: {
			cout << "Nhap CCCD moi: ";
			string cccdMoi;
			getline(cin, cccdMoi);
			tk.cccd = cccdMoi;
			thayDoi = true;
			break;
		}
		case 3: {
			cout << "Nhap so dien thoai moi: ";
			string sdtMoi;
			getline(cin, sdtMoi);
			tk.sdt = sdtMoi;
			thayDoi = true;
			break;
		}
		case 4: {
			string mkMoi = sinhMKtudong();
			cout << "Mat khau moi duoc tao tu dong: " << mkMoi << endl;
			tk.MK = bamMK(mkMoi);
			thayDoi = true;
			break;
		}
		case 0:
			cout << "Huy cap nhat.\n";
			return;
		default:
			cout << "Lua chon khong hop le.\n";
			return;
		}

		if (thayDoi) {
			LuulaiTaiKhoan(ds);
			cout << "Cap nhat thanh cong.\n";
		}
	}
    // Hàm cho phép t?o ví thông qua d?i tu?ng qlVi bên trong class
    void TaoViChoNguoiDung(const string& tenChuVi) {
        qlVi.TaoVi(tenChuVi);
    }



private:
	string FileQuanLyTK;//File lý tt tài khoản
	string FileGD;//File lý lịch sử giao dịch
	string currentUser;//tên tài khoản hiện tại
	bool isAdmin;//quyền admin
	QLVi qlVi;//Quản lí ví giá lập

	bool KtTenDangNhap( string TenDN)
	{
		ifstream nhapfile(FileQuanLyTK);
		string tmp;
		while (getline(nhapfile, tmp))
		{
			stringstream ss(tmp);
			string TenTkinFile;
			getline(ss, TenTkinFile, ',');
			if (TenTkinFile == TenDN)
			{
				nhapfile.close();
				return 1;
			}
		}
		nhapfile.close();
		return 0;
	}
//Ktra xác thực tên người dùng và MK
	bool KtThongtinTK(string TenTK, string MK)
	{
		ifstream nhapFile(FileQuanLyTK);
		string tmp;
		while (getline(nhapFile, tmp))
		{
			stringstream ss(tmp);
			string TenTKinFile, MKinFile;
			getline(ss, TenTKinFile, ',');
			getline(ss, MKinFile, ',');
			//so sánh tên tên và mk
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
			cout << "Khong the mo tep ghi tai khoan" << endl;
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
// Dùng để đọc tất cả dữ liệu tài khoảng trong file
vector<Taikhoan> docDanhSachTK(string& tenFile) {
    vector<Taikhoan> danhSach;
    ifstream nhapFile(tenFile);
    if (!nhapFile.is_open()) {
        cout << "Không th? m? file " << tenFile << endl;
        return danhSach;
    }

    string tmp;
    while (getline(nhapFile, tmp)) {
        stringstream ss(tmp);
        Taikhoan tk;
        string adminStr, ViChinhStr, ViPhuStr;

        // Ðọc các trường theo ðúng thứ tự và phân cách bằng dấu phẩy
        getline(ss, tk.ten, ',');
        getline(ss, tk.MK, ',');
        getline(ss, tk.Ngaysinh, ',');
        getline(ss, tk.cccd, ',');
        getline(ss, tk.sdt, ',');
        getline(ss, ViChinhStr, ',');
        getline(ss, ViPhuStr, ',');
        getline(ss, adminStr);

        // Chuyển đổi các trường
        try {
            tk.ViChinh = ViChinhStr.empty() ? 0 : stoi(ViChinhStr);
            tk.ViPhu = ViPhuStr.empty() ? 0 : stoi(ViPhuStr);
            tk.admin = (adminStr == "1");
        } catch (const std::invalid_argument& e) {
            cerr << "Lỗi chuyển đổi số: " << e.what() << endl;
            continue; // Bỏ qua dòng lệnh
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
        cout << "\n============= HE THONG QUAN LY TAI KHOAN =============\n";
        cout << "1. Dang nhap\n";
        cout << "2. Tao tai khoan moi\n";
        cout << "3. Thoat\n";
        cout << "Lua chon cua ban: ";
        cin >> choice;
        
        switch(choice) {
            case 1: {
                qlTK.DangNhap();
                if (!qlTK.getCurrentUser().empty()) {
                    int userChoice;
                    do {
                        cout << "\n============= MENU NGUOI DUNG =============\n";
                        cout << "1. Xem thong tin vi\n";
                        cout << "2. Nap tien vao vi\n";
                        cout << "3. Chuyen tien\n";
                        cout << "4. Thay doi mat khau\n";
                        cout << "5. Cap nhat thong tin ca nhan\n";
                        
                        // N?u là admin
                        if (qlTK.isAdminLoggedIn()) {
                            cout << "6. Quan ly tai khoan (Admin)\n";
                            cout << "7. Tao vi moi\n";
                        }
                        
                        cout << "0. Dang xuat\n";
                        cout << "Lua chon cua ban: ";
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
                                    cout << "Nhap ten chu vi: ";
                                    cin >> owner;
                                    qlTK.TaoViChoNguoiDung(owner); 
                                }
                                break;
                            case 0:
                                qlTK.DangXuat();
                                cout << "Da dang xuat thanh cong.\n";
                                break;
                            default:
                                cout << "Lua chon khong hop le.\n";
                        }
                    } while (userChoice != 0);
                }
                break;
            }
            case 2:
                qlTK.taoTK();
                break;
            case 3:
                cout << "Cam on ban da su dung dich vu!\n";
                break;
            default:
                cout << "Lua chon khong hop le. Vui long chon lai.\n";
        }
    } while (choice != 3);
    
    return 0;
}
