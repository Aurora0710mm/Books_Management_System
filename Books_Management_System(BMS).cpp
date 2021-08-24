// 图书管理系统
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

/*定义图书有关的数据结构*/
class book
{
	public:
		
    int BookNum;         /*图书编号*/
    char BookName[20];   /*图书名字*/
    char BookAuthor[15];/*图书作者*/
    int nowCount;        /*图书现存量*/
    int allCount;        /*图书库存量*/
};
 
/*定义与读者有关的数据结构*/
class reader
{
	public:
    int ReaderNum;         /*读者编号*/
    char ReaderName[20];   /*读者的姓名*/
    char ReaderSex[8];     /*读者的性别*/
    int ReaderAge;         /*读者的年龄*/
    char ReaderTel[15];    /*读者的电话*/
    int ReaderCount;       /*读者目前已经借阅的数目*/
    char password[16];     /*读者证件密码*/
    char date[10][100];         /*读者借阅日期*/
    int ReaderBorrow[10];  /*当前已经借阅的图书编号*/
    char BorrowName[10][40];/*已经借阅的图书名字*/
};
 
/*定义与每条图书纪录有关的数据结构*/
typedef struct booknode
{
    struct book data;        /*数据域*/
    struct booknode*next;   /*指针域*/
}BookNode;
 
/*定义与每条读者纪录有关的数据结构*/
typedef struct readernode
{
    struct reader data;       /*数据域*/
    struct readernode*next;  /*指针域*/
}ReaderNode;

/*定义图书馆中读者和藏书的总数目*/
struct rbnum
{
    int rnum;              /*读者总个数*/
    int bnum;              /*书籍总本数*/
}allRBnum;
 
 
/*******************************************/
/*函数声明*/
BookNode *OpenBook();                                     /*打开图书库文件*/
ReaderNode *OpenReader();                                 /*打开读者库文件*/
void InBook(BookNode *pbook);                             /*图书录入*/
void SaveBook(BookNode *pbook);                           /*图书信息保存*/
void SaveReader(ReaderNode *preader);                     /*读者信息保存*/
void BorrowBook(BookNode *pbook, ReaderNode *preader);    /*图书借阅*/
void ReturnBook(BookNode *pbook, ReaderNode *preader);    /*图书归还*/
void LookBook(BookNode *pbook);                           /*浏览图书库*/
void LookReader(ReaderNode *preader);                     /*浏览读者库*/
void InReader(ReaderNode *preader);                       /*借书证办理*/
void FindReaderAll(ReaderNode *preader);                  /*读者信息查询*/
ReaderNode *LoginReader(ReaderNode *preader);             /*读者登陆验证*/
int LoginAdmin();                                         /*管理员登陆验证*/
void ModifyReader(ReaderNode *preader);                   /*修改读者信息*/
void ModifyBook(BookNode *pbook);                         /*修改图书信息*/
void GotoXY(int x, int y);                                /*光标定位*/
void erase(int n);                                        /*擦除菜单选项*/
void menu(int n);                                         /*菜单选项*/
void FindBook(BookNode *pbook);                           /*图书搜索*/
void LocalTime(char *strTime);                            /*获取系统本地时间*/
/*******************************************/
/*打开图书文件book，将其存入链表中*/
BookNode *OpenBook()
{
    FILE *fpbook;
    BookNode *pb = NULL,*rb = NULL;
    allRBnum.bnum =0;         /*初始化藏书0本*/
    BookNode *pbook =(BookNode *)malloc(sizeof(BookNode));  /*头指针*/
    if(!pbook)
    {
        printf("空间申请失败\n");
        exit(0);
    }
    pbook->next = NULL;
    rb = pbook;
    fpbook =fopen(".\\book.txt","a+");  /*追加的方式打开一个二进制文件*/
    if (!fpbook)
    {
        printf("文件打开失败\n");
        exit(0);
    }
    while (!feof(fpbook))
    {
        pb = (BookNode*)malloc(sizeof(BookNode));   /*申请空间*/
        if (!pb)
        {
            printf("空间申请失败\n");
            exit(0);
        }
        if (fread(pb,sizeof(BookNode),1,fpbook)== 1)
        {
            pb->next =NULL;
            rb->next =pb;
            rb = pb;             /*rb指针向后移一位*/
            allRBnum.bnum+= pb->data.allCount;    /*总图书数目统计*/
        }
    }
    fclose(fpbook);          /*关闭文件*/
//  printf("打开文件成功!图书馆总共藏书：%d本\n",allRBnum.bnum);
    return pbook;
}
 
