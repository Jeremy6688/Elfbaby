#include<stdio.h>
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
#include <time.h>
#include <sqlite3.h>
#include <signal.h>

#define N 128
#define DATABASE "database.db"

#define handle_error(msg) \
do { perror(msg); exit(EXIT_FAILURE); } while(0)

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
	char time[N];
	char events[N];
};

void get_system_time(char* timedata)
{
	time_t t;
	struct tm *tp;

	time(&t);
	tp = localtime(&t);
	sprintf(timedata,"%d-%d-%d %d:%d:%d",tp->tm_year+1900,tp->tm_mon+1,\
			tp->tm_mday,tp->tm_hour,tp->tm_min,tp->tm_sec);
	return ;
}

//root用户和普通用户登录请求处理函数
void process_login_request(int acceptfd,struct msg msg,sqlite3 *db)
{
	char sql[N] = {0};
	char *errmsg;
	char **result;
	int nrow,ncolumn;

	
	sprintf(sql,"select * from usrinfo where name='%s' and flags= %d and passwd='%s';",msg.name,msg.flags,msg.passwd);
	// sprintf(sql,"select * from usrinfo where name='%s' and passwd='%s';",msg.name,msg.passwd);
	if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg) != SQLITE_OK){
		printf("%s.\n",errmsg);		
	}

	if(nrow == 0){
		strcpy(msg.text,"Username or password is error or please log in to the normal user !\n");
		send(acceptfd,&msg,sizeof(msg),0);
		return;
	}else{
		strcpy(msg.text,"YES");
		send(acceptfd,&msg,sizeof(msg),0);
		return;
	}
}

//普通用户查询请求函数
void process_query_normal_request(int acceptfd,struct msg msg,sqlite3 *db)
{
	char sql[N] = {0};
	char *errmsg;
	char **result;
	int nrow,ncolumn;

	sprintf(sql,"select * from usrinfo where name='%s' and flags= %d;",msg.name,msg.flags);
	if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg) != SQLITE_OK){
		printf("%s.\n",errmsg);		
	}

	if(nrow == 0){
		strcpy(msg.text,"Staff name does not exist !\n");
		send(acceptfd,&msg,sizeof(msg),0);
		return;
	}else{
		strcpy(msg.text,"Successful information query !");
		strcpy(msg.jnumber,result[8]);
		strcpy(msg.name,result[9]);
		strcpy(msg.edge,result[10]);
		strcpy(msg.sex,result[11]);
		strcpy(msg.department,result[12]);
		strcpy(msg.passwd,result[13]);
		strcpy(msg.phone,result[14]);
		msg.flags = (*result[15] - 48);
		send(acceptfd,&msg,sizeof(msg),0);
		return;
	}

}

//普通用户修改年龄请求函数
void process_modify_normal_edge_request(int acceptfd,struct msg msg,sqlite3 *db)
{
	char sql[N] = {0};
	char *errmsg;
	char **result;
	int nrow,ncolumn;

	send(acceptfd,&msg,sizeof(msg),0);

	recv(acceptfd,&msg,sizeof(msg),0);//接收客户端发送的年龄修改信息，然后开始修改

	//此处存在bug,因为工号是唯一的，依据姓名修改的话，会将同姓名的人的年龄信息也给修改了
	sprintf(sql,"update usrinfo set edge='%s' where name='%s';",msg.edge,msg.name);
	if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg) != SQLITE_OK){
		printf("%s.\n",errmsg);		
	}

	strcpy(msg.text,"Modify age success !");
	send(acceptfd,&msg,sizeof(msg),0);
	return;
	

}

//普通用户修改年龄手机号函数
void process_modify_normal_phone_request(int acceptfd,struct msg msg,sqlite3 *db)
{
	char sql[N] = {0};
	char *errmsg;
	char **result;
	int nrow,ncolumn;

	send(acceptfd,&msg,sizeof(msg),0);

	recv(acceptfd,&msg,sizeof(msg),0);//接收客户端发送的手机号修改信息，然后开始修改
	
	//此处存在bug,因为工号是唯一的，依据姓名修改的话，会将同姓名的人的年龄信息也给修改了
	sprintf(sql,"update usrinfo set phone='%s' where name='%s';",msg.phone,msg.name);
	if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg) != SQLITE_OK){
		printf("%s.\n",errmsg);		
	}

	strcpy(msg.text,"Modify phone success !");
	send(acceptfd,&msg,sizeof(msg),0);
	return;
}

