#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

struct User{
  int id;
  char name[32];
  char password[32];
};

const char ADMIN_USERNAME[6] = "Admin";
const char ADMIN_PASSWORD[32] = "12345678";
char username[32], password[32], role[8];

bool isLogged = false;

void printJudul(char *string){
  printf("---------------------- %s --------------------- \n", string);
}

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
  char posisi[12];
  char status[16];
}; 

void inputTiket(){
  
}

void initKursi(int jumlahKursi, struct Kursi kursi[]){
  char baris = 'A';
  for(int i = 0; i < jumlahKursi; i++){
    for(int j = 0; j < jumlahKursi; j++){
      sprintf(kursi[j].posisi, "%c%d", baris,  j + 1);
      strcpy(kursi[j].status, "kosong");
    }
  }
}

void printKursi(struct Kursi kursi[], int jumlahKursi){
  
}

void handleRegister(struct User user[]){
  char username[32], password[32];
  
  printJudul("Registrasi User");
  printf("Masukan username anda: ");
  scanf(" %[^\n]s", username);
  printf("Masukan password anda: ");
  scanf(" %[^\n]s", password);
  
  while(strlen(username) == 0 || strlen(password) == 0){
    printf("Username dan password tidak boleh kosong!\n");
    printf("Masukan username anda:");
    scanf("%[^\n]s", username);
    printf("Masukan password anda:");
    scanf("%[^\n]s", password);
  }
  
  for(int i = 0; i <= 1; i++){
    user[i].id += 1;
    strcpy(user[i].name, username);
    strcpy(user[i].password, password);
  }
  
  printf("Registrasi berhasil! \n\n");
  
}

void login(struct User *user){
  int choice;
  printJudul("Selamat datang di sistem pemesanan tiket pesawat");
  
  printf("Silahkan anda bisa login atau registrasi terlebih dahulu\n");
  printf("1. Login\n");
  printf("2. Registrasi\n");
  printf("Pilihan anda (Hanya Angka): ");
  
  scanf("%d", &choice);

  if(choice != 1){
    handleRegister(user);
  }
  
  printJudul("Login");
  printf("Masukan username: ");
  scanf(" %[^\n]c", username);
  printf("Masukan password: ");
  scanf(" %s", password);
  
  int arrayUserLength = sizeof(user) / sizeof(user[0]) == 0 ? 1 : sizeof(user) / sizeof(user[0]);
  printf("array length = %d\n", arrayUserLength);
  
  for(int i = 0; i < arrayUserLength; i++){
    if(strcmp(user[i].name, username) == 0){
      if(strcmp(user[i].password, password) == 0){
        isLogged = true;
      }
      else{
        printf("Password salah\n");
      }
    }
    else if(strcmp(username, ADMIN_USERNAME) == 0){
      if(strcmp(password, ADMIN_PASSWORD) == 0){
        isLogged = true;
        strcpy(role, "Admin");
      }
      else{
        printf("Password salah\n");
      }
    }
    else{
      printf("Username tidak ditemukan\n");
    }
  }
}

void menu(){
  
}

int main(){
  
  struct User user[128];
  
  login(user);
  
  int jumlahTiket = 180;
  // struct Tiket tiket[jumlahTiket];
  // struct Kursi kursi[jumlahTiket];
  // initKursi(jumlahTiket, kursi);
    
  // for(int i = 0; i < jumlahTiket; i++){      
  //   printf("%s ", kursi[i].posisi);     
  // }
  
  printf("isLogged: %b\n", isLogged);
  return 0;
}
