///////////////////////////////////////////////////////////////////////////////////////////
// File name  : 2018202018_spls_advanced.c												 //
// Date 	  : 2023/04/05																 //
// Os         : Ubuntu 16.04 LTS 64bits													 //
// Authors    : Yu Seung Jae													         //
// Student ID : 2018202018 																 //
//---------------------------------------------------------------------------------------//
// Title : System Programming Assignment #1-2                                            //
// Description : 저번주에 구현한 ls 명령어에 옵션과 예외 처리를 추가하고 파일의 절대경로를 추가하여       //
// 출력하는 프로그램 																		 //
///////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <dirent.h> // using DIR, struct dirent
#include <stdlib.h>
#include <string.h> // strcasecmp ( sorting ascending )
#include <unistd.h> // linux function
#include <pwd.h> // struct passwd
#include <grp.h> // struct group
#include <sys/stat.h> // struct stat
#include <sys/types.h> // struct stat, passwd, ..
#include <time.h> // struct tm
///////////////////////////////////////////////////////////////////////////////////////////
// file_check																			 //
// ======================================================================================//
// Input : struct stat -> to use st_mode												 //
// Output : void																		 //
// Purpose : checking file type															 //
///////////////////////////////////////////////////////////////////////////////////////////
void file_check(struct stat buf){
	char f;
	if (S_ISDIR(buf.st_mode)){ // directory
		f = 'd';
	}
	else if (S_ISLNK(buf.st_mode)){ // link
		f = 'l';
	}
	else if (S_ISCHR(buf.st_mode)){ // character special
		f = 'c';
	}
	else if (S_ISBLK(buf.st_mode)){ // block special
		f = 'b';
	}
	else if (S_ISSOCK(buf.st_mode)){ // socket
		f = 's';
	}
	else if (S_ISFIFO(buf.st_mode)){ // FIFO
		f = 'P';
	}
	else if (S_ISREG(buf.st_mode)){ // regular
		f = '-';
	}
	printf("%c", f);
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////
// file_check_																			 //
// ======================================================================================//
// Input : struct stat -> to use st_mode												 //
// Output : char -> 'd' = directory, 'l' = symbolic link, ... 'n' = no exist file      	 //
// Purpose : checking file type and return												 //
///////////////////////////////////////////////////////////////////////////////////////////
char file_check_(struct stat buf){
	char f;
	if (S_ISDIR(buf.st_mode)){ // directory
		f = 'd';
	}
	else if (S_ISLNK(buf.st_mode)){ // link
		f = 'l';
	}
	else if (S_ISCHR(buf.st_mode)){ // character special
		f = 'c';
	}
	else if (S_ISBLK(buf.st_mode)){ // block special
		f = 'b';
	}
	else if (S_ISSOCK(buf.st_mode)){ // socket
		f = 's';
	}
	else if (S_ISFIFO(buf.st_mode)){  // FIFO
		f = 'P';
	}
	else if (S_ISREG(buf.st_mode)){ // regular
		f = '-';
	}
	else { // no exist file
		f = 'n';
	}
	return f;
}
///////////////////////////////////////////////////////////////////////////////////////////
// file_permission																		 //
// ======================================================================================//
// Input : struct stat -> to use st_mode												 //
// Output : void																		 //
// Purpose : checking file permission													 //
///////////////////////////////////////////////////////////////////////////////////////////
void file_permission(struct stat buf){
	char perm[12];
	if (buf.st_mode & S_IRUSR){ // whether read permission to user is or not
		perm[0] = 'r';
	}
	else {
		perm[0] = '-';
	}
	if (buf.st_mode & S_IWUSR) { // whether write permission to user is or not
		perm[1] = 'w';
	}
	else {
		perm[1] = '-';
	}
	if (buf.st_mode & S_IXUSR) { // whether execute permission to user is or not
		perm[2] = 'x';	
	}
	else {
		perm[2] = '-';
	}
	if (buf.st_mode & S_IRGRP) { // whether read permission to group is or not
		perm[3] = 'r';
	}
	else {
		perm[3] = '-';
	}
	if (buf.st_mode & S_IWGRP) { // whether write permission to group is or not
		perm[4] = 'w';
	}
	else {
		perm[4] = '-';
	}
	if (buf.st_mode & S_IXGRP) { // whether execute permission to group is or not
		perm[5] = 'x';
	}
	else {
		perm[5] = '-';
	}
	if (buf.st_mode & S_IROTH) { // whether read permission to others is or not
		perm[6] = 'r';
	}
	else {
		perm[6] = '-';
	}
	if (buf.st_mode & S_IWOTH) { // whether write permission to others is or not
		perm[7] = 'w';
	}
	else {
		perm[7] = '-';
	}
	if (buf.st_mode & S_IXOTH) { // whether execute permission to others is or not
		perm[8] = 'x';
	}
	else {
		perm[8] = '-';
	}
	for (int i = 0; i < 9; i++){ // print permission 
		printf("%c", perm[i]);
	}
	printf("\t");
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////
// file_check																			 //
// ======================================================================================//
// Input : struct stat -> to use st_nlink												 //
// Output : void																		 //
// Purpose : checking how many are links of file										 //
///////////////////////////////////////////////////////////////////////////////////////////
void file_link(struct stat buf){
	unsigned long link = 0;
	link = buf.st_nlink; // the number of link
	printf("%lu  ", link);
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////
// file_owner_name																		 //
// ======================================================================================//
// Input : struct stat -> to use st_uid												 	 //
// Output : void																		 //
// Purpose : checking file user id														 //
///////////////////////////////////////////////////////////////////////////////////////////
void file_owner_name(struct stat buf){
	char* owner_name;
	struct passwd *o; // struct passwd to get pw_name
	o = getpwuid(buf.st_uid);
	owner_name = o->pw_name;
	printf("%s ", owner_name); 
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////
// file_group_name																		 //
// ======================================================================================//
// Input : struct stat -> to use st_gid												 	 //
// Output : void																		 //
// Purpose : checking file group id														 //
///////////////////////////////////////////////////////////////////////////////////////////
void file_group_name(struct stat buf){
	char* group_name;
	struct group *o; // struct group to get gr_name
	o = getgrgid(buf.st_gid);
	group_name = o->gr_name;
	printf("%s ", group_name);
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////
// file_size																			 //
// ======================================================================================//
// Input : struct stat -> to use st_size												 //
// Output : void																		 //
// Purpose : checking file size 												         //
///////////////////////////////////////////////////////////////////////////////////////////
void file_size(struct stat buf){
	off_t size = 0;
	size = buf.st_size; //file size
	printf("%ld  \t", size);
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////
// file_time																			 //
// ======================================================================================//
// Input : struct stat -> to use st_mtime												 //
// Output : void																		 //
// Purpose : checking file's modified time												 //
///////////////////////////////////////////////////////////////////////////////////////////
void file_time(struct stat buf){
	char* Mon[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"}; // 0 ~ 11
	struct tm *o; // struct tm to get st_mtime
	const time_t *mtime = &buf.st_mtime;
	o = localtime(mtime);
	int Mon_num = o->tm_mon;
	int day = o->tm_mday;
	int hour = o->tm_hour;
	int min = o->tm_min;
	printf("%s  %d %d:%d  ", Mon[Mon_num], day, hour, min); 
	return;
}
///////////////////////////////////////////////////////////////////////////////////////////
// print_ls																		         //
// ======================================================================================//
// Input : struct stat -> checking file information									     //
// Output : void																		 //
// Purpose : print file informations													 //
///////////////////////////////////////////////////////////////////////////////////////////
void print_ls(struct stat buf){ // select file stat and print file information using file stat
		file_check(buf);
        file_permission(buf);
        file_link(buf);
        file_owner_name(buf);
        file_group_name(buf);
        file_size(buf);
        file_time(buf);
	return;
}

///////////////////////////////////////////////////////////////////////////////////////////
// ls																		 			 //
// ======================================================================================//
// Input : DIR* -> which file selected												     //
// Output : void																		 //
// Purpose : 디렉토리 내의 파일들 이름 출력 / 파일 이름 출력									  //
///////////////////////////////////////////////////////////////////////////////////////////
void ls(DIR *a) {
	int count = 0; // use to compare string
	char *temp; // use to sort
	char *name_list[1024]; // use to sort
	struct dirent *dir;
	
	while ((dir = readdir(a)) != NULL){ // until all variable
	if (dir->d_name[0] == '.'){
		continue;
	}
	
		name_list[count] = dir->d_name;
		count++;  // counting only non-hidden files
	}
		for (int i = 0; i < count-1; i++){ // sort name_list using bubble sorting
		for (int j = 0; j < count-1-i; j++){
		if (strcasecmp(name_list[j], name_list[j+1]) > 0){
		temp = name_list[j];
		name_list[j] = name_list[j+1];
		name_list[j+1] = temp;
		}
	}
}
	for (int i = 0; i < count; i++){
	printf("%s\n", name_list[i]);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////
// ls_a																		 			 //
// ======================================================================================//
// Input : DIR* -> which file selected												     //
// Output : void																		 //
// Purpose : 디렉토리 내의 파일들 이름 출력 / 파일 이름 출력(숨김파일 포함)							 //
///////////////////////////////////////////////////////////////////////////////////////////
void ls_a(DIR *a) {
        int count = 0; // use to compare string
        char *temp1; // use to sort
		char *temp2;
        char *name_list[1024]; // use to sort
        struct dirent *dir;

        while ((dir = readdir(a)) != NULL){ // until all variable
                name_list[count] = dir->d_name;
                count++;  // counting only non-hidden files
        }
                for (int i = 0; i < count-1; i++){ // sort name_list using bubble sorting
                	for (int j = 0; j < count-1-i; j++){ // ignore hidden icon '.'
						if (name_list[j][0] == '.'){ // if hidden file, compare second characater
							temp1 = name_list[j] + 1;
						}
						else {
							temp1 = name_list[j];
						}
						if (name_list[j+1][0] == '.'){
							temp2 = name_list[j+1] + 1;
						}
						else {
							temp2 = name_list[j+1];
						}
						if (strcasecmp(temp1, temp2) > 0){
							char* temp = name_list[j+1];
							name_list[j+1] = name_list[j];
							name_list[j] = temp;
						}
        			}
				}
        for (int i = 0; i < count; i++){
        printf("%s\n", name_list[i]);
        }
}
///////////////////////////////////////////////////////////////////////////////////////////
// ls_l																	 			     //
// ======================================================================================//
// Input : DIR* -> which file selected												     //
// Output : void																		 //
// Purpose :디렉토리 내의 파일들의 정보 출력 / 경로 출력										 //
// 			파일의 정보 출력				 												//
///////////////////////////////////////////////////////////////////////////////////////////
void ls_l(DIR *a, char* argv) {
        int count = 0; // use to compare string
        char *temp; // use to sort
        char *name_list[1024]; // use to sort
        struct dirent *dir; // dirent
		char buf_link[1024][100]; // checking if this file is symbolic link
		char ch; // file checking
	//struct stat buf[1024];
	struct stat buf[100]; // file information array
	char cwd[1024]; // current working directory
	int blocksize = 0; // blocksize initialize
	getcwd(cwd , sizeof(cwd));
        while ((dir = readdir(a)) != NULL){ // until all variable
        	if (dir->d_name[0] == '.'){ // ignore hidden file
                continue;
        	}	
		else {
                name_list[count] = dir->d_name;
                count++;  // counting only non-hidden files
        	}
}
                for (int i = 0; i < count-1; i++){ // sort name_list using bubble sorting
                for (int j = 0; j < count-1-i; j++){
                if (strcasecmp(name_list[j], name_list[j+1]) > 0){
                temp = name_list[j];
                name_list[j] = name_list[j+1];
                name_list[j+1] = temp;
                }
        }
}
	char argv_orig[1000]; // not to modify argv array;
	strcpy(argv_orig, argv); // copy argv to argv_orig
	if (argv[0] == '/'){
		printf("Directory Path : %s\n", argv);
	}
	else if (argv[0] == '.' && argv[1] == '/'){
		printf("Directory Path : %s\n", cwd);
	}
	else if (argv[0] == '.' && argv[1] == ' '){
		printf("Directory path : %s\n", cwd);
	}
	else {
		printf("Directory path : %s/%s\n", cwd, argv);
	} // directory print
	strcat(argv_orig, "/"); // directory + /
	for (int i = 0; i < count; i++){
		strcpy(argv_orig, argv); // initialize continuously
		strcat(argv_orig, "/"); // directory + /
		lstat(strcat(argv_orig, name_list[i]), &buf[i]); // lstat((directory + / + file name[i]), &buf[i])
		blocksize += buf[i].st_blocks; // blocksize add
		ch = file_check_(buf[i]); // file type checking
		if (ch == 'l'){ // if symbolic link
			int trash = readlink(argv_orig, buf_link[i], 100); // trash is not used, save origin file name of link in buf_link
			//printf("%s\n", buf_link[i]);
		}
		memset(argv_orig, 0, sizeof(argv)); // initialize strcat
	}
	printf("total : %d\n", blocksize/2); // total blocksize print
	for (int i = 0; i < count; i++){
		print_ls(buf[i]); // print file information
		printf("%s", name_list[i]); // print file name
		if (file_check_(buf[i]) == 'l'){ // if file is symbolic link
			printf(" -> %s", buf_link[i]); // show origin file name of symbolic link
		}
		printf("\n");
	}
}
///////////////////////////////////////////////////////////////////////////////////////////
// ls_al																 			     //
// ======================================================================================//
// Input : DIR* -> which file selected												     //
// Output : void																		 //
// Purpose :디렉토리 내의 파일들의 정보 출력 / 경로 출력										 //
// 			파일의 정보 출력 (숨김파일 포함) 												   //
///////////////////////////////////////////////////////////////////////////////////////////
void ls_al(DIR *a, char *argv) {
		int count = 0; // use to compare string
        char *temp; // use to sort
        char *name_list[1024]; // use to sort
        struct dirent *dir;
		char *name_list_plus[1024];
		char *temp1, *temp2;
		char buf_link[1024][100];
		char ch;
	//struct stat buf[1024];
	struct stat buf[100];
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	int blocksize = 0;
        while ((dir = readdir(a)) != NULL){ // until all variable
                name_list[count] = dir->d_name;
                count++; // contain hidden file
		}
                for (int i = 0; i < count-1; i++){ // sort name_list using bubble sorting
                for (int j = 0; j < count-1-i; j++){
                if (name_list[j][0] == '.'){ // hidden file, consider second file character
							temp1 = name_list[j] + 1;
						}
						else {
							temp1 = name_list[j];
						}
						if (name_list[j+1][0] == '.'){
							temp2 = name_list[j+1] + 1;
						}
						else {
							temp2 = name_list[j+1];
						}
						if (strcasecmp(temp1, temp2) > 0){
							char* temp = name_list[j+1];
							name_list[j+1] = name_list[j];
							name_list[j] = temp;
						}
        				}
				}
	char argv_orig[1000];
	strcpy(argv_orig, argv);
	if (argv[0] == '/'){
		printf("Directory Path : %s\n", argv);
	}
	else if (argv[0] == '.' && argv[1] == '/'){
		printf("Directory Path : %s\n", cwd);
	}
	else if (argv[0] == '.' && argv[1] == ' '){
		printf("Directory path : %s\n", cwd);
	}
	else {
		printf("Directory path : %s/%s\n", cwd, argv);
	}
	strcat(argv_orig, "/"); // directory + /
	for (int i = 0; i < count; i++){
		strcpy(argv_orig, argv);
		strcat(argv_orig, "/");
		lstat(strcat(argv_orig, name_list[i]), &buf[i]); // lstat((directory + / + file name[i]), &buf[i])
		blocksize += buf[i].st_blocks; // blocksize add
		ch = file_check_(buf[i]); // checking file type
		if (ch == 'l'){
			int trash = readlink(argv_orig, buf_link[i], 100); // trash is not used, save origin file name of link in buf_link
		}
		memset(argv_orig, 0, sizeof(argv)); // initialize continuously
	}
	printf("total : %d\n", blocksize/2); // total blocksize print
	for (int i = 0; i < count; i++){
		print_ls(buf[i]); // print file information
		printf("%s", name_list[i]); // print file name
		if (file_check_(buf[i]) == 'l'){ // if file type is symbolic link
			printf(" -> %s", buf_link[i]); 
		}
		printf("\n");
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
// main															 			             //
// ======================================================================================//
// Input : int -> argument count, char** -> argument vector								 //
// Output : void																		 //
// Purpose : 프로그램의 인자를 확인하여 flag를 정의하고 인자의 갯수에 따라 각기 다른 실행			    //
///////////////////////////////////////////////////////////////////////////////////////////
int main (int argc, char **argv){
	int aflag = 0;
        int lflag = 0;
        int c = 0;
		char cwd[1024];
		getcwd(cwd, sizeof(cwd)); // get current working directory using no argument of directory
        while ((c = getopt(argc, argv, "al")) != -1) // flag setting
        {
                switch(c)
                {
                        case 'a': // -a
                                aflag = 1;
                                break;
                        case 'l': // -l
                                lflag = 1;
                                break;
                        case '?': // else
                                printf("Error : command is not exist\n");
                                exit(0);
                }
        }
		//printf("%d %d\n", aflag, lflag);
        DIR *dirp;
		struct stat st[999]; // temporarily stat
		char checking; // file_check_
	if (argc == 1){ // if no argument or argument is "."
		if ((dirp = opendir(".")) == NULL){ // only "ls cwd";
			lstat(argv[1], &st[0]); 
			file_check(st[0]); // file type check
			printf("cannot access %s: No such file or directory\n", argv[1]);
			exit(0);
		} // assign current directory
		ls(dirp);
	}
	else if (argc == 2){ // only an option or a file
		if ((argv[1][0]) == '-'){ // "ls argv[1]" or "ls -(options)"
			dirp = opendir(".");
		}
		//dirp = opendir(argv[1]);
		else //(dirp == NULL){
		{
			dirp = opendir(argv[1]);
			lstat(argv[1], &st[0]);
			checking = file_check_(st[0]);
			if (dirp == NULL){
				if (checking == 'n'){ // non exist file
					printf("cannot access %s: No such file or directory\n", argv[1]);
					exit(0);
				}
				else {
					printf("%s\n", argv[1]); // ls (non-directory file)
					return 0;
				}
			}
		}
		if (aflag == 0 && lflag == 0){ // ls
			dirp = opendir(argv[1]); // file name
			ls(dirp);
		}
		else if (aflag == 1 && lflag == 0){ // ls -a
			dirp = opendir("."); // cwd
                ls_a(dirp);
        }
        else if (aflag == 0 && lflag == 1){ // ls -l
			dirp = opendir("."); // cwd
                ls_l(dirp, cwd);
        }
        else { // ls -la , -al, -a -l, ..
			dirp = opendir("."); // cwd
                ls_al(dirp, cwd);
		}
	}
	
    else if (argc > 2){
		int oc = 0; // how many options
		int arr[1000]; // marking file type 
						// 1 : non-directory 
						// 2 : non-exist file
		char* temtem; // sort argument 
		
		for (int i = 1; i < argc; i++){
			if (argv[i][0] == '-'){
				oc++;
			}
		}
		for (int i = 1+oc; i < argc-1; i++){
			for (int j = 1+oc; j < argc-1-i; j++){
				if (strcasecmp(argv[j], argv[j + 1]) > 0){
					temtem = argv[j];
					argv[j] = argv[j+1];
					argv[j+1] = temtem;
				}
			}
		}
		for (int i = 1+oc; i < argc; i++){ // first, checking non exist files
			lstat(argv[i], &st[i]);
			checking = file_check_(st[i]);
			if (checking == 'n'){ // no exist
				printf("cannot access '%s' : No such file or directory\n", argv[i]);
				arr[i] = 2;
			}
		}
		for (int i = 1+oc; i < argc; i++){
			lstat(argv[i], &st[i]);
			checking = file_check_(st[i]);
			if (checking == 'n'){
				continue;
			}
			else if (checking != 'd'){ // second, print non directory files
				//printf("cannot access '%s': No such file or directory\n", argv[i]);
				arr[i] = 1;
				if (aflag == 0 && lflag == 0){
					printf("%s\n", argv[i]);
				}
				else if (aflag == 1 && lflag == 0){
					printf("%s\n", argv[i]);
				}
				else if (aflag == 0 && lflag == 1){
					lstat(argv[i], &st[i]);
					print_ls(st[i]);
					printf("%s\n", argv[i]);
				}
				else {
					lstat(argv[i], &st[i]);
					print_ls(st[i]);
					printf("%s\n", argv[i]);
				}
			}
		}
			// else, directory path print
        if (aflag == 0 && lflag == 0){ // ls
			for (int i = 1; i < argc; i++){
				if (arr[i] == 1 || arr[i] == 2){
					continue;
				}
				else {
					dirp = opendir(argv[i]);
					if (argv[i] == ".\0"){
						printf("%s : \n", cwd);
					}
					else{
						printf("%s : \n", argv[i]);            		
					}
					ls(dirp);
				}
			}
        }
        else if (aflag == 1 && lflag == 0){ // ls -a
			for (int i = 2; i < argc; i++){
				if (arr[i] == 1 || arr[i] == 2){
					continue;
				}
				else{
					if (argc > 3){
						printf("%s : \n", argv[i]);
					}
					dirp = opendir(argv[i]);
            		ls_a(dirp);
				}
			}
        }
        else if (aflag == 0 && lflag == 1){ // ls -l
			for (int i = 2; i < argc; i++){
				if (arr[i] ==  1 || arr[i] == 2){
					continue;
				}
				else {
					dirp = opendir(argv[i]);
					ls_l(dirp, argv[i]);
				}
			}			
		}
        else if (aflag == 1 && lflag == 1){ // ls -al
			// if (arr[i] == 1){
			// 	continue;
			// }
			//else {
				if ((oc + 1) == argc){
					dirp = opendir(cwd);
					ls_al(dirp, cwd);
				}
				if (oc == 1){
					for (int i = 2; i < argc; i++){
						if (arr[i] == 1 || arr[i] == 2){
							continue;
						}
						else {
							dirp = opendir(argv[i]);
							ls_al(dirp, argv[i]);
						}
					}
				}
				else if (oc >= 2){
					for (int i = (1+oc); i < argc; i++){
						if (arr[i] == 1 || arr[i] == 2){
							continue;
						}
						else{
							dirp = opendir(argv[i]);
            				ls_al(dirp, argv[i]);
						}
					}
				}
				else {
					printf("Wrong options\n");
				}
			//}
      //}
		}
	}

        closedir(dirp);
        return 0;
}