//普通用户修改密码请求函数
void process_modify_normal_passwd_request(int acceptfd,struct msg msg,sqlite3 *db)
{
	char sql[N] = {0};
	char *errmsg;
	char **result;
	int nrow,ncolumn;

	send(acceptfd,&msg,sizeof(msg),0);

	recv(acceptfd,&msg,sizeof(msg),0);//接收客户端发送的手机号修改信息，然后开始修改
	
	//此处存在bug,因为工号是唯一的，依据姓名修改的话，会将同姓名的人的年龄信息也给修改了
	sprintf(sql,"update usrinfo set passwd='%s' where name='%s';",msg.passwd,msg.name);
	if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg) != SQLITE_OK){
		printf("%s.\n",errmsg);		
	}

	strcpy(msg.text,"Modify passwd success !");
	send(acceptfd,&msg,sizeof(msg),0);
	return;
}

//普通用户修改请求函数
void process_modify_normal_request(int acceptfd,struct msg msg,sqlite3 *db)
{
	switch(msg.type2){
		case 1:process_modify_normal_edge_request(acceptfd, msg, db);
		break;
		case 2:process_modify_normal_phone_request(acceptfd, msg, db);
		break;
		case 3:process_modify_normal_passwd_request(acceptfd, msg, db);
		break;
	}
}

//root用户查询名字请求
void process_query_root_name_request(int acceptfd,struct msg msg,sqlite3 *db)
{
	char sql[N] = {0};
	char *errmsg;
	char **result;
	int nrow,ncolumn;

	send(acceptfd,&msg,sizeof(msg),0);

	recv(acceptfd,&msg,sizeof(msg),0);//接收客户端发送的名字信息，然后开始查询

	sprintf(sql,"select * from usrinfo where name='%s';",msg.name);
	if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg) != SQLITE_OK){
		printf("%s.\n",errmsg);		
	}

	if(nrow == 0){
		strcpy(msg.text,"Staff name does not exist !\n");
		send(acceptfd,&msg,sizeof(msg),0);
		return;
	}else{
		strcpy(msg.text,"Successful information query !");
		strcpy(msg.jnumber,result[8]);
		strcpy(msg.name,result[9]);
		strcpy(msg.edge,result[10]);
		strcpy(msg.sex,result[11]);
		strcpy(msg.department,result[12]);
		strcpy(msg.passwd,result[13]);
		strcpy(msg.phone,result[14]);
		msg.flags = (*result[15] - 48);
		send(acceptfd,&msg,sizeof(msg),0);
		return;
	}
}

//root用户查询全部员工信息请求
void process_query_root_all_request(int acceptfd,struct msg msg,sqlite3 *db)
{
	// printf("----------%s-------------_%d.\n",__func__,__LINE__);
	char sql[N] = {0};
	char *errmsg;
	char **result;
	int nrow,ncolumn;
	int i = 0;

	sprintf(sql,"select * from usrinfo;");
	if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg) != SQLITE_OK){
		printf("%s.\n",errmsg);		
	}
	if(nrow == 0){
		strcpy(msg.text,"Staff name does not exist !\n");
		send(acceptfd,&msg,sizeof(msg),0);
		return;
	}else{
		for(i = 8;i < (nrow + 1)*ncolumn;i += 8){
			strcpy(msg.jnumber,result[i]);
			strcpy(msg.name,result[i + 1]);
			strcpy(msg.edge,result[i + 2]);
			strcpy(msg.sex,result[i + 3]);
			strcpy(msg.department,result[i + 4]);
			strcpy(msg.passwd,result[i + 5]);
			strcpy(msg.phone,result[i + 6]);
			msg.flags = (*result[i + 7] - 48);
			send(acceptfd,&msg,sizeof(msg),0);
		}

		strcpy(msg.text,"Successful information query !");
		send(acceptfd,&msg,sizeof(msg),0);
		return;
	}
}

