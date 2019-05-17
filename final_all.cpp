#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <string.h>
#include <math.h>
#include <set>
#include <vector>
#include <list>
using namespace std;
#define MININFO 100000

/*
    core MapList
*/
struct FileInfo{
    int id;						/*path of the file*/
	list<pair<int,int>> positions;		/*positions of keyword*/
    int freq;							/*frequency of keyword*/
};

typedef std::map<std::string, std::list<FileInfo>> MapList;


/*全局变量*/
map<string, double> word_dict;
map<string, double> char_dict;

int stringToNum(const string& str){  
    istringstream iss(str);  
    int num;  
    iss >> num;  
    return num;      
} 

/*读取char生成char map,读取word生成word map*/
template <class T>
void get_freq_dict(const char *fn, T &dict)
{
	string rawkey, total_s,rawvalue_s;//total用来表示所有的词的个数
	ifstream infile;
	infile.open(fn);
	infile >> total_s;
    infile >> total_s;//将total读出来
	int total=stringToNum(total_s);
	while (infile >> rawkey && infile >> rawvalue_s) {
		int rawvalue=stringToNum(rawvalue_s);
        double freqP = (1.0*rawvalue)/(1.0*total);
		dict.insert(pair<string, double>(rawkey, freqP));
	}
	infile.close();
}

/* 将aticle的每一句话提取出来，放入vector中 */
vector<pair<int,string>> SplitAticle()
{
    /* 
     *total_pos:    记录汉字或字符在整个article中的位置
     *i:            作为utf8的参数
     *is_ch：       用于标识$，跳过
     */
    int total_pos = 0, i = 0, is_ch = 0;
    char utf8[3] = {0};                        
    char c;
    string str = "";
    vector<pair<int,string>> strs;
    fstream infile("article.txt", ios::in);  
    while(infile.get(c)){
        //如果碰到$,停止处理，一直等到下一个$出现
        if(c == '$'){                               
            total_pos++;
            is_ch = !is_ch;
            continue;
        }
        if(is_ch == 0){
            //如果是字母和数字
            if((c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A') || (c >= '0' && c <= '9')){
                total_pos++;
                /*
                 *char sigle[2] = {c,0};
                 *待完善，对字母数字的处理
                 *
                }*/
            }
            //如果是单字节的符号编码
            else if(c == ',' || c == ':' || c == ';' || c == '=' || c == '-' || c == '&' || c == '^' || c == '?' ||
                    c == '.' || c == '#' || c == '+' || c == '%' || c == '~' || c == '_' || c == ' ' || c == '/' || 
                    c == '[' || c == ']' || c == '<' || c == '>' || c == '(' || c == ')' || c == '{' || c == '}' ||
                    c == '\"' || c == '\\' || c == '\r' || c == '\n' || c == '\t' || c == ' ' || c == '\''){
                total_pos++;
                if(i == 0 && str != ""){
                    strs.push_back(pair<int,string>(total_pos,str));
                }
                str = "";
            }
            //如果是三字节的中文编码
            else{
                utf8[i++] = c;
                if(i == 3){
                    total_pos++;
                    i = 0;
                    //如果三字节的utf8是标点符
                    if(!strcmp(utf8,"、") || !strcmp(utf8,"，") || !strcmp(utf8,"？") || !strcmp(utf8,"（")|| !strcmp(utf8,"）")|| !strcmp(utf8,"。")|| !strcmp(utf8,"；")|| !strcmp(utf8,"：") ||
                       !strcmp(utf8,"“") || !strcmp(utf8,"”") || !strcmp(utf8,"①") || !strcmp(utf8,"②") || !strcmp(utf8,"③") || !strcmp(utf8,"⑥") || !strcmp(utf8,"↓") || !strcmp(utf8,"●")){
                        if(str != "")
                            strs.push_back(pair<int,string>(total_pos,str));
                        str = "";
                    }
                    //如果是中文
                    else
                        str += (string)utf8;
                }           
            }            
        }
        else
            total_pos++;
    }
    infile.close();
    return strs;
}

template<typename T> string toString(const T& t) {
	ostringstream oss;  //创建一个格式化输出流
	oss << t;             //把值传递入流中
	return oss.str();
}

string position(int pos, int n)//��������֮�������������������ַ���
{
	string change;
    //change=change+to_string(pos);
	while (n>0){
        string a="+";
		//cout << pos<< endl;
		change = change + a+toString(pos);
		pos++;
		n--;
	}
	change=change.substr(1,change.length()-1);
	return change;
}


int my_find(string a)
{
	map<string, double>::iterator iter;
	for (iter = word_dict.begin(); iter != word_dict.end(); iter++){
		if (a == iter->first)
			return 1;

	}
	return 0;
}

void getAllSubstrings(string str, map<string, string> &pos_word)
{
	if (str.size() == 0)
		return;
	else{
		for (int i = 0; i < str.size(); i=i+3){
			for (int j = 3; i+j <= str.size(); j=j+3){
				//cout << str.substr(i,  j) <<endl;
				string pos;
				//cout << i << "   " << j << endl;
				pos = position(i / 3, j / 3);
				//cout << pos << endl;
				int m;
				m = my_find(str.substr(i, j));
				
				if (m == 1)
					pos_word.insert(pair<string, string>(pos, str.substr(i, j)));
			}
		}
	}
}
void generate_set(int code[], string arr[], set<string> &groups, int n)
{
	string tmp="";
	int i;
	for (i = 0; i < n; i++){
		// Print if the corresponding value is true.
		if (code[i] == 1){
			tmp.append(arr[i]);
			tmp.append(",");
		}
	}
	int len= tmp.length();
	if(len>0)	
		tmp = tmp.substr(0,len-1);
	groups.insert(tmp);
}
 
void GenGrayCode(string arr[], int n,set<string> &groups) 
{ 
	int r, i, k, j, l, code[n], a[n];
	r = pow(2, n);
	//����2^n���Ӽ�.
	for (i = 0; i < r; i++){
		k = i;
		// ��i�Ķ�����ֵ����a[]��.
		for (j = n - 1; j >= 0; j--, k /= 2) 
			a[j] = k % 2;
		k = 0;
		l = 0;
		// �Ӷ�����ֵ������Ӧ��code.
		for (j = 0; j < n; j++){	
			// ����һ��ֵ��1����������code
			if (k == 0){
				if (a[j] == 1) 
					k++; 
				code[l] = a[j];
				l++; 
			}
			else if (a[j] == a[j - 1]){
				code[l] = 0;
				l++;
			}
			else{
				code[l] = 1;
				l++;
			}
		}
		generate_set(code, arr, groups, n);
	}
	/*
	set<string>::iterator it;
	for (it = groups.begin(); it != groups.end(); it++)
		cout << *it << "\n";
		*/

}
vector<string> split(const string &s, const string &seperator) {
	vector<string> result;
	typedef string::size_type string_size;
	string_size i = 0;

	while (i != s.size()) {
		//找到字符串中首个不等于分割符的字母
		int flag = 0;
		while (i != s.size() && flag == 0) {
			flag = 1;
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[i] == seperator[x]) {
					++i;
					flag = 0;
					break;
				}
		}

		//找到又一个分割符，将两个分割符之间的字符串取出
		flag = 0;
		string_size j = i;
		while (j != s.size() && flag == 0) {
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[j] == seperator[x]) {
					flag = 1;
					break;
				}
			if (flag == 0)
				++j;
		}
		if (i != j) {
			result.push_back(s.substr(i, j - i));
			i = j;
		}
	}
	return result;
}

