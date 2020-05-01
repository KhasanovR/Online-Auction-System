#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <openssl/sha.h>

unsigned char hash[SHA_DIGEST_LENGTH];
char hashed[SHA_DIGEST_LENGTH*2];

int sockfd;
char buffer[1000];
char server_reply[2000];
ssize_t n;

struct sockaddr_in servaddr;

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

typedef struct 
{
    char title[255];
    float minPrice;
    char description[255];
}lothome;

lothome lth;
static General g,g1,g2;

//BUILDER
GtkBuilder      *builder;
//BOX
GtkWidget       *lines[5];
//EDITORS
GtkEditable     *Name_Profile;
GtkEditable     *Password_Profile;
GtkEditable     *CardID_Profile;
//WINDOWS
GtkWidget       *wmenu;
GtkWidget       *wabout;
GtkWidget       *wauthorisation;
GtkWidget       *wregistration;
GtkWidget       *wprofile;
GtkWidget       *waddinglot;
GtkWidget       *wconfirmation;
GtkWidget       *wavailable;
GtkWidget       *wauction;
GtkWidget       *wdescription;
GtkWidget       *wtermsconditions;
//LABELS
GtkWidget       *confirmPasswordLabel;
GtkWidget       *lotTitle[5];
GtkWidget       *lotMinPrice[5];
GtkWidget       *lottime[5];
GtkWidget       *labelDescription;
GtkWidget       *titleLabel_Description;
GtkWidget       *startTimeLabel_Description;
GtkWidget       *endTimeLabel_Description;
GtkWidget       *minPriceLabel_Description;

//BUTTONS
GtkWidget       *backButton_Profile;
GtkWidget       *joinButton_Description;
GtkWidget       *editButton_Profile;
GtkWidget       *deleteAccountButton_Profile;
GtkWidget       *toggleButton_Confirmation;
GtkWidget       *confirmButton_Confirmation;
GtkWidget       *confirmButton_TermsCondition;
GtkWidget       *toggleButton_TermsCondition;
//ENTRIES
GtkWidget       *entryPrice_Auction;

GtkWidget       *entryName_Profile;
GtkWidget       *entryPassword_Profile;
GtkWidget       *entryConfirmPassword_Profile;
GtkWidget       *entryCardID_Profile;

GtkWidget       *entryName_Login;
GtkWidget       *entryPassword_Login;

GtkWidget       *entryTitle_AddingLot;
GtkWidget       *entryMinPrice_AddingLot;
GtkWidget       *entryDescription_AddingLot;

GtkWidget       *entryName_Registration;
GtkWidget       *entryPassword_Registration;
GtkWidget       *entryConfirmPassword_Registration;
GtkWidget       *entryCardID_Registration;
//CSS
GtkCssProvider  *provider;
GdkScreen       *screen;
GdkDisplay      *display;


GtkWidget       *lbl_time;

enum{
    NAME,
    PRICE
};

struct MainWindowObjects
{
    GtkWindow       *topWindow;
    GtkTreeView     *treeviewNodes;
    GtkListStore    *liststoreNodes;

} MainWindowObjects;
char passw[64];

typedef struct {
    GtkWidget *lbl_time;
} app_widgets;
 
gboolean timer_handler(app_widgets *widgets);

struct tm tm1;
struct tm tm2;
struct tm tm3;

time_t t1;
time_t t2;
time_t mytime;

int indx = 0;

pthread_t tid[2];

void* SendAndReciev(void *arg);


