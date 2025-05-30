#include <iostream>
#include <stdio.h>
#include <string>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <cstring>
#include <stdlib.h>
#include <limits>
#include <io.h>
#include <vector>

using namespace std;

// ==================== DEKLARASI STRUCT ====================

// menyimpan data barang
struct stokBarang
{
    string namaBarang;
    int jml_barang;
    float harga;
};

// menyimpan data keuangan
struct keuangan
{
    float pemasukan = 0, pengeluaran = 0, sisa_saldo = 0;
};

// menyimpan data cabang
struct cabang
{
    string namaCabang, username, password;
    stokBarang stok[1000];
    keuangan keuangan;
    int jml_stok = 0;
    cabang *next;
};
cabang* head = NULL;

// fungsi konversi
string intToString(int num) {
    stringstream ss;
    ss << num;
    return ss.str();
}

// ==================== SORTING & SEARCHING ====================

// Fungsi Shell Sort universal 
void shellSort(stokBarang arr[], int size, int mode = 0) {
    // mode: 0=by name asc, 1=by name desc, 2=by stock asc, 3=by stock desc
    for (int gap = size/2; gap > 0; gap /= 2) {
        for (int i = gap; i < size; i++) {
            stokBarang temp = arr[i];
            int j;
            
            if (mode == 0) { // by name asc
                for (j = i; j >= gap && arr[j-gap].namaBarang > temp.namaBarang; j -= gap)
                    arr[j] = arr[j-gap];
            }
            else if (mode == 1) { // by name desc
                for (j = i; j >= gap && arr[j-gap].namaBarang < temp.namaBarang; j -= gap)
                    arr[j] = arr[j-gap];
            }
            else if (mode == 2) { // by stock asc
                for (j = i; j >= gap && arr[j-gap].jml_barang > temp.jml_barang; j -= gap)
                    arr[j] = arr[j-gap];
            }
            else if (mode == 3) { // by stock desc
                for (j = i; j >= gap && arr[j-gap].jml_barang < temp.jml_barang; j -= gap)
                    arr[j] = arr[j-gap];
            }
            
            arr[j] = temp;
        }
    }
}

// Fungsi Sequential Search universal 
int searchStok(stokBarang arr[], int size, string keyword, int mode = 0) {
    // mode: 0=by name, 1=by stock
    for (int i = 0; i < size; i++) {
        if (mode == 0) { // search by name
            if (arr[i].namaBarang.find(keyword) != string::npos)
                return i;
        }
        else if (mode == 1) { // search by stock
            if (intToString(arr[i].jml_barang) == keyword)
                return i;
        }
    }
    return -1;
}

// ==================== FILE ====================

// fungsi untuk menyimpan akun semua cabang
void saveAllCabang(cabang* head) {
    FILE* file = fopen("akun.txt", "w");  
    if (file == NULL) {
        cout << "Gagal membuka file untuk penyimpanan." << endl;
        return;
    }
    
    cabang* current = head;
    while (current != NULL) {
        fprintf(file, "%s\n%s\n%s\n", 
            current->namaCabang.c_str(),
            current->username.c_str(),
            current->password.c_str());
            current = current->next;
        }
        fclose(file);
    }

// fungsi untuk membaca data file akun semua cabang 
void readFileAkun(cabang** head) {  // Gunakan pointer ke pointer untuk mengubah head
        FILE* file = fopen("akun.txt", "r");
        if (file == NULL) {
            cout << "File tidak ditemukan, akan dibuat baru saat menyimpan." << endl;
            return;
        }
    
        char buffer[256];
        while (true) {
            cabang* new_cabang = new cabang();
            
            // Baca namaCabang
            if (fgets(buffer, sizeof(buffer), file) == NULL) break;
            buffer[strcspn(buffer, "\n")] = '\0';  // Hapus newline
            new_cabang->namaCabang = buffer;
            
            // Baca username
            if (fgets(buffer, sizeof(buffer), file) == NULL) break;
            buffer[strcspn(buffer, "\n")] = '\0';
            new_cabang->username = buffer;
            
            // Baca password
            if (fgets(buffer, sizeof(buffer), file) == NULL) break;
            buffer[strcspn(buffer, "\n")] = '\0';
            new_cabang->password = buffer;
            
            new_cabang->next = NULL;
    
            
            // Tambahkan ke linked list
            if (*head == NULL) {
                *head = new_cabang;
            } else {
                cabang* temp = *head;
                while (temp->next != NULL) {
                    temp = temp->next;
                }
                temp->next = new_cabang;
            }
        }
        fclose(file);
}