int remove(string a){
	int m;
	vector<string> v = split(a,",+");

	m = 0;
	for (int i = 0; i < v.size(); i++){
		for (int j = i + 1; j < v.size(); j++){
			//cout << v[i] << "  " << v[j] << endl;
			if (v[i] == v[j]){
				m = 1;
				return m;
			}
		}
		//cout << v[i] << endl;
	}
	return m;

}

/* 移除字符串 */
void conflict_remove(set<string> &groups){
	set <string>::iterator itList;
	for (itList = groups.begin(); itList != groups.end(); ){
		int m;
		m = remove(*itList);
		if (m == 1){
			set <string>::iterator tmp = itList;
			itList++;
			groups.erase(tmp);
		}
		else
			itList++;
	}
}

/*把c中的符号作为分割符，对s进行分割，分割后的子串放入vector v中*/
void SplitString(const string s, vector<string> *v, const string c){
	string::size_type pos1,pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while(string::npos != pos2){
		v->push_back(s.substr(pos1, pos2-pos1));
		pos1 = pos2 + c.size();
		pos2 = s.find(c,pos1);
	}
	if(pos1 != s.length())
		v->push_back(s.substr(pos1));
}


/* 传入一个string(1+2,3+4,12+13)添加进入maplist */
void buildmaplist(vector<string> &strs, map<string,string> pos_word, MapList &maplist, int fileId, int headPos){
	vector<string>::iterator it;
	for(it = strs.begin(); it != strs.end(); it++){
		/* 获取词语的开始position和结束position */
		string str = pos_word[*it];
		int start = atoi(it->substr(0, it->find_first_of("+")).c_str());
		int end = atoi(it->substr(it->find_last_of("+")+1, it->length()-(it->find_last_of("+")+1)).c_str());
		/* 构造position pair */
		pair<int,int> p;
		p.first = headPos + start;
		p.second = headPos + end;

		if(maplist.count(str) == 0){/* maplist中不存在该striing */
			list<FileInfo> filelist;
			FileInfo finfo;
			finfo.id = fileId;
			finfo.freq = 1;
			finfo.positions.push_back(p);
			filelist.push_back(finfo);
			maplist.insert(pair<string,list<FileInfo>>(str,filelist));
		}else{
			int flag = 0;
			list<FileInfo>::iterator it2;
			/* 遍历list中的struct，查看是否有同一个id的fileinfo */
			for(it2 = maplist[str].begin(); it2 != maplist[str].end(); it2++){
				if(it2->id == fileId){
					it2->freq++;
					it2->positions.push_back(p);
					flag = 1;
					break;
				}
			}
			/* 没有找到相同fileinfo */
			if(flag == 0){
				FileInfo finfo;
				finfo.id = fileId;
				finfo.freq = 1;
				finfo.positions.push_back(p);
				maplist[str].push_back(finfo);
			}
		}
	}
}

