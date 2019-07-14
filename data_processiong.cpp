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
//�и�һ���дʻ㣬���ɴʻ����� 
void creatvector(string all){
   	//all += ' ';  //����һ������ǰ���ǿո� 
	string tmp = "";
	for(int i = 0; i < all.size(); i++){//�����ʻ����� 
		if(all[i] == ' '){
			if(tmp == "") ;
			else{
				list<string>::iterator it = find(wordvector.begin(),wordvector.end(),tmp);
				if(it == wordvector.end()) //֮ǰû���ֹ��ĵ��ʣ�����ʻ������� 
					wordvector.push_back(tmp);	
			}
			tmp = "";
		}
		else tmp += all[i];//ȷ��һ�����ʵ������ 
	}
}	
//ȷ�����ַ�����str���Ӵ�sub���ֵĴ���num 
int fun3(string str,string sub)
{
	int num = 0;
	while(1){
		size_t pos = str.find(sub);
  		if (pos != -1 )  num++;
  		else break;
  	    str = str.substr(pos+sub.size()-1); //ȥ�������Ӵ�sub�Ĳ��֣����������Ӵ�sub 
	}
	return num;
}

int main(){
	fstream  f("D:\\files\\SMIE\\third_up\\ai\\����5-6����������\\text.txt");
	fstream  Amatrix("D:\\files\\SMIE\\third_up\\ai\\����5-6����������\\A.txt");
	fstream  Bmatrix("D:\\files\\SMIE\\third_up\\ai\\����5-6����������\\B.txt");
	fstream  OneHot("D:\\files\\SMIE\\third_up\\ai\\����5-6����������\\OneHot.txt",ios::out);
	fstream  TF("D:\\files\\SMIE\\third_up\\ai\\����5-6����������\\TF.txt",ios::out);
	fstream  TF_IDF("D:\\files\\SMIE\\third_up\\ai\\����5-6����������\\TF_IDF.txt",ios::out);
	fstream  Xishu("D:\\files\\SMIE\\third_up\\ai\\����5-6����������\\Smatrix.txt",ios::out);
	fstream  Cmatrix("D:\\files\\SMIE\\third_up\\ai\\����5-6����������\\AplusB.txt",ios::out);
	
	string a ;
	while(getline(f,a)){
		size_t pos = a.rfind('\t');  //�ҳ��ڶ����Ʊ����λ�� 
		a = ' '+a.substr(pos+1)+' '; // �����ݵ�ǰ�����ո��Ա�ÿ�����ʵķָ� 
		lineword.push_back(a);       //�������ݼ����дʻ��б��� 
		creatvector(a);              //�������ݽ��зָ�����ʻ��б� 
	}
	//cout<<"end"<<wordvector.size()<<endl;
	int ROW = lineword.size();
	int COL = wordvector.size();
	
	//�½�һ����ά���� Onehot[ROW+1][COL+1]
	//���һ�е�Ԫ����ÿһ�з�0Ԫ�صĸ��� ,���һ����ÿһ��Ԫ�ص��ܺ� 
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
	//��ÿһ�дʻ����������Ƿ���ִʻ�������ĵ��ʣ��Լ����ֵĴ��� 
	for(it1 = lineword.begin();  it1 != lineword.end(); it1++){
		 string  linew = *it1; 
		 for(it2 = wordvector.begin();  it2 != wordvector.end(); it2++){
		 	string vec = *it2;
		 	vec =' '+vec+ ' ';//Ϊ�˱���is��this���ֵ��ʻ�����ǰ��ӿո������� 
			Onehot[row][col] = fun3(linew,vec);//fun3������row���У����ֵ���vec�Ĵ��� 
			Onehot[row][COL] += Onehot[row][col];//���һ����ĳ������ 
			if(Onehot[row][col]){
				Onehot[ROW][col]++;
				node tmpp;
				tmpp.one = row;
				tmpp.two = col;
				tmpp.three = 1;
				xishu.push_back(tmpp); 	//����0Ԫ��Ū�ɽڵ㣬���뵽ϡ���б��� 
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
	
	//ϡ�����
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
	
	//A,B�Ӻ�
	while(itA != Amat.end() && itB != Bmat.end()){//����A.B
		node tmpA = *itA;
		node tmpB = *itB;
		if(tmpA.one == tmpB.one && tmpA.two == tmpB.two){
		    //�������Ԫ��λ����ͬ����Ӧ��ֵ���
			tmpA.three += tmpB.three;
			Cmat.push_back(tmpA);
			itA++;//A��B��������һ��Ԫ��
			itB++;
		}
		else if(tmpA.one < tmpB.one || tmpA.one == tmpB.one && tmpA.two < tmpB.two ){
			//���AԪ�ص�iֵ��B��С������iֵ��ͬʱ��AԪ�ص�j�Ƚ�С��AԪ���Ƚ����б�
			Cmat.push_back(tmpA);
			itA++;
		}
		else{
			Cmat.push_back(tmpB);
			itB++;
		}
	}
	//���AԪ�ػ���ʣ��,�ͽ�A�ļ�������
	while(itA != Amat.end()){
		node tmp = *itA;
		Cmat.push_back(tmp);
		itA++;
	}
	//���BԪ�ػ���ʣ��,�ͽ�B�ļ�������
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


