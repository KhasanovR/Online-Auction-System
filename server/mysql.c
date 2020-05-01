#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

const char *server = "localhost";
const char *user = "os";
const char *psswd = "osisthebest"; /* set connection vriables */
const char *database = "auction";

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;

static long UnixTimeFromMysqlString(char *s)

{

	struct tm tmlol;
	strptime(s, "%Y-%m-%d %H:%M:%S", &tmlol);

	time_t t = mktime(&tmlol);
	return t;
}

typedef struct 
{
	int lot_id;
    float min_price;
    float winning_bid;
    int winning_bidder;
    char title[255];
    char description[255];
    int owner_id;
    time_t start_time;
	time_t stop_time;
} lot;

typedef struct 
{
	int command;
    int  client_id;
    char username[30];
    char password[64];
    char visa_card_number[19];
    int bid_id;
    int bid_lot_id;
    float bid_amount;
    int bidder_client_id;
    lot lt[5];
}General;


void connect_to_database()
{
	conn = mysql_init(NULL);
	/* Connect to database */
	if (!mysql_real_connect(conn, server, user, psswd, database, 0, NULL, 0)) 
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}else{
		printf("Connected to database ................\n");
	}

	
}

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  // exit(1);        
}


void close_connection()   /* close connection */
{
	printf("Closing to database ................\n");
	mysql_close(conn);
}


int get_user_id(char username[30])/*Gets user's id*/
{	
	connect_to_database();
	char query[1024];
  	sprintf(query,"SELECT id FROM client WHERE username=\'%s\'",username);
	int id = 0;
	if(mysql_query(conn,query))
	{
      	finish_with_error(conn);
      	return id;
	}
	else{
		res=mysql_store_result(conn);

		if((row = mysql_fetch_row(res))!=NULL )
		{ 	
			id = atoi(row[0]);
		}
		mysql_free_result(res);
	}	
	close_connection();	
	return id;	
}


General login_user(char username[30],char password[64])/*chech user login*/
{	
	connect_to_database();
	char query[1024];
	General ct;
	ct.command = 0;
	strcpy(ct.username,"-");
	strcpy(ct.password,"-");
	int id;
  	sprintf(query,"SELECT * FROM client WHERE username=\'%s\' and password=\'%s\'",username,password);
	if (mysql_query(conn,query))
	{ 
    	finish_with_error(conn);
    	return ct;
  	}else{
		res=mysql_store_result(conn);
		row=mysql_fetch_row(res);
		if(res != NULL && row){
			id = atoi(row[0]);
			ct.client_id = id;
			strcpy(ct.username,row[1]);
			strcpy(ct.password,row[2]);
			strcpy(ct.visa_card_number,row[3]);
			if(id != 0)
				ct.command = 1;
		}
		mysql_free_result(res);	
	}
	close_connection();
	return ct;	
}

int delete_user(char username[30])/*delete user*/
{	
	connect_to_database();
	char query[1024];
  	sprintf(query,"UPDATE client SET username=concat(\'%s\','deleted'), password=concat('deleted',MD5(curtime())), visa_card_num=NULL WHERE username='%s'",username,username);
	if (mysql_query(conn,query))
	{ 
      finish_with_error(conn);
	  return 0;	
	}else
	  {	
		  mysql_free_result(res);	
		  close_connection();
		  return 1;
	  }
}

int check_visa_card(char visa_card_num[19])
{
  connect_to_database();
  if (mysql_query(conn, "select visa_card_num from visa_card ")) 
    {
      finish_with_error(conn);
      return 0;
    }
    else{
	  res = mysql_store_result(conn);
	  
	  if (res == NULL) 
	  {
	    finish_with_error(conn);
	  	return 0;
	  }  
	  
	  while ((row = mysql_fetch_row(res))) 
	  { 
	    if(!strcmp(row[0],visa_card_num))
	    {    
	      mysql_free_result(res);  
	      close_connection();
	      return 1;
	    }
	  }
	  
	  mysql_free_result(res);  
	}
  close_connection();
  return 0;
}

