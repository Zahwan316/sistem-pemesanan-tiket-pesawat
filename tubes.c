// Login ke admin dengan username: Admin dan password: 12345678
// Kemudian tambah flight terlebih dahulu
// Lalu bisa logout untuk lanjut ke pemesanan di menu user


#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h> // ganti ke windows.h untuk windows

#define MAX_FLIGHTS 100
#define MAX_LOGS 256

enum Status {
  AVAILABLE,
  BOOKED,
};

enum TiketStatus {
  CANCELLED = 0,
  COMPLETED = 1,
};

struct User{
  int id;
  char name[32];
  char password[32];
};

struct Tiket{
  int id_tiket;
  int flight_id;
  int status;
  char pemilik[50];
  char kursi[4];
  char createdBy[32];
  char tanggal_pemesanan[24];
};

struct Kursi{
  int flight_id;
  char posisi[12];
  enum Status status;
}; 

struct Flight{
  int id;
  int harga;
  char kode[16];
  char asal[16];
  char tujuan[16];
  char tanggal[16];
  char jam[6];
  struct Kursi kursi[180];
};

struct Log{
  char user[32];
  char message[128];
  char datetime[32];
};

struct Log logHistory[MAX_LOGS];

const char ADMIN_USERNAME[6] = "Admin";
const char ADMIN_PASSWORD[32] = "12345678";
char username[32], password[32], role[8];
bool isLogged = false;
int jumlahKursi = 180;

//GLOBAL VARIABLES
int GLOBAL_LOG_COUNT = 0;
int GLOBAL_TICKET_INDEX = 0;
int GLOBAL_FLIGHT_COUNT = 0;
int GLOBAL_KURSI_COUNT = 0;

struct Flight flight[MAX_FLIGHTS];


//------------------------------------ UTILS FUNCTIONS -----------------------------------------

void loadingPlaneMove(){
  system("clear");

  for(int i = 0; i < 60; i++){
    system("clear");
    printf("Memuat Sistem...\n\n");

    for(int s = 0; s < i; s++) printf(" ");
    printf("   ___|___\n");

    for(int s = 0; s < i; s++) printf(" ");
    printf("--o--(_)--o--\n");

    for(int s = 0; s < i; s++) printf(" ");
    printf("    /___\\\n");

    printf("\n");
    usleep(120000);
  }
}

void getDateAndTime(char *tanggal, char *jam, char *dateWithTime){
  time_t now = time(NULL);
  struct tm *local = localtime(&now);
  
  strftime(tanggal, 16, "%d-%m-%Y", local);
  strftime(jam, 6, "%H:%M", local);
  strftime(dateWithTime, 24, "%d-%m-%Y %H:%M", local);
}

void createFile(char *fileName){
  FILE *file;
  printf("Load File... \n");
  file = fopen(fileName, "w");
  if(file == NULL){
    printf("File not found, system will create a new file");
    // exit(1);
  }
  fclose(file);
  printf("File is loaded \n");
}

