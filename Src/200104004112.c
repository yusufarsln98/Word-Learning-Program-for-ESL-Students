#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ======= WORD LEARNING PROGRAM FOR ESL STUDENTS ======= */

/* Type definations and word definations */
typedef enum{syn = 1, ant = 2, syn_ant = 3} wordtype;
typedef enum{false = 0, true = 1} bool;
typedef enum{error = 0, success = 1} filereadtype;

#define TRUE 1
#define FALSE -1

#define WORDLENGTH 30               /* Longest word */
#define MAXUSERNUMBER 15            /* Max number of users */

/* Define file name */
#define SYNONYMS "synonyms.txt"
#define ANTONYMS "antonyms.txt"
#define USERS    "userfiles.txt"

/* Struct for keep the synonyms and antonyms in a linked list */
typedef struct word_t
{
    char *word;
    struct word_t *next_word;
} word_t;

/* Struct for create linked list for keep base word and its synonyms or antonyms */
typedef struct wordlist_t
{
    char *baseword;
    char *ID;
    word_t *correspond;
    struct wordlist_t *next_baseword;
} wordlist_t;

/* Struct for create linked list to keep user informations */
typedef struct userrecord_t
{
    char word[WORDLENGTH];      /* Word */
    wordtype ID;                /* ID */
    int queue;                  /* Unique adres from 1 to n */
    int corr_syn;               /* Correct answers for synonyms */
    int wron_syn;               /* Wrong answers for synonyms */
    int corr_ant;               /* Correct answers for antonyms */
    int wron_ant;               /* Wrong answers for antonyms */
    int qnum_syn;               /* Number of synonyms questions */
    int qnum_ant;               /* Number of antonyms questions */
    struct userrecord_t *next_word;
} userrecord_t;


/* ------------- FUNCTION PROTOTYPES ------------- */
filereadtype Read_Word_Files(wordlist_t **list, char *file_name);
word_t *Create_word_Node(char *token);
wordlist_t *Create_wordlist_Node(char *line);
filereadtype Update_wordlist(wordlist_t *list_head, char *file_name);
void Free_wordlist(wordlist_t *list_head);
filereadtype Get_usernames(char user_array[][WORDLENGTH], int *usernumber);
filereadtype Add_userfile(char *username);
bool Is_user_exist(char user_array[][WORDLENGTH], int usernumber, char *username);
void Create_userrecord(wordlist_t *syno_head, wordlist_t *anto_head, userrecord_t **new_user);
userrecord_t *Create_userrecord_node(wordlist_t *list_head);
userrecord_t *Is_userrecord_Node_Exist(userrecord_t *list_head, char *word);
filereadtype Update_userrecord(userrecord_t *list_head, char *username);
filereadtype Get_userrecords(userrecord_t **user, char *username);
userrecord_t *Create_userrecord_node_buffer(userrecord_t node);
void Free_user(userrecord_t *list_head);
void Add_syno_anto(wordlist_t *syno_head, wordlist_t *anto_head);
wordlist_t *is_word_exist(wordlist_t *list_head, char *buffer);
int Find_correct_number(userrecord_t *list_head);
int Find_wrong_number(userrecord_t *list_head);
int Find_word_number(userrecord_t *list_head);
userrecord_t *Find_possible_record_node(userrecord_t *list_head);
int Find_lcm(userrecord_t *list_head);
int gcd(int a, int b);
int Find_array_size(userrecord_t *list_head, int LCM);
void Fill_array(userrecord_t *list_head, int *shuffle, int LCM);
int Find_possible_question(userrecord_t *node);
bool Is_word_correspond(wordlist_t *list_head, char *base, char *word);
void Add_new_word(wordlist_t *list_head, char *base, char *word);
/* ------------- FUNCTION PROTOTYPES ------------- */


