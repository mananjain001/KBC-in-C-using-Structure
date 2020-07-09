#include<stdio.h>
#include<stdlib.h>
#include<ctype.h> // For using toupper function.
#include<unistd.h> // Because for changing terminal local mode we need to use I/O file descriptors .. here STDIN_FILENO and as read function is also used(internally).
#include<termios.h> //To change the terminal local mode from canonical to noncanonical
#include"ques.h" //This header file contains questions to be used in the game.
int fifty_fifty,flip_ques,count,sum,cls,q_no; 
int lifeline;
/*  > fifty_fifty is used for storing whether fifty fifty lifeline used or not.
    > flip_ques is used for storing whether flip a question lifeline is used or not.
    > count keep track of the question player is currently on.
    > sum keeps track of the total win of the palyer.
    > cls is used to close the game (
        1.When non specified input is entered by player.
        2.When player gives wrong answer.
        3.When player doesnt want to continue palying the game.
        4.When player wins the game.
    )
    > q_no is used to track the question player is currently on .. it is different from count as count is increased by 1 whenever player gives correct answer.
    > lifeline keeps track of the number of lifeline(s) player is left with.

*/
int game_start(){
    //This function is used to start game when any key is pressed.
    int start;
    struct termios pre,post; //Declaring termios struct variables pre and post..so that we can return to our initial terminal local mode(here canonical) after entering any key.
  
    tcgetattr(STDIN_FILENO,&pre); // Storing initial terminal local mode in pre.
    post = pre; //So that we can bring change in current terminal local mode
    post.c_lflag &= ~(ICANON | ECHO); // Used for changing the mode from canonical  and echo(showing input on screen) to non canonical and non echo.
    tcsetattr(STDIN_FILENO,TCSANOW,&post); // Changing the mode by setting the changes done in post.TCSANOW is used so that the changes take place immediately.
    start = getchar(); // This is for taking input.
    tcsetattr(STDIN_FILENO,TCSANOW,&pre); // Changing the terminal local mode to initial (pre) state immediately.
  
    return start;
    
}

void clear(){
    //To give better user interface this printf will clear the console screen every time it is called.
    printf("\e[1;1H\e[2J");
}

