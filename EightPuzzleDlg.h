
// EightPuzzleDlg.h: 头文件
//

#pragma once


// CEightPuzzleDlg 对话框
class CEightPuzzleDlg : public CDialogEx
{
	// 构造
public:
	CEightPuzzleDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EIGHTPUZZLE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	typedef struct Point {
		int x; int y;
		Point() {};
		Point(int x, int y) {
			this->x = x; this->y = y;
		}
	};
	typedef struct EightPuzzle {
		char Num[9];
		int Blank;
		bool operator==(const EightPuzzle& other) {
			for (int i = 0; i < 9; i++)
			{
				if (Num[i] != other.Num[i]) {
					return 0;
				}
				else
				{
					continue;
				}
			}
			return 1;
		};
		void operator=(const EightPuzzle& other) {
			for (int i = 0; i < 9; i++)
			{
				Num[i] = other.Num[i];
			}
			Blank = other.Blank;
		};
		void MoveUp() {
			int temp;
			temp = Num[Blank - 3];
			Num[Blank - 3] = '0';
			Num[Blank] = temp;
			Blank = Blank - 3;
		};
		void MoveDown() {
			int temp;
			temp = Num[Blank + 3];
			Num[Blank + 3] = '0';
			Num[Blank] = temp;
			Blank = Blank + 3;
		};
		void MoveLeft() {
			int temp;
			temp = Num[Blank - 1];
			Num[Blank - 1] = '0';
			Num[Blank] = temp;
			Blank = Blank - 1;
		};
		void MoveRight() {
			int temp;
			temp = Num[Blank + 1];
			Num[Blank + 1] = '0';
			Num[Blank] = temp;
			Blank = Blank + 1;
		};;

	};
	typedef struct H {
		int Up;
		int Down;
		int Left;
		int Right;
	};
	typedef struct status {
		EightPuzzle e;
		int f;
		int g;
		void operator=(const status& other) {
			e = other.e;
			f = other.f;
			g = other.g;
		};
	};
	typedef struct statusStack {
		int rear;
		status data[368880];
	};
	typedef struct arr {
		int flag[362880];
	};
	typedef struct result {
		int data[200];
		int rear;
	};
public:
	CPen blackPen;
	CBrush White;
	CFont StaticFont;
	CFont number;
	CRect mainRect;
	CDC* pdc;
	Point startPos;
	Point demoPos;
	Point targetPos;

	result* result1; //储存结果
	//arr* arr1;
	int G;				//步数
	/*char* start = new char[9];
	char* target = new char[9];*/

	EightPuzzle start;	//初始状态
	EightPuzzle target;	//目标状态
	EightPuzzle demo;	//演示状态
	int demo_i;
public:

	CString  C2S(char a) {
		//char x[1];
		//x[0] = a;
		CString str;
		str.Format(_T("%c"), a);
		return str;
	}
	void S2C(CString str, char* num) {
		USES_CONVERSION;
		for (int i = 0; i < 9; i++) {
			num[i] = T2A(str)[i];
		}
	}
	void DrawBlank(Point pos, int space) {
		CPen* oldPen = pdc->SelectObject(&blackPen);
		CRect blankRect;
		blankRect.top = pos.y; blankRect.left = pos.x; blankRect.bottom = blankRect.top + space; blankRect.right = blankRect.left + space;
		pdc->SetBkColor(RGB(255, 255, 255));
		pdc->Rectangle(&blankRect);
		pdc->SelectObject(&oldPen);
	}
	void DrawFrame(Point pos) {
		int space = 60;
		int temp = pos.y;
		for (int i = 0; i < 3; i++) {
			pos.x += space - 1;
			pos.y = temp;
			for (int j = 0; j < 3; j++) {
				pos.y += space - 1;
				DrawBlank(pos, space);
			}
		}
	}
	void DrawStaticFont(Point Pos, char* x) {
		Pos.x += 60;
		pdc->SelectObject(&StaticFont);
		USES_CONVERSION;
		pdc->SetBkMode(TRANSPARENT);
		pdc->TextOutW(Pos.x, Pos.y, A2T(x));
	}
	void DrawNum(Point Pos, char* x) {
		if ('8' >= x[8] && x[8] >= '0') {
			DrawFrame(Pos);
			Pos.x += 80; Pos.y += 70;
			int tempx = Pos.x;
			for (int i = 0; i < 9; i++) {
				pdc->SelectObject(number);
				USES_CONVERSION;
				if (x[i] != '0') {
					pdc->TextOutW(Pos.x, Pos.y, C2S(x[i]));
				}
				if ((i + 1) % 3 == 0) {
					Pos.y += 60;
				}
				Pos.x += 60;
				if ((i + 1) % 3 == 0) {
					Pos.x = tempx;
				}
			}
		}
	}
	
