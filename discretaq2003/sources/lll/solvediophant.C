#define zlength 16000 \

/*5:*/
#line 95 "solvediophant.w"

/*6:*/
#line 122 "solvediophant.w"

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/times.h>   
#include <unistd.h> 

#include "diophant.h"

/*:6*/
#line 96 "solvediophant.w"
;
/*7:*/
#line 136 "solvediophant.w"

int user_time,time_0,time_1;
char timestring[256];

/*8:*/
#line 147 "solvediophant.w"

int os_ticks()
{
struct tms tms_buffer;

if(-1==times(&tms_buffer))
return(-1);
return(tms_buffer.tms_utime);
}

int os_ticks_per_second()
{
int clk_tck= 1;

clk_tck= sysconf(_SC_CLK_TCK);
return(clk_tck);
}
/*:8*/
#line 140 "solvediophant.w"
;
/*9:*/
#line 166 "solvediophant.w"

int os_ticks_to_hms_tps(int ticks,int tps,int*h,int*m,int*s)
{
int l1;

l1= ticks/tps;
*s= l1%60;
l1-= *s;
l1/= 60;
*m= l1%60;
l1-= *m;
l1/= 60;
*h= l1;
return(1);
}

int os_ticks_to_hms(int ticks,int*h,int*m,int*s)
{
os_ticks_to_hms_tps(ticks,os_ticks_per_second(),h,m,s);
return(1);
}

/*:9*/
#line 141 "solvediophant.w"
;
/*10:*/
#line 188 "solvediophant.w"

void print_delta_time_tps(int l,int tps,char*str)
{
int h,m,s;

os_ticks_to_hms_tps(l,tps,&h,&m,&s);
sprintf(str,"%d:%02d:%02d",h,m,s);
}

void print_delta_time(int l,char*str)
{
print_delta_time_tps(l,os_ticks_per_second(),str);
}

/*:10*/
#line 142 "solvediophant.w"
;

/*:7*/
#line 97 "solvediophant.w"
;

