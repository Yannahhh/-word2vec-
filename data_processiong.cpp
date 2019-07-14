#include<iostream>
#include<fstream>
#include<string>
#include<list> 
#include<algorithm>
#include<cmath>
using namespace std;

struct node{
	int one;
	int two;
	int three;
};

list <string> lineword;
list <string> wordvector;
list <node> xishu;
//切割一个行词汇，构成词汇向量 
void creatvector(string all){
   	//all += ' ';  //利用一个单词前后都是空格 
	string tmp = "";
	for(int i = 0; i < all.size(); i++){//建立词汇向量 
		if(all[i] == ' '){
			if(tmp == "") ;
			else{
				list<string>::iterator it = find(wordvector.begin(),wordvector.end(),tmp);
				if(it == wordvector.end()) //之前没出现过的单词，加入词汇向量中 
					wordvector.push_back(tmp);	
			}
			tmp = "";
		}
		else tmp += all[i];//确保一个单词的完成性 
	}
}	
//确定在字符串中str中子串sub出现的次数num 
int fun3(string str,string sub)
{
	int num = 0;
	while(1){
		size_t pos = str.find(sub);
  		if (pos != -1 )  num++;
  		else break;
  	    str = str.substr(pos+sub.size()-1); //去掉含有子串sub的部分，继续查找子串sub 
	}
	return num;
}

