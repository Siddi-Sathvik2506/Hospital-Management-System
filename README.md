# 🏥 Hospital Management System (HMS)

A **console-based Hospital Management System** developed in **ANSI C**, designed to manage core hospital operations using **binary files** for persistent storage. The system supports **role-based access** and runs completely offline without any external database.

## 📌 Overview

* **Language:** ANSI C
* **Interface:** Console-based
* **Storage:** Binary files (`.dat`)
* **Execution:** Local machine
* **Database:** Not required

### 👥 Supported Roles

* **Admin**
* **Doctor**
* **Pharmacy Owner**

Each role has its own dedicated menu and access privileges.

## ✨ Features

### 🔑 Admin

* Add, view, update, delete, and discharge patients
* Add and view doctors
* Book and view appointments
* Allocate and free hospital beds
* View bed occupancy report
* Generate patient billing records
* Manage medicine inventory
* Manage laboratory results
* Trigger daily system report

### 🩺 Doctor

* View patient details
* View assigned appointments
* View laboratory test results

### 💊 Pharmacy Owner

* Add medicines
* View medicine inventory (quantity & expiry)

---

## 🧱 Data Structures

All entities are implemented as **C structures** and stored in separate binary files.

| Entity      | Struct Name   | Key Fields                                      |
| ----------- | ------------- | ----------------------------------------------- |
| Patient     | `Patient`     | id, name, age, gender, disease, admitted, bedNo |
| Doctor      | `Doctor`      | id, name, specialization                        |
| Bed         | `Bed`         | bedNo, occupied, patientId                      |
| Appointment | `Appointment` | patientId, doctorId, date                       |
| Bill        | `Bill`        | patientId, consultation, medicine, room, total  |
| Medicine    | `Medicine`    | id, name, quantity, expiry                      |
| Lab         | `Lab`         | patientId, testName, result                     |
| User        | `User`        | username, password, role                        |

---

## 💾 Storage Files

Each module persists data in its own `.dat` file:

| File Name          | Description              |
| ------------------ | ------------------------ |
| `patients.dat`     | Patient records          |
| `doctors.dat`      | Doctor records           |
| `beds.dat`         | Bed occupancy data       |
| `appointments.dat` | Appointment records      |
| `billing.dat`      | Billing records          |
| `medicines.dat`    | Medicine inventory       |
| `labs.dat`         | Laboratory results       |
| `users.dat`        | User credentials & roles |

---

## ⚙️ Initialization

On the **first run**, the system automatically initializes core data:

### 🛏 Beds

* Creates **20 beds**
* All beds are marked **free**

### 👤 Default Users

| Username | Password  | Role           |
| -------- | --------- | -------------- |
| admin    | admin123  | Admin          |
| doctor   | doc123    | Doctor         |
| pharmacy | pharma123 | Pharmacy Owner |

---

## 🔐 Authentication & Menus

* User credentials are validated from `users.dat`
* Role-based menu is displayed after login
* Menu runs in a loop until **Exit** is selected

---

## 🧩 Core Modules & Key Functions

### Patient Management

* `addPatient()`
* `viewPatients()`
* `updatePatient()`
* `deletePatient()`
* `dischargePatient()`

> Admission automatically assigns the **first available bed**

---

### Doctor Management

* `addDoctor()`
* `viewDoctors()`

---

### Appointment Management

* `bookAppointment()`
* `viewAppointments()`

---

### Bed Management

* `initBeds()`
* `allocateBed(int patientId)`
* `freeBed(int bedNo)`
* `bedReport()`

---

### Billing

* `generateBill()`

> Calculates consultation, medicine, and room charges and stores them in `billing.dat`

---

### Pharmacy

* `addMedicine()`
* `viewMedicines()`

---

### Laboratory

* `addLabResult()`
* `viewLabResults()`

---

### Reports

* `dailyReport()`
  (Currently prints a simple system success message)

---

## ▶️ Build & Run

### Compile

```bash
gcc -o hms "Hospital management system.c"
```

### Run

```bash
./hms
```

---

## 📘 Quick Function Reference
+--------------+----------------------------------------------------------------------------------------------+
| Area         | Functions                                                                                    |
| ------------ | -------------------------------------------------------------------------------------------- |
| Setup        | `initBeds()`, `initUsers()`, `login()`                                                       |
| Patients     | `addPatient()`, `viewPatients()`, `updatePatient()`, `deletePatient()`, `dischargePatient()` |
| Doctors      | `addDoctor()`, `viewDoctors()`                                                               |
| Appointments | `bookAppointment()`, `viewAppointments()`                                                    |
| Beds         | `allocateBed()`, `freeBed()`, `bedReport()`                                                  |
| Billing      | `generateBill()`                                                                             |
| Pharmacy     | `addMedicine()`, `viewMedicines()`                                                           |
| Lab          | `addLabResult()`, `viewLabResults()`                                                         |
| Reports      | `dailyReport()`                                                                              |
+--------------+----------------------------------------------------------------------------------------------+
