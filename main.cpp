#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <graphics.h>
using namespace std;
int inputSatirSay=0;
int inputSatirSay2=0;
int **R;//Dinamik R Matrisi
double **Q;//Dinamik Q Matrisi
int **hamleler; //Gecisler bu matriste tutulmaktadir
int *outPathTut;//Qmatisinin cikislari bu dizide tutulmaktadir.
class Dosyalama
{
public:
    //dosyadaki eleman sayisinin belirleleyip  inputSatirSay degiskenine atamaktadir.
    void dosyaSatirSay(char name[20])
    {
        char adi[20];
        FILE *dosya;
        char karakter;
        if((dosya=fopen(name,"r"))== NULL)
        {
            puts("Dosyasi acilmadi. !\n");
            puts("Dosya ismini kontrol ediniz.!\n");
        }
        else
        {
            do
            {
                karakter=getc(dosya);
                if(karakter=='\n')
                {
                    inputSatirSay++;
                }
            }
            while(feof(dosya)==NULL);
        }
        fclose(dosya);
        printf("satir sayisi: %d",inputSatirSay);
    }
    //Komsuluklarin oldugu dosyayi okuyup olan komusuluklara gore R matrisini guncelliyor,0 atiyor
    void inputOku(char name2[20],int hedef)
    {
        char adi2[20];
        FILE *dosya2;
        char *pch;
        int i=0;
        int tut;
        if((dosya2=fopen(name2,"r"))== NULL)
        {
            puts("Dosyasi acilmadi. !\n");
            puts("Dosya ismini kontrol ediniz.!\n");
        }
        else
        {
            while(fscanf(dosya2,"%s",adi2)!=EOF)
            {
                tut=0;
                pch=strtok(adi2,",");
                if(pch!=NULL)
                {
                    while(pch!=NULL)
                    {
                        hamleler[i][tut]=atoi(pch);
                        if(atoi(pch)==hedef)
                        {
                            R[inputSatirSay2][atoi(pch)]=100;
                        }
                        else
                            R[inputSatirSay2][atoi(pch)]=0;
                        printf ("%s ",pch);
                        pch=strtok(NULL,",");
                        tut++;
                    }
                    hamleler[i][tut]=-1;

                    printf("i:%d :tut:%d",i,tut);
                    hamleler[hedef][tut]=hedef;
                    hamleler[hedef][tut+1]=-1;
                    i++;
                    printf("\n");
                }
                inputSatirSay2++;
            }

        }
        fclose(dosya2);

    }
    //R matrisinin son halini dosyaya kaydediyor.
    void rMatrisDosyaKaydet()
    {

        FILE * dosya;
        dosya=fopen("outR.txt","w");

        for(int i=0; i<inputSatirSay; i++)
        {
            for(int j=0; j<inputSatirSay; j++)
            {
                fprintf(dosya,"%4d",R[i][j]);
            }
            fprintf(dosya,"\n");
        }
        printf("\nR matrisi yazdirma islemi tamamlandi");
        fclose(dosya);
    }
    // Q matrisinin son halini dosyaya kaydediyor.
    void qMatrisDosyaKaydet()
    {

        FILE * dosya;
        dosya=fopen("outQ.txt","w");

        for(int i=0; i<inputSatirSay; i++)
        {
            for(int j=0; j<inputSatirSay; j++)
            {
                fprintf(dosya,"%.02lf   ",Q[i][j]);
            }
            fprintf(dosya,"\n");
        }
        printf("\nQ matrisi yazdirma islemi tamamlandi");
        fclose(dosya);
    }
    //Q matrisinden elde edilen cikislari dosyaya kaydediyor.
    void qMatrisCikisKaydet()
    {

        FILE * dosya;
        dosya=fopen("outPath.txt","w");
        for(int i=0; outPathTut[i]!=-1; i++)
        {
            fprintf(dosya,"%4d",outPathTut[i]);
        }
        printf("\nQ matrisi cikis yolu yazdirma islemi tamamlandi");
        fclose(dosya);
    }
};

