#define _CRT_SECURE_NO_WARNINGS
/*#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>*/ //uncomment this block to check for heap memory allocation leaks.
// Read https://docs.microsoft.com/en-us/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2019
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct StudentCourseGrade
{
	char courseName[35];
	int grade;
}StudentCourseGrade;

typedef struct Student
{
	char name[35];
	StudentCourseGrade* grades; //dynamic array of courses
	int numberOfCourses;
}Student;


//Part A
void countStudentsAndCourses(const char* fileName, int** coursesPerStudent, int* numberOfStudents);
int countPipes(const char* lineBuffer, int maxCount);
char*** makeStudentArrayFromFile(const char* fileName, int** coursesPerStudent, int* numberOfStudents);
void printStudentArray(const char* const* const* students, const int* coursesPerStudent, int numberOfStudents);
void factorGivenCourse(char** const* students, const int* coursesPerStudent, int numberOfStudents, const char* courseName, int factor);
void studentsToFile(char*** students, int* coursesPerStudent, int numberOfStudents);

//Part B
Student* transformStudentArray(char*** students, const int* coursesPerStudent, int numberOfStudents);
void writeToBinFile(const char* fileName, Student* students, int numberOfStudents);
Student* readFromBinFile(const char* fileName);

int main()
{
	//Part A
	int* coursesPerStudent = NULL;
	int numberOfStudents = 0;
	char*** students = makeStudentArrayFromFile("studentList.txt", &coursesPerStudent, &numberOfStudents);
	factorGivenCourse(students, coursesPerStudent, numberOfStudents, "Advanced Topics in C", +5);
	printStudentArray(students, coursesPerStudent, numberOfStudents);
	//studentsToFile(students, coursesPerStudent, numberOfStudents); //this frees all memory. Part B fails if this line runs. uncomment for testing (and comment out Part B)

	//Part B
	Student* transformedStudents = transformStudentArray(students, coursesPerStudent, numberOfStudents);
	writeToBinFile("students.bin", transformedStudents, numberOfStudents);
	Student* testReadStudents = readFromBinFile("students.bin");

	//add code to free all arrays of struct Student


	/*_CrtDumpMemoryLeaks();*/ //uncomment this block to check for heap memory allocation leaks.
	// Read https://docs.microsoft.com/en-us/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2019

	return 0;
}

void countStudentsAndCourses(const char* fileName, int** coursesPerStudent, int* numberOfStudents)
{
	//add code here
}

int countPipes(const char* lineBuffer, int maxCount)//סופרת קורסים
{
	int countpipe = 0;
	//FILE* studentfile = fopen("studentList.txt", "rt");
	if (lineBuffer == NULL)
		exit(-1);
	if (maxCount <= 0)
		exit(0);
	int i = 1;//רוצים לקדם מצביע גודל פחות אחד פעמים, כי הפעם הראשונה כבר מוצבעת
	while (lineBuffer != '\0' ||  i != maxCount)
	{
		if (lineBuffer == '|')
			countpipe++;
		lineBuffer++;
		i++;
	}
	return countpipe;//לקחת בחשבון שכאן המצביע מצביע על הבאפר הבא
}


char*** makeStudentArrayFromFile(const char* fileName, int** coursesPerStudent, int* numberOfStudents)
{
	//add code here
	//int sizNameFile = strlen(fileName);strlen(Line);//אורך השרשור במידה וצריך להעביר מיקום קובץ עי העתקת מחרוזות
	
	FILE * myClass ;
	myClass = fopen("studentList.txt", "rt");
	//FILE* myClass = fopen(fileName, "rt"); //צריך דרך המחרוזת שמקבלים
	assert(myClass); //  אם מתקבל ערך
	
	int numOfStudent = 0;
	char Line[1023];

	while ((feof(myClass)) == 0) //in fail is 0 כל עוד אנחנו בתוך הקובץ ולא בסופו
	{
		fgets(Line, 1023, myClass);// עובר על שורה ומעדכן את הסמן
		numOfStudent++;
	}

	*numberOfStudents = numOfStudent; //תוכן המצביע יהיה מספר הסטודנטים = בתוך הכתובת ישב המספר
	rewind(myClass);
	int *s;//מצביע למערך בגודל מספר הסטודנטים
	fclose(myClass);
}

void factorGivenCourse(char** const* students, const int* coursesPerStudent, int numberOfStudents, const char* courseName, int factor)
{
	//add code here
}

void printStudentArray(const char* const* const* students, const int* coursesPerStudent, int numberOfStudents)
{
	for (int i = 0; i < numberOfStudents; i++)
	{
		printf("name: %s\n*********\n", students[i][0]);
		for (int j = 1; j <= 2 * coursesPerStudent[i]; j += 2)
		{
			printf("course: %s\n", students[i][j]);
			printf("grade: %s\n", students[i][j + 1]);
		}
		printf("\n");
	}
}

void studentsToFile(char*** students, int* coursesPerStudent, int numberOfStudents)
{
	//add code here
}

void writeToBinFile(const char* fileName, Student* students, int numberOfStudents)
{
	//add code here
}

Student* readFromBinFile(const char* fileName)
{
	//add code here
}

Student* transformStudentArray(char*** students, const int* coursesPerStudent, int numberOfStudents)
{
	//add code here
}
