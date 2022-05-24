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

void countStudentsAndCourses(const char* fileName, int** coursesPerStudent, int* numberOfStudents)//נדרש לבדוק תקינות
{

	//int sizNameFile = strlen(fileName);strlen(Line);//אורך השרשור במידה וצריך להעביר מיקום קובץ עי העתקת מחרוזות

	FILE* myCLass = fopen(fileName, "rt");
	if (myCLass == NULL) {
		printf("unable to open file");
		exit(1);
	}
	int numOfStudent = 0;
	char Line[1023];

	while ((feof(myCLass)) == 0) //in fail is 0 כל עוד אנחנו בתוך הקובץ ולא בסופו
	{
		fgets(Line, 1023, myCLass);// עובר על שורה ומעדכן את הסמן
		numOfStudent++;
	}
	int i = 0;
	*numberOfStudents = numOfStudent; //תוכן המצביע יהיה מספר הסטודנטים = בתוך הכתובת ישב המספר
	printf("%d", numOfStudent);
	rewind(myCLass);

	*coursesPerStudent = (int*)malloc(numOfStudent * sizeof(int));// מערך דינאמי של מספר הקורסים לכל סטודנט

	while (numOfStudent >= 1)
	{
		fgets(Line, 1023, myCLass);
		(*coursesPerStudent)[i] = countPipes(Line, 1023);
		i++;
		numOfStudent--;
	}
	fclose(myCLass);

}

int countPipes(const char* lineBuffer, int maxCount)//סופרת קורסים נדרש לבדוק תקינות
{
	int countpipe = 0;

	if (lineBuffer == NULL)
		exit(-1);
	if (maxCount <= 0)
		exit(0);

	//int i = 1;//רוצים לקדם מצביע גודל פחות אחד פעמים, כי הפעם הראשונה כבר מוצבעת
	while (*lineBuffer != '\0'/* || i >= maxCount*/)
	{
		if (*lineBuffer == '|')
			countpipe++;
		lineBuffer++;
		//i++;
	}
	return countpipe;//לקחת בחשבון שכאן המצביע מצביע על הבאפר הבא
}


char*** makeStudentArrayFromFile(const char* fileName, int** coursesPerStudent, int* numberOfStudents)
{
	countStudentsAndCourses(fileName, coursesPerStudent, numberOfStudents);//קורא לפונקציה על מנת לעדכן את הנתונים הנדרשים
	FILE* myfle = fopen(fileName, "rt");
	if (myfle == NULL) {
		printf("Unable to open file\n");
		exit(1);
	}//בודק האם ניתן לפתוח את הקובץ

	char* buffer = (char*)malloc(sizeof(char) * 1024);//מערך דינמאי על מנת לשלוט במיקום של השורה

	char*** arrStudent = (char***)malloc(sizeof(char**) * (*numberOfStudents));//הקצאה דינאמית כאורך הסטודנטים

	for (int i = 0; i < (*numberOfStudents); i++)// מערך שרץ כמספר הסטודנטים כמספר השורות
	{
		arrStudent[i] = (char**)malloc(sizeof(char*) * ((*coursesPerStudent)[i]) * 2 + 1);// הקצאה ע
		fgets(buffer, 1024, myfle);

		char* token = NULL;
		token = strtok(buffer, "|");

		arrStudent[i][0] = (char*)malloc(sizeof(char) * (strlen(token) + 1));//נותן את גודל המחרוזת של המידע עבור כל סטודנט
		if (arrStudent[i][0] == NULL) exit(1);
		strcpy(arrStudent[i][0], token);//העתקנו את שם הסטודנט למשתנה חדש שהוקצא בזכרון

		for (int j = 0; j < ((*coursesPerStudent)[i]); j++)
		{

			token = strtok(NULL, ",|\n");
			arrStudent[i][2 * j + 1] = (char*)malloc(sizeof(char) * (strlen(token) + 1));
			if (arrStudent[i][2 * j + 1] == NULL)
				exit(1);
			strcpy(arrStudent[i][2 * j + 1], token);

			token = strtok(NULL, ",|\n");
			arrStudent[i][2 * j + 2] = (char*)malloc(sizeof(char) * (strlen(token) + 1));
			if (arrStudent[i][2 * j + 2] == NULL) exit(1);
			strcpy(arrStudent[i][2 * j + 2], token);

		}
	}
	return arrStudent;
	fclose(myfle);
}