//root用户查询请求函数
void process_query_root_request(int acceptfd,struct msg msg,sqlite3 *db)
{
	switch(msg.type2){
		case 1:process_query_root_name_request(acceptfd, msg, db);
		break;
		case 2:process_query_root_all_request(acceptfd, msg, db);
		break;
	}
}

//root用户修改名字请求函数
void process_modify_root_name_request(int acceptfd,struct msg msg,sqlite3 *db)
{
	char sql[N] = {0};
	char *errmsg;
	char **result;
	int nrow,ncolumn;

	send(acceptfd,&msg,sizeof(msg),0);
	
	recv(acceptfd,&msg,sizeof(msg),0);//接收客户端发送的姓名修改信息，然后开始修改

	sprintf(sql,"update usrinfo set name='%s' where jnumber='%s';",msg.name,msg.jnumber);
	if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg) != SQLITE_OK){
		printf("%s.\n",errmsg);		
	}

	strcpy(msg.text,"Modify name success !");
	send(acceptfd,&msg,sizeof(msg),0);
	return;
}

//root用户修改年龄请求函数
void process_modify_root_edge_request(int acceptfd,struct msg msg,sqlite3 *db)
{
	char sql[N] = {0};
	char *errmsg;
	char **result;
	int nrow,ncolumn;

	send(acceptfd,&msg,sizeof(msg),0);
	
	recv(acceptfd,&msg,sizeof(msg),0);//接收客户端发送的姓名修改信息，然后开始修改

	sprintf(sql,"update usrinfo set edge='%s' where jnumber='%s';",msg.edge,msg.jnumber);
	if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg) != SQLITE_OK){
		printf("%s.\n",errmsg);		
	}

	strcpy(msg.text,"Modify age success !");
	send(acceptfd,&msg,sizeof(msg),0);
	return;
}

//root用户修改性别请求函数
void process_modify_root_sex_request(int acceptfd,struct msg msg,sqlite3 *db)
{
	char sql[N] = {0};
	char *errmsg;
	char **result;
	int nrow,ncolumn;

	send(acceptfd,&msg,sizeof(msg),0);
	
	recv(acceptfd,&msg,sizeof(msg),0);//接收客户端发送的姓名修改信息，然后开始修改

	sprintf(sql,"update usrinfo set sex='%s' where jnumber='%s';",msg.sex,msg.jnumber);
	if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg) != SQLITE_OK){
		printf("%s.\n",errmsg);		
	}

	strcpy(msg.text,"Modify sex success !");
	send(acceptfd,&msg,sizeof(msg),0);
	return;
}

//root用户修改部门请求函数
void process_modify_root_department_request(int acceptfd,struct msg msg,sqlite3 *db)
{
	char sql[N] = {0};
	char *errmsg;
	char **result;
	int nrow,ncolumn;

	send(acceptfd,&msg,sizeof(msg),0);
	
	recv(acceptfd,&msg,sizeof(msg),0);//接收客户端发送的姓名修改信息，然后开始修改

	sprintf(sql,"update usrinfo set department='%s' where jnumber='%s';",msg.department,msg.jnumber);
	if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg) != SQLITE_OK){
		printf("%s.\n",errmsg);		
	}

	strcpy(msg.text,"Modify department success !");
	send(acceptfd,&msg,sizeof(msg),0);
	return;
}

//root用户修改手机号请求函数
void process_modify_root_phone_request(int acceptfd,struct msg msg,sqlite3 *db)
{
	char sql[N] = {0};
	char *errmsg;
	char **result;
	int nrow,ncolumn;

	send(acceptfd,&msg,sizeof(msg),0);
	
	recv(acceptfd,&msg,sizeof(msg),0);//接收客户端发送的姓名修改信息，然后开始修改

	sprintf(sql,"update usrinfo set phone='%s' where jnumber='%s';",msg.phone,msg.jnumber);
	if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg) != SQLITE_OK){
		printf("%s.\n",errmsg);		
	}

	strcpy(msg.text,"Modify phone success !");
	send(acceptfd,&msg,sizeof(msg),0);
	return;
}