// ==================== MANAJEMEN CABANG ====================

//fungsi menambahkan cabang 
void tambahCabangFile(cabang *head){
    FILE* file = fopen("akun.txt", "a");
    if (file == NULL) {
        cout << "Gagal membuka file untuk penyimpanan." << endl;
        return;
    }
    cabang* current = head;
    while (current != NULL) {
        fprintf(file, "%s\n%s\n%s\n", 
                current->namaCabang.c_str(),
                current->username.c_str(),
                current->password.c_str());
        current = current->next;
    }
    fclose(file);
}

void tambahCabang(){
    system("cls");

    cout << "\n===================================================\n";
    cout << "|| " << setw(45) << left << "TAMBAH CABANG BARU" << "||\n";
    cout << "===================================================\n";

    cabang* new_cabang = new cabang;
    cout << "Nama cabang: ";
    getline(cin, new_cabang->namaCabang);
    cout << "username: "; 
    getline(cin, new_cabang->username);
    cout << "password: "; 
    getline(cin, new_cabang->password);

    new_cabang->next = NULL;

    if (head == NULL)
    {
        head = new_cabang;
    } else {
        cabang* temp = head;
        while (temp->next != NULL){
            temp = temp->next;
        }
        temp->next = new_cabang;
    }
    tambahCabangFile(new_cabang);
    cout << "Cabang berhasil ditambahkan\n";
    system("pause");
}

// fungsi menampilkan cabang yang ada
void lihatCabang(){
    system("cls");

    cout << "\n===================================================\n";
    cout << "|| " << setw(45) << left << "DAFTAR CABANG" << "||\n";
    cout << "===================================================\n";

    if(head == NULL){
        cout << "belum ada cabang yang ditambahkan" << endl;
        system("pause");
        return;
    }
    
    cout << "\n+------+----------------------+\n";
    cout << "| No.  | Nama Cabang          |\n";
    cout << "+------+----------------------+\n";
    
    int i = 1;
    cabang* temp = head;
    while (temp != NULL){
        cout << "| " << setw(4) << left << i++ << " | " 
             << setw(20) << temp->namaCabang << " |\n";
        temp = temp->next;
    }
    cout << "+------+----------------------+\n";
    system("pause");
}

// fungsi hapus cabang
void hapusCabang(string namaCabang) {
    if (head == NULL) {
        cout << "Belum ada cabang." << endl;
        system("pause");
        return;
    }

    // Jika yang dihapus adalah head
    if (head->namaCabang == namaCabang) {
        cabang* hapus = head;
        head = head->next;
        delete hapus;
        cout << "Cabang berhasil dihapus." << endl;
        saveAllCabang(head); // Simpan perubahan ke file
        return;
    }

    cabang* bantu = head;
    while (bantu->next != NULL && bantu->next->namaCabang != namaCabang) {
        bantu = bantu->next;
    }

    if (bantu->next == NULL) {
        cout << "Cabang tidak ditemukan." << endl;
    } else {
        cabang* hapus = bantu->next;
        bantu->next = hapus->next;
        delete hapus;
        saveAllCabang(head); // Simpan perubahan ke file
        cout << "Cabang berhasil dihapus." << endl;
    }
    system("pause");
}

// ==================== MANAJEMEN STOK ====================
void stokCabangFile(cabang *temp){
    string filename = temp->namaCabang + "_stok.txt";
    // Cek apakah file sudah ada
    bool fileExists = (access(filename.c_str(), F_OK) != -1);
    FILE* file = fopen(filename.c_str(), "a");
    if (file == NULL) {
        cout << "Gagal membuka file untuk penyimpanan." << endl;
        return;
    }
    if (!fileExists) {
        fprintf(file, "Nama Barang,Jumlah,Harga\n");
    }
    for(int i = 0; i < temp->jml_stok; i++){
        fprintf(file, "%s,%d,%.2f\n", 
            temp->stok[i].namaBarang.c_str(),
            temp->stok[i].jml_barang,
            temp->stok[i].harga);
        }
        fclose(file);
}

