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
    // system("cls");
    cout << "==========================\n";
    cout << "||  TAMBAH CABANG BARU  ||\n";
    cout << "==========================\n";

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
    // system("pause");
}

// fungsi menampilkan cabang yang ada
void lihatCabang(){
    // system("cls");

    cout << "=====================\n";
    cout << "||  DAFTAR CABANG  ||\n";
    cout << "=====================\n";

    if(head == NULL){
        cout << "belum ada cabang yang ditambahkan" << endl;
        // system("pause");
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
    // system("pause");
}

// fungsi hapus cabang
void hapusCabang(string namaCabang) {
    if (head == NULL) {
        cout << "Belum ada cabang." << endl;
        // system("pause");
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
    // system("pause");
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
    // system("cls");
    cout << "=====================================\n";
    cout << "||       TAMBAH STOK BARANG        ||\n";
    cout << "=====================================\n";

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
    // system("pause");
}

void lihatStockFile(cabang *temp) {
    // system("cls");

    cout << "\n===============================================\n";
    cout << "||       LAPORAN STOK " << temp->namaCabang << "         ||\n";
    cout << "===============================================\n";
    string filename = temp->namaCabang + "_stok.txt";
    FILE* file = fopen(filename.c_str(), "r");
    if (file == NULL) {
        cout << "Belum ada data stok untuk cabang ini." << endl;
        // system("pause");
        return;
    }

    cout << "\n+------+----------------------+----------+------------+\n";
    cout << "| No.  | Nama Barang          | Jumlah   | Harga      |\n";
    cout << "+------+----------------------+----------+------------+\n";

    char buffer[256];
    // Lewati header (baris pertama)
    fgets(buffer, sizeof(buffer), file);
    int i = 1;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        stringstream ss(buffer);
        string nama, jumlah, harga;
        
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
                 << "Rp " << setw(8) << fixed << setprecision(2) << harga << "|\n";
    }
    
    cout << "+------+----------------------+----------+------------+\n";
    fclose(file);
    // system("pause");
}

void cariStok(cabang *temp) {
    // system("cls");
    cout << "========================\n";
    cout << "||  CARI STOK BARANG  ||\n";
    cout << "========================\n";
    string filename = temp->namaCabang + "_stok.txt";
    FILE* file = fopen(filename.c_str(), "r");
    if (file == NULL) {
        cout << "Belum ada data stok untuk cabang ini." << endl;
        // system("cls");
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
                 << "Rp." << setw(8) << fixed << setprecision(2) << harga << "|\n";
            ditemukan = true;
        }
    }

    cout << "+----------------------+----------+------------+\n";
    
    if (!ditemukan) {
        cout << "Barang tidak ditemukan" << endl;
    }
    
    fclose(file);
    // system("pause");
}

// ==================== MANAJEMEN KEUANGAN ====================

void keuanganCabangFile(cabang *temp) {
    string filename = temp->namaCabang + "_keuangan.txt";
    bool fileExists = (access(filename.c_str(), F_OK) != -1);
    
    FILE* file = fopen(filename.c_str(), "a");
    if (file == NULL) {
        cout << "[ERROR] Gagal membuka file: " << filename << endl;
        return;
    }

    if (!fileExists) {
        fprintf(file, "Kategori    Jumlah\n");
        fprintf(file, "-------------------------\n");
    }

    fprintf(file, "Pemasukan    %.2f\n", temp->keuangan.pemasukan);
    fprintf(file, "Pengeluaran    %.2f\n", temp->keuangan.pengeluaran);
    fprintf(file, "Saldo    %.2f\n", temp->keuangan.sisa_saldo);
    fprintf(file, "-------------------------\n");
    
    fclose(file);
}