	int GetGap(EightPuzzle Start, EightPuzzle End) {
		int sum = 0;
		for (int i = 0; i < 9; i++) {
			if (Start.Num[i] != End.Num[i]) {
				sum++;
			}
		}
		//if (Start.Blank == End.Blank) { sum--; }
		return sum;
	}
	void PushStatus(statusStack* s, EightPuzzle e, int f, int g) {
		//28,28,28....10,10
		s->rear++;
		int i = s->rear - 1;
		while (s->data[i].f < f && i != 0) {
			s->data[i + 1].e = s->data[i].e;
			s->data[i + 1].f = s->data[i].f;
			s->data[i + 1].g = s->data[i].g;
			i--;
		}
		s->data[i + 1].e = e;
		s->data[i + 1].f = f;
		s->data[i + 1].g = g;
	}
	void PushStatusResult(statusStack* s, EightPuzzle e, int f, int g) {
		//1,1,1,2,2,2,3,....19,19,19
		s->rear++;
		int i = s->rear - 1;
		while (s->data[i].g > g && i != 0) {
			s->data[i + 1].e = s->data[i].e;
			s->data[i + 1].f = s->data[i].f;
			s->data[i + 1].g = s->data[i].g;
			i--;
		}
		s->data[i + 1].e = e;
		s->data[i + 1].f = f;
		s->data[i + 1].g = g;
	}
	status PopStatus(statusStack* s) {
		status S;
		S.e = s->data[s->rear].e;
		S.f = s->data[s->rear].f;
		S.g = s->data[s->rear].g;
		s->rear--;
		return S;
	}
	result* InitResult() {
		result* r = new result;
		r->rear = 0;
		for (int i = 0; i < 200; i++)
		{
			r->data[i] = -1;
		}
		return r;
	}
	void PushResult(result* r, int x) {
		r->rear++;
		r->data[r->rear] = x;
	}
	EightPuzzle VisitUp(EightPuzzle e) {
		if (e.Blank != 0 && e.Blank != 1 && e.Blank != 2) {
			e.MoveUp(); return e;
		}
		else {
			EightPuzzle err;
			for (int i = 0; i < 9; i++) {
				err.Num[i] = -1;
			}
			err.Blank = -1;
			return err;
		}

	}
	EightPuzzle VisitDown(EightPuzzle e) {
		if (e.Blank != 6 && e.Blank != 7 && e.Blank != 8) {
			e.MoveDown();
			return e;
		}
		else {
			EightPuzzle err;
			for (int i = 0; i < 9; i++) {
				err.Num[i] = -1;
			}
			err.Blank = -1;
			return err;
		}
	}
	EightPuzzle VisitLeft(EightPuzzle e) {
		if (e.Blank != 0 && e.Blank != 3 && e.Blank != 6) {
			e.MoveLeft();
			return e;
		}
		else {
			EightPuzzle err;
			for (int i = 0; i < 9; i++) {
				err.Num[i] = -1;
			}
			err.Blank = -1;
			return err;
		}
	}
	EightPuzzle VisitRight(EightPuzzle e) {
		if (e.Blank != 2 && e.Blank != 5 && e.Blank != 8) {
			e.MoveRight();
			return e;
		}
		else {
			EightPuzzle err;
			for (int i = 0; i < 9; i++) {
				err.Num[i] = -1;
			}
			err.Blank = -1;
			return err;
		}
	}

