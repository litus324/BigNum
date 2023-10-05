#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
using namespace std;

typedef struct Node* ptrNode;
typedef struct NodeInt* ptrNodeInt;


struct Node
{
	long long data;
	ptrNode pNext;
};
struct List {
	ptrNode pHead;
	ptrNode pTail;
};
struct NUM {
	int sign; // lưu dấu
	int lengthNum; // lưu độ dài chuỗi số
	List ListNum; // danh sách liên kết mỗi node chứa k kí tự số
};

ptrNode createNode(long long x) {
	ptrNode p = new Node;
	p->data = x;
	p->pNext = NULL;
	return p;
}

NUM subtract(NUM a, NUM b, int k);
NUM add(NUM a, NUM b, int k);
string NUMToString(NUM x, int k);

void CreateList(List& l)
{
	l.pHead = NULL;
	l.pTail = NULL;
}
void insertTail(List& l, long long x) {
	ptrNode p = createNode(x);
	if (!l.pTail)
		l.pHead = l.pTail = p;
	else {
		l.pTail->pNext = p;
		l.pTail = p;
	}
}
void insertHead(List& l, long long x) {
	ptrNode p = createNode(x);
	if (!l.pHead)
		l.pHead = l.pTail = p;
	else {
		p->pNext = l.pHead;
		l.pHead = p;
	}
}

//khởi tạo big int 
// ví dụ k = 3, big int  = 1234567 
// sau khi khởi tạo big int sẽ đc lưu: 567 432 1
NUM CreateNum(string s, int k) {
	NUM num;
	num.sign = 0; // dấu bằng 0 là số dương
	num.lengthNum = s.length();
	CreateList(num.ListNum);
	int countK = 0;
	long long val = 0;

	//các node có độ dài là k kí tự số
	for (int i = s.length() - 1; i >= 0; i--) {
		if (countK == k) {
			insertTail(num.ListNum, val);
			countK = 0;
			val = 0;
		}
		if (countK == 0)
			val = (s[i] - '0');
		else
			val = (int)(s[i] - '0') * int(pow(10, countK)) + val;
		countK++;
	}
	//nếu còn dư số lượng kí tự số ngắn hơn k thì lưu vào node cuối cùng
	if (val != 0) insertTail(num.ListNum, val);
	return num;
}
//xóa list lưu chuỗi số của big int
void destroyList(List& l) {
	ptrNode q;
	ptrNode p = l.pHead;
	while (p) {
		q = p;
		p = p->pNext;
		delete q;
	}
	l.pHead = l.pTail = NULL;
}
//xóa thông tin big int
void destroyNUM(NUM num)
{
	num.sign = 0;
	num.lengthNum = 0;
	destroyList(num.ListNum);
}

//đảo ngược List lưu chuỗi số 
// ví dụ k = 3, big int  = 1234567 
// sau khi khởi tạo big int sẽ đc lưu theo thứ tự: 567 432 1
//sau khi reverse big int dc lưu theo thứ tự: 1 234 567
NUM reverseNum(NUM x)
{
	NUM y;
	y.sign = x.sign;
	y.lengthNum = x.lengthNum;
	CreateList(y.ListNum);
	for (ptrNode i = x.ListNum.pHead; i; i = i->pNext) {
		insertHead(y.ListNum, i->data);
	}
	return y;
}

