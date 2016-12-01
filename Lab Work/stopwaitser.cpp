#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int recvwithtimeout(int s,char * buf,int len,int timeout,int utime)
{
fd_set fds;
int n;
struct timeval tv;
FD_ZERO(&fds);
FD_SET(s,&fds);
tv.tv_sec=timeout;
tv.tv_usec=utime;
n=select(s+1,&fds,NULL,NULL,&tv);
if(n==0)
return -2;//timeout
if(n==-1)
return -1;//error
return recv(s,buf,len,0);
}

string crcxor(string a,string b)
{
	//cout<<a.length()<<endl<<b.length()<<endl;
	int len=a.length();
	for(int i=0;i<a.length();i++)
	{
		//cout<<i<<endl;
		if(a[i]==b[i])
		{
			a[i]='0';
		}
		else
		{
			a[i]='1';
		}
	}
	//a[len]='\0';
	return a;
}

string crc(string a,string b)
{
string zero(b.length(),'0');
	if(zero==a)
	{
	//cout<<a<<endl;
	return "ZERO";
	}
	//cout<<a.length()<<endl<<b.length()<<endl;
	if(a.length()<b.length())
	{
		//cout<<a<<endl<<b<<endl;
		return a;
	}
	string x=crcxor(a.substr(0,b.length()),b);
	//cout << "X = " << x <<endl;
	int fin=x.find("1");
	if(fin!=string::npos)
		//cout<<fin;
		x=x.substr(fin,x.length()-fin);
	//cout << "X = " <<x<<endl;
	string str=x+a.substr(b.length(),a.length()-b.length()+1);
	//cout<<str<<endl;
	string s=crc(str,b);
	//cout << "S = " << s << endl;
	return s;
}

string append(string a ,int b)
{
for(int i=0;i<b;i++)
{
a=a+'0';
}
return a;
}

string appendwithremainder(string mes,string rem)
{
int j=mes.length()-1;
for(int i=rem.length()-1;i>=0;i--)
{
mes[j]=rem[i];
j--;
}
return mes;
}

bool noError(string mes, string poly)
{
string cr=crc(mes,poly);
return cr=="ZERO";
}

int main(int argc,char* argv[])
{
  if(argc !=2)
    {
      cerr << "Usage syntax: ./server <port_no>" << endl;
      return 0;
    }
  int sockfd;
  string bitstr="01010010111010111011101";
  string divisor="100000111";
  struct sockaddr_in my_addr;

  sockfd=socket(PF_INET,SOCK_STREAM,0);  //Creating the socket

  my_addr.sin_family=AF_INET;
  my_addr.sin_port=htons(atoi(argv[1]));
  my_addr.sin_addr.s_addr=INADDR_ANY;
  memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

  int temp=bind(sockfd, (struct sockaddr *)&my_addr, (sizeof my_addr)); //Binding the socket to the address

  if(temp==0)
    cout << "Server started running at "<< inet_ntoa(my_addr.sin_addr)  << ":" << argv[1] << endl;
  else
    {
      cout << "Failed to bind socket" << endl;
      return -1;
    }

  listen(sockfd,10);/*Have a file divide it into 64 pkts,put error at random places and send it, do CRC and */
  while(1)
  {
  struct sockaddr_in client_addr;
  socklen_t client_size=sizeof client_addr;
  int client_fd=accept(sockfd,(struct sockaddr*)&client_addr, &client_size);

  cout << "Accepted incoming connection" << endl;
  int i=0,j=0;
  //cout<<bitstr<<endl;
  bitstr=append(bitstr,divisor.length()-1);
  //cout<<bitstr<<endl;
  bitstr=appendwithremainder(bitstr,crc(bitstr,divisor));
  //cout<<bitstr<<endl;
  //cout<<noError(bitstr,divisor)<<endl;
  char * buf=new char[bitstr.length()+1];
  strcpy(buf,bitstr.c_str());
  while(j<=10)
  {
  char BS[12];
/*  if(recvwithtimeout(client_fd,BS,sizeof BS,0,1))
  cout<<"Delay ACK received"<<endl;*/
  cout<<j<<endl;
  srand(time(NULL));
 // int ran=(rand()%2)*3;
  char bu[2];
  //sleep(ran);
  //cout<<ran<<endl;
  int x=rand()%3;
  cout<<x<<endl;
  if(x==0)
  {
  //cout<<stringarr[i%2]<<endl;
  send(client_fd,buf,sizeof buf,0);//ACK
  }
  else if(x==1)
  {
  send(client_fd,"11011010101010011011100",sizeof buf,0);//NACK
  }//if 2 then frame is dropped
  int time=recvwithtimeout(client_fd,bu,sizeof(int),2,0);
  int buf=atoi(bu);
  if(time==-2)
  {
  cout<<"Timeout"<<endl;// This will contain all cases where it waits more than it's time out time(Frame Drops,Ack drops,Nack drops,Delayed Ack and Delayed Nack).Hence retransmission takes place
  //j++;
  sleep(1);
  continue;
  }
  if(time>0)
  {
  if(buf==0)
  {
  cout<<"NACK"<<endl;
  j++;
  sleep(1);
  continue;
  }
  if(buf==1)
  {
  cout<<"ACK"<<endl;//Successful Transmission takes place so counter is inc
  i++;
  j++;
  sleep(1);
  continue;
  }
  }
  }
  }
  //close(client_fd);
  close(sockfd);
  return 0;
  }