	H GetH(EightPuzzle Start, EightPuzzle End) {
		H h;
		if (!(Start.Blank != 0 && Start.Blank != 1 && Start.Blank != 2)) {
			h.Up = 100;
		}
		else {
			h.Up = GetGap(VisitUp(Start), End);
		}
		if (!(Start.Blank != 6 && Start.Blank != 7 && Start.Blank != 8)) {
			h.Down = 100;
		}
		else {
			h.Down = GetGap(VisitDown(Start), End);
		}
		if (!(Start.Blank != 0 && Start.Blank != 3 && Start.Blank != 6)) {
			h.Left = 100;
		}
		else {
			h.Left = GetGap(VisitLeft(Start), End);

		}
		if (!(Start.Blank != 2 && Start.Blank != 5 && Start.Blank != 8)) {
			h.Right = 100;
		}
		else {
			h.Right = GetGap(VisitRight(Start), End);
		}
		return h;
	}
	bool solvable(EightPuzzle Start, EightPuzzle Target) {
		int i, j;
		int count_num = 0, count_target = 0;
		for (i = 0; i < 9; i++)
		{
			for (j = 0; j < i; j++)
			{
				if ((Start.Num[j] < Start.Num[i]) && (Start.Num[j] != '0'))
				{
					count_num++;
				}
				if (Target.Num[j] < Target.Num[i] && Target.Num[j] != '0')
				{
					count_target++;
				}
			}
		}
		if ((count_num + count_target) % 2 == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool isNum(CString str) {
		char str1[9];
		S2C(str, str1);
		for (int i = 0; i < 9; i++) {
			if (str1[i] < '9' && str1[i] >= '0') {
				continue;
			}
			else {
				return false;
			}
		}
		return true;
	}
	bool isRepeat(CString str) {
		char str1[9];
		S2C(str, str1);
		int arr[9];
		for (int i = 0; i < 9; i++) {
			arr[i] = 0;
		}
		for (int i = 0; i < 9; i++) {
			if (arr[str1[i]-'0'] != 1) {
				arr[str1[i]-'0'] = 1;
				continue;
			}
			else {
				return true;
			}
		}
		return false;
	}
	int E2I(EightPuzzle e) {
		int temp1 = 0;
		int temp2 = 362880;
		for (int i = 0; i < 9; i++) {
			temp1 += (temp2 / (9 - i)) * (((e.Num[i]) - '0' + 1) - 1);
			temp2 /= (9 - i);
		}
		temp1 += e.Blank;
		return temp1 - 1;
	}
	void FillResultText(CString& str, result *r) {
		for (int i = r->rear; i > 0; i--) {
			switch (r->data[i])
			{
			case 1: 
				str += _T("上 ");
				break;
			case 2:
				str += _T("下 ");
				break;
			case 3:
				str += _T("左 ");
				break;
			case 4:
				str += _T("右 ");
				break;
			default:
				break;
			}
		}
	}
	
	void Solve(EightPuzzle Start, EightPuzzle Target) {
		if (!solvable(Start, Target)) {
			MessageBox(_T("此题无解！"), _T("提示"));
		}
		else {
			//声明数组判重
			arr* arr1 = new arr;
			for (int i = 0; i < 362880; i++) {
				arr1->flag[i] = 0;
			}
			//储存遍历时的状态
			statusStack* S;
			S = (statusStack*)malloc(sizeof(statusStack));
			S->rear = 0;
			//储存结果
			statusStack* SS;
			SS = (statusStack*)malloc(sizeof(statusStack));
			SS->rear = 0;
			PushStatusResult(SS,start, 0, 0);//g=0
			while (!(Start == Target)) {
				//Start.Print();
				G++;
				H h1 = GetH(Start, Target);
				if (h1.Up < 100 && arr1->flag[E2I(VisitUp(Start))] != 1) {
					PushStatus(S, VisitUp(Start), h1.Up + G, G);
					PushStatusResult(SS, VisitUp(Start), h1.Up + G, G);
					arr1->flag[E2I(VisitUp(Start))] = 1;
				}
				if (h1.Down < 100 && arr1->flag[E2I(VisitDown(Start))] != 1) {
					PushStatus(S, VisitDown(Start), h1.Down + G, G);
					PushStatusResult(SS, VisitDown(Start), h1.Down + G, G);
					arr1->flag[E2I(VisitDown(Start))] = 1;
				}
				if (h1.Left < 100 && arr1->flag[E2I(VisitLeft(Start))] != 1) {
					PushStatus(S, VisitLeft(Start), h1.Left + G, G);
					PushStatusResult(SS, VisitLeft(Start), h1.Left + G, G);
					arr1->flag[E2I(VisitLeft(Start))] = 1;
				}
				if (h1.Right < 100 && arr1->flag[E2I(VisitRight(Start))] != 1) {
					PushStatus(S, VisitRight(Start), h1.Right + G, G);
					PushStatusResult(SS, VisitRight(Start), h1.Right + G, G);
					arr1->flag[E2I(VisitRight(Start))] = 1;
				}
				status S1 = PopStatus(S);



				Start = S1.e;
				G = S1.g;
			}
			//释放内存
			delete(arr1);
			free(S);
			result1 = InitResult();
			int statusAmount = SS->rear;
			status SS1;
			while (!(SS1.e == target)) {
				SS1 = PopStatus(SS);
			}
			
			//SS1.e == target
			status temp = PopStatus(SS); //初始化
			while (SS->rear != 0 && !(SS1.e == start))
			{				
				while (SS->rear != 0 && temp.g != SS1.g - 1 || GetGap(temp.e,SS1.e)!=2){		//temp为上一步
					temp = PopStatus(SS);
				}
				//不断查找上一步
				if (VisitUp(temp.e) == SS1.e) {
					PushResult(result1, 1);
					SS1 = temp;
				}else if (VisitDown(temp.e) == SS1.e) {
					PushResult(result1, 2);
					SS1 = temp;
				}else if (VisitLeft(temp.e) == SS1.e) {
					PushResult(result1, 3);
					SS1 = temp;
				}else if (VisitRight(temp.e) == SS1.e) {
 					PushResult(result1, 4);
					SS1 = temp;
				}
				else {
					temp = PopStatus(SS);
				}
			}
			free(SS);
			int moveTimes = result1->rear;
			CString resultText("");
			FillResultText(resultText,result1);		//把结果转换并且填充到结果字符串内
			GetDlgItem(IDC_EDIT4)->SetWindowTextW(resultText);
			CString str;
			str.Format(_T("解题成功！搜索了%d种状态,解题需要移动%d步。"), statusAmount, moveTimes);
			MessageBox(str, _T("提示"));
		}
	}
	// 实现
protected:
	HICON m_hIcon;

	//生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