/* ============ MAIN FUNCTION ============ */
int main()
{
    wordlist_t *_Synonyms = NULL;                   /* List for keep synonyms.txt elements */
    wordlist_t *_Antonyms = NULL;                   /* List for keep antonyms.txt elements */
    userrecord_t *_CurrentUser = NULL;              /* List for keep user records */
    userrecord_t *record_node = NULL;               /* Keeps one userrecord_t node */

    char user_array[MAXUSERNUMBER][WORDLENGTH],     /* Keeps user names */
         username[WORDLENGTH],                      /* To take user name from terminal */
         word[WORDLENGTH] = "\0",                   /* To take answer of questions */
         opt[WORDLENGTH] = "\0";                    /* Operator variables */
    char subopt = '\0';                             /* Operator variables */

    int operator = -1,                              /* Operator variables */
        usernumber = 0,                             /* Number of users */
        userindex = -1,                             /* Index of users array */
        possibility = 0,                            /* Question type possibility */
        i = 0,                                      /* Loop initiliazer */
        menu = 0;
    
    filereadtype flag;                              /* Check if the file opened successfully */

    srand(time(NULL));                              /* Random number generetor */


    /* ----- Reading files ----- */

    /* Take synonyms from text file */
    flag = Read_Word_Files(&_Synonyms, SYNONYMS);   
    if (flag == error) return error;

    /* Take antonyms from text file */
    flag = Read_Word_Files(&_Antonyms, ANTONYMS);
    if (flag == error)
    {
        Free_wordlist(_Synonyms);
        return error;
    }

    /* Get previous user names */
    flag = Get_usernames(user_array, &usernumber);
    if (flag == error)
    {
        Free_wordlist(_Synonyms);
        Free_wordlist(_Antonyms);
        return error;   
    }
    /* ----- End of reading files ----- */


    /* ============== Take the user name ============== */

    printf("\nWelcome to synonyms and antonyms learning program for ESL students!\n");
    printf("--------------------------------------------------------------------\n");
    printf("\nKnock Knock! Who are you?\n");
    do
    {
        /* --- Ask for the operation --- */
        printf("1. I'm a new user.\n");
        printf("2. I'm old.\n");
        scanf("%d", &operator);
        getchar();

        if (operator == 1)                          /* If operator is 'new user' */
        {
            /* --- Ask for the user name --- */
            printf("\nGood to see you! Please type a user name.\n(To exit: -1)\n");
            scanf("%[^\n]%*c", username);
            
            /* If username is different from -1(exit) */
            if (strcmp(username, "-1") == 0) printf("\nSee you later! \n");
            else
            {
                /* Check if user name is taken */
                while (Is_user_exist(user_array, usernumber, username) == true)
                {
                    /* If it is taken then ask a new user name */
                    printf("\nWhoops!! This user name has already been taken. \nPlease type a new user name.\n");
                    scanf("%[^\n]%*c", username);
                    if (strcmp(username, "-1") == 0)
                    {
                        printf("\nSee you later! \n");
                        break;
                    }
                }
                /* Add new username to the userfiles.txt */
                flag = Add_userfile(username);
                if (flag == error)
                {
                    Free_wordlist(_Synonyms);
                    Free_wordlist(_Antonyms);
                    return error; 
                }
            }
        }
        else if (operator == 2)                         /* If operator is 'old' */
        {
            if (usernumber == 0)                        /* If there is no one in the list */
            {
                /* Then ask user if he/she wants to be first user */
                printf("\nOh no! There is nobody old here!\n");
                printf("Do you want to be first user? If so, type a user name.\n(To exit: -1)\n");
                scanf("%[^\n]%*c", username);
                if (strcmp(username, "-1") == 0) printf("\nSee you later! \n");
                else
                {
                    /* Check if user name is taken */
                    while (Is_user_exist(user_array, usernumber, username) == true)
                    {
                        printf("\nThis user name has already been taken. Please type a new user name.\n");
                        scanf("%[^\n]%*c", username);
                        if (strcmp(username, "-1") == 0)
                        {
                            printf("\nSee you later! \n");
                            break;
                        }
                    }
                    Add_userfile(username);
                    if (flag == error)
                    {
                        Free_wordlist(_Synonyms);
                        Free_wordlist(_Antonyms);
                        return error; 
                    }
                    operator = 1;
                }
            }
            else                                            /* If there are users in the list */
            {
                /* Then ask the user to select his/her name */
                printf("\nPlease select your user name.\n");
                for (i = 0; i < usernumber; i++) printf("%d.\t%.*s\n", i, (int)(strlen(user_array[i]) - 8), user_array[i]);
                do
                {
                    scanf("%d", &userindex);
                    getchar();
                    if (!(userindex >= 0 && userindex <= usernumber - 1))
                    {
                        printf("Wrong entry! Try Again.\n");
                    }
                } while (!(userindex >= 0 && userindex <= usernumber - 1));
                strncpy(username, user_array[userindex], strlen(user_array[userindex]) - 8);
            }
        }
        else printf("\nInvalid Enter! Try Again.\n");
        
    } while(!(operator == 1 || operator == 2));
    
    /* If user wants to terminate the program, then free the memory and return */
    if (strcmp(username, "-1") == 0)
    {
        Free_wordlist(_Synonyms);
        Free_wordlist(_Antonyms);
        return 0;
    }
    /* ============== End of take user name ============== */


    /* ============== Create userrecord_t lists for the users ============== */
    /*
        When user decide to start the program,
        According to the condition,
        1- If user is 'new' it will be created userrecord_t type linked list from
        _Synonyms and _Antonyms lists.
        2- If user is 'old' it will read records from binary files
        through Get_userrecords();

        Then operations going to be done with the list first.
    */

    /* If user is 'new' then create a list for user */
    if (operator == 1) Create_userrecord(_Synonyms, _Antonyms, &_CurrentUser);
    
    /* Otherwise, get records from binary files. */ 
    else                
    {
        flag = Get_userrecords(&_CurrentUser, username);
        if (flag == error)
        {
            Free_user(_CurrentUser);
            Free_wordlist(_Synonyms);
            Free_wordlist(_Antonyms);
            return error;
        }
    }
    /* ============== Create userrecord_t lists for the users ============== */

    /* ============================== MENU ============================== */
    /* --- Print a legend --- */
    printf("\n\n--- Last Performance of Current User ---\n");
    printf("----------------------------------------\n\n");
    printf(" User: %s\n Number of correct answers:  %d\n Number of wrong answers:  %d\n Number of word in the list (Commons count as 1): %d\n", 
    username, Find_correct_number(_CurrentUser), Find_wrong_number(_CurrentUser), Find_word_number(_CurrentUser));
    printf("----------------------------------------\n");

    while (1)
    {
        printf("\n\n=============== MENU ===============\n");
        printf("\nEnter the operation you want to do: \n");
        printf("\n1. Add a new synonym or antonym \n");
        printf("2. Start the quiz \n");
        printf("3. Print current performance \n");
        printf("4. Exit \n");
        scanf("%d", &menu);

        if (!(menu == 1 || menu == 2 || menu == 3 || menu == 4))
        {
            printf("\nInvalid Operation! Please try again.\n");
        }
        /*
            According to the instructions there should be a part for adding synonyms or antonyms of words.
            Following condition orient user to the Add_syno_anto(); part of the program to add words to the list.
        */
        else if (menu == 1) Add_syno_anto(_Synonyms, _Antonyms);
        else if (menu == 2)
        {
            /*
                In this part with Find_possible_record_node(); function, program finds a possible 'word' to ask user.
                This possibility is depend on the 2 basic mentality:
                If a word asked more then the other words, It will has less probability to being asked.(But if user give a wrong answer).
                According to the number of questions and number of correct and wrong answer I've created a formula to calculate possibility.
                They will be explained in the related functions.
            */
            word[0] = '\0';
            printf("\n--- Synonym and Antonym Test---");
            while (strcmp(word, "-1") != 0)
            {
                printf("\n-------------------------------\n");
                for (i = 0; i < WORDLENGTH; i++) word[i] = '\0';
                
                /* Find possible word to ask user by Find_possible_record_node(); */
                record_node = Find_possible_record_node(_CurrentUser);
                /*
                    After finding the node there will be three type of word.
                    1- Word can be in the both _Synonyms and _Antonyms list.
                    2- Word can be in only _Synonyms list.
                    3- Word can be in only _Antonyms list.
                
                */
                if (record_node->ID == syn_ant)         /* If word is in both list */     
                {
                    /* Find_possible_question(); going to return more possible question
                    depend on the number of questions asked. 
                    If a word being asked 4 times for synonyms and 1 time for antonyms
                    It will have 4 time more possibilty to ask antonym of it */
                    
                    possibility = Find_possible_question(record_node);

                    if (possibility % 2 == 0)       /* If question is (0)synonym */
                    {
                        /* Ask synonym of the word. (-1) for exit */
                        printf("\nWhat is a synonym of '%s'?(To back: -1)\n --> ", record_node->word);
                        scanf(" %s", word);
                        if (strcmp(word, "-1") != 0)
                        {
                            /* If the given answer is in the synonym list then congratulate user and increase correct number */
                            if (Is_word_correspond(_Synonyms, record_node->word, word) == true) 
                            {
                                printf("\nYou got it right. Thumbs up!\n");
                                record_node->corr_syn++;
                            }
                            /* 
                                If the answer is false, ask user to accept it as a correct answer. 
                                If user approve it then put it to the list and increase correct number. 
                                Other wise increase wrong number. 
                            */
                            else 
                            {
                                for (i = 0; i < WORDLENGTH; i++) opt[i] = '\0';
                                printf("\nWrong answer! Do you want to add '%s' as synonym of '%s' (y/n)\n", word, record_node->word);
                                scanf(" %s", opt);
                                if (strcmp(opt, "y") == 0)
                                {
                                    printf("\n'%s' successfully add as an synonym of '%s'!\n", word, record_node->word);
                                    Add_new_word(_Synonyms, record_node->word, word);
                                    record_node->corr_syn++;
                                }
                                else
                                {
                                    printf("Do not worry! You'll find next time.\n");
                                    record_node->wron_syn++;
                                }
                            }
                            record_node->qnum_syn++;    /* Increase number of question asked */
                        }
                    }
                    else if (possibility % 2 == 1)      /* If question is (1)antonym */
                    {
                        /* Ask antonym of the word. (-1) for exit */
                        printf("\nWhat is a antonym of '%s'?(To back: -1)\n --> ", record_node->word);
                        scanf(" %s", word);
                        if (strcmp(word, "-1") != 0)
                        {
                            /* If the given answer is in the synonym list then congratulate user and increase correct number */
                            if (Is_word_correspond(_Antonyms, record_node->word, word) == true) 
                            {
                                printf("\nYou got it right. Thumbs up!\n");
                                record_node->corr_ant++;
                            }
                            else 
                            {
                                /* 
                                    If the answer is false, ask user to accept it as a correct answer. 
                                    If user approve it then put it to the list and increase correct number. 
                                    Other wise increase wrong number. 
                                */
                                for (i = 0; i < WORDLENGTH; i++) opt[i] = '\0';
                                printf("\nWrong answer! Do you want to add '%s' as antonym of '%s' (y/n)\n", word, record_node->word);
                                scanf(" %s", opt);
                                if (strcmp(opt, "y") == 0)
                                {
                                    printf("\n'%s' successfully add as an antonym of '%s'!\n", word, record_node->word);
                                    Add_new_word(_Antonyms, record_node->word, word);
                                    record_node->corr_ant++;
                                }
                                else
                                {
                                    printf("Do not worry! You'll find next time.\n");
                                    record_node->wron_ant++;
                                }
                            }
                            record_node->qnum_ant++;    /* Increase number of question asked */
                        }            
                    }
                }
                
                else if (record_node->ID == syn)        /* If word is only in synonyms list */
                {
                    /* Ask synonym of the word. (-1) for exit */
                    printf("\nWhat is a synonym of '%s'?(To back: -1)\n --> ", record_node->word);
                    scanf(" %s", word);

                    if (strcmp(word, "-1") != 0)
                    {
                        /* If the given answer is in the synonym list then congratulate user and increase correct number */
                        if (Is_word_correspond(_Synonyms, record_node->word, word) == true) 
                        {
                            printf("\nYou got it right. Thumbs up!\n");
                            record_node->corr_syn++;
                        }
                        else 
                        {
                            /* 
                                If the answer is false, ask user to accept it as a correct answer. 
                                If user approve it then put it to the list and increase correct number. 
                                Other wise increase wrong number. 
                            */
                            for (i = 0; i < WORDLENGTH; i++) opt[i] = '\0';
                            printf("\nWrong answer! Do you want to add '%s' as synonym of '%s' (y/n)\n", word, record_node->word);
                            scanf(" %s", opt);
                            if (strcmp(opt, "y") == 0)
                            {
                                printf("\n'%s' successfully add as an synonym of '%s'!\n", word, record_node->word);
                                Add_new_word(_Synonyms, record_node->word, word);
                                record_node->corr_syn++;
                            }
                            else
                            {
                                printf("Do not worry! You'll find next time.\n");
                                record_node->wron_syn++;
                            }
                        }
                        record_node->qnum_syn++;        /* Increase number of question asked */
                    }
                }
                
                else if (record_node->ID == ant)        /* If word is only in antonyms list */
                {
                    /* Ask antonym of the word. (-1) for exit */
                    printf("\nWhat is a antonym of '%s'?(To back: -1)\n --> ", record_node->word);
                    scanf(" %s", word);
                    if (strcmp(word, "-1") != 0)
                    {
                        /* If the given answer is in the synonym list then congratulate user and increase correct number */
                        if (Is_word_correspond(_Antonyms, record_node->word, word) == true) 
                        {
                            printf("\nYou got it right. Thumbs up!\n");
                            record_node->corr_ant++;
                        }
                        else 
                        {
                            /* 
                                If the answer is false, ask user to accept it as a correct answer. 
                                If user approve it then put it to the list and increase correct number. 
                                Other wise increase wrong number. 
                            */
                            for (i = 0; i < WORDLENGTH; i++) opt[i] = '\0';
                            printf("\nWrong answer! Do you want to add '%s' as antonym of '%s' (y/n)\n", word, record_node->word);
                            scanf(" %s", opt);
                            if (strcmp(opt, "y") == 0)
                            {
                                printf("\n'%s' successfully add as an antonym of '%s'!\n", word, record_node->word);
                                Add_new_word(_Antonyms, record_node->word, word);
                                record_node->corr_ant++;
                            }
                            else
                            {
                                printf("Do not worry! You'll find next time.\n");
                                record_node->wron_ant++;
                            }
                        }
                        record_node->qnum_ant++;        /* Increase number of question asked */
                    }
                }
            } 
        }
        else if (menu == 3)
        {
            /* ---- Print current performance of the user ---- */
            printf("\n--- Performance of Current User ---");
            printf("\n-----------------------------------\n");
            printf("\n Current user: %s \n Number of correct answers:  %d\n Number of wrong answers:  %d\n Number of word in the list (Commons count as 1): %d\n", 
                   username, Find_correct_number(_CurrentUser), Find_wrong_number(_CurrentUser), Find_word_number(_CurrentUser));
        }
        else if (menu == 4) break;
    }
    printf("\nProgram Terminated. See you later!\n");
    printf("====================================\n\n");
    /* ============================== END OF THE MENU ============================== */

    /* ======= Update the files and user records ======= */
    flag = Update_userrecord(_CurrentUser, username);
    flag = Update_wordlist(_Synonyms, SYNONYMS);
    flag = Update_wordlist(_Antonyms, ANTONYMS);

    /* ======= Free the linked lists ======= */
    Free_user(_CurrentUser);
    Free_wordlist(_Synonyms);
    Free_wordlist(_Antonyms);

    return 0;
}

