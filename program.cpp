#include <iostream>
#include <string>
#include "splashkit.h"
#include "utilities.h"

using namespace std;

// Constant declaration
const int MAX_STUDENTS = 100;
const int MAX_TUTORS = 100;
const int MAX_TIMESLOTS = 10;
const int MAX_SESSIONS = 100;
const int BOX_WIDTH = 75.;

// Enum and struct declaration
enum DayOfWeek
{
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY
};

enum Subject
{
    SIT102,
    SIT111,
    SIT182,
    SIT190,
    NO_SUBJECT
};

enum AvailabilityStatus
{
    AVAILABLE,
    BOOKED,
    UNAVAILABLE
};

struct TimeSlot
{
    DayOfWeek day;
    int startTime;
    int endTime;
    AvailabilityStatus status;
};

struct Student
{
    string name;
    string email;
    string course;
    Subject subjects[4];
};

struct Tutor
{
    string name;
    string email;
    string biography;
    Subject subjects[4];
    TimeSlot availability[MAX_TIMESLOTS];
};

struct Session
{
    Student student;
    Tutor tutor;
    TimeSlot timeSlot;
};

// Initialising arrays after declaration of structs and enums

Student students[MAX_STUDENTS];
Tutor tutors[MAX_TUTORS];
Session sessions[MAX_SESSIONS];

// Initialise Students, Tutors, and Sessions
int numStudents = 0;
int numTutors = 0;
int numSessions = 0;

// Create studentName variable to be referenced to throughout program
string studentName;

// Handles changing enum values to strings to print appropriately to the terminal
string subjectToString(Subject subject)
{
    switch (subject)
    {
    case SIT102:
        return "SIT102";
    case SIT111:
        return "SIT111";
    case SIT182:
        return "SIT182";
    case SIT190:
        return "SIT190";
    default:
        return "";
    }
}

// To handle any text that writes beyond the bounds of the program

void printWrappedText(const string &text)
{
    string remainingText = text;
    while (remainingText.length() > BOX_WIDTH)
    {
        size_t spacePos = remainingText.rfind(' ', BOX_WIDTH);
        if (spacePos != string::npos)
        {
            write_line(remainingText.substr(0, spacePos));
            remainingText.erase(0, spacePos + 1); // Skip the space
        }
        else
        {
            write_line(remainingText.substr(0, BOX_WIDTH));
            remainingText.erase(0, BOX_WIDTH);
        }
    }
    if (!remainingText.empty())
    {
        write_line(remainingText);
    }
}

// Handles converting the DayOfWeek enum to a string for printing in the terminal
string dayOfWeekToString(DayOfWeek day)
{
    switch (day)
    {
    case MONDAY:
        return "Monday";
    case TUESDAY:
        return "Tuesday";
    case WEDNESDAY:
        return "Wednesday";
    case THURSDAY:
        return "Thursday";
    case FRIDAY:
        return "Friday";
    default:
        return "";
    }
}

// For easy readability in terminal 
string timeToString(int time)
{
    int hour = time / 100;
    int minute = time % 100;
    return to_string(hour) + ":" + (minute < 10 ? "0" : "") + to_string(minute); //Formatting of time element
}

string confirmSession(const Student &student, const Tutor &tutor, const TimeSlot &timeSlot, bool isBookingSuccessful)
{
    write_line("╔══════════════════════════════════════════════════════════════════════════╗");
    write_line("║                          Session Confirmation                            ║");
    write_line("╚══════════════════════════════════════════════════════════════════════════╝");

    if (isBookingSuccessful)
    {
        return "Session Confirmation: Booking Successful!\nStudent: " + student.name + "\nTutor: " + tutor.name + "\nTime: " + timeToString(timeSlot.startTime) + " - " + timeToString(timeSlot.endTime);
    }
    else
    {
        return "Session Confirmation: Booking Failed.\nThe selected time slot is already taken. Please choose another time.";
    }
}

void viewSession(const string &studentEmail)
{
    write_line("╔══════════════════════════════════════════════════════════════════════════╗");
    write_line("║                             Booked Session                               ║");
    write_line("╚══════════════════════════════════════════════════════════════════════════╝");

    bool found = false;
    for (int i = 0; i < numSessions; i++)
    {
        if (sessions[i].student.email == studentEmail)
        {
            write_line(to_string(i) + ". Tutor: " + sessions[i].tutor.name);
            write_line("   Day: " + dayOfWeekToString(sessions[i].timeSlot.day));
            write_line("   Time: " + timeToString(sessions[i].timeSlot.startTime) + " - " + timeToString(sessions[i].timeSlot.endTime));
            found = true;
        }
    }
    if (!found)
    {
        write_line("You haven't booked any sessions yet!");
    }
}

