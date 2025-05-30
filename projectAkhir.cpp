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


struct stokBarang
{
    string namaBarang;
    int jml_barang;
    float harga;
};

struct keuangan
{
    float pemasukan = 0, pengeluaran = 0, sisa_saldo = 0;
};

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
    cout << "Cabang berhasil ditambahkan";
    tambahCabangFile(new_cabang);
}

void lihatCabang(){
    int i = 1;
    cabang* temp = head;
    if(temp == NULL){
        cout << "belum ada cabang yang ditambahkan" << endl;
        return;
    }
    while (temp != NULL){
        cout << i++ << ". " << temp->namaCabang << endl;
        temp = temp->next;
    }
}

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

void hapusCabang(string namaCabang) {
    if (head == NULL) {
        cout << "Belum ada cabang." << endl;
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
}

void rekapStokKeFilePusat() {
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

    cout << "[✓] Rekap stok berhasil disimpan ke 'pusat_stok.txt'.\n";
    cout << "[i] Total barang unik: " << totalStok.size() << endl;
}



void lihatStokSemuaCabang(){
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
            
            // Tampilkan hasil
            for (int i = 0; i < jumlahBarang; i++) {
                cout << "Nama: " << tempStok[i].namaBarang << endl;
                cout << "Stok: " << tempStok[i].jml_barang << endl;
                cout << "Harga: " << tempStok[i].harga << endl;
                cout << "-------------------" << endl;
            }
        }
        
        temp = temp->next;
    }
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
    double totalPemasukan = 0;
    double totalPengeluaran = 0;
    double totalSaldo = 0;

    cabang* temp = head;
    while (temp != NULL) {
        string filename = temp->namaCabang + "_keuangan.txt";
        cout << "[DEBUG] Membaca file: " << filename << endl;
        
        FILE* file = fopen(filename.c_str(), "r");
        if (file == NULL) {
            cout << "[!] Gagal membuka file: " << filename << endl;
            temp = temp->next;
            continue;
        }

        char buffer[256];
        while (fgets(buffer, sizeof(buffer), file)) {
            string line(buffer);
            
            cout << "[DEBUG] Baris: " << line;

            // Skip baris yang tidak relevan
            if (line.find("Kategori") != string::npos || 
                line.find("---") != string::npos ||
                line.empty()) {
                continue;
            }

            // Parse format: "Kategori<spasi>Nilai"
            char kategori[50];
            double nilai;
            
            if (sscanf(line.c_str(), "%49s %lf", kategori, &nilai) == 2) {
                cout << "[DEBUG] Parsed: " << kategori << " = " << nilai << endl;

                if (strcmp(kategori, "Pemasukan") == 0) {
                    totalPemasukan += nilai;
                } else if (strcmp(kategori, "Pengeluaran") == 0) {
                    totalPengeluaran += nilai;
                } else if (strcmp(kategori, "Saldo") == 0) {
                    totalSaldo += nilai;
                }
            } else {
                cout << "[!] Gagal parse baris: " << line;
            }
        }
        fclose(file);
        temp = temp->next;
    }

    // Simpan ke file pusat
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

    // Output ke console 
    cout << "\n=== HASIL REKAP FINAL ===" << endl;
    cout << "---------------------------------" << endl;
    cout << left << setw(20) << "Total Pemasukan" << ": Rp. " << fixed << setprecision(2) << totalPemasukan << endl;
    cout << setw(20) << "Total Pengeluaran" << ": Rp. " << totalPengeluaran << endl;
    cout << setw(20) << "Total Saldo" << ": Rp. " << totalSaldo << endl;
    cout << setw(20) << "Saldo Bersih" << ": Rp. " << (totalPemasukan - totalPengeluaran) << endl;
    cout << "---------------------------------" << endl;
}