//so sánh NUM a có lớn hơn NUM b không
//nếu a < b  trả về 0
// a > b trả về 1
// a = b trả về 2
int compare(NUM a, NUM b, int k)
{
	//vì các số nhập vào đều là dương nên b luôn dương -> b.sign = 0

	//nếu a âm trả về 0
	if (a.sign == 1)
		return 0;

	//nếu a và b cùng dương
	else {
		//nếu a dài hơn trả về 1
		if (a.lengthNum > b.lengthNum)
			return 1;

		//nếu a ngắn hơn b trả về 0
		else if (a.lengthNum < b.lengthNum)
			return 0;

		//nếu độ dài a bằng b
		else {
			// đảo ngược a và b lại để so sánh
			a = reverseNum(a);
			b = reverseNum(b);
			//duyệt từng node để so sánh
			ptrNode i = a.ListNum.pHead;
			ptrNode j = b.ListNum.pHead;
			while (i != NULL) {
				if (i->data > j->data) {
					return 1;
				}
				if (i->data < j->data) {
					return 0;
				}
				i = i->pNext;
				j = j->pNext;
			}
			// nếu a và b bằng nhau
			return 2;
		}
	}
}
//cộng 2 NUM
NUM add(NUM a, NUM b, int k)
{
	NUM x;
	x.lengthNum = 0;
	CreateList(x.ListNum);
	//nếu 2 số khác dấu đưa về hàm trừ 2 số

	if (a.sign == 1 && b.sign == 0)
		return subtract(b, a, k);

	//nếu 2 số cùng dấu
	if (a.sign == 0 && b.sign == 0)
		//nếu a và b cùng dương thì tổng dương
		x.sign = 0;
	else
		//nếu a và b cùng âm thì tổng là âm
		x.sign = 1;

	//chuyển số có só chữ số lớn hơn về a, số nhỏ hơn về b 
	if (a.lengthNum < b.lengthNum) {
		NUM tam;
		CreateList(tam.ListNum);
		tam = a;
		a = b;
		b = tam;
	}
	//chiều dài kí tự số ở tổng sẽ TẠM gán bằng chiều dài kí tự số lớn hơn
	x.lengthNum = a.lengthNum;

	int du = 0;
	long long SumData = 0;
	ptrNode j = b.ListNum.pHead;
	for (ptrNode i = a.ListNum.pHead; i; i = i->pNext) {

		//nếu lúc này b đã hết số để cộng do b ngắn hơn a
		if (j == NULL) {
			// nếu như giá trị node này của a cộng dư lớn hơn độ dài k
			if (i->data + du == int(pow(10, k))) {
				insertTail(x.ListNum, 0);
				du = 1;
			}
			//nếu ko node tương ứng của tổng sẽ bằng node của a
			else {
				insertTail(x.ListNum, i->data);
				du = 0;
			}
		}

		else {
			SumData = i->data + j->data + du;
			du = SumData / int(pow(10, k));
			SumData = SumData % int(pow(10, k));
			j = j->pNext;
			insertTail(x.ListNum, SumData);
		}
	}
	//nếu sau khi cộng 2 node mà vẫn tồn tại dư thì thêm node có giá trị dư vào cuối
	if (du == 1) 
		insertTail(x.ListNum, 1);
	//tính chiều dài chuỗi số
	for (ptrNode i = x.ListNum.pHead; i; i = i->pNext) {
		if (i != x.ListNum.pTail) x.lengthNum += k;
		else {
			long long tam = i->data;
			while (tam != 0) {
				x.lengthNum++;
				tam = tam / 10;
			}
		}
	}

	return x;
}