void process_modify_root_passwd_request(int acceptfd,struct msg msg,sqlite3 *db)
{
	char sql[N] = {0};
	char *errmsg;
	char **result;
	int nrow,ncolumn;

	send(acceptfd,&msg,sizeof(msg),0);
	
	recv(acceptfd,&msg,sizeof(msg),0);//接收客户端发送的姓名修改信息，然后开始修改

	sprintf(sql,"update usrinfo set passwd='%s' where jnumber='%s';",msg.passwd,msg.jnumber);
	if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg) != SQLITE_OK){
		printf("%s.\n",errmsg);		
	}
	strcpy(msg.text,"Modify passwd success !");
	send(acceptfd,&msg,sizeof(msg),0);
	return;
}

//root用户修改请求函数
void process_modify_root_request(int acceptfd,struct msg msg,sqlite3 *db)
{
	// printf("----------%s-------------_%d.\n",__func__,__LINE__);
	switch(msg.type2){
		case 1:process_modify_root_name_request(acceptfd, msg, db);
		break;
		case 2:process_modify_root_edge_request(acceptfd, msg, db);
		break;
		case 3:process_modify_root_sex_request(acceptfd, msg, db);
		break;
		case 4:process_modify_root_department_request(acceptfd, msg, db);
		break;
		case 5:process_modify_root_phone_request(acceptfd, msg, db);
		break;
		case 6:process_modify_root_passwd_request(acceptfd, msg, db);
		break;
	}

}

//root用户添加请求函数
void process_add_root_request(int acceptfd,struct msg msg,sqlite3 *db)
{
	char sql[N] = {0};
	char *errmsg;
	if(strncmp(msg.jnumber,"1",1) != 0){
		
		strcpy(msg.text,"Please add staff information ! ^_^ ");
		send(acceptfd,&msg,sizeof(msg),0);

		recv(acceptfd,&msg,sizeof(msg),0);
		sprintf(sql,"insert into usrinfo values('%s','%s','%s','%s','%s','%s','%s', %d);",msg.jnumber,msg.name,msg.edge,msg.sex,msg.department,msg.passwd,msg.phone,msg.flags);
		if(sqlite3_exec(db,sql,NULL,NULL,&errmsg) != SQLITE_OK){
			printf("%s.\n",errmsg);		
		}else{
			printf("User added successfully.\n");
			strcpy(msg.text,"User added successfully !");
			send(acceptfd,&msg,sizeof(msg),0);
		}
	}else{
		strcpy(msg.text,"Error! User added failed.Please add another job number !");
		send(acceptfd,&msg,sizeof(msg),0);
	}

	return;
}

//root用户删除请求函数
void process_delete_root_request(int acceptfd,struct msg msg,sqlite3 *db)
{
	char sql[N] = {0};
	char *errmsg;

	if(strncmp(msg.jnumber,"1",1) != 0){
		sprintf(sql,"delete from usrinfo where jnumber = '%s';",msg.jnumber);
		if(sqlite3_exec(db,sql,NULL,NULL,&errmsg) != SQLITE_OK){
			printf("%s.\n",errmsg);		
		}else{
			printf("User deleted successfully.\n");
			strcpy(msg.text,"User deleted successfully !");
			send(acceptfd,&msg,sizeof(msg),0);
			return;
		}
	}else{
		strcpy(msg.text,"Error! User deleted failed.Please try again !");
		send(acceptfd,&msg,sizeof(msg),0);
	}

	strcpy(msg.text,"User deleted successfully !");
	send(acceptfd,&msg,sizeof(msg),0);
	return;
}

