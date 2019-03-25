#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define N 128

#define T 1//退出请求
#define L 2//登录请求
#define QN 3//普通用户查询请求
#define MN 4//普通用户修改请求
#define QR 5//root用户查询请求
#define MR 6//root用户修改请求
#define AR 7//root用户添加请求
#define DR 8//root用户删除请求
#define HR 9//root用户查询历史请求


struct msg {
	int type;
	int type2;
	char name[N];
	char edge[N];
	char sex[N];
	char jnumber[N];
	char department[N];
	char passwd[N];
	char phone[N];
	int flags;
	char text[N];
	char timeer[N];
	char events[N];
};

//root用户按照名字查询
void do_query_root_name(int sockfd,struct msg msg)
{

	msg.type2 = 1;
	send(sockfd,&msg,sizeof(msg),0);

	recv(sockfd,&msg,sizeof(msg),0);

	printf("Please enter name >>>\n");
	scanf("%s",msg.name);
	getchar();
	send(sockfd,&msg,sizeof(msg),0);//发送客户端的名字信息

	recv(sockfd,&msg,sizeof(msg),0);//接收服务器端发送过来的员工信息
	if(strncmp(msg.text,"Suc",3) == 0){
		printf("%s\n", msg.text);
		printf("jnumber:%-4sname:%-7sedge:%-4ssex:%-7sdepartment:%-9spasswd:%-7sphone:%-11sflags:%d\n",msg.jnumber,msg.name,msg.edge,msg.sex,msg.department,msg.passwd,msg.phone,msg.flags);
	}else{
		printf("%s\n", msg.text);
	}
}

//root用户查询全部用户信息函数
void do_query_root_all(int sockfd,struct msg msg)
{
	msg.type2 = 2;
	send(sockfd,&msg,sizeof(msg),0);

	while(1){
		recv(sockfd,&msg,sizeof(msg),0);
		if(strncmp(msg.text,"Suc",3) == 0){
			printf("%s.\n",msg.text);
			return ;
		}else{
			printf("jnumber:%-4sname:%-7sedge:%-4ssex:%-7sdepartment:%-9spasswd:%-7sphone:%-11sflags:%d\n",msg.jnumber,msg.name,msg.edge,msg.sex,msg.department,msg.passwd,msg.phone,msg.flags);
			// return ;
		}
	}
}

//root用户查询函数
void do_query_root(int sockfd,struct msg msg)
{
	msg.type = QR;
	while(1){

		int choice;

		printf("**************************************************************.\n");
		printf("***Dear root user:********************************************.\n");
		printf("**********Welcome to the employee management system***********.\n");
		printf("**********Please select the way to find >>>*******************.\n");
		printf("*************1.name    2.all    3.quit************************.\n");
		printf("**************************************************************.\n");

		printf("please input your choice>>>");
		scanf("%d",&choice);
		getchar();

		switch(choice){
			case 1:
			do_query_root_name(sockfd,msg);
			break;
			case 2:
			do_query_root_all(sockfd,msg);
			break;
			case 3:
			goto end;
			default:
			printf("Input errors, please enter again>>>\n");
		}
	}
	end:
	return ;
}

//root用户修改姓名函数
void do_modify_root_name(int sockfd,struct msg msg)
{
	msg.type2 = 1;
	send(sockfd,&msg,sizeof(msg),0);
	recv(sockfd,&msg,sizeof(msg),0);
	
	printf("Please enter the number(eg. 1 2 3 ...) to be modified>>>\n");
	scanf("%s",msg.jnumber);
	getchar();

	printf("Please enter the name to be modified>>>\n");
	scanf("%s",msg.name);
	getchar();

	send(sockfd,&msg,sizeof(msg),0);//发送客户端的姓名修改信息

	recv(sockfd,&msg,sizeof(msg),0);
	printf("%s\n", msg.text);

}

//root用户修改年龄函数
void do_modify_root_edge(int sockfd,struct msg msg)
{
	msg.type2 = 2;
	send(sockfd,&msg,sizeof(msg),0);
	recv(sockfd,&msg,sizeof(msg),0);
	
	printf("Please enter the number(eg. 1 2 3 ...) to be modified>>>\n");
	scanf("%s",msg.jnumber);
	getchar();

	printf("Please enter the age to be modified>>>\n");
	scanf("%s",msg.edge);
	getchar();

	send(sockfd,&msg,sizeof(msg),0);//发送客户端的姓名修改信息

	recv(sockfd,&msg,sizeof(msg),0);
	printf("%s\n", msg.text);

}