/* ========== Creating linked word lists ========== */
filereadtype Read_Word_Files(wordlist_t **list, char *file_name)
{
    FILE *infilep;                              /* Initiliazers */
    wordlist_t *_New_wordlist_Node = NULL;      /* Initiliazers */
    char line[1024];                            /* Initiliazers */

    infilep = fopen(file_name, "r");            /* Open file */
    if (infilep == NULL)
    {
        printf("The %s file could not be opened!\n", file_name);
        return error;
    }

    while(fgets(line, 1024, infilep) != NULL)   /* Read file until the last line */
    {   
        if (strlen(line) < 5) break;            /* If there is emptly lines then stop reading */
        
        /* Create node by Create_wordlist_Node(); Then add it to the list */
        _New_wordlist_Node = Create_wordlist_Node(line);
        if ((*list) != NULL) _New_wordlist_Node->next_baseword = (*list);
        
        (*list) = _New_wordlist_Node;
    }
    fclose(infilep);
    return success;
}

wordlist_t *Create_wordlist_Node(char *line)
{
    wordlist_t *_New_Node = NULL;       /* Initiliazers */
    word_t *_New_word_Node = NULL;      /* Initiliazers */

    int i = 0;                          /* Initiliazers */
    char *token = NULL;                 /* Initiliazers */
    char str[2];                        /* Initiliazers */
    str[0] = ' ';                       /* Initiliazers */

    /* 
        This function takes a line from word lists. 
        Then through strtok function decompose it and with this components
        creates wordlist_t node.
        Finally returns that node.
    */

    /* Allocate memory for new node */
    _New_Node = (wordlist_t *)malloc(sizeof(wordlist_t));

    /* Until the end of the line, decompose the line. */
    for (token = strtok(line, str), i = 0; token != NULL; token = strtok(NULL, str), i++)
    {
        if (token[strlen(token) - 1] == '\n')           /* There may be CARRIAGE RETURN char in the line, handle it */
        {
            if (token[strlen(token) - 2] == 13) token[strlen(token) - 2] = '\0';
            else token[strlen(token) - 1] = '\0';
        }

        /* According to the loop condition place the components in to the node */
        if (i == 0)                     /* Loop 0: For base word. Insert it to the node */                               
        {
            _New_Node->baseword = (char *)malloc(strlen(token) + 1);
            strcpy(_New_Node->baseword, token);
            _New_Node->baseword[strlen(token)] = '\0';
        } 
        else if (i == 1)                /* Loop 1: For ID. = or <> */
        {
            _New_Node->ID = (char *)malloc(strlen(token + 1));
            strcpy(_New_Node->ID, token);
            _New_Node->ID[strlen(token)] = '\0';
        }
        else                            /* Loop 2, 3...: For the word synonyms or antonyms */
        {
            /* Create word_t node then insert it to the list */
            _New_word_Node = Create_word_Node(token);
            if (_New_Node->correspond != NULL) _New_word_Node->next_word = _New_Node->correspond;
            _New_Node->correspond = _New_word_Node;
        }
    }
    _New_Node->next_baseword = NULL;
    return _New_Node;
}