//root用户查询历史记录请求处理函数
void process_query_history_root_request(int acceptfd,struct msg msg,sqlite3 *db)
{
	char sql[N] = {0};
	char *errmsg;
	char **result;
	int nrow,ncolumn;
	int i = 0;

	sprintf(sql,"select * from historyinfo;");
	if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg) != SQLITE_OK){
		printf("%s.\n",errmsg);		
	}
	if(nrow == 0){
		strcpy(msg.text,"Query history root failed !\n");
		send(acceptfd,&msg,sizeof(msg),0);
		return;
	}else{
		for(i = 3;i < (nrow + 1)*ncolumn;i += 3){
			strcpy(msg.time,result[i]);
			strcpy(msg.name,result[i + 1]);
			strcpy(msg.events,result[i + 2]);
			send(acceptfd,&msg,sizeof(msg),0);
		}

		strcpy(msg.text,"Successful history information query !");
		send(acceptfd,&msg,sizeof(msg),0);
		return;
	}

}

void process_quit_request(int acceptfd,struct msg msg,sqlite3 *db)  
{
	//printf("----------%s-------------_%d.\n",__func__,__LINE__);
	strcpy(msg.text,"quit");
	send(acceptfd,&msg,sizeof(msg),0);
	close(acceptfd);
}

int main(int argc, const char *argv[])
{
	//socket->填充->绑定->监听->等待连接->数据交互->关闭 
	int sockfd;
	int acceptfd;	
	struct msg msg;
	ssize_t recvbytes;
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

	//绑定网络套接字和网络结构体
	if(bind(sockfd, (const struct sockaddr *)&serveraddr,addrlen) == -1){
		printf("bind failed.\n");
		exit(-1);
	}

	//监听套接字，将主动套接字转化为被动套接字
	if(listen(sockfd,10) == -1){
		printf("listen failed.\n");
		exit(-1);
	}

	char *errmsg;
	sqlite3 *db;

	if(sqlite3_open(DATABASE,&db) != SQLITE_OK){
		printf("%s.\n",sqlite3_errmsg(db));
	}else{
		printf("the database open success.\n");
	}

	if(sqlite3_exec(db,"create table usrinfo(jnumber text primary key,name text,edge text,sex text,department text,passwd text,phone text, flags text);",NULL,NULL,&errmsg)!= SQLITE_OK){
		printf("%s.\n",errmsg);
	}else{
		sqlite3_exec(db,"insert into usrinfo values('01','root','20','man','R&D','1','110', '1');",NULL,NULL,&errmsg);
		printf("create usrinfo table success.\n");
	}

	if(sqlite3_exec(db,"create table historyinfo(time text,name text,events text);",NULL,NULL,&errmsg)!= SQLITE_OK){
		printf("%s.\n",errmsg);
	}else{
		printf("create historyinfo table success.\n");
	}

	pid_t pid;

	while(1){
		//数据交互 
		acceptfd = accept(sockfd,(struct sockaddr *)&clientaddr,&cli_len);
		if(acceptfd == -1){
			printf("acceptfd failed.\n");
			exit(-1);
		}
		pid = fork();
		if(pid < 0){
			handle_error("fork failed.\n");
		}
		if(pid == 0){
			while(1){
				if(recv(acceptfd,&msg,sizeof(msg),0) == 0){
					goto end;
				}
				//printf("msg.type :%d.\n",msg.type);
				switch(msg.type)
				{
					case L://登录请求
					process_login_request(acceptfd,msg,db);
					break;
					case QN://普通用户查询请求
					process_query_normal_request(acceptfd,msg,db);
					break;
					case MN://普通用户修改请求
					process_modify_normal_request(acceptfd,msg,db);
					break;
					case QR://root用户查询请求
					process_query_root_request(acceptfd,msg,db);
					break;
					case MR://root用户修改请求
					process_modify_root_request(acceptfd,msg,db);
					break;
					case AR://root用户添加请求
					process_add_root_request(acceptfd,msg,db);
					break;
					case DR://root用户删除请求
					process_delete_root_request(acceptfd,msg,db);
					break;
					case HR://root用户查询历史请求
					process_query_history_root_request(acceptfd,msg,db);
					break;
					case T://退出请求
					process_quit_request(acceptfd,msg,db);
					break;
				}
			}
		}
		if(pid > 0){
			printf("client ip: %s.\n",inet_ntoa(clientaddr.sin_addr));
			close(acceptfd);
		}
	}
	end:
	close(sockfd);

	return 0;
}