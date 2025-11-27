#include <stdio.h>
#include <stdlib.h>

struct Tiket{
  char kode_penerbangan[50];  
  char pemilik[50];
  char tujuan[5];
  char asal[5];
  char kursi[4];
  char tanggal[16];
  char jam[4];
  char kelas[12];
};

struct Kursi{
  char posisi[4];
  char status[16];
};

void inputTiket()

int main(){
    int jumlahTiket = 180;
   struct Tiket tiket;
   struct Kursi kursi;
    
    
   return 0;
}