word_t *Create_word_Node(char *token)
{
    word_t *_New_Node = NULL;
    /* Takes a word then create a word_t node from that word(token) */
    _New_Node = (word_t *)malloc(sizeof(word_t));
    _New_Node->word = (char *)malloc(strlen(token) + 1);
    strcpy(_New_Node->word, token);
    _New_Node->word[strlen(token)] = '\0';
    _New_Node->next_word = NULL;
    return _New_Node;
}
/* ========== End of creating linked lists ========== */

/* ========== Getting user names into an array  ========== */
filereadtype Get_usernames(char user_array[][WORDLENGTH], int *usernumber)
{
    FILE *fp;                   /* Initiliazers */
    char line[1024];            /* Initiliazers */

    fp = fopen(USERS, "a+");     /* Open file */
    if (fp == NULL)
    {
        printf("The %s file could not be opened!\n", USERS);
        return error;
    }
    /* Until the end of the line take user names from the userfiles.txt */
    while (fgets(line, 1024, fp) != NULL)
    {
        if (line[0] == '\n' || line[0] == 13 || line[0] == -1) break;
        if (line[strlen(line) - 1] == '\n')
        {
            if (line[strlen(line) - 2] == 13) line[strlen(line) - 2] = '\0';
            else line[strlen(line) - 1] = '\0';
        }
        strcpy(user_array[*usernumber], line);
        (*usernumber)++;
    }
    fclose(fp);
    return success;
}
/* ========== Getting user names into an array  ========== */

