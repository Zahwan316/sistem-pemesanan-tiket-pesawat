#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define MAX_FLIGHTS 100

const char ADMIN_USERNAME[6] = "Admin";
const char ADMIN_PASSWORD[32] = "12345678";
char username[32], password[32], role[8];
bool isLogged = false;
int jumlahKursi = 180;

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
  char pemilik[50];
  char kursi[4];
  char createdBy[32];
  int status;
  char tanggal_pemesanan[24];
};

struct Kursi{
  char posisi[12];
  enum Status status;
}; 

struct Flight{
  int id;
  char kode[16];
  char asal[16];
  char tujuan[16];
  char tanggal[16];
  char jam[6];
  int harga;
  struct Kursi kursi[180];
};

void getDateAndTime(char *tanggal, char *jam, char *dateWithTime){
  time_t now = time(NULL);
  struct tm *local = localtime(&now);
  
  strftime(tanggal, 16, "%d-%m-%Y", local);
  strftime(jam, 6, "%H:%M", local);
  strftime(dateWithTime, 24, "%d-%m-%Y %H:%M", local);
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

int GLOBAL_FLIGHT_COUNT = 0;

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
  
  printf("Flight berhasil dihapus\n");
  
}

void adminMenu(struct Flight *flight, struct Kursi *kursi){
  printJudul("MENU ADMIN");
  printf("1. Tambah Flight \n");
  printf("2. Tampilkan Semua Flight \n");
  printf("3. Edit Flight \n");
  printf("4. Hapus Flight \n");
  printf("5. Logout \n");
  
  int choice;
  printf("Pilih menu: ");
  scanf("%d", &choice);
  switch(choice){
    case(1):
      addFlight(flight, kursi);
      break;
    case(2):
      printAllFlight(flight);
      break;
    case(3):
      editFlight(flight);
      break;
    case(4):
      deleteFlight(flight);
      break;
    case(5):
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
  printf(" TIKET #%d\n", tiket->id_tiket);
  line();

  printf(" Nama Penumpang : %s\n", tiket->pemilik);
  printf(" Kursi          : %s\n", tiket->kursi);
  printf(" Status         : %s\n",
         tiket->status == COMPLETED ? "COMPLETED" : "CANCELLED");
  printf(" Dipesan Oleh   : %s\n", tiket->createdBy);
  printf(" Tgl Pemesanan  : %s\n", tiket->tanggal_pemesanan);

  subLine();
  printf(" DETAIL PENERBANGAN\n");
  subLine();

  int fIdx = tiket->flight_id - 1;
  printf(" Kode Flight    : %s\n", flight[fIdx].kode);
  printf(" Rute           : %s -> %s\n", flight[fIdx].asal, flight[fIdx].tujuan);
  printf(" Tanggal        : %s\n", flight[fIdx].tanggal);
  printf(" Jam            : %s\n", flight[fIdx].jam);

  line();
  printf("\n");
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

int GLOBAL_TICKET_INDEX = 0;

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
}

// -------------------------- SEARCH TICKET BY NAME AND SEAT --------------------------
void searchTicketByName(struct Tiket *tiket, struct Flight *flight){
  char nama[50];
  printf("Masukkan nama yang ingin dicari: ");
  scanf("%s", nama);
  
  for(int i = 0; i < GLOBAL_TICKET_INDEX; i++){
    if(strcmp(tiket[i].pemilik, nama) == 0){
      printTiket(&tiket[i], flight);
    }
    else{
      printf("Tiket dengan nama %s tidak ditemukan\n", nama);
    }
  }
}

void searchTicketBySeat(struct Tiket *tiket, struct Flight *flight){
  char kursi[50];
  printf("Masukkan nomor kursi yang ingin dicari: ");
  scanf("%s", kursi);
  
  for(int i = 0; i < GLOBAL_TICKET_INDEX; i++){
    if(strcmp(tiket[i].kursi, kursi) == 0){
      printTiket(&tiket[i], flight);
    }
    else{
      printf("Tiket dengan nomor kursi %s tidak ditemukan\n", kursi);
    }
  }
}

// void searchTicketByDepartureDate(struct Tiket *tiket){
//   char tanggal[50];
//   printf("Masukkan tanggal keberangkatan yang ingin dicari: ");
//   scanf("%s", tanggal);
  
//   for(int i = 0; i < GLOBAL_TICKET_INDEX; i++){
//     if(strcmp(tiket[i].tanggal_keberangkatan, tanggal) == 0){
//       printTiket(&tiket[i]);
//     }
//     else{
//       printf("Tiket dengan tanggal keberangkatan %s tidak ditemukan\n", tanggal);
//     }
//   }
// }

void searchTicketByPurchaseDate(struct Tiket *tiket, struct Flight *flight){
  char tanggal[50];
  printf("Masukkan tanggal pemesanan yang ingin dicari: ");
  scanf("%s", tanggal);
  
  for(int i = 0; i < GLOBAL_TICKET_INDEX; i++){
    if(strcmp(tiket[i].tanggal_pemesanan, tanggal) == 0){
      printTiket(&tiket[i], flight);
    }
    else{
      printf("Tiket dengan tanggal pemesanan %s tidak ditemukan\n", tanggal);
    }
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
  printf("4. Tanggal Pemesanan\n");
  
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
    case 4:
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

// void sortTicketByDepartureDate(struct Tiket *tiket){
//   int i, j;
//   for(i = 0; i < GLOBAL_TICKET_INDEX - 1; i++){
//     int minIndex = i;
//     for(j = i + 1; j < GLOBAL_TICKET_INDEX; j++){
//       if(strcmp(tiket[j].tanggal_keberangkatan, tiket[minIndex].tanggal_keberangkatan) < 0){
//         minIndex = j;
//       }
//     }
//     swap(&tiket[i], &tiket[minIndex]);
//   }
// }

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

void sortTicket(struct Tiket *tiket, struct Flight *flight){
  if(GLOBAL_TICKET_INDEX == 0){
    printf("Tidak ada tiket yang tersedia\n");
    return;
  }
  int choice;
  
  printf("Urutkan Tiket Berdasarkan: \n");
  printf("1. Kursi \n");
  // printf("2. Tanggal Keberangkatan \n");
  printf("3. Tanggal Pemesanan \n");
  
  printf("Pilihan Kamu: ");
  scanf("%d", &choice);
  
  switch(choice){
    case 1:
      sortTicketBySeat(tiket);
      break;
    // case 2:
    //   sortTicketByDepartureDate(tiket);
    //   break;
    case 3:
      sortTicketByPurchaseDate(tiket);
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
      //printf("Kursi sudah terisi\n");
      return true;
    }
  }
  
  //printf("Kursi tersedia\n");
  return false;
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
  
  switch(choice){
    case 1:
      printf("Masukkan nama baru: ");
      scanf(" %[^\n]]", tiket[idCari - 1].pemilik);
      
      printf("Nama berhasil diubah\n");
      break;
    case 2:
      char selectedSeat[10];
      char *answer = "y";
      bool isAvailableSeat = false;
      printJudul("EDIT KURSI");
      printKursi(flight[tiket[idCari - 1].flight_id].kursi, jumlahKursi);
      do{
        printf("Masukkan nomor kursi baru: ");
        scanf(" %s", selectedSeat);
        
        if(checkAvailableSeat(kursi, selectedSeat)){
          printf("Kursi sudah terisi\n");
        } else {
          printf("Apakah anda yakin ingin mengubah posisi kursi? (y/n): ");
          scanf(" %c", answer);
          int idxKursiAfter = searchKursi(kursi, jumlahKursi, selectedSeat);
          int idxKursiBefore = searchKursi(kursi, jumlahKursi, tiket[idCari - 1].kursi);
          if(strcmp(answer, "y") == 0){
            flight[tiket[idCari - 1].flight_id].kursi[idxKursiBefore].status = AVAILABLE;
            flight[tiket[idCari - 1].flight_id].kursi[idxKursiAfter].status = BOOKED;
            strcpy(tiket[idCari - 1].kursi, selectedSeat);
            printf("Nomor kursi berhasil diubah\n");
            isAvailableSeat = true;
          }
        }
      }
      while(!isAvailableSeat && strcmp(answer, "n") == 0);
      
      break;
    default:
      printf("Pilihan tidak valid\n");
      break;
  }
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
  
  char *choice = "y";
  
  if(idCari > 0 && idCari <= GLOBAL_TICKET_INDEX){
    if(strcmp(tiket[idCari - 1].createdBy, username) == 0){
      printf("Apakah anda yakin ingin membatalkan tiket ini? (y/n): ");
      scanf(" %c", &answer);
      
      if(strcmp(choice, "y") == 0){
        int idxKursi = searchKursi(kursi, jumlahKursi, tiket[idCari - 1].kursi);
        flight[tiket[idCari - 1].flight_id].kursi[idxKursi].status = AVAILABLE;
        tiket[idCari - 1].status = CANCELLED;
        printf("Tiket berhasil dibatalkan\n");
      }
    } else {
      printf("Anda tidak memiliki akses untuk membatalkan tiket ini\n");
    }
  }
  else{
    printf("ID Tiket tidak valid\n");
  }
}

void menu(struct Tiket *tiket, struct User *user, int totalUser, struct Kursi *kursi, int jumlahKursi, struct Flight *flight){
  int choice;
  printJudul("Selamat datang di tiket pemesanan pesawat");
  printf("1. Beli Tiket\n");
  printf("2. Tampilkan Semua Tiket Yang Dipesan Oleh Saya\n");
  printf("3. Cari Tiket\n");
  printf("4. Urutkan Tiket\n");
  printf("5. Edit Tiket (Jika ada kesalahan data!)\n");
  printf("6. Batalkan Tiket \n");
  printf("7. Logout\n");
  
  printf("Pilihan Kamu: ");
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
  struct Flight flight[MAX_FLIGHTS];
  
  int totalUser = openFile(user);
  
  while(1){
    if(!isLogout()){
      if(strcmp(username, ADMIN_USERNAME) == 0){
        adminMenu(flight, kursi);
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