//root用户修改性别函数
void do_modify_root_sex(int sockfd,struct msg msg)
{
	msg.type2 = 3;
	send(sockfd,&msg,sizeof(msg),0);
	recv(sockfd,&msg,sizeof(msg),0);
	
	printf("Please enter the number(eg. 1 2 3 ...) to be modified>>>\n");
	scanf("%s",msg.jnumber);
	getchar();

	printf("Please enter the sex to be modified>>>\n");
	scanf("%s",msg.sex);
	getchar();

	send(sockfd,&msg,sizeof(msg),0);//发送客户端的姓名修改信息

	recv(sockfd,&msg,sizeof(msg),0);
	printf("%s\n", msg.text);
}

//root用户修改部门函数
void do_modify_root_department(int sockfd,struct msg msg)
{
	msg.type2 = 4;
	send(sockfd,&msg,sizeof(msg),0);
	recv(sockfd,&msg,sizeof(msg),0);
	
	printf("Please enter the number(eg. 1 2 3 ...) to be modified>>>\n");
	scanf("%s",msg.jnumber);
	getchar();

	printf("Please enter the department to be modified>>>\n");
	scanf("%s",msg.department);
	getchar();

	send(sockfd,&msg,sizeof(msg),0);//发送客户端的姓名修改信息

	recv(sockfd,&msg,sizeof(msg),0);
	printf("%s\n", msg.text);

}

//root用户修改手机号函数
void do_modify_root_phone(int sockfd,struct msg msg)
{
	msg.type2 = 5;
	send(sockfd,&msg,sizeof(msg),0);
	recv(sockfd,&msg,sizeof(msg),0);
	
	printf("Please enter the number(eg. 1 2 3 ...) to be modified>>>\n");
	scanf("%s",msg.jnumber);
	getchar();

	printf("Please enter the phone to be modified>>>\n");
	scanf("%s",msg.phone);
	getchar();

	send(sockfd,&msg,sizeof(msg),0);//发送客户端的姓名修改信息

	recv(sockfd,&msg,sizeof(msg),0);
	printf("%s\n", msg.text);


}

//root用户修改密码函数
void do_modify_root_passwd(int sockfd,struct msg msg)
{
	msg.type2 = 6;
	send(sockfd,&msg,sizeof(msg),0);
	recv(sockfd,&msg,sizeof(msg),0);
	
	printf("Please enter the number(eg. 1 2 3 ...) to be modified>>>\n");
	scanf("%s",msg.jnumber);
	getchar();

	printf("Please enter the passwd to be modified>>>\n");
	scanf("%s",msg.passwd);
	getchar();

	send(sockfd,&msg,sizeof(msg),0);//发送客户端的姓名修改信息
	recv(sockfd,&msg,sizeof(msg),0);
	printf("%s\n", msg.text);
}

//root用户修改函数
void do_modify_root(int sockfd,struct msg msg)
{
	msg.type = MR;

	while(1){

		int choice;

		printf("************************************************************************************.\n");
		printf("***Dear root user:******************************************************************.\n");
		printf("***********************Welcome to the employee management system********************.\n");
		printf("*******************Please enter the option you want to modify >>>*******************.\n");
		printf("***********1.name 2.edge 3.sex 4.department 5.phone 6.passwd  7.quit****************.\n");
		printf("************************************************************************************.\n");

		printf("please input your choice>>>");
		scanf("%d",&choice);
		getchar();

		switch(choice){
			case 1:
			do_modify_root_name(sockfd,msg);
			break;
			case 2:
			do_modify_root_edge(sockfd,msg);
			break;
			case 3:
			do_modify_root_sex(sockfd,msg);
			break;
			case 4:
			do_modify_root_department(sockfd,msg);
			break;
			case 5:
			do_modify_root_phone(sockfd,msg);
			break;
			case 6:
			do_modify_root_passwd(sockfd,msg);
			break;
			case 7:
			goto end;
			default:
			printf("Input errors, please enter again>>>\n");
		}
	}
	end:
	return ;
}

