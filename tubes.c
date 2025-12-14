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
  char tujuan[16];
  char asal[16];
  char kursi[4];
  char jam[6];
  char kelas[12];
  char createdBy[32];
  char tanggal_keberangkatan[16];
  char tanggal_pemesanan[16];
};

struct Kursi{
  char posisi[12];
  enum Status status;
}; 

void getDateAndTime(char *tanggal, char *jam){
  time_t now = time(NULL);
  struct tm *local = localtime(&now);
  
  strftime(tanggal, 16, "%d-%m-%Y", local);
  strftime(jam, 6, "%H:%M", local);
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

  // for(int j = 0; j < usrIndex; j++){
  //   printf("name: %s \npassword: %s \n", user[j].name, user[j].password);
  // }
  
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

void printTiket(int i, struct Tiket *tiket){
  printf("---------Tiket %d------------ \n", i+1);
  printf("Nama\t\t: %s\n", tiket[i].pemilik);
  printf("Kursi\t\t: %s\n", tiket[i].kursi);
  printf("Kelas\t\t: %s\n", tiket[i].kelas);
  printf("Jam\t\t: %s\n", tiket[i].jam);
  printf("Asal\t\t: %s\n", tiket[i].asal);
  printf("Tujuan\t\t: %s\n", tiket[i].tujuan);
  printf("Tanggal Pemesanan\t: %s\n", tiket[i].tanggal_pemesanan);
  printf("Tanggal Keberangkatan\t: %s\n", tiket[i].tanggal_keberangkatan);
  printf("\n");
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
  scanf(" %[^\n]", asal);
  printf("Tujuan\t: ");
  scanf(" %[^\n]", tujuan);
  printf("Tanggal Keberangkatan(D-M-Y)\t: ");
  scanf(" %[^\n]", tanggal_keberangkatan);
  printf("Jumlah Tiket yang dibeli: ");
  scanf("%d", &totalTiketDibeli);
  getchar();
  
  for(int i = 0; i < totalTiketDibeli; i++){
    char tanggal_pemesanan[16];
    char jam[6];
    char kelas[12];
    int idx = GLOBAL_TICKET_INDEX++;
    
    printf("---------------Tiket %d-----------------\n", idx+1);
    printf("Masukan nama\t: ");
    scanf(" %[^\n]", tiket[idx].pemilik);   
    printf("--------\n");
    printKursi(kursi, jumlahTiket);
    printf("--------\n");
    printf("Pilih kursi(Harus Kapital)\t: ");
    scanf(" %[^\n]", tiket[idx].kursi);
    
    int getIndexKursi = searchKursi(kursi, jumlahTiket, tiket[idx].kursi);
    if(kursi[getIndexKursi].status == BOOKED){
      printf("Maaf, kursi sudah dibooking \n");
      idx--;
      continue;
    }
  
    printf("Kelas\t\t: ");
    scanf(" %[^\n]", kelas);
    getDateAndTime(tanggal_pemesanan, jam);
    
    if(getIndexKursi != -1){
      kursi[getIndexKursi].status = BOOKED;
    }
    
    //save to data ticket
    tiket[idx].id_tiket = idx+1;
    strcpy(tiket[idx].kelas, kelas);
    strcpy(tiket[idx].tanggal_pemesanan, tanggal_pemesanan);
    strcpy(tiket[idx].jam, jam);
    strcpy(tiket[idx].asal, asal);
    strcpy(tiket[idx].tujuan, tujuan);
    strcpy(tiket[idx].tanggal_keberangkatan, tanggal_keberangkatan);
    strcpy(tiket[idx].createdBy, username);
    
    printf("\n \n");
  }
  
  printf("Tiket berhasil dibeli \n\n");
}

void printMyTicket(struct Tiket *tiket){
  if(GLOBAL_TICKET_INDEX == 0){
    printf("Tidak ada tiket yang tersedia\n");
    return;
  }
  
  for(int i = 0; i < GLOBAL_TICKET_INDEX; i++){
    if(strcmp(tiket[i].pemilik, username) == 0){
      printTiket(i, &tiket[i]);
    }
    else{
      printf("Tiket tidak ditemukan\n");
      break;
    }
  }
}

void searchTicketByName(struct Tiket *tiket){
  char nama[50];
  printf("Masukkan nama yang ingin dicari: ");
  scanf("%s", nama);
  
  for(int i = 0; i < GLOBAL_TICKET_INDEX; i++){
    if(strcmp(tiket[i].pemilik, nama) == 0){
      printTiket(i, &tiket[i]);
    }
    else{
      printf("Tiket dengan nama %s tidak ditemukan\n", nama);
    }
  }
}

void searchTicketBySeat(struct Tiket *tiket){
  char kursi[50];
  printf("Masukkan nomor kursi yang ingin dicari: ");
  scanf("%s", kursi);
  
  for(int i = 0; i < GLOBAL_TICKET_INDEX; i++){
    if(strcmp(tiket[i].kursi, kursi) == 0){
      printTiket(i, &tiket[i]);
    }
    else{
      printf("Tiket dengan nomor kursi %s tidak ditemukan\n", kursi);
    }
  }
}

void searchTicketByDepartureDate(struct Tiket *tiket){
  char tanggal[50];
  printf("Masukkan tanggal keberangkatan yang ingin dicari: ");
  scanf("%s", tanggal);
  
  for(int i = 0; i < GLOBAL_TICKET_INDEX; i++){
    if(strcmp(tiket[i].tanggal_keberangkatan, tanggal) == 0){
      printTiket(i, &tiket[i]);
    }
    else{
      printf("Tiket dengan tanggal keberangkatan %s tidak ditemukan\n", tanggal);
    }
  }
}

void searchTicketByPurchaseDate(struct Tiket *tiket){
  char tanggal[50];
  printf("Masukkan tanggal pemesanan yang ingin dicari: ");
  scanf("%s", tanggal);
  
  for(int i = 0; i < GLOBAL_TICKET_INDEX; i++){
    if(strcmp(tiket[i].tanggal_pemesanan, tanggal) == 0){
      printTiket(i, &tiket[i]);
    }
    else{
      printf("Tiket dengan tanggal pemesanan %s tidak ditemukan\n", tanggal);
    }
  }
}

void searchTicket(struct Tiket *tiket){
  if(GLOBAL_TICKET_INDEX == 0){
    printf("Tidak ada tiket yang tersedia\n");
    return;
  }
  printf("Cari Tiket Berdasarkan: \n");
  printf("1. Nama\n");
  printf("2. Kursi\n");
  printf("3. Tanggal Keberangkatan\n");
  printf("4. Tanggal Pemesanan\n");
  
  int choice;
  printf("Pilihan Kamu: ");
  scanf("%d", &choice);
  
  switch(choice){
    case 1:
      searchTicketByName(tiket);
      break;
    case 2:
      searchTicketBySeat(tiket);
      break;
    case 3:
      searchTicketByDepartureDate(tiket);
      break;
    case 4:
      searchTicketByPurchaseDate(tiket);
      break;
    default:
      printf("Pilihan tidak valid\n");
      break;
  }
}

void swap(struct Tiket *a, struct Tiket *b){
  struct Tiket temp = *a;
  *a = *b;
  *b = temp;
}

void sortTicketBySeat(struct Tiket *tiket){
  int i, j;
  for(i = 0; i < GLOBAL_TICKET_INDEX - 1; i++){
    for(j = 0; j < GLOBAL_TICKET_INDEX - i - 1; j++){
      if(strcmp(tiket[j].kursi, tiket[j+1].kursi) > 0){
        swap(&tiket[j], &tiket[j+1]);
      }
    }
  }
}

void sortTicketByDepartureDate(struct Tiket *tiket){
  int i, j;
  for(i = 0; i < GLOBAL_TICKET_INDEX - 1; i++){
    int minIndex = i;
    for(j = i + 1; j < GLOBAL_TICKET_INDEX; j++){
      if(strcmp(tiket[j].tanggal_keberangkatan, tiket[minIndex].tanggal_keberangkatan) < 0){
        minIndex = j;
      }
    }
    swap(&tiket[i], &tiket[minIndex]);
  }
}

void sortTicketByPurchaseDate(struct Tiket *tiket){
  int i, j;
  for(i = 0; i < GLOBAL_TICKET_INDEX - 1; i++){
    for(j = 0; j < GLOBAL_TICKET_INDEX - i - 1; j++){
      if(strcmp(tiket[j].tanggal_pemesanan, tiket[j+1].tanggal_pemesanan) > 0){
        swap(&tiket[j], &tiket[j+1]);
      }
    }
  }
}

void sortTicket(struct Tiket *tiket){
  if(GLOBAL_TICKET_INDEX == 0){
    printf("Tidak ada tiket yang tersedia\n");
    return;
  }
  int choice;
  
  printf("Urutkan Tiket Berdasarkan: \n");
  printf("1. Kursi \n");
  printf("2. Tanggal Keberangkatan \n");
  printf("3. Tanggal Pemesanan \n");
  
  printf("Pilihan Kamu: ");
  scanf("%d", &choice);
  
  switch(choice){
    case 1:
      sortTicketBySeat(tiket);
      break;
    case 2:
      sortTicketByDepartureDate(tiket);
      break;
    case 3:
      sortTicketByPurchaseDate(tiket);
      break;
    default:
      printf("Pilihan tidak valid\n");
      break;
  }
}

void menu(struct Tiket *tiket, struct User *user, int totalUser, struct Kursi *kursi, int jumlahKursi){
  int choice;
  printJudul("Selamat datang di tiket pemesanan pesawat");
  printf("1. Beli Tiket\n");
  printf("2. Tampilkan Semua Tiket Saya\n");
  printf("3. Cari Tiket\n");
  printf("4. Urutkan Tiket\n");
  printf("5. Logout\n");
  
  printf("Pilihan Kamu: ");
  scanf("%d", &choice);
  printJudul("");
  printf("\n");
  
  switch(choice){
    case 1:
      buyTicket(kursi, tiket, jumlahKursi);
      break;
    case 2:
      printMyTicket(tiket);
      break;
    case 3:
      searchTicket(tiket);
      break;
    case 4:
      sortTicket(tiket);
      break;
    case 5:
      logout();
      break;
    default:
      printf("Pilihan tidak valid\n");
      break;
  }  
}

bool isLogout(){
  return strcmp(username, "") == 0 && strcmp(password, "") == 0 && strcmp(role, "") == 0;
}

int main(){
  int jumlahKursi = 180;
  
  struct User user[128];
  struct Kursi kursi[jumlahKursi];
  struct Tiket tiket[jumlahKursi];
  
  initKursi(jumlahKursi, kursi);
  int totalUser = openFile(user);
  
  //buyTicket(kursi, tiket, jumlahKursi);
  while(1){
    if(!isLogout()){
      menu(tiket, user, totalUser, kursi, jumlahKursi);
    }
    else{
      login(user, totalUser);
    }
    
  }
  //printKursi(kursi, jumlahKursi);
 
  
  printf("isLogged: %b\n", isLogged);
  return 0;
}
