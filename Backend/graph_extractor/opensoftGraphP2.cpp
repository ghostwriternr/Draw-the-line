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

Mat dst, cdst;
Mat src,src1;
int thresh = 120,thresh1=130,thresh2=20;
int ffg[100000];
int parent[100001];
int childL[100001];
int visitedDFS[100001];
vector<int> connectedLines;
vector<int> linesNeighbour[100001];
map<int,pair<pair<pair<int,int>,int>,int> > mappedLines;
bool alreadyFunced=false;
int countImages=0;
int minx,miny,maxx,maxy;


void giveValues(Vec4i &res,pair<pair<pair<int,int>,int>,int> a){
	res[0]=a.first.first.first;
	res[1]=a.first.first.second;
	res[2]=a.first.second;
	res[3]=a.second;
	return ;
}


void cutout(int minx,int miny,int maxx,int maxy){
	/*vector<pair<int,int> > points;
	points.push_back(make_pair(minx,miny));
	points.push_back(make_pair(minx,maxy));
	points.push_back(make_pair(maxx,miny));
	points.push_back(make_pair(maxx,maxy));

	int left = ((double)maxx-minx)/10.0;
	int up = ((double)maxy-miny)/10.0;
	int fx,fy,height,width;
	int maxrows = src.rows,maxcols = src.cols;
	fx = (minx - left);
	fy = (miny - up);
	width = (maxx-minx+2*left);
	height = (maxy-miny+2*up);
	if( fx < 0)
		fx = 0;
	if(fy < 0)
		fy = 0;
	if((maxx+left) >= maxcols-1)
		width = maxcols-fx-1;
	if((maxy+up) >= maxrows-1)
		height = maxrows-fy-1;
	printf("Coordi-> %d %d %d %d\n",minx,miny,maxx,maxy);
	printf("cutout vals -> %d %d %d %d %d %d %d %d\n",fx+width,fy+height,maxcols,maxrows,width,height,left,up );
	printf("Axis cutout values->%d %d %d---\n",minx,maxy,maxx-minx);


	cv:: Rect myRect(fx/2.0,fy/2.0,width/2.0,height/2.0);
	cv:: Rect myRect1(minx/2.0,maxy/2.0 - 10,(maxx-minx)/2.0,20);
	cv::Mat imagecropped=src1(myRect);
	string name="graph_";
	cv::Mat imagecroppedAxes=src1(myRect1);
	string name1="graph_axes_";
	name=name+to_string(countImages);
	name1=name1+to_string(countImages);
	imshow(name,imagecropped);
	imshow(name1,imagecroppedAxes);
	imwrite(name+".jpg",imagecropped);*/
}

