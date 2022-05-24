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
		(*coursesPerStudent)[i] = countPipes(fgets(Line, 1023, myCLass), strlen(fgets(Line, 1023, myCLass)));
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

	int i = 1;//רוצים לקדם מצביע גודל פחות אחד פעמים, כי הפעם הראשונה כבר מוצבעת
	while (lineBuffer != '\0' || i != maxCount)
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
	countStudentsAndCourses(fileName, coursesPerStudent, numberOfStudents);//קורא לפונקציה על מנת לעדכן את הנתונים הנדרשים
	FILE* myfle = fopen(fileName, "rt");
	if (myfle == NULL) {
		printf("Unable to open file\n");
		exit(1);
	}//בודק האם ניתן לפתוח את הקובץ

	char* buffer = (char*)malloc(sizeof(char) * 1024);//מערך דינמאי על מנת לשלוט במיקום של השורה

	int numofbits = 0;//אורך המערך לכל סטודנט משתנה

	char*** arrStudent = (char***)malloc(sizeof(char**) * (*numberOfStudents));//הקצאה דינאמית כאורך הסטודנטים

	for (int i = 0; i <= (*numberOfStudents); i++)// מערך שרץ כמספר הסטודנטים כמספר השורות
	{
		char** arrInfoStudent = (char**)malloc(sizeof(char*) * ((*coursesPerStudent)[i]) * 2 + 1);// הקצאה ע
		fgets(buffer, 1024, myfle);
		for (int j = 0; j < (*coursesPerStudent[i] * 2 + 1); j++)
		{
			int valName = strlen(strtok(buffer, '|'));//נותן את גודל המחרוזת של המידע עבור כל סטודנט
			int carsure = valName + 1;//סמן הזז בקובץ
			char* elementname = (char*)malloc(sizeof(char) * (valName + 1));//מבצע הקצאה דינאמית עבור שם הסטודנט ברשימה
			if (elementname == NULL) return;
			strcpy(elementname, strtok(buffer, '|'));//העתקנו את שם הסטודנט למשתנה חדש שהוקצא בזכרון
			*arrStudent[i][j] = elementname;//מכניס את השם של הסטודנט לעמודה במערך הסטודנטים

			for (int a = 0; a < (*coursesPerStudent[i]); a++)
			{
				int valNameOfcurse = strlen(strtok(buffer[+carsure], ','));
				char* elementCurse = (char*)malloc(sizeof(char) * (valNameOfcurse + 1));
				strcpy(elementCurse, strtok(buffer[+carsure], ','));
				*arrStudent[i][j + a + 1] = elementCurse;
				carsure = carsure + (valNameOfcurse + 1);
				int counterCursers = (coursesPerStudent[i] - 1);

				if (a < counterCursers)
				{
					int valScore = strlen(strtok(buffer[+carsure], '|'));
					char* elemenScore = (char*)malloc(sizeof(char) * (valScore + 1));
					strcpy(elemenScore, strtok(buffer[+carsure], '|'));
					*arrStudent[i][j + a + 2] = elemenScore;
					carsure = carsure + (valScore + 1);
				}
				else//ציון של הקורס האחרון עד סוף השורה
				{
					int valScore = strlen(strtok(buffer[+carsure], NULL));
					char* elemenScore = (char*)malloc(sizeof(char) * (valScore + 1));
					if (elemenScore == NULL)return;
					strcpy(elemenScore, strtok(buffer[+carsure], NULL));
					*arrStudent[i][j + a + 2] = elemenScore;
					carsure = carsure + (valScore + 1);
				}


			}


		}
	}


	return &arrStudent;
	fclose(myfle);
}

void factorGivenCourse(char** const* students, const int* coursesPerStudent, int numberOfStudents, const char* courseName, int factor)
{
	if (factor < -20 && factor > 20)
		return;

	for (int a = 0; a < numberOfStudents; a++) //יעבור סטודנט סטודנט
	{

		int* qours = (2 * coursesPerStudent[a]);
		for (int q = 1; q < *qours; q + 2)
		{
			if (strcmp(courseName, *students[a][q])) // מחזיר 1 אם זה הקורס, תא שם הקורס
			{
				int scor = *students[a][q + 1]; //תא הציון

				if (scor >= 100)
					scor = 100;


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