//root用户添加函数
void do_add_root(int sockfd,struct msg msg)
{
	msg.type = AR;

	loop:
	printf("Please enter the job number(eg. 1 2 3 ...) to be added >>>\n");
	scanf("%s",msg.jnumber);
	getchar();

	send(sockfd,&msg,sizeof(msg),0);	
	recv(sockfd,&msg,sizeof(msg),0);
	if(strncmp(msg.text,"Err",3) == 0){
		printf("%s\n", msg.text);
		goto loop;
	}else{
		printf("%s\n", msg.text);
		printf("Please enter the job number(eg. 1 2 3 ...) to be added >>>\n");
		scanf("%s",msg.jnumber);
		getchar();

		printf("Please enter the name to be added >>>\n");
		scanf("%s",msg.name);
		getchar();

		printf("Please enter the edge to be added >>>\n");
		scanf("%s",msg.edge);
		getchar();

		printf("Please enter the sex(eg. man or women) to be added >>>\n");
		scanf("%s",msg.sex);
		getchar();

		printf("Please enter the department to be added >>>\n");
		scanf("%s",msg.department);
		getchar();

		printf("Please enter the passwd to be added >>>\n");
		scanf("%s",msg.passwd);
		getchar();

		printf("Please enter the phone to be added >>>\n");
		scanf("%s",msg.phone);
		getchar();

		printf("Please enter the flags(eg. 1:root  0:normal) to be added >>>\n");
		scanf("%d",&msg.flags);
		getchar();

		send(sockfd,&msg,sizeof(msg),0);
		recv(sockfd,&msg,sizeof(msg),0);
		printf("%s\n", msg.text);//服务器端提示删除成功  User deleted successfully
	}
}

//root用户删除函数
void do_delete_root(int sockfd,struct msg msg)
{
	msg.type = DR;
	loop:
	printf("Please enter the job number(eg. 1 2 3 ...) to be deleted >>>\n");
	scanf("%s",msg.jnumber);
	getchar();

	send(sockfd,&msg,sizeof(msg),0);
	recv(sockfd,&msg,sizeof(msg),0);
	if(strncmp(msg.text,"Err",3) == 0){
		printf("%s\n", msg.text);
		goto loop;
	}else{
		printf("%s\n", msg.text);//服务器端提示删除成功  User deleted successfully
	}
	
}

//root用户查询历史记录函数
void do_query_history_root(int sockfd,struct msg msg)
{
	msg.type = HR;
	send(sockfd,&msg,sizeof(msg),0);

	while(1){
		recv(sockfd,&msg,sizeof(msg),0);
		if(strncmp(msg.text,"Suc",3) == 0){
			printf("%s.\n",msg.text);
			return ;
		}else{
			printf("time:%-18sname:%-7sevents:%s\n",msg.timeer,msg.name,msg.events);
		}
	}

}

//root用户处理函数
void root_handler(int sockfd,struct msg msg)
{
	msg.type = L;
	msg.flags = 1;
	printf("please input your name >>>");
	scanf("%s",msg.name);
	getchar();

	printf("please input your passwd >>>");
	scanf("%s",msg.passwd);
	getchar();

	send(sockfd,&msg,sizeof(msg),0);
	recv(sockfd,&msg,sizeof(msg),0);

	if(strncmp(msg.text,"YES",3) == 0){
		while(1){
			int choice;
			printf("**************************************************************.\n");
			printf("***Dear root user:********************************************.\n");
			printf("**********Welcome to the employee management system***********.\n");
			printf("*****************Please enter your choice >>>*****************.\n");
			printf("***1.query 2.modify  3.add 4.delete 5.query_history 6.quit****.\n");
			printf("**************************************************************.\n");

			printf("please input your choice>>>");
			scanf("%d",&choice);
			getchar();

			switch(choice){
				case 1:
				do_query_root(sockfd,msg);
				break;
				case 2:
				do_modify_root(sockfd,msg);
				break;
				case 3:
				do_add_root(sockfd,msg);
				break;
				case 4:
				do_delete_root(sockfd,msg);
				break;
				case 5:
				do_query_history_root(sockfd,msg);
				break;
				case 6:
				//close(sockfd);
				goto end;
				default:
				printf("Input errors, please enter again>>>\n");
			}
		}
	}else{
		printf("%s.\n",msg.text);//服务器端反馈：用户名不存在或者密码错误
	}
	end:
	return ;
}