General register_user(char username[30],char password[64], char visa_card_number[19]) /*register  user */
{	

	int id = get_user_id(username);
	General ct;
	ct.command = 1;
	if(!check_visa_card(visa_card_number)){
		ct.command = 0;
		return ct;
	}
	if(id==0)
	{
		connect_to_database();
		char query[1024];
		sprintf(query,"INSERT INTO client (username, password, visa_card_num) VALUES( \'%s\',\'%s\',\'%s\')",username,password,visa_card_number);
		if (mysql_query(conn,query)) 
		{
			finish_with_error(conn);
			return ct;
		}else{
			printf("%s\n\n","user created"); 	
			}
		close_connection();
		ct.client_id = get_user_id(username);
		strcpy(ct.username,username);
		strcpy(ct.password,password);
		strcpy(ct.visa_card_number,visa_card_number);
	}
	else
		ct.command = 0;
	return ct;
}

General update_user(int id, char username[30],char password[64], char visa_card_number[19])/* update user*/
{
	General ct;
	ct.command = 0;
	connect_to_database();
	char query[1024];
  	sprintf(query,"UPDATE client SET username=\'%s\', password=\'%s\', visa_card_num=\'%s\' where id=%d",username,password,visa_card_number,id);
	// printf("%s\n\n", query);
	if (mysql_query(conn,query)) 
	{
      finish_with_error(conn);
      return ct;
  	}else{
  		ct.command = 1;
			printf("%s\n\n","user updated"); 
			General ct;
			ct.client_id = id;
			strcpy(ct.username,username);
			strcpy(ct.password,password);
			strcpy(ct.visa_card_number,visa_card_number);
	  }
	close_connection();
	return ct;
}

time_t convert(char *str)
{
    struct tm tm;
    strptime(str, "%Y-%m-%d %H:%M:%S", &tm);
    time_t t = mktime(&tm);  // t is now your desired time_t
	tm = *localtime(&t);
    // printf("now: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	return t;
}


time_t get_last_time()
{	
	connect_to_database();
	time_t t;
	char *str;
	if (mysql_query(conn, "SELECT max(stop_time) FROM lot")) 
	{
	 	finish_with_error(conn);
		return time(NULL);
	}else{
	res = mysql_store_result(conn);
	if( (row = mysql_fetch_row(res)) != NULL )
	{	
		str = row[0];
		t = convert(str);
		time_t now = time(NULL);
		double seconds = difftime(t, now);
  		if (seconds < 0) {
			  t = now;
		}
	}
	else
	{
		t = time(NULL);
	struct tm tm = *localtime(&t);
	t = mktime(&tm);
	mysql_free_result(res);
	}
	}	
	close_connection();
	return t;
}

void update_lot_time()/*updates lotes which does not used*/
{
	connect_to_database();
	char query1[1024] = "UPDATE lot SET start_time = DATE_ADD(curtime(), INTERVAL 5 MINUTE),stop_time=DATE_ADD(start_time, INTERVAL 5 MINUTE) WHERE stop_time < curtime() and winning_bid is NULL and winning_bidder is NULL; ";
	
	if (mysql_query(conn,query1)) 
	{
      finish_with_error(conn);
      return;
  	}else{
		printf("%s\n\n","lot_updated");   
	  }

	close_connection();

	connect_to_database();
	char query2[1024] = "UPDATE lot, (SELECT MAX(stop_time) as max_time from lot WHERE winning_bid is NULL and winning_bidder is NULL) as tb SET start_time = max_time, stop_time=DATE_ADD(start_time, INTERVAL 5 MINUTE) WHERE stop_time < curtime() and  max_time > curtime() and winning_bid is NULL and winning_bidder is NULL;";
	
	if (mysql_query(conn,query2)) 
	{
      finish_with_error(conn);
      return;
  	}else{
		printf("%s\n\n","lot_updated");   
		// mysql_free_result(res);	
	  }
	close_connection();
}

