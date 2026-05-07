# 🍽️ Automated Restaurant Management System (ARMS)

A high-performance simulation engine developed for the **Data Structures & Algorithms** course (Spring 2026). This project models complex restaurant workflows using optimized, custom-built data structures.

---

## 👨‍💻 Team Members
* **Mohamed Ahmed Mohamed**
* **Yousuf Safwat**
* **Anton Azer**

---

## 📌 Project Overview
This system simulates a multi-channel restaurant environment supporting **Dine-in**, **Takeaway**, and **Delivery** logistics. It focuses on the mathematical optimization of resource allocation—specifically chefs, tables, and scooters—through event-driven simulation.

### Core Capabilities:
- **Chef & Scooter Dispatch:** Logic-based assignment and maintenance tracking.
- **Table Management:** Best-fit allocation and sharing support.
- **Order Lifecycle:** From placement and cooking to delivery and cancellation.
- **Reporting:** Automatic generation of detailed performance statistics.

---

## 🚀 Key Features

### ✅ Advanced Order Handling
- **Dine-In (OD):** Integrated with the table management system.
- **Takeaway (OT):** High-velocity queueing for immediate customer pickup.
- **Delivery (OV):** Complex routing including:
  - *Grilled Orders* (Specialist preparation)
  - *Normal Orders* (Standard preparation)
  - *Cold Delivery* (Specific transport logic)

### ✅ Smart Resource Assignment
- **Chefs:** Categorized into **Special (CS)** and **Normal (CN)**. The system uses a priority-based strategy to assign chefs based on the order's requirements and current kitchen load.
- **Scooters:** Assigned based on the **Shortest Traveled Distance** logic to minimize wait times. Includes a maintenance simulation after a specific number of trips.
- **Tables:** Implements a **Best-Fit Strategy** to maximize seating capacity and supports table sharing for eligible orders.

### ✅ Simulation Modes
- **Interactive Mode:** Timestep-by-timestep visualization of the restaurant state for debugging.
- **Silent Mode:** High-speed execution that outputs a comprehensive performance statistics file.

---

## 🧠 Data Structure Architecture
To ensure maximum efficiency, the project avoids standard libraries in favor of custom-engineered structures:

| Data Structure | Implementation Use Case |
| :--- | :--- |
| **Linked Queue** | Standard FCFS pending lists (Takeaway/Normal). |
| **Priority Queue** | Handling **OVG (Urgent)** orders, Table Best-Fit selection, and Scooter assignment. |
| **Augmented List** | Custom cancellation logic for orders in the pending/cooking phase. |
| **In-Service PQ** | Tracking completion times to trigger "Ready" events. |

---

## 📊 Statistics & Reporting
The system generates a final report including:
- **Wait Time ($T_w$):** Delay before service begins.
- **Service Time ($T_s$):** Active preparation or delivery duration.
- **Utilization Rates:** Efficiency percentages for both Chefs and Scooters.
- **Ratios:** Successful vs. Cancelled order comparisons.

---

## 🎯 Learning Objectives
- **Object-Oriented Programming:** Modular design for orders, staff, and resources.
- **Algorithm Optimization:** Transitioning from $O(n)$ searches to $O(\log n)$ heap operations.
- **Simulation Logic:** Synchronizing concurrent events via a global time-step.
