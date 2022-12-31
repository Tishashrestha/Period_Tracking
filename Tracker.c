#include<stdio.h>
#include<windows.h>

void flush(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

//---------------------------------------------------------

struct AddNote{
  int day;
  int month;
  int year;
  char note[];
};
struct period_dates_read{
  int day, month, year;
};
struct pd{
	int m, d, y;
}s;

//---------------------------------------------------------

void SetColorAndBackground(int ForgC, int BackC){
     WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);;
     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
     return;
}

//---------------------------------------------------------

//True if leap year
/*int isLeapYear( int y ){
    return(y % 400 == 0) || ((y % 4 == 0) && (y % 100 != 0));
}*/
int isLeapYear(int y){
    if((y % 400 == 0) || ((y % 4 == 0) && (y % 100 != 0)))
        return 0;

    else
        return 1;
}
//---------------------------------------------------------

//The number of leap year
int leapYears( int y ){
    return y/4 - y/100 + y/400;
}

//---------------------------------------------------------

//The number of days since the beginning of the year
int todayOf( int y, int m, int d) {
    static int DayOfMonth[] = { -1,0,31,59,90,120,151,181,212,243,273,304,334};
    return DayOfMonth[m] + d + ((m>2 && isLeapYear(y)==0)? 1 : 0);
}

//---------------------------------------------------------

//Total number of days
long days( int y, int m, int d){
    int lastYear;
    lastYear = y - 1;
    return 365L * lastYear + leapYears(lastYear) + todayOf(y,m,d);
}

//---------------------------------------------------------

void periodTracker(){
	int new, r, re, i, tm;
    redo_date:
    printf("PERIOD TRACKER\n\n");
    printf("Enter the last date of your period (dd mm yyyy)\n");
    scanf("%d %d %d", &s.d, &s.m, &s.y);

    r = leap_check(s.y);
    if (r == 0){
        if(s.m<=0 || s.m>12){
            printf("Month Not Valid!!");
            goto redo_date;
        }
        if(s.d<=0 || s.d>31){
            printf("Date Not Valid");
            goto redo_date;
        }
        if(s.m == 2){
            if(s.d>28){
                printf("The Month only has 28 days");
                goto redo_date;
            }
        }
        else if(s.m == 4 || s.m == 6 || s.m == 9 || s.m == 11){
            if(s.d>30){
                printf("The Month Contains only 30 days");
                goto redo_date;
            }
        }
    }
    else{
        if(s.m<0 || s.m>12){
            printf("Month Not Valid!!");
            goto redo_date;
        }
        if(s.d<0 || s.d>31){
            printf("Date Not Valid");
            goto redo_date;
        }
        if(s.m == 2){
            if(s.d>29){
                printf("The Month only has 29 days (leap Year)");
                goto redo_date;
            }
        }
        else if(s.m == 4 || s.m == 6 || s.m == 9 || s.m == 11){
            if(s.d>30){
                printf("The Month Contains only 30 days");
                goto redo_date;
            }
        }
    }
    
    system("cls");
    new = s.d;
    FILE *period_file = fopen("period_dates.txt", "w");
    
    printf("\nHere is your predicted period date for the next month\n\n\n");
    for(i=1; i<=1; i++){
        new += 27;
        re = leap_check(s.y);
        if(re==1){
            if(s.m == 2){
                if(new > 28){
                    new = new - 28;
                    s.m++;
                }
                printf("%02d/%02d/%04d\n", new,s.m,s.y);
            }
            else if(s.m == 4 || s.m == 6 || s.m == 9 || s.m == 11){
                if(new > 30){
                    new = new - 30;
                    s.m++;
                    if(s.m > 12){
                        s.m = 1;
                        s.y++;
                    }
                }
                printf("%02d/%02d/%04d\n", new,s.m,s.y);
            }
            else{
                if(new > 31){
                    new = new - 31;
                    s.m++;
                    if(s.m > 12){
                        s.m = 1;
                        s.y++;
                    }
                }
                printf("%02d/%02d/%04d\n", new,s.m,s.y);
            }
        }
        else{
            if(s.m == 2){
                if(new > 29){
                    new = new - 28;
                    s.m++;
                }
                printf("%02d/%02d/%04d\n", new,s.m,s.y);
            }
            else if(s.m == 4 || s.m == 6 || s.m == 9 || s.m == 11){
                if(new > 30){
                    new = new - 30;
                    s.m++;
                    if(s.m > 12){
                        s.m = 1;
                        s.y++;
                    }
                }
                printf("%02d/%02d/%04d\n", new,s.m,s.y);
            }
            else{
                if(new > 31){
                    new = new - 31;
                    s.m++;
                    if(s.m > 12){
                        s.m = 1;
                        s.y++;
                    }
                }
                printf("%02d/%02d/%04d\n", new,s.m,s.y);
            }
        }
      fprintf(period_file,"%d %d %d\n", new, s.m, s.y);
    }
    fclose(period_file);
}

int leap_check(int year){
    if((year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0))){
        return 0;
    }
    else{
        return 1;
    }
}
//---------------------------------------------------------