int response(int n){
    int prize;
    int k =n;
    
    printf("Question %d : \t%s \n",++q_no,id[k].q);
    printf("Option\t 1 : \t%s \n",id[k].op_1);
    printf("Option\t 2 : \t%s \n",id[k].op_2);
    printf("Option\t 3 : \t%s \n",id[k].op_3);
    printf("Option\t 4 : \t%s \n",id[k].op_4);
    printf("Do you know the answer (Y / N / y / n) ?\n");
    char y_n;
    scanf(" %c",&y_n);
    if(toupper(y_n) == 'Y'){
        //If player knows the answer then he is directly asked whether he wants to continue the game or not.
        goto con;
    }
LIFE :  if(toupper(y_n) == 'N'){
            if(lifeline<2){
                //This will run only if any of his lifeline(s) is left.
                printf("Do you want to use a lifeline (Y / N / y / n) ?\n");
                char ch;
                scanf(" %c",&ch);
                if(toupper(ch) == 'Y'){
                    if((fifty_fifty == 0)||(flip_ques == 0)){
                        //This will run only if any of his lifeline(s) is left.
                        int life;

                        if(fifty_fifty == 0){
                            printf("For Fifty Fifty Lifeline\t:\t(Press 5) \n");
                            if(flip_ques == 0){
                                printf("For Flip A Question Lifeline\t:\t(Press 6) \n");
                            }
                        }
                        else if(flip_ques == 0){
                            printf("For Flip A Question Lifeline : (Press 6) \n");
                        }   
                        scanf("%d",&life);
                        if(life == 5){
                            //If player wants to use Fifty Fifty lifeline.
                            printf("Option : %d\t%s\n",id[k].corr,id[k].op_corr);
                            printf("Option : 4\t%s\n",id[k].op_4);
                            lifeline++;
                            fifty_fifty++;
                            printf("Do you know the answer (Y / N / y / n) ? \n");
                            char y_n1; // taking answer for above asked question.
                            scanf(" %c",&y_n1);
                            if(toupper(y_n1) == 'Y'){
                                goto conti;//If player enters y or Y s/he is asked to enter an option from question. 
                            }
                            else if(toupper(y_n1) == 'N'){
                                //In case player wants to use his other lifeline.
                                goto LIFE;
                            }
                            else{
                                //This is executed when non specified input is given by player.
                                cls =1;
                                goto end;
                            }
                        }
                        else if(life == 6){
                            //If player wants to use flip the question lifeline.
                            k= 17;
                            printf("Question  : \t%s \n",id[k].q);
                            printf("Option\t1 : \t%s \n",id[k].op_1);
                            printf("Option\t2 : \t%s \n",id[k].op_2);
                            printf("Option\t3 : \t%s \n",id[k].op_3);
                            printf("Option\t4 : \t%s \n",id[k].op_4);
                            lifeline++;
                            flip_ques++;
                            printf("Do you know the answer (Y / N / y / n) ?\n");
                            // Taking input from player on asked ques.
                            char y_n2;
                            scanf(" %c",&y_n2);
                            if(toupper(y_n2) == 'Y'){
                                goto conti;//If player enters y or Y s/he is asked to enter an option from new question. 
                            }
                            else if(toupper(y_n2) == 'N'){
                                //In case player wants to use his other lifeline.
                                goto LIFE;
                            }
                            else{
                                //This is executed when non specified input is given by player.
                                cls = 1;
                                goto end;
                            }
                        }
                    }               
                }
                else if(toupper(ch) == 'N'){
                    goto con; // It will ask the player whether he/she want to continue.
                }
                else{
                    //This is executed when non specified input is given by player.
                    cls=1;
                    goto end;
                }
            }
            else{
                //If player doesnt have any lifelines left.
                printf("You Dont have any Lifeline Left\n\n");
                goto con;
            }
        }
        if((toupper(y_n) != 'Y') && (toupper(y_n) != 'N')){
            //This is executed when non specified input is given by player.
            cls = 1;
            goto end;
        }

con:    printf("Do you Want to continue (Y / N / y / n) ?\n");
        //This is included in case player dont want to play game any more.
        char choice;                        //Taking input in response to ques. asked.
        scanf(" %c",&choice);
        if(toupper(choice) == 'Y'){
            goto conti;     //It will directly ask the player to give ans.
        }
        else{               //If player doesnt want to play .
            printf("\nThanks for playing with us.\n");
            printf("\nYour total win is : Rs.%d\n",sum);
            cls = 1;
            goto end;

        }

conti:  printf("Type in your Option (1 / 2 / 3 / 4) : ");
    
        int ans;                            //variable which will store answer of player. 
        scanf("%d",&ans);

        if(ans == id[k].corr){              //If answer given by player is correct.
            count ++;                       //Increasing the count variable by 1.
        
            switch(count){                  
                //Using switch statement to denote the prize for player according to question no.            
                case 1 : 
                    prize = 5000;
                    sum = sum + prize;
                    break;

                case 2 : 
                    prize = 5000;
                    sum = sum + prize;
                    break;
            
                case 3 : 
                    prize = 10000;
                    sum = sum + prize;
                    break;
            
                case 4 : 
                    prize = 20000;
                    sum = sum + prize;
                    break;
            
                case 5 : 
                    prize = 40000;
                    sum = sum + prize;
                    break;
            
                case 6 : 
                    prize = 80000;
                    sum = sum + prize;
                    break;
            
                case 7 : 
                    prize = 160000;
                    sum = sum + prize;
                    break;
            
                case 8 : 
                    prize = 320000;
                    sum = sum + prize;
                    break;
            
                case 9 : 
                    prize = 610000;
                    sum = sum + prize;
                    break;
            
                case 10 : 
                    prize = 1250000;
                    sum = sum + prize;
                    break;
            
                case 11 : 
                    prize = 2500000;
                    sum = sum + prize;
                    break;
            
                case 12 : 
                    prize = 5000000;
                    sum = sum + prize;
                    break;
            
                case 13 : 
                    prize = 20000000;
                    sum = sum + prize;
                    break;
            
                case 14 : 
                    prize = 20000000;
                    sum = sum + prize;
                    break;
            
                case 15 : 
                    prize = 20000000;
                    sum = sum + prize;
                    cls = 1;    //Initialising cls to 1 so that game doesnt run after 15 ques.
                    printf("\nWow You Have Won This Game !! \n");
                    break;
       
            }
            printf("\nCongratulations ! \n");
            printf("Your total win is : Rs.%d \n\n",sum);
        }
        else if(ans != id[k].corr){
            printf("\nWrong Answer !\n");
            printf("\nYour journey ends here\n");
            if(count <= 5){                         //Setting the condition for intermediate level 0
                printf("\nYour total win is : Rs.%d \n",sum);
            }
            
            else if((count >5) && (count <=8)){     //Setting the condition for intermediate level 1
                printf("\nYour total win is : Rs.80000\n");
            }
            else if((count >8) && (count <= 12)){   //Setting the condition for intermediate level 2
                printf("\nYour total win is : Rs.640000\n");
            }
            else if(count > 12){                    //Setting the condition for intermediate level 3
                printf("\nYour total win is : Rs.10000000\n");
            }
        
            cls =1;                                 

        }
    

end : return 0;

}

