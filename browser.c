/* CSCI-4061 Fall 2022
 * Group Member #1: Amir Mohamed, moha1276
 * Group Member #2: Thomas Suiter, suite014
 * Group Member #3: Shannon Wallace, walla423
 */

#include "wrapper.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <gtk/gtk.h>

/* === PROVIDED CODE === */
// Function Definitions
void new_tab_created_cb(GtkButton *button, gpointer data);
int run_control();
int on_blacklist(char *uri);
int bad_format (char *uri);
void uri_entered_cb(GtkWidget* entry, gpointer data);
void init_blacklist (char *fname);

/* === PROVIDED CODE === */
// Global Definitions
#define MAX_TAB 100                 //Maximum number of tabs allowed
#define MAX_BAD 1000                //Maximum number of URL's in blacklist allowed
#define MAX_URL 100                 //Maximum char length of a url allowed

/* === STUDENTS IMPLEMENT=== */
// HINT: What globals might you want to declare?
char WWW[5] = {'w','w','w','.','\0'}; // "www." for string comparisons
char blackList[MAX_BAD][MAX_URL]; // allocating array of strings to hold blacklist strings
int tabNumber = 0;

/* === PROVIDED CODE === */
/*
 * Name:		          new_tab_created_cb
 *
 * Input arguments:	
 *      'button'      - whose click generated this callback
 *			'data'        - auxillary data passed along for handling
 *			                this event.
 *
 * Output arguments:   void
 * 
 * Description:        This is the callback function for the 'create_new_tab'
 *			               event which is generated when the user clicks the '+'
 *			               button in the controller-tab. The controller-tab
 *			               redirects the request to the parent (/router) process
 *			               which then creates a new child process for creating
 *			               and managing this new tab.
 */
// NO-OP for now
void new_tab_created_cb(GtkButton *button, gpointer data)
{}
 
/* === PROVIDED CODE === */
/*
 * Name:                run_control
 * Output arguments:    void
 * Function:            This function will make a CONTROLLER window and be blocked until the program terminates.
 */
int run_control()
{
  // (a) Init a browser_window object
	browser_window * b_window = NULL;

	// (b) Create controller window with callback function
	create_browser(CONTROLLER_TAB, 0, G_CALLBACK(new_tab_created_cb),
		       G_CALLBACK(uri_entered_cb), &b_window);

	// (c) enter the GTK infinite loop
	show_browser();
	return 0;
}

/* === STUDENTS IMPLEMENT=== */
/* 
    Function: on_blacklist  --> "Check if the provided URI is in th blacklist"
    Input:    char* uri     --> "URI to check against the blacklist"
    Returns:  True  (1) if uri in blacklist
              False (0) if uri not in blacklist
    Hints:
            (a) File I/O
            (b) Handle case with and without "www." (see writeup for details)
            (c) How should you handle "http://" and "https://" ??
*/ 
int on_blacklist (char *uri) {
  //STUDENTS IMPLEMENT
  // bool exists = 0;
  // for(int i = 0; i < 3; i++){
  //   if(strcmp(uri, blackList[i]) == 0) {
  //     exists = 1;
  //   }
  //   else {
  //     exists = 0;
  //   }
  // }
  return 0;
}

/* === STUDENTS IMPLEMENT=== */
/* 
    Function: bad_format    --> "Check for a badly formatted url string"
    Input:    char* uri     --> "URI to check if it is bad"
    Returns:  True  (1) if uri is badly formatted 
              False (0) if uri is well formatted
    Hints:
              (a) String checking for http:// or https://
*/
int bad_format (char *uri) {
    // Check if url starts with http or https //
  // Start bad form as true (1) //
  bool badForm = 1;
  if(strncmp(uri, "https://", 8) == 0){
    //fprintf(stderr, "https\n");
    badForm = 0;
  }
  else if(strncmp(uri, "http://", 7) == 0){
    //fprintf(stderr, "http\n");
    badForm = 0;
  }
  else 
    return badForm;

  // Check if url is shorter than max url length //
  if(strlen(uri) < MAX_URL) {
    //fprintf(stderr, "url length: %zu\n", strlen(uri)) ;
    badForm = 0;
  }
  else
    badForm = 1;

  return badForm;
}

/* === STUDENTS IMPLEMENT=== */
/*
 * Name:                uri_entered_cb
 *
 * Input arguments:     
 *                      'entry'-address bar where the url was entered
 *			                'data'-auxiliary data sent along with the event
 *
 * Output arguments:     void
 * 
 * Function:             When the user hits the enter after entering the url
 *			                 in the address bar, 'activate' event is generated
 *			                 for the Widget Entry, for which 'uri_entered_cb'
 *			                 callback is called. Controller-tab captures this event
 *			                 and sends the browsing request to the router(/parent)
 *			                 process.
 * Hints:
 *                       (a) What happens if data is empty? No Url passed in? Handle that
 *                       (b) Get the URL from the GtkWidget (hint: look at wrapper.h)
 *                       (c) Print the URL you got, this is the intermediate submission
 *                       (d) Check for a bad url format THEN check if it is in the blacklist
 *                       (e) Check for number of tabs! Look at constraints section in lab
 *                       (f) Open the URL, this will need some 'forking' some 'execing' etc. 
 */
