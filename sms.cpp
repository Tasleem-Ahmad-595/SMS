#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <cctype>
#include <iomanip> 
using namespace std;

// variables used by admin
string admin_pass = "Admin@24";
const int max_teacher = 10;
string teacher_name[max_teacher]= { "" }, teacher_subject[max_teacher]= { "" }, teacher_class[max_teacher]= { "" }, teacher_password[max_teacher]= { "" }, teacher_ID[max_teacher]= { "" };
int current_teacher_count = 0;

// variables used by teacher
const int max_student = 40;
string student_ID[max_student] = { "" }, student_name[max_student] = { "" }, student_class[max_student] = { "" }, student_password[max_student] ;
int current_student_count = 0;

float student_quizzes[max_student][10];   // Array to store quiz marks (10 quizzes max)
float student_assignments[max_student][10]; // Array to store assignment marks (10 assignments max)
float student_midterms[max_student][2];    // Array to store midterm marks (2 midterms)
float student_finals[max_student][1];
string subject_file_path = "";

// List of subject files containing grades
const int max_subjects = 4;
string subject_files[max_subjects] = {"calculus_grades.txt","physics_grades.txt", "english_grades.txt","programming fundamentals_grades.txt"};

string cgpa_output_file = "CGPA_file.txt";
string subject_names[] = { "calculus","physics","english","programming fundamentals" };
string marks_file[max_subjects];



// Function to save teacher data to a file
void saveTeacherToFile(string teacher_name[], string teacher_subject[], string teacher_class[], string teacher_password[], string teacher_ID[], int current_teacher_count) {
    if (current_teacher_count <= 0) {
        cout << "ERROR: No teacher data to save.\n";
        return;
    }

    ofstream outFile("teachers data.txt", std::ios::out);
    if (!outFile) {
        cout << "ERROR: Unable to open file for saving data.\n";
        return;
    }
  
    // Write each teacher's data in a structured format
    for (int i = 0; i < current_teacher_count; ++i) {
        if (!teacher_ID[i].empty() && !teacher_name[i].empty() && !teacher_subject[i].empty() && !teacher_class[i].empty() && !teacher_password[i].empty()) {
            outFile << teacher_ID[i] << "|"  // Use | as a delimiter
                << teacher_name[i] << "|"
                << teacher_subject[i] << "|"
                << teacher_class[i] << "|"
                << teacher_password[i] << "\n";
        }
    }

    outFile.close(); // Close the file
    cout << "\n--- Data saved successfully! ----\n";
}


// Function to load teacher data from a file
void loadTeacherFromFile(string teacher_name[], string teacher_subject[], string teacher_class[], string teacher_password[], string teacher_ID[], int& current_teacher_count) {
    ifstream inFile("teachers data.txt");
    if (!inFile) {
        return;
    }
    current_teacher_count = 0;
    while (!inFile.eof() && current_teacher_count < max_teacher) {
        getline(inFile, teacher_ID[current_teacher_count], '|');
        getline(inFile, teacher_name[current_teacher_count], '|');
        getline(inFile, teacher_subject[current_teacher_count], '|');
        getline(inFile, teacher_class[current_teacher_count], '|');
        getline(inFile, teacher_password[current_teacher_count]);
        current_teacher_count++;
    }
    inFile.close();
}

// Function to add a teacher
void addteacher(string teacher_name[], string teacher_subject[], string teacher_class[], string teacher_password[], string teacher_ID[], int& current_teacher_count, int max_teacher) {
    if (current_teacher_count >= max_teacher) {
        cout << "ERROR: Cannot add more teachers. Maximum capacity reached!\n";
        return;
    }

    string id, name, subject, teacher_class_name, password;

    while (true) {
        cout << "Enter Teacher ID (3 digits): ";
        cin >> id;
        if (id.length() != 3 || !isdigit(id[0]) || !isdigit(id[1]) || !isdigit(id[2])) {
            cout << "Must contain exactly 3 digits. Enter again: ";
            continue;
        }
        break;
    }

    for (int i = 0; i < current_teacher_count; ++i) {
        if (teacher_ID[i] == id) {
            cout << "ERROR: A teacher with this ID already exists. Try again.\n";
            return;
        }
    }
    // ADDING TEACHER NAME

    cin.ignore(); // clear the input buffer
    cout << "Enter Teacher Name (No digits): ";
    getline(cin, name);

    // Check if input is empty
    while (name.empty()) {
        cout << "ERROR: Teacher name cannot be empty. Enter again: ";
        getline(cin, name); // re-prompt for input
    }

    while (true) {
        bool valid = true;
        for (int i = 0; i < name.length(); ++i) {
            if (!isalpha(name[i]) && name[i] != ' ') {
                cout << "ERROR: Teacher name must contain only alphabets and spaces. Enter again: ";
                getline(cin, name);
                valid = false;
                break;
            }
        }
        if (valid) break;
    }
    // ADDING TEACHER SUBJECT
    cout << "Enter Teacher Subject (No digits, choose any '1' subject : Physics,Calculus,English or Programming Fundamentals):\n";
    
    getline(cin, subject);

    // Convert to lowercase for uniformity
    for (int i = 0; i < subject.length(); ++i) {
        subject[i] = tolower(subject[i]);
    }

    // Check if the subject is one of the allowed subjects
    while (true) {
        if ((subject != "programming fundamentals") && (subject != "calculus") && (subject != "english") && (subject != "physics")) {
            cout << "ERROR: Invalid subject or incorrect spellings! Enter again: ";
            getline(cin, subject);
        }
        else {
            break;
        }
    }

    // Check if input is empty
    while (subject.empty()) {
        cout << "ERROR: Teacher subject cannot be empty. Enter again: ";
        getline(cin, subject); // re-prompt for input
    }

    // Validate that subject contains only letters and spaces
    bool valid = true;
    for (int i = 0; i < subject.length(); ++i) {
        if (!isalpha(subject[i]) && subject[i] != ' ') {
            cout << "ERROR: Invalid Teacher subject (Must not contain digits or special characters)! Enter again: ";
            getline(cin, subject);
            valid = false;
            break;
        }
        if (valid) {
            break;
        }
    }

   
   
    // ADDING TEACHER CLASS
    while (true) {
        bool validClass = false;
        cout << "Enter Class Assigned (must contain alphabet,digit and a '-'): ";
        getline(cin, teacher_class_name);

        // Check if input is empty
        if (teacher_class_name.empty()) {
            cout << "ERROR: Class assigned cannot be empty. Enter again: ";
            continue; // re-prompt for input
        }

        validClass = true;
        bool hasAlpha = false, hasDigit = false, hasHyphen = false;

        for (int i = 0; i < teacher_class_name.length(); ++i) {
            if (isalpha(teacher_class_name[i])) {
                hasAlpha = true;
            }
            else if (isdigit(teacher_class_name[i])) {
                hasDigit = true;
            }
            else if (teacher_class_name[i] == '-') {
                hasHyphen = true;
            }

            // Break out of loop early if all conditions are met
            if (hasAlpha && hasDigit && hasHyphen) {
                break;
            }
        }

        if (!hasAlpha || !hasDigit || !hasHyphen) {
            cout << "ERROR: Teacher class must contain at least one alphabet, one digit, and one '-'. Enter again: ";
            validClass = false;
        }

        if (validClass) break;
    }
    // ADDING TEACHER PASSWORD
    cout << "Set a Password for this Teacher: ";
    getline(cin, password);

    // Check if input is empty
    while (password.empty()) {
        cout << "ERROR: Password must be set. Enter again: ";
        getline(cin, password); // re-prompt for input
    }

    teacher_ID[current_teacher_count] = id;
    teacher_name[current_teacher_count] = name;
    teacher_subject[current_teacher_count] = subject;
    teacher_class[current_teacher_count] = teacher_class_name;
    teacher_password[current_teacher_count] = password;

    current_teacher_count++;
    saveTeacherToFile(teacher_name, teacher_subject, teacher_class, teacher_password, teacher_ID, current_teacher_count);
    cout << "\n--- Teacher added successfully! ---\n";
}

