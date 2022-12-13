#include <stdio.h>
#include <string.h>
#include "document.h"
/* Made by: Faadil A. Shaikh
118836140
fshaikh1 */

/*This function initializes the document to have 0 paragraphs*/
int init_document(Document *doc, const char *name){
  if(doc != NULL && name != NULL && strlen(name) <= MAX_STR_SIZE){
    strcpy(doc -> name, name);
    doc -> number_of_paragraphs = 0;
    return SUCCESS;
    } else {
      return FAILURE;
    }
}

/*This function loops through the entire document and prints out its contents, leaving a gap betwen individual paragraphs*/
int print_document(Document *doc){
  int i,j;
  if(doc != NULL){
    printf("Document name: \"%s\"\n", doc -> name);
    printf("Number of Paragraphs: %d\n", doc -> number_of_paragraphs);
    
    for (i = 0; i < doc -> number_of_paragraphs;i++){
      for (j = 0; j < doc -> paragraphs[i].number_of_lines; j++){
        printf("%s\n", doc -> paragraphs[i].lines[j]);
      }
      
      if (i != doc -> number_of_paragraphs - 1){
        printf("\n");
        }
    }
    return SUCCESS;
  } else {
    return FAILURE;
  }
}

/*This function moves all the paragraphs below the specified paragraph down one space in the array to have another paragraph.*/
int add_paragraph_after(Document *doc, int paragraph_number){
  if (doc != NULL && paragraph_number < MAX_PARAGRAPHS && doc -> number_of_paragraphs >= paragraph_number){
    int i;
    for (i = doc -> number_of_paragraphs; i >= paragraph_number; i--){
      doc -> paragraphs[i + 1] = doc -> paragraphs[i];
    }
    doc -> paragraphs[paragraph_number].number_of_lines = 0;
    doc -> number_of_paragraphs++;
    return SUCCESS;
  } else {
    return FAILURE;
  }
}

/*This function works similarly to the above function but for the individual lines and adds the specified value to the new line rather than leaving it empty*/
int add_line_after(Document *doc, int paragraph_number, int line_number, const char *new_line) {
  if(doc != NULL && doc -> number_of_paragraphs >= paragraph_number && doc -> paragraphs[paragraph_number - 1].number_of_lines < MAX_PARAGRAPH_LINES && doc -> paragraphs[paragraph_number - 1].number_of_lines >= line_number && new_line != NULL){
    int i;
    for (i = doc -> paragraphs[paragraph_number - 1].number_of_lines; i >= line_number; i--){
      
      strcpy(doc -> paragraphs[paragraph_number - 1].lines[i + 1],doc -> paragraphs[paragraph_number - 1].lines[i]);
    }
    strcpy(doc -> paragraphs[paragraph_number - 1].lines[line_number],new_line);
    doc -> paragraphs[paragraph_number - 1].number_of_lines++;
    return SUCCESS;
  } else {
    return FAILURE; 
  }
}

/*This function changes the specified variable to the number of lines in the specified paragraph*/
int get_number_lines_paragraph(Document *doc, int paragraph_number, int *number_of_lines){
  if(doc != NULL && number_of_lines != NULL && paragraph_number <= doc -> number_of_paragraphs){
    *number_of_lines = doc -> paragraphs[paragraph_number - 1].number_of_lines;
    return SUCCESS;
    } else {
      return FAILURE;
    }
}

/*This function calls the add_line_after function, this time putting the number of lines in the paragraph as the specified line to add after*/
int append_line(Document *doc, int paragraph_number, const char *new_line){
  return add_line_after(doc, paragraph_number,doc -> paragraphs[paragraph_number - 1].number_of_lines,new_line);
}

/*This function works in the opposite way to the add_line_after function in that it moves all the lines below the specified line up rather than down*/
int remove_line(Document *doc, int paragraph_number, int line_number){
  if(doc != NULL && paragraph_number <= doc -> number_of_paragraphs && doc -> paragraphs[ paragraph_number - 1].number_of_lines >= line_number ) {
    int i;
    for (i = line_number; i < doc -> paragraphs[ paragraph_number - 1].number_of_lines; i++) {
      strcpy(doc -> paragraphs[paragraph_number - 1].lines[i - 1],doc -> paragraphs[paragraph_number - 1].lines[i]);
    }
    doc -> paragraphs[paragraph_number - 1].number_of_lines--;
    return SUCCESS;
  } else {
    return FAILURE;
    }
}