void getImage(){
	for(int index=0;index<connectedLines.size();index++){
		printf("connectedLines %d---\n",connectedLines[index]);
	}
	printf("\n\n");
	printf("size--->%d\n",connectedLines.size());

	if(connectedLines.size()==2){
		//if dosent work delete this section and return
		printf("herer-->>>\n");
		int minx=INT_MAX;
		int maxx=-1;
		int miny=INT_MAX;
		int maxy=-1;
		//get the values
		for(int index=0;index<connectedLines.size();index++){
			Vec4i l;
			giveValues(l,mappedLines[connectedLines[index]]);
			minx=min(minx,min(l[0],l[2]));
			maxx=max(maxx,max(l[0],l[2]));
			maxy=max(maxy,max(l[1],l[3]));
			miny=min(miny,min(l[1],l[3]));
		}
		//qwe
		cutout(minx,miny,maxx,maxy);
		/*cout << "value x and y" << ((double)minx-maxx)/10 << " " << ((double)miny-maxy)/10 << endl;
		int left = ((double)maxx-minx)/50;
		int up = ((double)maxy-miny)/50;

		cv:: Rect myRect(minx-left,miny-up,maxx-minx+(2*left),maxy-miny+(2*up));
		cv::Mat imagecropped=src(myRect);
		string name="test";
		name=name+to_string(countImages);
		imshow(name,imagecropped);
		imwrite(name+".jpg",imagecropped);*/
		countImages++;
	}
	else{
		Vec4i l0;
		Vec4i l1;
		Vec4i l2;
		Vec4i l3;
		if(connectedLines.size()==3){
			giveValues(l0,mappedLines[connectedLines[0]]);
			giveValues(l1,mappedLines[connectedLines[1]]);
			giveValues(l2,mappedLines[connectedLines[2]]);
			int horizc=0;
			int vertc=0;
			vector<Vec4i> hLines;
			vector<Vec4i> vLines;
			printf("%d--%d--%d--%d\n",l0[0],l0[1],l0[2],l0[3]);
			printf("%d--%d--%d--%d\n",l1[0],l1[1],l1[2],l1[3]);
			printf("%d--%d--%d--%d\n",l2[0],l2[1],l2[2],l2[3]);

			printf("size is 3---\n");
			if(abs(l0[0]-l0[2])<=10){
				vLines.push_back(l0);
			}
			else{
				hLines.push_back(l0);
			}
			if(abs(l1[0]-l1[2])<=10){
				vLines.push_back(l1);
			}
			else{
				hLines.push_back(l1);
			}
			if(abs(l2[0]-l2[2])<=10){
				vLines.push_back(l2);
			}
			else{
				hLines.push_back(l2);
			}
			printf("connected lines-->>><<-%d---%d\n",vLines.size(),hLines.size());

			if(vLines.size()==2 && hLines.size()==1){
				//qwe
				printf("herer--\n");
				int minx=min(min(min(min(min(hLines[0][0],hLines[0][2]),vLines[1][0]),vLines[1][2]),vLines[0][0]),vLines[0][2]);
				int miny=max(max(max(max(max(hLines[0][1],hLines[0][3]),vLines[1][1]),vLines[1][3]),vLines[0][1]),vLines[0][3]);
				int maxx=min(min(min(min(min(hLines[0][0],hLines[0][2]),vLines[1][0]),vLines[1][2]),vLines[0][0]),vLines[0][2]);
				int maxy=max(max(max(max(max(hLines[0][1],hLines[0][3]),vLines[1][1]),vLines[1][3]),vLines[0][1]),vLines[0][3]);
				cutout(minx,miny,maxx,maxy);
				// cout << "lol value x and y" << ((double)minx-maxx)/10 << " " << ((double)miny-maxy)/10 << endl;

				// int left = ((double)maxx-minx)/10;
				// int up = ((double)maxy-miny)/10;

				// cv:: Rect myRect(minx-left,miny-up,maxx-minx+(2*left),maxy-miny+(2*up));
				// cv::Mat imagecropped=src(myRect);
				// string name="test";
				// name=name+to_string(countImages);
				// imshow(name,imagecropped);
				// imwrite(name+".jpg",imagecropped);
				// countImages++;
			}
			else if(vLines.size()==1 && hLines.size()==2){
				printf("here---\n");
				Vec4i uhorl;
				Vec4i lhorl;
				if(hLines[0][1]>hLines[1][1]){
					lhorl=hLines[1];
					uhorl=hLines[0];
				}
				else{
					lhorl=hLines[0];
					uhorl=hLines[1];
				}
				Vec4i verl=vLines[0];
				printf("herer-->>");
				int minx=min(min(min(min(min(hLines[0][0],hLines[0][2]),hLines[1][0]),hLines[1][2]),vLines[0][0]),vLines[0][2]);
				int miny=max(max(max(max(max(hLines[0][1],hLines[0][3]),hLines[1][1]),hLines[1][3]),vLines[0][1]),vLines[0][3]);
				int maxx=min(min(min(min(min(hLines[0][0],hLines[0][2]),hLines[1][0]),hLines[1][2]),vLines[0][0]),vLines[0][2]);
				int maxy=max(max(max(max(max(hLines[0][1],hLines[0][3]),hLines[1][1]),hLines[1][3]),vLines[0][1]),vLines[0][3]);
				cutout(minx,miny,maxx,maxy);
					
				// cout << "value minx miny maxx maxy" << (double)lhorl[0]/10 << " " << (double)lhorl[1]/10 << " "  << (double)lhorl[2]/10 << " " << (double)lhorl[3]/10 << endl;
				// cv::Rect myRect(lhorl[0]-50,lhorl[1]-50,max(max(lhorl[2],lhorl[0])-min(lhorl[2],lhorl[0]),max(uhorl[2],uhorl[0])-min(uhorl[2],uhorl[0]))+50,max(verl[1],verl[3])-min(verl[3],verl[1])+50);
				// cv::Mat imagecropped=src(myRect);
				// string name="test";
				// name=name+to_string(countImages);
				// imshow(name,imagecropped);
				// imwrite(name+".jpg",imagecropped);
				// countImages++;
			}
		}
		else if(connectedLines.size()==4){
			vector<Vec4i> hLines;
			vector<Vec4i> vLines;
			giveValues(l0,mappedLines[connectedLines[0]]);
			giveValues(l1,mappedLines[connectedLines[1]]);
			giveValues(l2,mappedLines[connectedLines[2]]);
			giveValues(l3,mappedLines[connectedLines[3]]);
			printf("%d--%d--%d--%d\n",l0[0],l0[1],l0[2],l0[3]);
			printf("%d--%d--%d--%d\n",l1[0],l1[1],l1[2],l1[3]);
			printf("%d--%d--%d--%d\n",l2[0],l2[1],l2[2],l2[3]);
			printf("%d--%d--%d--%d\n",l3[0],l3[1],l3[2],l3[3]);
			if(abs(l0[0]-l0[2])<=10){
				vLines.push_back(l0);
			}
			else{
				hLines.push_back(l0);
			}
			if(abs(l1[0]-l1[2])<=10){
				vLines.push_back(l1);
			}
			else{
				hLines.push_back(l1);
			}
			if(abs(l2[0]-l2[2])<=10){
				vLines.push_back(l2);
			}
			else{
				hLines.push_back(l2);
			}
			if(abs(l3[0]-l3[2])<=10){
				vLines.push_back(l3);
			}
			else{
				hLines.push_back(l3);
			}
			//assert(vLines.size()==2 && hLines.size()==2);
			printf("%d--%d\n",hLines.size(),vLines.size());
			giveValues(l0,mappedLines[connectedLines[0]]);
			giveValues(l1,mappedLines[connectedLines[1]]);
			giveValues(l2,mappedLines[connectedLines[2]]);
			giveValues(l3,mappedLines[connectedLines[3]]);
			int minx=min(min(min(min(min(min(min(hLines[0][0],hLines[0][2]),hLines[1][0]),hLines[1][2]),vLines[0][0]),vLines[0][2]),vLines[1][0]),vLines[1][2]);
			int miny=min(min(min(min(min(min(min(hLines[0][1],hLines[0][3]),hLines[1][1]),hLines[1][3]),vLines[0][1]),vLines[0][3]),vLines[1][1]),vLines[1][3]);
			int maxx=max(max(max(max(max(max(max(hLines[0][0],hLines[0][2]),hLines[1][0]),hLines[1][2]),vLines[0][0]),vLines[0][2]),vLines[1][0]),vLines[1][2]);
			int maxy=max(max(max(max(max(max(max(hLines[0][1],hLines[0][3]),hLines[1][1]),hLines[1][3]),vLines[0][1]),vLines[0][3]),vLines[1][1]),vLines[1][3]);
			cutout(minx,miny,maxx,maxy);

			// cout << "lol1 value x and y" << ((double)minx-maxx)/10 << " " << ((double)miny-maxy)/10 << endl;
			// int left = ((double)maxx-minx)/50;
			// int up = ((double)maxy-miny)/50;

			// cv:: Rect myRect(minx-left,miny-up,maxx-minx+(2*left),maxy-miny+(2*up));
			// cv::Mat imagecropped=src(myRect);
			// string name="test";
			// name=name+to_string(countImages);
			// imshow(name,imagecropped);
			// imwrite(name+".jpg",imagecropped);
			// countImages++;
		}
		else if(connectedLines.size()>4){
			printf("herer-->>>\n");
			int minx=INT_MAX;
			int maxx=-1;
			int miny=INT_MAX;
			int maxy=-1;
			//get the values
			for(int index=0;index<connectedLines.size();index++){
				Vec4i l;
				giveValues(l,mappedLines[connectedLines[index]]);
				minx=min(minx,min(l[0],l[2]));
				maxx=max(maxx,max(l[0],l[2]));
				maxy=max(maxy,max(l[1],l[3]));
				miny=min(miny,min(l[1],l[3]));
			}
			//qwe
			cutout(minx,miny,maxx,maxy);
			/*cout << "lol2 value x and y" << ((double)minx-maxx)/10 << " " << ((double)miny-maxy)/10 << endl;
			int left = ((double)maxx-minx)/100;
			int up = ((double)maxy-miny)/100;

			cv:: Rect myRect(minx,miny,maxx-minx,maxy-miny);
			cv::Mat imagecropped=src(myRect);
			string name="test";
			name=name+to_string(countImages);
			imshow(name,imagecropped);
			imwrite(name+".jpg",imagecropped);*/
			countImages++;
		}
	}
}

