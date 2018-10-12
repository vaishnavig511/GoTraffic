#include<bits/stdc++.h>
#include<conio.h>
#include<ctype.h>
#include<dos.h>
#include<fstream>
using namespace std;
//------------------------------------------Header Files
struct mnt
{
char name[20];
int mdtindex;
int SkipFlag;
}mnt[10];
//-------------------------
struct mdt
{
int srno;
char inst[20];
}mdt[10];
//-------------------------
struct ala
{
int index;
char arg[20];
}ala[10];
//-------------------------
int mntc,mdtc,alac;
//-----------------------------------------------------
char *replace_str(char *str, char *orig, char *rep)
{
    static char buffer[4096];
    char *p;

    if(!(p = strstr(str, orig)))  // Is 'orig' even in 'str'?
    {
        return str;
    }

    strncpy(buffer, str, p-str); // Copy characters from 'str' start to 'orig' st$
    buffer[p-str] = '\0';
    sprintf(buffer+(p-str),"%s%s",rep,p+strlen(orig));

    if(!(p = strstr(buffer, orig)))  // Is 'orig' even in 'buffer'?
        return buffer;
    else
        replace_str(buffer,orig,rep);
}
//--------------------------------------------------------------
void dispALA()
{
    int i;
    cout<<"\nIndex\tArgument";
    cout<<"\n------------------";
    for(i=0;i<alac;i++)
        cout<<"\n"<<ala[i].index<<"        "<<ala[i].arg;
}
//---------------------------------------------------------------
void dispMDT()
{
  int i;
  cout<<"\nIndex\tArgument";
  cout<<"\n------------------";

  for(i=0;i<mdtc;i++)
    cout<<"\n "<<mdt[i].srno<<"        "<<mdt[i].inst;
}
//---------------------------------------------------------------
void pass1()
{
    char ch,ch1;
    int flag=1;
    int i=0,j=0,k=0,l=0;
    char token[10],token1[10];
    char temp[5],temp1[5];
    char tline[80],line[80];

//FILE *src;
    ifstream src;
    ofstream dest;
//FILE *dest;
    //clrscr();
    strcpy(temp,"#");
    //temp="#";
    src.open("ip.txt");
//src = fopen("ip.c","r");
//dest = fopen("op.c","w");
    dest.open("op.txt",ios::app);
    do{
            do{                   //----> To Seperate Out Token
     // ch = fgetc(src);
                    src>>ch;
       //----> ch : Current Pointer
                    token[i] = ch;
                    i++;
             }while(ch!=EOF && !isspace(ch));
             token[i-1] = '\0';  //----> Mask Current ch
      //--------------------------------------------
      //--------------------------------------------
      //--------------------------------------------
            if(temp!="macro") //--> Check For "macro"
            {
                do{                  //--> Macro Name
                    //ch1 = fgetc(src);
                    src>>ch1;
                    token1[j] = ch1;
                    j++;
                  }while(ch1!=EOF && !isspace(ch1));
                token1[j-1] = '\0';
              //-----------------------------------
                strcpy(mnt[mntc].name,token1);
                mnt[mntc].mdtindex = mdtc;
                mntc++;
              //-------------------------------------
              //If no Parameter i.e ALA is Empty
              //-------------------------------------
                if(ch1=='\n')
                {
                    mnt[mntc-1].SkipFlag = 1;
                    goto Mid;
                }
              //-------------------------------------
              //Create ALA
              //-------------------------------------
                do{                          //ALA Content
                //ch1 = fgetc(src);
                        src>>ch1;
                        if(ch1=='&')
                            flag=1;
                        if(ch1==','||ch1=='\n')
                        {
                            token1[k]='\0';
                            flag=0;
                            ala[alac].index = alac;
                            strcpy(ala[alac].arg,token1);
                            //ala[alac].arg=token1;
                            alac++;
                            k=0;
                        }
                        if(flag==1)
                            token1[k++] = ch1;
                  }while(ch1!=EOF && ch1!='\n');
              //-----------------------------------------
         Mid:
              flag=1;
              j=0;
              do
              {
                   line[0]='\0';
                   do{                          //MDT
                      //ch1 = fgetc(src);
                      src>>ch1;
                      line[j++]=ch1;
                      }
                      while(ch1!=EOF && ch1!='\n');
                      line[j-1]='\0';

                    for(l = 0 ;l<alac;l++)
                    {

                                sprintf(temp1,"%d",l); //0
                                strcat(temp,temp1);    //#0
                                sprintf(tline,"%s",replace_str(line,ala[l].arg,temp));
                                 //  if(strcmp(tline,""))
                                strcpy(line,tline);
                                strcpy(temp,"#");
                    }
                    strcpy(mdt[mdtc].inst,line);
                    mdt[mdtc].srno = mdtc;
                    mdtc++;
                    j=0;
               }while(strcmp(line,"mend"));
            ch = ' ';
         }//end if
         else
            dest<<token;
            // fprintf(dest,"%s",token);
     //---------------------------------------------------------------
     if(ch=='\n')
        dest<<"\n";
        //fprintf(dest,"\n");
     if(ch==' ')
       dest<<" ";
       // fprintf(dest," ");
     //----------------------------------------------------------------
    if(isspace(ch))
        {
        token[0]='\0';
        i=0;
        }
     else
        {
        token[0]= ch;
        token[1]= '\0';
        i=1;
        }
	}while(ch!=EOF); //Outer od
    src.close();
    dest.close();
  //fclose(src);
  //fclose(dest);
}
//----------------------------------------------------------------
void disp()
{
    int i;
    char ch;
    ifstream src;
    src.open("op.c");
    //FILE *src;
    //src = fopen("op.c","r");
    do{
           src>>ch;
           cout<<ch;
            //ch = fgetc(src);
            //printf("%c",ch);
      }while(ch!=EOF);
}
//----------------------------------------------------------------
void pass2()
{
    char ch;
    int counter=0;
    int start_index;
    char ch1,ch2,pch; //comment validation
    int len,flag=0,sflag=0;
    int i = 0,j=0,k=0,l=0;
    char token[10];
    char token1[10];
    char temp[5],temp1[5];
    char tline[80];
    char line[80];
    char repl[10];

//FILE *src;
    ifstream src;

    alac = 0;
    strcpy(temp,"#");
    //temp="#";
    src.open("op.txt");
//src = fopen("op.c","r");

    do
    {
            do{                //For Seperate Token
                    src>>ch;
                    //ch = fgetc(src);
                    token[i] = ch;
                    i++;
              }while(ch!=EOF && !isspace(ch));
            token[i-1] = '\0';
     //--------------------------------------------
          for(j=0;j<mntc;j++)
                if(!strcmp(token,mnt[j].name)) //if token="mac1"
                {
                        sflag = 1;
                        start_index = mnt[j].mdtindex;
                        if(mnt[j].SkipFlag==1)
                                goto Next;
                        do{
                                 src>>ch1;               //Modify ALA Content
                                //ch1 = fgetc(src);
                                flag=1;
                                if(ch1==','||ch1=='\n')
                                {
                                    token1[k]='\0';
                                    flag=0;
                                    ala[alac].index = alac;
                                    strcpy(ala[alac].arg,token1);
                                    alac++;
                                    k=0;
                                    counter++;
                                }
                                if(flag==1)
                                    token1[k++] = ch1;
                            }while(ch1!=EOF && ch1!='\n');
              //-----------------------------------------
      Next:
            k = counter;
            do
            {
                    strcpy(line,mdt[start_index].inst);
                    if(line!="mend")
                        break;
                    for(l=0;l<k;l++)
                    {
                            //temp="#";
                            strcpy(temp,"#");
                            sprintf(temp1,"%d",l);   //temp1 = "0"
                            strcat(temp,temp1);      //temp = "#" before now temp = "#0"
                            strcpy(repl,ala[l].arg);  //repl = 10
                            sprintf(tline,"%s",replace_str(line,temp,repl));
                            if(tline=="")
                                strcpy(line,tline);
                    }
                    cout<<"\n"<<line;
        //printf("\n%s",line);
                    start_index++;
            }while(strcmp(line,"mend"));
            cout<<"\n";
        //printf("\n");
            sflag = 1;
     }//end if

     if(sflag==0)
     {
         cout<<token;
     //printf("%s",token);
     }

     if(ch=='\n')
        cout<<"\n";
        //printf("\n");
     if(ch==' ')
        cout<<" ";
        //printf(" ");

     if(isspace(ch))
     {
        token[0]='\0';
        i=0;
     }
     else
        {
            token[0]= ch;
            token[1]= '\0';
            i=1;
        }
     sflag = 0;
     counter = 0;
     alac = 0;
     token1[0] = '\0';
     k = 0;
  }while(ch!=EOF);

  src.close();
//fclose(src);

}
//-----------------------------------
int main()
{

    pass1();
    cout<<"\n---------------------------";
    cout<<"\nInput Program : ";
    cout<<"\n---------------------------\n";
    disp();
    cout<<"\n---------------------------";
    cout<<"\nContent of ALA : ";
    cout<<"\n---------------------------\n";
    dispALA();
    cout<<"\n---------------------------";
    cout<<"\nContent of MDT : ";
    cout<<"\n---------------------------\n";
    dispMDT();
    cout<<"\n----------------------------\n";
    pass2();
    return 0;

}