class Output
{
public:
    int outPathMaxGetir(int durum)
    {
        double buyuk=0;
        int durumYer;
        for(int i=0; i<inputSatirSay; i++)
        {
            buyuk=max(Q[durum][i],buyuk);
        }
        for(int i=0; i<inputSatirSay; i++)
        {
            if(Q[durum][i]==buyuk)
            {
                durumYer=i;
                break;
            }
        }
        return durumYer;
    }

    void outPath(int durum,int hedef)
    {
        int i=0;
        outPathTut= (int *) malloc( sizeof(int)*inputSatirSay);
        outPathTut[i]=durum;
        while(1)
        {
            i++;
            durum=outPathMaxGetir(durum);
            outPathTut[i]=durum;
            if(durum==hedef)
                break;
        }
        outPathTut[i+1]=-1;
        for(int i=0; outPathTut[i]!=-1; i++)
        {
            printf("%d  ",outPathTut[i]);
        }
    }
    double qMaxGetir(int sonrakidurum)
    {
        double buyuk=0;
        int i;
        for( i=0; hamleler[sonrakidurum][i]!=-1; i++)
        {
            if(hamleler[sonrakidurum][i]==-1)
                break;
            buyuk=max(Q[sonrakidurum][hamleler[sonrakidurum][i]],buyuk);
        }

        printf("  QMax:%lf\n",buyuk);
        return buyuk;
    }
    int rastgele (int x,int hedef)
    {
        int i=0;
        do
        {
            i++;
        }
        while(hamleler[x][i]!=-1);
        printf("i:%d\n",i);
        return hamleler[x][rand()%i];
    }

