#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <cstring>
#include <stdlib.h>
#include <limits>

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
        cout << "\n=== Stok Cabang " << temp->namaCabang << " ===" << endl;
        
        if (temp->jml_stok == 0) {
            cout << "Belum ada barang" << endl;
        } else {
            // Buat salinan untuk di-sort
            stokBarang tempStok[1000];
            for (int i = 0; i < temp->jml_stok; i++) {
                tempStok[i] = temp->stok[i];
            }
            
            shellSort(tempStok, temp->jml_stok, mode);
            
            // Tampilkan hasil
            for (int i = 0; i < temp->jml_stok; i++) {
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

void lihatLaporanKeuanganSemuaCabang() {
    cabang* temp = head;
    if (temp == NULL) {
        cout << "Belum ada cabang yang terdaftar." << endl;
        return;
    }

    float totalSaldo = 0;
    cout << "\n=== Laporan Keuangan Semua Cabang ===" << endl;
    while (temp != NULL) {
        cout << "Cabang: " << temp->namaCabang << endl;
        cout << "Pemasukan: " << temp->keuangan.pemasukan << endl;
        cout << "Pengeluaran: " << temp->keuangan.pengeluaran << endl;
        cout << "Saldo: " << temp->keuangan.sisa_saldo << endl;
        cout << "-------------------------" << endl;
        totalSaldo += temp->keuangan.sisa_saldo;
        temp = temp->next;
    }
    cout << "TOTAL SALDO SEMUA CABANG: " << totalSaldo << endl;
}

void stokCabangFile(cabang *temp){
    string filename = temp->namaCabang + "_stok.txt";
    FILE* file = fopen(filename.c_str(), "a");
    if (file == NULL) {
        cout << "Gagal membuka file untuk penyimpanan." << endl;
        return;
    }
    fprintf(file, "Nama Barang,Jumlah,Harga\n");  // Header CSV
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

void lihatStockFile(cabang *temp){
    string filename = temp->namaCabang + "_stok.txt";
    FILE* file = fopen(filename.c_str(), "r");
    if (file == NULL) {
        cout << "Gagal membuka file untuk pembacaan." << endl;
        return;
    }
    char buffer[256];
    cout << "=== Stok Cabang " << temp->namaCabang << " ===" << endl;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        cout << buffer;
    }
    fclose(file);
}

void lihatStokCabang(cabang *temp){
    if(temp->jml_stok == 0){
        cout << "belum ada stok" << endl;
        return;
    }
    lihatStockFile(temp);
}

void cariStok(cabang *temp){
    string keyword;
    int mode;
    
    cout << "Pilih pencarian:\n";
    cout << "0. Berdasarkan nama\n1. Berdasarkan stok\n";
    cout << "Pilihan: "; 
    cin >> mode;
    cin.ignore();
    
    cout << (mode ? "Jumlah stok: " : "Nama barang: ");
    getline(cin, keyword);
    
    int index = searchStok(temp->stok, temp->jml_stok, keyword, mode);
    if (index != -1) {
        cout << "\nHasil Pencarian:" << endl;
        cout << "Nama: " << temp->stok[index].namaBarang << endl;
        cout << "Stok: " << temp->stok[index].jml_barang << endl;
        cout << "Harga: " << temp->stok[index].harga << endl;
    } else {
        cout << "Barang tidak ditemukan" << endl;
    }
}

void lihatLaporanKeuanganCabang(cabang *temp) {
    cout << "\n=== Laporan Keuangan Cabang " << temp->namaCabang << " ===" << endl;
    cout << "Pemasukan: " << temp->keuangan.pemasukan << endl;
    cout << "Pengeluaran: " << temp->keuangan.pengeluaran << endl;
    cout << "Saldo: " << temp->keuangan.sisa_saldo << endl;
}

void keuanganCabangFile(cabang *temp){
    string filename = string(temp->namaCabang) + "_keuangan.txt";
    FILE* file = fopen(filename.c_str(), "a");
    if (file == NULL) {
        cout << "Gagal membuka file untuk penyimpanan." << endl;
        return;
    }
    fprintf(file, "Pemasukan: %.2f\n", temp->keuangan.pemasukan);
    fprintf(file, "Pengeluaran: %.2f\n", temp->keuangan.pengeluaran);
    fprintf(file, "Saldo: %.2f\n\n", temp->keuangan.sisa_saldo);
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
        cout << "3. Lihat stok barang" << endl;
        cout << "4. Cari stok" << endl;
        cout << "5. Lihat keuangan" << endl;
        cout << "6. Hapus cabang" << endl;
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
            lihatStokSemuaCabang();
            break;
        case 4:
            cariBarangSemuaCabang();
            break;
        case 5:
            lihatLaporanKeuanganSemuaCabang();
            break;
        case 6: {
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
        cout << "menu" << endl;
        cout << "1. lihat stok produk" << endl;
        cout << "2. buat laporan keuangan" << endl;
        cout << "3. tambah stok" << endl;
        cout << "4. cari stok" << endl;
        cout << "5. Lihat Laporan Keuangan" << endl;
        cout << "0. keluar" << endl;
        cout << "pilih menu: "; cin >> pilih;
        switch (pilih)
        {
        case 1:
            lihatStokCabang(temp);
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
            lihatLaporanKeuanganCabang(temp);
            break;
        case 0:
            exit(0);
            break;
        default:
            cout << "Pilihan Tidak Valid!" << endl;
            break;
        }
    } while (pilih != 0);
    
}

cabang* loginCabang(const string& username, const string& password) {
    cabang* temp = head;
    while (temp != NULL) {
        if (temp->username == username && temp->password == password) {
            cout << "Login berhasil, selamat datang di cabang " << temp->namaCabang << endl;
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