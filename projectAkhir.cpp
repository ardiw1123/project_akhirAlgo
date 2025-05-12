#include <iostream>
#include <stdio.h>
#include <string>

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
    stokBarang stok[100];
    keuangan keuangan;
    int jml_stok = 0;
    cabang *next;
};
cabang* head = nullptr;

void tambahCabang(){
    cabang* new_cabang = new cabang;
    cout << "Nama cabang: "; cin.ignore();
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
void menuPusat() {
}
void cabangFunction(){
    int pilih;
    cabang* temp = head;
    do
    {
        cout << "Selamat Datang admin " << temp->namaCabang;
        cout << "menu";
        cout << "1. lihat stok produk";
        cout << "2. buat laporan keuangan";
        cout << "3. tambah stok";
        cout << "4. lihat laporan penjualan";
        cout << "0. keluar";
        cout << "pilih menu: "; cin >> pilih;
        switch (pilih)
        {
        case 1:
            /* code */
            break;
        case 2:
            /* code */
            break;
        case 3:
            /* code */
            break;
        case 4:
            /* code */
            break;
        case 0:
            /* code */
            break;
        default:
            break;
        }
    } while (pilih != 0);
    
}

void loginCabang(string username, string password) {
    cout << "username: "; cin.ignore();
    getline(cin, username);
    cout << "password: "; getline(cin, password);
    cabang* temp = head;
    while (temp != nullptr)
    {
        if(temp->username == username && temp->password == password) {
            cout << "login berhasil, selamat datang di cabang " << temp->namaCabang << endl;
        }
        temp = temp->next;
    }
    cout << "Login gagal. Username atau password salah";
}


void loginFunction(){
    string username, password;
    cout << "Username: "; cin.ignore(); getline(cin, username);
    cout << "password: "; getline(cin, password);

    if (username == "adminPusat" && password == "adminPusat")
    {
        menuPusat();
    } else{
        loginCabang(username, password);
        cabangFunction();
    }
}


int main(){

}