void registerStudent()
{

    write_line("╔══════════════════════════════════════════════════════════════════════════╗");
    write_line("║                          Student Registration                            ║");
    write_line("╚══════════════════════════════════════════════════════════════════════════╝");

    if (numStudents >= MAX_STUDENTS)
    {
        write_line("Our apologies! Student registration is full.");
        return;
    }

    Student newStudent;
    write_line("Please enter your name: ");
    newStudent.name = read_line();
    write_line("Please enter your student email: ");
    newStudent.email = read_line();
    write_line("Please enter your course: ");
    newStudent.course = read_line();

    write_line("Please enter your units: ");
    string subjectsInput;
    subjectsInput = read_line();

    // NO_SUBJECT initialisaion for quality of life
    for (int i = 0; i < 4; i++)
    {
        newStudent.subjects[i] = NO_SUBJECT;
    }

    // Extract subject codes
    size_t pos = 0;
    string delimiter = ",";
    int index = 0;
    while ((pos = subjectsInput.find(delimiter)) != string::npos && index < 4)
    {
        string subjectCode = subjectsInput.substr(0, pos);
        if (subjectCode.substr(0, 3) == "SIT") // Check if the string starts with "SIT"
        {
            newStudent.subjects[index++] = static_cast<Subject>(stoi(subjectCode.substr(3)));
        }
        subjectsInput.erase(0, pos + delimiter.length());
    }
    if (subjectsInput.substr(0, 3) == "SIT" && index < 4)
    {
        newStudent.subjects[index] = static_cast<Subject>(stoi(subjectsInput.substr(3))); // Exclude "SIT"
    }

    students[numStudents++] = newStudent;
    write_line("Student registered successfully!");

    studentName = newStudent.name;

    bool hasBookedSessions = false;
    for (int i = 0; i < numSessions; i++)
    {
        if (sessions[i].student.email == newStudent.email)
        {
            hasBookedSessions = true;
            break;
        }
    }

    if (hasBookedSessions)
    {
        viewSession(newStudent.email);
    }
}

void bookSession(const string &studentEmail)
{
    write_line("╔══════════════════════════════════════════════════════════════════════════╗");
    write_line("║                              Book Session                                ║");
    write_line("╚══════════════════════════════════════════════════════════════════════════╝");

    static bool tutorPrinted[MAX_TUTORS] = {false};

    int subjectChoice;
    write_line("Select subject (0: SIT102, 1: SIT111, 2: SIT182, 3: SIT190): ");
    subjectChoice = stoi(read_line());

    // Find tutors who teach the selected subject
    Subject selectedSubject = static_cast<Subject>(subjectChoice);
    write_line("Available tutors for " + subjectToString(selectedSubject) + ":");
    bool found = false;

    for (int i = 0; i < numTutors; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (tutors[i].subjects[j] == selectedSubject)
            {
                if (!tutorPrinted[i])
                {
                    write_line("Tutor Name: " + tutors[i].name + ", Email: " + tutors[i].email);
                    tutorPrinted[i] = true;
                    found = true;
                }
                break;
            }
        }
    }

    if (!found)
    {
        write_line("Sorry, no tutors were found for the selected subject!");
        return;
    }

    for (int i = 0; i < MAX_TUTORS; i++)
    {
        tutorPrinted[i] = false;
    }

    // Let the student select a tutor
    string tutorEmail;
    write_line("Enter the email of the tutor you want to book: ");
    tutorEmail = read_line();

    Tutor *tutor = nullptr;

    // Find the selected tutor
    for (int i = 0; i < numTutors; i++)
    {
        if (tutors[i].email == tutorEmail)
        {
            tutor = &tutors[i];
            break;
        }
    }

    if (tutor == nullptr)
    {
        write_line("Tutor not found.");
        return;
    }

    // Loop until a valid day is selected
    int dayChoice;
    DayOfWeek selectedDay;
    bool validDaySelected = false;
    do
    {
        write_line("Select day (0: Monday, 1: Tuesday, 2: Wednesday, 3: Thursday, 4: Friday): ");
        dayChoice = stoi(read_line());

        // Validate the day choice
        if (dayChoice < 0 || dayChoice > 4)
        {
            write_line("Oops, this day isn't available. Please select a day from the list.");
            continue;
        }

        selectedDay = static_cast<DayOfWeek>(dayChoice);

        write_line("Available tutoring sessions for " + tutor->name + " on " + dayOfWeekToString(selectedDay) + ":");

        bool tutorInfoPrinted = false;
        bool availableSlots = false;

        // Loop to print available time slots
        for (int i = 0; i < MAX_TIMESLOTS; i++)
        {
            if (tutor->availability[i].day == selectedDay &&
                tutor->availability[i].status == AVAILABLE &&
                tutor->availability[i].startTime != 0 &&
                tutor->availability[i].endTime != 0)
            {
                if (!tutorInfoPrinted)
                {
                    write_line("Tutor Name: " + tutor->name + ", Email: " + tutor->email);
                    tutorInfoPrinted = true;
                }
                write_line(to_string(i) + ". Time: " + timeToString(tutor->availability[i].startTime) + " - " + timeToString(tutor->availability[i].endTime));
                availableSlots = true;
            }
        }
        if (!availableSlots)
        {
            string tryAnotherDay;
            write_line("We're sorry, but " + tutor->name + " isn't available on this day! Would you like to try another day? (yes/no): ");
            tryAnotherDay = read_line();

            if (tryAnotherDay != "yes" && tryAnotherDay != "Yes")
            {
                return;
            }
        }
        else
        {
            validDaySelected = true;
        }
    } while (!validDaySelected);

    // Time slot selection
    int timeSlotChoice;
    write_line("Enter the session you want to book: ");
    timeSlotChoice = stoi(read_line());

    if (timeSlotChoice < 0 || timeSlotChoice >= MAX_TIMESLOTS ||
        tutor->availability[timeSlotChoice].day != selectedDay ||
        tutor->availability[timeSlotChoice].status != AVAILABLE)
    {
        cout << "We're sorry! You can't select this slot!" << endl;
        return;
    }

    // Book the session
    tutor->availability[timeSlotChoice].status = BOOKED;
    Session newSession = {students[0], *tutor, tutor->availability[timeSlotChoice]};
    sessions[numSessions++] = newSession;

    string confirmationMessage = confirmSession(students[0], *tutor, tutor->availability[timeSlotChoice], true);
    write_line(confirmationMessage);
}