void tambahStok(cabang* temp){
    system("cls");

    cout << "\n===================================================\n";
    cout << "|| " << setw(45) << left << "TAMBAH STOK BARANG" << "||\n";
    cout << "===================================================\n";

    int jml;
    cout << "Jumlah barang yang ingin diinput: "; cin >> jml;

    for(int i=0; i<jml; i++){
        cout << "Barang ke-" << i+1 << endl;
        cout << "Nama barang: "; cin.ignore(); getline(cin, temp->stok[temp->jml_stok].namaBarang);
        cout << "Jumlah barang: "; cin >> temp->stok[temp->jml_stok].jml_barang;
        cout << "Harga barang: "; cin >> temp->stok[temp->jml_stok].harga;
        temp->jml_stok++;
        cout << endl;
    }

    stokCabangFile(temp);
    cout << "Stok berhasil ditambahkan.\n";
    system("pause");
}

void lihatStockFile(cabang *temp) {
    system("cls");

    cout << "\n===================================================\n";
    cout << "|| " << setw(45) << left << "LAPORAN STOK " << temp->namaCabang << "||\n";
    cout << "===================================================\n";
    string filename = temp->namaCabang + "_stok.txt";
    FILE* file = fopen(filename.c_str(), "r");
    if (file == NULL) {
        cout << "Belum ada data stok untuk cabang ini." << endl;
        system("pause");
        return;
    }

    cout << "\n+------+----------------------+----------+------------+\n";
    cout << "| No.  | Nama Barang          | Jumlah   | Harga      |\n";
    cout << "+------+----------------------+----------+------------+\n";

    char buffer[256];
    // Lewati header (baris pertama)
    fgets(buffer, sizeof(buffer), file);
    
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        stringstream ss(buffer);
        string nama, jumlah, harga;
        int i = 0;
        
        getline(ss, nama, ',');
        getline(ss, jumlah, ',');
        getline(ss, harga);
        
        // Bersihkan whitespace
        nama.erase(remove(nama.begin(), nama.end(), ' '), nama.end());
        jumlah.erase(remove(jumlah.begin(), jumlah.end(), ' '), jumlah.end());
        harga.erase(remove(harga.begin(), harga.end(), ' '), harga.end());
        
        cout << "| " << setw(4) << left << i++ << " | " 
                 << setw(20) << nama << " | " 
                 << setw(8) << jumlah << " | " 
                 << "Rp. " << setw(8) << fixed << setprecision(2) << harga << " |\n";
    }
    
    cout << "+------+----------------------+----------+------------+\n";
    fclose(file);
    system("pause");
}

void cariStok(cabang *temp) {
    system("cls");

    cout << "\n===================================================\n";
    cout << "|| " << setw(45) << left << "CARI STOK BARANG" << "||\n";
    cout << "===================================================\n";
    string filename = temp->namaCabang + "_stok.txt";
    FILE* file = fopen(filename.c_str(), "r");
    if (file == NULL) {
        cout << "Belum ada data stok untuk cabang ini." << endl;
        system("cls");
        return;
    }
    
    int mode;
    cout << "Pilih pencarian:\n";
    cout << "0. Berdasarkan nama\n1. Berdasarkan stok\n";
    cout << "Pilihan: "; 
    cin >> mode;
    cin.ignore();
    
    string keyword;
    cout << (mode ? "Jumlah stok: " : "Nama barang: ");
    getline(cin, keyword);
    
    bool ditemukan = false;
    cout << "\nHasil Pencarian:" << endl;
    cout << "+----------------------+----------+------------+\n";
    cout << "| Nama Barang          | Jumlah   | Harga      |\n";
    cout << "+----------------------+----------+------------+\n";

    char buffer[256];
    // Lewati header
    fgets(buffer, sizeof(buffer), file);
    
    while (fgets(buffer, sizeof(buffer), file)) {
        char nama[100];
        int jumlah;
        float harga;
        
        // Parsing data dari CSV
        if (sscanf(buffer, "%[^,],%d,%f", nama, &jumlah, &harga) != 3) {
            continue; // Skip baris yang tidak valid
        }
        
        bool match = false;
        if (mode == 0) {
            // Cari berdasarkan nama (case sensitive sederhana)
            match = (strstr(nama, keyword.c_str()) != NULL);
        } else {
            // Cari berdasarkan stok
            match = (jumlah == stoi(keyword));
        }
        
        if (match) {
            cout << "| " << setw(20) << left << nama << " | " 
                 << setw(8) << jumlah << " | " 
                 << "Rp. " << setw(8) << fixed << setprecision(2) << harga << " |\n";
            ditemukan = true;
        }
    }

    cout << "+----------------------+----------+------------+\n";
    
    if (!ditemukan) {
        cout << "Barang tidak ditemukan" << endl;
    }
    
    fclose(file);
    system("pause");
}