/*打开读者文件reader，将其存入链表中*/
ReaderNode *OpenReader()
{
    FILE *fpreader;
    ReaderNode *pr = NULL,*rr = NULL;
    allRBnum.rnum =0;         /*初始化读者人数0位*/
    ReaderNode *preader =(ReaderNode *)malloc(sizeof(ReaderNode)); /*头指针*/
    if (!preader)
    {
        printf("空间申请失败\n");
        exit(0);
    }
    preader->next =NULL;
    rr = preader;
    fpreader =fopen(".\\reader.txt","a+");   /*追加的方式打开一个二进制文件*/
    if (!fpreader)
    {
        printf("文件打开失败\n");
        exit(0);
    }
    while(!feof(fpreader))
    {
        pr = (ReaderNode*)malloc(sizeof(ReaderNode));   /*申请空间*/
        if (!pr)
        {
            printf("空间申请失败\n");
            exit(0);
        }
        if(fread(pr,sizeof(ReaderNode),1,fpreader) == 1)
        {
            pr->next =NULL;
            rr->next =pr;
            rr = pr;             /*rr指针向后移一位*/
            ++allRBnum.rnum;      /*读者数目+1*/
        }
    }
    fclose(fpreader);          /*关闭文件*/
//  printf("打开文件成功!拥有有所读者数：%d位\n",allRBnum.rnum);
    return preader;
}
 