void DFSLines(int u,int p){
	visitedDFS[u]=true;
	printf("int DFS->%d....",u);
	connectedLines.push_back(u);
	for(int index=0;index<linesNeighbour[u].size();index++){
		if(!visitedDFS[linesNeighbour[u][index]]){
			DFSLines(linesNeighbour[u][index],u);
		}
	}
}

int parentFind(int index){
	if(parent[index]!=index){
		index=parent[index];
	}
	return index;
}

void union1(int x,int y){
	int x1=parentFind(x);
	int y1=parentFind(y);
	printf("-->>>%d....%d\n",x1,y1);

	if(x1!=y1){
		if(childL[x1]>childL[y1]){
			childL[x1]+=childL[y1];
			parent[y1]=parent[x1];
		}
		else{
			childL[y1]+=childL[x1];
			parent[x1]=parent[y1];
		}
	}
}
vector<Vec4i> lines;
//mappingLineInt<Vec4i,int> mp1;
vector<Vec4i> linesPerpendicular[100001];
/*bool vectorcompare (pair<float,int> v,pair<float,int> w){
	if(v.first < (w.first) )
		return true; 
	else if(v.first == w.first){
		//if( (lines[v.second][0] < lines[w.second][0]) || (lines[v.second][1] < lines[w.second][1]))
		if((lines[v.second][0] == lines[v.second][2] && lines[w.second][0] == lines[w.second][2] && lines[v.second][0] > lines[w.second][0])
			|| (lines[v.second][1] == lines[v.second][3] && lines[w.second][1] == lines[w.second][3] && lines[v.second][1] < lines[w.second][1]) )
			return true;
		else
			return false;
	}
	else
		return false;
}*/

