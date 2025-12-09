#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

const char ADMIN_USERNAME[6] = "Admin";
const char ADMIN_PASSWORD[32] = "12345678";
char username[32], password[32], role[8];
bool isLogged = false;

enum Status {
  AVAILABLE,
  BOOKED,
};

struct User{
  int id;
  char name[32];
  char password[32];
};

struct Tiket{
  int id_tiket;
  char kode_maskapai[50];  
  char pemilik[50];
  char tujuan[5];
  char asal[5];
  char kursi[4];
  char tanggal_keberangkatan[16];
  char tanggal_pemesanan[16];
  char jam[6];
  char kelas[12];
};

struct Kursi{
  char posisi[12];
  enum Status status;
}; 

void getDateAndTime(char *tanggal, char *jam){
  time_t now = time(NULL);
  struct tm *local = localtime(&now);
  
  strftime(tanggal, sizeof(tanggal), "%d-%m-%y", local);
  strftime(jam, sizeof(jam), "%H:%M", local);
}

void createFile(){
  FILE *file;
  printf("Load File... \n");
  file = fopen("user.txt", "w");
  if(file == NULL){
    printf("File not found");
    exit(1);
  }
  fprintf(file, "Lorem Ipsum dolor");
  fclose(file);
  printf("File is loaded \n");
}

int openFile(struct User *user){
  FILE *file;
  file = fopen("user.txt", "r");
  if(file == NULL){
    printf("File not found");
    exit(1);
  }
  
  char buffer[240];
  int i = 0;
  int usrIndex = 0;

  
  while(fgets(buffer, sizeof(buffer), file)){
    buffer[strcspn(buffer, "\n")] = '\0';
    if(i % 2 == 0){
      strcpy(user[usrIndex].name, buffer);
    }
    else{
      strcpy(user[usrIndex].password, buffer);
      usrIndex++;
    }
      
    i++;
  }

  for(int j = 0; j < usrIndex; j++){
    printf("name: %s \npassword: %s \n", user[j].name, user[j].password);
  }
  
  printf("users = %d \n", usrIndex);
  
  fclose(file);
  return usrIndex;
}

void insertDataToFile(char *username, char *password){
  FILE *file;
  file = fopen("user.txt", "a");
  if(file == NULL){
    printf("File not found");
    exit(1);
  }
  fprintf(file, "%s\n%s\n", username, password);
  fclose(file);
}

void printJudul(char *string){
  printf("---------------------- %s --------------------- \n", string);
}

void inputTiket(){
  
}

void initKursi(int jumlahKursi, struct Kursi kursi[]){
  char baris = 'A';
  int rows = jumlahKursi / 8;
  int cols = jumlahKursi / rows;
  int index = 0;
  for(int i = 0; i < rows; i++){
    for(int j = 0; j < cols; j++){
      sprintf(kursi[index].posisi, "%c%d", baris,  j + 1);
      kursi[index].status = AVAILABLE;
      index++;
    }
    baris++;
  }
}

void printKursi(struct Kursi kursi[], int jumlahKursi){
  int jmlhBaris = jumlahKursi / 8;
  int jmlhKolom = jumlahKursi / jmlhBaris;
  int index = 0;
  char baris = 'A';
  
  for(int i = 0; i < jmlhBaris; i++){
    for(int j = 0; j < jmlhKolom; j++){
      if(j == 4){
        printf("  ");
      }
      //printf("%c%d ", baris, j + 1);
      //printf("%d ", kursi[index].status);
      if(kursi[index].status == AVAILABLE){
        printf("%s ", kursi[index].posisi);
      }
      else{
        printf("X ");
      }
      index++;
    }
    printf("\n");
    baris++;
  }
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
  insertDataToFile(username, password);
  printf("Registrasi berhasil! \n\n");  
}

void login(struct User user[], int userCount){
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
  
  if(strcmp(username, ADMIN_USERNAME) == 0 && strcmp(password, ADMIN_PASSWORD) == 0){
    isLogged = true;
    strcpy(role, "Admin");
    printf("Login admin berhasil\n");
    return;
  }
  
  bool isUserFound = false;
  bool isWrongPassword = false;
  
  do{
    printJudul("Login");
    printf("Masukan username: ");
    scanf(" %[^\n]c", username);
    printf("Masukan password: ");
    scanf(" %s", password);
    
    isWrongPassword = false;
    isUserFound = false;
    
    for(int i = 0; i <= userCount; i++){
      if(strcmp(user[i].name, username) == 0){
        isUserFound = true;
        if(strcmp(user[i].password, password) == 0){
          isLogged = true;
          isWrongPassword = false;
          printf("Login berhasil\n\n");
        }
        else{
          //printf("Password salah\n\n");
          isWrongPassword = true;
        }
      }
    }
    
    if(!isUserFound){
      printf("Username tidak ditemukan\n\n");
    }
    
    if(isWrongPassword){
      printf("Password salah\n\n");
    }
  }
  while(!isLogged); 
}