void cancelSession(const string &studentEmail)
{
    write_line("╔══════════════════════════════════════════════════════════════════════════╗");
    write_line("║                             Cancel Session                               ║");
    write_line("╚══════════════════════════════════════════════════════════════════════════╝");

    bool foundSession = false;

    for (int i = 0; i < numSessions; i++)
    {
        if (sessions[i].student.email == studentEmail)
        {
            foundSession = true;
            break;
        }
    }

    if (!foundSession) 
    {
        write_line("You haven't booked any sessions yet!");
        return;
    }

    viewSession(studentEmail);

    int sessionIndex;
    write_line("Select the session you want to cancel: ");
    sessionIndex = stoi(read_line());

    if (sessionIndex < 0 || sessionIndex >= numSessions || sessions[sessionIndex].student.email != studentEmail)
    {
        write_line("Invalid session selection.");
        return;
    }
    // Find the tutor for the selected session
    Tutor *tutor = nullptr;
    for (int i = 0; i < numTutors; i++)
    {
        if (tutors[i].email == sessions[sessionIndex].tutor.email)
        {
            tutor = &tutors[i];
            break;
        }
    }

    if (tutor == nullptr)
    {
        write_line("Tutor not found.");
        return;
    }

    // Mark the tutor's timeslot as available again
    for (int j = 0; j < MAX_TIMESLOTS; j++)
    {
        if (tutor->availability[j].day == sessions[sessionIndex].timeSlot.day &&
            tutor->availability[j].startTime == sessions[sessionIndex].timeSlot.startTime &&
            tutor->availability[j].endTime == sessions[sessionIndex].timeSlot.endTime)
        {
            tutor->availability[j].status = AVAILABLE;
            break;
        }
    }

    // Shift sessions array to remove the canceled session
    for (int j = sessionIndex; j < numSessions - 1; j++)
    {
        sessions[j] = sessions[j + 1];
    }
    numSessions--;

    write_line("Session cancelled successfully.");
}

void searchBySubject()
{
    write_line("╔══════════════════════════════════════════════════════════════════════════╗");
    write_line("║                            Filter by Subject                             ║");
    write_line("╚══════════════════════════════════════════════════════════════════════════╝");

    int subject;
    write_line("Enter subject to search for (0: SIT102, 1: SIT111, 2: SIT182, 3: SIT190): ");
    subject = stoi(read_line());

    Subject searchSubject = static_cast<Subject>(subject);
    bool found = false;

    for (int i = 0; i < numTutors; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (tutors[i].subjects[j] == searchSubject)
            {
                write_line("Tutor Name: " + tutors[i].name + ", Email: " + tutors[i].email);
                found = true;
                break;
            }
        }
    }

    if (!found)
    {
        write_line("No tutors found for the selected subject.");
    }
}

