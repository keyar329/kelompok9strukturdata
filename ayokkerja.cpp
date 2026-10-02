/*
 * ============================================================
 *  SISTEM MANAJEMEN ANTRIAN RESTORAN NUSANTARA RAYA
 *  Mata Kuliah : Struktur Data
 *  Topik       : Array dan Stack
 * ============================================================
 */

#include <iostream>
#include <string>
using namespace std;

// ============================================================
//  KONSTANTA
// ============================================================
const int MAX_ARRAY   = 50;   // kapasitas array statis
const int MAX_STACK   = 50;   // kapasitas stack antrian aktif
const int MAX_HISTORY = 100;  // kapasitas stack riwayat

// ============================================================
//  STRUCT PESANAN
// ============================================================
struct Pesanan {
    string idPesanan;
    string namaPelanggan;
    string namaMenu;
    int    jumlahPorsi;
    string waktuMasuk;
    string status;  // "ANTRI" / "PROSES" / "SELESAI"
};

// ============================================================
//  ARRAY STATIS - penyimpanan utama semua pesanan
// ============================================================
Pesanan arrayPesanan[MAX_ARRAY];
int jumlahArray = 0;  // jumlah elemen aktif di array

// ============================================================
//  STACK ANTRIAN AKTIF (2 stack untuk perilaku FIFO)
// ============================================================
Pesanan stackIn[MAX_STACK];   // pesanan masuk ditumpuk di sini
Pesanan stackOut[MAX_STACK];  // pesanan diambil dari sini
int topIn  = 0;               // indeks atas stack_in
int topOut = 0;               // indeks atas stack_out

// ============================================================
//  STACK RIWAYAT (LIFO - pesanan selesai)
// ============================================================
Pesanan history[MAX_HISTORY];
int topHistory = 0;

// ============================================================
//  FUNGSI BANTU - cetak satu baris pesanan
// ============================================================
void cetakPesanan(Pesanan p) {
    cout << "  ID Pesanan  : " << p.idPesanan     << endl;
    cout << "  Pelanggan   : " << p.namaPelanggan << endl;
    cout << "  Menu        : " << p.namaMenu       << endl;
    cout << "  Porsi       : " << p.jumlahPorsi    << endl;
    cout << "  Waktu Masuk : " << p.waktuMasuk     << endl;
    cout << "  Status      : " << p.status         << endl;
    cout << "  ------------------------------------------" << endl;
}

// ============================================================
//  OPERASI ARRAY
// ============================================================

// Menyisipkan pesanan pada indeks tertentu
void insertAt(int index, Pesanan p) {
    if (jumlahArray >= MAX_ARRAY) {
        cout << "[!] Array penuh, tidak bisa menyisipkan pesanan." << endl;
        return;
    }
    if (index < 0 || index > jumlahArray) {
        cout << "[!] Index tidak valid." << endl;
        return;
    }
    // Geser elemen ke kanan mulai dari akhir sampai index
    for (int i = jumlahArray - 1; i >= index; i--) {
        arrayPesanan[i + 1] = arrayPesanan[i];
    }
    arrayPesanan[index] = p;
    jumlahArray++;
}

// Menghapus pesanan pada indeks tertentu
void deleteAt(int index) {
    if (jumlahArray == 0) {
        cout << "[!] Array kosong." << endl;
        return;
    }
    if (index < 0 || index >= jumlahArray) {
        cout << "[!] Index tidak valid." << endl;
        return;
    }
    // Geser elemen ke kiri dari index
    for (int i = index; i < jumlahArray - 1; i++) {
        arrayPesanan[i] = arrayPesanan[i + 1];
    }
    jumlahArray--;
}

// Mencari pesanan berdasarkan ID di array
int search(string idPesanan) {
    for (int i = 0; i < jumlahArray; i++) {
        if (arrayPesanan[i].idPesanan == idPesanan) {
            return i;  // kembalikan index jika ditemukan
        }
    }
    return -1;  // tidak ditemukan
}

// Menampilkan seluruh isi array
void displayArray() {
    if (jumlahArray == 0) {
        cout << "  [!] Array kosong." << endl;
        return;
    }
    for (int i = 0; i < jumlahArray; i++) {
        cout << "  [" << i << "]" << endl;
        cetakPesanan(arrayPesanan[i]);
    }
}

// ============================================================
//  OPERASI STACK ANTRIAN AKTIF
// ============================================================

// Menambahkan pesanan baru ke antrian (push ke stack_in)
void push(Pesanan p) {
    if (topIn >= MAX_STACK) {
        cout << "[!] Antrian penuh, tidak bisa menambah pesanan." << endl;
        return;
    }
    stackIn[topIn] = p;
    topIn++;
}

// Memindahkan semua isi stack_in ke stack_out (untuk perilaku FIFO)
void tuangKeStackOut() {
    while (topIn > 0) {
        topIn--;
        stackOut[topOut] = stackIn[topIn];
        topOut++;
    }
}