float dis(double x1,double y1,double x2,double y2){
	return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}
bool vectorcompare(pair<Vec4i,Vec4i> v,pair<Vec4i,Vec4i> w){
	Vec4i l1,l2,l3,l4;
	l1 = v.first;
	l2 = v.second;
	l3 = w.first;
	l4 = w.second;
	double d1,d2,d3,d4;
	d1 = dis(l1[0],l1[1],l1[2],l1[3]);
	d2 = dis(l2[0],l2[1],l2[2],l2[3]);
	d3 = dis(l3[0],l3[1],l3[2],l3[3]);
	d4 = dis(l4[0],l4[1],l4[2],l4[3]);
	if(d1 >= d3 && d2 >= d4)
		return true;
	else
		return false;
}


/** Do line segments (x1, y1)--(x2, y2) and (x3, y3)--(x4, y4) intersect? */
bool DoLineSegmentsIntersect(double x1, double y1, double x2, double y2,double x3, double y3, double x4, double y4) {
	//first line if vertical second is horizontal
	if(x4<x3){
		swap(x4,x3);
		swap(y3,y4);
	}
	if(y1>y2){
		swap(y1,y2);
		swap(x1,x2);
	}
	double dist1=0;
	double dist2=0;
	int thres1=10;
	if(x1<x3 && x1<x4){
		dist1=min(abs(x1-x3),abs(x1-x4));
		dist2=min(abs(y1-y3),abs(y2-y3));
		if(dist1*dist1+dist2*dist2<=thres1){
			return true;
		}
		else{
			return false;
		}
	}
	else if(x3<x1 && x1<x4){
		if(y3<y1){
			dist1=abs(y1-y3);
			if(dist1<=thres1){
				return true;
			}
			else{
				return false;
			}
		}
		else if(y3>y1 && y3<=y2){
			return true;
		}
		else{
			dist1=abs(y3-y2);
			if(dist1<=thres1){
				return true;
			}
			else{
				return false;
			}
		}
	}
	else if(x1>=x3 && x1>=x4){
		dist1=min(abs(x1-x3),abs(x1-x4));
		dist2=min(abs(y1-y3),abs(y2-y3));
		if(dist1*dist1+dist2*dist2<=thres1){
			return true;
		}
		else{
			return false;
		}
	}
}

