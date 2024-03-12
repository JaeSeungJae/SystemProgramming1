/////////////////////////////////////////////////////////////////////////////////
// File name  :	2018202018_simple_ls.c					       //
// Date       :  2023 / 03 / 24                                                //
// OS         :  Ubuntu 16.04 LTS 64bits                                       //
// Author     :  Yu Seung Jae                                                  //
// Student ID :  2018202018                                                    //
// --------------------------------------------------------------------------- //
// Title : System Programming Assignment #1-1                                  //
// Description : Linux 명령어 중 하나인 ls 명령어를 c언어를 통한 구현 및 기능  //
// 추가시킨 프로그램을 만들었습니다.                                           //
/////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <dirent.h> // using DIR, struct dirent
#include <stdlib.h>
#include <string.h> // strcasecmp ( sorting ascending )

/////////////////////////////////////////////////////////////////////////////////// ls                                                                          //
// ============================================================================//
// input : int -> name_list의 배열 수 카운트                                   //
//         char* -> 버블 정렬 내에서 값 교환 시에 사용                         //
//         char*[] -> d_name 저장용 배열                                       //
//         struch dirent* -> readdir함수 출력값을 저장하는 용도                //
//                                                                             //
// output : void                                                               //
//                                                                             //
// Purpose : DIR* 값을 입력받아 그 경로의 파일들을 출력해주는 함수             //
/////////////////////////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////////////////////////// main                                                                        //
// ============================================================================//
// input : int -> argument count                                               //
//         char*[] -> argument vector                                          //
//                                                                             //
// output : int -> 0 success                                                   //
//                                                                             //
// purpose : main                                                              //
/////////////////////////////////////////////////////////////////////////////////
int main (int argc, char *argv[]){
        DIR *dirp;
	if (argc == 1 || argv[1] == "."){ // if no argument or argument is "."
		dirp = opendir("."); // assign current directory
		ls(dirp);
	 }

	

	else {
	if (argc != 2){ // if arguments are more than 2
		printf("simple_ls: only one directory path can be processed\n");
		exit(1);
	}
        if ((dirp = opendir(argv[1])) == NULL){ // no directory
		printf("simple_ls: cannot access '%s' : No such directory\n", argv[1]);
		exit(1);
	}
        	dirp = opendir(argv[1]);
		ls(dirp);
	}


        closedir(dirp);
        return 0;
}