/* ========== Add new user names into the list ========== */
bool Is_user_exist(char user_array[][WORDLENGTH], int usernumber, char *username)
{
    int i;
    /* Check if user name is already exist */
    for (i = 0; i < usernumber; i++)
    {
        if (strncmp(user_array[i], username, strlen(username)) == 0) return true;
    }
    return false;
}

filereadtype Add_userfile(char *username)
{
    FILE *fp;
    /* Add user name into the userfiles.txt file */
    fp = fopen(USERS, "a");
    if (fp == NULL)
    {
        printf("The %s file could not be opened!\n", USERS);
        return error;
    }
    fprintf(fp, "%s.worddat\n", username);
    fclose(fp);
    return success;
}
/* ========== End of the adding usernames into the list ========== */

/* ============== Add synonyms and antonyms ============== */
void Add_syno_anto(wordlist_t *syno_head, wordlist_t *anto_head)
{
    int operator = 0, i;                    /* Operator and initiliazers */
    char base[WORDLENGTH] = "\0";           /* The word user wants to add syn. or ant. of it */
    char word[WORDLENGTH];                  /* The word user wants to as syn. or ant. */
    wordlist_t *controller = NULL;          /* Check if the base in the list */
    word_t *word_node = NULL;               /* Create word_t node from char word[]; */

    /* ============================ MENU ============================ */
    /*
        Until user enter the "-1", loop continue to take words.
        Firstly, main word asked for user,
        If there is not any word corresponding to the it,
        Error message will be printed and ask for a new entry.

        If there is such a main word in the list it will ask for the new antonym/synonym.  
    */
    printf("\n--- Add synonym or antonym of a word ---\n");
    printf("----------------------------------------\n");
    while (strcmp(base, "-1") != 0)         
    {
        for (i = 0; i < (int)strlen(base) + 1; i++) base[i] = '\0';

        /* Take main word */
        printf("\nPlease enter the word you want to add synonym or antonym of it. (To back: -1)\n");
        scanf(" %s", base);

        if (strcmp(base, "-1") != 0)    /* If it is not "-1"(exit) do the following operations */
        {
            /* Ask for operation until user enter a correct one */
            printf("\nWhich one do you want to add?\n1-\tSynonym\n2-\tAntonym\n");
            while (!(operator == 1 || operator == 2))   
            {
                scanf("%d", &operator);
                getchar();
                if (!(operator == 1 || operator == 2)) printf("Enter 1 for Synonym 2 for Antonym.\n");
                
                if (operator == 1)          /* If operation is (1)synonym then check the the _Synonyms */
                {
                    controller = is_word_exist(syno_head, base);    /* Check if there is such a word */
                    if (controller == NULL) printf("\nword: %s, DOES NOT EXIST in the %s file\n", base, SYNONYMS);
                    else                    /* If there is a word then ask user for the enter synonym of it */
                    {
                        printf("Please enter the synonym of the %s.\n", base);
                        scanf(" %s", word);

                        /* Create node and put it to the head of the list */
                        word_node = Create_word_Node(word);
                        word_node->next_word = controller->correspond;
                        controller->correspond = word_node;
                        printf("\n'%s' successfully add to the '%s' as a new synonym.\n", word, base);
                    }
                }
                else if (operator == 2)     /* If operation is (2)antonym then check the the _Antonyms */
                {
                    controller = is_word_exist(anto_head, base);    /* Check if there is such a word */
                    if (controller == NULL) printf("\nword: %s, DOES NOT EXIST in the %s file\n", base, ANTONYMS);
                    else                    /* If there is a word then ask user for the enter antonym of it */
                    {   
                        printf("Please enter the antonym of the %s.\n", base);
                        scanf(" %s", word);
                        word_node = Create_word_Node(word);
                        word_node->next_word = controller->correspond;
                        controller->correspond = word_node;
                        printf("\n'%s' successfully add to the '%s' as a new antonym.\n", word, base);
                    }
                }
            }
        }
        operator = 0;
    }
}