void onTrackbar1(int,void*)
{
	//printf("kola\n");
}

int lenLine(Vec4i l){
	return sqrt((l[2]-l[0])*(l[2]-l[0])+(l[3]-l[1])*(l[3]-l[1]));
}

void onTrackbar()
{
	if(alreadyFunced){
		return ;
	}
	vector<Vec4i> lines1;
	cvtColor(dst, cdst, CV_GRAY2BGR);
	HoughLinesP(dst, lines1, 1, CV_PI/180, thresh, thresh1, thresh2 );
	lines = lines1;
	printf("%d--->\n",lines.size());
	//This is to eliminate lines which are same
	//
	vector<pair<Vec4i,Vec4i> > intersection;
	int counterLines=0;
	/*for(size_t i=0;i<lines.size();i++){
		 Vec4i l1=lines[i];
		 	line( cdst, Point(l1[0], l1[1]), Point(l1[2], l1[3]), Scalar(0,0,255), 3, CV_AA);
	}
	imshow("detected lines",cdst);
	 waitKey(0);
	 exit(0);*/
	printf("sizes  ---> %d\n", lines.size());
	vector<Vec4i> bigLinesRemoved;
	int yohe=0;
	for(size_t i=0;i<lines.size();i++){
		Vec4i l = lines[i];
		int lenll = lenLine(l);
		int threshlen = 50;
		if(abs(l[0]-l[2])<=10){
			if(lenll >= (src.rows-threshlen))
				yohe++;
			else
				bigLinesRemoved.push_back(l);
		}
		else if(abs(l[1]-l[3])<=10){
			if(lenll >= (src.cols-threshlen))
				yohe++;
			else
				bigLinesRemoved.push_back(l);
		}	
		else
			bigLinesRemoved.push_back(l);
	}
		
	lines = bigLinesRemoved;
	printf("sizes  ---> %d %d\n", yohe, lines.size());
	//exit(0);
	
	memset(ffg,0,sizeof(ffg));
	vector<Vec4i> finalLines;
	double threshold=0.0001;

	//lines=finalLines;i
	vector<int> isCorrect;
	for(size_t i=0;i<lines.size();i++){
		isCorrect.push_back(true);
	}

	for(size_t i=0;i<lines.size();i++){
		bool poss=true;
		Vec4i l=lines[i];
		for(size_t j=0;j<lines.size();j++){
			if(i!=j){
				Vec4i l1=lines[j];
				int thres11=500;
				if(abs(l[0]-l[2])<=10 && abs(l1[0]-l1[2])<=10){
					int dist1=abs(l[0]-l1[0]);
					int length1=abs(l[1]-l[3]);
					int length2=abs(l1[1]-l1[3]);
					if(dist1<=thres11 && 20<=dist1 && abs(length1-length2)<=5){
						isCorrect[i]=false;
						isCorrect[j]=false;
					}
				}
				else if(abs(l[1]-l[3])<=10 && abs(l1[1]-l1[3])<=10){
					int dist1=abs(l1[1]-l[1]);
					int length1=abs(l[0]-l[2]);
					int length2=abs(l1[0]-l1[2]);
					if(dist1<=thres11 && 20<=dist1 && abs(length1-length2)<=5){
						isCorrect[i]=false;
						isCorrect[j]=false;
					}
				}
			}
		}
	}

	for(size_t i=0;i<lines.size();i++){
		if(isCorrect[i]){
			finalLines.push_back(lines[i]);
		}
	}
	lines=finalLines;
	//sort(finalLines.begin(),finalLines.end());
	int count1=0;
	for( size_t i = 0; i < lines.size(); i++ ){
		Vec4i l = lines[i];	
		int flagortho = 0;
		for( size_t j= 0; j < lines.size(); j++ ){
			Vec4i l1 = lines[j];
			if(abs(l[0]-l[2])<=10 && (abs(l1[1]-l1[3]))<=10){
				count1++;
				if(DoLineSegmentsIntersect(l[0],l[1],l[2],l[3],l1[0],l1[1],l1[2],l1[3]) ){
					intersection.push_back(make_pair(l,l1));
					printf("in intersection\n");
				}
			}
		}
	}




	/*set<int> trash;
	sort(intersection.begin(),intersection.end(),vectorcompare);
	for(int i=0;i<intersection.size();i++){
		Vec4i l = intersection[i].first;
		Vec4i ll = intersection[i].second;
		int mainlenver = lenLine(l);
		int mainlenhor = lenLine(ll);
		for(int j=0;j<intersection.size();j++){
			if(i!=j){
				Vec4i l1 = intersection[j].first;
				Vec4i ll1 = intersection[j].first;
				int templenver = lenLine(l1);
				int templenhor = lenLine(ll1);
				if(templenhor < (mainlenhor-590) && DoLineSegmentsIntersect(l[0],l[1],l[2],l[3],ll1[0],ll1[1],ll1[2],ll1[3]) ){
					if(trash.find(j) == trash.end())
						trash.insert(j);
				}
				if(templenver < (mainlenver-590) && DoLineSegmentsIntersect(ll[0],ll[1],ll[2],ll[3],l1[0],l1[1],l1[2],l1[3]) ){
					if(trash.find(j) == trash.end())
						trash.insert(j);
				}
			}
		}
	}
	vector<pair<Vec4i,Vec4i> > removeTrashLines;
	for(int i=0;i<intersection.size();i++){
		if(trash.find(i) == trash.end())
			removeTrashLines.push_back(intersection[i]);
	}
	intersection = removeTrashLines;

	for(size_t i=0;i<intersection.size();i++){
		Vec4i l1=intersection[i].first;
		Vec4i l=intersection[i].second;
		line( cdst, Point(l1[0], l1[1]), Point(l1[2], l1[3]), Scalar(0,0,255), 3, CV_AA);
		line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);		 	
	}
	imshow("detected lines",cdst);
	 waitKey(0);
	 exit(0);*/
	

	//trying to find the graph of connected lines and using them further
	vector<pair<pair<pair<int,int>,int>,int> > lines;
	map<pair<pair<pair<int,int>,int>,int>,int> linesMapped;
	bool present[100001];
	finalLines.clear();

	for(int index=0;index<intersection.size();index++){
		finalLines.push_back(intersection[index].first);
		finalLines.push_back(intersection[index].second);
	}
	//finalLines now has the lines 

	int indexLines=0;
	for(size_t index=0;index<intersection.size();index++){
		Vec4i l=intersection[index].first;
		Vec4i l1=intersection[index].second;
		if(linesMapped.count(make_pair(make_pair(make_pair(l[0],l[1]),l[2]),l[3]))==0){
			linesMapped[make_pair(make_pair(make_pair(l[0],l[1]),l[2]),l[3])]=indexLines++;
			mappedLines[indexLines-1]=make_pair(make_pair(make_pair(l[0],l[1]),l[2]),l[3]);
		}
		if(linesMapped.count(make_pair(make_pair(make_pair(l1[0],l1[1]),l1[2]),l1[3]))==0){
			linesMapped[make_pair(make_pair(make_pair(l1[0],l1[1]),l1[2]),l1[3])]=indexLines++;
			mappedLines[indexLines-1]=make_pair(make_pair(make_pair(l1[0],l1[1]),l1[2]),l1[3]);
		}
	}
	//now mapped lines to integer
	for(int index=0;index<indexLines;index++){
		parent[index]=index;
		childL[index]=1;
	}

	//to remove the lines which are very same
	for(int index=0;index<finalLines.size();index++){
		for(int index1=0;index1<finalLines.size();index1++){
			Vec4i l=finalLines[index];
			Vec4i l1=finalLines[index1];
			if(abs(l[0]-l1[0])<=10 && abs(l[1]-l1[1])<=10 && abs(l[2]-l1[2])<=10 && abs(l[3]-l1[3])<=10){
				printf("herer in union\n");
				union1(linesMapped[make_pair(make_pair(make_pair(l[0],l[1]),l[2]),l[3])],linesMapped[make_pair(make_pair(make_pair(l1[0],l1[1]),l1[2]),l1[3])]);
			}
		}
	}
	//
	for(size_t index=0;index<intersection.size();index++){
		Vec4i l=intersection[index].first;
		Vec4i l1=intersection[index].second;
		int index1=linesMapped[make_pair(make_pair(make_pair(l[0],l[1]),l[2]),l[3])];
		int index2=linesMapped[make_pair(make_pair(make_pair(l1[0],l1[1]),l1[2]),l1[3])];
		linesNeighbour[parentFind(index1)].push_back(parentFind(index2));
		linesNeighbour[parentFind(index2)].push_back(parentFind(index1));
		printf("Edge between -> %d___%d\n",parentFind(index1),parentFind(index2));
	}

	for(size_t index=0;index<indexLines;index++){
		if(!visitedDFS[index]){
			printf("in DFS--\n");
			DFSLines(index,-1);
			getImage();
			connectedLines.clear();
		}
	}
	//end of code

	printf("%d--->>>\n",count1);

	// sort(intersection.begin(),intersection.end(),vectorcompare);
	// set<int> filter;
	// for(int i=0;i<intersection.size();i++){
	// 	if(filter.find(i) == filter.end()){
	// 		for(int j=i+1;j<intersection.size();j++){
	// 			if(filter.find(j) == filter.end()){
	// 				// code to check if inside the outer alreadyu sorted pair
	// 			}
	// 		}
	// 	}
	// }

	//map<Vec4i,int> mappingLines;
	minx=miny=INT_MAX;
	maxx=maxy=-1;

	for(int i=0;i<intersection.size();i++){
			Vec4i l1=intersection[i].first;
			Vec4i l=intersection[i].second;
			printf("here->%d\n",i);
			line( cdst, Point(l1[0], l1[1]), Point(l1[2], l1[3]), Scalar(0,0,255), 3, CV_AA);
			line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
			minx=min(minx,min(l1[0],l1[2]));
			maxx=max(maxx,max(l1[0],l1[2]));
			miny=min(miny,min(l1[1],l1[3]));
			maxy=max(maxy,max(l1[1],l1[3]));
			//break;
	}
	imshow("detected lines", cdst);
	waitKey(0);
	cout << "kk" << endl;
	FILE *ftr=fopen("graph_0.txt","w");
	fprintf(ftr,"%d\n",minx);
	fprintf(ftr,"%d\n",miny);
	fprintf(ftr,"%d\n",maxx);
	fprintf(ftr,"%d\n",maxy);
	alreadyFunced=true;
}

//iterate over all the files generated
int main(){
	vector<pair<float,int> > lv;

	src= imread("graph_0.jpg");
	src1 = imread("graph_0.jgp");
	cout << src.rows << " "  << src.cols;

	resize(src, src, cvSize((src.cols)*2, (src.rows)*2));
	Point2f a(0,0);
	//circle(src,a,500,Scalar(255), 2, 8, 0);
 	Canny(src, dst, 50, 200, 3);
 	cvtColor(dst, cdst, CV_GRAY2BGR);
 	namedWindow( "detected lines",  WINDOW_NORMAL);
    /*createTrackbar( "Threshold1", "detected lines", &thresh, 1000, onTrackbar);
    createTrackbar( "Threshold2", "detected lines", &thresh1, 1000, onTrackbar1);
    onTrackbar(thresh,0);
    onTrackbar(thresh1,0);*/
    onTrackbar();
    waitKey(0);
}