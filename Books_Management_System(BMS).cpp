// ͼ�����ϵͳ
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

/*����ͼ���йص����ݽṹ*/
class book
{
	public:
		
    int BookNum;         /*ͼ����*/
    char BookName[20];   /*ͼ������*/
    char BookAuthor[15];/*ͼ������*/
    int nowCount;        /*ͼ���ִ���*/
    int allCount;        /*ͼ������*/
};
 
/*����������йص����ݽṹ*/
class reader
{
	public:
    int ReaderNum;         /*���߱��*/
    char ReaderName[20];   /*���ߵ�����*/
    char ReaderSex[8];     /*���ߵ��Ա�*/
    int ReaderAge;         /*���ߵ�����*/
    char ReaderTel[15];    /*���ߵĵ绰*/
    int ReaderCount;       /*����Ŀǰ�Ѿ����ĵ���Ŀ*/
    char password[16];     /*����֤������*/
    char date[10][100];         /*���߽�������*/
    int ReaderBorrow[10];  /*��ǰ�Ѿ����ĵ�ͼ����*/
    char BorrowName[10][40];/*�Ѿ����ĵ�ͼ������*/
};
 
/*������ÿ��ͼ���¼�йص����ݽṹ*/
typedef struct booknode
{
    struct book data;        /*������*/
    struct booknode*next;   /*ָ����*/
}BookNode;
 
/*������ÿ�����߼�¼�йص����ݽṹ*/
typedef struct readernode
{
    struct reader data;       /*������*/
    struct readernode*next;  /*ָ����*/
}ReaderNode;

/*����ͼ����ж��ߺͲ��������Ŀ*/
struct rbnum
{
    int rnum;              /*�����ܸ���*/
    int bnum;              /*�鼮�ܱ���*/
}allRBnum;
 
 
/*******************************************/
/*��������*/
BookNode *OpenBook();                                     /*��ͼ����ļ�*/
ReaderNode *OpenReader();                                 /*�򿪶��߿��ļ�*/
void InBook(BookNode *pbook);                             /*ͼ��¼��*/
void SaveBook(BookNode *pbook);                           /*ͼ����Ϣ����*/
void SaveReader(ReaderNode *preader);                     /*������Ϣ����*/
void BorrowBook(BookNode *pbook, ReaderNode *preader);    /*ͼ�����*/
void ReturnBook(BookNode *pbook, ReaderNode *preader);    /*ͼ��黹*/
void LookBook(BookNode *pbook);                           /*���ͼ���*/
void LookReader(ReaderNode *preader);                     /*������߿�*/
void InReader(ReaderNode *preader);                       /*����֤����*/
void FindReaderAll(ReaderNode *preader);                  /*������Ϣ��ѯ*/
ReaderNode *LoginReader(ReaderNode *preader);             /*���ߵ�½��֤*/
int LoginAdmin();                                         /*����Ա��½��֤*/
void ModifyReader(ReaderNode *preader);                   /*�޸Ķ�����Ϣ*/
void ModifyBook(BookNode *pbook);                         /*�޸�ͼ����Ϣ*/
void GotoXY(int x, int y);                                /*��궨λ*/
void erase(int n);                                        /*�����˵�ѡ��*/
void menu(int n);                                         /*�˵�ѡ��*/
void FindBook(BookNode *pbook);                           /*ͼ������*/
void LocalTime(char *strTime);                            /*��ȡϵͳ����ʱ��*/
/*******************************************/
/*��ͼ���ļ�book���������������*/
BookNode *OpenBook()
{
    FILE *fpbook;
    BookNode *pb = NULL,*rb = NULL;
    allRBnum.bnum =0;         /*��ʼ������0��*/
    BookNode *pbook =(BookNode *)malloc(sizeof(BookNode));  /*ͷָ��*/
    if(!pbook)
    {
        printf("�ռ�����ʧ��\n");
        exit(0);
    }
    pbook->next = NULL;
    rb = pbook;
    fpbook =fopen(".\\book.txt","a+");  /*׷�ӵķ�ʽ��һ���������ļ�*/
    if (!fpbook)
    {
        printf("�ļ���ʧ��\n");
        exit(0);
    }
    while (!feof(fpbook))
    {
        pb = (BookNode*)malloc(sizeof(BookNode));   /*����ռ�*/
        if (!pb)
        {
            printf("�ռ�����ʧ��\n");
            exit(0);
        }
        if (fread(pb,sizeof(BookNode),1,fpbook)== 1)
        {
            pb->next =NULL;
            rb->next =pb;
            rb = pb;             /*rbָ�������һλ*/
            allRBnum.bnum+= pb->data.allCount;    /*��ͼ����Ŀͳ��*/
        }
    }
    fclose(fpbook);          /*�ر��ļ�*/
//  printf("���ļ��ɹ�!ͼ����ܹ����飺%d��\n",allRBnum.bnum);
    return pbook;
}
 