//普通用户查询自己的全部信息
void do_query_normal_yourself(int sockfd,struct msg msg)
{
	/*msg.type = QN;
	msg.flags = 0;
	send(sockfd,&msg,sizeof(msg),0);

	recv(sockfd,&msg,sizeof(msg),0);//接收服务器端发送过来的员工信息
	if(strncmp(msg.text,"Suc",3) == 0){
		printf("%s\n", msg.text);
		printf("jnumber:%-5sname:%-5sedge:%-5ssex:%-5sdepartment:%-5sphone:%-5spasswd:%-5sflags:%d\n",msg.jnumber,msg.name,msg.edge,msg.sex,msg.department,msg.phone,msg.passwd,msg.flags);
	}else{
		printf("%s\n", msg.text);
	}*/
}

//普通用户查询函数
void do_query_normal(int sockfd,struct msg msg)
{
	msg.type = QN;
	msg.flags = 0;
	send(sockfd,&msg,sizeof(msg),0);

	recv(sockfd,&msg,sizeof(msg),0);//接收服务器端发送过来的员工信息
	if(strncmp(msg.text,"Suc",3) == 0){
		printf("%s\n", msg.text);
				printf("jnumber:%-4sname:%-7sedge:%-4ssex:%-7sdepartment:%-9spasswd:%-7sphone:%-11sflags:%d\n",msg.jnumber,msg.name,msg.edge,msg.sex,msg.department,msg.passwd,msg.phone,msg.flags);
	}else{
		printf("%s\n", msg.text);
	}

	/*while(1){

		int choice;

		printf("**************************************************************.\n");
		printf("***Dear ordinary user:****************************************.\n");
		printf("**********Welcome to the employee management system***********.\n");
		printf("*****************Please enter your choice >>>*****************.\n");
		printf("*****************1.query yourself    2.quit*******************.\n");
		printf("**************************************************************.\n");

		printf("please input your choice>>>");
		scanf("%d",&choice);
		getchar();

		switch(choice){
			case 1:
			do_query_normal_yourself(sockfd,msg);
			break;
			case 2:
			//close(sockfd);
			goto end;
		}
	}
	end:
	return ;*/
}

//普通用户修改年龄函数
void do_modify_normal_edge(int sockfd,struct msg msg)
{
	msg.type2 = 1;
	send(sockfd,&msg,sizeof(msg),0);

	recv(sockfd,&msg,sizeof(msg),0);
	printf("Please enter your age>>>\n");
	scanf("%s",msg.edge);
	getchar();

	// printf("Please enter your jobnumber>>>\n");
	// scanf("%s",msg.jnumber);
	// getchar();

	send(sockfd,&msg,sizeof(msg),0);//发送客户端的年龄修改信息

	recv(sockfd,&msg,sizeof(msg),0);

	printf("%s\n", msg.text);
	
}

//普通用户修改手机号函数
void do_modify_normal_phone(int sockfd,struct msg msg)
{
	msg.type2 = 2;
	send(sockfd,&msg,sizeof(msg),0);

	recv(sockfd,&msg,sizeof(msg),0);
	printf("Please enter your phone>>>\n");
	scanf("%s",msg.phone);
	getchar();

	// printf("Please enter your jobnumber>>>\n");
	// scanf("%s",msg.jnumber);
	// getchar();

	send(sockfd,&msg,sizeof(msg),0);//发送客户端的手机号修改信息

	recv(sockfd,&msg,sizeof(msg),0);

	printf("%s\n", msg.text);
}

//普通用户修改密码函数
void do_modify_normal_passwd(int sockfd,struct msg msg)
{
	msg.type2 = 3;
	send(sockfd,&msg,sizeof(msg),0);

	recv(sockfd,&msg,sizeof(msg),0);
	printf("Please enter your passwd>>>\n");
	scanf("%s",msg.passwd);
	getchar();

	// printf("Please enter your jobnumber>>>\n");
	// scanf("%s",msg.jnumber);
	// getchar();

	send(sockfd,&msg,sizeof(msg),0);//发送客户端的密码修改信息

	recv(sockfd,&msg,sizeof(msg),0);

	printf("%s\n", msg.text);
}