// Function to delete a teacher
void deleteteacher(string teacher_name[], string teacher_subject[], string teacher_class[], string teacher_password[], string teacher_ID[], int& current_teacher_count) {
    if (current_teacher_count == 0) {
        cout << "ERROR: No teachers to remove!\n";
        return;
    }
    string name;
    
    string id;
    cout << "Enter Teacher ID to remove: ";
    cin >> id;

    // Search for the teacher ID in the array
    int index = -1;
    for (int i = 0; i < current_teacher_count; ++i) {
        if (teacher_ID[i] == id) {
            index = i;
            name = teacher_name[i];
            break;
        }
    }

    if (index == -1) {
        // If the teacher ID is not found
        cout << "ERROR: Teacher with ID " << id << " not found!\n";
        return;
    }

    // Shift the array elements after the removed teacher
    for (int i = index; i < current_teacher_count - 1; ++i) {
        teacher_ID[i] = teacher_ID[i + 1];
        teacher_name[i] = teacher_name[i + 1];
        teacher_subject[i] = teacher_subject[i + 1];
        teacher_class[i] = teacher_class[i + 1];
        teacher_password[i] = teacher_password[i + 1];
    }

  

    // Decrease the teacher count
    current_teacher_count--;

    // Save the updated list to file
    saveTeacherToFile(teacher_name, teacher_subject, teacher_class, teacher_password, teacher_ID, current_teacher_count);
    cout << "\n---- Teacher with ID: '" << id << "' and name: '" <<name<<"' has been removed successfully -----\n";
}





// Function to load student data from a file
void loadStudentFromFile(string student_ID[], string student_name[], string student_class[], string student_password[], int& current_student_count) {
    ifstream inFile("students data.txt");
    if (!inFile) {
        return;
    }
    current_student_count = 0;
    while (!inFile.eof() && current_student_count < max_student) {
        
        getline(inFile, student_ID[current_student_count], '|');
        getline(inFile, student_name[current_student_count], '|');
        getline(inFile, student_class[current_student_count], '|');
        getline(inFile, student_password[current_student_count]);
        current_student_count++;
    }
    inFile.close();
}

// Function to save student data to a file
void saveStudentToFile(string teacher_ID[], string teacher_name[], string teacher_class[], string teacher_password[], int current_student_count) {
    if (current_student_count <= 0) {
        cout << "ERROR: No student data to save.\n";
        return;
    }

    ofstream outFile("students data.txt", std::ios::out);
    if (!outFile) {
        cout << "ERROR: Unable to open file for saving data.\n";
        return;
    }

    // Write each teacher's data in a structured format
    for (int i = 0; i < current_student_count; ++i) {
        if (!student_ID[i].empty() && !student_name[i].empty() && !student_class[i].empty() && !student_password[i].empty()) {
            outFile << student_ID[i] << "|"  // Use | as a delimiter
               << student_name[i] << "|"
               << student_class[i] << "|"
               <<student_password[i] << "\n";
        }
    }

    outFile.close(); // Close the file
    cout << "\n--- Data saved successfully! ----\n";
}