    void QMatrisiBulma(int baslangic,int hedef,int iterasyon)
    {
        int hamle;
        int tursayisi;
        double QMax;
        int sonrakidurum;
        int durum;

        for(tursayisi=0; tursayisi<iterasyon; tursayisi++)
        {
            int sayac=0;
            durum=baslangic;
            while(1)
            {
                hamle=rastgele(durum,hedef);
                printf("Durum:%d",durum);
                sonrakidurum=hamle;
                printf(" SonrakiDurumu:%d",sonrakidurum);
                QMax=qMaxGetir(sonrakidurum); //Maksimumu bulan fonksiyonuz yaz
                Q[durum][hamle] =(double)R[durum][hamle] + 0.8*QMax;
                if(durum==hedef)
                    break;
                durum = sonrakidurum;
            }
            printf("\n---------------------------%d\n",tursayisi);
        }
    }
};
class Ekran
{
public:
    // Gorsel Arayuzden bilgi girisini saglamaktadir.
    void yazDosyaAdi(char name[25])
    {
        settextstyle(8,0,1);
        setcolor(0);
        int i=0;
        char ch;
        outtextxy(30,30,"R matrisi icin verilerin oldugu dosya ismini giriniz:");
        bar(620,25,880,50);
        name[0]='\0';
        do
        {
            ch=getch();
            setbkcolor(15);
            if(ch==8)
            {
                if(i)
                {
                    name[i-1]='\0';
                    i--;
                    bar(620,25,880,50);
                }
            }
            else if(ch!=13)
            {
                if(i< 25 && ch>= ' ' && ch<= '~')
                {
                    name[i]=ch;
                    name[i+1]='\0';
                    i++;
                }
            }
            outtextxy(625,27,name);
        }
        while(ch!=13);
    }
    int yazBaslangicHedefIterasyon(int belirle)
    {
        setlinestyle(DASHED_LINE,0,THICK_WIDTH);
        setbkcolor(7);
        setcolor(0);
        int i=0;
        char ch;
        char deger[7];
        if(belirle==0)
        {
            outtextxy(30,60,"Baslangic odacik numarasini giriniz:");
            bar(425,55,510,80);
        }
        if(belirle==1)
        {
            outtextxy(30,90,"Hedef odacik numarasini giriniz:");
            bar(425,85,510,110);
        }
        if(belirle==2)
        {
            outtextxy(30,120,"Iterasyon sayisini giriniz:");
            bar(425,115,510,140);
        }
        deger[0]='\0';
        do
        {
            setbkcolor(15);
            do
            {
                ch=getch();
                while(ch==13 && deger[0]==NULL)
                {
                    MessageBoxA(NULL,"Sayi girdiginizden emin misiniz?","",MB_OK);
                    ch=getch();
                }
                if(ch==8 || ch==13)
                    break;
            }
            while(ch>=58 || ch<=47);
            if(ch==8)
            {
                if(i)
                {
                    deger[i-1]='\0';
                    i--;
                    if(belirle==0)
                    {
                        bar(425,55,510,80);
                    }
                    if(belirle==1)
                    {
                        bar(425,85,510,110);
                    }
                    if(belirle==2)
                    {
                        bar(425,115,510,140);
                    }
                }
            }
            else if(ch!=13)
            {
                if(i<7 && ch>= ' ' && ch<= '~')
                {
                    deger[i]=ch;
                    deger[i+1]='\0';
                    i++;
                }
            }
            if(belirle==0)
            {
                outtextxy(430,58,deger);
            }
            if(belirle==1)
            {
                outtextxy(430,88,deger);
            }
            if(belirle==2)
            {
                outtextxy(430,118,deger);
            }
        }
        while(ch!=13);
        return atoi(deger);
    }
    void  qMatrisCizdir(int baslangic,int hedef)
    {
        int labirentUzunluk=500;
        int genislik=labirentUzunluk/sqrt(inputSatirSay);
        settextstyle(8,0,4);
        setfillstyle(SOLID_FILL,7);           //dolgu deseni,renk doldurur
        setcolor(0);
        int satir;
        int sutun=0;
        int i;
        int j=0;
        int boyut=sqrt(inputSatirSay);
        do
        {
            satir=60;
            i=0;
            do
            {
                bar(satir,150+sutun,genislik+satir,150+genislik+sutun);
                rectangle(satir,150+sutun,genislik+satir,150+genislik+sutun);
                satir+=genislik;
                i++;
            }
            while(i<boyut);
            sutun+=genislik;
            j++;
        }
        while(j<boyut);

        int satir1,sutun1;//Sirayla dugumlerin oldugu satir ve sutunu tutarlar.
        int satir2,sutun2;
        int dugumSatir=sqrt(inputSatirSay);
        for(int i=0; i<inputSatirSay; i++)
        {

            for(int j=0; hamleler[i][j]!=-1; j++)
            {
                if(hamleler[i][j]>=sqrt(inputSatirSay))
                {
                    satir1=hamleler[i][j]/sqrt(inputSatirSay);
                    sutun1=(hamleler[i][j])%dugumSatir;

                    printf("\ni:%d hamleler[%d][%d]:%d b:%d",i,i,j,hamleler[i][j],sutun1);

                }
                if(hamleler[i][j]<sqrt(inputSatirSay))
                {
                    satir1=0;
                    sutun1=hamleler[i][j];
                }

                if(i>=sqrt(inputSatirSay))
                {
                    satir2=i/sqrt(inputSatirSay);
                    sutun2=i%dugumSatir;

                }
                if(i<sqrt(inputSatirSay))
                {
                    satir2=0;
                    sutun2=i;
                }
                if(satir1>satir2&&sutun2==sutun1) // yukari asagi1 i>hamle[i][j]
                {
                    setcolor(7);
                    line(60+genislik*sutun1,150+genislik*satir1,60+genislik+genislik*sutun1,150+genislik*satir1);
                }

                else if(sutun2>sutun1&&satir1==satir2)
                {
                    setcolor(7);
                    line(60+genislik+genislik*sutun1,150+genislik*satir1,60+genislik+genislik*sutun1,150+genislik+genislik*satir1);
                }
            }
            if(i==baslangic && i<sqrt(inputSatirSay))
            {
                setcolor(7);
                line(60+genislik*sutun2,150+genislik*satir2,60+genislik+genislik*sutun2,150+genislik*satir2);
            }
            else   if(i==baslangic && i>=(inputSatirSay-sqrt(inputSatirSay)))
            {
                setcolor(7);
                line(60+genislik*sutun2,150+genislik+genislik*satir2,60+genislik+genislik*sutun2,150+genislik+genislik*satir2);
            }
            if(i==hedef&& i>=(inputSatirSay-sqrt(inputSatirSay)))
            {
                setcolor(7);
                line(60+genislik*sutun2,150+genislik+genislik*satir2,60+genislik+genislik*sutun2,150+genislik+genislik*satir2);

            }
            else  if(i==hedef && i<sqrt(inputSatirSay))
            {
                setcolor(7);
                line(60+genislik*sutun2,150+genislik*satir2,60+genislik+genislik*sutun2,150+genislik*satir2);
                line(60+genislik*sutun2,150+genislik+genislik*satir2,60+genislik+genislik*sutun2,150+genislik+genislik*satir2);
            }
        }
    }