wordlist_t *is_word_exist(wordlist_t *list_head, char *buffer)
{
    /* Check the end of the list then return the node if there is a word */
    for (; list_head != NULL; list_head = list_head->next_baseword)
    {
        if (strcmp(list_head->baseword, buffer) == 0) return list_head;
    }
    return NULL;
}
/* ============== End of synonyms and antonyms ============== */

/* ============== Get user records from binary files ============== */
filereadtype Get_userrecords(userrecord_t **user, char *username)
{
    userrecord_t buffer, *new_node;             /* Buffer for reading nodes, new_node for create linked list */
    FILE *fp = NULL;                            /* File pointer */
    char filename[WORDLENGTH + 8];              /* Filename from username */
    size_t size = 0;                            /* Size of struct except *next_word pointer */
    
    /* Find size of records */
    size = sizeof(userrecord_t) - sizeof((*user)->next_word);
    /* Re-create username format */
    strcpy(filename, username);
    strcat(filename, ".worddat");
    filename[strlen(username) + 8] = '\0';

    /* --- Open file then read informations struct by struct bytes from binary file --- */
    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("The %s file could not be opened!\n", filename);
        return error;
    }
    
    /* Until end of the file, read file struct by struct */
    while (fread(&buffer, size, 1, fp) != 0)
    {
        /* Create node and put them into the list */
        new_node = Create_userrecord_node_buffer(buffer);
        if ((*user) != NULL) new_node->next_word = (*user);
        (*user) = new_node;
    }
    fclose(fp);
    return success;
}

userrecord_t *Create_userrecord_node_buffer(userrecord_t node)
{
    userrecord_t *new_node = NULL;
    /* Create userrecord_t node from buffer node read from binary file */
    new_node = (userrecord_t *)malloc(sizeof(userrecord_t));
    strcpy(new_node->word, node.word);
    new_node->ID = node.ID;
    new_node->corr_syn = node.corr_syn;
    new_node->wron_syn = node.wron_syn;
    new_node->corr_ant = node.corr_ant;
    new_node->wron_ant = node.wron_ant;
    new_node->qnum_syn = node.qnum_syn;
    new_node->qnum_ant = node.qnum_ant;
    new_node->queue = node.queue;
    new_node->next_word = NULL;

    return new_node;
}
/* ============== End of  get user records from files ============== */

/* ============== Create new_user from _Synonyms and _Antonyms lists ============== */
void Create_userrecord(wordlist_t *syno_head, wordlist_t *anto_head, userrecord_t **new_user)
{
    userrecord_t *new_node, *controller;    /* Initilizers */
    int i = 1;                              /* Create ID for each node */

    /* 
        There is 2 list from files.
        Firstly, from synonyms list I inserted all information to the 
        userrecord_t new_user list.
        Then, since there is the same word from both list,
        I've checked if it is inserted alread, if so, I updated to the list,
        otherwise, add a new node.
    */
    /* Go first element to the last element of synonyms list */
    for (; syno_head != NULL; syno_head = syno_head->next_baseword) 
    {
        /* Create userrecord_t node with information in the synonym list */
        new_node = Create_userrecord_node(syno_head);
        new_node->ID = syn;                 /* Mark the node type */
        new_node->queue = i;                /* Give an ID according to the reading order */
        
        /* Add new_node to the list */
        if ((*new_user) != NULL) new_node->next_word = (*new_user);
        (*new_user) = new_node;

        i++;
        
    }
    /* Go first element to the last element of antonyms list */
    for (; anto_head != NULL; anto_head = anto_head->next_baseword)
    {
        /* Create userrecord_t node with information in the antonym list */
        new_node = Create_userrecord_node(anto_head);

        /* Check if node is exist */
        controller = Is_userrecord_Node_Exist(*new_user, new_node->word);
        if (controller != NULL)         /* If so, then update it */
        {
            controller->ID = syn_ant;
            free(new_node);
        }
        else                            /* Otherwise, add new node to the new_user list */
        {
            new_node->ID = ant;
            new_node->queue = i;
            if ((*new_user) != NULL) new_node->next_word = (*new_user);
            (*new_user) = new_node;
            i++;
        }
    }
}

userrecord_t *Create_userrecord_node(wordlist_t *list_head)
{
    /* Create a new node */
    userrecord_t *node = NULL;
    node = (userrecord_t *)malloc(sizeof(userrecord_t));
    strcpy(node->word, list_head->baseword);
    node->wron_syn = 0;
    node->wron_ant = 0;
    node->corr_syn = 0;
    node->corr_ant = 0;
    node->qnum_syn = 1;
    node->qnum_ant = 1;
    node->queue = 0;
    node->ID = 0;
    node->next_word = NULL;
    return node;
}

userrecord_t *Is_userrecord_Node_Exist(userrecord_t *list_head, char *word)
{
    /* Check end of the list, If the word is already exist then return true */
    for (; list_head != NULL; list_head = list_head->next_word)
    {
        if (strcmp(list_head->word, word) == 0) return list_head;
    }
    return NULL;
}
/* ============== End of create new_user from lists ============== */