void searchByAvail()
{
    write_line("╔══════════════════════════════════════════════════════════════════════════╗");
    write_line("║                         Filter by Availability                           ║");
    write_line("╚══════════════════════════════════════════════════════════════════════════╝");

    int day;
    printWrappedText("Enter day to search for availability (0: Monday, 1: Tuesday, 2: Wednesday, 3: Thursday, 4: Friday): ");
    day = stoi(read_line());

    bool found = false;

    for (int i = 0; i < numTutors; i++)
    {
        bool isAvailable = false;
        for (int j = 0; j < MAX_TIMESLOTS; j++)
        {
            if (tutors[i].availability[j].day == static_cast<DayOfWeek>(day) &&
                tutors[i].availability[j].status == AVAILABLE)
            {
                isAvailable = true;
                break;
            }
        }

        if (isAvailable)
        {
            write_line("Tutor Name: " + tutors[i].name + ", Email: " + tutors[i].email + ", Subjects: ");
            bool firstSubject = true;
            for (int j = 0; j < 4; j++)
            {
                if (tutors[i].subjects[j] != NO_SUBJECT)
                {
                    if (!firstSubject)
                    {
                        write(", ");
                    }
                    write(subjectToString(tutors[i].subjects[j]));
                    firstSubject = false;
                }
            }
            write_line();
            found = true;
        }
    }

    if (!found)
    {
        write_line("No tutors available on the selected day.");
    }
}

void browseTutors()
{
    write_line("╔══════════════════════════════════════════════════════════════════════════╗");
    write_line("║                             Browse Tutors                                ║");
    write_line("╚══════════════════════════════════════════════════════════════════════════╝");

    for (int i = 0; i < numTutors; ++i)
    {
        write_line("Tutor Name: " + tutors[i].name);
        write_line("Subjects: ");
        for (int j = 0; j < 4; ++j)
        {
            if (tutors[i].subjects[j] != NO_SUBJECT)
            {
                write(subjectToString(tutors[i].subjects[j]) + ", ");
            }
        }
        write_line();
        write_line("Biography: ");
        printWrappedText(tutors[i].biography);
        write_line("Availability: ");

        bool availableSlotsFound = false;
        for (int j = 0; j < MAX_TIMESLOTS; ++j)
        {
            if (tutors[i].availability[j].status == AVAILABLE &&
                tutors[i].availability[j].startTime != 0 &&
                tutors[i].availability[j].endTime != 0)
            {
                write_line("- " + dayOfWeekToString(tutors[i].availability[j].day) + " " + timeToString(tutors[i].availability[j].startTime) + " - " + timeToString(tutors[i].availability[j].endTime));
                availableSlotsFound = true;
            }
        }
        if (!availableSlotsFound)
        {
            write_line("No available slots");
        }
        write_line("----------------------------------------------------------------------------");
    }

    write_line("╚══════════════════════════════════════════════════════════════════════════╝");
}