// ==================== MANAJEMEN KEUANGAN ====================

void keuanganCabangFile(cabang *temp){
    string filename = string(temp->namaCabang) + "_keuangan.txt";
    bool fileExists = (access(filename.c_str(), F_OK) != -1);
    FILE* file = fopen(filename.c_str(), "a");
    if (file == NULL) {
        cout << "Gagal membuka file untuk penyimpanan." << endl;
        return;
    }
    // Cek apakah file sudah ada
    if (!fileExists) {
        fprintf(file, "Kategori\t\tJumlah\n");
        fprintf(file, "-------------------------\n");
    }
    fprintf(file, "Pemasukan\t\t%.2f\n", temp->keuangan.pemasukan);
    fprintf(file, "Pengeluaran\t\t%.2f\n", temp->keuangan.pengeluaran);
    fprintf(file, "Saldo\t\t\t%.2f\n", temp->keuangan.sisa_saldo);
    fprintf(file, "-------------------------\n");
    fclose(file);
}

void buatLaporanKeuangan(cabang *temp){
    system("cls");

    cout << "\n===================================================\n";
    cout << "|| " << setw(45) << left << "BUAT LAPORAN KEUANGAN " << "||\n";
    cout << "===================================================\n";
    
    int jml_jual, jml_beli;
    float total_pemasukan = 0, total_pengeluaran = 0;
    char nama[50];
    int jumlah;
    float harga;

    printf("\n=== Input Pemasukan ===\n");
    printf("Jumlah jenis barang terjual hari ini: ");
    scanf("%d", &jml_jual); getchar();
    for (int i = 0; i < jml_jual; i++) {
        printf("Nama barang ke-%d: ", i+1);
        fgets(nama, 50, stdin); nama[strcspn(nama, "\n")] = 0;
        printf("Jumlah terjual: "); scanf("%d", &jumlah);
        printf("Harga satuan: "); scanf("%f", &harga); getchar();
        total_pemasukan += jumlah * harga;
        cout << endl;
    }
    
    printf("\n=== Input Pengeluaran ===\n");
    printf("Jumlah jenis barang masuk hari ini: ");
    scanf("%d", &jml_beli); getchar();
    for (int i = 0; i < jml_beli; i++) {
        printf("Nama barang ke-%d: ", i+1);
        fgets(nama, 50, stdin); nama[strcspn(nama, "\n")] = 0;
        printf("Jumlah dibeli: "); scanf("%d", &jumlah);
        printf("Harga satuan: "); scanf("%f", &harga); getchar();
        total_pengeluaran += jumlah * harga;

        cout << endl;
    }

    temp->keuangan.pemasukan += total_pemasukan;
    temp->keuangan.pengeluaran += total_pengeluaran;
    temp->keuangan.sisa_saldo = temp->keuangan.pemasukan - temp->keuangan.pengeluaran;

    keuanganCabangFile(temp);
    printf("\nLaporan keuangan berhasil dicatat!\n");
    system("pause");
}