    void qMatrisiCikisCizdir(int baslangic,int hedef)
    {
        int labirentUzunluk=500;
        int genislik=labirentUzunluk/sqrt(inputSatirSay);
        int satir,sutun,satir2,sutun2;
        int dugumSatir=sqrt(inputSatirSay);
        for(int i=0; outPathTut[i]!=-1; i++)
        {


            if(outPathTut[i]>=sqrt(inputSatirSay))
            {
                satir=outPathTut[i]/sqrt(inputSatirSay);
                sutun=outPathTut[i]%dugumSatir;

                printf("\ni:%d QCikis[%d]:%d sutun:%d",i,i,outPathTut[i],sutun);

            }
            else if(outPathTut[i]<sqrt(inputSatirSay))
            {
                satir=0;
                sutun=outPathTut[i];
            }

            if(outPathTut[i+1]>=sqrt(inputSatirSay))
            {
                satir2=outPathTut[i+1]/sqrt(inputSatirSay);
                sutun2=outPathTut[i+1]%dugumSatir;

            }
            else   if(outPathTut[i+1]<sqrt(inputSatirSay))
            {
                satir2=0;
                sutun2=outPathTut[i+1];
            }
            if(outPathTut[i]==hedef)
            {
                //line(105+90*sutun,230+60*satir,105+90*sutun,260+60*satir);
                setcolor(4);
                circle(60+genislik/2+genislik*sutun,150+genislik/2+genislik*satir,6);
                circle(60+genislik/2+genislik*sutun,150+genislik/2+genislik*satir,7);
                circle(60+genislik/2+genislik*sutun,150+genislik/2+genislik*satir,8);
            }
            if(outPathTut[i+1]==-1)
                break;
            if(satir2>satir&& sutun==sutun2)
            {

                setcolor(4);
                line(60+genislik/2+genislik*sutun,150+genislik/2+genislik*satir,60+genislik/2+genislik*sutun,150+genislik/2+genislik+genislik*satir);
            }
            else if(satir>satir2 && sutun==sutun2)
            {

                setcolor(4);
                line(60+genislik/2+genislik*sutun2,150+genislik/2+genislik*satir2,60+genislik/2+genislik*sutun2,150+genislik/2+genislik+genislik*satir2);
            }
            else if(sutun>sutun2 && satir==satir2)
            {
                setcolor(4);
                line(60+genislik/2+genislik*sutun2,150+genislik/2+genislik*satir2,60+genislik/2+genislik+genislik*sutun2,150+genislik/2+genislik*satir2);
            }
            else if(sutun2>sutun && satir==satir2)
            {
                setcolor(4);
                line(60+genislik/2+genislik*sutun,150+genislik/2+genislik*satir,60+genislik/2+genislik+genislik*sutun,150+genislik/2+genislik*satir);
            }
            delay(50);
        }
        settextstyle(8,0,1);
        setbkcolor(7);
        outtextxy(580,300,"Yolun yuvarlak ucu");
        outtextxy(580,330,"cikisi gostermektedir");
    }
};