int main()
{
    // Tutor data for demonstration purposes
    Tutor tutor1 = {
        "Jason Daniels",
        "jdaniels@edu.au",
        "Jason Daniels has been a university tutor for 2 years and specialises in programming and computer systems. He enjoys helping others and exploring Melbourne on the weekends!",
        {SIT102, SIT111, NO_SUBJECT, NO_SUBJECT},
        {{MONDAY, 900, 1100, AVAILABLE},
         {TUESDAY, 1000, 1200, AVAILABLE},
         {WEDNESDAY, 1300, 1500, AVAILABLE},
         {THURSDAY, 0, 0, UNAVAILABLE},
         {FRIDAY, 0, 0, UNAVAILABLE}}};

    Tutor tutor2 = {
        "Annie Thompson",
        "athompson@edu.au",
        "Annie Thompson is a newly graduated Cyber Security student specialises in cyber security and networks. She loves ",
        {SIT182, NO_SUBJECT, NO_SUBJECT, NO_SUBJECT},
        {{MONDAY, 0, 0, UNAVAILABLE},
         {TUESDAY, 0, 0, UNAVAILABLE},
         {WEDNESDAY, 0, 0, UNAVAILABLE},
         {THURSDAY, 1000, 1200, AVAILABLE},
         {FRIDAY, 1400, 1600, AVAILABLE}}};

    Tutor tutor3 = {
        "Harry Long",
        "hlong@edu.au",
        "Harry Long is a volunteer who enjoys dedicating his spare time to helping out undergraduate students in mathematics. He likes taking walks and eating at cafe's.",
        {SIT190, NO_SUBJECT, NO_SUBJECT, NO_SUBJECT},
        {{MONDAY, 0, 0, UNAVAILABLE},
         {TUESDAY, 1400, 1600, AVAILABLE},
         {WEDNESDAY, 0, 0, UNAVAILABLE},
         {THURSDAY, 0, 0, UNAVAILABLE},
         {FRIDAY, 1000, 1200, AVAILABLE},
         {FRIDAY, 1300, 1500, AVAILABLE}}};

    Tutor tutor4 = {
        "Josh James",
        "jjames@edu.au",
        "Josh James is a friendly face on campus, and loves to help other students in many different specialties. He is always spending his free time teaching his cats fun tricks and reading.",
        {SIT102, SIT111, SIT190, NO_SUBJECT},
        {{MONDAY, 1000, 1200, AVAILABLE},
         {MONDAY, 1300, 1500, AVAILABLE},
         {TUESDAY, 0, 0, UNAVAILABLE},
         {WEDNESDAY, 900, 1100, AVAILABLE},
         {WEDNESDAY, 1200, 1400, AVAILABLE},
         {THURSDAY, 0, 0, UNAVAILABLE},
         {FRIDAY, 0, 0, UNAVAILABLE}}};
        
    Tutor tutor5 = {
        "Jessica Doe",
        "jdoe@edu.au",
        "Jessica Doe is a long-term Cyber Security tutor and has 10 years industry experience. She enjoys helping others, and spending time with her pets.",
        {SIT182, SIT190, NO_SUBJECT, NO_SUBJECT},
        {{MONDAY, 1400, 1600, AVAILABLE},
         {TUESDAY, 1400, 1600, AVAILABLE},
         {WEDNESDAY, 1000, 1200, AVAILABLE},
         {THURSDAY, 1000, 1200, AVAILABLE},
         {THURSDAY, 1300, 1500, AVAILABLE},
         {THURSDAY, 1600, 1800, AVAILABLE},
         {FRIDAY, 0, 0, UNAVAILABLE}}};

    // Initialising tutors to the array
    tutors[numTutors++] = tutor1;
    tutors[numTutors++] = tutor2;
    tutors[numTutors++] = tutor3;
    tutors[numTutors++] = tutor4;
    tutors[numTutors++] = tutor5;

    int choice;

    do
    {
        if (!studentName.empty())
        {
            write_line("Welcome " + studentName + ", to your Tutoring Network!");
        }

        write_line("╔══════════════════════════════════════════════════════════════════════════╗");
        write_line("║                         Student Peer Tutoring Network                    ║");
        write_line("╠══════════════════════════════════════════════════════════════════════════╣");
        write_line("║                                                                          ║");
        write_line("║     1. Register Account                                                  ║");
        write_line("║     2. Book Session                                                      ║");
        write_line("║     3. Cancel Session                                                    ║");
        write_line("║     4. Filter By Subject                                                 ║");
        write_line("║     5. Filter By Availability                                            ║");
        write_line("║     6. View Tutors                                                       ║");
        write_line("║     7. View Session                                                      ║");
        write_line("║     8. Exit                                                              ║");
        write_line("║                                                                          ║");
        write_line("╚══════════════════════════════════════════════════════════════════════════╝");
        write_line("Please select an option: ");
        choice = stoi(read_line());

        switch (choice)
        {
        case 1:
            registerStudent();
            break;
        case 2:
            if (numStudents > 0)
            {
                bookSession(students[numStudents - 1].email);
            }
            else
            {
                write_line("No students registered yet.");
            }
            break;
        case 3:
            if (numStudents > 0)
            {
                cancelSession(students[numStudents - 1].email);
            }
            else
            {
                write_line("No students registered yet.");
            }
            break;
        case 4:
            searchBySubject();
            break;
        case 5:
            searchByAvail();
            break;
        case 6:
            browseTutors();
            break;
        case 7:
            if (numStudents > 0)
            {
                viewSession(students[numStudents - 1].email);
            }
            else
            {
                write_line("No students registered yet.");
            }
            break;
        case 8:
            write_line("Thank you for using the Student Peer Tutoring Network. Have a great day!");
            break;
        default:
            write_line("Invalid choice. Please try again.");
        }
    } while (choice != 8);

    return 0;
}