// Mengambil pesanan paling depan dari antrian (FIFO)
// Mengembalikan pesanan dan mengisi parameter p
// Return true jika berhasil, false jika antrian kosong
bool pop(Pesanan &p) {
    if (topOut == 0) {
        if (topIn == 0) {
            cout << "[!] Antrian kosong." << endl;
            return false;
        }
        tuangKeStackOut();
    }
    topOut--;
    p = stackOut[topOut];
    return true;
}

// Melihat pesanan paling depan tanpa mengambilnya
bool peek(Pesanan &p) {
    if (topOut == 0) {
        if (topIn == 0) {
            cout << "[!] Antrian kosong." << endl;
            return false;
        }
        tuangKeStackOut();
    }
    p = stackOut[topOut - 1];
    return true;
}

// Mengecek apakah antrian kosong
bool isEmpty() {
    return (topIn == 0 && topOut == 0);
}

// Mengembalikan jumlah pesanan dalam antrian
int size() {
    return topIn + topOut;
}

// Membatalkan pesanan terakhir yang dimasukkan (undo)
void undoLastOrder() {
    if (topIn == 0) {
        cout << "[!] Tidak ada pesanan yang bisa dibatalkan." << endl;
        return;
    }
    topIn--;
    Pesanan p = stackIn[topIn];
    cout << "[OK] Pesanan " << p.idPesanan << " (" << p.namaPelanggan
         << ") berhasil dibatalkan." << endl;
}

// Menampilkan seluruh isi antrian aktif (tanpa mengubah stack)
void displayAntrian() {
    if (isEmpty()) {
        cout << "  [!] Antrian kosong." << endl;
        return;
    }
    int urutan = 1;
    // Tampilkan stack_out dari atas (pesanan terdepan)
    for (int i = topOut - 1; i >= 0; i--) {
        cout << "  Urutan " << urutan++ << " (terdepan):" << endl;
        cetakPesanan(stackOut[i]);
    }
    // Tampilkan stack_in dari bawah (urutan masuk)
    for (int i = 0; i < topIn; i++) {
        cout << "  Urutan " << urutan++ << ":" << endl;
        cetakPesanan(stackIn[i]);
    }
}

// ============================================================
//  OPERASI STACK RIWAYAT
// ============================================================

// Menyimpan pesanan selesai ke riwayat
void pushHistory(Pesanan p) {
    if (topHistory >= MAX_HISTORY) {
        cout << "[!] Riwayat penuh." << endl;
        return;
    }
    history[topHistory] = p;
    topHistory++;
}

// Mengeluarkan riwayat pesanan terakhir
bool popHistory(Pesanan &p) {
    if (topHistory == 0) {
        cout << "[!] Riwayat kosong." << endl;
        return false;
    }
    topHistory--;
    p = history[topHistory];
    return true;
}

// Melihat pesanan terakhir di riwayat tanpa mengambil
bool peekHistory(Pesanan &p) {
    if (topHistory == 0) {
        cout << "[!] Riwayat kosong." << endl;
        return false;
    }
    p = history[topHistory - 1];
    return true;
}

// Menampilkan seluruh riwayat dari yang terbaru
void displayHistory() {
    if (topHistory == 0) {
        cout << "  [!] Riwayat kosong." << endl;
        return;
    }
    for (int i = topHistory - 1; i >= 0; i--) {
        cout << "  Riwayat ke-" << (topHistory - i) << ":" << endl;
        cetakPesanan(history[i]);
    }
}

// Mencari pesanan di riwayat berdasarkan ID
void searchHistory(string idPesanan) {
    for (int i = topHistory - 1; i >= 0; i--) {
        if (history[i].idPesanan == idPesanan) {
            cout << "[OK] Pesanan ditemukan di riwayat:" << endl;
            cetakPesanan(history[i]);
            return;
        }
    }
    cout << "[!] Pesanan dengan ID " << idPesanan << " tidak ditemukan di riwayat." << endl;
}

// ============================================================
//  FUNGSI INPUT DATA PESANAN DARI USER
// ============================================================
Pesanan inputPesanan() {
    Pesanan p;
    cout << "  Masukkan ID Pesanan    : "; getline(cin, p.idPesanan);
    cout << "  Masukkan Nama Pelanggan: "; getline(cin, p.namaPelanggan);
    cout << "  Masukkan Nama Menu     : "; getline(cin, p.namaMenu);
    cout << "  Masukkan Jumlah Porsi  : "; cin >> p.jumlahPorsi;
    cin.ignore(1000, '\n');
    cout << "  Masukkan Waktu Masuk   : "; getline(cin, p.waktuMasuk);
    p.status = "ANTRI";
    return p;
}