/* ============== Possibility calculations ============== */
userrecord_t *Find_possible_record_node(userrecord_t *list_head)
{
    int i, ID, LCM, array_size;                             /* Initiliazers */
    int *shuffle;                                           /* Initiliazers */
    
    LCM = Find_lcm(list_head);                              /* Find Least Common Multiple of the list */
    array_size = Find_array_size(list_head, LCM);           /* Create an array size with this informations */

    /* 
        Create an array and send it to the Fill_array() function.
        To understand the shuffle idea please go the the 
        Fill_array(); and Find_array_size(); functions. 
    */

    shuffle = (int *)calloc(array_size, sizeof(int));
    
    /* --- To prevent any error by very long time usage of program. --- */
    if (shuffle == NULL)                
    {
        printf("\n\nMemory usage reached max. Please terminate the program!\n\n");
        return list_head;
    }


    Fill_array(list_head, shuffle, LCM);
    
    /* Find index randomly and free the array */
    i = rand() % array_size;
    ID = shuffle[i];
    free(shuffle);

    /* Go to the ID from shuffle array and return as the question */
    while (list_head->queue != ID) list_head = list_head->next_word;
    
    return list_head;
}

int Find_array_size(userrecord_t *list_head, int LCM)
{
    int ans = 0,            /* Total size */
        qnum,               /* Question number */
        priority,           /* Prioriy created with qnum and weighted_grade */
        weigthed_grade;     /* Formula created by me :-) */

    /*
        (Before explanation, remember that userrecord_t stuct has an queue variable
         which keep the ORDER of the list as an unique number. from 1 to end of the list)
        
        Lets assume that,
        There is 4 word records like this:
            Question number     Correct Number      Wrong Number
        1-       4                   2                   2
        2-       7                   1                   6
        3-       2                   0                   2
        4-       4                   4                   0
        -------------------------------------------------------
        
        We know that a word asked more then other has less possibility to be asked again but user answer is WRONG.
        So, I'm going to give each word some weighted values with the formula I created.
        I've LCM as a parameter.
        
        In this case LCM will be --> 28

        Weighted Values:
        1-  
        qnum = 4
        weighted_grade = (2 * 2) - 2 = 2
        priority = 28 / 4 = 7
        new priority = priority(7) + (weigthed_grade(2) * priority(7)) = 21
        answer (possibility) = 21
        -------------------------
        2-  
        qnum = 7
        weighted_grade = (2 * 6) - 1 = 11
        priority = 28 / 7 = 4
        new priority = priority(4) + (weigthed_grade(11) * priority(4)) = 48
        answer (possibility) = 48
        -------------------------
        3-  
        qnum = 2
        weighted_grade = (2 * 2) - 0 = 4
        priority = 28 / 2 = 14
        new priority = priority(14) + (weigthed_grade(4) * priority(14)) = 70
        answer (possibility) = 70
        -------------------------
        4-  
        qnum = 4
        weighted_grade = (2 * 0) - 4 = -4
        weighted_grade < 0 => weighted_grade = 0
        priority = 28 / 4 = 7
        new priority = priority(7) + (weigthed_grade(0) * priority(7)) = 7
        answer (possibility) = 7
        -------------------------

        Possibilities = 21, 48, 70, 7

        When we examine the probabilities, we can observe that number 3, which was asked less frequently, 
        and number 2, which was given a lot of wrong answers, weighed more.

        When we compare numbers 1 and 4, we can observe that the probability of number 4 with more correct answers 
        is lower then the number 1.
    */

    for (; list_head != NULL; list_head = list_head->next_word)
    {
        /* My formula application for the size of array */
        qnum = list_head->qnum_ant + list_head->qnum_syn;
        weigthed_grade =  2 * (list_head->wron_ant + list_head->wron_syn) - (list_head->corr_ant + list_head->corr_syn);
        if (weigthed_grade < 0) weigthed_grade = 0;
        priority = LCM / qnum;
        priority += ((weigthed_grade) * priority);
        ans += priority;
    }
    return ans;
}

void Fill_array(userrecord_t *list_head, int *shuffle, int LCM)
{
    int i, j = 0, qnum, weigthed_grade, ans, priority;  /* Initiliazers */

    /* 
        According to the formula from Find_array_size(); function
        We create a array with that weights according to the answers.
        Then fill the array with unique numbers in the struct (->queue)
        And finally, with the size of array, we choose a random index of array
        in the Find_possible_record_node(); function
    */
    for (; list_head != NULL; list_head = list_head->next_word)
    {
        /* My formula application for the size of array */
        qnum = list_head->qnum_ant + list_head->qnum_syn;
        weigthed_grade = 2 * (list_head->wron_ant + list_head->wron_syn) - (list_head->corr_ant + list_head->corr_syn);
        if (weigthed_grade < 0) weigthed_grade = 0;
        priority = LCM / qnum;
        priority += ((weigthed_grade) * priority);
        ans = priority;
        for (i = 0; i < ans; i++)
        {
            shuffle[j] = list_head->queue;
            j++;
        }
    }
}

int Find_possible_question(userrecord_t *node)
{
    int *shuffle;       /* Initiliazers */
    int i, j, size;
    /* 
        Some words have both synonyms and antonyms
        So, if a word asked for synonyms for 1 time asked for antonyms for 5 time
        Possibility of asking antonyms should be 
        5 times larger then asking synonyms.
    */
    /* Create an array */
    size = (node->qnum_ant + node->qnum_syn);
    
    /* Fill that array with 0 or 1 (odd or even) inversely proportional with number of questions */
    shuffle = (int *)calloc(size, sizeof(int));
    for (i = 0; i < node->qnum_syn; i++) shuffle[i] = 1;
    for (j = i; j < size; j++) shuffle[j] = 0;
    
    /* Find the index and make free the memory and return index */
    i = rand() % size;
    i = shuffle[i];
    free(shuffle);
    return i;
}

