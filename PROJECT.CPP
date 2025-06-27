#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>
#include <ctime>
#include <iomanip>
using namespace std;

// Time simulation
int currentTime = 0;

// Patient Structure 
struct Patient {
    string name;
    int age;
    int severity;
    int arrivalTime;
    string problemType;
    string assignedDoctor;

    // Default constructor
    Patient() {
        name = "";
        age = 0;
        severity = 0;
        arrivalTime = 0;
        problemType = "";
        assignedDoctor = "";
    }

    // Parameterized constructor
    Patient(string n, int a, int s, string prob, string doc) {
        name = n;
        age = a;
        severity = s;
        problemType = prob;
        assignedDoctor = doc;
        arrivalTime = currentTime++;
    }
};


// Comparator 
struct ComparePatients {
    bool operator()(const Patient& p1, const Patient& p2) {
        if (p1.severity == p2.severity)
            return p1.arrivalTime > p2.arrivalTime; // Earlier comes first
        return p1.severity > p2.severity; // Lower severity = higher priority
    }
};

// Global Structures
priority_queue<Patient, vector<Patient>, ComparePatients> patientQueue;
unordered_map<string, Patient> patientRecords;
queue<Patient> treatedHistory;

// Doctor Mapping and Availability
unordered_map<string, string> doctorMap = {
    {"fever", "Dr. Mehta (General Physician)"},
    {"fracture", "Dr. Kapoor (Orthopedic)"},
    {"chest pain", "Dr. Sharma (Cardiologist)"},
    {"burn", "Dr. Sinha (Plastic Surgeon)"},
    {"headache", "Dr. Rao (Neurologist)"},
    {"pregnancy", "Dr. Rathi (Gynecologist)"}
};

unordered_map<string, bool> doctorAvailability = {
    {"Dr. Mehta (General Physician)", true},
    {"Dr. Kapoor (Orthopedic)", true},
    {"Dr. Sharma (Cardiologist)", true},
    {"Dr. Sinha (Plastic Surgeon)", true},
    {"Dr. Rao (Neurologist)", true},
    {"Dr. Rathi (Gynecologist)", true}
};

// Functions 

void registerPatient() {
    string name, problem;
    int age, severity;

    cout << "\nEnter Patient Name: ";
    cin >> name;
    cout << "Enter Age: ";
    cin >> age;
    cout << "Enter Severity (1 = Critical to 5 = Low): ";
    cin >> severity;
    cin.ignore();
    cout << "Enter Problem Type (e.g., fever, chest pain, fracture): ";
    getline(cin, problem);

    string doctor = "General Doctor";
    if (doctorMap.find(problem) != doctorMap.end()) {
        doctor = doctorMap[problem];
    }

    string status = (doctorAvailability[doctor]) ? "Available" : "Busy";
    Patient p(name, age, severity, problem, doctor);
    patientQueue.push(p);
    patientRecords[name] = p;

    cout << "Patient Registered Successfully.\n";
    cout << "Assigned Doctor: " << doctor << " — Status: " << status << endl;
}

void viewQueue() {
    if (patientQueue.empty()) {
        cout << "\nNo patients in queue.\n";
        return;
    }

    priority_queue<Patient, vector<Patient>, ComparePatients> tempQueue = patientQueue;
    cout << "\n Current Patient Queue:\n";
    while (!tempQueue.empty()) {
        Patient p = tempQueue.top();
        tempQueue.pop();
        cout << "- " << p.name << " | Age: " << p.age
             << " | Severity: " << p.severity
             << " | Doctor: " << p.assignedDoctor
             << " | Arrived: " << p.arrivalTime << endl;
    }
}

void treatNextPatient() {
    if (patientQueue.empty()) {
        cout << "\nNo patients to treat.\n";
        return;
    }

    Patient p = patientQueue.top();
    patientQueue.pop();
    treatedHistory.push(p);
    doctorAvailability[p.assignedDoctor] = false;

    cout << "\n Treating Patient: " << p.name << endl;
    cout << "Problem: " << p.problemType << " | Severity: " << p.severity << endl;
    cout << "Doctor: " << p.assignedDoctor << " — Marked as BUSY\n";
}

void markDoctorAvailable() {
    string doc;
    cout << "\nEnter Doctor's Full Name (as shown in assignment): ";
    cin.ignore();
    getline(cin, doc);

    if (doctorAvailability.find(doc) != doctorAvailability.end()) {
        doctorAvailability[doc] = true;
        cout  << doc << " is now AVAILABLE.\n";
    } else {
        cout << " Doctor not found.\n";
    }
}

void viewDoctorAvailability() {
    cout << "\n Doctor Availability:\n";
    for (auto& pair : doctorAvailability) {
        cout << "- " << pair.first << " : " << (pair.second ? "Available" : "Busy") << endl;
    }
}

void viewTreatmentHistory() {
    if (treatedHistory.empty()) {
        cout << "\nNo patients have been treated yet.\n";
        return;
    }

    queue<Patient> temp = treatedHistory;
    cout << "\n Treated Patients:\n";
    while (!temp.empty()) {
        Patient p = temp.front();
        temp.pop();
        cout << "- " << p.name << " | Problem: " << p.problemType
             << " | Doctor: " << p.assignedDoctor << endl;
    }
}

// Main Menu 
int main() {
    int choice;
    cout << " Welcome to SmartER: Emergency Room Queue Manager\n";

    do {
        cout << "\n====== Main Menu ======\n";
        cout << "1. Register New Patient\n";
        cout << "2. View Patient Queue\n";
        cout << "3. Treat Next Patient\n";
        cout << "4. Mark Doctor as Available\n";
        cout << "5. View Doctor Availability\n";
        cout << "6. View Treatment History\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: registerPatient(); break;
            case 2: viewQueue(); break;
            case 3: treatNextPatient(); break;
            case 4: markDoctorAvailable(); break;
            case 5: viewDoctorAvailability(); break;
            case 6: viewTreatmentHistory(); break;
            case 7: cout << " Exiting system. Stay healthy!\n"; break;
            default: cout << " Invalid choice. Try again.\n";
        }

    } while (choice != 7);

    return 0;
}