int main(int argc, char *argv[])
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        perror("Could not create socket\n");
    }
    puts("Socket Created");
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8888);
    servaddr.sin_addr.s_addr = inet_addr("192.168.43.183");

    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    gtk_init(&argc, &argv);
 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/menu.glade", NULL);

    //WINDOWS
    wmenu = GTK_WIDGET(gtk_builder_get_object(builder, "MENU"));
    wabout = GTK_WIDGET(gtk_builder_get_object(builder, "AboutWindow"));
    wauthorisation = GTK_WIDGET(gtk_builder_get_object(builder, "AuthorisationWindow"));
    wregistration = GTK_WIDGET(gtk_builder_get_object(builder, "RegistrationWindow"));
    wprofile = GTK_WIDGET(gtk_builder_get_object(builder, "ProfileWindow"));
    waddinglot = GTK_WIDGET(gtk_builder_get_object(builder, "AddingLotWindow"));
    wconfirmation = GTK_WIDGET(gtk_builder_get_object(builder, "ConfirmationWindow"));
    wavailable = GTK_WIDGET(gtk_builder_get_object(builder, "AvailableLots"));
    wauction = GTK_WIDGET(gtk_builder_get_object(builder, "AuctionWindow"));
    wdescription = GTK_WIDGET(gtk_builder_get_object(builder, "DescriptionWindow"));
    wtermsconditions = GTK_WIDGET(gtk_builder_get_object(builder, "TermsAndConditionsWindow"));
    //BOX
    lines[0] = GTK_WIDGET(gtk_builder_get_object(builder,"line0"));
    lines[1] = GTK_WIDGET(gtk_builder_get_object(builder,"line1"));
    lines[2] = GTK_WIDGET(gtk_builder_get_object(builder,"line2"));
    lines[3] = GTK_WIDGET(gtk_builder_get_object(builder,"line3"));
    lines[4]= GTK_WIDGET(gtk_builder_get_object(builder,"line4"));
    //LABELS
    confirmPasswordLabel = GTK_WIDGET(gtk_builder_get_object(builder, "confirmPasswordLabel_Profile"));
    labelDescription = GTK_WIDGET(gtk_builder_get_object(builder,"labelDescription"));
    lotTitle[0] = GTK_WIDGET(gtk_builder_get_object(builder,"lot0tittle"));
    lotTitle[1] = GTK_WIDGET(gtk_builder_get_object(builder,"lot1tittle"));
    lotTitle[2] = GTK_WIDGET(gtk_builder_get_object(builder,"lot2tittle"));
    lotTitle[3] = GTK_WIDGET(gtk_builder_get_object(builder,"lot3tittle"));
    lotTitle[4] = GTK_WIDGET(gtk_builder_get_object(builder,"lot4tittle"));

    lotMinPrice[0] = GTK_WIDGET(gtk_builder_get_object(builder,"lot0MinPrice"));
    lotMinPrice[1] = GTK_WIDGET(gtk_builder_get_object(builder,"lot1MinPrice"));
    lotMinPrice[2] = GTK_WIDGET(gtk_builder_get_object(builder,"lot2MinPrice"));
    lotMinPrice[3] = GTK_WIDGET(gtk_builder_get_object(builder,"lot3MinPrice"));
    lotMinPrice[4] = GTK_WIDGET(gtk_builder_get_object(builder,"lot4MinPrice"));

    lottime[0] = GTK_WIDGET(gtk_builder_get_object(builder,"lot0time"));
    lottime[1] = GTK_WIDGET(gtk_builder_get_object(builder,"lot1time"));
    lottime[2] = GTK_WIDGET(gtk_builder_get_object(builder,"lot2time"));
    lottime[3] = GTK_WIDGET(gtk_builder_get_object(builder,"lot3time"));
    lottime[4] = GTK_WIDGET(gtk_builder_get_object(builder,"lot4time"));

    titleLabel_Description = GTK_WIDGET(gtk_builder_get_object(builder,"titleLabel_Description"));
    startTimeLabel_Description = GTK_WIDGET(gtk_builder_get_object(builder,"startTime_Description"));
    endTimeLabel_Description = GTK_WIDGET(gtk_builder_get_object(builder,"endTime_Description"));
    minPriceLabel_Description = GTK_WIDGET(gtk_builder_get_object(builder,"minPrice_Description"));
    //BUTTONS
    backButton_Profile = GTK_WIDGET(gtk_builder_get_object(builder, "backToMenu_Profile"));
    editButton_Profile = GTK_WIDGET(gtk_builder_get_object(builder, "editButton_Profile"));
    deleteAccountButton_Profile = GTK_WIDGET(gtk_builder_get_object(builder, "deleteAccountButton_Profile"));
    toggleButton_Confirmation = GTK_WIDGET(gtk_builder_get_object(builder, "toggleButton_Confirmation"));
    confirmButton_Confirmation = GTK_WIDGET(gtk_builder_get_object(builder, "confirmButton_Confirmation"));
    confirmButton_TermsCondition = GTK_WIDGET(gtk_builder_get_object(builder, "confirmButton_TermsCondition"));
    toggleButton_TermsCondition = GTK_WIDGET(gtk_builder_get_object(builder, "toggleButton_TermsCondition"));
    joinButton_Description = GTK_WIDGET(gtk_builder_get_object(builder, "joinToAuction_Description"));
    //ENTRIES
    entryPrice_Auction = GTK_WIDGET(gtk_builder_get_object(builder, "priceEntry_Auction"));

    entryName_Login = GTK_WIDGET(gtk_builder_get_object(builder, "NameTextBox_Login"));
    entryPassword_Login = GTK_WIDGET(gtk_builder_get_object(builder, "PasswordTextBox_Login"));
    
    entryName_Registration = GTK_WIDGET(gtk_builder_get_object(builder, "NameTextBox_Register"));
    entryPassword_Registration = GTK_WIDGET(gtk_builder_get_object(builder, "PasswordTextBox_Register"));
    entryConfirmPassword_Registration = GTK_WIDGET(gtk_builder_get_object(builder, "ConfirmPasswordTextBox_Register"));
    entryCardID_Registration = GTK_WIDGET(gtk_builder_get_object(builder, "CardIDTextBox_Register"));

    entryName_Profile = GTK_WIDGET(gtk_builder_get_object(builder, "NameTextBox_Profile"));
    entryPassword_Profile = GTK_WIDGET(gtk_builder_get_object(builder, "PasswordTextBox_Profile"));
    entryCardID_Profile = GTK_WIDGET(gtk_builder_get_object(builder, "CardIDTextBox_Profile"));
    entryConfirmPassword_Profile = GTK_WIDGET(gtk_builder_get_object(builder, "ConfirmPasswordTextBox_Profile"));

    entryTitle_AddingLot = GTK_WIDGET(gtk_builder_get_object(builder, "TittleTextBox_AddingSlot"));
    entryMinPrice_AddingLot = GTK_WIDGET(gtk_builder_get_object(builder, "MinPriceTextBox_AddingSlot"));
    entryDescription_AddingLot = GTK_WIDGET(gtk_builder_get_object(builder, "DescriptionTextBox_AddingSlot"));    
    //EDITORS
    Name_Profile = GTK_WIDGET(gtk_builder_get_object(builder, "NameTextBox_Profile"));
    Password_Profile = GTK_WIDGET(gtk_builder_get_object(builder, "PasswordTextBox_Profile"));
    CardID_Profile = GTK_WIDGET(gtk_builder_get_object(builder, "CardIDTextBox_Profile"));

    MainWindowObjects.topWindow=GTK_WINDOW(gtk_builder_get_object(builder, "AuctionWindow"));
    MainWindowObjects.treeviewNodes= GTK_TREE_VIEW( gtk_builder_get_object( builder, "treeview" ) );
    MainWindowObjects.liststoreNodes= GTK_LIST_STORE( gtk_builder_get_object(builder, "list_store") );

    GtkTreeSelection *selection;
    selection = gtk_tree_view_get_selection( GTK_TREE_VIEW(MainWindowObjects.treeviewNodes) );
    gtk_tree_selection_set_mode( selection, GTK_SELECTION_MULTIPLE );

    gtk_builder_connect_signals(builder, NULL);


    //stlyesheet connecting part
    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen (display);
    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(provider),"style.css",NULL);
 

    lbl_time = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_time"));

    g_object_unref(builder);
 
    gtk_widget_show(wauthorisation);                
    gtk_main();
 
    return 0;
}
//DESCRIPTION
void on_backToAvailable_Description_clicked()
{
    gtk_widget_hide(wdescription);
    gtk_widget_show(wavailable);
}