// Function to add a student
void addstudent(string student_ID[], string student_name[], string student_class[], string student_password[],  int& current_student_count, int max_student) {
    if (current_student_count >= max_student) {
        cout << "ERROR: Cannot add more students. Maximum capacity reached (40)!\n";
        return;
    }

    string id, name, student_class_name, password;

    while (true) {
        cout << "Enter Student ID (4 digits): ";
        cin >> id;
        if (id.length() != 4 || !isdigit(id[0]) || !isdigit(id[1]) || !isdigit(id[2]) || !isdigit(id[3])) {
            cout << "ERROR: Must contain exactly 4 digits. Enter again: ";
            continue;
        }
        break;
    }

    for (int i = 0; i < current_student_count; ++i) {
        if (student_ID[i] == id) {
            cout << "ERROR: A student with this ID already exists. Try again.\n";
            return;
        }
    }
    // ADDING STUDENT NAME

    cin.ignore(); // clear the input buffer
    cout << "Enter Student Name (No digits , max 20 characters): ";
    getline(cin, name);

    // Check if input is empty
    while (name.empty()) {
        cout << "ERROR: Student name cannot be empty. Enter again: ";
        getline(cin, name); // re-prompt for input
    }

    while (true) {
        bool valid = true;
        for (int i = 0; i < name.length(); ++i) {
            if (!isalpha(name[i]) && name[i] != ' ') {
                cout << "ERROR: Student name must contain only alphabets and spaces. Enter again: ";
                getline(cin, name);
                valid = false;
                break;
            }
        }
        if (valid) break;
    }
   // ADDING STUDENT CLASS
    while (true) {
        bool validClass = false;
        cout << "Enter Class Assigned (must contain alphabet,digit and a '-'): ";
        getline(cin, student_class_name);

        // Check if input is empty
        if (student_class_name.empty()) {
            cout << "ERROR: Class assigned cannot be empty. Enter again: ";
            continue; // re-prompt for input
        }

        validClass = true;
        bool hasAlpha = false, hasDigit = false, hasHyphen = false;

        for (size_t i = 0; i < student_class_name.length(); ++i) {
            if (isalpha(student_class_name[i])) {
                hasAlpha = true;
            }
            else if (isdigit(student_class_name[i])) {
                hasDigit = true;
            }
            else if (student_class_name[i] == '-') {
                hasHyphen = true;
            }

            // Break out of loop early if all conditions are met
            if (hasAlpha && hasDigit && hasHyphen) {
                break;
            }
        }

        if (!hasAlpha || !hasDigit || !hasHyphen) {
            cout << "ERROR: Student class must contain at least one alphabet, one digit, and one '-'. Enter again: ";
            validClass = false;
        }

        if (validClass) break;
    }
    // ADDING STUDENT PASSWORD
    cout << "Set a Password for this Student: ";
    getline(cin, password);

    // Check if input is empty
    while (password.empty()) {
        cout << "ERROR: Password must be set. Enter again: ";
        getline(cin, password); // re-prompt for input
    }

    student_ID[current_student_count] = id;
    student_name[current_student_count] = name;
    student_class[current_student_count] = student_class_name;
    student_password[current_student_count] = password;

    current_student_count++;
    saveStudentToFile(student_ID, student_name, student_class, student_password, current_student_count);
    cout << "\n---- Student added successfully! -----\n";
}
// Function to delete a student
void deletestudent(string student_ID[], string student_name[], string student_class[], string student_password[], int& current_student_count) {
    if (current_student_count == 0) {
        cout << "ERROR: No students to remove!\n";
        return;
    }
    string name;
    string id;
    cout << "Enter Student ID to remove: ";
    cin >> id;

    // Search for the student ID in the array
    int index = -1;
    for (int i = 0; i < current_student_count; ++i) {
        if (student_ID[i] == id) {
            index = i;
            name = student_name[i];
            break;
        }
    }

    if (index == -1) {
        // If the student ID is not found
        cout << "ERROR: Student with ID " << id << " not found!\n";
        return;
    }

    // Shift the array elements after the removed student
    for (int i = index; i < current_student_count - 1; ++i) {
        student_ID[i] = student_ID[i + 1];
        student_name[i] = student_name[i + 1];
        student_class[i] = student_class[i + 1];
        student_password[i] = student_password[i + 1];
    }


    // Decrease the student count
    current_student_count--;

    // Save the updated list to file
    saveStudentToFile(student_ID, student_name, student_class, student_password, current_teacher_count);
    cout << "\n---- Student with ID '" << id <<"' and name: '" << name << "' has been removed successfully ----\n";
}



// functon to save mark to the file

void saveMarkstofile(string subject_file_path, int index, int num_quizzes, int num_assignments) {
    // Open the file for reading
    ifstream infile(subject_file_path);
    string lines[200]; // Array to store lines from the file
    int line_count = 0;
    bool id_found = false;


    // Read the file line by line
    while (getline(infile, lines[line_count])) {
        // Extract the ID from the line (first word)
        string current_id = lines[line_count].substr(0, lines[line_count].find(" "));

        // Check if it matches the student's ID
        if (current_id == student_ID[index]) {
            id_found = true;
            

            // Construct the new line with updated data
            lines[line_count] = student_ID[index] + " " + to_string( num_quizzes) + " ";
            for (int i = 0; i < num_quizzes; ++i) {
                lines[line_count] += to_string(student_quizzes[index][i]) + " ";
            }

            lines[line_count] += to_string(num_assignments) + " ";
            for (int i = 0; i < num_assignments; ++i) {
                lines[line_count] += to_string(student_assignments[index][i]) + " ";
            }

             // Number of midterms
            for (int i = 0; i < 2; ++i) {
                lines[line_count] += to_string(student_midterms[index][i]) + " ";
            }

            lines[line_count] += to_string(student_finals[index][0]);
        }

        ++line_count;
    }
    infile.close();

    // If ID not found, append the new line
    if (!id_found) {
        string new_line = student_ID[index] + " " + to_string(num_quizzes) + " ";
        for (int i = 0; i < num_quizzes; ++i) {
            new_line += to_string(student_quizzes[index][i]) + " ";
        }

        new_line += to_string(num_assignments) + " ";
        for (int i = 0; i < num_assignments; ++i) {
            new_line += to_string(student_assignments[index][i]) + " ";
        }

         // Number of midterms
        for (int i = 0; i < 2; ++i) {
            new_line += to_string(student_midterms[index][i]) + " ";
        }

        new_line +=  to_string(student_finals[index][0]);

        lines[line_count++] = new_line; // Add the new line to the array
    }

    // Write all lines back to the file
    ofstream outfile(subject_file_path);
    outfile  << fixed << setprecision(2);

    for (int i = 0; i < line_count; ++i) {
        outfile << lines[i] << endl;
    }
    outfile.close();
}


