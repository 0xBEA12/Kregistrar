#include <stdio.h>
#include <stdlib.h>
#include <string.h>
////////////////////////////////////////////////////////       
typedef struct ucfClass 
{
  char ID[10]; // class ID, ex: "COP-3502"
  char days[6]; // a combination of "MTWRF" (days)
  char time[20]; // class time, ex: "10:30 AM - 12:00 PM"
} class;
////////////////////////////////////////////////////////
typedef struct ucfStudent 
{
  char lastName[21]; // student last name
  char firstName[21]; // student first name
  int ID; // student ID
  int laptopID; // serial number of the laptop the student picks up
  int errorCode; // flag to determine if they will make mistakes
  int numClasses; // number of classes the student will register
  class *classes; // array of said classes (2 be dynamically allocated)
  int enterTime; // time student arrived, in minutes, after 12:00 PM
  int timeLeft; // countdown timer to measure the 5 min. reg. process
  int timeRegistered; // Time student finished reg. and left Registrar
  struct ucfStudent *next; // pointer to next student in queue
} student;
////////////////////////////////////////////////////////
typedef struct stack
{
  int* laptopArray;
  int top;
} stack;
////////////////////////////////////////////////////////
typedef struct Queue
{
  student *head, *tail;
} queue;
////////////////////////////////////////////////////////
//Stack functions
void StackInitialize(stack* stackPtr, int numLaptops)
{
  stackPtr->top = -1;
  stackPtr->laptopArray = (int*)malloc(numLaptops * sizeof(int));
}
////////////////////////////////////////////////////////
int StackEmpty(stack* stackPtr) 
{ 
  return (stackPtr->top == -1);
}
////////////////////////////////////////////////////////
int StackFull(stack* stackPtr, int stackSIZE) 
{
  return (stackPtr->top == stackSIZE - 1);
}
////////////////////////////////////////////////////////
int StackPush(stack* stackPtr, int value, int stackSIZE) 
{
 if(StackFull(stackPtr, stackSIZE)) return 0;
 
 stackPtr->laptopArray[stackPtr->top + 1] = value;
 (stackPtr->top)++;

 return 1; 
}
////////////////////////////////////////////////////////
int StackPop(stack* stackPtr) 
{ 
  int retVal;

  if(StackEmpty(stackPtr)) return -1;

  retVal = stackPtr->laptopArray[stackPtr->top]; 

  (stackPtr->top)--; 
  return retVal; 
}
////////////////////////////////////////////////////////
int StackTop(stack* stackPtr) 
{ 
  if(StackEmpty(stackPtr)) return -1; 

  return stackPtr->laptopArray[stackPtr->top]; 
}
////////////////////////////////////////////////////////
//Queue functions
void QueueInitialize(queue* queuePtr)
{
  queuePtr->head = NULL;
  queuePtr->tail = NULL;
}
////////////////////////////////////////////////////////
int Enqueue(queue* queuePtr, student* nextStudent)
{
  student* tempPtr = NULL;
  //if queue is empty
  if(queuePtr->tail == NULL)
  {
    queuePtr->head = nextStudent;
    queuePtr->tail = nextStudent;
    queuePtr->tail->next = NULL;
  }
  //if queue is not empty
  else if(queuePtr->tail != NULL)
  {
    //queuePtr->tail = nextStudent;
    queuePtr->tail->next = nextStudent;
    queuePtr->tail = nextStudent;
    queuePtr->tail->next = NULL;
  }
  return 0;
}
////////////////////////////////////////////////////////
student* Dequeue(queue* queuePtr)
{
  student* temp;

//if queue is empty cannot dqueue
  if(queuePtr == NULL)
  {
    return NULL;
  }
  //if queue is not empty
  else // queuePtr != NULL
  {
    temp = queuePtr->head;
    queuePtr->head = queuePtr->head->next;
    //if queue has only one element and will be empty
    //after dequeuing
    if(queuePtr->head == NULL)
    {
      queuePtr->tail = NULL;
    }
    return temp;
  }
}
////////////////////////////////////////////////////////
int Front1(queue* queuePtr)
{
  if(queuePtr->head != NULL)
  {
    return queuePtr->head->enterTime;
  }
  else 
  {
    return -1;
  }
}
////////////////////////////////////////////////////////
int Front2(queue* queuePtr)
{
  return queuePtr->head == NULL;
}
////////////////////////////////////////////////////////
void PrintTime(FILE* KRO, int time)
{
  int minToHour = 60;
  int hour = time / minToHour;
  if(hour == 0)
  {
    hour = 12;
  }
  int minutes = time % minToHour;
  fprintf(KRO, "%d:%0.2d PM:  ", hour, minutes);
}
////////////////////////////////////////////////////////
student* ListSort(student* studentReturning, student* list)
{
  student* currentList = list;

  if (currentList == NULL)
  {
    studentReturning->next = NULL;
    return studentReturning; // return or do something
  }

  if (currentList->next == NULL)
  {
    // compare and place item
    int ret1 = (strcmp(currentList->lastName, studentReturning->lastName));
    int ret2 = (strcmp(currentList->firstName, studentReturning->firstName));

    if (ret1 > 0 || (ret1 == 0 && ret2 > 0))
    {
      // Goes before
      studentReturning->next = currentList;
      return studentReturning;
    }
    else
    {
      // studentReturning goes after
      currentList->next = studentReturning;
      studentReturning->next = NULL;
      return currentList;
    }
  }

  while(currentList->next != NULL)
  {
    //////*currentList < studentreturning < currentList->next*/
    int ret1 = (strcmp(currentList->next->lastName, studentReturning->lastName));
    int ret2 = (strcmp(currentList->next->firstName, studentReturning->firstName));

    int shouldWeInsert;
    if (ret1 > 0 || ret1 == 0 && ret2 > 0)
      shouldWeInsert = 1;
    else
      shouldWeInsert = 0;


    if (shouldWeInsert == 1)
    {
      // insert between currentList and currentList->next
      student* temp = currentList->next;
      currentList->next = studentReturning;
      studentReturning->next = temp;
      return list;
    }

    currentList = currentList->next;
  }

  // insert at end of currentList
  currentList->next = studentReturning;
  studentReturning->next = NULL;

  return list;
}