void on_joinToAuction_Description_clicked()
{
    gtk_widget_hide(wdescription);
    gtk_widget_show(wauction);

    t1 = g2.lt[indx].stop_time;
    tm1 = *localtime(&t1);

    g_timeout_add_seconds(1, (GSourceFunc)timer_handler, lbl_time);
}

//AUCTION
void on_leaveButton_Auction_clicked()
{
    gtk_widget_hide(wauction);
    gtk_widget_show(wavailable);
}

float last = 0;
void on_bidButton_Auction_clicked()
{
    // gtk_list_store_clear (GTK_LIST_STORE(MainWindowObjects.liststoreNodes));
    g1.command = 4;
    g1.bid_lot_id = g2.lt[indx].lot_id;
    int check = 1;
    const gchar *price = gtk_entry_get_text(entryPrice_Auction);
    for(int i = 0; i <strlen(price);i++){
        if((price[i] < '0' || price[i] > '9') && price[i]!='.')
        {
            check = 0;
            break;
        }
    }
    g1.bid_amount = atof(price);
    g1.bidder_client_id = g1.client_id;
    printf("%d %f %d\n",g1.bid_lot_id, g1.bid_amount, g1.bidder_client_id);
    int err;
    printf("%f - %f - %f\n", g1.bid_amount, g2.lt[indx].min_price, g2.lt[indx].winning_bid);
    if(check &&  g1.bid_amount > g2.lt[indx].min_price &&  g1.bid_amount > last)
    {   
        last = g1.bid_amount;
        printf("%f\n", last);
        for (int i = 0; i < 2; ++i)
        {
         
            err = pthread_create(&(tid[i]), NULL, &SendAndReciev, NULL );
            if (err != 0)
                printf("\ncan't create thread :[%s]", strerror(err));
            // elses
                // printf("\n Thread created successfully\n");
        }
        if(g2.command  != 0){
            GtkTreeIter iter;
            gtk_list_store_append(GTK_LIST_STORE(MainWindowObjects.liststoreNodes), &iter);
            gtk_list_store_set(GTK_LIST_STORE(MainWindowObjects.liststoreNodes), &iter, NAME,g1.client_id, PRICE, gtk_entry_get_text(entryPrice_Auction),-1);
            gtk_entry_set_text(entryPrice_Auction,"");
        }
    }
   
}