/*This function takes in the data from an array of strings and saves it into a document, with empty strings indicating a new paragraph begining*/
int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], int data_lines){
  if(doc != NULL && data != NULL && data_lines != 0){
    int i, para_num = 0, line_num = 0;
    doc -> number_of_paragraphs = 1;
    for (i = 0; i < data_lines; i++){ 
      if(!strcmp(data[i],"")) {
        para_num++;
        line_num = 0;
      } else {
        strcpy(doc -> paragraphs[para_num].lines[line_num],data[i]);
        doc -> paragraphs[para_num].number_of_lines++;
        line_num++;
      }
    }
    doc -> number_of_paragraphs = para_num + 1;
    return SUCCESS;
    } else {
        return FAILURE;
    }
}


/*This function loops through the entire doc trying to find instances of the target string using the strstr function. Then the function makes a new string by concatenating the replacement function into a new line and replacing the original line.*/
int replace_text(Document *doc, const char *target, const char *replacement){
  if(doc != NULL && target != NULL && replacement != NULL){
    int i, j, len = strlen(target);
    char *ptr, *current_position;
    char new_str[MAX_STR_SIZE + 1];

    if (strcmp(replacement, target)){
      for (i = 0; i < doc -> number_of_paragraphs; i++){
        for (j = 0; j < doc -> paragraphs[i].number_of_lines; j++){
          
          ptr = strstr(doc -> paragraphs[i].lines[j],target);
          current_position = doc -> paragraphs[i].lines[j];
          strcpy(new_str,"");
          while(ptr != NULL){
            strncat(new_str,current_position,ptr - current_position);
            strcat(new_str,replacement);
            current_position = ptr + len;
            ptr = strstr(current_position,target);
            /*printf("%s\n",new_str);*/
            }
          strcat(new_str,current_position);
          /*printf("\nFull Thing: %s\n",new_str);*/
          strcpy(doc -> paragraphs[i].lines[j],new_str);
        }
      }
    }
    return SUCCESS;
    } else {
      return FAILURE;
    }
}

/*This function calls the replace_text function, this time haveing replace be a new string of the already highlighted text*/
int highlight_text(Document *doc, const char *target){
  if (target != NULL && doc != NULL){
    char new_str[MAX_STR_SIZE + 1];
    strcpy(new_str,HIGHLIGHT_START_STR);
    strcat(new_str,target);
    strcat(new_str,HIGHLIGHT_END_STR);
    replace_text(doc, target, new_str);
    return SUCCESS;
    } else {
      return FAILURE;
    }
}

/*This function also calls the replace_text function, this time replacing every instance of target with the empty string*/
int remove_text(Document *doc, const char *target){
  return replace_text(doc, target, "");
}

/*This function sets the number of paragraphs in the document to 0, effectively reseting the document*/
int reset_document(Document *doc){
  if(doc != NULL){
    doc -> number_of_paragraphs = 0;
    return SUCCESS;
  } else {
    return FAILURE;
  }
}

/*This file works the same as load document but this time reads in from an inputted file*/
int load_file(Document *doc, const char *filename){
  FILE *doc_input;
  char line[1024], *in, slice[1024];
  int taken, para = 1;
    if((doc != NULL) && (filename != NULL)){
      doc -> number_of_paragraphs = 1;
      doc_input = fopen(filename,"r");
      do{
        in = fgets(line,1024,doc_input);
        taken = sscanf(line, " %s ", slice);
        if (taken == 0){
          para++;
          doc -> number_of_paragraphs++;
          if(para > MAX_PARAGRAPHS){
            fclose(doc_input);
            return FAILURE;
          }
        } else {
          if (append_line(doc, para, in) == FAILURE){
            fclose(doc_input);
            return FAILURE;
          }
        }
        }while(in != NULL);
      fclose(doc_input);
    return SUCCESS;
    } else {
        return FAILURE;
    }
}

/*This file works the same as print document but this time prints it into an inputted file*/
int save_document(Document *doc, const char *filename){
  int i,j;
  FILE *doc_input;
  if((doc != NULL) || (filename != NULL)){
    doc_input = fopen(filename,"w");
    fprintf(doc_input, "Document name: \"%s\"\n", doc -> name);
    fprintf(doc_input, "Number of Paragraphs: %d\n", doc -> number_of_paragraphs);
    
    for (i = 0; i < doc -> number_of_paragraphs;i++){
      for (j = 0; j < doc -> paragraphs[i].number_of_lines; j++){
        fprintf(doc_input, "%s\n", doc -> paragraphs[i].lines[j]);
      }
      
      if (i != doc -> number_of_paragraphs - 1){
        fprintf(doc_input, "\n");
        }
    }
    fclose(doc_input);
    return SUCCESS;
  } else {
    return FAILURE;
  }
}
