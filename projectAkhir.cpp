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
}

void lihatCabang(){
    int i = 1;
    cabang* temp = head;
    if(temp == nullptr){
        cout << "belum ada cabang yang ditambahkan";
        return;
    }
    while (temp != nullptr){
        cout << i++ << ". " << temp->namaCabang << endl;
        temp = temp->next;
    }
}

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
void tambahStok(cabang* temp){
    int jml;
    cout << "berapa barang yang ingin diinput: "; cin >> jml;
    for(int i=0; i<jml; i++){
        cout << "Barang ke-" << i+1;
        cout << "nama barang: "; cin.ignore(); getline(cin, temp->stok[temp->jml_stok].namaBarang);
        cout << "Jumlah barang: "; cin >> temp->stok[temp->jml_stok].jml_barang;
        cout << "Harga barang: "; cin >> temp->stok[temp->jml_stok].harga;
        temp->jml_stok++;
    }

    cout << "Stok berhasil ditambahkan.\n";
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
    else{
        while(temp->stok != nullptr)
        cout << "Nama Barang: " << temp->stok->namaBarang << endl;
        cout << "Kuantitas: " << temp->stok->jml_barang << endl;
        cout << "Harga: " << temp->stok->harga << endl;
        temp = temp->next;
    }

}

void buatLaporanKeuangan(cabang *temp){
}

void cabangFunction(cabang *temp){
    int pilih;
    do
    {
        cout << "Selamat Datang admin " << temp->namaCabang;
        cout << "menu";
        cout << "1. lihat stok produk";
        cout << "2. buat laporan keuangan";
        cout << "3. tambah stok";
        cout << "4. cari stok";
        cout << "0. keluar";
        cout << "pilih menu: "; cin >> pilih;
        switch (pilih)
        {
        case 1:
            lihatStokCabang(temp);
            break;
        case 2:
            /* code */
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

cabang* loginCabang() {
    string username, password;
    cout << "username: "; cin.ignore();
    getline(cin, username);
    cout << "password: "; getline(cin, password);
    cabang* temp = head;
    while (temp != nullptr)
    {
        if(temp->username == username && temp->password == password) {
            cout << "login berhasil, selamat datang di cabang " << temp->namaCabang << endl;
            return temp;
        }
        temp = temp->next;
    }
    cout << "Login gagal. Username atau password salah";
    return nullptr;
}


void loginFunction(){
    string username, password;
    cout << "Username: "; getline(cin, username);
    cout << "Password: "; getline(cin, password);

    if (username == "adminPusat" && password == "adminPusat")
    {
        menuPusat();
    } else{
        cabang* cabangLogin = loginCabang();
        if (cabangLogin != nullptr )
        {
            cabangFunction(cabangLogin);
        }
    }
}
int main(){
    loginFunction();
}