void PrintDRP(FILE* KRO, student* list, int days, int numStudents)
{
  student* tempList2 = list;

  fprintf(KRO, "\n*** Day %d:  UCF Daily Registration Report ***:\n\n", days);
   fprintf(KRO, "The Registrar received %d registrations as follows:\n\n", numStudents);

   while(tempList2 != NULL)
   {
      fprintf(KRO, "%s, %s, ID # %d\n", tempList2->lastName, tempList2->firstName, tempList2->ID);
      fprintf(KRO, "\tTime Registered:  ");
      PrintTime(KRO, tempList2->timeRegistered);
      fprintf(KRO, "\n");
      fprintf(KRO, "\tClasses:\n");

      for(int m = 0; m < tempList2->numClasses; m++)
      {
        fprintf(KRO, "\t| %-8s | %-5s | %-19s |\n", tempList2->classes[m].ID, tempList2->classes[m].days, tempList2->classes[m].time);
      }

      tempList2 = tempList2->next;
   }

   fprintf(KRO, "\n\n");
}

void FreeStudentsList(student* head)
{
  student* curr = head;
  student* next;
  while (curr != NULL)
  {
    next = curr->next;
    free(curr->classes);
    free(curr);
    curr = next;
  }
}
                    
////////////////////////////////////////////////////////
int main(void) 
{
  //initialize everything
  FILE* KRI = fopen("KnightsRegistrar.in", "r");
  FILE* KR1 = fopen("KnightsRegistrar.out", "r");
  FILE* KR2 = fopen("KR2", "r");
  FILE* KRO = fopen("KnightsRegistrar.out", "w");
  stack laptops;
  queue* outsideLine = (queue*)malloc(sizeof(queue));
  QueueInitialize(outsideLine);
  queue* laptopWaitingLine = (queue*)malloc(sizeof(queue));
  QueueInitialize(laptopWaitingLine);
  queue* registering = (queue*)malloc(sizeof(queue));
  QueueInitialize(registering);
  queue* laptopReturnLine = (queue*)malloc(sizeof(queue));
  QueueInitialize(laptopReturnLine);
  student* nextStudent = NULL;
  student* tempStudent = NULL;
  student* tempStudent2 = NULL;
  student* tempStudent3 = NULL;
  student* studentSettingUp = NULL;
  student* studentReturning = NULL;
  student* current = NULL;
  student* list = NULL;
  int numLaptops, laptopID, days, numStudents, studentsLeft, time = 0, x = 0;

  //adds laptops to stack
  fscanf(KRI, "%d", &numLaptops);
  StackInitialize(&laptops, numLaptops);
  for(int i = 0; i < numLaptops; i++)
  {
    fscanf(KRI, "%d", &laptopID);
    StackPush(&laptops, laptopID, numLaptops);
  }

  //loops through each day's initialization, simulation,
  //and daily report
  fscanf(KRI, "%d", &days);
  for(int j = 0; j < days; j++)
  {
    fprintf(KRO, "**********\nDay %d:\n**********\n\n", j+1);
    //initial daily loop to initialize all students
    //and their classes info
    fscanf(KRI, "%d", &numStudents);
    for(int k = 0; k < numStudents; k++)
    {
      //queues students into outside line
      nextStudent = (student*)malloc(sizeof(student));
      fscanf(KRI, "%d %s %s %d %d %d", &nextStudent->enterTime, nextStudent->lastName, nextStudent->firstName, &nextStudent->ID, &nextStudent->numClasses, &nextStudent->errorCode);
      Enqueue(outsideLine, nextStudent);

      //scans in info for student's classes array
      nextStudent->classes = (class*)malloc(nextStudent->numClasses * sizeof(class));
      for(int l = 0; l < nextStudent->numClasses; l++)
      {
        fscanf(KRI, "%s %s ", nextStudent->classes[l].ID, nextStudent->classes[l].days);
        fgets(nextStudent->classes[l].time, 20, KRI);
        strtok(nextStudent->classes[l].time, "\n");
      }
      
      //nextStudent->next = NULL;
    }

    //simulation is executed through while loop
    time = 0;
    studentsLeft = numStudents;
    while(studentsLeft > 0)
    {
      //reset each tempStudent on every tick
      tempStudent = NULL;
      tempStudent2 = NULL;
      tempStudent3 = NULL;

//step #7.2 studentReturning-->tempStudent3
      //if student has error code 1 use this part of step #7
      if(studentReturning != NULL && studentReturning->errorCode == 1)
      {
        PrintTime(KRO, time);
        fprintf(KRO, "%s %s made an error and must redo the registration.\n", studentReturning->firstName, studentReturning->lastName);
        studentReturning->errorCode = 0;
        studentReturning->timeLeft = 5;
        Enqueue(registering, studentReturning);
        studentReturning = NULL;
        }
//step #7.1 studentReturning-->list
      //if student has error code 0 use this part of step #7 instead
      else if(studentReturning != NULL && studentReturning->errorCode == 0)
      {
        //return laptop and student info is added to registration report
        PrintTime(KRO, time);
        fprintf(KRO, "%s %s successfully registered and returned laptop # %d.\n", studentReturning->firstName, studentReturning->lastName, studentReturning->laptopID);
        StackPush(&laptops, studentReturning->laptopID, numLaptops);
        list = ListSort(studentReturning, list);
        studentReturning = NULL;
        studentsLeft--;
      }

//step #3 studentSettingUp-->registering
      //studentSettingUp must take laptop after studentReturning
      //returns laptop by rule #1 and before registration is simulated
      if (studentSettingUp != NULL)
      {         
        // Let the student register after using LDM
        studentSettingUp->laptopID = StackPop(&laptops);
        PrintTime(KRO, time);
        fprintf(KRO, "%s %s has checked-out laptop # %d.\n", studentSettingUp->firstName, studentSettingUp->lastName, studentSettingUp->laptopID);
        studentSettingUp->timeLeft = 5;
        Enqueue(registering, studentSettingUp);
        studentSettingUp = NULL;
      }

//step #4 registering-->(tempStudent2 && tempStudent3)
      if(!Front2(registering))
      {
        //if student has spent 5 minutes registering
        //temporarily store them for step 5
        if(registering->head->timeLeft == 0)
        {
          tempStudent2 = Dequeue(registering);
          if(!Front2(registering))
          {
            if(registering->head->timeLeft == 0)
            {
              tempStudent3 = Dequeue(registering);
            }
          }
        }
        
        //for the students who have not registered for 5 minutes
        //iterate through the registering queue and decrement
        //their time registering by 1
        current = registering->head;
        while(current != NULL)
        {
          current->timeLeft--;
          current = current->next;
        }

      }

//step #5 (tempStudent2 && tempStudent3)-->laptopReturnLine
      //if two students finish registering at the same time
      if(tempStudent2 != NULL && tempStudent3 != NULL)
      {
        //give priority to the student who got there first
        if(tempStudent2->enterTime <= tempStudent3->enterTime)
        {
          Enqueue(laptopReturnLine, tempStudent2);
          PrintTime(KRO, time);
          fprintf(KRO, "%s %s finished registering and entered the laptop return line.\n", tempStudent2->firstName, tempStudent2->lastName);
          Enqueue(laptopReturnLine, tempStudent3);
          PrintTime(KRO, time);
          fprintf(KRO, "%s %s finished registering and entered the laptop return line.\n", tempStudent3->firstName, tempStudent3->lastName);
        }
        else
        {
          Enqueue(laptopReturnLine, tempStudent3);
          PrintTime(KRO, time);
          fprintf(KRO, "%s %s finished registering and entered the laptop return line.\n", tempStudent3->firstName, tempStudent3->lastName);
          Enqueue(laptopReturnLine, tempStudent2);
          PrintTime(KRO, time);
          fprintf(KRO, "%s %s finished registering and entered the laptop return line.\n", tempStudent2->firstName, tempStudent2->lastName);
        }
      }
      //if only one student registered in this minute
      else if(tempStudent2 != NULL || tempStudent3 != NULL)
      {        
        if(tempStudent2 != NULL)
        { 
          Enqueue(laptopReturnLine, tempStudent2);
          PrintTime(KRO, time);
          fprintf(KRO, "%s %s finished registering and entered the laptop return line.\n", tempStudent2->firstName, tempStudent2->lastName);
        }
        else if(tempStudent3 != NULL)
        {
          Enqueue(laptopReturnLine, tempStudent3);
          PrintTime(KRO, time);
          fprintf(KRO, "%s %s finished registering and entered the laptop return line.\n", tempStudent3->firstName, tempStudent3->lastName);
        }
      }

//step #6 laptopReturnLine-->studentReturning
      
      if(laptopReturnLine->head != NULL)
      {
        studentReturning = Dequeue(laptopReturnLine);
      }

//Step #1 OutsideLine-->laptopWaitingLine
      //must be simulated after studentSettingUp
      while(Front1(outsideLine) == time)
      {
        tempStudent = Dequeue(outsideLine);
        Enqueue(laptopWaitingLine, tempStudent);
        PrintTime(KRO, time);
        fprintf(KRO, "%s %s has arrived at the Registrar and entered the laptop line.\n", tempStudent->firstName, tempStudent->lastName);
      }
     
//Step#2 laptopWaitingLine-->studentSettingUp
      if(laptopWaitingLine->head != NULL)
      {
        if(!StackEmpty(&laptops) && (studentSettingUp == NULL))
        {
          studentSettingUp = Dequeue(laptopWaitingLine);
        }
      }
      
      //add one minute at the end of each simulation tick
      time++;
    }

    free(outsideLine);
    free(laptopWaitingLine);
    free(laptopReturnLine);
    free(registering);

    PrintDRP(KRO, list, days, numStudents);
    FreeStudentsList(list);
  
  }
  
  return 0;
}