void uri_entered_cb(GtkWidget* entry, gpointer data)
{
  // (a) What happens if data is empty? No Url passed in? Handle that
  if (data == NULL) {
    perror("No URL entered");
    return;
  }

 // (b) Get the URL from the GtkWidget (hint: look at wrapper.h)
  char *url_pointer = get_entered_uri(entry);

  char uri[MAX_URL];

  strncpy(uri, url_pointer, MAX_URL); // convert character pointer to string
  // we'll probably need to use other string functions on this 
  // so making sure it's got the null end character is important
  // gotta use strncpy() (not strcpy()) to prevent case where entered URL >> MAX_URL. 

 // (c) Print the URL you got, this is the intermediate submission
  puts(uri);
  
 // (d) Check for a bad url format THEN check if it is in the blacklist
  bool isBad = bad_format(url_pointer);
  if(isBad == 1) {
    alert("Bad URL format entered.");
    return;
  }

  bool inBlacklist = on_blacklist(url_pointer);
  if(inBlacklist == 1){
    printf("This URL exists in the blackList.");
  }
 // (e) Check for number of tabs! Look at constraints section in lab
 if(tabNumber >= MAX_TAB){
  alert("maximal no. of tabs cannot exceed MAX_TABS");
  return;
 }

 // (f) Open the URL, this will need some 'forking' some 'execing' etc. 
  pid_t pid = fork();
  if (pid == -1) 
  {
    perror("fork() failed");
    exit(1);
  }
  else if (pid == 0)
  {   
    //pass in the tabNumber 
    //convert tabNumber into a string
    char tab[20];
    sprintf(tab, "%d", tabNumber);   
    // int exe = execl("./render", "render", tab , url_pointer, NULL);
    // if(exe != 0){
    //   printf("Failed to execute render. Error code: %d \n", exe);
    // }
    exit(0);
  }
  else {
    tabNumber++;
  }

  return;
}



/* === STUDENTS IMPLEMENT=== */
/* 
    Function: init_blacklist --> "Open a passed in filepath to a blacklist of url's, read and parse into an array"
    Input:    char* fname    --> "file path to the blacklist file"
    Returns:  void
    Hints:
            (a) File I/O (fopen, fgets, etc.)
            (b) If we want this list of url's to be accessible elsewhere, where do we put the array?
*/
void init_blacklist (char *fname) {
		
	FILE *f;
	f = fopen(fname, "r"); 
	if (f==NULL) {
    perror("error opening blacklist");
 		exit(0);
  }
    
  // allocating character slots for storing one line/URL while working
  char tempString[MAX_URL]; 
  
  for (int i = 0; 
	  (fgets(tempString, MAX_URL, f));
	  i++) { 
  
    // truncate www. if it exists
    char first4[5];
    strncpy(first4, tempString, 4);  
    if (strcmp(first4, WWW) == 0) // if first4 == www, then: 
    { 
      // move pointer to start of string back 4 spaces and copy into blacklist
      strncpy(blackList[i], (tempString+4), (MAX_URL)); 
      // ex |www.google.com >> www.|google.com
    }

    // if it doesn't start with "www." then:
    else {
      strncpy(blackList[i], tempString, MAX_URL);
    }

  printf("blacklist[%d]: %s", i, blackList[i]);  

  }  

  if (fclose(f)) {
  	perror("cannot close file");
  }
  return;
}

/* === STUDENTS IMPLEMENT=== */
/* 
    Function: main 
    Hints:
            (a) Check for a blacklist file as argument, fail if not there [PROVIDED]
            (b) Initialize the blacklist of url's
            (c) Create a controller process then run the controller
                (i)  What should controller do if it is exited? Look at writeup (KILL! WAIT!)
            (d) Parent should not exit until the controller process is done 
*/
int main(int argc, char **argv)
{

  //STUDENT IMPLEMENT

  // (a) Check arguments for blacklist, error and warn if no blacklist
  if (argc != 2) {
    fprintf (stderr, "browser <blacklist_file>\n");
    exit (0);
  }
  
  // initialize pointer then open blacklist file
	char *fileName;
	fileName = argv[1];

  // (b) Initialize the blacklist of url's ** in progress
	init_blacklist(fileName);


  // (c) Create a controller process then run the controller
  //         (i)  What should controller do if it is exited? Look at writeup (KILL! WAIT!)

  pid_t controller = fork();
  int status = 0;
   
  if (controller == -1){
    perror("error creating controller fork");

  }
  else if (controller == 0) { // if process = child
    run_control();
    //have to kill & wait for processes
  }
  else { // if process = parent

  // (d) Parent should not exit until the controller process is done 
    wait(&status);
    // printf("parent done waiting");
  }


  return 0;
}