int main(){
    printf("\n\n");
    printf("Welcome to the game of KBC .\n");
    
    int n=0;
    printf("\n\n");
    //RULES AND REGULATIONS :
    printf("These are the prizes according to question numbers.\n");
    printf("Question 1  : Prize : Rs.5000\n");
    printf("Question 2  : Prize : Rs.10000\n");
    printf("Question 3  : Prize : Rs.20000\n");
    printf("Question 4  : Prize : Rs.40000\n");
    printf("Question 5  : Prize : Rs.80000\t \t \t \tIntermediate Level 1\n");
    printf("Question 6  : Prize : Rs.160000\n");
    printf("Question 7  : Prize : Rs.320000\n");
    printf("Question 8  : Prize : Rs.640000\t \t \t \tIntermediate Level 2\n");
    printf("Question 9  : Prize : Rs.1250000\n");
    printf("Question 10 : Prize : Rs.2500000\n");
    printf("Question 11 : Prize : Rs.5000000\n");
    printf("Question 12 : Prize : Rs.10000000\t \t \tIntermediate Level 3\n");
    printf("Question 13 : Prize : Rs.30000000\n");
    printf("Question 14 : Prize : Rs.50000000\n");
    printf("Question 15 : Prize : Rs.70000000\n");
    printf("\n\n");
    printf("Rule 1: You have to select an option(1/2/3/4) for a given question.\n");
    printf("Rule 2: You have two lifelines 50-50 and flip a question . These can be used only once and both can be used one after the other.\n");
    printf("Rule 3: If you give input which is not specified then game will end abruptly with you taking away the prize money of previous question.\n");
    
    printf("\n\n");
    printf("50-50 Lifeline will remove two incorrect options for a given question.\n");
    printf("Flip A Question Lifeline will give you a new question.\n");
    printf("\n\n");
    printf("If you give wrong answer to a question you will fall to the intermediate level.\n");
    printf("Intermediate Level : 0 : Till Question 5 your win will be the prize of previous question correctly answered.\n");
    printf("Intermediate Level : 1 : If you reach till question 8 and give wrong answer then your total win will be Rs.80000\n");
    printf("Intermediate Level : 2 : If you reach till question 12 and give wrong answer then your total win will be Rs.640000\n");
    printf("Intermediate Level : 3 : If you reach beyond question 12 and give wrong answer then your total win will be Rs.10000000\n");
    printf("\n");
    printf("If you quit the game (without giving wrong answer) then your total win will be the prize of previous question correctly answered.\n\n");
    printf("Press any key to start the game .\n");

    game_start();   //Starting the game by pressing any key.
    clear();
repeat: n++;
        response(n);
        
        if(cls != 1){
            printf("Do you Want to continue (Y / N / y / n) ?  \n");
            char choice_1;  //Taking input for the question asked.      
            scanf(" %c",&choice_1);
            clear();
            if(toupper(choice_1) == 'Y'){
                goto repeat;               
                
            }
            else{
                //In case player doesnt want to continue playing game.
                printf("\nThanks for playing with us.\n");
                printf("\nYour total win is : Rs.%d\n",sum);
                cls = 1;
                
            }
        }

return 0;
    
}