//display calendar at m y
void calendar(int y, int m){
    FILE *fp;
	struct AddNote note, *notes;
    int len, j, hasNote = 0;
    char choice;
    
	const char *NameOfMonth[] = { NULL,"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
	char Week[] = "Su   Mo   Tu   We   Th   Fr   Sa";
    int DayOfMonth[]={ -1,31,28,31,30,31,30,31,31,30,31,30,31 };
    
	int weekOfTopDay;
    int i,day;
    weekOfTopDay = days(y, m, 1) % 7;
    
    fp = fopen("note.bin", "rb");
    if (fp == NULL) {
      printf("Couldn't read notes\n");
    }
    
    len = 0;
    while(fread(&note, sizeof(struct AddNote), 1, fp)) {		//fread() returns a numerical, integer value which is TRUE for while condition
      if (note.year == y && note.month == m) {
        len++;
      }
    }
    rewind(fp);
    
	j = 0;
    notes = (struct AddNote*) malloc (sizeof(struct AddNote) * len);
    while(fread(&note, sizeof(struct AddNote), 1, fp)) {
      if (note.year == y && note.month == m) {
        notes[j] = note;
        j++;
      }
    }
    fclose(fp);
	
	if(isLeapYear(y)==0)
        DayOfMonth[2] = 29;
        
	printf("\n     %s %d\n%s\n", NameOfMonth[m], y, Week);
	
	for(i=0; i<weekOfTopDay; i++){			//Space before 1
        printf("     ");
	}
	
	int count = 0;
    FILE *period_color = fopen("period_dates.txt", "r");
    struct period_dates_read temp;
    
	while(fscanf(period_color, "%d %d %d\n", &temp.day, &temp.month, &temp.year) != EOF){
      count++;
    }
        
    struct period_dates_read per[count];
    rewind(period_color);
    
	int x = 0;
    while(fscanf(period_color, "%d %d %d\n", &per[x].day,&per[x].month, &per[x].year) != EOF){
      x++;
    }
    fclose(period_color);
    
    for(i=weekOfTopDay,day=1;day <= DayOfMonth[m];i++,day++){
		hasNote = 0; 
        for (j = 0; j < 31; j++) {
          if ((per[j].day == day && per[j].month == m && per[j].year == y) /*|| (notes[j].day == day)*/) {
          	SetColorAndBackground(12,0);
            printf("%2d   ", day);
            SetColorAndBackground(7,0);
            hasNote = 1;
            break;
          }
          else if (notes[j].day == day) {
            SetColorAndBackground(10,0);
            printf("%2d   ", day);
            SetColorAndBackground(7,0);
            hasNote = 1;
            break;
          }
          else{
          	break;
		  }
        }
        
        if (hasNote == 0) {
            printf("%2d   ",day);
        }
        if(i % 7 == 6){
            printf("\n");
    	}
    	
	}
		printf("\nEnter 's' to view notes in this month.\n");
		scanf("\n%c", &choice);
		fflush(stdin);
	    if (choice == 's') {
	      printf("Here are list of notes for %d %d\n", m, y);
	      for (j = 0; j < len; j++) {
	        printf("%d: %s\n", notes[j].day, notes[j]);
	      }
	    }
		else {
	      return;
	    }
	return;
}

//---------------------------------------------------------

int main(int argc, char* argv[]){
    int year, month, day;
    char choice, choice101;
    struct AddNote note;
    
	FILE *fp = fopen("note.bin", "a+b");
    //fclose(fp);
    
    do{
    	printf("WELCOME TO PERIOD CALENDAR\n\n");
	    printf("1. Print calendar of a month\n");
	    printf("2. Add Note\n");
	    printf("3. Track your period\n");
	    printf("4. Exit\n\n");
	    printf("Enter your choice: ");
	    scanf("\n%c", &choice);
	    system("cls");
	    switch(choice) {
        case '1':
        	enter:
        	printf("Enter the month and year: ");
	        scanf("%d %d", &month, &year);
	        fflush(stdin);
			
			if ((month<1) || (month>12)){
				printf("Invalid month. Try again.\n");
				goto enter;
			}
			else{
				system("cls");
				do{
		            calendar(year, month);
					printf("\n\nThe highlighted color is your next predicted period dates.\n\n");
					printf("Press 'p' to go to previous month\nPress 'n' to go to next month\nPress any other key to continue\n");
		            printf("\n\nChoice: ");
		            scanf("%c", &choice101);
		            fflush(stdin);
		            
		            if(choice101 == 'n'){
		                system("cls");
		                month++;
		                if(month > 12){
		                    month = 1;
		                    year++;
		                }
		            }
		            else if(choice101 == 'p'){
		                system("cls");
		                month--;
		                if(month < 1){
		                    month = 12;
		                    year--;
		                }
		            }
		            else{
		                choice101 = 0;
		            }
	          	}while(choice101 != 0); 
		        break;
			}
        case '2':
	        printf("Enter the day, month and year: ");
	        scanf("%d %d %d", &note.day, &note.month, &note.year);
	        fflush(stdin);
	        printf("Enter the note: ");
	        fgets(note.note, 255, stdin);
	        fp = fopen("note.bin", "a+b");
	        if (fp == NULL) {
	          printf("File note.bin can not be opened\n");
	          exit(1);
	        }
	        fwrite(&note, sizeof(struct AddNote), 1, fp);
	        printf("Note added sucessfully\n");
	        fclose(fp);
	        break;
	    case '3':
			periodTracker();
			break;
	    case '4':
	        printf("Bye!");
	        exit(0);
	        break;
	    default:
	        printf("Not a valid option\n");
	        break;
      }
    }while(choice!=0);
    return 0;
}