int main()
{
    initwindow(900,700);
    setbkcolor(7);
    cleardevice();
    char name[25];
    int baslangic,hedef,iterasyon;
    int belirle;

    //  printf("R matrisi icin verilerin oldugu dosya ismini giriniz:");
    //  scanf("%s",name);
    class Dosyalama input;
    class Output QMatrisiOlustur;
    class Ekran  ekranim;

    ekranim.yazDosyaAdi(name);
    belirle=0;
    baslangic=ekranim.yazBaslangicHedefIterasyon(belirle);
    belirle=1;
    hedef=ekranim.yazBaslangicHedefIterasyon(belirle);
    belirle=2;
    iterasyon=ekranim.yazBaslangicHedefIterasyon(belirle);
    input.dosyaSatirSay(name);

    //R matrisi dinamik tanimlandi
    R= (int**)malloc(inputSatirSay*sizeof(int));
    for(int i = 0; i <inputSatirSay; i++)
        R[i] = (int*) malloc(inputSatirSay*sizeof(int));

    //Q matrisi dinamik tanimala
    Q= (double**)malloc(inputSatirSay*sizeof(double));
    for(int i = 0; i <inputSatirSay; i++)
        Q[i] = (double*) malloc(inputSatirSay*sizeof(double));

    //hamleler matrisi dinamik tanimala
    hamleler= (int**)malloc(inputSatirSay*sizeof(int));
    for(int i = 0; i <inputSatirSay; i++)
        hamleler[i] = (int*) malloc(inputSatirSay*sizeof(int));

    printf("\n");
    for(int i=0; i<inputSatirSay; i++)
    {
        for(int j=0; j<inputSatirSay; j++)
        {
            if(i==hedef && j==hedef)
            {
                R[i][j]=100;
            }
            else
            {
                R[i][j]=-1;
            }

        }
    }
    for(int i=0; i<inputSatirSay; i++)
    {
        for(int j=0; j<inputSatirSay; j++)
        {
            Q[i][j]=0;
        }
    }
    //R matrisinin son halini verir
    input.inputOku(name,hedef);
    ekranim.qMatrisCizdir(baslangic,hedef);
    printf("\n\n");
//R matrisinin input verilerinden sonraki hali
    printf("-----R Matrisi---------\n");
    for(int i=0; i<inputSatirSay; i++)
    {
        for(int j=0; j<inputSatirSay; j++)
        {
            //       printf("%4d",R[i][j]);
        }
        printf("\n");
    }

    //Qmatrisini olusturur
    QMatrisiOlustur.QMatrisiBulma(baslangic,hedef,iterasyon);

    printf("\n-------Q Matrisi--------\n");
    for(int i=0; i<inputSatirSay; i++)
    {
        for(int j=0; j<inputSatirSay; j++)
        {
            //      printf("%.02lf ",Q[i][j]);
        }
        printf("\n");
    }
    printf("\n\n--OutPath---\n");
    QMatrisiOlustur.outPath(baslangic,hedef);
    ekranim.qMatrisiCikisCizdir(baslangic,hedef);
    printf("\n");

    input.rMatrisDosyaKaydet();
    input.qMatrisDosyaKaydet();
    input.qMatrisCikisKaydet();
//R matrisinin input verilerinden sonraki hali
    getch();
    return 0;
}

