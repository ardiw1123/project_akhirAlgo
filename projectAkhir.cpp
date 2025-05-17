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
    stokBarang stok[1000];
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

void hapusCabang(string namaCabang){
    if (head == nullptr)
    {
        cout << "belum ada cabang" << endl;
    }
    cabang *bantu, *hapus = head;
    while(bantu->next->namaCabang != namaCabang){
        bantu = bantu->next;
    } 
    hapus = bantu->next;
    if(hapus->next== nullptr){
        delete hapus;
        bantu->next = nullptr;
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
            cout << "Masukkan nama cabang yang ingin dihapus: "; getline(cin, namaCabang);
            hapusCabang(namaCabang);
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

void lihatStokCabang(cabang *temp){
    if(temp->jml_stok == 0) cout << "belum ada stok" << endl;
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
        cout << "4. lihat laporan penjualan";
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
    cout << "Username: "; cin.ignore(); getline(cin, username);
    cout << "password: "; getline(cin, password);

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

}