/*�򿪶����ļ�reader���������������*/
ReaderNode *OpenReader()
{
    FILE *fpreader;
    ReaderNode *pr = NULL,*rr = NULL;
    allRBnum.rnum =0;         /*��ʼ����������0λ*/
    ReaderNode *preader =(ReaderNode *)malloc(sizeof(ReaderNode)); /*ͷָ��*/
    if (!preader)
    {
        printf("�ռ�����ʧ��\n");
        exit(0);
    }
    preader->next =NULL;
    rr = preader;
    fpreader =fopen(".\\reader.txt","a+");   /*׷�ӵķ�ʽ��һ���������ļ�*/
    if (!fpreader)
    {
        printf("�ļ���ʧ��\n");
        exit(0);
    }
    while(!feof(fpreader))
    {
        pr = (ReaderNode*)malloc(sizeof(ReaderNode));   /*����ռ�*/
        if (!pr)
        {
            printf("�ռ�����ʧ��\n");
            exit(0);
        }
        if(fread(pr,sizeof(ReaderNode),1,fpreader) == 1)
        {
            pr->next =NULL;
            rr->next =pr;
            rr = pr;             /*rrָ�������һλ*/
            ++allRBnum.rnum;      /*������Ŀ+1*/
        }
    }
    fclose(fpreader);          /*�ر��ļ�*/
//  printf("���ļ��ɹ�!ӵ��������������%dλ\n",allRBnum.rnum);
    return preader;
}
 