void buatLaporanKeuangan(cabang *temp) {
    // system("cls");
    
    cout << "=======================================\n";
    cout << "||       BUAT LAPORAN KEUANGAN       ||\n";
    cout << "=======================================\n";
    
    // Input pemasukan
    cout << "\n=== INPUT PEMASUKAN ===\n";
    int jml_jual;
    cout << "Jumlah jenis barang terjual hari ini: ";
    cin >> jml_jual; 
    cin.ignore();
    
    for (int i = 0; i < jml_jual; i++) {
        char nama[50];
        int jumlah;
        float harga;
        
        cout << "\nBarang ke-" << (i+1) << endl;
        cout << "Nama barang  : "; 
        fgets(nama, 50, stdin); 
        nama[strcspn(nama, "\n")] = 0;
        
        cout << "Jumlah terjual: "; 
        cin >> jumlah;
        
        cout << "Harga satuan  : "; 
        cin >> harga; 
        cin.ignore();
        
        temp->keuangan.pemasukan += jumlah * harga;
    }
    
    // Input pengeluaran
    cout << "\n=== INPUT PENGELUARAN ===\n";
    int jml_beli;
    cout << "Jumlah jenis barang masuk hari ini: ";
    cin >> jml_beli; 
    cin.ignore();
    
    for (int i = 0; i < jml_beli; i++) {
        char nama[50];
        int jumlah;
        float harga;
        
        cout << "\nBarang ke-" << (i+1) << endl;
        cout << "Nama barang  : "; 
        fgets(nama, 50, stdin); 
        nama[strcspn(nama, "\n")] = 0;
        
        cout << "Jumlah dibeli : "; 
        cin >> jumlah;
        
        cout << "Harga satuan  : "; 
        cin >> harga; 
        cin.ignore();
        
        temp->keuangan.pengeluaran += jumlah * harga;
    }
    
    // Update saldo
    temp->keuangan.sisa_saldo = temp->keuangan.pemasukan - temp->keuangan.pengeluaran;
    
    // Simpan ke file
    keuanganCabangFile(temp);
    
    cout << "\nLaporan keuangan berhasil dicatat!\n";
    // system("pause");
}

void lihatKeuanganCabang(cabang *temp) {
    // system("cls");
    cout << "==================================\n";
    cout << "||       LAPORAN KEUANGAN       ||\n";
    cout << "==================================\n";
    
    string filename = temp->namaCabang + "_keuangan.txt";
    FILE* file = fopen(filename.c_str(), "r");
    
    if (file == NULL) {
        cout << "\nBelum ada laporan keuangan tersedia\n";
        // system("pause");
        return;
    }
    
    // Header
    cout << "\nKategori          Jumlah (Rp)\n";
    cout << "------------------------------\n";
    
    char buffer[256];
    double saldo_awal = 0, saldo_akhir = 0;
    bool first_saldo = true;

    // Lewati header file
    fgets(buffer, sizeof(buffer), file); // Kategori header
    fgets(buffer, sizeof(buffer), file); // Garis pemisah
    
    while (fgets(buffer, sizeof(buffer), file)) {
        // Hapus newline
        buffer[strcspn(buffer, "\n")] = 0;
        
        // Skip garis pemisah
        if (strstr(buffer, "---") != NULL) continue;
        
        // Parse baris
        char kategori[50];
        double nilai;
        
        if (sscanf(buffer, "%49[^0-9] %lf", kategori, &nilai) == 2) {
            // Bersihkan spasi di kategori
            char *p = kategori;
            while (*p == ' ' || *p == '\t') p++;
            char *end = kategori + strlen(kategori) - 1;
            while (end > p && (*end == ' ' || *end == '\t')) end--;
            *(end+1) = '\0';
            
            printf("%-15s    %12.2f\n", p, nilai);
            
            if (strcmp(p, "Saldo") == 0) {
                if (first_saldo) {
                    saldo_awal = nilai;
                    first_saldo = false;
                }
                saldo_akhir = nilai;
            }
        }
    }
    fclose(file);
    
    // Tampilkan ringkasan
    cout << "------------------------------\n";
    printf("%-15s    %12.2f\n", "Saldo Awal", saldo_awal);
    printf("%-15s    %12.2f\n", "Saldo Akhir", saldo_akhir);
    printf("%-15s    %12.2f\n", "Total Saldo", (saldo_akhir + saldo_awal));
    
    // system("pause");
}

// ==================== FUNGSI ALL ====================
void rekapStokKeFilePusat() {
    // system("cls");
    cout << "===============================\n";
    cout << "||  REKAP STOK SEMUA CABANG  ||\n";
    cout << "===============================\n";

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

    // system("pause");
}