//AVAILABLE
_Bool check_time(int i)
{
    time_t time_check, cur_time;
    double diff_time;
    time_check = g2.lt[i].start_time;
    cur_time = time(NULL);

    diff_time = difftime(time_check, cur_time);

    if(diff_time <= 0)
    {
        return TRUE;
    }

    return FALSE;
}

void description_fill(int i)
{   
    indx = i;
    g1.bidder_client_id = g1.client_id;
    // last = atof(gtk_label_get_text(lotMinPrice[i]));
    gtk_label_set_text(GTK_LABEL(minPriceLabel_Description), gtk_label_get_text(lotMinPrice[i]));    
    gtk_label_set_text(GTK_LABEL(startTimeLabel_Description), ctime(&g2.lt[i].start_time));
    gtk_label_set_text(GTK_LABEL(endTimeLabel_Description), ctime(&g2.lt[i].stop_time)); 

}
void on_more1_clicked()
{
    g1.bidder_client_id = g1.client_id;
    gtk_widget_hide(wavailable);
    gtk_widget_show(wdescription);
    description_fill(0);
    gtk_widget_set_sensitive(joinButton_Description, check_time(0));
    gtk_list_store_clear(MainWindowObjects.liststoreNodes);
}
void on_more2_clicked()
{
    g1.bidder_client_id = g1.client_id;
    gtk_widget_hide(wavailable);
    gtk_widget_show(wdescription);
    description_fill(1);
    gtk_widget_set_sensitive(joinButton_Description,  check_time(1));
    gtk_list_store_clear(MainWindowObjects.liststoreNodes);

}
void on_more3_clicked()
{
    g1.bidder_client_id = g1.client_id;
    gtk_widget_hide(wavailable);
    gtk_widget_show(wdescription);
    description_fill(2);
    gtk_widget_set_sensitive(joinButton_Description,  check_time(2));
    gtk_list_store_clear(MainWindowObjects.liststoreNodes);

}
void on_more4_clicked()
{
    g1.bidder_client_id = g1.client_id;
    gtk_widget_hide(wavailable);
    gtk_widget_show(wdescription);
    description_fill(3);
    gtk_widget_set_sensitive(joinButton_Description,  check_time(3));
    gtk_list_store_clear(MainWindowObjects.liststoreNodes);

}
void on_more5_clicked()
{
    g1.bidder_client_id = g1.client_id;
    gtk_widget_hide(wavailable);
    gtk_widget_show(wdescription);
    description_fill(4);
    gtk_widget_set_sensitive(joinButton_Description, check_time(4));
    gtk_list_store_clear(MainWindowObjects.liststoreNodes);

}
void on_backToMenu_AvailableLots_clicked()
{
    gtk_widget_hide(wavailable);
    gtk_widget_show(wmenu);
}