// function to load marks from the files
void loadMarks(string subject_file_path) {
    ifstream file(subject_file_path);
    if (file.is_open()) {
        for (int i = 0; i < max_student; ++i) {
            file >> student_ID[i];

            int num_quizzes;
            file >> num_quizzes;
            for (int j = 0; j < num_quizzes; ++j) {
                file >> student_quizzes[i][j];
            }

            int num_assignments;
            file >> num_assignments;
            for (int j = 0; j < num_assignments; ++j) {
                file >> student_assignments[i][j];
            }

            int num_midterms;
            file >> num_midterms;
            for (int j = 0; j < num_midterms; ++j) {
                file >> student_midterms[i][j];
            }

            int num_finals;
            file >> num_finals;
            for (int j = 0; j < num_finals; ++j) {
                file >> student_finals[i][j];
            }

            if (file.eof()) break;
        }
        file.close();
    }
    
}



// function to assign marks to a student

void assignmarks(string subject_file_path) {
    if (current_student_count == 0) {
        cout << "ERROR: No students available to assign marks.\n";
        return;
    }

    string student_id;
    cout << "Enter Student ID to assign marks: ";
    cin >> student_id;

    int index = -1;
    for (int i = 0; i < current_student_count; ++i) {
        if (student_ID[i] == student_id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "ERROR: Student with ID " << student_id << " not found!\n";
        return;
    }

    cout << "\n----- Assigning marks to ID: '" << student_ID[index] << "' and Name: '" << student_name[index] << "' -----\n";

    // Check if marks for this student ID already exist in the file
    ifstream infile(subject_file_path);
    string lines[200]; // Array to store lines from the file
    int line_count = 0;
    bool id_found = false;

    while (getline(infile, lines[line_count])) {
        string current_id = lines[line_count].substr(0, lines[line_count].find(" "));
        if (current_id == student_ID[index]) {
            id_found = true;
            break;
        }
        ++line_count;
    }
    infile.close();

    if (id_found) {
        char confirmation;
        cout << "NOTICE: wStudent data for ID " << student_id << " already exists. Press 'Y' to update the marks or press any key to cancel.: ";
        cin >> confirmation;

        if (confirmation != 'y' && confirmation != 'Y') {
            cout << "Update canceled. No changes were made.\n";
            return;
        }
    }
    

    // Input for Quizzes
    int num_quizzes;
    while (true) {
        cout << "Enter total number of quizzes (max 10): ";   
        cin >> num_quizzes;
        if (num_quizzes < 1 || num_quizzes > 10) {
            cout << "NOTICE: Number of quizes cannot be negative,zero or greater than maximum limit! enter again: ";
            cin >> num_quizzes;
        }
        break;
    }
    cout << "Enter marks for " << num_quizzes << " quizzes (0-10): \n";
    for (int i = 0; i < num_quizzes; ++i) {
        cout << "Quiz " << i + 1 << ": ";
        cin >> student_quizzes[index][i]; 
        while (student_quizzes[index][i] < 0 || student_quizzes[index][i] > 10) {
            cout << "ERROR: Invalid marks range! enter again: ";
            cin >> student_quizzes[index][i];
        }
    }

    // Input for Assignments
     int num_assignments;
    while (true) {
        cout << "Enter total number of assignments (max 10): ";   
        cin >> num_assignments;
        if (num_assignments < 1 || num_assignments > 10) {
            cout << "NOTICE: Number of assignments cannot be negative,zero or exced the maximum limit! enter again: ";
            cin >> num_assignments;
        }
        break;
    }
    cout << "Enter marks for " << num_assignments << " assignments (0-10): \n";
    for (int i = 0; i < num_assignments; ++i) {
        cout << "Assignment " << i + 1 << ": ";
        cin >> student_assignments[index][i]; 
        while (student_assignments[index][i] < 0 || student_assignments[index][i] > 10) {
            cout << "ERROR: Invalid marks range! Enter again: ";
            cin >> student_assignments[index][i];
        }
    }

    // Input for Midterms
    cout << "Enter marks for Two Midterms (0-40): \n";
    for (int i = 0; i < 2; ++i) {
        cout << "Midterm " << i + 1 << ": ";
        cin >> student_midterms[index][i];
        while (student_midterms[index][i] < 0 || student_midterms[index][i] > 40) {
            cout << "ERROR: Invalid marks range! enter again: ";
            cin >> student_midterms[index][i];
        }
    }

    // Input for Finals
    cout << "Enter marks for Final exams (0-100): ";
    cin >> student_finals[index][0];
    while (student_finals[index][0] < 0 || student_finals[index][0] > 100) {
        cout << "ERROR: Invalid marks range! enter again: ";
        cin >> student_finals[index][0];
    }

    // Save the updated marks to the subject file
    saveMarkstofile(subject_file_path, index , num_quizzes , num_assignments);
    cout << "\n---- Marks successfully assigned to " << student_name[index] << " -----\n";
}






// Function to assign grades based on the final weighted score
char assignGrade(double final_score) {
    if (final_score >= 90) return 'A';
    else if (final_score >= 80) return 'B';
    else if (final_score >= 70) return 'C';
    else if (final_score >= 60) return 'D';
    else if (final_score >= 50) return 'E';
    else return 'F';
}

void calculateGrades(string subject_file_path, string output_file_path) {
    ifstream infile(subject_file_path);
    ofstream outfile(output_file_path);

    if (!infile.is_open() || !outfile.is_open()) {
        cout << "ERROR: Unable to open file.\n";
        return;
    }

    string student_id;
    int num_quizzes, num_assignments;
    double quiz_mark, assignment_mark, mid1, mid2, final_exam;
    double total_quiz_marks, total_assignment_marks;

    while (infile >> student_id >> num_quizzes) {
        total_quiz_marks = 0;

        // Read and sum quiz marks
        for (int i = 0; i < num_quizzes; ++i) {
            infile >> quiz_mark;
            total_quiz_marks += quiz_mark;
        }

        infile >> num_assignments;
        total_assignment_marks = 0;

        // Read and sum assignment marks
        for (int i = 0; i < num_assignments; ++i) {
            infile >> assignment_mark;
            total_assignment_marks += assignment_mark;
        }

        infile >> mid1 >> mid2 >> final_exam;

        // Avoid division by zero for quizzes and assignments
        double quiz_score;
        if (num_quizzes > 0) { 
            quiz_score = (total_quiz_marks / (num_quizzes * 10.0)) * 100;
        }
        else
            quiz_score = 0;
        
        double assignment_score;
        if (num_assignments > 0) {
            assignment_score=(total_assignment_marks / (num_assignments * 10.0)) * 100;
        } else
            assignment_score = 0;

        // Scale midterm and final scores to percentages
        double midterm_score = ((mid1 + mid2) / 80.0) * 100; // Midterms out of 40 each
        double final_score_scaled = (final_exam / 100.0) * 100; // Final out of 100

        // Calculate the weighted score
        double final_score = (quiz_score * 0.10) + (assignment_score * 0.10) +
            (midterm_score * 0.30) + (final_score_scaled * 0.50);

        // Assign grade
        char grade = assignGrade(final_score);

        // Write to output file
        outfile << student_id << " " << grade << endl;
    }

    cout << "\n----- Grading Successful! ------\n";

    infile.close();
    outfile.close();
}

// Function to convert grades to grade points
double gradeToPoint(char grade) {
    switch (grade) {
    case 'A': return 4.0;
    case 'B': return 3.0;
    case 'C': return 2.0;
    case 'D': return 1.0;
    case 'E': return 0.5;
    case 'F': return 0.0;
    default: return 0.0;
    }
}
// function to calculate CGPA according to the grade points
void calculateCGPA() {
    ofstream outfile(cgpa_output_file);

    if (!outfile.is_open()) {
        cout << "ERROR: Unable to open output file.\n";
        return;
    }

    // Step 1: Filter `subject_files` to keep only the files that exist
    string valid_files[max_subjects];
    int valid_file_count = 0;

    for (int i = 0; i < max_subjects; ++i) {
        ifstream infile(subject_files[i]);
        if (infile.is_open()) {
            valid_files[valid_file_count++] = subject_files[i];
        }       
        infile.close();
    }

    if (valid_file_count == 0) {
        cout << "ERROR: No valid subject files found. Cannot calculate CGPA.\n";
        outfile.close();
        return;
    }

    // Step 2: Collect student IDs manually
    int current_student_count = 0;
    for (int i = 0; i < max_student; ++i) {
        if (student_ID[i].empty()) {
            current_student_count = i;
            break;
        }
    }

    // Step 3: Calculate CGPA for each student
    for (int i = 0; i < current_student_count; ++i) {
        string current_id = student_ID[i];
        double total_points = 0.0;
        int subjects_count = 0;
        bool complete_data = true;

        for (int j = 0; j < valid_file_count; ++j) {
            ifstream infile(valid_files[j]);
            string id_in_file;
            char grade;
            bool found = false;

            // Search for the current student ID in the valid subject file
            while (infile >> id_in_file >> grade) {
                if (id_in_file == current_id) {
                    total_points += gradeToPoint(grade);
                    ++subjects_count;
                    found = true;
                    break;
                }
            }

            infile.close();

            // If the student is not found in the current subject file, mark data as incomplete
            if (!found) {
                complete_data = false;
                break;
            }
        }

        // Step 4: Write results to the output file
        if (complete_data && subjects_count == valid_file_count) {
            double cgpa = total_points / subjects_count;
            outfile << current_id << " " << student_name[i] << " " << cgpa << endl;
        }
        else {
            outfile << current_id << " " << student_name[i] << " Incomplete Data\n";
        }
    }

    cout << "\n----- CGPA Calculation Successful! -----\n";
    outfile.close();
}


// function to calculate average cgpa
void showAverageCGPA() {
    ifstream infile(cgpa_output_file);

    if (!infile.is_open()) {
        cout << "ERROR: Unable to open the CGPA output file.\n";
        return;
    }

    double total_cgpa = 0.0;
    int valid_student_count = 0;

    string id, name, cgpa_or_status;

    // Read the file line by line
    while (infile >> id >> name >> cgpa_or_status) {
        // Skip students with "Incomplete" CGPA
        if (cgpa_or_status == "Incomplete") {
            continue;
        }

        // Convert CGPA from string to double and add to total
        double cgpa = stof(cgpa_or_status); // Directly convert since CGPA is guaranteed valid
        total_cgpa += cgpa;
        valid_student_count++;
    }

    infile.close();

    // Calculate and display the average CGPA
    if (valid_student_count > 0) {
        double average_cgpa = total_cgpa / valid_student_count;
        cout << "\n--------------------------------------------------------------------------\n";
        cout << "Average CGPA of all students: " << fixed << setprecision(2) << average_cgpa << endl;
        cout << "--------------------------------------------------------------------------\n";
    }
    else {
        cout << "ERROR: No valid CGPA data found to calculate the average.\n";
    }
}








// function to list failed students

void listFailedStudents(string subjectFilePath) {
    ifstream infile(subjectFilePath);

    if (!infile.is_open()) {
        cout << "ERROR: Could not open the file!"<< endl;
        return;
    }

    string student_id, student_name_in_file;
    float cgpa;
    bool found = false; // To check if any students failed
   

    cout << "\nList of FAILED Students (CGPA < 2.0)\n";
    cout << "'''''''''''''''''''''''''''''''''''''''''''''''\n";
    cout << "ID\tName\tClass\tCGPA\n";

    while (infile >> student_id >> student_name_in_file >> cgpa) {
        if (cgpa < 2.00) {
            // Find the student details from the global variables
            for (int i = 0; i < current_student_count; ++i) {
                if (student_ID[i] == student_id) {
                    found = true;

                    // Print student details
                    cout << student_ID[i] << "\t" << student_name[i] << "\t"
                        << student_class[i] << "\t" << fixed << setprecision(2) << cgpa << endl;
                }
            }
        }
    }

    if (!found) {
        cout << "\nNOTICE: No students failed this course.\n\n";
    }

    infile.close();
}









// Function to list passed students
void listPassedStudents(string subjectFilePath) {
    ifstream infile(subjectFilePath);

    if (!infile.is_open()) {
        cout << "ERROR: Could not open the file!" << endl;
        return;
    }

    string student_id, student_name_in_file;
    float cgpa;
    bool found = false; // To check if any students passed
   

    cout << "\nList of PASSED Students (CGPA >= 2.0)\n";
    cout << "'''''''''''''''''''''''''''''''''''''''''''''''\n";
    cout << "ID\tName\t\tClass\tCGPA\n";

    while (infile >> student_id >> student_name_in_file >> cgpa) {
        if (cgpa >= 2.00) {
            // Find the student details from the global variables
            for (int i = 0; i < current_student_count; ++i) {
                if (student_ID[i] == student_id) {
                    found = true;

                    // Print student details
                    cout << student_ID[i] << "\t" << student_name[i] << "\t\t"
                        << student_class[i] << "\t" << fixed << setprecision(2) << cgpa << endl;
                }
            }
        }
    }

    if (!found) {
        cout << "\nNOTICE: No students passed this course.\n\n";
    }

    infile.close();
}





// function for viewing marks by the teacher
void viewMarks(string subject_file_path, string t_subject) {
    ifstream file(subject_file_path);
    if (!file.is_open()) {
        cout << "Error opening file: " << subject_file_path << endl;
        return;
    }
    t_subject[0] = toupper(t_subject[0]);
    cout << "\n                                 MARKS REPORT for '" << t_subject << "'\n-------------------------------------------------------------------------------------------\n";
    cout << fixed << setprecision(1); // Set precision for floating-point output
    cout << left << setw(10) << " ID"
        << setw(15) << "Name"
        << setw(10) << "Class"
        << setw(15) << "Quizzes"
        << setw(17) << "Assignments"
        << setw(15) << "Midterms"
        << setw(15) << "Finals" << endl;
    cout << "-------------------------------------------------------------------------------------------" << endl;

    string student_id, stud_name, stud_class;
    int num_quizzes, num_assignments;
    float quizzes[10], assignments[10], midterms[2], finals[1];

    while (file >> student_id >> num_quizzes) {


        int student_index = -1;

        // Find the student index based on student_id
        for (int i = 0; i < current_student_count; ++i) {
            if (student_ID[i] == student_id) {
                stud_name = student_name[i];
                stud_class = student_class[i];
                break;
            }
        }
    

        // Read quizzes
        for (int i = 0; i < num_quizzes; ++i) {
            file >> quizzes[i];
        }

        // Read assignments
        file >> num_assignments;
        for (int i = 0; i < num_assignments; ++i) {
            file >> assignments[i];
        }

        // Read midterms
        for (int i = 0; i < 2; ++i) {
            file >> midterms[i];
        }

        // Read final exam
        file >> finals[0];

        // Output the stored data in a formatted table
        cout << left << setw(10) << student_id
            << setw(15) << stud_name
            << setw(10) << stud_class;

        for (int i = 0; i < num_quizzes; ++i) {
            cout << left << setw(4) << quizzes[i];
            if (i < num_quizzes - 1) {
                cout << ", "; // Add comma only if it's not the last element
            }
        }
        cout << "      ";
        for (int i = 0; i < num_assignments; ++i) {
            cout << left << setw(4) << assignments[i];
            if (i < num_assignments - 1) {
                cout << ", "; // No space after comma
            }
        }
        cout << "      ";
        for (int i = 0; i < 2; ++i) {
            cout << left << setw(4) << midterms[i];
            if (i < 1) {
                cout << ", "; // No space after comma
            }
        }
        cout << "      ";
        cout << left << setw(4) << finals[0];

        cout << endl;

       
    }

    file.close();
}




// Function to view student grades in each subject
void viewStudentGrades(string student_id, string subject_names[], int subject_count) {
    cout << "\n                               GRADES REPORT FOR STUDENT ID: " << student_id << "\n";
    cout << "------------------------------------------------------------------------------------------------\n";

    for (int subj = 0; subj < subject_count; ++subj) {
        string subject = subject_names[subj];
        string grades_file = subject + "_grades.txt";

        ifstream grades_infile(grades_file);

        if (!grades_infile.is_open()) {
            
            continue;
        }

        // Format the subject name for display
        subject[0] = toupper(subject[0]);
        cout << "\n                                 GRADES REPORT for '" << subject << "'\n";
        cout << "-------------------------------------------------------------------------------------------\n";
        cout << fixed << setprecision(1);
        cout << left << setw(15) << "Student ID" << setw(17) << "Grade" << endl;
        cout << "-------------------------------------------------------------------------------------------\n";

        string file_student_id, grade;

        bool student_found = false;

        // Retrieve grade from grades file
        while (grades_infile >> file_student_id >> grade) {
            if (file_student_id == student_id) {
                student_found = true;
                cout << left << setw(15) << file_student_id << setw(17) << grade << endl;
                break;
            }
        }

        if (!student_found) {
            cout << "No grade available for student in subject: " << subject << endl;
        }

        grades_infile.close();
    }

    cout << "-------------------------------------------------------------------------------------------\n";
}







// function for student to view thier CGPA
void viewStudentCGPA(string student_id) {
    ifstream file("CGPA_file.txt");
    if (!file.is_open()) {
        cout << "Error opening CGPA file." << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        // Split the line into tokens based on spaces
        size_t pos = 0;
        string id, name;
        float cgpa;

        // Extract student ID
        pos = line.find(' ');
        id = line.substr(0, pos);

        // Move past student ID and get the CGPA
        int next_pos = line.find_last_of(' ');
        cgpa = stof(line.substr(next_pos + 1));

        // Check if this line corresponds to the given student ID
        if (id == student_id) {
            // Output the CGPA
            cout << "\n\n                                 CGPA REPORT for student ID: '" << student_id << "'\n-------------------------------------------------------------------------------------------\n";
            cout << left << setw(10) << "ID" << setw(10) << "CGPA" << endl;
            cout << "-------------------------------------------------------------------------------------------" << endl;
            cout << left << setw(10) << id << left << setw(10) << cgpa << endl;
            cout << "-------------------------------------------------------------------------------------------" << endl;

            // No need to continue searching once we find the student ID
            break;
        }
    }

    file.close();
}





// function to view marks by the student

void viewStudentMarks(string login_id) {
    // Construct marks file names
    for (int i = 0; i < max_subjects; i++) {
        marks_file[i] = subject_names[i] + "_marks.txt";
    }

    int valid_file_count = 0;

    // Filter valid marks files
    for (int i = 0; i < max_subjects; ++i) {
        ifstream infile(marks_file[i]);
        if (infile.is_open()) {
            marks_file[valid_file_count++] = marks_file[i];
        }
        else {
            // Clear the invalid file entry
            marks_file[i] = "";
        }
        infile.close();
    }

    if (valid_file_count == 0) {
        cout << "ERROR: No valid subject files found.\n";
        return;
    }

    string subject;
    cout << "Enter the subject to view marks ( Calculus / Physics / English / Programming Fundamentals ): ";
    cin >> subject;

    for (int i = 0; i < subject.length(); ++i) {
        subject[i] = tolower(subject[i]);
    }

    bool subject_found = false;
    for (int i = 0; i < valid_file_count; ++i) {
        if (marks_file[i] == subject + "_marks.txt") {
            subject_found = true;
            ifstream infile(marks_file[i]);

            if (!infile.is_open()) {
                cout << "ERROR: Unable to open the marks file for " << subject << ".\n";
                return;
            }

            string student_id;
            double quiz[10], assignment[10], midterm1, midterm2, final_exam;
            int num_quizzes = 0, num_assignments = 0;

            // Reading the file for the specific student
            while (infile >> student_id) {
                if (student_id == login_id) {
                    // Match found, read marks for this student
                    infile >> num_quizzes;

                    // Read quizzes
                    for (int k = 0; k < num_quizzes; ++k) {
                        infile >> quiz[k];
                    }

                    infile >> num_assignments;

                    // Read assignments
                    for (int k = 0; k < num_assignments; ++k) {
                        infile >> assignment[k];
                    }

                    infile >> midterm1 >> midterm2 >> final_exam;

                    string name;
                    string stud_class;
                    for (int i = 0; i < max_student; ++i) {
                        if (student_id == student_ID[i]) {
                            name = student_name[i];
                            stud_class = student_class[i];
                        }
                    }

                    // Display marks
                    cout << "Student ID: '" << student_id << "' Name: '" << name << "' Class: '" << stud_class <<"'"<< endl;
                    cout << "\n------------Quizzes----------\n\n";
                    for (int k = 0; k < num_quizzes; ++k) {
                        cout << "  Quiz " << k + 1 << ": " << quiz[k] << endl;
                    }
                    cout << "\n----------Assignments--------\n\n";
                    for (int k = 0; k < num_assignments; ++k) {
                        cout << "  Assignment " << k + 1 << ": " << assignment[k] << endl;
                    }
                    cout << "\n-----------Midterms----------\n\n";
                    cout << "  Midterm 1: "<<midterm1 << " , Midterm 2: " << midterm2 << endl;
                    cout << "\n----------Final Exam---------\n\n";
                    cout << "  Final exam: "<<final_exam << endl;

                    infile.close();
                    return;
                }
            }

            infile.close();

            // If student ID is not found in the file
            cout << "ERROR: No marks found for student ID " << login_id << " in subject " << subject << ".\n";
            return;
        }
    }

    if (!subject_found) {
        cout << "ERROR: Subject not found or no valid data available.\n";
    }
}












// MAIN FUNCTION
int main() {
    cout << "\n|<>><><><><><><><>><><><><><><><><><><><><>><><( STUDENT MANAGEMENT SYSTEM )><><><><><><><><><><><><><><><><><><><><>|\n\n";
    
   
    int opt;

    // Load data from file when the program starts
    loadTeacherFromFile(teacher_name, teacher_subject, teacher_class, teacher_password, teacher_ID, current_teacher_count);
    loadStudentFromFile(student_ID, student_name, student_class, student_password, current_student_count);
    
    do {
        cout << "=========   Select any option   ==========\n\n"
            << "1. Admin\n2. Teacher\n3. Student\n4. Exit\n";
        cin >> opt;
        cin.ignore();

        switch (opt) {
        case 1: {  // ADMIN
            string pass;

            while (true) {
                cout << "Please enter the ADMIN PASSWORD: ";
                getline(cin, pass);

                if (pass == admin_pass) {
                    cout << "\n<--(  Login successful! Welcome, Admin  )-->\n";
                    break;
                }
                else {
                    cout << "ERROR: Incorrect password. Please try again.\n";
                }
            }

            int adminOpt;
            do {
                cout << "\n=========   Select any option   ==========\n\n"
                    << "1. ADD Teacher\n2. REMOVE Teacher\n3. Back to home\n";
                cin >> adminOpt;

                switch (adminOpt) {
                case 1:
                    addteacher(teacher_name, teacher_subject, teacher_class, teacher_password, teacher_ID, current_teacher_count, max_teacher);
                    break;
                case 2:
                    deleteteacher(teacher_name, teacher_subject, teacher_class, teacher_password, teacher_ID, current_teacher_count);
                    break;
                case 3:
                    break;  // Back to main menu
                default:
                    cout << "ERROR: Invalid option selection! Try again.\n";
                    break;
                }
            } while (adminOpt != 3);  // Loop until user selects "Back to home"
            break;
        }

        case 2: {  // TEACHER
            string login_id;
            string login_password;
            string t_subject;
            bool logged_in = false;  // Track login status

            while (!logged_in) {
                cout << "Enter Teacher ID: ";
                cin >> login_id;
                cin.ignore();

                cout << "Enter Password: ";
                getline(cin, login_password);

                // Validate credentials
                for (int i = 0; i < current_teacher_count; ++i) {
                    if (teacher_ID[i] == login_id && teacher_password[i] == login_password) {
                        cout << "\n<--(  Login successful! Welcome, '" << teacher_name[i] << "' )-->\n";
                        logged_in = true;
                        t_subject = teacher_subject[i];
                        break;
                    }
                }

                if (!logged_in) {
                    cout << "ERROR: Invalid ID or Password. Please try again.\n";
                }
            }
         
            subject_file_path = t_subject + ("_marks.txt");
            string subject_grade_file_path = t_subject + ("_grades.txt");
            string cgpa_file_path = "CGPA_file.txt";

            loadMarks(subject_file_path);
            string output_file_path = t_subject + ("_grades.txt");
            
            // Teacher menu
            int teacherOpt;
            do {
                cout << "\n=========   Select any option   ==========\n\n"
                    << "1. Add Student\n"
                    << "2. Delete Student\n"
                    << "3. Assign Marks\n"
                    << "4. Calculate grades\n"
                    << "5. Calculate CGPA\n"
                    << "6. Average CGPA\n"
                    << "7. List of Failed Students\n"
                    << "8. List of Passed Students\n"
                    << "9. View all students marks\n"
                    << "10. Log Out\n";
                cin >> teacherOpt;

                switch (teacherOpt) {
                case 1:
                    addstudent(student_ID, student_name, student_class, student_password, current_student_count, max_student);
                    break;
                case 2:
                    deletestudent(student_ID, student_name, student_class, student_password, current_student_count);
                    break;
                case 3:
                    assignmarks(subject_file_path);
                    break;
                case 4:
                    calculateGrades(subject_file_path, output_file_path);
                    break;
                case 5:
                    calculateCGPA();
                    break;
                case 6:
                    showAverageCGPA();
                    break;
                case 7:
                    listFailedStudents(cgpa_file_path);
                    break;
                case 8:
                    listPassedStudents(cgpa_file_path);
                    break;
                case 9:
                    viewMarks(subject_file_path, t_subject);
                    break;
                case 10:
                    cout << "\nLogging Out...\n\n";
                    break;
                default:
                    cout << "ERROR: Invalid option. Try again.\n";
                }
            } while (teacherOpt != 10);
            break;
        }

        case 3: {
            string login_id;
            string login_password;
            bool logged_in = false; // Track login status
            

            while (!logged_in) {
                cout << "Enter Student ID: ";
                cin >> login_id;
                cin.ignore();

                cout << "Enter Password: ";
                getline(cin, login_password);

                // Validate credentials
                for (int i = 0; i < current_student_count; ++i) {
                    if (student_ID[i] == login_id && student_password[i] == login_password) {
                        cout << "\n<--(   Login successful! Welcome, '" << student_name[i] << "'  )-->\n";
                        logged_in = true;
                       
                        break;
                    }
                }

                if (!logged_in) {
                    cout << "ERROR: Invalid ID or Password. Please try again: \n";
                }
            }

            // Student menu
            
            int subject_count = 4;
            int StudentOpt;
            do {
                cout << "\n=========   Select any option   ==========\n\n"
                    << "1. View Marks\n"
                    << "2. View Grades\n"
                    << "3. View CGPA\n"
                    << "4. Log Out\n";
                cin >> StudentOpt;

                switch (StudentOpt) {
                case 1:
                    viewStudentMarks(login_id);
                    break;
                case 2:
                    viewStudentGrades(login_id, subject_names,subject_count);
                    break;
                case 3:
                    viewStudentCGPA(login_id);
                    break;
                case 4:
                    cout << "Logging out...\n";
                    break;
                default: 
                    cout << "ERROR: Invalid selection try again: ";
                      
               }
            } while (StudentOpt != 4);
            
            break;
        }
        case 4: {
            cout << "Exiting the system!\n";
            break;
        }
        default:
            cout << "ERROR: Invalid option selection! Try again.\n\n";
            break;
        }

    } while (opt != 4);  // Keep showing the main menu until "Exit" is selected

    return 0;
}