void lihatStokSemuaCabang(){
    // system("cls");
    cout << "===================================\n";
    cout << "||      STOK SEMUA CABANG        ||\n";
    cout << "===================================\n";
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
                         << "Rp." << setw(8) << fixed << setprecision(2) << tempStok[i].harga << "|\n";
            }
            cout << "+----------------------+----------+------------+\n";
        }
        
        temp = temp->next;
    }
    // system("pause");
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
    cout << "-----------------------" << endl;
    
    while (temp != NULL) {
        // Baca dari file stok cabang
        string filename = temp->namaCabang + "_stok.txt";
        FILE* file = fopen(filename.c_str(), "r");
        
        if (file != NULL) {
            char buffer[256];
            // Skip header
            fgets(buffer, sizeof(buffer), file);
            
            while (fgets(buffer, sizeof(buffer), file)) {
                char namaBarang[100];
                int stok;
                double harga;
                
                // Parse format: NamaBarang,Stok,Harga
                if (sscanf(buffer, "%99[^,],%d,%lf", namaBarang, &stok, &harga) == 3) {
                    bool match = false;
                    
                    if (mode == 0) { // Cari berdasarkan nama
                        string namaStr(namaBarang);
                        // Case insensitive search
                        string namaLower = namaStr;
                        transform(namaLower.begin(), namaLower.end(), namaLower.begin(), ::tolower);
                        string keywordLower = keyword;
                        transform(keywordLower.begin(), keywordLower.end(), keywordLower.begin(), ::tolower);
                        
                        match = (namaLower.find(keywordLower) != string::npos);
                    } else { // Cari berdasarkan stok
                        int stokCari;
                        try {
                            stokCari = stoi(keyword);
                            match = (stok == stokCari);
                        } catch (...) {
                            cout << "[Warning] Input stok tidak valid\n";
                            fclose(file);
                            return;
                        }
                    }
                    
                    if (match) {
                        cout << "Cabang: " << temp->namaCabang << endl;
                        cout << "Nama: " << namaBarang << endl;
                        cout << "Stok: " << stok << endl;
                        cout << "Harga: Rp. " << fixed << setprecision(2) << harga << endl;
                        cout << "-------------------" << endl;
                        found = true;
                    }
                }
            }
            fclose(file);
        } else {
            cout << "[Warning] Tidak bisa baca stok cabang " << temp->namaCabang << endl;
        }
        
        temp = temp->next;
    }

    if (!found) {
        cout << "Barang tidak ditemukan di semua cabang." << endl;
    }
}

void rekapKeuanganKeFilePusat() {
    // system("cls");
    cout << "===================================\n";
    cout << "||  REKAP KEUANGAN SEMUA CABANG  ||\n";
    cout << "===================================\n";

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
    cout << "---------------------------------\n";
    cout << left << setw(15) << "Total Pemasukan" << "  : " << setw(12) << right << fixed << setprecision(2) << totalPemasukan << endl;
    cout << left << setw(15) << "Total Pengeluaran" << ": " << setw(12) << right << totalPengeluaran << endl;
    cout << left << setw(15) << "Total Saldo" << "  : " << setw(12) << right << totalSaldo << endl;
    cout << left << setw(15) << "Bersih" << "  : " << setw(12) << right << (totalPemasukan - totalPengeluaran) << endl;
    cout << "---------------------------------\n";
    
    cout << "\nRekap keuangan berhasil disimpan ke 'pusat_keuangan.txt'.\n";

    // system("pause");
}