void on_auctionButton_availableLots_clicked()
{
    gtk_widget_hide(wavailable);
    gtk_widget_show(wdescription);
}
//AUTHORISATION
void on_loginButton_Login_clicked()
{
   ;
    g1.command = 0;
    g2.command = 0;
    strcpy(g1.username,"-");
    strcpy(g1.password,"-");

    const gchar *name = gtk_entry_get_text (entryName_Login);
    const gchar *password = gtk_entry_get_text (entryPassword_Login);
    // passw = password;
    strcpy(passw, password);
    //CHECKING FOR NAME AND PASSWORD!!!
  
    g1.command = 2;
    strcpy(g1.username,name);
    memset(hashed, 0x0, SHA_DIGEST_LENGTH*2);
    memset(hash, 0x0, SHA_DIGEST_LENGTH);
    SHA1((unsigned char *)password, strlen(password), hash);
 
    for (int i=0; i < SHA_DIGEST_LENGTH; i++) {
        sprintf((char*)&(hashed[i*2]), "%02x", hash[i]);
    }
    strcpy(g1.password, hashed);
    // for(int i =0; i<SHA_DIGEST_LENGTH; i++)
    //     printf("%02x", g1.password[i]);
    // printf("\n");
    for(;;){
        if(send(sockfd, (General *)&g1, sizeof(General),0)<0)
        {
            printf("Error\n");
            return 1;
        }
        if(recv(sockfd,(General *)&g2, sizeof(General),0)<0)
        {
            puts("Error\n");
            // break;
        }
        printf("%d\n",g2.command);
        if(g2.command  != 0){
            g1 = g2;
            gtk_entry_set_text(entryName_Login,"");
            gtk_entry_set_text(entryPassword_Login,"");
            gtk_widget_hide(wauthorisation);
            gtk_widget_show(wmenu);
            break;
        }
        else if(g2.command == 0)
            break;
        
 
    }

}

void on_registrationButton_Login_clicked()
{
    gtk_widget_hide(wauthorisation);
    gtk_widget_show(wtermsconditions); 
}
//TERMS AND CONDITIONS
void on_toggleButton_TermsCondition_toggled()
{
        gtk_widget_set_sensitive(confirmButton_TermsCondition, gtk_toggle_button_get_active (toggleButton_TermsCondition));
}
void on_confirmButton_TermsCondition_clicked()
{
    gtk_toggle_button_set_mode (toggleButton_TermsCondition,FALSE);
    gtk_widget_hide(wtermsconditions);
    gtk_widget_show(wregistration);
}
void on_declineButton_TermsCondition_clicked()
{
    gtk_widget_hide(wtermsconditions);
    gtk_widget_show(wauthorisation);
}
//REGISTRATION
void on_registrationButton_clicked()
{
    const gchar *name = gtk_entry_get_text (entryName_Registration);
    const gchar *password = gtk_entry_get_text (entryPassword_Registration);
    const gchar *confirmPassword = gtk_entry_get_text (entryConfirmPassword_Registration);
    const gchar *cardID = gtk_entry_get_text (entryCardID_Registration);
    strcpy(passw, password);
    // printf("%s\n",client.name);


    if(strcmp(password,confirmPassword)==0 && strcmp(password,"")!=0)
    {
    
        g1.command = 1;
        strcpy(g1.username,name);
        memset(hashed, 0x0, SHA_DIGEST_LENGTH*2);
        memset(hash, 0x0, SHA_DIGEST_LENGTH);
        SHA1((unsigned char *)password, strlen(password), hash);
     
        for (int i=0; i < SHA_DIGEST_LENGTH; i++) {
            sprintf((char*)&(hashed[i*2]), "%02x", hash[i]);
        }
        strcpy(g1.password, hashed);
        strcpy(g1.visa_card_number, cardID);

    for(;;){
                if(send(sockfd, (General *)&g1, sizeof(General),0)<0)
                {
                    printf("Error\n");
                    return 1;
                }
                if(recv(sockfd,(General *)&g2, sizeof(General),0)<0)
                {
                    puts("Error");
                    // break;
                }
                // printf("%d\n",sizeof(GtkWidget));
                if(g2.command  != 0){
                    g1 = g2;
                    gtk_entry_set_text(entryName_Registration,"");
                    gtk_entry_set_text(entryPassword_Registration,"");
                    gtk_entry_set_text(entryConfirmPassword_Registration,"");
                    gtk_entry_set_text(entryCardID_Registration,"");
                    gtk_widget_hide(wregistration);
                    gtk_widget_show(wmenu);
                }
                else if(g2.command == 0)
                    break;
            }        
        
    }
    else
    {
        
    }    
}
//MENU 
void on_logoutButton_clicked()
{
    //LOGING OUT
    close(sockfd);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    gtk_widget_hide(wmenu);
    gtk_widget_show(wauthorisation);
}