int openFile(struct User *user){
  FILE *file;
  file = fopen("user.txt", "r");
    char buffer[240];
    int i = 0;
    int usrIndex = 0;
  if(file == NULL){
    printf("File not found \n");
    createFile("user.txt");
    return 0;
  }
  else{   
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
  }
  

  // for(int j = 0; j < usrIndex; j++){
  //   printf("name: %s \npassword: %s \n", user[j].name, user[j].password);
  // }
  
  fclose(file);
  printf("users = %d \n", usrIndex);
  
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

void insertLog(char *username, char *action){
  FILE *file;
  file = fopen("log.txt", "a");
  if(file == NULL){
    printf("File not found");
    createFile("log.txt");
  }
  fprintf(file, "%s\n%s\n", username, action);
  fclose(file);
}

void getLogFromFile(struct Log *log){
  FILE *file = fopen("log.txt", "r");
    if(!file) return;
  
    char line[256];
    while(fgets(line, sizeof(line), file)){
      if(GLOBAL_LOG_COUNT >= MAX_LOGS) break;
  
      line[strcspn(line, "\n")] = 0;
  
      char *user = strtok(line, "|");
      char *time = strtok(NULL, "|");
      char *msg  = strtok(NULL, "|");
  
      if(user && time && msg){
        strcpy(logHistory[GLOBAL_LOG_COUNT].user, user);
        strcpy(logHistory[GLOBAL_LOG_COUNT].datetime, time);
        strcpy(logHistory[GLOBAL_LOG_COUNT].message, msg);
        GLOBAL_LOG_COUNT++;
      }
    }
    fclose(file);
}

void getDateTimeLog(char *buffer){
  time_t now = time(NULL);
  struct tm *local = localtime(&now);
  strftime(buffer, 32, "%d-%m-%Y %H:%M", local);
}

void insertLogToStruct(char *message){
  if(GLOBAL_LOG_COUNT >= MAX_LOGS){
      printf("Log sudah penuh!\n");
      return;
    }
  
    char datetime[32];
    getDateTimeLog(datetime);
  
    strcpy(logHistory[GLOBAL_LOG_COUNT].user, username);
    strcpy(logHistory[GLOBAL_LOG_COUNT].datetime, datetime);
    strcpy(logHistory[GLOBAL_LOG_COUNT].message, message);
  
    FILE *file = fopen("log.txt", "a");
    if(file){
      fprintf(file, "%s|%s|%s\n", username, datetime, message);
      fclose(file);
    }
  
    GLOBAL_LOG_COUNT++;
}

void saveFlightToFile(){
  FILE *file = fopen("flight.txt", "w");
  if(!file){
    printf("Gagal menyimpan flight\n");
    return;
  }

  for(int i = 0; i < GLOBAL_FLIGHT_COUNT; i++){
    fprintf(file, "%d|%s|%s|%s|%s|%s|%d\n",
      flight[i].id,
      flight[i].kode,
      flight[i].asal,
      flight[i].tujuan,
      flight[i].tanggal,
      flight[i].jam,
      flight[i].harga
    );
  }

  fclose(file);
}

void loadFlightFromFile(){
  FILE *file = fopen("flight.txt", "r");
  if(!file) return;

  char line[256];

  while(fgets(line, sizeof(line), file)){
    if(GLOBAL_FLIGHT_COUNT >= MAX_FLIGHTS) break;

    line[strcspn(line, "\n")] = 0;

    char *token = strtok(line, "|");
    flight[GLOBAL_FLIGHT_COUNT].id = atoi(token);

    token = strtok(NULL, "|");
    strcpy(flight[GLOBAL_FLIGHT_COUNT].kode, token);

    token = strtok(NULL, "|");
    strcpy(flight[GLOBAL_FLIGHT_COUNT].asal, token);

    token = strtok(NULL, "|");
    strcpy(flight[GLOBAL_FLIGHT_COUNT].tujuan, token);

    token = strtok(NULL, "|");
    strcpy(flight[GLOBAL_FLIGHT_COUNT].tanggal, token);

    token = strtok(NULL, "|");
    strcpy(flight[GLOBAL_FLIGHT_COUNT].jam, token);

    token = strtok(NULL, "|");
    flight[GLOBAL_FLIGHT_COUNT].harga = atoi(token);

    GLOBAL_FLIGHT_COUNT++;
  }

  fclose(file);
}


// ------------------------------------- END UTILS FUNCTIONS -----------------------------

//------------------------------------ DECORATION FUNCTIONS -----------------------------------------

void printJudul(char *string){
  printf("---------------------- %s --------------------- \n", string);
}

void line(){
  printf("============================================================\n");
}

void subLine(){
  printf("------------------------------------------------------------\n");
}

void centerTitle(const char *title){
  line();
  printf(" %-56s\n", title);
  line();
}

// ------------------------------------- END DECORATION FILE -----------------------------

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
  insertLogToStruct("Registrasi User");
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
    
    if(strcmp(username, ADMIN_USERNAME) == 0 && strcmp(password, ADMIN_PASSWORD) == 0){
      isLogged = true;
      strcpy(role, "Admin");
      printf("Login admin berhasil\n");
      return;
    }
    
    for(int i = 0; i <= userCount; i++){
      if(strcmp(user[i].name, username) == 0){
        isUserFound = true;
        if(strcmp(user[i].password, password) == 0){
          isLogged = true;
          isWrongPassword = false;
          insertLogToStruct("Login ke aplikasi");
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
  insertLogToStruct("Log out dari aplikasi");
  isLogged = false;
  strcpy(role, "");
  strcpy(username, "");
  strcpy(password, "");
  printf("Logout berhasil\n");
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

// ---------------------------ADMIN MENU AND FUNCTION---------------------------------


// struct Flight{
//   int id;
//   char kode[16];
//   char asal[16];
//   char tujuan[16];
//   char tanggal[16];
//   char jam[6];
//   int harga;
//   struct Kursi kursi[180];
// };



void checkIsNullTotalFlight(int TOTAL_FLIGHT){
  if(TOTAL_FLIGHT == 0){
    printf("Tidak ada flight yang tersedia\n");
    return;
  }
}

void printFlight(struct Flight *flight){
  printf("----------------------------------------------\n");
  printf(" FLIGHT #%d | %s\n", flight->id, flight->kode);
  printf("----------------------------------------------\n");
  printf(" Rute     : %s -> %s\n", flight->asal, flight->tujuan);
  printf(" Tanggal  : %s\n", flight->tanggal);
  printf(" Jam      : %s\n", flight->jam);
  printf(" Harga    : Rp %d\n", flight->harga);
  printf("----------------------------------------------\n\n");
}

void printAllFlight(struct Flight *flight){
  printf("Semua Flight:\n");
  for(int i = 0; i < GLOBAL_FLIGHT_COUNT; i++){
    printFlight(&flight[i]);
  }
  
  insertLogToStruct("Menampilkan semua flight");
}

void addFlight(struct Flight *flight, struct Kursi *kursi){
  bool isStop = false;
  printJudul("ADD FLIGHT");
  while(!isStop){
    int idx = GLOBAL_FLIGHT_COUNT++;
    
    printf("Masukkan kode flight: ");
    scanf(" %[^\n]", flight[idx].kode);
    
    printf("Masukan Asal flight: ");
    scanf(" %[^\n]", flight[idx].asal);
    
    printf("Masukkan tujuan flight: ");
    scanf(" %[^\n]", flight[idx].tujuan);
    
    printf("Masukkan tanggal flight: ");
    scanf(" %[^\n]", flight[idx].tanggal);
    
    printf("Masukkan jam flight: ");
    scanf(" %[^\n]", flight[idx].jam);
    
    printf("Masukkan harga tiket: ");
    scanf("%d", &flight[idx].harga);
    
    
    flight[idx].id = idx + 1;
    initKursi(jumlahKursi, flight[idx].kursi);
    
    printf("Flight berhasil ditambahkan!\n");
    
    insertLogToStruct("Menambahkan flight !");
    
    printf("Apakah ingin menambahkan flight lagi? (y/n): ");
    char choice;
    scanf(" %c", &choice);
    if(choice == 'n'){
      isStop = true;
    }
  }
}

void editFlight(struct Flight *flight){
  checkIsNullTotalFlight(GLOBAL_FLIGHT_COUNT);
  printf("EDIT FLIGHT");
  bool isContinue = false;
  while(!isContinue){
    
    for(int i = 0; i < GLOBAL_FLIGHT_COUNT; i++){
      printFlight(&flight[i]);
    }
    
    printf("Masukkan ID flight yang ingin diedit: ");
    int id_tiket;
    scanf("%d", &id_tiket);
    
    printf("Pilih data yang ingin diedit:\n");
    printf("1. Tujuan\n");
    printf("2. Asal\n");
    printf("3. Tanggal\n");
    printf("4. Jam\n");
    printf("5. Harga\n");
    
    int choice;
    printf("Pilihanmu: ");
    scanf("%d", &choice);
    
    switch(choice){
      case 1:
        printf("Masukkan tujuan flight: ");
        scanf(" %[^\n]", flight[id_tiket - 1].tujuan);
        break;
      case 2:
        printf("Masukkan asal flight: ");
        scanf(" %[^\n]", flight[id_tiket - 1].asal);
        break;
      case 3:
        printf("Masukkan tanggal flight: ");
        scanf(" %[^\n]", flight[id_tiket - 1].tanggal);
        break;
      case 4:
        printf("Masukkan jam flight: ");
        scanf(" %[^\n]", flight[id_tiket - 1].jam);
        break;
      case 5:
        printf("Masukkan harga tiket: ");
        scanf("%d", &flight[id_tiket - 1].harga);
        break;
      default:
        printf("Pilihan tidak valid\n");
        break;
    }
     
    insertLogToStruct("Mengedit flight !");
    
    printf("Apakah ingin mengedit flight lagi? (y/n): ");
    
    char choiceToStop;
    scanf(" %c", &choiceToStop);
    if(choiceToStop == 'n'){
      isContinue = true;
    }
  }
}

void deleteFlight(struct Flight *flight){
  checkIsNullTotalFlight(GLOBAL_FLIGHT_COUNT);
  
  for(int i = 0; i < GLOBAL_FLIGHT_COUNT; i++){
    printFlight(&flight[i]);
  }
  printJudul("Hapus Flight");
  
  int id_tiket;
  printf("Masukkan ID tiket yang ingin dihapus: ");
  scanf("%d", &id_tiket);
  
  int idx = -1;
  
  for(int i = 0; i < GLOBAL_FLIGHT_COUNT; i++){
    if(flight[i].id == id_tiket){
      idx = i;
      break;
    }
  }
  
  if(idx == -1){
    printf("ID tiket tidak valid\n");
  }
  
  for(int i = idx; i < GLOBAL_FLIGHT_COUNT - 1; i++){
    flight[i] = flight[i + 1];
  }
  
  (GLOBAL_FLIGHT_COUNT)--;
  
  insertLogToStruct("Menghapus Log !");
  
  printf("Flight berhasil dihapus\n"); 
}

void printLogHistory(struct Log *logHistory){
  printJudul("LOG HISTORY");
 
   if(GLOBAL_LOG_COUNT == 0){
     printf("Belum ada log\n");
     return;
   }
 
   for(int i = 0; i < GLOBAL_LOG_COUNT; i++){
     printf("[%s] %s - %s\n",
       logHistory[i].datetime,
       logHistory[i].user,
       logHistory[i].message
     );
   }
}

void adminMenu(struct Flight *flight, struct Kursi *kursi, struct Log *log){
  printJudul("MENU ADMIN");
  printf("1. Tambah Flight \n");
  printf("2. Tampilkan Semua Flight \n");
  printf("3. Edit Flight \n");
  printf("4. Hapus Flight \n");
  printf("5. Tampilkan Log History \n");
  printf("6. Logout \n");
  
  int choice;
  printf("Pilih menu: ");
  scanf("%d", &choice);
  switch(choice){
    case(1):
      addFlight(flight, kursi);
      saveFlightToFile();
      break;
    case(2):
      printAllFlight(flight);
      break;
    case(3):
      editFlight(flight);
      saveFlightToFile();
      break;
    case(4):
      deleteFlight(flight);
      saveFlightToFile();
      break;
    case(5):
      printLogHistory(log);
      break;
    case(6):
      logout();
      break;
    default:
      printf("Pilihan tidak valid\n");
      break;
  }
}

// ------------------------------- END ADMIN MENU & FUNCTION -------------------------------------
 

// -------------------------------- USER MENU AND FUNCTION -----------------------------------------

void printTiket(struct Tiket *tiket, struct Flight *flight){
    line();
    printf(" TIKET ID        : %d\n", tiket->id_tiket);
    subLine();
    printf(" NAMA PENUMPANG  : %s\n", tiket->pemilik);
    printf(" KURSI           : %s\n", tiket->kursi);
    printf(" STATUS          : %s\n",
      tiket->status == COMPLETED ? "COMPLETED" : "CANCELLED");
    printf(" DIPESAN OLEH    : %s\n", tiket->createdBy);
    printf(" TGL PEMESANAN   : %s\n", tiket->tanggal_pemesanan);
    
    subLine();
    printf(" DETAIL PENERBANGAN\n");
    subLine();
    
    int fIdx = tiket->flight_id - 1;
    printf(" KODE FLIGHT     : %s\n", flight[fIdx].kode);
    printf(" RUTE            : %s -> %s\n",
           flight[fIdx].asal,
           flight[fIdx].tujuan);
    printf(" TANGGAL         : %s\n", flight[fIdx].tanggal);
    printf(" JAM             : %s\n", flight[fIdx].jam);
    
    line();
    printf("\n");
}

int findTiketIndexById(struct Tiket *tiket, int id){
  for(int i = 0; i < GLOBAL_TICKET_INDEX; i++){
    if(tiket[i].id_tiket == id){
      return i;
    }
  }
  return -1;
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

void buyTicket(struct Kursi kursi[], struct Tiket tiket[], int jumlahTiket, struct Flight *flight){
  int totalTiketDibeli;
  int selectedFlightID;
  
  printAllFlight(flight);
  
  if(GLOBAL_FLIGHT_COUNT == 0){
    printf("Tidak ada flight yang tersedia\n");
    return;
  }
 
  printf("Pilih ID Penerbangan: ");
  scanf("%d", &selectedFlightID);
  
  
  printf("Jumlah Tiket yang dibeli: ");
  scanf("%d", &totalTiketDibeli);
  getchar();
  
  for(int i = 0; i < totalTiketDibeli; i++){
    char tanggal_pemesanan[16];

    int idx = GLOBAL_TICKET_INDEX++;
    
    printf("---------------Tiket %d-----------------\n", idx+1);
    printf("Masukan nama\t: ");
    scanf(" %[^\n]", tiket[idx].pemilik);   
    printf("--------\n");
    
    printKursi(flight[selectedFlightID - 1].kursi, jumlahTiket);
    printf("--------\n");
    
    bool isSelectedSeatHasOwner = false;
    
    //insert kursi
    do{
      printf("Pilih kursi(Harus Kapital)\t: ");
      scanf(" %[^\n]", tiket[idx].kursi);
      int getIndexKursi = searchKursi(flight[selectedFlightID - 1].kursi, jumlahTiket, tiket[idx].kursi);
      if(flight[selectedFlightID - 1].kursi[getIndexKursi].status == BOOKED){
        printf("Maaf, kursi sudah dibooking \n\n");
        isSelectedSeatHasOwner = true;
      }
      else{
        isSelectedSeatHasOwner = false;
      }
      if(getIndexKursi != -1){
        flight[selectedFlightID - 1].kursi[getIndexKursi].status = BOOKED;
      }
    }while(isSelectedSeatHasOwner);
    getDateAndTime(NULL, NULL, tanggal_pemesanan);
    
    // --save to data ticket
    tiket[idx].id_tiket = idx+1;
    tiket[idx].status = COMPLETED;
    tiket[idx].flight_id = selectedFlightID;
    strcpy(tiket[idx].tanggal_pemesanan, tanggal_pemesanan);
    strcpy(tiket[idx].createdBy, username);
    
    printf("\n \n");
  }
  
  insertLogToStruct("Membeli Tiket");
  
  printf("Tiket berhasil dibeli \n\n");
}

void printMyTicket(struct Tiket *tiket, struct Flight *flight){
  int idx = GLOBAL_TICKET_INDEX;
  
  if(GLOBAL_TICKET_INDEX == 0){
    printf("Tidak ada tiket yang tersedia\n");
    return;
  }
  
  int hasFound = 0;
  
  // bug on here, if user show my ticket
  for(int i = 0; i < idx; i++){
    if(strcmp(tiket[i].createdBy, username) == 0){
      printTiket(&tiket[i], flight);
      hasFound++;
    }
  }
  
  if(!hasFound){
    printf("Tiket tidak ditemukan\n");
  }
  
  insertLogToStruct("Menampilkan Tiket");
}

// -------------------------- SEARCH TICKET BY NAME AND SEAT --------------------------
void searchTicketByName(struct Tiket *tiket, struct Flight *flight){
  char nama[50];
  printf("Masukkan nama yang ingin dicari: ");
  scanf(" %[^\n]", nama);
  
  int hasFound = 0;
  
  for(int i = 0; i < GLOBAL_TICKET_INDEX; i++){
    if(strcmp(tiket[i].pemilik, nama) == 0){
      printTiket(&tiket[i], flight);
      hasFound++;
    }
  }
  
  if(!hasFound){
    printf("Tiket dengan nama %s tidak ditemukan\n", nama);
  }  
}

void searchTicketBySeat(struct Tiket *tiket, struct Flight *flight){
  char kursi[50];
  printf("Masukkan nomor kursi yang ingin dicari: ");
  scanf(" %[^\n]", kursi);
  
  int hasFound = 0;
  for(int i = 0; i < GLOBAL_TICKET_INDEX; i++){
    if(strcmp(tiket[i].kursi, kursi) == 0){
      printTiket(&tiket[i], flight);
      hasFound++;
    }
  }
  
  if(!hasFound){
    printf("Tiket dengan nomor kursi %s tidak ditemukan\n", kursi);
  }
}

void searchTicketByPurchaseDate(struct Tiket *tiket, struct Flight *flight){
  char tanggal[50];
  printf("Masukkan tanggal pemesanan yang ingin dicari: ");
  scanf(" %[^\n]", tanggal);
  
  int hasFound = 0;
  for(int i = 0; i < GLOBAL_TICKET_INDEX; i++){
    if(strcmp(tiket[i].tanggal_pemesanan, tanggal) == 0){
      printTiket(&tiket[i], flight);
      hasFound++;
    }
  }
  
  if(!hasFound){
    printf("Tiket dengan tanggal pemesanan %s tidak ditemukan\n", tanggal);
  }
}

void searchTicket(struct Tiket *tiket, struct Flight *flight){
  if(GLOBAL_TICKET_INDEX == 0){
    printf("Tidak ada tiket yang tersedia\n");
    return;
  }
  printf("Cari Tiket Berdasarkan: \n");
  printf("1. Nama\n");
  printf("2. Kursi\n");
  // printf("3. Tanggal Keberangkatan\n");
  printf("3. Tanggal Pemesanan\n");
  
  int choice;
  printf("Pilihan Kamu: ");
  scanf("%d", &choice);
  
  switch(choice){
    case 1:
      searchTicketByName(tiket, flight);
      break;
    case 2:
      searchTicketBySeat(tiket, flight);
      break;
    // case 3:
    //   searchTicketByDepartureDate(tiket);
    //   break;
    case 3:
      searchTicketByPurchaseDate(tiket, flight);
      break;
    default:
      printf("Pilihan tidak valid\n");
      break;
  }
}

// ---------------------------- END SEARCH TICKET -------------------------------------


// ---------------------------- SORT TICKET -------------------------------------
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

void sortTicketByName(struct Tiket *tiket){
  int i, j;
  for(i = 0; i < GLOBAL_TICKET_INDEX - 1; i++){
    for(j = 0; j < GLOBAL_TICKET_INDEX - i - 1; j++){
      if(strcmp(tiket[j].pemilik, tiket[j+1].pemilik) > 0){
        swap(&tiket[j], &tiket[j+1]);
      }
    }
  }
}

void sortTicket(struct Tiket *tiket, struct Flight *flight){
  if(GLOBAL_TICKET_INDEX == 0){
    printf("Tidak ada tiket yang tersedia\n");
    return;
  }
  int choice;
  
  printf("Urutkan Tiket Berdasarkan: \n");
  printf("1. Kursi \n");
  // printf("2. Tanggal Keberangkatan \n");
  printf("2. Tanggal Pemesanan \n");
  printf("3. Nama Penumpang \n");
  
  printf("Pilihan Kamu: ");
  scanf("%d", &choice);
  
  switch(choice){
    case 1:
      sortTicketBySeat(tiket);
      break;
    // case 2:
    //   sortTicketByDepartureDate(tiket);
    //   break;
    case 2:
      sortTicketByPurchaseDate(tiket);
      break;
    case 3:
      sortTicketByName(tiket);
      break;
    default:
      printf("Pilihan tidak valid\n");
      break;
  }
}

// -------------------------- END SORT TICKET -------------------------------------

bool checkAvailableSeat(struct Kursi *kursi, char *selectedSeat){
  for(int i = 0; i < jumlahKursi; i++){
    if(strcmp(kursi[i].posisi, selectedSeat) == 0){
      return kursi[i].status == BOOKED;
    }
  }
  return false; // kursi tidak ditemukan
}

void editTiket(struct Tiket *tiket, struct Kursi *kursi, struct Flight *flight){
  if(GLOBAL_TICKET_INDEX == 0){
    printf("Tidak ada tiket yang tersedia\n");
    return;
  }
  
  int choice;
  int idCari;
  
  printf("1. Edit Nama Penumpang\n");
  printf("2. Edit Nomor Kursi\n");
  // printf("3. Edit Tanggal Keberangkatan\n");
  
  printf("Pilih yang ingin anda edit: ");
  scanf("%d", &choice);
  
  if(choice < 1 || choice > 3){
    printf("Pilihan tidak valid\n");
    return;
  }
  
  for(int i = 0; i < GLOBAL_TICKET_INDEX; i++){
    if(strcmp(tiket[i].createdBy, username) == 0){
      printTiket(&tiket[i], flight);
    }
  }
  
  printf("Pilih Id Tiket yang ingin diganti: ");
  scanf("%d", &idCari);
  
  int idx = findTiketIndexById(tiket, idCari);
  
  if(idx == -1){
    printf("Tiket tidak ditemukan\n");
    return;
  }
  
  switch(choice){
    case 1:
      printf("Masukkan nama baru: ");
      scanf(" %[^\n]", tiket[idx].pemilik);
      
      printf("Nama berhasil diubah\n");
      break;
    case 2:
      char selectedSeat[10];
      char answer;
      bool isDone = false;
    
      int flightIdx = tiket[idx].flight_id - 1;
    
      printKursi(flight[flightIdx].kursi, jumlahKursi);
    
      do{
        printf("Masukkan nomor kursi baru: ");
        scanf(" %s", selectedSeat);
    
        if(checkAvailableSeat(flight[flightIdx].kursi, selectedSeat)){
          printf("Kursi sudah terisi\n");
          continue;
        }
    
        printf("Yakin ingin mengganti kursi? (y/n): ");
        scanf(" %c", &answer);
    
        if(answer == 'y'){
          int before = searchKursi(flight[flightIdx].kursi, jumlahKursi, tiket[idx].kursi);
          int after  = searchKursi(flight[flightIdx].kursi, jumlahKursi, selectedSeat);
    
          flight[flightIdx].kursi[before].status = AVAILABLE;
          flight[flightIdx].kursi[after].status  = BOOKED;
    
          strcpy(tiket[idx].kursi, selectedSeat);
          isDone = true;
          printf("Kursi berhasil diubah\n");
        }
    
      } while(!isDone && answer != 'n');
      break;
    default:
      printf("Pilihan tidak valid\n");
      break;
  }
  
  insertLogToStruct("Mengedit Tiket");
}

void cancelTicket(struct Tiket *tiket, struct Kursi *kursi, struct Flight *flight){
  if(GLOBAL_TICKET_INDEX == 0){
    printf("Tidak ada tiket yang tersedia\n");
    return;
  }
  
  int idCari;
  char answer;
  
  for(int i = 0; i < GLOBAL_TICKET_INDEX; i++){
    if(strcmp(tiket[i].createdBy, username) == 0){
      printTiket(&tiket[i], flight);
    }
  }
  
  printf("Masukkan ID Tiket yang ingin dibatalkan: ");
  scanf("%d", &idCari);
  
  char choice;
  
  if(idCari > 0 && idCari <= GLOBAL_TICKET_INDEX){
    if(strcmp(tiket[idCari - 1].createdBy, username) == 0){
      printf("Apakah anda yakin ingin membatalkan tiket ini? (y/n): ");
      scanf(" %c", &choice);
      
      if(choice == 'y' || choice == 'Y'){
        int idxKursi = searchKursi(kursi, jumlahKursi, tiket[idCari - 1].kursi);
        flight[tiket[idCari - 1].flight_id].kursi[idxKursi].status = AVAILABLE;
        tiket[idCari - 1].status = CANCELLED;
        printf("Tiket berhasil dibatalkan\n");
      }
      else{
        printf("Tiket tidak dibatalkan\n");
      }
    } else {
      printf("Anda tidak memiliki akses untuk membatalkan tiket ini\n");
    }
  }
  else{
    printf("ID Tiket tidak valid\n");
  }
  
  insertLogToStruct("Cancel Tiket");
}

void menu(struct Tiket *tiket, struct User *user, int totalUser, struct Kursi *kursi, int jumlahKursi, struct Flight *flight){
  int choice;
  printJudul("MENU PEMESANAN TIKET");
  printf(" 1. Beli Tiket\n");
  printf(" 2. Tiket Saya\n");
  printf(" 3. Cari Tiket\n");
  printf(" 4. Urutkan Tiket\n");
  printf(" 5. Edit Tiket\n");
  printf(" 6. Batalkan Tiket\n");
  printf(" 7. Logout\n");
  subLine();
  printf(" Pilihan Anda : ");
  scanf("%d", &choice);
  printJudul("");
  printf("\n");
  
  switch(choice){
    case 1:
      buyTicket(kursi, tiket, jumlahKursi, flight);
      break;
    case 2:
      printMyTicket(tiket, flight);
      break;
    case 3:
      searchTicket(tiket, flight);
      break;
    case 4:
      sortTicket(tiket, flight);
      break;
    case 5:
      editTiket(tiket, kursi, flight);
      break;
    case 6:
      cancelTicket(tiket, kursi, flight);
      break;
    case 7:
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

// --------------------------- END USER MENU AND FUNCTION ---------------------------


int main(){
  struct User user[128];
  struct Kursi kursi[jumlahKursi];
  struct Tiket tiket[jumlahKursi];
  
  //loadFlightFromFile();
  
  int totalUser = openFile(user);
  getLogFromFile(logHistory);
  
  loadingPlaneMove();
  
  while(1){
    if(!isLogout()){
      if(strcmp(username, ADMIN_USERNAME) == 0){
        adminMenu(flight, kursi, logHistory);
      }
      else{
        menu(tiket, user, totalUser, kursi, jumlahKursi, flight);
      }
    }
    else{
      login(user, totalUser);
    }
    
  }
  //printKursi(kursi, jumlahKursi);
 
  
  printf("isLogged: %b\n", isLogged);
  return 0;
}