void lihatLaporanKeuanganSemuaCabang() {
    cout << "===========================================\n";
    cout << "||  LIHAT LAPORAN KEUANGAN SEMUA CABANG  ||\n";
    cout << "===========================================\n";
    
    cabang *temp = head;
    if (temp == NULL) {
        cout << "Belum ada cabang yang terdaftar." << endl;
        return;
    }

    double total_keseluruhan = 0;
    bool ada_laporan = false;

    while (temp != NULL) {
        string filename = temp->namaCabang + "_keuangan.txt";
        FILE* file = fopen(filename.c_str(), "r");
        
        if (file == NULL) {
            cout << "\n[Laporan untuk cabang " << temp->namaCabang << "] Belum tersedia.\n";
            temp = temp->next;
            continue;
        }

        ada_laporan = true;
        cout << "\n=== LAPORAN KEUANGAN CABANG " << temp->namaCabang << " ===\n";
        cout << "+----------------------+----------------+\n";
        cout << "| Kategori             | Jumlah (Rp)    |\n";
        cout << "+----------------------+----------------+\n";

        char buffer[256];
        double pemasukan = 0, pengeluaran = 0, saldo_awal = 0, saldo_akhir = 0;
        bool first_saldo = true;

        while (fgets(buffer, sizeof(buffer), file)) {
            string line(buffer);
            line.erase(line.find_last_not_of("\n") + 1);

            if (line.empty() || line.find("---") != string::npos || line.find("Kategori") != string::npos) {
                continue;
            }

            size_t space_pos = line.find_last_of(" \t");
            if (space_pos != string::npos) {
                string kategori = line.substr(0, space_pos);
                string str_nilai = line.substr(space_pos + 1);

                // Clean up category name
                kategori.erase(0, kategori.find_first_not_of(" \t"));
                kategori.erase(kategori.find_last_not_of(" \t") + 1);

                try {
                    double nilai = stod(str_nilai);
                    
                    // Format output
                    printf("| %-20s | %14.2f |\n", kategori.c_str(), nilai);

                    if (kategori == "Pemasukan") {
                        pemasukan += nilai;
                    } else if (kategori == "Pengeluaran") {
                        pengeluaran += nilai;
                    } else if (kategori == "Saldo") {
                        if (first_saldo) {
                            saldo_awal = nilai;
                            first_saldo = false;
                        }
                        saldo_akhir = nilai;
                    }
                } catch (...) {
                    cout << "| [Format Error]           |                |\n";
                }
            }
        }
        fclose(file);

        double saldo_bersih = pemasukan - pengeluaran;
        total_keseluruhan += saldo_bersih;

        cout << "+----------------------+----------------+\n";
        printf("| %-20s | %14.2f |\n", "Total Pemasukan", pemasukan);
        printf("| %-20s | %14.2f |\n", "Total Pengeluaran", pengeluaran);
        printf("| %-20s | %14.2f |\n", "Saldo Awal", saldo_awal);
        printf("| %-20s | %14.2f |\n", "Saldo Akhir", saldo_akhir);
        printf("| %-20s | %14.2f |\n", "Saldo Bersih", saldo_bersih);
        cout << "+----------------------+----------------+\n";

        temp = temp->next;
    }

    if (ada_laporan) {
        cout << "\n=== REKAP KESELURUHAN ===\n";
        cout << "+----------------------+----------------+\n";
        printf("| %-20s | %14.2f |\n", "TOTAL SALDO", total_keseluruhan);
        cout << "+----------------------+----------------+\n";
    } else {
        cout << "\nTidak ada laporan keuangan yang tersedia.\n";
    }
}
// ==================== FUNGSI MENU ====================

void menuPusat() {
    int menu;
    do{
        system("cls");
        cout << "=====================================\n";
        cout << "||         MENU ADMIN PUSAT        ||\n";
        cout << "=====================================\n";
        cout << "|| 1. Lihat Daftar Cabang          ||" << endl;
        cout << "|| 2. Tambah Cabang                ||" << endl;
        cout << "|| 3. Rekap Stok Barang dari Cabang||" << endl;
        cout << "|| 4. Lihat stok barang            ||" << endl;
        cout << "|| 5. Cari Stok                    ||" << endl;
        cout << "|| 6. Rekap Keuangan dari Cabang   ||" << endl;
        cout << "|| 7. Lihat Laporan Keuangan       ||" << endl;
        cout << "|| 8. Hapus Cabang                 ||" << endl;
        cout << "|| 0. exit                         ||" << endl;
        cout << "=====================================\n";
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
            // system("cls");

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
        cout << "Press any key to continue...\n";
        cin.get();
    } while(menu != 0);
}

void cabangFunction(cabang *temp){
    int pilih;
    do
    { system("cls");
        cout << "Selamat Datang admin " << temp->namaCabang << endl;
        cout << "===================================\n";
        cout << "||       MENU ADMIN CABANG       ||\n";
        cout << "===================================\n";
        cout << "|| 1. Lihat Stok Produk          ||" << endl;
        cout << "|| 2. Buat Laporan Keuangan      ||" << endl;
        cout << "|| 3. Tambah Stok                ||" << endl;
        cout << "|| 4. Cari Stok                  ||" << endl;
        cout << "|| 5. Lihat Laporan Keuangan     ||" << endl;
        cout << "|| 0. Keluar                     ||" << endl;
        cout << "===================================\n";
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
        cout << "Press any key to continue...\n";
        cin.get();
    } while (pilih != 0);
    
}

cabang* loginCabang(const string& username, const string& password) {
    cabang* temp = head;
    while (temp != NULL) {
        if (temp->username == username && temp->password == password) {
            // cout << "Login berhasil, selamat datang di " << temp->namaCabang << endl;
            return temp;
        }
        temp = temp->next;
    }
    cout << "Login gagal. Username atau password salah." << endl;
    return NULL;
}

void loginFunction() {
    system("cls");
    cout << "====================================\n";
    cout << "|| LOGIN SISTEM MANAJEMEN CABANG  ||\n";
    cout << "====================================\n";
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