void on_availableLotsButton_clicked()
{
    gtk_widget_hide(wmenu);
    gtk_widget_show(wavailable); 
    g1.command = 3;
    // memset(&g2, 0, sizeof(General));
    
    for(;;)
    {   
            if(send(sockfd, (General *)&g1, sizeof(General),0)<0)
            {
                printf("Error\n");
                return 1;
            }
            if(recv(sockfd,(General *)&g2, sizeof(General),0)<0)
            {
                puts("Error");
            }
            printf("%d\n",g2.command);
            if(g2.command  != 0)
            {

               for (int i = 0; i<5 && g2.lt[i].min_price != 0; ++i)
                {
                    memcpy(g1.lt,g2.lt,sizeof(g2.lt));
                   gtk_widget_show(lines[i]);
                    printf("%s\n", g2.lt[i].title);
                    gtk_label_set_text(GTK_LABEL(lotTitle[i]),g2.lt[i].title);
                    printf("%f\n", g2.lt[i].min_price);
                    char* str = &g2.lt[i].min_price;
                    sprintf(str, "%g", (g2.lt[i].min_price));
                    gtk_label_set_text(lotMinPrice[i],str);
                    struct tm *tm = localtime(&g2.lt[i].start_time);
                     tm->tm_year += 1900;
                       tm->tm_mon += 1;
                     sprintf(str,"%d-%d-%d %d:%d:%d\n",  tm->tm_year, tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min,tm->tm_sec);
                    gtk_label_set_text(lottime[i],ctime(&g2.lt[i].start_time));
                }   
                                
                break;
            }
            else if(g2.command == 0)
                break;
    } 


}
void on_addingLotButton_clicked()
{
    gtk_widget_hide(wmenu);
    gtk_widget_show(waddinglot);
}


gboolean timer_handler (app_widgets *widgets)
{
    mytime = time(NULL);
    tm2 = *localtime(&mytime);
    tm3 = *localtime(&mytime);

    tm3.tm_hour = tm1.tm_hour - tm2.tm_hour;
    tm3.tm_min = tm1.tm_min - tm2.tm_min;
    tm3.tm_sec = tm1.tm_sec - tm2.tm_sec;


    if(tm3.tm_sec < 0)
    {
        tm3.tm_sec += 60;
        tm3.tm_min--;
        if(tm3.tm_min < 0)
        {
            tm3.tm_min += 60;
            tm3.tm_hour--;
        }
    }

    mytime = mktime(&tm3);    

    char *time_str = ctime(&mytime);
    
    time_str[strlen(time_str)-5] = '\0';
    gtk_label_set_text(GTK_LABEL(lbl_time), time_str);
    
    return TRUE; 
}


void on_aboutButton_clicked()
{
    gtk_widget_hide(wmenu);
    gtk_widget_show(wabout);  
}

void on_profileButton_clicked()
{
    //printf("%s\n", g1.username);
    //printf("%i\n", gtk_entry_get_text("345"));
    gtk_entry_set_text(Name_Profile, g1.username);
    gtk_entry_set_text(entryPassword_Profile, passw);
    gtk_entry_set_text(entryCardID_Profile, g1.visa_card_number);

    


    gtk_widget_hide(wmenu);
    gtk_widget_show(wprofile);  
}