void factorGivenCourse(char** const* students, const int* coursesPerStudent, int numberOfStudents, const char* courseName, int factor)
{
	if (factor < -20 && factor > 20)
		return;

	for (int a = 0; a < numberOfStudents; a++) //יעבור סטודנט סטודנט
	{
		int qours = (coursesPerStudent[a]*2);
		for (int i = 1; i < qours; i =i+2)
		{
			if ((strcmp(courseName, students[a][i]))==0) // מחזיר 1 אם זה הקורס, תא שם הקורס
			{
				int  scor;
				scor = atoi(students[a][i + 1]); //תא הציון'
				scor = scor + factor;
				if (scor >= 100)
					scor = 100;
				else if (scor <= 0)
					scor = 0;
				_itoa(scor, students[a][i + 1], 10);
				
			}

		}

	}
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
	//students = makeStudentArrayFromFile("studentList.txt",&coursesPerStudent,&numberOfStudents);
	FILE* myfle = fopen("studentListTest.txt", "wt");// מוד קריאה וכתיבה תוך כדי דריסה
	if (myfle == NULL) {
		printf("Unable to open file\n");
		exit(1);
	}//בודק האם ניתן לפתוח את הקובץ
	char*** ps = students;
	for (int i = 0; i < (numberOfStudents); i++)// מערך שרץ כמספר הסטודנטים כמספר השורות
	{
		char buffer[1024] = "";
		strcat(buffer, students[i][0]);// מכניס שם לשורה
		free(students[i][0]);

		int a = ((coursesPerStudent[i])*2);
		for (int j = 1; j < a ; j += 2) //הכנסת קורסים
		{
			strcat(buffer, "|");
			strcat(buffer,students[i][j]);//הכנסת שם קורס
			free(students[i][j]);
			strcat(buffer, ",");
			strcat(buffer, students[i][j + 1]);//הכנסת הציון
			free(students[i][j + 1]);
		}
		
		fputs(buffer, myfle);//לא מתבצע 
		fputs("\n", myfle);

	}
	free(students);
	free(coursesPerStudent);
	fclose(myfle);
}

void writeToBinFile(const char* fileName, Student* students, int numberOfStudents)
{
	FILE* mySbinfile = fopen(fileName, "wb");
	if (mySbinfile == NULL)
	{
		printf("Unable to read file");
		exit(1);
	}

	fwrite(&numberOfStudents, sizeof(int), 1, mySbinfile);

	for (int i = 0; i < numberOfStudents; i++)
	{
		fwrite(students, sizeof(students), 1, mySbinfile); //שם שם סטודנט בקובץ
		students++;
	}

	fclose(mySbinfile);
}

Student* readFromBinFile(const char* fileName)
{
	FILE* mySbinfile = fopen(fileName, "rb");
	if (mySbinfile == NULL)
	{
		printf("Unable to read file");
		exit(1);
	}
	Student* buffer=NULL;
	int numofstudents;
	fread(buffer, sizeof(buffer), 1, fileName);
	numofstudents = buffer->numberOfCourses;
	Student* ParrStudent = (Student*)malloc(sizeof(Student) * (numofstudents));
	ParrStudent = buffer;
}

Student* transformStudentArray(char*** students, const int* coursesPerStudent, int numberOfStudents)
{
	 Student* arrStudent = (Student*)malloc(sizeof(Student) * (numberOfStudents));//הקצאה דינאמית כאורך הסטודנטים
	 if (arrStudent == NULL) exit(1);

	for (int i = 0; i < (numberOfStudents); i++)
	{
		strcpy(arrStudent->name, students[i][0]);//מעתיד את שם הסטודנט למבנה
		arrStudent->numberOfCourses = coursesPerStudent[i];
		StudentCourseGrade* pstg = (StudentCourseGrade*)malloc(sizeof(StudentCourseGrade) * (coursesPerStudent[i]));
		if (pstg == NULL) exit(1);

		int a = coursesPerStudent[i]*2;
		for (int j = 1; j < a; j=j+2)
		{
			strcpy(pstg->courseName, students[i][j]);
			pstg->grade = atoi(students[i][j + 1]);
		}
		arrStudent->grades = pstg;
	}
	return(arrStudent);
}