void lihatLaporanKeuanganSemuaCabang() {
    cabang* temp = head;
    if (temp == NULL) {
        printf("\nBelum ada cabang yang terdaftar.\n");
        return;
    }

    // Header tabel
    printf("\n=== LAPORAN KEUANGAN SEMUA CABANG ===\n");
    printf("+----------------------+----------------+----------------+----------------+\n");
    printf("| %-20s | %14s | %14s | %14s |\n", "CABANG", "PEMASUKAN", "PENGELUARAN", "SALDO");
    printf("+----------------------+----------------+----------------+----------------+\n");

    double totalPemasukan = 0, totalPengeluaran = 0, totalSaldo = 0;

    while (temp != NULL) {
        char filename[100];
        sprintf(filename, "%s_keuangan.txt", temp->namaCabang.c_str());
        
        FILE* file = fopen(filename, "r");
        double pemasukan = 0, pengeluaran = 0, saldo = 0;
        
        if (file != NULL) {
            char buffer[256];
            while (fgets(buffer, sizeof(buffer), file)) {
                char* line = buffer;
                
                // Remove newline character
                line[strcspn(line, "\n")] = 0;
                
                if (strstr(line, "Pemasukan")) {
                    sscanf(line, "Pemasukan %lf", &pemasukan);
                } 
                else if (strstr(line, "Pengeluaran")) {
                    sscanf(line, "Pengeluaran %lf", &pengeluaran);
                } 
                else if (strstr(line, "Saldo")) {
                    sscanf(line, "Saldo %lf", &saldo);
                }
            }
            fclose(file);
        }

        // Format output untuk setiap cabang
        printf("| %-20s | %14.2f | %14.2f | %14.2f |\n", 
               temp->namaCabang.c_str(), 
               pemasukan, 
               pengeluaran, 
               saldo);
        
        totalPemasukan += pemasukan;
        totalPengeluaran += pengeluaran;
        totalSaldo += saldo;
        
        temp = temp->next;
    }

    // Footer tabel dengan total
    printf("+----------------------+----------------+----------------+----------------+\n");
    printf("| %-20s | %14.2f | %14.2f | %14.2f |\n", 
           "TOTAL", 
           totalPemasukan, 
           totalPengeluaran, 
           totalSaldo);
    printf("+----------------------+----------------+----------------+----------------+\n");
}

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
    int jml;
    cout << "berapa barang yang ingin diinput: "; cin >> jml;

    for(int i=0; i<jml; i++){
        cout << "Barang ke-" << i+1 << endl;
        cout << "Nama barang: "; cin.ignore(); getline(cin, temp->stok[temp->jml_stok].namaBarang);
        cout << "Jumlah barang: "; cin >> temp->stok[temp->jml_stok].jml_barang;
        cout << "Harga barang: "; cin >> temp->stok[temp->jml_stok].harga;
        temp->jml_stok++;
    }

    cout << "Stok berhasil ditambahkan.\n";
    stokCabangFile(temp);
}

void lihatStockFile(cabang *temp) {
    string filename = temp->namaCabang + "_stok.txt";
    FILE* file = fopen(filename.c_str(), "r");
    if (file == NULL) {
        cout << "Gagal membuka file stok cabang." << endl;
        return;
    }

    cout << "\n=== STOK CABANG " << temp->namaCabang << " ===\n";
    cout << "---------------------------------\n";
    cout << left << setw(20) << "NAMA BARANG" 
         << setw(10) << "JUMLAH" 
         << setw(10) << "HARGA" << endl;
    cout << "---------------------------------\n";

    char buffer[256];
    // Lewati header (baris pertama)
    fgets(buffer, sizeof(buffer), file);
    
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
        
        cout << left << setw(20) << nama 
             << setw(10) << jumlah 
             << "Rp. " << harga << endl;
    }
    
    cout << "---------------------------------\n";
    fclose(file);
}

void cariStok(cabang *temp) {
    string filename = temp->namaCabang + "_stok.txt";
    FILE* file = fopen(filename.c_str(), "r");
    if (file == NULL) {
        cout << "File stok tidak ditemukan" << endl;
        return;
    }

    char buffer[256];
    // Lewati header
    fgets(buffer, sizeof(buffer), file);
    
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
            cout << "-----------------------------" << endl;
            cout << "Nama: " << nama << endl;
            cout << "Stok: " << jumlah << endl;
            cout << "Harga: Rp. " << fixed << setprecision(2) << harga << endl;
            ditemukan = true;
        }
    }
    
    if (!ditemukan) {
        cout << "Barang tidak ditemukan" << endl;
    }
    
    fclose(file);
}

void lihatKeuanganCabang(cabang *temp) {
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
}

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
    }

    temp->keuangan.pemasukan += total_pemasukan;
    temp->keuangan.pengeluaran += total_pengeluaran;
    temp->keuangan.sisa_saldo = temp->keuangan.pemasukan - temp->keuangan.pengeluaran;

    printf("\nLaporan keuangan berhasil dicatat.\n");
    keuanganCabangFile(temp);
}

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

void menuPusat() {
    int menu;
    do{
        cout << "Menu" << endl;
        cout << "1. Lihat daftar cabang" << endl;
        cout << "2. Tambah cabang" << endl;
        cout << "3. Load stok barang dari cabang" << endl;
        cout << "4. Lihat stok barang" << endl;
        cout << "5. Cari stok" << endl;
        cout << "6. Load keuangan dari cabang" << endl;
        cout << "7. Lihat laporan keuangan" << endl;
        cout << "8. Hapus cabang" << endl;
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
        system("cls");
        cout << "Selamat Datang admin " << temp->namaCabang << endl;
        cout << "menu" << endl;
        cout << "1. lihat stok produk" << endl;
        cout << "2. buat laporan keuangan" << endl;
        cout << "3. tambah stok" << endl;
        cout << "4. cari stok" << endl;
        cout << "5. Lihat Laporan Keuangan" << endl;
        cout << "0. keluar" << endl;
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
            cout << "Login berhasil, selamat datang di " << temp->namaCabang << endl;
            return temp;
        }
        temp = temp->next;
    }
    cout << "Login gagal. Username atau password salah." << endl;
    return NULL;
}

void loginFunction() {
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
    loginFunction();
    saveAllCabang(head);
}