void on_exitButton_clicked()
{
    gtk_main_quit();
}
//ABOUT
void on_backToMenuAbout_clicked()
{
    gtk_widget_hide(wabout);
    gtk_widget_show(wmenu);
}
//ADDINGLOT
void on_AddingLotButton_clicked()
{   
    //GETTING LOT INFO
    const gchar *title = gtk_entry_get_text (entryTitle_AddingLot);
    const gchar *minPrice = gtk_entry_get_text (entryMinPrice_AddingLot);
    const gchar *description = gtk_entry_get_text (entryDescription_AddingLot);

    strcpy(lth.title,title);
    lth.minPrice = atof(minPrice);
    strcpy(lth.description,description);

    gtk_widget_hide(waddinglot);
    gtk_widget_show(wconfirmation);

}

void on_backToMenu_Adding_clicked()
{
    gtk_widget_hide(waddinglot);
    gtk_widget_show(wmenu);
}
//CONFIRMATION
void on_toggleButton_Confirmation_toggled()
{
        gtk_widget_set_sensitive(confirmButton_Confirmation, gtk_toggle_button_get_active (toggleButton_Confirmation));
}
void on_confirmButton_Confirmation_clicked()
{
    g1.command = 6;
    printf("%d\n", g1.client_id);
    strcpy(g1.lt[0].title,lth.title);
    g1.lt[0].min_price = lth.minPrice;
    strcpy(g1.lt[0].description,lth.description);
    g1.lt[0].owner_id = g1.client_id;
    printf("%s\n", lth.title);

            for(;;){
                        if(send(sockfd, (General *)&g1, sizeof(General),0)<0)
                        {
                            printf("Error\n");
                            return 1;
                        }
                        if(recv(sockfd,(General *)&g2, sizeof(General),0)<0)
                        {
                            puts("Error");
                            // break;
                        }
                        // printf("%d\n",sizeof(GtkWidget));
                        if(g2.command  != 0){
                            // g1 = g2;
                            
                            gtk_entry_set_text(entryTitle_AddingLot,"");
                            gtk_entry_set_text(entryMinPrice_AddingLot,"");
                            gtk_entry_set_text(entryDescription_AddingLot,"");
                            gtk_widget_hide(wconfirmation);
                            gtk_widget_show(waddinglot);
                            break;
                        }
                        else if(g2.command == 0)
                            break;
                    } 


}
void on_declineButton_Confirmation_clicked()
{
    gtk_widget_hide(wconfirmation);
    gtk_widget_show(waddinglot);
}
//PROFILE
void on_backToMenu_Profile_clicked()
{
    gtk_widget_hide(wprofile);
    gtk_widget_show(wmenu);
}

void on_deleteAccountButton_Profile_clicked()
{
        g1.command = 5;

            
            for(;;){
                        if(send(sockfd, (General *)&g1, sizeof(General),0)<0)
                        {
                            printf("Error\n");
                            return 1;
                        }
                        if(recv(sockfd,(General *)&g2, sizeof(General),0)<0)
                        {
                            puts("Error");
                            // break;
                        }
                        // printf("%d\n",sizeof(GtkWidget));
                        if(g2.command  != 0){
                            // g1 = g2;
                            memset(&g1,NULL, sizeof(General));
                            memset(&g2,NULL, sizeof(General));
                            gtk_entry_set_text(Name_Profile,"");
                            gtk_entry_set_text(entryPassword_Profile,"");
                            gtk_entry_set_text(entryConfirmPassword_Profile,"");
                            gtk_entry_set_text(entryCardID_Profile,"");
                            gtk_widget_hide(wprofile);
                            gtk_widget_show(wauthorisation);
                            break;
                        }
                        else if(g2.command == 0)
                            break;
                    } 

}