//trừ 2 NUM
NUM subtract(NUM a, NUM b, int k)
{
	NUM x;
	x.lengthNum = 0;
	CreateList(x.ListNum);
	//vì các số nhập vào đều là dương nên chỉ có 2 TH :
	//TH1: (-a - b)  với -a là kết quả của các phép tính trước
	//TH2: (a - b) 

	//xét TH1: ta đưa về phép cộng 2 số âm -(a + b)
	if (a.sign == 1) {
		b.sign = 1;
		return add(a, b, k);
	}
	//TH2: a - b 
	else {
		//Nếu a == b thì hiệu 2 số bằng 0
		if (compare(a, b, k) == 2) {
			x.sign = 0;
			x.lengthNum = 0;
			insertHead(x.ListNum, 0);
			return x;
		}
		//nếu a < b => a-b mang dấu âm. đỏi thành b - a và gán dấu âm cho kết quả
		else if (compare(a, b, k) == 0) {
			NUM tam;
			CreateList(tam.ListNum);
			tam = a;
			a = b;
			b = tam;
			//đánh dấu âm cho kết quả
			x.sign = 1;

		}
		//nếu a > b => a-b mang dấu dương
		else
			//đánh dấu dương cho kết quả
			x.sign = 0;


		//bắt đầu tính a - b

		int no = 0;//nhớ số nợ
		long long subData = 0;// lưu hiệu a - b tại node đg xét
		ptrNode j = b.ListNum.pHead;
		for (ptrNode i = a.ListNum.pHead; i; i = i->pNext)
		{
			//nếu b đã hết giá trị
			if (j == NULL) {
				//nếu giá trị node đg xét tại a trừ nợ lớn hơn 0 thì tiếp tục lưu
				if (i->data - no > 0) {
					insertTail(x.ListNum, i->data - no);
					no = 0;
				}
				else {
					//nếu giá trị node đg xét tại a trừ nợ bằng 0 và node đg xét chưa phải node cuối cùng của a thì vẫn lưu
					if (i != a.ListNum.pTail)
						insertTail(x.ListNum, 0);
				}
			}

			else {
				subData = i->data - j->data - no;
				no = 0;
				if (subData < 0) {
					subData = int(pow(10, k)) + subData;
					no = 1;
				}
				insertTail(x.ListNum, subData);
				subData = 0;
				j = j->pNext;
			}
		}
		//cập nhật chiều dài chuỗi số
		for (ptrNode i = x.ListNum.pHead; i; i = i->pNext) {
			if (i != x.ListNum.pTail) x.lengthNum += k;
			else {
				long long tam = i->data;
				while (tam != 0) {
					x.lengthNum++;
					tam = tam / 10;
				}
			}
		}
		return x;
	}
}


struct NodeInt {
	NUM data;
	ptrNodeInt pNext;
};
struct ListInt {
	ptrNodeInt pHead;
	ptrNodeInt pTail;
};
void createListInt(ListInt& a) {
	a.pHead = NULL;
	a.pTail = NULL;
}
ptrNodeInt CreateInt(NUM x) {
	ptrNodeInt p = new NodeInt;
	p->data = x;
	p->pNext = NULL;
	return p;
}
void addTail(ListInt& a, NUM x) {
	ptrNodeInt p = CreateInt(x);
	if (!a.pTail)
		a.pHead = a.pTail = p;
	else {
		a.pTail->pNext = p;
		a.pTail = p;
	}
}
void addHead(ListInt& l, NUM x) {
	ptrNodeInt p = CreateInt(x);
	if (!l.pHead)
		l.pHead = l.pTail = p;
	else {
		p->pNext = l.pHead;
		l.pHead = p;
	}
}
void QuickSort(ListInt& l, int k) {
	ListInt l1, l2;
	ptrNodeInt tag, p;
	if (l.pHead == l.pTail) return;
	createListInt(l1);
	createListInt(l2);
	tag = l.pHead;
	l.pHead = l.pHead->pNext; // cap nhat lai l.pHead
	tag->pNext = NULL;// co lap tag
	while (l.pHead != NULL) {
		p = l.pHead;
		l.pHead = l.pHead->pNext;
		p->pNext = NULL;
		if (compare(p->data, tag->data, k) != 1) addHead(l1, p->data);
		else addHead(l2, p->data);
	}
	QuickSort(l1, k); // goi de qui sap xep l1, l2
	QuickSort(l2, k);
	if (l1.pHead != NULL) { // l1 k rong
		l.pHead = l1.pHead; // lay pHead cua l1 gan cho pHead cua l;
		l1.pTail->pNext = tag;
	} // l1 rong
	else l.pHead = tag;
	tag->pNext = l2.pHead;
	if (l2.pHead != NULL) l.pTail = l2.pTail;
	else l.pTail = tag;
}

void Split(ptrNodeInt H, ptrNodeInt& H1, ptrNodeInt& H2)
{
	ptrNodeInt fast;
	ptrNodeInt slow;

	if (H == NULL || H->pNext == NULL)
	{
		H1 = H;
		H2 = NULL;
	}
	else
	{
		slow = H;
		fast = H->pNext;

		while (fast != NULL)
		{
			fast = fast->pNext;
			if (fast != NULL)
			{
				slow = slow->pNext;
				fast = fast->pNext;
			}
		}

		H1 = H;
		H2 = slow->pNext;
		slow->pNext = NULL;
	}
}