int main(){
	fstream  f("D:\\files\\SMIE\\third_up\\ai\\周五5-6节验收数据\\text.txt");
	fstream  Amatrix("D:\\files\\SMIE\\third_up\\ai\\周五5-6节验收数据\\A.txt");
	fstream  Bmatrix("D:\\files\\SMIE\\third_up\\ai\\周五5-6节验收数据\\B.txt");
	fstream  OneHot("D:\\files\\SMIE\\third_up\\ai\\周五5-6节验收数据\\OneHot.txt",ios::out);
	fstream  TF("D:\\files\\SMIE\\third_up\\ai\\周五5-6节验收数据\\TF.txt",ios::out);
	fstream  TF_IDF("D:\\files\\SMIE\\third_up\\ai\\周五5-6节验收数据\\TF_IDF.txt",ios::out);
	fstream  Xishu("D:\\files\\SMIE\\third_up\\ai\\周五5-6节验收数据\\Smatrix.txt",ios::out);
	fstream  Cmatrix("D:\\files\\SMIE\\third_up\\ai\\周五5-6节验收数据\\AplusB.txt",ios::out);
	
	string a ;
	while(getline(f,a)){
		size_t pos = a.rfind('\t');  //找出第二个制表符的位置 
		a = ' '+a.substr(pos+1)+' '; // 在内容的前后加入空格，以便每个单词的分割 
		lineword.push_back(a);       //将行内容加入行词汇列表中 
		creatvector(a);              //将行内容进行分割，创建词汇列表 
	}
	//cout<<"end"<<wordvector.size()<<endl;
	int ROW = lineword.size();
	int COL = wordvector.size();
	
	//新建一个二维数组 Onehot[ROW+1][COL+1]
	//最后一行的元素是每一列非0元素的个数 ,最后一列是每一行元素的总和 
	int **Onehot;
	Onehot = new int *[ROW+1];
	
	for(int i = 0; i < ROW+1; i++)
		Onehot[i] = new int [COL+1];
	
	for(int i = 0; i < ROW+1; i++)
		for(int j = 0; j < COL+1; j++)
			Onehot[i][j] = 0;

	int col = 0;
	int row = 0;
	list<string>::iterator it1;
	list<string>::iterator it2;
	//对每一行词汇做处理，看是否出现词汇向量里的单词，以及出现的次数 
	for(it1 = lineword.begin();  it1 != lineword.end(); it1++){
		 string  linew = *it1; 
		 for(it2 = wordvector.begin();  it2 != wordvector.end(); it2++){
		 	string vec = *it2;
		 	vec =' '+vec+ ' ';//为了避免is和this这种单词混淆，前后加空格以区分 
			Onehot[row][col] = fun3(linew,vec);//fun3返回在row行中，出现单词vec的次数 
			Onehot[row][COL] += Onehot[row][col];//最后一列是某行总数 
			if(Onehot[row][col]){
				Onehot[ROW][col]++;
				node tmpp;
				tmpp.one = row;
				tmpp.two = col;
				tmpp.three = 1;
				xishu.push_back(tmpp); 	//将非0元素弄成节点，加入到稀疏列表中 
			} 
			col++;
		 }
		 row++;
		 col = 0;
	} 
	//Onehot  cout<<"**********Onehot****************"<<endl; 
	string tmps; 
	for(int i =0 ;i<ROW;i++){
		for(int j =0 ;j<COL;j++)
			 OneHot << ((Onehot[i][j])?1:0) << " ";
		OneHot<<'\n';
	}
	//TF cout<<endl<<"*************TF****************"<<endl<<endl; 
	for(int i =0 ;i<ROW;i++){
		for(int j =0 ;j<COL;j++)
			TF<<double(Onehot[i][j])/double(Onehot[i][COL])<<' ';
		TF<<'\n';
	}
	
	//TF-IDFcout<<endl<<"**********TF-IDF****************"<<endl<<endl; 
	for(int i =0 ;i<ROW;i++){
		for(int j =0 ;j<COL;j++)
			TF_IDF<<double(Onehot[i][j])/double(Onehot[i][COL])*log(double(ROW)/double(1+Onehot[ROW][j]))<<' ';
		TF_IDF<<'\n';
	}
	
	//稀疏矩阵
	 Xishu<<ROW<<'\n'<<COL<<'\n'<<xishu.size()<<'\n';
	 list<node>::iterator it;
	 for(it = xishu.begin(); it!=xishu.end();it++)
	   Xishu<<(*it).one<<" "<<(*it).two<<" "<<(*it).three<<'\n';
	   	
	for(int i = 0; i<ROW+1; i++)
	   delete [] Onehot[i];
	
	int Ai,Aj,Ak;
	int Bi,Bj,Bk;
	Amatrix>>Ai>>Aj>>Ak;
	Bmatrix>>Bi>>Bj>>Bk;
	
	list<node> Amat;
	list<node> Bmat; 
	list<node> Cmat;
	for(int i = 0; i<Ak ;i++){
		node tmp;
		Amatrix>>tmp.one>>tmp.two>>tmp.three;
		Amat.push_back(tmp);
	}
	for(int i = 0; i<Bk ;i++){
		node tmp;
		Bmatrix>>tmp.one>>tmp.two>>tmp.three;
		Bmat.push_back(tmp);
	}

	list<node>::iterator itA = Amat.begin();
	list<node>::iterator itB = Bmat.begin();
	
	//A,B加和
	while(itA != Amat.end() && itB != Bmat.end()){//遍历A.B
		node tmpA = *itA;
		node tmpB = *itB;
		if(tmpA.one == tmpB.one && tmpA.two == tmpB.two){
		    //如果两个元素位置相同，对应的值相加
			tmpA.three += tmpB.three;
			Cmat.push_back(tmpA);
			itA++;//A，B都跳到下一个元素
			itB++;
		}
		else if(tmpA.one < tmpB.one || tmpA.one == tmpB.one && tmpA.two < tmpB.two ){
			//如果A元素的i值比B的小，或者i值相同时，A元素的j比较小，A元素先进入列表
			Cmat.push_back(tmpA);
			itA++;
		}
		else{
			Cmat.push_back(tmpB);
			itB++;
		}
	}
	//如果A元素还有剩余,就将A的加入其中
	while(itA != Amat.end()){
		node tmp = *itA;
		Cmat.push_back(tmp);
		itA++;
	}
	//如果B元素还有剩余,就将B的加入其中
	while(itB != Bmat.end()){
		node tmp = *itB;
		Cmat.push_back(tmp);
		itB++;
	}	
	Cmatrix << max(Ai,Bi) <<'\n'<< max(Aj,Bj) <<'\n' << Cmat.size()<<'\n';
	list<node>::iterator itC;
	for(itC = Cmat.begin() ; itC != Cmat.end() ;itC++) 
		Cmatrix<<(*itC).one<<" "<<(*itC).two<<" "<<(*itC).three<<'\n';
	
	f.close();	
	OneHot.close();
	TF.close();
	TF_IDF.close();
	Xishu.close();
	Amatrix.close();
	Bmatrix.close();
	Cmatrix.close();	
	return 0;
} 


