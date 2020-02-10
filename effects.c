#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long parser(char *line,int end,int start){       //convert hex to decimal from end to start inclusively
	char s[100]; 
	char c[10];
	s[0]='\0';
	for (int i=end;i>=start;i-=3){
		strncpy(c,&(line[i]),2);
		c[2]='\0';
		//printf("%s\n",c);
		strcat(s,c);
	}
	//printf("%s\n",s);

	//printf("%d\n",op);
	return strtol(s,NULL,16);
}

void output_helper(short sample){
	char str[20],pa[20];
	sprintf(pa,"%08x",sample);
	strncpy(str,&(pa[6]),2);
	str[2]='\0';
	strcat(str," ");
	strncat(str,&(pa[4]),2);
	str[5]='\0';
	strcat(str," ");
	str[6]='\0';
	printf("%s", str);
}

int output_fin(char *line, int num, int *count, int start,int channel){     //output for fin
	for(int i=start;i<=55;i+=(6*channel)){
		//printf("%d\n",i);
		
			
			//printf("%lf\n",(*count)/(double)num);
			short sample = parser(line,i+3,i);
			if (*count<num){
				*count+=1;
				sample = sample*(*count)/(double)num;
			}
			output_helper(sample);

			if (channel==2){
				sample = parser(line,i+9,i+6);
				if (*count<num){
					sample = sample*(*count)/(double)num;
				}
				output_helper(sample);
			}
		
	}
	char tail[50];
	strncpy(tail,&(line[58]),17);
	tail[17]='\0';
	printf("%s\n",tail);
	return 0;	
}

int output_fout(char *line, int num, int *size, int start,int channel){            //ouput for fout
	for(int i=start;i<=55;i+=(6*channel)){
		short sample = parser(line,i+3,i);
		if (*size<num&&*size>0){
			*size-=1;
			sample = sample*(*size)/(double)num;
		}else if(*size>=num){
			*size-=1;
		}else if(*size==0){
			sample = 0;
		}
		output_helper(sample);

		if(channel==2){
			sample = parser(line,i+9,i+6);
			if (*size<num&&*size>0){
				sample = sample*(*size)/(double)num;
			}else if(*size==0){
				sample=0;
			}
			output_helper(sample);
		}
		//printf("%lf %d %d\n",(*size)/(double)num,*size, num);
		
	}
	char tail[50];
	strncpy(tail,&(line[58]),17);
	tail[17]='\0';
	printf("%s\n",tail);	
	return 0;
}

int output_pan(char *line, int num, int *count, int start){     //output for pan, support only stereo 
	for(int i=start;i<=55;i+=12){
		short sample = parser(line,i+3,i);
		if (*count<num){
			*count+=1;
			sample = sample*(num-*count)/(double)num;
		}else if(*count>=num){
			sample = 0;
		}
		output_helper(sample);

		sample = parser(line,i+9,i+6);
		if (*count<num){
			sample = sample*(*count)/(double)num;
		}
		output_helper(sample);
	}
	char tail[50];
	strncpy(tail,&(line[58]),17);
	tail[17]='\0';
	printf("%s\n",tail);
	return 0;	
}


int fin(int duration){
	char line[500];
	int line_num=1;
	int count=0;
	int num=0;              //number of samples need to be modified
	int channel=0;
	while(fgets(line,300,stdin)!=NULL){
		if(line_num==1){
			printf("%s",line);
		}else if(line_num==2){
			channel = parser(line,31,28);
			int rate = parser(line,43,34);
			num = (duration*rate)/1000; 
			printf("%s",line);  
		}else if(line_num==3){
			printf("%.46s",line);
			output_fin(line,num,&count,46,channel);
		}else{
			printf("%.10s",line);
			output_fin(line,num,&count,10,channel);
		}
		line_num++;
	}
	return 0;
}

int fout(int duration){
	char line[500];
	int line_num=1;
	int num=0;            //number of samples need to be modified
	int size=0;           //total number of samples
	int channel=0;
	while(fgets(line,300,stdin)!=NULL){
		if(line_num==1){
			printf("%s",line);
		}
		else if(line_num==2){
			channel = parser(line,31,28);
			int rate = parser(line,43,34);
			num = (duration*rate)/1000;
			printf("%s",line);   
		}else if(line_num==3){
			size = parser(line,43,34);
			size = size/(2*channel);
			printf("%.46s",line);
			output_fout(line,num,&size,46,channel);
		}else{
			printf("%.10s",line);
			output_fout(line,num,&size,10,channel);
		}
		line_num++;
	}
	return 0;
}

int pan(int duration){
	char line[500];
	int line_num=1;
	int count=0;
	int num=0;              //number of samples need to be modified
	while(fgets(line,300,stdin)!=NULL){
		if(line_num==1){
			printf("%s",line);
		}else if(line_num==2){
			int rate = parser(line,43,34);
			num = (duration*rate)/1000; 
			printf("%s",line);  
		}else if(line_num==3){
			printf("%.46s",line);
			output_pan(line,num,&count,46);
		}else{
			printf("%.10s",line);
			output_pan(line,num,&count,10);
		}
		line_num++;
	}
	return 0;
}

int main(int argc,char *argv[]){
	if(argc==3){
		int i = strtol(argv[2],NULL,10);
		if(strcmp(argv[1],"-fin")==0){
			fin(i);
		}else if(strcmp(argv[1],"-fout")==0){
			fout(i);
		}else if(strcmp(argv[1],"-pan")==0){
			pan(i);
		}else{
			printf("First input invalid!");
			return 1;
		}
	}else{
		printf("exactly two command line arguments needed!");
		return 1;
	}
	return 0;
}		 