/*图书录入*/
void InBook(BookNode *pbook)
{
    printf(">>>>>>>>>>>>>>>>>>>>>>>>   图书登记入库   <<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    BookNode *pb = pbook;
    BookNode *pt =pbook;      /*pt临时指针用于遍历整个图书库*/
    BookNode *pr =(BookNode *)malloc(sizeof(BookNode));
    while (pb->next !=NULL)   /*找到链表的尾部 开始添加书目*/
    {
        pb = pb->next;
    }
   
    int i = 1;        /*用于临时报号*/
    printf("提示：当书号为0时，结束图书录入!\n");
    printf("________________________________________________________________\n");
    printf("现在正在进行第 %d 本书登记.\n",i++);
    printf("书号：");
    scanf("%d",&(pr->data.BookNum));   /*书号*/
    while(pr->data.BookNum != 0)
    {
        printf("书名：");
        scanf("%s",pr->data.BookName);   /*书名*/
        printf("作者名：");
        scanf("%s",pr->data.BookAuthor);   /*作者名*/
        pr->data.nowCount= 1;            /*现存量初始化为1*/
        pr->data.allCount= 1;            /*总存储量初始化为1*/
        while (pt->next!= NULL)
        {
            if(pr->data.BookNum != pt->data.BookNum)
                pt =pt->next;
            else                             /*说明该图书已经存在*/
            {
                pt->data.nowCount+= 1;      /*现存书目+1*/
                pt->data.allCount+= 1;      /*总数目+1*/
                break;
            }
        }
        if (pt->next ==NULL)          /*图书还未入库 则入库*/
        {
            if(pr->data.BookNum != pt->data.BookNum)
            {
                pr->next= NULL;
                pb->next= pr;             /*将新建的结点插入链表尾部*/
                pb = pr;
            }
            else
            {
                pt->data.nowCount+= 1;      /*现存书目+1*/
                pt->data.allCount+= 1;      /*总数目+1*/
            }
        }
        printf("操作成功.\n");
        pr = (BookNode*)malloc(sizeof(BookNode));   /*申请下一个空间*/
        printf("现在正在进行第 %d 本书登记.\n",i++);
        printf("书号：");
        scanf("%d",&(pr->data.BookNum));   /*书号*/
        pt = pbook;                   /*pt临时指针移动到头指针位置*/
    }
    printf("________________________________________________________________\n");
    SaveBook(pbook);             /*图书信息保存*/
    printf("\n>>>图书入库完毕<<<--按任意键回主菜单>>>>>>>>>>");
    getch();
    system("cls");
    return;
}
/*图书保存*/
void SaveBook(BookNode *pbook)
{
    BookNode *pb =pbook->next;
    FILE *fpbook;
    fpbook =fopen(".\\book.txt","w");                  /*只写的方式打开文件*/
    if (!fpbook)
    {
        printf("文件打开失败\n");
        exit(0);
    }
    while (pb != NULL)
    {
        if(fwrite(pb,sizeof(BookNode),1,fpbook) == 1)  /*每次写一条记录*/
        {
            pb =pb->next;
        }
    }
    fclose(fpbook);
    return;
}
/*读者信息保存*/
void SaveReader(ReaderNode *preader)
{
    ReaderNode *pr =preader->next;
    FILE *fpreader;
    fpreader =fopen(".\\reader.txt","w");                  /*只写的方式打开文件*/
    if (!fpreader)
    {
        printf("文件打开失败\n");
        exit(0);
    }
    while (pr != NULL)
    {
        if(fwrite(pr,sizeof(ReaderNode),1,fpreader) == 1)   /*每次写一条记录*/
        {
            pr =pr->next;
        }
    }
    fclose(fpreader);
    return;
}
 
/*图书借阅*/ 
void BorrowBook(BookNode *pbook, ReaderNode *preader)
{
    printf(">>>>>>>>>>>>>>>>>>>>>>>>   读者图书借阅   <<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    int booknum = 0, flag= 0,time = 3, i = 2;
    BookNode *pt =pbook;           /*图书临时指针*/
    ReaderNode *rt =NULL;       /*读者临时指针*/
    ReaderNode *rr =(ReaderNode *)malloc(sizeof(ReaderNode));  /*读者登陆临时节点*/
    /******************************************/
    if((rt=LoginReader(preader)) == NULL)   /*读者登陆验证*/
    {
        printf("读者验证失败.\n");
        return;
    }
    /******************************************/
    printf("________________________________________________________________\n");
    printf("输入要借阅书籍的索书号：");
    scanf("%d",&booknum);
    while(pt->next!=NULL)
    {
        if(pt->data.BookNum == booknum)
        {
            if(pt->data.nowCount >= 1)
            {
                printf("该书现存量：%d 本，可以借阅.\n",pt->data.nowCount);
                LocalTime(rt->data.date[rt->data.ReaderCount]);   /*登记当前借阅图书的日期*/
                rt->data.ReaderBorrow[rt->data.ReaderCount]= pt->data.BookNum;  /*纪录当前图书编号*/ 
                strcpy(rt->data.BorrowName[rt->data.ReaderCount],pt->data.BookName);/*纪录当前图书名字*/
                --pt->data.nowCount;  /*图书现在库存-1*/
                ++rt->data.ReaderCount;/*读者借阅本数+1*/ 
                printf(">>>借书成功<<<\n");
                printf("提醒：请在30天之内归还本书!!!超期按每天0.1元收费!!!\n");
                SaveBook(pbook);             /*图书信息保存*/
                SaveReader(preader);         /*读者信息保存*/
                printf("________________________________________________________________\n");
                printf("\n>>>图书借阅完毕<<<--按任意键回主菜单>>>>>>>>>>");
                getch();
                system("cls");
                return;
            }
            printf(">>>很遗憾!该本图书库存目前为空，不可借阅<<<\n");
            printf("________________________________________________________________\n");
            printf("\n>>>图书借阅失败<<<--按任意键回主菜单>>>>>>>>>>");
            getch();
            system("cls");
            return;
        }
        pt =pt->next;            /*遍历图书库*/
    }
    if (pt->next ==NULL)
    {
        printf(">>>很遗憾!没有找到该图书<<<\n");
    }
    printf("________________________________________________________________\n");
    printf("\n>>>图书借阅失败<<<--按任意键回主菜单>>>>>>>>>>");
    getch();
    system("cls");
    return;
} 
/*图书归还*/
void ReturnBook(BookNode *pbook, ReaderNode *preader)
{
    printf(">>>>>>>>>>>>>>>>>>>>>>>>   读者图书归还   <<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    int booknum = 0, i =0;
    BookNode *pt = pbook;
    ReaderNode *pr =preader;
    printf("________________________________________________________________\n");
    printf("归还索书号：");
    scanf("%d",&booknum);
    while (pt->next !=NULL)      /*遍历图书库*/
    {
        if(pt->next->data.BookNum == booknum)
        {
            while(pr->next != NULL)                /*读者借书信息修改*/
            {
                for (i =0; i < pr->data.ReaderCount; ++i)   /*遍历每个读者所借的所有书籍*/
                    if(pr->data.ReaderBorrow[i] == pt->next->data.BookNum)
                    {
                        ++pt->next->data.nowCount;              /*归还图书现在库存+1*/
                        pr->data.ReaderBorrow[i]= 0;         /*借书号注销*/
                        strcpy(pr->data.date[i],"");          /*借书时间注销*/
                        strcpy(pr->data.BorrowName[i],"");    /*借书名字注销*/
                        --pr->data.ReaderCount;               /*借书量-1*/
                        printf("归还成功\n");
                        printf("________________________________________________________________\n");
                        printf("\n>>>图书归还完毕<<<--按任意键回主菜单>>>>>>>>>>");
                        SaveBook(pbook);             /*图书信息保存*/
                        SaveReader(preader);         /*读者信息保存*/
                        getch();
                        system("cls");
                        return;
                    }
                    pr =pr->next;
            }
            break;
        }
        pt = pt->next;
    }
    if (pt->next==NULL)
    {
        printf("非本图书馆图书.\n");
        printf("________________________________________________________________\n");
        printf("\n>>>图书归还失败<<<--按任意键回主菜单>>>>>>>>>>");
        getch();
        system("cls");
        return;
    }
    printf(">>>请确认图书索书号输入正确<<<\n");
    printf("本馆该书没有被借出.\n");
    printf("\n>>>图书归还失败<<<--按任意键回主菜单>>>>>>>>>>");
    getch();
    system("cls");
    return;
}
/*浏览图书信息*/
void LookBook(BookNode *pbook)
{
    printf(">>>>>>>>>>>>>>>>>>>>>>>>   浏览图书信息   <<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    BookNode *pt = pbook;
    printf("________________________________________________________________\n");
    printf("\n书号\t\t书名\t\t作者\t\t现存量\t库存量\n");
    while (pt->next !=NULL)
    {
        printf("%d\t\t",pt->next->data.BookNum);    /*书号*/
        printf("%s\t\t",pt->next->data.BookName);   /*书名*/
        printf("%s\t\t",pt->next->data.BookAuthor);/*作者*/
        printf("%d\t",pt->next->data.nowCount);   /*现存量*/
        printf("%d\t\n",pt->next->data.allCount);   /*库存量*/
        pt = pt->next;
    }
    printf("________________________________________________________________\n");
    printf("\n>>>浏览完毕<<<--按任意键回主菜单>>>>>>>>>>");
    getch();
    system("cls");
    return;
}
/*浏览读者信息*/
void LookReader(ReaderNode *preader)
{
    printf(">>>>>>>>>>>>>>>>>>>>>>>>   浏览读者信息   <<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    /*管理员权限验证*/
    if (LoginAdmin() == 0)
    {
        printf("管理员验证失败.\n");
        printf("\n>>>没有管理员权限<<<--按任意键回主菜单>>>>>>>>>>");
        getch();
        system("cls");
        return;
    }
    printf("管理员验证成功.\n");
    /******************************/
    ReaderNode *pt =preader;
    printf("________________________________________________________________\n");
    printf("\n借书证号\t姓名\t性别\t年龄\t电话\t已借阅图书本数\n");
    while (pt->next !=NULL)
    {
        printf("%d\t",pt->next->data.ReaderNum);     /*证件号*/
        printf("%s\t",pt->next->data.ReaderName);      /*姓名*/
        printf("%s\t",pt->next->data.ReaderSex);       /*性别*/
        printf("%d\t",pt->next->data.ReaderAge);       /*年龄*/
        printf("%s\t",pt->next->data.ReaderTel);       /*电话*/
        printf("%d\t\n",pt->next->data.ReaderCount);   /*当前借阅图书本数*/
        pt = pt->next;
    }
    printf("________________________________________________________________\n");
    printf("\n>>>浏览完毕<<<--按任意键回主菜单>>>>>>>>>>");
    getch();
    system("cls");
    return;
}
 
/*读者借书证办理*/
void InReader(ReaderNode *preader)
{
    printf(">>>>>>>>>>>>>>>>>>>>>>>>   读者证件办理   <<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    ReaderNode *pr =preader;
    ReaderNode *pt =preader;      /*pt临时指针用于遍历整个读者库*/
    ReaderNode *rr =(ReaderNode *)malloc(sizeof(ReaderNode));
    int i = 1;                     /*用于临时报号*/
    /*管理员权限验证*/
    if (LoginAdmin() == 0)
    {
        printf("管理员验证失败.\n");
        printf("\n>>>没有管理员权限<<<--按任意键回主菜单>>>>>>>>>>");
        getch();
        system("cls");
        return;
    }
    printf("管理员验证成功.\n");
    /***************************************************/
    while (pr->next !=NULL)       /*找到链表的尾部  开始添加读者*/
    {
        pr = pr->next;
    }
 
    printf("提示：当证件号为0时，结束证件办理!\n");
    printf("________________________________________________________________\n");
    printf("现在正在进行第 %d 位读者证件办理.\n",i++);
    printf("借书证号：");
    scanf("%d",&(rr->data.ReaderNum));        /*借书证号*/
    while(rr->data.ReaderNum != 0)
    {
        printf("姓名：");
        scanf("%s",rr->data.ReaderName);      /*姓名*/
        printf("性别：");
        scanf("%s",rr->data.ReaderSex);       /*性别*/
        printf("年龄：");                    
        scanf("%d",&(rr->data.ReaderAge));    /*年龄*/
        printf("电话：");
        scanf("%s",rr->data.ReaderTel);       /*电话*/
        rr->data.ReaderCount= 0;             /*已经借阅图书本数初始化为0*/
        strcpy(rr->data.password,"123456");   /*证件密码默认为：123456*/
        while (pt->next!= NULL)
        {
            if(rr->data.ReaderNum != pt->data.ReaderNum)
                pt =pt->next;
            else                              /*说明该读者已经存在*/
            {
                printf("该证件号已经存在,证件办理失败\n");
                printf("________________________________________________________________\n");
                printf("\n>>>办理失败<<<--按任意键回主菜单>>>>>>>>>>");
                getch();
                system("cls");
                return;
            }
        }
        if (pt->next ==NULL)          /*该证件号可以办理*/
        {
            rr->next =NULL;
            pr->next =rr;             /*将新建的结点插入链表尾部*/
            pr = rr;
        }
        printf(">>>证件办理成功<<<\n");
        printf("提醒：借书证默认密码全部是\"123456\"\n");
        rr = (ReaderNode*)malloc(sizeof(ReaderNode));   /*申请下一个空间*/
        printf("\n现在正在进行第 %d 位读者证件办理.\n",i++);
        printf("借书证号：");
        scanf("%d",&(rr->data.ReaderNum));   /*证件号*/
        pt = preader;                   /*pt临时指针移动到头指针位置*/
    }
    printf("________________________________________________________________\n");
    SaveReader(preader);    /*证件信息存档*/
    printf("\n>>>证件办理完毕<<<--按任意键回主菜单>>>>>>>>>>");
    getch();
    system("cls");
    return;
}
/*读者信息查询*/
void FindReaderAll(ReaderNode *preader)
{
    printf(">>>>>>>>>>>>>>>>>>>>>>>>   读者信息查询   <<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    int i = 0;
    ReaderNode *rt = NULL;
    /*****************************/
    if((rt=LoginReader(preader)) == NULL)
    {
        printf("读者验证失败.\n");
        return;
    }
    /*****************************/
    printf("________________________________________________________________\n");
    printf("借书证\t\t姓名\t性别\t年龄\t电话\t已借阅图书本数\t密码\n");
    printf("%d\t",rt->data.ReaderNum);     /*借书证*/
    printf("%s\t",rt->data.ReaderName);    /*姓名*/
    printf("%s\t",rt->data.ReaderSex);     /*性别*/
    printf("%d\t",rt->data.ReaderAge);     /*年龄*/
    printf("%s\t",rt->data.ReaderTel);     /*电话*/
    printf("%d\t",rt->data.ReaderCount);   /*已借阅图书本数*/
    printf("%s\t",rt->data.password);      /*密码*/
 
    printf("\n\n已借图书书目\t图书名字\t借书日期\n");
    for (i = 0; i <rt->data.ReaderCount; ++i)
    {
        printf("%d\t\t",rt->data.ReaderBorrow[i]);
        printf("%s\t\t",rt->data.BorrowName[i]);
        printf("%s\n",rt->data.date[i]);
    }
    printf("________________________________________________________________\n");
    printf("\n>>>信息查询完毕<<<--按任意键回主菜单>>>>>>>>>>");
    getch();
    system("cls");
    return;
 
}
/*修改图书信息*/
void ModifyBook(BookNode *pbook)
{
    printf(">>>>>>>>>>>>>>>>>>>>>>>>   图书信息修改   <<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    /*管理员权限验证*/
    if (LoginAdmin() == 0)
    {
        printf("管理员验证失败.\n");
        printf("\n>>>没有管理员权限<<<--按任意键回主菜单>>>>>>>>>>");
        getch();
        system("cls");
        return;
    }
    printf("管理员验证成功.\n");
    /*************************/
    int nSelect = 0;
    int BookNum = 0;
    char BookName[20];
    char BookAuthor[15];
    BookNode *pt = pbook;
    BookNode *p2t = pbook;
    BookNode *pr =(BookNode *)malloc(sizeof(BookNode));  /*临时图书指针*/
    printf("________________________________________________________________\n");
    printf("输入图书编号：");
    scanf("%d",&pr->data.BookNum);
    while (pt->next !=NULL)
    {
        if(pt->data.BookNum == pr->data.BookNum)
            break;
        pt = pt->next;
    }
    if (pt->next ==NULL)
    {
        if(pt->data.BookNum != pr->data.BookNum)
        {
            printf("未找到该编号图书.\n");
            printf("\n>>>信息修改失败<<<--按任意键回主菜单>>>>>>>>>>");
            getch();
            system("cls");
            return;
        }
    }
    printf(">>>>>图书信息如下<<<<<\n");
    printf("________________________________________________________________\n");
    printf("\n书号\t\t书名\t\t作者\t\t现存量\t库存量\n");
    printf("%d\t\t",pt->data.BookNum);       /*书号*/
    printf("%s\t\t",pt->data.BookName);      /*书名*/
    printf("%s\t\t",pt->data.BookAuthor);  /*作者*/
    printf("%d\t",pt->data.nowCount);      /*现存量*/
    printf("%d\t\n",pt->data.allCount);    /*库存量*/
    printf("________________________________________________________________\n");
    printf(">>>>可以选择的修改信息<<<<\n");
    printf("1.图书编号\n");
    printf("2.图书名字\n");
    printf("3.图书作者\n");
    printf(">>>>选择要修改的信息：");
    scanf("%d",&nSelect);
    switch(nSelect)
    {
    case 1:
        {
            printf("请输入新的图书编号：");
            scanf("%d",&BookNum);
            while(p2t->next != NULL)
            {
                if(p2t->next->data.BookNum == BookNum)
                    break;
                p2t =p2t->next;
            }
            if(p2t->next == NULL)
            {
                printf("该编号图书已存在，修改失败.\n");
                return;
            }
            pt->data.BookNum= BookNum;
            printf("修改图书编号成功.\n");
        }
        break;
    case 2:
        {
            printf("输入新的图书名字：");
            scanf("%s",BookName);
            strcpy(pt->data.BookName,BookName);
            printf("修改图书名字成功.\n");
        }
        break;
    case 3:
        {
            printf("输入新图书作者：");
            scanf("%s",BookAuthor);
            strcpy(pt->data.BookAuthor,BookAuthor);
            printf("修改图书作者成功.\n");
        }
        break;
    }
    printf("________________________________________________________________\n");
    SaveBook(pbook);             /*图书信息保存*/
    printf("\n>>>信息修改完毕<<<--按任意键回主菜单>>>>>>>>>>");
    getch();
    system("cls");
    return;
}
/*修改读者信息*/
void ModifyReader(ReaderNode *preader)
{
    printf(">>>>>>>>>>>>>>>>>>>>>>>>   读者信息修改   <<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    int nSelect = 0;
    char ReaderTel[15];
    char password[16];
    ReaderNode *rt = NULL;
    if((rt=LoginReader(preader)) == NULL)
    {
        printf("读者验证失败.\n");
        return;
    }
    printf("________________________________________________________________\n");
    printf(">>>>可修改的内容<<<<\n");
    printf("1.读者电话\n");
    printf("2.读者密码\n\n");
    printf(">>>>选择要修改的内容：");
    scanf("%d",&nSelect);
    switch(nSelect)
    {
    case 1:
        {
            printf("输入新的电话号码：");
            scanf("%s",ReaderTel);
            strcpy(rt->data.ReaderTel,ReaderTel);
            printf("修改读者电话号码成功.\n");
        }
        break;
    case 2:
        {
            printf("输入新的密码：");
            scanf("%s",password);
            strcpy(rt->data.password,password);
            printf("修改读者密码成功.\n");
        }
        break;
    }
    printf("________________________________________________________________\n");
    SaveReader(preader);         /*读者信息保存*/
    printf("\n>>>信息修改完毕<<<--按任意键回主菜单>>>>>>>>>>");
    getch();
    system("cls");
    return;
}

/*图书搜索*/
void FindBook(BookNode *pbook)
{
    printf(">>>>>>>>>>>>>>>>>>>>>>>>   图书资源搜索   <<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    int nSelect = 0;
    int BookNum = 0;         /*图书编号*/
    char BookName[20];   /*图书名字*/
    char BookAuthor[15];/*图书作者*/
    BookNode *pt = pbook;
    printf(">>>>可选的搜索方式<<<<\n");
    printf("1.图书名\n");
    printf("2.作者名\n");
    printf("3.索书号\n");
    printf(">>>>选择搜索方式：");
    scanf("%d",&nSelect);
    switch(nSelect)
    {
    case 1:
        {
            printf("输入图书名：");
            scanf("%s",BookName);
            while(pt->next != NULL)
            {
                if(strcmp(pt->next->data.BookName,BookName)==0)
                {
                    printf(">>>存在此书<<<\n");
                    printf(">>>>>图书信息如下<<<<<\n");
                    printf("________________________________________________________________\n");
                    printf("\n书号\t\t书名\t\t作者\t\t现存量\t库存量\n");
                    printf("%d\t\t",pt->next->data.BookNum);       /*书号*/
                    printf("%s\t\t",pt->next->data.BookName);      /*书名*/
                    printf("%s\t\t",pt->next->data.BookAuthor);    /*作者*/
                    printf("%d\t",pt->next->data.nowCount);        /*现存量*/
                    printf("%d\t\n",pt->next->data.allCount);      /*库存量*/
                    printf("________________________________________________________________\n");
                    break;
                }
                pt =pt->next;
            }
        }
        break;
    case 2:
        {
            printf("输入作者名：");
            scanf("%s",BookAuthor);
            while(pt->next != NULL)
            {
                if (strcmp(pt->next->data.BookAuthor,BookAuthor)==0)
                {
                    printf(">>>存在此书<<<\n");
                    printf(">>>>>图书信息如下<<<<<\n");
                    printf("________________________________________________________________\n");
                    printf("\n书号\t\t书名\t\t作者\t\t现存量\t库存量\n");
                    printf("%d\t\t",pt->next->data.BookNum);       /*书号*/
                    printf("%s\t\t",pt->next->data.BookName);      /*书名*/
                    printf("%s\t\t",pt->next->data.BookAuthor);    /*作者*/
                    printf("%d\t",pt->next->data.nowCount);        /*现存量*/
                    printf("%d\t\n",pt->next->data.allCount);      /*库存量*/
                    printf("________________________________________________________________\n");
                    break;
                }
                pt =pt->next;
            }
        }
        break;
    case 3:
        {
            printf("输入索书号：");
            scanf("%d",&BookNum);
            while(pt->next != NULL)
            {
                if(pt->next->data.BookNum == BookNum)
                {
                    printf(">>>存在此书<<<\n");
                    printf(">>>>>图书信息如下<<<<<\n");
                    printf("________________________________________________________________\n");
                    printf("\n书号\t\t书名\t\t作者\t\t现存量\t库存量\n");
                    printf("%d\t\t",pt->next->data.BookNum);       /*书号*/
                    printf("%s\t\t",pt->next->data.BookName);      /*书名*/
                    printf("%s\t\t",pt->next->data.BookAuthor);    /*作者*/
                    printf("%d\t",pt->next->data.nowCount);        /*现存量*/
                    printf("%d\t\n",pt->next->data.allCount);      /*库存量*/
                    printf("________________________________________________________________\n");
                    break;
                }
                pt =pt->next;
            }
        }
        break;
    default:printf("error\n");Sleep(100);system("cls");return;
    }
    if (pt->next == NULL)
    {
        printf("<未找到相应图书>\n");
        printf("\n>>>图书搜索失败<<<--按任意键回主菜单>>>>>>>>>>");
    }
    else
        printf("\n>>>图书搜索成功<<<--按任意键回主菜单>>>>>>>>>>");
    getch();
    system("cls");
    return;
}
/*管理员登陆验证*/
int LoginAdmin()
{
    char adminzh[16];
    char adminmm[16];
    printf("管理员账号登陆：");
    scanf("%s",adminzh);      /*管理员账号*/
    printf("管理员密码：");          
    scanf("%s",adminmm);      /*管理员密码*/
    if(!(strcmp("admin",adminzh) == 0 &&strcmp("1234",adminmm) == 0))
        return 0;
    return 1;
}
/*读者登陆验证*/
ReaderNode *LoginReader(ReaderNode *preader)
{
    int time = 3, i = 2;
    ReaderNode *rt =preader;       /*读者临时指针*/
    ReaderNode *rr =(ReaderNode *)malloc(sizeof(ReaderNode));  /*读者登陆临时节点*/
    printf(">>>提示：输入借书证号为0时，默认退出登陆\n");
    printf("输入你的借书证号：");
    scanf("%d",&(rr->data.ReaderNum));
    while (rr->data.ReaderNum!= 0)   /*读者信息登陆,证件号为0时，退出登陆*/
    {
        printf("输入证件密码：");
        scanf("%s",rr->data.password);
        while (rt->next!= NULL)     /*检索读者库中是否存在该证件*/
        {
            if(rr->data.ReaderNum == rt->next->data.ReaderNum)  /*证件存在*/
            {
                if(strcmp(rr->data.password,rt->next->data.password) == 0)  /*密码是否正确*/
                {
                    printf("读者验证成功.\n");
                    rt =rt->next;
                    return rt;      /*密码正确*/
                }
                printf(">>>密码错误<<<\n");
                break;              /*密码错误*/
            }
            rt =rt->next;
        }
        if (rt->next ==NULL)
        {
            printf(">>>证件号错误<<<\n");
        }
        if (--time ==0)            /*3次错误锁定借书证*/
            break;
        printf("第%d次输入你的借书证号，3次错误证件将被锁定：",i++);
        scanf("%d",&(rr->data.ReaderNum));
        rt = preader;
    }
    if (time == 0)
    {
        printf(">>>>>>>错误3次,用户被锁定<<<<<<<\n");
        printf("\n>>>证件已被锁定<<<--按任意键回主菜单>>>>>>>>>>");
        getch();
        system("cls");
        return NULL;
    }
    if(rr->data.ReaderNum == 0)
    {
        printf(">>成功退出登陆<<\n");
        printf("\n>>>登陆已安全退出<<<--按任意键回主菜单>>>>>>>>>>");
        getch();
        system("cls");
        return NULL;
    }
    return NULL;
}
/*获取系统本地时间*/
void LocalTime(char *strTime)
{
    time_t t;
    tm *tp;
    t=time(NULL);
    tp=localtime(&t);   /*转化为当地时间*/
    sprintf(strTime,"%d-%d-%d%d:%d:%d",\
        tp->tm_year+1900,tp->tm_mday,tp->tm_mon+1,tp->tm_hour,tp->tm_min,tp->tm_sec);/*格式化时间*/
    return;
}
/*光标定位*/
void GotoXY(int x, int y)
{
    COORD c;
    c.X = x-1;
    c.Y = y-1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
/*擦除菜单选项*/
void erase(int n)
{
    switch(n)
    {
    case 1:  GotoXY(22,5); printf("  "); break;
    case 2:  GotoXY(22,6); printf("  "); break;
    case 3:  GotoXY(22,7); printf("  "); break;
    case 4:  GotoXY(22,8); printf("  "); break;
    case 5:  GotoXY(22,9); printf("  "); break;
    case 6:  GotoXY(22,10); printf("  "); break;
    case 7:  GotoXY(22,11); printf("  "); break;
    case 8:  GotoXY(22,12); printf("  "); break;
    case 9:  GotoXY(22,13); printf("  "); break;
    case 10:  GotoXY(22,14); printf("  "); break;
    case 0:  GotoXY(22,15); printf("  "); break;
    }
    return;
}
/*菜单选项*/
void menu(int n)
{
    switch(n)
    {
    case 1:  GotoXY(22,5); printf(">>");break;
    case 2:  GotoXY(22,6); printf(">>");break;
    case 3:  GotoXY(22,7); printf(">>");break;
    case 4:  GotoXY(22,8); printf(">>");break;
    case 5:  GotoXY(22,9); printf(">>");break;
    case 6:  GotoXY(22,10); printf(">>");break;
    case 7:  GotoXY(22,11); printf(">>");break;
    case 8:  GotoXY(22,12); printf(">>");break;
    case 9:  GotoXY(22,13); printf(">>");break;
    case 10:  GotoXY(21,14); printf(">>");break;
    case 0:  GotoXY(22,15); printf(">>");break;
    }
    GotoXY(38,18);
    Sleep(200);
    system("cls");
    return;
}
/*主函数*/
int main()
{
    int nSelect = 1;
    int last = 1;
    char strTime[100];
    BookNode *pbook =(BookNode *)malloc(sizeof(BookNode));
    ReaderNode *preader =(ReaderNode *)malloc(sizeof(ReaderNode));
    pbook->data.allCount= 0;        /*总藏书初始化为0*/
    pbook->data.nowCount= 0;        /*现存书初始化为0*/
    preader->data.ReaderCount= 0;   /*读者数初始化为0*/   
    pbook =OpenBook();              /*打开图书文件 返回图书链表头指针*/
    preader =OpenReader();          /*打开读者文件 返回读者链表头指针*/
   
    while(1)
    {
    //  system("color c");
        printf(">>>>>>>>>>>>>>>>>>>>>>>>     目    录    <<<<<<<<<<<<<<<<<<<<<<<<<<\n\n");
       LocalTime(strTime);                            /*获取系统时间*/
        printf("                ※ 当前时间：%s ※\n\n",strTime);
        printf("                     >>  1.  浏览图书信息\n");
        printf("                         2.  浏览读者信息\n");
        printf("                         3.  图书登记入库\n");
        printf("                         4.  读者证件办理\n");
        printf("                         5.  读者图书借阅\n");
        printf("                         6.  读者图书归还\n");
        printf("                         7.  读者信息查询\n");
        printf("                         8.  读者信息修改\n");
        printf("                         9.  图书信息修改\n");
        printf("                        10.  图书资源搜索\n");
        printf("                         0.  退出管理系统.\n");
 
        printf("\n\n选择你要进行的操作>>>>>>>>>>>>>>>>>>>");
        last = nSelect;
        scanf("%d",&nSelect);
        fflush(stdin);
        erase(last);           /*擦掉上次选项*/
        menu(nSelect);         /*菜单选择*/
        switch(nSelect)
        {
        case 1:LookBook(pbook); break;                      /*浏览图书信息*/
        case 2:LookReader(preader); break;                  /*浏览读者信息*/
        case 3:InBook(pbook); break;pbook = OpenBook();     /*图书登记入库*/
        case 4:InReader(preader); break;                    /*读者证件办理*/
        case 5:BorrowBook(pbook,preader); break;            /*读者图书借阅*/
        case 6:ReturnBook(pbook,preader); break;preader = OpenReader(); /*读者图书归还*/
        case 7:FindReaderAll(preader); break;               /*读者信息查询*/
        case 8:ModifyReader(preader); break;                /*读者信息修改*/
        case 9:ModifyBook(pbook); break;                     /*图书信息修改*/
        case 10:FindBook(pbook); break;                     /*图书资源搜索*/
        case 0:                                              /*退出管理系统*/
            {
                printf("感谢您的使用^@^\n");
                SaveBook(pbook);             /*图书信息保存*/
                SaveReader(preader);         /*读者信息保存*/
                exit(0);
            }
            break;
        default:printf("error\n");break;
        }
    }
   
    return 0;
}