void on_editButton_Profile_clicked()
{
    const gchar *str = gtk_button_get_label(editButton_Profile);
    gchar *s = "EDIT";
    if(*s == *str)
    {  
        gtk_editable_set_editable (Name_Profile,TRUE);
        gtk_editable_set_editable (Password_Profile,TRUE);
        gtk_editable_set_editable (CardID_Profile,TRUE);
        
        gtk_widget_set_visible (confirmPasswordLabel, TRUE);
        gtk_widget_set_visible (entryConfirmPassword_Profile, TRUE);
        gtk_widget_set_sensitive (backButton_Profile, FALSE);
        gtk_button_set_label (editButton_Profile, "OK");        
    }
    else
    {
        //HERE I TAKE VALUES OF PROFILE
        const gchar *name = gtk_entry_get_text (Name_Profile);
        const gchar *password = gtk_entry_get_text (entryPassword_Profile);
        const gchar *confirmPassword = gtk_entry_get_text (entryConfirmPassword_Profile);
        const gchar *cardID = gtk_entry_get_text (entryCardID_Profile);

        if(strcmp(password,confirmPassword)==0)
        {
            g1.command = 7;
            strcpy(g1.username, name);
            memset(hashed, 0x0, SHA_DIGEST_LENGTH*2);
            memset(hash, 0x0, SHA_DIGEST_LENGTH);
            SHA1((unsigned char *)password, strlen(password), hash);
 
            for (int i=0; i < SHA_DIGEST_LENGTH; i++) {
                sprintf((char*)&(hashed[i*2]), "%02x", hash[i]);
            }
            strcpy(g1.password, hashed);
            strcpy(g1.visa_card_number, cardID);
            
            for(;;){
                        if(send(sockfd, (General *)&g1, sizeof(General),0)<0)
                        {
                            printf("Error\n");
                            return 1;
                        }
                        if(recv(sockfd,(General *)&g2, sizeof(General),0)<0)
                        {
                            puts("Error");
                            // break;
                        }
                        printf("%d\n",g2.command);
                        if(g2.command  != 0){
                            g1 = g2;
                            gtk_editable_set_editable (Name_Profile,FALSE);
                            gtk_editable_set_editable (Password_Profile,FALSE);
                            gtk_editable_set_editable (CardID_Profile,FALSE);
            
                            gtk_widget_set_visible (confirmPasswordLabel, FALSE);
                            gtk_widget_set_visible (entryConfirmPassword_Profile, FALSE);
                            gtk_entry_set_text (entryConfirmPassword_Profile,"");
                            gtk_widget_set_sensitive (backButton_Profile, TRUE);
                            gtk_button_set_label (editButton_Profile, "EDIT");
                            break;
                          
                        }
                        else if(g2.command == 0)
                            break;
                    } 

        }
       
    }
    
}
//DESTROYS
void on_RegistrationWindow_destroy()
{
    gtk_main_quit();
}
void on_ProfileWindow_destroy()
{
    gtk_main_quit();
}
void on_MENU_destroy()
{
    gtk_main_quit();
}
void on_ConfirmationWindow_destroy()
{
    gtk_main_quit();
}
void on_AvailableLots_destroy()
{
    gtk_main_quit();
}
void on_AuthorisationWindow_destroy()
{
    gtk_main_quit();
}
void on_AddingLotWindow_destroy()
{
    gtk_main_quit();
}
void on_AboutWindow_destroy()
{
    gtk_main_quit();
}
void on_AuctionWindow_destroy()
{
    gtk_main_quit();
}
void on_TermsAndConditionsWindow_destroy()
{
    gtk_main_quit();
}
void on_DescriptionWindow_destroy()
{
    gtk_main_quit();
}

void on_window_main_destroy()
{
    gtk_main_quit();
}

void* SendAndReciev(void *arg)
{
    // General g = *((General *)arg);;
    pthread_t id = pthread_self();

    if(pthread_equal(id,tid[0]))
    {
	printf("--->%d %f %d\n",g1.bid_lot_id, g1.bid_amount, g1.bidder_client_id);
        if(send(sockfd, (General *)&g1, sizeof(g1),0)<0)
        {
            printf("Error\n");
            return ;
        }
    }
    else
    {
        if(recv(sockfd, (General *)&g2, sizeof(General),0)<0)
        {
            printf("Error\n");
            return ;
        }
    }

    for(int i=0; i<(0xFFFFFFFF);i++);

    return NULL;
}