//普通用户修改函数
void do_modify_normal(int sockfd,struct msg msg)
{
	msg.type = MN;
	while(1){
		int choice;

		printf("**************************************************************.\n");
		printf("***Dear ordinary user:****************************************.\n");
		printf("**********Welcome to the employee management system***********.\n");
		printf("*****************Please enter your choice >>>*****************.\n");
		printf("****************1.edge  2.phone   3.passwd   4.quit***********.\n");
		printf("**************************************************************.\n");

		printf("please input your choice>>>");
		scanf("%d",&choice);
		getchar();

		switch(choice){
			case 1:
			do_modify_normal_edge(sockfd,msg);
			break;
			case 2:
			do_modify_normal_phone(sockfd,msg);
			break;
			case 3:
			do_modify_normal_passwd(sockfd,msg);
			break;
			case 4:
			//close(sockfd);
			goto end;
			default:
			printf("Input errors, please enter again>>>\n");
		}
	}
	end:
	return ;
}

//普通用户处理函数
void normal_handler(int sockfd,struct msg msg)
{
	msg.type = L;
	msg.flags = 0;

	printf("please input your name >>>");
	scanf("%s",msg.name);
	getchar();

	printf("please input your passwd >>>");
	scanf("%s",msg.passwd);
	getchar();

	send(sockfd,&msg,sizeof(msg),0);
	recv(sockfd,&msg,sizeof(msg),0);

	if(strncmp(msg.text,"YES",3) == 0){
		while(1){
			int choice;

			printf("**************************************************************.\n");
			printf("***Dear ordinary user:****************************************.\n");
			printf("**********Welcome to the employee management system***********.\n");
			printf("*************Please enter your choice >>>*********************.\n");
			printf("*************1.query    2.modify    3.quit********************.\n");
			printf("**************************************************************.\n");


			printf("please input your choice>>>");
			scanf("%d",&choice);
			getchar();

			switch(choice){
				case 1:
				do_query_normal(sockfd,msg);
				break;
				case 2:
				do_modify_normal(sockfd,msg);
				break;
				case 3:
				//close(sockfd);
				goto end;
				default:
				printf("Input errors, please enter again>>>\n");
			}
		}
	}else{
		printf("%s.\n",msg.text);//服务器端反馈：用户名不存在或者密码错误
	}
	end:
	return ;
}

//用户退出函数
void do_quit(int sockfd,struct msg msg)
{
	msg.type = T;
	send(sockfd,&msg,sizeof(msg),0);
	recv(sockfd,&msg,sizeof(msg),0);
	if(strncmp(msg.text,"quit",4) == 0)
		printf("thank you for your service.\n");
}

int main(int argc, const char *argv[])
{
	//socket->填充->绑定->监听->等待连接->数据交互->关闭 
	int sockfd;
	int acceptfd;
	ssize_t recvbytes,sendbytes;
	char buf[N] = {0};
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	socklen_t addrlen = sizeof(serveraddr);
	socklen_t cli_len = sizeof(clientaddr);
	//创建网络通信的套接字
	sockfd = socket(AF_INET,SOCK_STREAM, 0);
	if(sockfd == -1){
		perror("socket failed.\n");
		exit(-1);
	}
	printf("sockfd :%d.\n",sockfd); 

	//填充网络结构体
	memset(&serveraddr,0,sizeof(serveraddr));
	memset(&clientaddr,0,sizeof(clientaddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port   = htons(atoi(argv[2]));
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);

	if(connect(sockfd,(const struct sockaddr *)&serveraddr,addrlen) == -1){
		perror("connect failed.\n");
		exit(-1);
	}

	int choice;
	struct msg msg;
	while(1){
		printf("**************************************************************.\n");
		printf("**********Welcome to the staff management system**************.\n");
		printf("**************Please enter your choice >>>********************.\n");
		printf("**************1.root-usr 2.normal-usr 3 quit******************.\n");
		printf("**************************************************************.\n");

		printf("input your choice:");
		scanf("%d",&choice);
		getchar();

		switch(choice){
			case 1:
			root_handler(sockfd,msg);
			break;
			case 2:
			normal_handler(sockfd,msg);
			break;
			case 3:
			do_quit(sockfd,msg);
			exit(EXIT_SUCCESS);
			default:
			printf("Input errors, please enter again>>>\n");
		}

	}
	close(sockfd);

	return 0;
}