/*把空缺的位置以单个字的形式从char_freq中查找信息量，累加词的信息量，算出最小的那个加入词典*/
string get_min_info(string handle, set<string> *groups, map<string,string> pos_word)
{
	string final_str;//最终分词结果的string
	double final_info = 1000000000000000000;//最终最小信息量
	set<string>::iterator sit;
	/* 开始计算信息量 */
	for(sit = groups->begin(); sit != groups->end(); sit++){
		//cout<<"group element: "<<*sit<<endl;
		double info = 0;//临时存放最小信息量
		string mid_str = *sit;

		/* 分割string */
		vector<string> sub_str;//存分出来的每个词
		SplitString(mid_str, &sub_str, ",");//分割string生成集合sub_str
		vector<string>::iterator vit;
		int handle_len =  handle.size();//handle的长度
		
		/* 计算单个字的信息量 */
		int pos = 0;
		while(3*pos < handle_len){
			int flag = 0;//用来标志handle中的某个字是否属于sub_str中,1属于,0不属于
			for(vit = sub_str.begin(); vit != sub_str.end(); vit++){
				if(vit->find(toString((long long)pos)) != string::npos){
					flag = 1;
				}
			}
			/* pos不出现在某一个sub_str中 */
			if(flag == 0){
				//计算pos处字的信息量
				map<string,double>::iterator ch_it;
				ch_it = char_dict.find(handle.substr(3*pos,3));
				if(ch_it != char_dict.end()){//如果char_dict中存在这个字
					//cout<<"单字:"<<ch_it->first<<"";
					info = info + (-0.8) * log(ch_it->second)/log(2.0);
				}
				else{//如果不存在这个字,分配一个固定信息量
					info = info + MININFO;
				}
				//cout<<info<<endl;
			}
			pos++;
		}
		//计算word的信息量,遍历sub_str获取每个子串的信息量
		for(vit = sub_str.begin(); vit != sub_str.end(); vit++){	
			int tmp=(-0.95) * log(word_dict[pos_word[*vit]])/log(2.0);
			//cout<<"词语:"<<pos_word[*vit]<< tmp <<endl;
			//vit是sub_str中的词的位置
			info = info + tmp;
		}
		//cout <<"info:"<< info << "\n\n";

		//将完成所有词和字信息量计算的info和最终的final_info比较
		if(final_info > info){
			final_info = info;
			final_str = mid_str;
		}
	}	
	cout << endl << "final info:" << final_info << endl << "final str: " << final_str << endl;
	return final_str;
}

/* 句子分词 */
int WordsParse(string str, MapList &maplist, int fileId, int headPos)
{
    cout<<str.size()<<endl;
   	map<string, string> pos_word;
	getAllSubstrings(str, pos_word);
	map<string,string>::iterator it;

	it = pos_word.begin();
	if(it==pos_word.end())
		cout<<"no\n";
	while (it != pos_word.end())
	{
		cout<<"init:";
		cout<<it->first;
		cout << it->second << endl;
		it++;
	}
	map<string, string>::iterator cur;
	set<string> groups;//����Ϸ�ʽ
	int n=pos_word.size();
	string arr[n];

	int i=0;
	//����map�е�key��λ��string����copy��arr
	for (cur=pos_word.begin() ; cur!=pos_word.end()&&i<n; i++,cur++)
	{
		arr[i]=(*cur).first;
	}
 
	//����code���������Ӽ�
	GenGrayCode(arr, n,groups);
	conflict_remove(groups);

	set<string>::iterator it_g;
	for (it_g = groups.begin(); it_g != groups.end(); it_g++)
		cout <<"after confict remove:"<< *it_g << "\n";
	cout << endl;

	/* 获取最终的句子分词结果 */
	string final_str = get_min_info(str, &groups, pos_word);

	/* 将一句话中的词语加入maplist */	
	vector<string> fin_str;
	SplitString(final_str, &fin_str, ",");
	buildmaplist(fin_str, pos_word, maplist, fileId, headPos);
	return 1;
}

int main(){
	MapList maplist;
    const char *filepath1 = "./frequency/wordfreq.txt";
	const char *filepath2 = "./frequency/charfreq.txt";
	get_freq_dict(filepath1, word_dict);
	get_freq_dict(filepath2, char_dict);

	//存放句子的vector
    vector<pair<int,string>> strs;
    strs = SplitAticle();
    vector<pair<int,string>>::iterator it;
    for(it = strs.begin(); it != strs.end(); it++){
        cout << it->first << ":" << it->second << endl;
        /* 传入句子、maplist、文件id、句子首pos */
		WordsParse( it->second, maplist, 10, it->first);
    }
	return 0;
}