int main(int argc,char*argv[]){
int i,j,flag;
/*12:*/
#line 211 "solvediophant.w"

#if defined(MPREC)
verylong factor_input= 0;
verylong norm_input= 0;
verylong*upperb;
verylong**A,*rhs;
#else
long factor_input;
long norm_input;
long*upperb;
long**A,*rhs;
#endif

int bkz_beta_input= 0;
int bkz_p_input= 0;
int iterate= 0;
int iterate_no= 0;
int silent;

int no_rows,no_columns;

long stop_after_solutions;
long stop_after_loops;
int free_RHS;
FILE*txt;
char*inputfile_name,*rowp;

char zeile[zlength];
char detectstring[100];

int*original_columns;
int no_original_columns;

/*:12*/
#line 101 "solvediophant.w"
;


/*13:*/
#line 244 "solvediophant.w"

if(argc<5){
printf("\nSyntax:\n\n");
printf("solvediophant [c0_factor] [max_norm] bkz [beta] [p] [filename]\n\n");
printf("Example:\n\n");
printf("solvediophant 1000 1 bkz 80 18 file.txt\n\n");
fflush(stdout);
return 1;
}
#if defined(MPREC)
zstrtoz(argv[1],&factor_input);
zstrtoz(argv[2],&norm_input);
#else
factor_input= atol(argv[1]);
norm_input= atol(argv[2]);
#endif

iterate= silent= 0;
for(i= 3;i<argc;i++){
if(strcmp(argv[i],"silent")==0){
silent= 1;
printf("No output of solutions, just counting.\n");
}
if(strcmp(argv[i],"iterate")==0){
iterate= 1;
iterate_no= atoi(argv[i+1]);
}
if(strcmp(argv[i],"bkz")==0){
iterate= 0;
bkz_beta_input= atoi(argv[i+1]);
bkz_p_input= atoi(argv[i+2]);
}
}
inputfile_name= argv[argc-1];

/*:13*/
#line 104 "solvediophant.w"
;
/*14:*/
#line 283 "solvediophant.w"

txt= fopen(inputfile_name,"r");
flag= 0;
free_RHS= 0;
stop_after_loops= 0;
stop_after_solutions= 0;
do{
fgets(zeile,zlength,txt);
if(strstr(zeile,"% stopafter")!=NULL){
sscanf(zeile,"%% stopafter %ld",&stop_after_solutions);
}
if(strstr(zeile,"% stoploops")!=NULL){
sscanf(zeile,"%% stoploops %ld",&stop_after_loops);
}
if(strstr(zeile,"% FREERHS")!=NULL){
free_RHS= 1;
}
}
while(zeile[0]=='%');
sscanf(zeile,"%d%d%d",&no_rows,&no_columns,&flag);

/*:14*/
#line 105 "solvediophant.w"
;
/*15:*/
#line 304 "solvediophant.w"

#if defined(MPREC)
A= (verylong**)calloc(no_rows,sizeof(verylong*));
for(j= 0;j<no_rows;j++){
A[j]= (verylong*)calloc(no_columns,sizeof(verylong));
for(i= 0;i<no_columns;i++)A[j][i]= 0;
for(i= 0;i<no_columns;i++)zzero(&(A[j][i]));
}
rhs= (verylong*)calloc(no_rows,sizeof(verylong));
for(i= 0;i<no_rows;i++)rhs[i]= 0;
for(i= 0;i<no_rows;i++)zzero(&(rhs[i]));
#else
A= (long**)calloc(no_rows,sizeof(long*));
for(j= 0;j<no_rows;j++){
A[j]= (long*)calloc(no_columns,sizeof(long));
for(i= 0;i<no_columns;i++)A[j][i]= 0;
}
rhs= (long*)calloc(no_rows,sizeof(long));
for(i= 0;i<no_rows;i++)rhs[i]= 0;
#endif  

/*:15*/
#line 106 "solvediophant.w"
;
/*16:*/
#line 325 "solvediophant.w"

#if defined(MPREC)
for(j= 0;j<no_rows;j++){
for(i= 0;i<no_columns;i++)
zfread(txt,&(A[j][i]));
zfread(txt,&(rhs[j]));
}
#else
for(j= 0;j<no_rows;j++){
for(i= 0;i<no_columns;i++)
fscanf(txt,"%ld",&(A[j][i]));
fscanf(txt,"%ld",&(rhs[j]));
}
#endif
/*:16*/
#line 107 "solvediophant.w"
;
/*17:*/
#line 341 "solvediophant.w"

sprintf(detectstring,"BOUNDS");
do{
rowp= fgets(zeile,zlength,txt);
}while((rowp!=NULL)&&(strstr(zeile,detectstring)==NULL));

if(rowp==NULL){
upperb= NULL;
printf("No %s \n",detectstring);fflush(stdout);
}else{
#if defined(MPREC)
upperb= (verylong*)calloc(no_columns,sizeof(verylong));
for(i= 0;i<no_columns;i++){
upperb[i]= 0;
zfread(txt,&(upperb[i]));
}
#else
upperb= (long*)calloc(no_columns,sizeof(long));
for(i= 0;i<no_columns;i++)
fscanf(txt,"%ld",&(upperb[i]));
#endif
}
fclose(txt);
txt= fopen(inputfile_name,"r");

/*:17*/
#line 108 "solvediophant.w"
;
/*18:*/
#line 367 "solvediophant.w"

sprintf(detectstring,"SELECTEDCOLUMNS");
do{
rowp= fgets(zeile,zlength,txt);
}while((rowp!=NULL)&&(strstr(zeile,detectstring)==NULL));

if(rowp!=NULL)fscanf(txt,"%d",&(no_original_columns));
else no_original_columns= no_columns;

original_columns= (int*)calloc(no_original_columns,sizeof(int));

if(rowp!=NULL){
for(i= 0;i<no_original_columns;i++)fscanf(txt,"%d",&(original_columns[i]));
}else{
for(i= 0;i<no_original_columns;i++)original_columns[i]= 1;
}
fclose(txt);

/*:18*/
#line 109 "solvediophant.w"
;


time_0= os_ticks();
diophant(A,rhs,upperb,no_columns,no_rows,
factor_input,norm_input,silent,iterate,iterate_no,bkz_beta_input,bkz_p_input,
stop_after_solutions,stop_after_loops,
free_RHS,original_columns,no_original_columns);
time_1= os_ticks();

/*11:*/
#line 202 "solvediophant.w"

user_time= time_1-time_0;
timestring[0]= 0;
print_delta_time(user_time,timestring);
printf("total enumeration time: %s\n",timestring);
fflush(stdout);

/*:11*/
#line 119 "solvediophant.w"
;
return 1;
}
/*:5*/
