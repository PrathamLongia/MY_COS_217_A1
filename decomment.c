
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

/* Defines constants that represent each state in the DFA */
enum StateType {NORMAL, IN_STRING, IN_CHAR, IN_ESCAPED_CHAR, 
IN_ESCAPED_STRING, START_COMMENT, IN_COMMENT, END_COMMENT};

/* Implement the NORMAL state of the DFA. 
c is the current DFA's character.  
Write c, or don't if c begins a comment. 
Return next state specificied by the DFA. */
enum StateType handleNormalState (int c)
{
   enum StateType state;
   if(c == '\"')
   {
      putchar(c);
      state = IN_STRING;
   } else if (c == '\''){
      putchar(c);
      state = IN_CHAR;
   } else if (c == '/'){
      state = START_COMMENT;
   } else {
      putchar(c);
      state = NORMAL;
   }
   return state;
}

/* Implement the IN_STRING state of the DFA. 
c is the current DFA's character.  Write c.
Return next state specificied by the DFA. */
enum StateType handleInStringState (int c)
{
   enum StateType state;
   if(c == '\"')
   {
      putchar(c);
      state = NORMAL;
   } else if (c == '\\'){
      putchar(c);
      state = IN_ESCAPED_STRING;
   } else {
      putchar(c);
      state = IN_STRING;
   }
   return state;
}

/* Implement the IN_CHAR state of the DFA. 
c is the current DFA's character.  Write c.
Return next state specificied by the DFA. */
enum StateType handleInCharState (int c)
{
   enum StateType state;
   if(c == '\'')
   {
      putchar(c);
      state = NORMAL;
   } else if (c == '\\'){
      putchar(c);
      state = IN_ESCAPED_CHAR;
   } else {
      putchar(c);
      state = IN_CHAR;
   }
   return state;
}

/* Implement the START_COMMENT state of the DFA. 
c is the current DFA's character.  Write c, unless c is a star.
Return next state specificied by the DFA. */
enum StateType handleStartCommentState (int c)
{
   enum StateType state;
   if(c == '/')
   {
      putchar(c);
      state = START_COMMENT;
   } else if (c == '\"') {
      putchar('/');
      putchar('\"');
      state = IN_STRING;
    } else if (c == '\'') {
      putchar('/');
      putchar('\'');
      state = IN_CHAR;
    } else if (c == '*') {
      putchar(' ');
      state = IN_COMMENT;
   } else {
      putchar('/');
      putchar(c);
      state = NORMAL;
   }
   return state;
}

/* Implement the IN_ESCAPED_STRING state of the DFA. 
c is the current DFA's character.  Write c.
Return to IN_STRING state. */
enum StateType handleInEscapedStringState (int c)
{
   enum StateType state;
   putchar(c);
   state = IN_STRING;
   return state;
}

/* Implement the IN_ESCAPED_CHAR state of the DFA. 
c is the current DFA's character.  Write c.
Return to IN_CHAR state. */
enum StateType handleInEscapedCharState (int c)
{
   enum StateType state;
   putchar(c);
   state = IN_CHAR;
   return state;
}

/* Implement the IN_COMMENT state of the DFA. 
c is the current DFA's character.  
Ignore c, unless it's a newline or ends the comment.
Return to the state specificied by the DFA. */
enum StateType handleInCommentState (int c) 
{
   enum StateType state;
   if(c == '\n')
   {
      putchar(c);
      state = IN_COMMENT;
   } else if (c == '*') {
      state = END_COMMENT;
   } else {
      state = IN_COMMENT;
   }
   return state;
}

/* Implement the END_COMMENT state of the DFA. 
c is the current DFA's character.
Return to the state specificied by the DFA. */
enum StateType handleEndCommentState (int c)
{
    enum StateType state;
   if(c == '\n')
   {
      putchar(c);
      state = IN_COMMENT;
   } else if (c == '*') {
      state = END_COMMENT;
   } else if (c == '/') {
      state = NORMAL;
   } else {
      state = IN_COMMENT;
   }
   return state;
}

/* Implement the DFA. Reads file from stdin. Removes comments. 
Returns error if file ends on an unterminated comment.*/
int main(void) {
    int c;
    int lastCommentLine;
    int line = 1;
    enum StateType state = NORMAL;
    while ((c = getchar()) != EOF) {
        if (c == '\n'){
         line++;
        }
        if (c == '*' && state == START_COMMENT){
         lastCommentLine = line;
        }
        switch (state) {
            case NORMAL:
                state = handleNormalState(c);
                break;
            case IN_STRING:
                state = handleInStringState(c);
                break;
            case IN_CHAR:
                state = handleInCharState(c);
                break;
            case START_COMMENT:
                state = handleStartCommentState(c);
                break;
            case IN_ESCAPED_STRING:
                state = handleInEscapedStringState(c);
                break;
            case IN_ESCAPED_CHAR:
                state = handleInEscapedCharState(c);
                break;
            case IN_COMMENT:
                state = handleInCommentState(c);
                break;
            case END_COMMENT:
                state = handleEndCommentState(c);
                break;
                
        }
    }
    switch (state) {
      case START_COMMENT:
         putchar('/');
      case IN_COMMENT:
         fprintf(stderr, "Error: line %d: unterminated comment\n", lastCommentLine);
         exit(EXIT_FAILURE);
      case END_COMMENT:
         fprintf(stderr, "Error: line %d: unterminated comment\n", lastCommentLine);
         exit(EXIT_FAILURE);
      default:
         exit(EXIT_SUCCESS);
    }
}