void lihatKeuanganCabang(cabang *temp) {
    system("cls");

    cout << "\n===================================================\n";
    cout << "|| "  << left << "LAPORAN KEUANGAN " << temp->namaCabang << setw(45) << "||\n";
    cout << "===================================================\n";
    string filename = temp->namaCabang + "_keuangan.txt";
    FILE* file = fopen(filename.c_str(), "r");
    
    if (file == NULL) {
        cout << "Belum ada laporan keuangan tersedia" << endl;
        system("pause");
        return;
    }

    cout << "\n+----------------------+----------------+\n";
    cout << "| Kategori             | Jumlah         |\n";
    cout << "+----------------------+----------------+\n";
    
    char buffer[256];
    double saldo_awal = 0.0;
    double saldo_akhir = 0.0;
    bool first_saldo = true;
    int entry_count = 0;

    // Baca header dan garis pemisah pertama
    fgets(buffer, sizeof(buffer), file); // Kategori header
    fgets(buffer, sizeof(buffer), file); // Garis pemisah

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        string line(buffer);
        if (!line.empty() && line.back() == '\n') {
            line.pop_back();
        }

        // Cek jika ini garis pemisah
        if (line.find("---") != string::npos) {
            entry_count++;
            continue;
        }

        // Parse data keuangan
        size_t tab_pos = line.find('\t');
        if (tab_pos != string::npos) {
            string kategori = line.substr(0, tab_pos);
            string str_nilai = line.substr(tab_pos + 2); // Skip \t\t
            double nilai = stod(str_nilai);

            if (kategori == "Saldo") {
                if (first_saldo) {
                    saldo_awal = nilai;
                    first_saldo = false;
                }
                saldo_akhir = nilai;
            }

            cout << "| " << setw(20) << left << kategori << " | " 
                 << "Rp. " << setw(11) << fixed << setprecision(2) << nilai << " |\n";
        }
        cout << "+----------------------+----------------+\n";
    fclose(file);
    system("pause");
    }
    
    // Hitung dan tampilkan ringkasan
    cout << "-------------------------" << endl;
    cout << "SALDO AWAL\t\tRp. " << saldo_awal << endl;
    cout << "SALDO AKHIR\t\tRp. " << saldo_akhir << endl;
    cout << "TOTAL PERUBAHAN\t\tRp. " << (saldo_akhir - saldo_awal) << endl;
    cout << "-------------------------" << endl;

    fclose(file);
    system("pause");
}