bool Is_word_correspond(wordlist_t *list_head, char *base, char *word)
{
    word_t *head;           /* Initiliazers */
    bool flag = false;

    /* Check from base word, synonyms or antonyms and return true if match happen 
       Also print the correct answers */
    printf("\nAnswers: ");
    while (strcmp(list_head->baseword, base) != 0) list_head = list_head->next_baseword;
    for (head = list_head->correspond; head != NULL; head = head->next_word)
    {
        if (strcmp(head->word, word) == 0) flag = true;
        printf("%s  ", head->word);
    }
    printf("\n");
    return flag;
}

void Add_new_word(wordlist_t *list_head, char *base, char *word)
{
    /* If user wants to add his/her answer as a correct answer then find the node and add it */
    word_t *new_word = NULL;
    for (; list_head != NULL; list_head = list_head->next_baseword)
    {
        if (strcmp(list_head->baseword, base) == 0) break;
    }
    new_word = Create_word_Node(word);

    new_word->next_word = list_head->correspond;
    list_head->correspond = new_word;
}

int Find_lcm(userrecord_t *list_head)
{
    /* Find Least Common Multiple of the list */
    int lcm = 0;
    lcm = (list_head->qnum_ant + list_head->qnum_syn);
    list_head = list_head->next_word;
    for (; list_head != NULL; list_head = list_head->next_word)
    {
        lcm = ((list_head->qnum_syn + list_head->qnum_ant) * lcm) /
               (gcd((list_head->qnum_syn + list_head->qnum_ant), lcm)); 
    }
    return lcm;
}

int gcd(int a, int b)
{
    /* Find greatest common divisor of the 2 number */
    if (b == 0) return a; 
    return gcd(b, a % b);
}
/* ============== Possibility calculations ============== */

/* ============== Updating user records and list ============== */
filereadtype Update_userrecord(userrecord_t *list_head, char *username)
{
    FILE *fp = NULL;                    /* Initiliaze */
    char filename[WORDLENGTH + 8];      

    /* Find size of struct without pointer */
    size_t size = 0;
    size = sizeof(userrecord_t) - sizeof(list_head->next_word);
    
    /* Re-create username format */
    strcpy(filename, username);
    strcat(filename, ".worddat");
    filename[strlen(username) + 8] = '\0';

    fp = fopen(filename, "wb");         /* Open file with binary mode */
    if (fp == NULL)
    {
        printf("The %s file could not be opened!\n", filename);
        return error;
    }
    while (list_head != NULL)           /* Until the end of the write informations to the binary file */
    {
        fwrite(list_head, size, 1, fp);
        list_head = list_head->next_word;
    }
    fclose(fp);
    return success;
}

filereadtype Update_wordlist(wordlist_t *list_head, char *file_name)
{
    FILE *outfile;                      /* Initilizers */
    word_t *head = NULL;

    outfile = fopen(file_name, "w");    /* Open file with writing mode */
    if (outfile == NULL)
    {
        printf("The %s file could not be opened!\n", file_name);
        return error;
    }
    /* Update list with informations */
    for (; list_head != NULL; list_head = list_head->next_baseword)
    {
        fprintf(outfile, "%s %s ", list_head->baseword, list_head->ID);
        head = list_head->correspond;
        for (; head->next_word != NULL; head = head->next_word)
        {
            fprintf(outfile, "%s ", head->word);
        }
        fprintf(outfile, "%s\n", head->word);
    }
    fclose(outfile);
    return success;
}
/* ============== End of the updatings ============== */


/* ========= Informatins for legends ========= */
int Find_correct_number(userrecord_t *list_head)
{
    /* Check end of the file and find number of correct answers */
    int sum = 0;
    for(; list_head != NULL; list_head = list_head->next_word)
    {
        sum += (list_head->corr_ant + list_head->corr_syn);
    }
    return sum;
}

int Find_wrong_number(userrecord_t *list_head)
{
    int sum = 0;
    /* Check end of the file and find number of wrong answers */
    for(; list_head != NULL; list_head = list_head->next_word)
    {
        sum += (list_head->wron_ant + list_head->wron_syn);
    }
    return sum;
}

int Find_word_number(userrecord_t *list_head)
{
    int sum = 0;
    /* Check end of the file and find number of words */
    for(; list_head != NULL; list_head = list_head->next_word) sum++;
    
    return sum;
}
/* ========= End of the legends ========= */


/* ============== Free the memory ============== */
void Free_wordlist(wordlist_t *list_head)
{
    wordlist_t *temp = NULL;
    word_t *head_wt = NULL, *temp_wt = NULL;

    while (list_head != NULL)
    {
        temp = list_head;
        head_wt = temp->correspond;
        while (head_wt != NULL)
        {
            temp_wt = head_wt;
            head_wt = head_wt->next_word;
            if (temp_wt != NULL) 
            {
                free(temp_wt->word);
                free(temp_wt);
            }
        }
        list_head = list_head->next_baseword;
        if (temp != NULL) 
        {
            free(temp->baseword);
            free(temp->ID);
            free(temp);
        }
    }
}

void Free_user(userrecord_t *list_head)
{
    userrecord_t *temp = NULL;
    while (list_head != NULL)
    {
        temp = list_head;
        list_head = list_head->next_word;
        if (temp != NULL) free(temp);
    }
}
/* ============== End of free the memory ============== */

/* ============================ END OF THE PROGRAM ============================ */
