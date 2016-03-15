#include <stdio.h>
#include <iostream>
#include <bits/stdc++.h>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

vector<string> fileName;
vector<string> directoryName;

int main(){
	system("ls ../../Images/Input > temp.txt");
	FILE *ftr=fopen("temp.txt","r");
	char name[2000];
	while(fscanf(ftr,"%s",&name)!=EOF){
		printf("%s->\n",name);
		string temp=name;
		fileName.push_back(temp);
	}

	for(int i=0;i<fileName.size();i++){

		//finished calling the first part of commands
		system("g++ -std=c++11 -ggdb `pkg-config --cflags opencv` -o opensoft1 opensoftGraphP1.cpp `pkg-config --libs opencv`");
		string command="./opensoft1 ../../Images/Input/"+fileName[i]+" "+fileName[i];
		char buf[1024];
		strcpy(buf,command.c_str());
		system(buf);
		//called the first command
	}

	system("ls > temp1.txt");
	ftr=fopen("temp1.txt","r");
	fileName.clear();

	while(fscanf(ftr,"%s",&name)!=EOF){
		printf("%s->\n",name);
		string temp=name;
		fileName.push_back(temp);
	}

	for(int i=0;i<fileName.size();i++){
		if(fileName[i][0]=='t' && fileName[i][1]=='e' && fileName[i][2]=='s' && fileName[i][3]=='t' && fileName[i][4]=='_'){
			system("g++ -std=c++11 -ggdb `pkg-config --cflags opencv` -o opensoft2 opensoftGraphP2.cpp `pkg-config --libs opencv`");
			string command="ls "+fileName[i]+"> temp3.txt";
			char buf[1024];
			strcpy(buf,command.c_str());
			system(buf);
			vector<string> files;
			ftr=fopen("temp3.txt","r");
			char name[1024];
			while(fscanf(ftr,"%s",&name)!=EOF){
				printf("%s->\n",name);
				string temp=name;
				files.push_back(temp);
			}

			for(int j=0;j<files.size();j++){
				cout<<files[j]<<"\n";
				if(files[j][0]=='g' && files[j][files[j].size()-1]=='g' && files[j][files[j].size()-2]=='p'){
					//now we have the image of the graph..now to get the data as well
					string temp;
					printf("herer\n");
					for(int k=0;k<files[j].size();k++){
						if(files[j][k]=='.'){
							break;
						}
						else{
							temp=temp+files[j][k];
						}
					}
					//splitting thr file name

					string command="./opensoft2 "+fileName[i]+"/"+files[j]+" "+fileName[i]+" "+temp+".txt";
					char buf[1024];
					strcpy(buf,command.c_str());
					system(buf);
					//running the second command too
				}
			}
		}
	}
}