// ==================== FUNGSI ALL ====================
void rekapStokKeFilePusat() {
    system("cls");

    cout << "\n===================================================\n";
    cout << "|| " << setw(45) << left << "REKAP STOK SEMUA CABANG" << "||\n";
    cout << "===================================================\n";
    struct TotalBarang {
        string namaBarang;
        int jumlah;
        double hargaRata; // Untuk rata-rata harga
        int countHarga;   // Menghitung berapa harga yang sudah diakumulasi
    };

    vector<TotalBarang> totalStok;

    cabang* temp = head;
    while (temp != NULL) {
        string filename = temp->namaCabang + "_stok.txt";
        FILE* file = fopen(filename.c_str(), "r");
        if (file == NULL) {
            cout << "[!] Gagal membuka file: " << filename << endl;
            temp = temp->next;
            continue;
        }

        char header[256];
        fgets(header, sizeof(header), file); // Skip header

        bool adaBarang = false;
        char nama[100];
        int jumlah;
        double harga;
        
        while (fscanf(file, " %99[^,], %d, %lf", nama, &jumlah, &harga) == 3) {
            adaBarang = true;
            string namaStr(nama);

            bool found = false;
            for (auto& item : totalStok) {
                if (item.namaBarang == namaStr) {
                    item.jumlah += jumlah;
                    item.hargaRata += harga;
                    item.countHarga++;
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                totalStok.push_back({namaStr, jumlah, harga, 1});
            }
        }

        if (!adaBarang) {
            cout << "[!] Tidak ada barang di file: " << filename << endl;
        }

        fclose(file);
        temp = temp->next;
    }

    if (totalStok.empty()) {
        cout << "[!] Tidak ada data stok dari semua cabang.\n";
        return;
    }

    FILE* pusatFile = fopen("pusat_stok.txt", "w");
    if (pusatFile == NULL) {
        cout << "[!] Gagal membuat file pusat_stok.txt\n";
        return;
    }

    fprintf(pusatFile, "Nama Barang,Total Jumlah,Harga Rata-rata\n");
    for (const auto& item : totalStok) {
        double avgPrice = item.hargaRata / item.countHarga;
        fprintf(pusatFile, "%s,%d,%.2lf\n", 
                item.namaBarang.c_str(), 
                item.jumlah, 
                avgPrice);
    }
    fclose(pusatFile);

    cout << "Rekap stok berhasil disimpan ke 'pusat_stok.txt'.\n";
    cout << "[i] Total barang unik: " << totalStok.size() << endl;

    system("pause");
}

void lihatStokSemuaCabang(){
    system("cls");

    cout << "\n===================================================\n";
    cout << "|| " << setw(45) << left << "STOK SEMUA CABANG" << "||\n";
    cout << "===================================================\n";
    cabang* temp = head;
    if (temp == NULL) {
        cout << "Belum ada cabang yang terdaftar." << endl;
        return;
    }
    int mode;
    cout << "Pilih sorting:\n";
    cout << "0. Nama A-Z\n1. Nama Z-A\n";
    cout << "2. Stok kecil-besar\n3. Stok besar-kecil\n";
    cout << "Pilihan: "; 
    cin >> mode;
    cin.ignore();

    while (temp != NULL) {
        string filename = temp->namaCabang + "_stok.txt";
        FILE* file = fopen(filename.c_str(), "r"); 
        if (file == NULL) {
            cout << "Gagal membuka file stok cabang: " << temp->namaCabang << endl;
            temp = temp->next;
            continue;
        }
        cout << "\n=== Stok Cabang " << temp->namaCabang << " ===" << endl;
         // Baca data dari file ke array sementara
        stokBarang tempStok[1000];
        int jumlahBarang = 0;
        char buffer[256];
        
        // Lewati header
        fgets(buffer, sizeof(buffer), file);
         while (fgets(buffer, sizeof(buffer), file) && jumlahBarang < 1000) {
            char nama[100];
            int stok;
            double harga;
            
            if (sscanf(buffer, "%[^,],%d,%lf", nama, &stok, &harga) == 3) {
                tempStok[jumlahBarang].namaBarang = nama;
                tempStok[jumlahBarang].jml_barang = stok;
                tempStok[jumlahBarang].harga = harga;
                jumlahBarang++;
            }
        }
        fclose(file);
         if (jumlahBarang == 0) {
            cout << "Belum ada barang" << endl;
        } else {
            // Sorting data
            shellSort(tempStok, jumlahBarang, mode);
            
            cout << "\n+----------------------+----------+------------+\n";
                cout << "| Nama Barang          | Jumlah   | Harga      |\n";
                cout << "+----------------------+----------+------------+\n";
            // Tampilkan hasil
            for (int i = 0; i < jumlahBarang; i++) {
                cout << "| " << setw(20) << left << tempStok[i].namaBarang << " | " 
                         << setw(8) << tempStok[i].jml_barang << " | " 
                         << "Rp. " << setw(8) << fixed << setprecision(2) << tempStok[i].harga << " |\n";
            }
            cout << "+----------------------+----------+------------+\n";
        }
        
        temp = temp->next;
    }
    system("pause");
}

void cariBarangSemuaCabang() {
    string keyword;
    int mode;
    
    cout << "Pilih pencarian:\n";
    cout << "0. Berdasarkan nama\n1. Berdasarkan stok\n";
    cout << "Pilihan: "; 
    cin >> mode;
    cin.ignore();
    
    cout << (mode ? "Jumlah stok: " : "Nama barang: ");
    getline(cin, keyword);

    cabang* temp = head;
    bool found = false;
    
    cout << "\n=== Hasil Pencarian ===" << endl;
    while (temp != NULL) {
        int index = searchStok(temp->stok, temp->jml_stok, keyword, mode);
        if (index != -1) {
            cout << "Cabang: " << temp->namaCabang << endl;
            cout << "Nama: " << temp->stok[index].namaBarang << endl;
            cout << "Stok: " << temp->stok[index].jml_barang << endl;
            cout << "Harga: " << temp->stok[index].harga << endl;
            cout << "-------------------" << endl;
            found = true;
        }
        temp = temp->next;
    }

    if (!found) {
        cout << "Barang tidak ditemukan di semua cabang." << endl;
    }
}

void rekapKeuanganKeFilePusat() {
    system("cls");

    cout << "\n===================================================\n";
    cout << "|| " << setw(45) << left << "REKAP KEUANGAN SEMUA CABANG" << "||\n";
    cout << "===================================================\n";

    double totalPemasukan = 0;
    double totalPengeluaran = 0;
    double totalSaldo = 0;

    cabang* temp = head;
    while (temp != NULL) {
        string filename = temp->namaCabang + "_keuangan.txt";
        FILE* file = fopen(filename.c_str(), "r");
        if (file == NULL) {
            cout << "[!] Gagal membuka file: " << filename << endl;
            temp = temp->next;
            continue;
        }

        char buffer[256];
        while (fgets(buffer, sizeof(buffer), file)) {
            string line(buffer);
            
            if (line.find("Kategori") != string::npos || 
                line.find("---") != string::npos ||
                line.empty()) {
                continue;
            }

            char kategori[50];
            double nilai;
            if (sscanf(line.c_str(), "%49s %lf", kategori, &nilai) == 2) {
                if (strcmp(kategori, "Pemasukan") == 0) {
                    totalPemasukan += nilai;
                } else if (strcmp(kategori, "Pengeluaran") == 0) {
                    totalPengeluaran += nilai;
                } else if (strcmp(kategori, "Saldo") == 0) {
                    totalSaldo += nilai;
                }
            }
        }
        fclose(file);
        temp = temp->next;
    }

    // Simpan ke file pusat dengan format yang diinginkan
    FILE* pusatFile = fopen("pusat_keuangan.txt", "w");
    if (pusatFile == NULL) {
        cout << "[!] Gagal membuat file rekap" << endl;
        return;
    }

    fprintf(pusatFile, "=== REKAP KEUANGAN SEMUA CABANG ===\n\n");
    fprintf(pusatFile, "%-15s: %12.2f\n", "Total Pemasukan", totalPemasukan);
    fprintf(pusatFile, "%-15s: %12.2f\n", "Total Pengeluaran", totalPengeluaran);
    fprintf(pusatFile, "%-15s: %12.2f\n", "Total Saldo", totalSaldo);
    fprintf(pusatFile, "%-15s: %12.2f\n", "Bersih", totalPemasukan - totalPengeluaran);
    fclose(pusatFile);

    // Tampilkan hasil di console
    cout << "\n=== HASIL REKAP FINAL ===\n";
    cout << "---------------------------------\n";
    cout << left << setw(15) << "Total Pemasukan" << ": " << setw(12) << right << fixed << setprecision(2) << totalPemasukan << endl;
    cout << left << setw(15) << "Total Pengeluaran" << ": " << setw(12) << right << totalPengeluaran << endl;
    cout << left << setw(15) << "Total Saldo" << ": " << setw(12) << right << totalSaldo << endl;
    cout << left << setw(15) << "Bersih" << ": " << setw(12) << right << (totalPemasukan - totalPengeluaran) << endl;
    cout << "---------------------------------\n";
    
    cout << "\nRekap keuangan berhasil disimpan ke 'pusat_keuangan.txt'.\n";

    system("pause");
}

void lihatLaporanKeuanganSemuaCabang() {
    system("cls");

    string filename = temp->namaCabang + "_keuangan.txt";
    FILE* file = fopen(filename.c_str(), "r");
    
    if (file == NULL) {
        cout << "Belum ada laporan keuangan tersedia" << endl;
        return;
    }

    cout << "\n=== LAPORAN KEUANGAN CABANG " << temp->namaCabang << " ===\n";
    
    char buffer[256];
    double saldo_awal = 0.0;
    double saldo_akhir = 0.0;
    bool first_saldo = true;
    int entry_count = 0;

    // Baca header dan garis pemisah pertama
    fgets(buffer, sizeof(buffer), file); // Kategori header
    fgets(buffer, sizeof(buffer), file); // Garis pemisah

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        string line(buffer);
        if (!line.empty() && line.back() == '\n') {
            line.pop_back();
        }

        // Cek jika ini garis pemisah
        if (line.find("---") != string::npos) {
            entry_count++;
            continue;
        }

        // Parse data keuangan
        size_t tab_pos = line.find('\t');
        if (tab_pos != string::npos) {
            string kategori = line.substr(0, tab_pos);
            string str_nilai = line.substr(tab_pos + 2); // Skip \t\t
            double nilai = stod(str_nilai);

            if (kategori == "Saldo") {
                if (first_saldo) {
                    saldo_awal = nilai;
                    first_saldo = false;
                }
                saldo_akhir = nilai;
            }

            cout << left << setw(12) << kategori << "Rp. " << fixed << setprecision(2) << nilai << endl;
        }
    }

    // Hitung dan tampilkan ringkasan
    cout << "-------------------------" << endl;
    cout << "SALDO AWAL\t\tRp. " << saldo_awal << endl;
    cout << "SALDO AKHIR\t\tRp. " << saldo_akhir << endl;
    cout << "TOTAL PERUBAHAN\t\tRp. " << (saldo_akhir - saldo_awal) << endl;
    cout << "-------------------------" << endl;

    fclose(file);
    system("pause");
}

