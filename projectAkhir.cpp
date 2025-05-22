#include <iostream>
#include <stdio.h>
#include <string>
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
cabang* head = nullptr;

void shellSort(cabang *cabangPtr) {  
    int n = cabangPtr->jml_stok;
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            stokBarang current = cabangPtr->stok[i];  // Simpan data sementara
            int j;
            // Bandingkan string dengan operator > (C++)
            for (j = i; j >= gap && cabangPtr->stok[j - gap].jml_barang > current.jml_barang; j -= gap) {
                cabangPtr->stok[j] = cabangPtr->stok[j - gap];
            }
            cabangPtr->stok[j] = current;
        }
    }
}

// int sequentialSearch(stokBarang stok[], int jumlah, const char* cari) {
//     for (int i = 0; i < jumlah; i++) {
//         if (strcmp(stok[i].namaBarang, cari) == 0) {
//             return i;
//         }
//     }
//     return -1;
// }

void tambahCabangFile(cabang *head){
    FILE* file = fopen("akun.txt", "a");
    if (file == NULL) {
        cout << "Gagal membuka file untuk penyimpanan." << endl;
        return;
    }
    cabang* current = head;
    while (current != nullptr) {
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
    cout << "username: "; getline(cin, new_cabang->username);
    cout << "password: "; getline(cin, new_cabang->password);

    new_cabang->next = nullptr;
    if (head == nullptr)
    {
        head = new_cabang;
    } else {
        cabang* temp = head;
        while (temp->next != nullptr){
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
    if(temp == nullptr){
        cout << "belum ada cabang yang ditambahkan" << endl;
        return;
    }
    while (temp != nullptr){
        cout << i++ << ". " << temp->namaCabang << endl;
        temp = temp->next;
    }
}

void saveAllCabang(cabang* head);

void hapusCabang(string namaCabang) {
    if (head == nullptr) {
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
    while (bantu->next != nullptr && bantu->next->namaCabang != namaCabang) {
        bantu = bantu->next;
    }

    if (bantu->next == nullptr) {
        cout << "Cabang tidak ditemukan." << endl;
    } else {
        cabang* hapus = bantu->next;
        bantu->next = hapus->next;
        delete hapus;
        saveAllCabang(head); // Simpan perubahan ke file
        cout << "Cabang berhasil dihapus." << endl;
    }
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
            break;
        case 4:
            break;
        case 5:
            break;
        case 6: {
            string namaCabang;
            cout << "Masukkan nama cabang yang ingin dihapus: ";
            getline(cin, namaCabang);
            hapusCabang(namaCabang);
            lihatCabang(); // untuk memastikan hasil
            break;
        }
        default:
            break;
        }
    } while(menu != 0);
}

void lihatStok(cabang *temp){
    if(temp->jml_stok == 0){
        cout << "Belum ada barang yang diinput" << endl;
        return;
    }
    for(int i = 0; i < temp->jml_stok; i++){
        cout << "Nama Barang: " << temp->stok[i].namaBarang << endl;
        cout << "Harga barang: " << temp->stok[i].harga << endl;
        cout << "Jumlah barang: " << temp->stok[i].jml_barang << endl;
    }
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
        cout << "nama barang: "; cin.ignore(); getline(cin, temp->stok[temp->jml_stok].namaBarang);
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
void cariStok(cabang *temp){
    string namaBarang;
    cout << "Nama barang yang dicari: "; cin.ignore(); getline(cin, namaBarang);
    for(int i = 0; i < temp->jml_stok; i++){
        if(temp->stok[i].namaBarang == namaBarang){
            cout << "Nama Barang: " << temp->stok[i].namaBarang << endl;
            cout << "Jumlah barang: " << temp->stok[i].jml_barang << endl;
            cout << "Harga barang: " << temp->stok[i].harga << endl;
            return;
        }
    }
    cout << "barang tidak ditemukan" << endl;
}
void lihatStokCabang(cabang *temp){
    if(temp->jml_stok == 0){
        cout << "belum ada stok" << endl;
        return;
    }
    lihatStockFile(temp);
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

void saveAllCabang(cabang* head) {
    FILE* file = fopen("akun.txt", "w");  
    if (file == NULL) {
        cout << "Gagal membuka file untuk penyimpanan." << endl;
        return;
    }
    
    cabang* current = head;
    while (current != nullptr) {
        fprintf(file, "%s\n%s\n%s\n", 
                current->namaCabang.c_str(),
                current->username.c_str(),
                current->password.c_str());
        current = current->next;
    }
    fclose(file);
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
        
        new_cabang->next = nullptr;

        // Tambahkan ke linked list
        if (*head == nullptr) {
            *head = new_cabang;
        } else {
            cabang* temp = *head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = new_cabang;
        }
    }
    fclose(file);
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
        case 0:
            /* code */
            break;
        default:
            break;
        }
    } while (pilih != 0);
    
}

cabang* loginCabang(const string& username, const string& password) {
    cabang* temp = head;
    while (temp != nullptr) {
        if (temp->username == username && temp->password == password) {
            cout << "Login berhasil, selamat datang di cabang " << temp->namaCabang << endl;
            return temp;
        }
        temp = temp->next;
    }
    cout << "Login gagal. Username atau password salah." << endl;
    return nullptr;
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
        if (cabangLogin != nullptr) {
            cabangFunction(cabangLogin);
        }
    }
}
int main(){
    readFileAkun(&head);
    loginFunction();
    saveAllCabang(head);
}