void MoveNode(ptrNodeInt& p, ptrNodeInt& Head)
{
	ptrNodeInt temp = Head;

	if (temp != NULL)
	{
		Head = temp->pNext;
		temp->pNext = p;
		p = temp;
	}
}
ptrNodeInt Merge(ptrNodeInt H1, ptrNodeInt H2, int k)
{
	ptrNodeInt result = NULL;
	ptrNodeInt* temp = &result;

	while (1)
	{
		if (H1 == NULL)
		{
			*temp = H2;
			break;
		}
		else if (H2 == NULL)
		{
			*temp = H1;
			break;
		}
		if (compare(H1->data, H2->data, k) != 1)
		{
			MoveNode(*temp, H1);
		}
		else
		{
			MoveNode(*temp, H2);
		}
		temp = &((*temp)->pNext);

	}
	return(result);
}

void MergeSort(ptrNodeInt& Head, int k)
{
	ptrNodeInt temp = Head;
	ptrNodeInt H1;
	ptrNodeInt H2;

	if ((temp == NULL) || (temp->pNext == NULL))
	{
		return;
	}

	Split(temp, H1, H2);

	MergeSort(H1, k);
	MergeSort(H2, k);

	Head = Merge(H1, H2, k);

}

string NUMToString(NUM x, int k) {
	string s;
	s = "";
	if (x.sign == 1)
		s = "NaN";

	else {
		int mu = k - 1;

		int tontai = 0;
		for (ptrNode i = x.ListNum.pHead; i; i = i->pNext) {
			int val = i->data;
			for (int j = mu; mu >= 0; mu--) {
				if (tontai == 0) {
					if (val / int(pow(10, mu)) == 0) {
						val = val % int(pow(10, mu));
					}
					else tontai = 1;
				}

				if (tontai == 1) {
					s += (val / int(pow(10, mu))) + 48;
					val = val % int(pow(10, mu));
				}
			}
			mu = k - 1;
		}
	}
	return s;
}
int toInt(char* s)
{
	long long t = 0;
	while (*s >= '0' && *s <= '9')
	{
		t = t * 10 + int(*s - '0');
		s++;
	}
	return t;
}
int main(int argc, char* argv[])
{
	ifstream fi(argv[3], ios::in);
	ofstream fo(argv[4], ios::out);
	string yc;
	string s, s1, s2;
	s1 = "";
	s2 = "";
	int k = toInt(argv[2]);
	int n;
	fi >> n;
	fo << n << endl;
	string mode = argv[1];
	fi.ignore();

	//nếu chương trình nhập chức năng cộng trừ
	if (mode == "-comp") {
		int dau = 0;
		for (int i = 0; i < n; i++) {
			getline(fi, s);
			int j = 0;
			while (s[j] != ' ') {
				s1 += s[j];
				j++;
			}
			//
			NUM n1 = CreateNum(s1, k);
			for (; j < s.size(); j++) {

				if (s[j] == '+')
					dau = 1;
				else if (s[j] == '-')
					dau = 2;

				else if (s[j] != ' ')
					s2 += s[j];
				if ((s[j] == ' ' || j == s.size() - 1) && s2 != "") {
					//
					NUM n2 = CreateNum(s2, k);
					if (dau == 1)
						n1 = add(n1, n2, k);
					else {
						n1 = subtract(n1, n2, k);
					}
					dau = 0;
					s2 = "";
				}
			}
			n1 = reverseNum(n1);
			string str = NUMToString(n1, k);
			fo << str << endl;
			destroyNUM(n1);
			s1 = "";
		}
	}
	//nếu nhập chức năng so sánh
	else {
		ListInt a;
		createListInt(a);
		for (int i = 0; i < n; i++) {
			fi >> s;
			addTail(a, CreateNum(s, k));
			s = "";
		}
		if (mode == "-qsort") QuickSort(a, k);
		if (mode == "-msort") MergeSort(a.pHead, k);

		for (ptrNodeInt i = a.pHead; i; i = i->pNext) {
			string str = NUMToString(reverseNum(i->data), k);
			fo << str << endl;
		}
	}
	fi.close();
	fo.close();
	return 0;
}