// ==================== FUNGSI MENU ====================

void menuPusat() {
    int menu;
    do{
        system("cls");
        cout << "\n===================================================\n";
    cout << "|| " << setw(45) << left << "MENU ADMIN PUSAT" << "||\n";
    cout << "===================================================\n";
        cout << "Menu" << endl;
        cout << "1. Lihat Daftar Cabang" << endl;
        cout << "2. Tambah Cabang" << endl;
        cout << "3. Rekap Stok Barang dari Cabang" << endl;
        cout << "4. Lihat stok barang" << endl;
        cout << "5. Cari Stok" << endl;
        cout << "6. Rekap Keuangan dari Cabang" << endl;
        cout << "7. Lihat Laporan Keuangan" << endl;
        cout << "8. Hapus Cabang" << endl;
        cout << "0. exit" << endl;
        cout << "pilih menu: "; cin >> menu;
        cin.ignore();
        switch(menu)
        {
        case 1:
            lihatCabang();
            break;
        case 2:
            tambahCabang();
            break;
        case 3:
            rekapStokKeFilePusat();
            break;
        case 4:
            lihatStokSemuaCabang();
            break;
        case 5:
            cariBarangSemuaCabang();
            break;
        case 6:
            rekapKeuanganKeFilePusat();
            break;
        case 7:
            lihatLaporanKeuanganSemuaCabang();
            break;
        case 8: {
            system("cls");

            cout << "\n===================================================\n";
            cout << "|| " << setw(45) << left << "HAPUS CABANG" << "||\n";
            cout << "===================================================\n";
            lihatCabang();
            string namaCabang;
            cout << "Masukkan nama cabang yang ingin dihapus: ";
            getline(cin, namaCabang);
            hapusCabang(namaCabang);
            lihatCabang(); // untuk memastikan hasil
            break;
        }
        case 0:
        exit(0);
        break;
        default:
            cout << "Pilihan Tidak Valid!!";
            break;
        }
    } while(menu != 0);
}