// ============================================================
//  PROGRAM UTAMA
// ============================================================
int main() {
    int pilihan;

    do {
        cout << endl;
        cout << "========================================" << endl;
        cout << "  SISTEM MANAJEMEN ANTRIAN RESTORAN    " << endl;
        cout << "========================================" << endl;
        cout << "  1. Tambah Pesanan ke Antrian"          << endl;
        cout << "  2. Tampilkan Antrian Aktif"            << endl;
        cout << "  3. Proses Pesanan (ambil dari antrian)"<< endl;
        cout << "  4. Batalkan Pesanan Terakhir (Undo)"   << endl;
        cout << "  5. Tampilkan Riwayat Pesanan"          << endl;
        cout << "  6. Cari Pesanan (Array / Riwayat)"     << endl;
        cout << "  7. Tampilkan Semua Data Array"         << endl;
        cout << "  8. Keluar"                             << endl;
        cout << "========================================" << endl;
        cout << "  Pilih menu (1-8): ";
        cin  >> pilihan;
        cin.ignore(1000, '\n');  // bersihkan newline sisa cin >> pilihan

        cout << endl;

        switch (pilihan) {

            // --------------------------------------------------
            // MENU 1: Tambah Pesanan ke Antrian
            // --------------------------------------------------
            case 1: {
                cout << "=== TAMBAH PESANAN ===" << endl;
                Pesanan p = inputPesanan();
                // Tambah ke stack antrian
                push(p);
                // Simpan juga ke array dengan insertAt di akhir
                insertAt(jumlahArray, p);
                cout << "[OK] Pesanan " << p.idPesanan << " berhasil ditambahkan." << endl;
                break;
            }

            // --------------------------------------------------
            // MENU 2: Tampilkan Antrian Aktif
            // --------------------------------------------------
            case 2: {
                cout << "=== ANTRIAN AKTIF ===" << endl;
                cout << "  Jumlah pesanan dalam antrian: " << size() << endl << endl;
                displayAntrian();
                break;
            }

            // --------------------------------------------------
            // MENU 3: Proses Pesanan (pop dari antrian)
            // --------------------------------------------------
            case 3: {
                cout << "=== PROSES PESANAN ===" << endl;
                Pesanan p;
                if (pop(p)) {
                    // Ubah status menjadi SELESAI
                    p.status = "SELESAI";
                    // Simpan ke riwayat
                    pushHistory(p);
                    // Update status di array juga
                    int idx = search(p.idPesanan);
                    if (idx != -1) {
                        arrayPesanan[idx].status = "SELESAI";
                    }
                    cout << "[OK] Pesanan berikut selesai diproses:" << endl;
                    cetakPesanan(p);
                }
                break;
            }

            // --------------------------------------------------
            // MENU 4: Batalkan Pesanan Terakhir (Undo)
            // --------------------------------------------------
            case 4: {
                cout << "=== BATALKAN PESANAN TERAKHIR ===" << endl;
                // Simpan id sebelum undo untuk hapus dari array
                if (topIn > 0) {
                    string idBatal = stackIn[topIn - 1].idPesanan;
                    undoLastOrder();
                    // Hapus dari array juga
                    int idx = search(idBatal);
                    if (idx != -1) {
                        deleteAt(idx);
                    }
                } else {
                    undoLastOrder();
                }
                break;
            }

            // --------------------------------------------------
            // MENU 5: Tampilkan Riwayat Pesanan
            // --------------------------------------------------
            case 5: {
                cout << "=== RIWAYAT PESANAN ===" << endl;
                cout << "  Total riwayat: " << topHistory << " pesanan" << endl << endl;
                displayHistory();
                break;
            }

            // --------------------------------------------------
            // MENU 6: Cari Pesanan (Array / Riwayat)
            // --------------------------------------------------
            case 6: {
                cout << "=== CARI PESANAN ===" << endl;
                cout << "  Cari di mana?" << endl;
                cout << "  1. Array (semua pesanan)"  << endl;
                cout << "  2. Riwayat (pesanan selesai)" << endl;
                cout << "  Pilih (1/2): ";
                int pilihanCari;
                cin >> pilihanCari;
                cout << "  Masukkan ID Pesanan: ";
                string idCari;
                cin >> idCari;

                if (pilihanCari == 1) {
                    int idx = search(idCari);
                    if (idx != -1) {
                        cout << "[OK] Pesanan ditemukan di array (index " << idx << "):" << endl;
                        cetakPesanan(arrayPesanan[idx]);
                    } else {
                        cout << "[!] Pesanan dengan ID " << idCari << " tidak ditemukan di array." << endl;
                    }
                } else if (pilihanCari == 2) {
                    searchHistory(idCari);
                } else {
                    cout << "[!] Pilihan tidak valid." << endl;
                }
                break;
            }

            // --------------------------------------------------
            // MENU 7: Tampilkan Semua Data Array
            // --------------------------------------------------
            case 7: {
                cout << "=== SEMUA DATA ARRAY ===" << endl;
                cout << "  Total data: " << jumlahArray << " pesanan" << endl << endl;
                displayArray();
                break;
            }

            // --------------------------------------------------
            // MENU 8: Keluar
            // --------------------------------------------------
            case 8: {
                cout << "Terima kasih telah menggunakan sistem ini." << endl;
                cout << "Program selesai." << endl;
                break;
            }

            default: {
                cout << "[!] Pilihan tidak valid. Masukkan angka 1-8." << endl;
                break;
            }
        }

    } while (pilihan != 8);

    return 0;
}