/*ͼ��¼��*/
void InBook(BookNode *pbook)
{
    printf(">>>>>>>>>>>>>>>>>>>>>>>>   ͼ��Ǽ����   <<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    BookNode *pb = pbook;
    BookNode *pt =pbook;      /*pt��ʱָ�����ڱ�������ͼ���*/
    BookNode *pr =(BookNode *)malloc(sizeof(BookNode));
    while (pb->next !=NULL)   /*�ҵ������β�� ��ʼ�����Ŀ*/
    {
        pb = pb->next;
    }
   
    int i = 1;        /*������ʱ����*/
    printf("��ʾ�������Ϊ0ʱ������ͼ��¼��!\n");
    printf("________________________________________________________________\n");
    printf("�������ڽ��е� %d ����Ǽ�.\n",i++);
    printf("��ţ�");
    scanf("%d",&(pr->data.BookNum));   /*���*/
    while(pr->data.BookNum != 0)
    {
        printf("������");
        scanf("%s",pr->data.BookName);   /*����*/
        printf("��������");
        scanf("%s",pr->data.BookAuthor);   /*������*/
        pr->data.nowCount= 1;            /*�ִ�����ʼ��Ϊ1*/
        pr->data.allCount= 1;            /*�ܴ洢����ʼ��Ϊ1*/
        while (pt->next!= NULL)
        {
            if(pr->data.BookNum != pt->data.BookNum)
                pt =pt->next;
            else                             /*˵����ͼ���Ѿ�����*/
            {
                pt->data.nowCount+= 1;      /*�ִ���Ŀ+1*/
                pt->data.allCount+= 1;      /*����Ŀ+1*/
                break;
            }
        }
        if (pt->next ==NULL)          /*ͼ�黹δ��� �����*/
        {
            if(pr->data.BookNum != pt->data.BookNum)
            {
                pr->next= NULL;
                pb->next= pr;             /*���½��Ľ���������β��*/
                pb = pr;
            }
            else
            {
                pt->data.nowCount+= 1;      /*�ִ���Ŀ+1*/
                pt->data.allCount+= 1;      /*����Ŀ+1*/
            }
        }
        printf("�����ɹ�.\n");
        pr = (BookNode*)malloc(sizeof(BookNode));   /*������һ���ռ�*/
        printf("�������ڽ��е� %d ����Ǽ�.\n",i++);
        printf("��ţ�");
        scanf("%d",&(pr->data.BookNum));   /*���*/
        pt = pbook;                   /*pt��ʱָ���ƶ���ͷָ��λ��*/
    }
    printf("________________________________________________________________\n");
    SaveBook(pbook);             /*ͼ����Ϣ����*/
    printf("\n>>>ͼ��������<<<--������������˵�>>>>>>>>>>");
    getch();
    system("cls");
    return;
}
/*ͼ�鱣��*/
void SaveBook(BookNode *pbook)
{
    BookNode *pb =pbook->next;
    FILE *fpbook;
    fpbook =fopen(".\\book.txt","w");                  /*ֻд�ķ�ʽ���ļ�*/
    if (!fpbook)
    {
        printf("�ļ���ʧ��\n");
        exit(0);
    }
    while (pb != NULL)
    {
        if(fwrite(pb,sizeof(BookNode),1,fpbook) == 1)  /*ÿ��дһ����¼*/
        {
            pb =pb->next;
        }
    }
    fclose(fpbook);
    return;
}
/*������Ϣ����*/
void SaveReader(ReaderNode *preader)
{
    ReaderNode *pr =preader->next;
    FILE *fpreader;
    fpreader =fopen(".\\reader.txt","w");                  /*ֻд�ķ�ʽ���ļ�*/
    if (!fpreader)
    {
        printf("�ļ���ʧ��\n");
        exit(0);
    }
    while (pr != NULL)
    {
        if(fwrite(pr,sizeof(ReaderNode),1,fpreader) == 1)   /*ÿ��дһ����¼*/
        {
            pr =pr->next;
        }
    }
    fclose(fpreader);
    return;
}
 
/*ͼ�����*/ 
void BorrowBook(BookNode *pbook, ReaderNode *preader)
{
    printf(">>>>>>>>>>>>>>>>>>>>>>>>   ����ͼ�����   <<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    int booknum = 0, flag= 0,time = 3, i = 2;
    BookNode *pt =pbook;           /*ͼ����ʱָ��*/
    ReaderNode *rt =NULL;       /*������ʱָ��*/
    ReaderNode *rr =(ReaderNode *)malloc(sizeof(ReaderNode));  /*���ߵ�½��ʱ�ڵ�*/
    /******************************************/
    if((rt=LoginReader(preader)) == NULL)   /*���ߵ�½��֤*/
    {
        printf("������֤ʧ��.\n");
        return;
    }
    /******************************************/
    printf("________________________________________________________________\n");
    printf("����Ҫ�����鼮������ţ�");
    scanf("%d",&booknum);
    while(pt->next!=NULL)
    {
        if(pt->data.BookNum == booknum)
        {
            if(pt->data.nowCount >= 1)
            {
                printf("�����ִ�����%d �������Խ���.\n",pt->data.nowCount);
                LocalTime(rt->data.date[rt->data.ReaderCount]);   /*�Ǽǵ�ǰ����ͼ�������*/
                rt->data.ReaderBorrow[rt->data.ReaderCount]= pt->data.BookNum;  /*��¼��ǰͼ����*/ 
                strcpy(rt->data.BorrowName[rt->data.ReaderCount],pt->data.BookName);/*��¼��ǰͼ������*/
                --pt->data.nowCount;  /*ͼ�����ڿ��-1*/
                ++rt->data.ReaderCount;/*���߽��ı���+1*/ 
                printf(">>>����ɹ�<<<\n");
                printf("���ѣ�����30��֮�ڹ黹����!!!���ڰ�ÿ��0.1Ԫ�շ�!!!\n");
                SaveBook(pbook);             /*ͼ����Ϣ����*/
                SaveReader(preader);         /*������Ϣ����*/
                printf("________________________________________________________________\n");
                printf("\n>>>ͼ��������<<<--������������˵�>>>>>>>>>>");
                getch();
                system("cls");
                return;
            }
            printf(">>>���ź�!�ñ�ͼ����ĿǰΪ�գ����ɽ���<<<\n");
            printf("________________________________________________________________\n");
            printf("\n>>>ͼ�����ʧ��<<<--������������˵�>>>>>>>>>>");
            getch();
            system("cls");
            return;
        }
        pt =pt->next;            /*����ͼ���*/
    }
    if (pt->next ==NULL)
    {
        printf(">>>���ź�!û���ҵ���ͼ��<<<\n");
    }
    printf("________________________________________________________________\n");
    printf("\n>>>ͼ�����ʧ��<<<--������������˵�>>>>>>>>>>");
    getch();
    system("cls");
    return;
} 
/*ͼ��黹*/
void ReturnBook(BookNode *pbook, ReaderNode *preader)
{
    printf(">>>>>>>>>>>>>>>>>>>>>>>>   ����ͼ��黹   <<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    int booknum = 0, i =0;
    BookNode *pt = pbook;
    ReaderNode *pr =preader;
    printf("________________________________________________________________\n");
    printf("�黹����ţ�");
    scanf("%d",&booknum);
    while (pt->next !=NULL)      /*����ͼ���*/
    {
        if(pt->next->data.BookNum == booknum)
        {
            while(pr->next != NULL)                /*���߽�����Ϣ�޸�*/
            {
                for (i =0; i < pr->data.ReaderCount; ++i)   /*����ÿ����������������鼮*/
                    if(pr->data.ReaderBorrow[i] == pt->next->data.BookNum)
                    {
                        ++pt->next->data.nowCount;              /*�黹ͼ�����ڿ��+1*/
                        pr->data.ReaderBorrow[i]= 0;         /*�����ע��*/
                        strcpy(pr->data.date[i],"");          /*����ʱ��ע��*/
                        strcpy(pr->data.BorrowName[i],"");    /*��������ע��*/
                        --pr->data.ReaderCount;               /*������-1*/
                        printf("�黹�ɹ�\n");
                        printf("________________________________________________________________\n");
                        printf("\n>>>ͼ��黹���<<<--������������˵�>>>>>>>>>>");
                        SaveBook(pbook);             /*ͼ����Ϣ����*/
                        SaveReader(preader);         /*������Ϣ����*/
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
        printf("�Ǳ�ͼ���ͼ��.\n");
        printf("________________________________________________________________\n");
        printf("\n>>>ͼ��黹ʧ��<<<--������������˵�>>>>>>>>>>");
        getch();
        system("cls");
        return;
    }
    printf(">>>��ȷ��ͼ�������������ȷ<<<\n");
    printf("���ݸ���û�б����.\n");
    printf("\n>>>ͼ��黹ʧ��<<<--������������˵�>>>>>>>>>>");
    getch();
    system("cls");
    return;
}
/*���ͼ����Ϣ*/
void LookBook(BookNode *pbook)
{
    printf(">>>>>>>>>>>>>>>>>>>>>>>>   ���ͼ����Ϣ   <<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    BookNode *pt = pbook;
    printf("________________________________________________________________\n");
    printf("\n���\t\t����\t\t����\t\t�ִ���\t�����\n");
    while (pt->next !=NULL)
    {
        printf("%d\t\t",pt->next->data.BookNum);    /*���*/
        printf("%s\t\t",pt->next->data.BookName);   /*����*/
        printf("%s\t\t",pt->next->data.BookAuthor);/*����*/
        printf("%d\t",pt->next->data.nowCount);   /*�ִ���*/
        printf("%d\t\n",pt->next->data.allCount);   /*�����*/
        pt = pt->next;
    }
    printf("________________________________________________________________\n");
    printf("\n>>>������<<<--������������˵�>>>>>>>>>>");
    getch();
    system("cls");
    return;
}
/*���������Ϣ*/
void LookReader(ReaderNode *preader)
{
    printf(">>>>>>>>>>>>>>>>>>>>>>>>   ���������Ϣ   <<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    /*����ԱȨ����֤*/
    if (LoginAdmin() == 0)
    {
        printf("����Ա��֤ʧ��.\n");
        printf("\n>>>û�й���ԱȨ��<<<--������������˵�>>>>>>>>>>");
        getch();
        system("cls");
        return;
    }
    printf("����Ա��֤�ɹ�.\n");
    /******************************/
    ReaderNode *pt =preader;
    printf("________________________________________________________________\n");
    printf("\n����֤��\t����\t�Ա�\t����\t�绰\t�ѽ���ͼ�鱾��\n");
    while (pt->next !=NULL)
    {
        printf("%d\t",pt->next->data.ReaderNum);     /*֤����*/
        printf("%s\t",pt->next->data.ReaderName);      /*����*/
        printf("%s\t",pt->next->data.ReaderSex);       /*�Ա�*/
        printf("%d\t",pt->next->data.ReaderAge);       /*����*/
        printf("%s\t",pt->next->data.ReaderTel);       /*�绰*/
        printf("%d\t\n",pt->next->data.ReaderCount);   /*��ǰ����ͼ�鱾��*/
        pt = pt->next;
    }
    printf("________________________________________________________________\n");
    printf("\n>>>������<<<--������������˵�>>>>>>>>>>");
    getch();
    system("cls");
    return;
}
 
/*���߽���֤����*/
void InReader(ReaderNode *preader)
{
    printf(">>>>>>>>>>>>>>>>>>>>>>>>   ����֤������   <<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    ReaderNode *pr =preader;
    ReaderNode *pt =preader;      /*pt��ʱָ�����ڱ����������߿�*/
    ReaderNode *rr =(ReaderNode *)malloc(sizeof(ReaderNode));
    int i = 1;                     /*������ʱ����*/
    /*����ԱȨ����֤*/
    if (LoginAdmin() == 0)
    {
        printf("����Ա��֤ʧ��.\n");
        printf("\n>>>û�й���ԱȨ��<<<--������������˵�>>>>>>>>>>");
        getch();
        system("cls");
        return;
    }
    printf("����Ա��֤�ɹ�.\n");
    /***************************************************/
    while (pr->next !=NULL)       /*�ҵ������β��  ��ʼ��Ӷ���*/
    {
        pr = pr->next;
    }
 
    printf("��ʾ����֤����Ϊ0ʱ������֤������!\n");
    printf("________________________________________________________________\n");
    printf("�������ڽ��е� %d λ����֤������.\n",i++);
    printf("����֤�ţ�");
    scanf("%d",&(rr->data.ReaderNum));        /*����֤��*/
    while(rr->data.ReaderNum != 0)
    {
        printf("������");
        scanf("%s",rr->data.ReaderName);      /*����*/
        printf("�Ա�");
        scanf("%s",rr->data.ReaderSex);       /*�Ա�*/
        printf("���䣺");                    
        scanf("%d",&(rr->data.ReaderAge));    /*����*/
        printf("�绰��");
        scanf("%s",rr->data.ReaderTel);       /*�绰*/
        rr->data.ReaderCount= 0;             /*�Ѿ�����ͼ�鱾����ʼ��Ϊ0*/
        strcpy(rr->data.password,"123456");   /*֤������Ĭ��Ϊ��123456*/
        while (pt->next!= NULL)
        {
            if(rr->data.ReaderNum != pt->data.ReaderNum)
                pt =pt->next;
            else                              /*˵���ö����Ѿ�����*/
            {
                printf("��֤�����Ѿ�����,֤������ʧ��\n");
                printf("________________________________________________________________\n");
                printf("\n>>>����ʧ��<<<--������������˵�>>>>>>>>>>");
                getch();
                system("cls");
                return;
            }
        }
        if (pt->next ==NULL)          /*��֤���ſ��԰���*/
        {
            rr->next =NULL;
            pr->next =rr;             /*���½��Ľ���������β��*/
            pr = rr;
        }
        printf(">>>֤������ɹ�<<<\n");
        printf("���ѣ�����֤Ĭ������ȫ����\"123456\"\n");
        rr = (ReaderNode*)malloc(sizeof(ReaderNode));   /*������һ���ռ�*/
        printf("\n�������ڽ��е� %d λ����֤������.\n",i++);
        printf("����֤�ţ�");
        scanf("%d",&(rr->data.ReaderNum));   /*֤����*/
        pt = preader;                   /*pt��ʱָ���ƶ���ͷָ��λ��*/
    }
    printf("________________________________________________________________\n");
    SaveReader(preader);    /*֤����Ϣ�浵*/
    printf("\n>>>֤���������<<<--������������˵�>>>>>>>>>>");
    getch();
    system("cls");
    return;
}
/*������Ϣ��ѯ*/
void FindReaderAll(ReaderNode *preader)
{
    printf(">>>>>>>>>>>>>>>>>>>>>>>>   ������Ϣ��ѯ   <<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    int i = 0;
    ReaderNode *rt = NULL;
    /*****************************/
    if((rt=LoginReader(preader)) == NULL)
    {
        printf("������֤ʧ��.\n");
        return;
    }
    /*****************************/
    printf("________________________________________________________________\n");
    printf("����֤\t\t����\t�Ա�\t����\t�绰\t�ѽ���ͼ�鱾��\t����\n");
    printf("%d\t",rt->data.ReaderNum);     /*����֤*/
    printf("%s\t",rt->data.ReaderName);    /*����*/
    printf("%s\t",rt->data.ReaderSex);     /*�Ա�*/
    printf("%d\t",rt->data.ReaderAge);     /*����*/
    printf("%s\t",rt->data.ReaderTel);     /*�绰*/
    printf("%d\t",rt->data.ReaderCount);   /*�ѽ���ͼ�鱾��*/
    printf("%s\t",rt->data.password);      /*����*/
 
    printf("\n\n�ѽ�ͼ����Ŀ\tͼ������\t��������\n");
    for (i = 0; i <rt->data.ReaderCount; ++i)
    {
        printf("%d\t\t",rt->data.ReaderBorrow[i]);
        printf("%s\t\t",rt->data.BorrowName[i]);
        printf("%s\n",rt->data.date[i]);
    }
    printf("________________________________________________________________\n");
    printf("\n>>>��Ϣ��ѯ���<<<--������������˵�>>>>>>>>>>");
    getch();
    system("cls");
    return;
 
}
/*�޸�ͼ����Ϣ*/
void ModifyBook(BookNode *pbook)
{
    printf(">>>>>>>>>>>>>>>>>>>>>>>>   ͼ����Ϣ�޸�   <<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    /*����ԱȨ����֤*/
    if (LoginAdmin() == 0)
    {
        printf("����Ա��֤ʧ��.\n");
        printf("\n>>>û�й���ԱȨ��<<<--������������˵�>>>>>>>>>>");
        getch();
        system("cls");
        return;
    }
    printf("����Ա��֤�ɹ�.\n");
    /*************************/
    int nSelect = 0;
    int BookNum = 0;
    char BookName[20];
    char BookAuthor[15];
    BookNode *pt = pbook;
    BookNode *p2t = pbook;
    BookNode *pr =(BookNode *)malloc(sizeof(BookNode));  /*��ʱͼ��ָ��*/
    printf("________________________________________________________________\n");
    printf("����ͼ���ţ�");
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
            printf("δ�ҵ��ñ��ͼ��.\n");
            printf("\n>>>��Ϣ�޸�ʧ��<<<--������������˵�>>>>>>>>>>");
            getch();
            system("cls");
            return;
        }
    }
    printf(">>>>>ͼ����Ϣ����<<<<<\n");
    printf("________________________________________________________________\n");
    printf("\n���\t\t����\t\t����\t\t�ִ���\t�����\n");
    printf("%d\t\t",pt->data.BookNum);       /*���*/
    printf("%s\t\t",pt->data.BookName);      /*����*/
    printf("%s\t\t",pt->data.BookAuthor);  /*����*/
    printf("%d\t",pt->data.nowCount);      /*�ִ���*/
    printf("%d\t\n",pt->data.allCount);    /*�����*/
    printf("________________________________________________________________\n");
    printf(">>>>����ѡ����޸���Ϣ<<<<\n");
    printf("1.ͼ����\n");
    printf("2.ͼ������\n");
    printf("3.ͼ������\n");
    printf(">>>>ѡ��Ҫ�޸ĵ���Ϣ��");
    scanf("%d",&nSelect);
    switch(nSelect)
    {
    case 1:
        {
            printf("�������µ�ͼ���ţ�");
            scanf("%d",&BookNum);
            while(p2t->next != NULL)
            {
                if(p2t->next->data.BookNum == BookNum)
                    break;
                p2t =p2t->next;
            }
            if(p2t->next == NULL)
            {
                printf("�ñ��ͼ���Ѵ��ڣ��޸�ʧ��.\n");
                return;
            }
            pt->data.BookNum= BookNum;
            printf("�޸�ͼ���ųɹ�.\n");
        }
        break;
    case 2:
        {
            printf("�����µ�ͼ�����֣�");
            scanf("%s",BookName);
            strcpy(pt->data.BookName,BookName);
            printf("�޸�ͼ�����ֳɹ�.\n");
        }
        break;
    case 3:
        {
            printf("������ͼ�����ߣ�");
            scanf("%s",BookAuthor);
            strcpy(pt->data.BookAuthor,BookAuthor);
            printf("�޸�ͼ�����߳ɹ�.\n");
        }
        break;
    }
    printf("________________________________________________________________\n");
    SaveBook(pbook);             /*ͼ����Ϣ����*/
    printf("\n>>>��Ϣ�޸����<<<--������������˵�>>>>>>>>>>");
    getch();
    system("cls");
    return;
}
/*�޸Ķ�����Ϣ*/
void ModifyReader(ReaderNode *preader)
{
    printf(">>>>>>>>>>>>>>>>>>>>>>>>   ������Ϣ�޸�   <<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    int nSelect = 0;
    char ReaderTel[15];
    char password[16];
    ReaderNode *rt = NULL;
    if((rt=LoginReader(preader)) == NULL)
    {
        printf("������֤ʧ��.\n");
        return;
    }
    printf("________________________________________________________________\n");
    printf(">>>>���޸ĵ�����<<<<\n");
    printf("1.���ߵ绰\n");
    printf("2.��������\n\n");
    printf(">>>>ѡ��Ҫ�޸ĵ����ݣ�");
    scanf("%d",&nSelect);
    switch(nSelect)
    {
    case 1:
        {
            printf("�����µĵ绰���룺");
            scanf("%s",ReaderTel);
            strcpy(rt->data.ReaderTel,ReaderTel);
            printf("�޸Ķ��ߵ绰����ɹ�.\n");
        }
        break;
    case 2:
        {
            printf("�����µ����룺");
            scanf("%s",password);
            strcpy(rt->data.password,password);
            printf("�޸Ķ�������ɹ�.\n");
        }
        break;
    }
    printf("________________________________________________________________\n");
    SaveReader(preader);         /*������Ϣ����*/
    printf("\n>>>��Ϣ�޸����<<<--������������˵�>>>>>>>>>>");
    getch();
    system("cls");
    return;
}

/*ͼ������*/
void FindBook(BookNode *pbook)
{
    printf(">>>>>>>>>>>>>>>>>>>>>>>>   ͼ����Դ����   <<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    int nSelect = 0;
    int BookNum = 0;         /*ͼ����*/
    char BookName[20];   /*ͼ������*/
    char BookAuthor[15];/*ͼ������*/
    BookNode *pt = pbook;
    printf(">>>>��ѡ��������ʽ<<<<\n");
    printf("1.ͼ����\n");
    printf("2.������\n");
    printf("3.�����\n");
    printf(">>>>ѡ��������ʽ��");
    scanf("%d",&nSelect);
    switch(nSelect)
    {
    case 1:
        {
            printf("����ͼ������");
            scanf("%s",BookName);
            while(pt->next != NULL)
            {
                if(strcmp(pt->next->data.BookName,BookName)==0)
                {
                    printf(">>>���ڴ���<<<\n");
                    printf(">>>>>ͼ����Ϣ����<<<<<\n");
                    printf("________________________________________________________________\n");
                    printf("\n���\t\t����\t\t����\t\t�ִ���\t�����\n");
                    printf("%d\t\t",pt->next->data.BookNum);       /*���*/
                    printf("%s\t\t",pt->next->data.BookName);      /*����*/
                    printf("%s\t\t",pt->next->data.BookAuthor);    /*����*/
                    printf("%d\t",pt->next->data.nowCount);        /*�ִ���*/
                    printf("%d\t\n",pt->next->data.allCount);      /*�����*/
                    printf("________________________________________________________________\n");
                    break;
                }
                pt =pt->next;
            }
        }
        break;
    case 2:
        {
            printf("������������");
            scanf("%s",BookAuthor);
            while(pt->next != NULL)
            {
                if (strcmp(pt->next->data.BookAuthor,BookAuthor)==0)
                {
                    printf(">>>���ڴ���<<<\n");
                    printf(">>>>>ͼ����Ϣ����<<<<<\n");
                    printf("________________________________________________________________\n");
                    printf("\n���\t\t����\t\t����\t\t�ִ���\t�����\n");
                    printf("%d\t\t",pt->next->data.BookNum);       /*���*/
                    printf("%s\t\t",pt->next->data.BookName);      /*����*/
                    printf("%s\t\t",pt->next->data.BookAuthor);    /*����*/
                    printf("%d\t",pt->next->data.nowCount);        /*�ִ���*/
                    printf("%d\t\n",pt->next->data.allCount);      /*�����*/
                    printf("________________________________________________________________\n");
                    break;
                }
                pt =pt->next;
            }
        }
        break;
    case 3:
        {
            printf("��������ţ�");
            scanf("%d",&BookNum);
            while(pt->next != NULL)
            {
                if(pt->next->data.BookNum == BookNum)
                {
                    printf(">>>���ڴ���<<<\n");
                    printf(">>>>>ͼ����Ϣ����<<<<<\n");
                    printf("________________________________________________________________\n");
                    printf("\n���\t\t����\t\t����\t\t�ִ���\t�����\n");
                    printf("%d\t\t",pt->next->data.BookNum);       /*���*/
                    printf("%s\t\t",pt->next->data.BookName);      /*����*/
                    printf("%s\t\t",pt->next->data.BookAuthor);    /*����*/
                    printf("%d\t",pt->next->data.nowCount);        /*�ִ���*/
                    printf("%d\t\n",pt->next->data.allCount);      /*�����*/
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
        printf("<δ�ҵ���Ӧͼ��>\n");
        printf("\n>>>ͼ������ʧ��<<<--������������˵�>>>>>>>>>>");
    }
    else
        printf("\n>>>ͼ�������ɹ�<<<--������������˵�>>>>>>>>>>");
    getch();
    system("cls");
    return;
}
/*����Ա��½��֤*/
int LoginAdmin()
{
    char adminzh[16];
    char adminmm[16];
    printf("����Ա�˺ŵ�½��");
    scanf("%s",adminzh);      /*����Ա�˺�*/
    printf("����Ա���룺");          
    scanf("%s",adminmm);      /*����Ա����*/
    if(!(strcmp("admin",adminzh) == 0 &&strcmp("1234",adminmm) == 0))
        return 0;
    return 1;
}
/*���ߵ�½��֤*/
ReaderNode *LoginReader(ReaderNode *preader)
{
    int time = 3, i = 2;
    ReaderNode *rt =preader;       /*������ʱָ��*/
    ReaderNode *rr =(ReaderNode *)malloc(sizeof(ReaderNode));  /*���ߵ�½��ʱ�ڵ�*/
    printf(">>>��ʾ���������֤��Ϊ0ʱ��Ĭ���˳���½\n");
    printf("������Ľ���֤�ţ�");
    scanf("%d",&(rr->data.ReaderNum));
    while (rr->data.ReaderNum!= 0)   /*������Ϣ��½,֤����Ϊ0ʱ���˳���½*/
    {
        printf("����֤�����룺");
        scanf("%s",rr->data.password);
        while (rt->next!= NULL)     /*�������߿����Ƿ���ڸ�֤��*/
        {
            if(rr->data.ReaderNum == rt->next->data.ReaderNum)  /*֤������*/
            {
                if(strcmp(rr->data.password,rt->next->data.password) == 0)  /*�����Ƿ���ȷ*/
                {
                    printf("������֤�ɹ�.\n");
                    rt =rt->next;
                    return rt;      /*������ȷ*/
                }
                printf(">>>�������<<<\n");
                break;              /*�������*/
            }
            rt =rt->next;
        }
        if (rt->next ==NULL)
        {
            printf(">>>֤���Ŵ���<<<\n");
        }
        if (--time ==0)            /*3�δ�����������֤*/
            break;
        printf("��%d��������Ľ���֤�ţ�3�δ���֤������������",i++);
        scanf("%d",&(rr->data.ReaderNum));
        rt = preader;
    }
    if (time == 0)
    {
        printf(">>>>>>>����3��,�û�������<<<<<<<\n");
        printf("\n>>>֤���ѱ�����<<<--������������˵�>>>>>>>>>>");
        getch();
        system("cls");
        return NULL;
    }
    if(rr->data.ReaderNum == 0)
    {
        printf(">>�ɹ��˳���½<<\n");
        printf("\n>>>��½�Ѱ�ȫ�˳�<<<--������������˵�>>>>>>>>>>");
        getch();
        system("cls");
        return NULL;
    }
    return NULL;
}
/*��ȡϵͳ����ʱ��*/
void LocalTime(char *strTime)
{
    time_t t;
    tm *tp;
    t=time(NULL);
    tp=localtime(&t);   /*ת��Ϊ����ʱ��*/
    sprintf(strTime,"%d-%d-%d%d:%d:%d",\
        tp->tm_year+1900,tp->tm_mday,tp->tm_mon+1,tp->tm_hour,tp->tm_min,tp->tm_sec);/*��ʽ��ʱ��*/
    return;
}
/*��궨λ*/
void GotoXY(int x, int y)
{
    COORD c;
    c.X = x-1;
    c.Y = y-1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
/*�����˵�ѡ��*/
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
/*�˵�ѡ��*/
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
/*������*/
int main()
{
    int nSelect = 1;
    int last = 1;
    char strTime[100];
    BookNode *pbook =(BookNode *)malloc(sizeof(BookNode));
    ReaderNode *preader =(ReaderNode *)malloc(sizeof(ReaderNode));
    pbook->data.allCount= 0;        /*�ܲ����ʼ��Ϊ0*/
    pbook->data.nowCount= 0;        /*�ִ����ʼ��Ϊ0*/
    preader->data.ReaderCount= 0;   /*��������ʼ��Ϊ0*/   
    pbook =OpenBook();              /*��ͼ���ļ� ����ͼ������ͷָ��*/
    preader =OpenReader();          /*�򿪶����ļ� ���ض�������ͷָ��*/
   
    while(1)
    {
    //  system("color c");
        printf(">>>>>>>>>>>>>>>>>>>>>>>>     Ŀ    ¼    <<<<<<<<<<<<<<<<<<<<<<<<<<\n\n");
       LocalTime(strTime);                            /*��ȡϵͳʱ��*/
        printf("                �� ��ǰʱ�䣺%s ��\n\n",strTime);
        printf("                     >>  1.  ���ͼ����Ϣ\n");
        printf("                         2.  ���������Ϣ\n");
        printf("                         3.  ͼ��Ǽ����\n");
        printf("                         4.  ����֤������\n");
        printf("                         5.  ����ͼ�����\n");
        printf("                         6.  ����ͼ��黹\n");
        printf("                         7.  ������Ϣ��ѯ\n");
        printf("                         8.  ������Ϣ�޸�\n");
        printf("                         9.  ͼ����Ϣ�޸�\n");
        printf("                        10.  ͼ����Դ����\n");
        printf("                         0.  �˳�����ϵͳ.\n");
 
        printf("\n\nѡ����Ҫ���еĲ���>>>>>>>>>>>>>>>>>>>");
        last = nSelect;
        scanf("%d",&nSelect);
        fflush(stdin);
        erase(last);           /*�����ϴ�ѡ��*/
        menu(nSelect);         /*�˵�ѡ��*/
        switch(nSelect)
        {
        case 1:LookBook(pbook); break;                      /*���ͼ����Ϣ*/
        case 2:LookReader(preader); break;                  /*���������Ϣ*/
        case 3:InBook(pbook); break;pbook = OpenBook();     /*ͼ��Ǽ����*/
        case 4:InReader(preader); break;                    /*����֤������*/
        case 5:BorrowBook(pbook,preader); break;            /*����ͼ�����*/
        case 6:ReturnBook(pbook,preader); break;preader = OpenReader(); /*����ͼ��黹*/
        case 7:FindReaderAll(preader); break;               /*������Ϣ��ѯ*/
        case 8:ModifyReader(preader); break;                /*������Ϣ�޸�*/
        case 9:ModifyBook(pbook); break;                     /*ͼ����Ϣ�޸�*/
        case 10:FindBook(pbook); break;                     /*ͼ����Դ����*/
        case 0:                                              /*�˳�����ϵͳ*/
            {
                printf("��л����ʹ��^@^\n");
                SaveBook(pbook);             /*ͼ����Ϣ����*/
                SaveReader(preader);         /*������Ϣ����*/
                exit(0);
            }
            break;
        default:printf("error\n");break;
        }
    }
   
    return 0;
}