int create_lot(float min_price, char title[255], char description[255], int owner_id) /*create  lot */
{
	
	time_t t = get_last_time();
	connect_to_database();
	struct tm tm = *localtime(&t);
	char start_time[20];
	sprintf(start_time, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d", tm.tm_year+1900,tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	char query[1024];
  	sprintf(query,"INSERT INTO lot (min_price,title,description,owner_id,start_time ,stop_time) VALUES( %2f,\'%s\',\'%s\',%d,\'%s\', date_add(\'%s\',INTERVAL 5 MINUTE))",min_price,title,description,owner_id,start_time,start_time);
	if (mysql_query(conn,query)) 
	{
      	finish_with_error(conn);
      	return 0;
  	}else{
		printf("%s\n\n","lot_created"); 
	}
	close_connection();
	return 1;
}


int  create_bid(int lot_id,float bid_amount, int bidder_client_id) /*create  bid */
{	
	char query[1024];
	sprintf(query,"INSERT INTO bid(lot_id, bid_amount, bidder_client_id) SELECT %d, %.2f, %d FROM lot WHERE id = %d and %.2f > min_price and %.2f > winning_bid and (SELECT curtime()) >= start_time and (SELECT curtime()) <= stop_time",lot_id, bid_amount, bidder_client_id,lot_id, bid_amount, bid_amount);	
	printf("%s\n\n", query);
	connect_to_database();
	if (mysql_query(conn,query)) 
	{
      finish_with_error(conn);
      return 0;
  	}else{
		printf("%s\n\n","bid_created"); 
		char query_lot[1024];
  		sprintf(query_lot,"UPDATE lot SET winning_bid=%.2f, winning_bidder=%d WHERE id=%d",bid_amount,bidder_client_id, lot_id);
			// if(mysql_store_result(conn))
				if (mysql_query(conn,query_lot))
				{
		 		 finish_with_error(conn);
				}else{
				  printf("%s\n\n","bid_created"); 
		 		}
	  }
		close_connection();
		return 1;
}

General available_lots()
{
	update_lot_time();
    General lt;
    connect_to_database();
    lt.command = 0;
    if (mysql_query(conn, "SELECT * FROM lot WHERE start_time <= curtime() and stop_time >= curtime() order by start_time LIMIT 10")) 
  	{

      finish_with_error(conn);
      return lt;
  	}
	else{
		res = mysql_store_result(conn);
  
		if (res == NULL) 
		{
			finish_with_error(conn);
			return lt;
		}
		int j = 0; 
	    while ((row = mysql_fetch_row(res)) ) 
		{ 
	        lt.lt[j].lot_id = atoi(row[0]);
	        lt.lt[j].min_price = atof(row[1]);
	        lt.lt[j].winning_bid = 0;
	        lt.lt[j].winning_bidder = 0;
	        strcpy(lt.lt[j].title , (char *)row[4]);
	        strcpy(lt.lt[j].description , (char *)row[5]);
	        lt.lt[j].owner_id = atoi(row[6]);
	        lt.lt[j].start_time = UnixTimeFromMysqlString((char *)row[7]);
			lt.lt[j].stop_time = UnixTimeFromMysqlString((char *)row[8]);
	        j++;
	        if(j == 5)
	        	break;

		}
		mysql_free_result(res);
	}
    close_connection();
    lt.command = 1;
	return lt;
}

void transaction()
{  
  connect_to_database();
  if (mysql_query(conn,"UPDATE visa_card SET balance=balance+0.97*(Select winning_bid from lot where winning_bid is not NULL and winning_bidder is not NULL and transaction_done=0 and stop_time<=curtime())  where visa_card_num=(select visa_card_num from client where client.id =(Select  owner_id from lot where winning_bid is not NULL and winning_bidder is not NULL and stop_time<=curtime()));"))
  { 
      finish_with_error(conn);
      return;
  }
  else{
      close_connection();
      connect_to_database();
      if (mysql_query(conn,"UPDATE visa_card SET balance=balance-(Select winning_bid from lot where winning_bid is not NULL and winning_bidder is not NULL and transaction_done=0 and stop_time<=curtime()) where visa_card_num=(select visa_card_num from client where client.id =(Select winning_bidder from lot where winning_bid is not NULL and winning_bidder is not NULL  and stop_time<=curtime()));"))
      { 
	    finish_with_error(conn);
	    return;
      }
      else{
	    	close_connection();

		    connect_to_database();
		    if (mysql_query(conn,"UPDATE lot set transaction_done=1 where winning_bid is not NULL and winning_bidder is not NULL and transaction_done=0 and stop_time<=curtime();"))
		    { 
		        finish_with_error(conn);
		        return;
		    }  
	   		close_connection();
      	}
  	}
}