void logout(){
  isLogged = false;
  strcpy(role, "");
  strcpy(username, "");
  strcpy(password, "");
  printf("Logout berhasil\n");
}

void menu(){
  
}

int searchKursi(struct Kursi kursi[], int jumlahKursi, char *find){
  int i = 0;
  while(i < jumlahKursi){
    if(strcmp(kursi[i].posisi, find) == 0){
      return i;
    }
    i++;
  }
  
  return -1;
}

int GLOBAL_TICKET_INDEX = 0;
void buyTicket(struct Kursi kursi[], struct Tiket tiket[], int jumlahTiket){
  int totalTiketDibeli;
  char asal[16];
  char tujuan[16];
  char tanggal_keberangkatan[16];
  
  printf("Asal\t: ");
  scanf(" %[^\n]s", asal);
  printf("Tujuan\t: ");
  scanf(" %[^\n]s", tujuan);
  printf("Tanggal Keberangkatan(D-M-Y)\t: ");
  scanf(" %[^\n]s", tanggal_keberangkatan);
  printf("tiket yang dibeli: ");
  scanf("%d", &totalTiketDibeli);
  printf("\n");
  
  for(int i = 0; i < totalTiketDibeli; i++){
    char tanggal_pemesanan[16];
    char jam[6];
    char kelas[12];
    int idx = GLOBAL_TICKET_INDEX++;
    
    printf("---------------Tiket %d-----------------\n", idx+1);
    printf("Masukan nama\t: ");
    scanf(" %[^\n]s", tiket[idx].pemilik);   
    printf("--------\n");
    printKursi(kursi, jumlahTiket);
    printf("--------\n");
    printf("Pilih kursi(Harus Kapital)\t: ");
    scanf(" %[^\n]s", tiket[idx].kursi);
    
    int getIndexKursi = searchKursi(kursi, jumlahTiket, tiket[idx].kursi);
    if(kursi[getIndexKursi].status == BOOKED){
      printf("Maaf, kursi sudah dibooking \n");
      idx--;
      continue;
    }
  
    printf("Kelas\t\t: ");
    scanf(" %[^\n]s", kelas);
    getDateAndTime(tanggal_pemesanan, jam);
    
    if(getIndexKursi != -1){
      kursi[getIndexKursi].status = BOOKED;
    }
    
    tiket[idx].id_tiket = idx+1;
    strcpy(tiket[idx].kelas, kelas);
    strcpy(tiket[idx].tanggal_pemesanan, tanggal_pemesanan);
    strcpy(tiket[idx].jam, jam);
    strcpy(tiket[idx].asal, asal);
    strcpy(tiket[idx].tujuan, tujuan);
    strcpy(tiket[idx].tanggal_keberangkatan, tanggal_keberangkatan);
  }
  
  printf("Tiket berhasil dibeli \n\n");
  for(int i = 0; i < totalTiketDibeli; i++){
    printf("---------Tiket %d\n------------", i+1);
    printf("Nama\t\t: %s\n", tiket[i].pemilik);
    printf("Kursi\t\t: %s\n", tiket[i].kursi);
    printf("Kelas\t\t: %s\n", tiket[i].kelas);
    printf("Tanggal Pemesanan\t: %s\n", tiket[i].tanggal_pemesanan);
    printf("Jam\t\t: %s\n", tiket[i].jam);
    printf("Asal\t\t: %s\n", tiket[i].asal);
    printf("Tujuan\t\t: %s\n", tiket[i].tujuan);
    printf("Tanggal Keberangkatan\t: %s\n", tiket[i].tanggal_keberangkatan);
    printf("\n");
  }
}

int main(){
  int jumlahKursi = 180;
  
  struct User user[128];
  struct Kursi kursi[jumlahKursi];
  struct Tiket tiket[jumlahKursi];
  
  initKursi(jumlahKursi, kursi);
  int totalUser = openFile(user);
  //login(user, totalUser);
  
  //printKursi(kursi, jumlahKursi);
  buyTicket(kursi, tiket, jumlahKursi);
 
  
  printf("isLogged: %b\n", isLogged);
  return 0;
}