void cabangFunction(cabang *temp){
    int pilih;
    do
    {
        cout << "Selamat Datang admin " << temp->namaCabang << endl;
        cout << "\n===================================================\n";
    cout << "|| " << setw(45) << left << "MENU ADMIN CABANG" << "||\n";
    cout << "===================================================\n";
        cout << "1. Lihat Stok Produk" << endl;
        cout << "2. Buat Laporan Keuangan" << endl;
        cout << "3. Tambah Stok" << endl;
        cout << "4. Cari Stok" << endl;
        cout << "5. Lihat Laporan Keuangan" << endl;
        cout << "0. Keluar" << endl;
        cout << "pilih menu: "; cin >> pilih;
        cin.ignore(); 
        switch (pilih)
        {
        case 1:
            lihatStockFile(temp);
            break;
        case 2:
            buatLaporanKeuangan(temp);
            break;
        case 3:
            tambahStok(temp);
            break;
        case 4:
            cariStok(temp);
            break;
        case 5:
            lihatKeuanganCabang(temp);
            break;
        case 0:
            exit(0);
            break;
        default:
            cout << "Pilihan Tidak Valid!" << endl;
            break;
        }
        system("pause");
    } while (pilih != 0);
    
}

cabang* loginCabang(const string& username, const string& password) {
    cabang* temp = head;
    while (temp != NULL) {
        if (temp->username == username && temp->password == password) {
            cout << "Login berhasil, selamat datang di " << temp->namaCabang << endl;
            return temp;
        }
        temp = temp->next;
    }
    cout << "Login gagal. Username atau password salah." << endl;
    return NULL;
}

void loginFunction() {
    cout << "\n===================================================\n";
    cout << "|| " << setw(45) << left << "LOGIN SISTEM MANAJEMEN CABANG" << "||\n";
    cout << "===================================================\n";
    string username, password;
    cout << "Username: "; 
    getline(cin, username);
    cout << "Password: "; 
    getline(cin, password);

    if (username == "adminPusat" && password == "adminPusat") {
        menuPusat();
    } else {
        cabang* cabangLogin = loginCabang(username, password);
        if (cabangLogin != NULL) {
            cabangFunction(cabangLogin);
        }
    }
}

int main(){
    readFileAkun(&head);
    
    while (true) {
        loginFunction();
        saveAllCabang(head);
    }
    
    return 0;
}