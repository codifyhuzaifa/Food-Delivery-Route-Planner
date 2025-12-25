# 🍔 Food Delivery Route Planner (C++ | DSA Project)

A **C++ based Food Delivery Route Planner** that simulates real-world food delivery operations using **graph algorithms and data structures**.  
The system allows customers to place single or multiple orders, calculates optimized delivery routes, applies traffic conditions dynamically, and generates complete bills.

---

## 🚀 Features

- 📍 **Shortest Route Calculation**
  - Uses **Dijkstra’s Algorithm** to compute optimal delivery paths.
- 🚦 **Traffic Factor (Unique Feature)**
  - Dynamically adjusts delivery time based on traffic conditions.
- 👥 **Single & Multi-Customer Orders**
  - Supports group deliveries (up to 5 customers).
- 🧾 **Automated Billing System**
  - Subtotal, discounts, delivery charges, and final bill generation.
- 🍽️ **Structured Menu System**
  - Restaurants → Categories → Items.
- 🛣️ **Optimized Multi-Delivery Route**
  - Greedy nearest-location approach for efficiency.
- ✅ **Input Validation**
  - Prevents invalid inputs and runtime errors.

---

## 🧠 Algorithms & Data Structures Used

- **Dijkstra’s Shortest Path Algorithm**
- **Greedy Algorithm** (Nearest delivery point)
- **Adjacency Matrix** for road network representation
- Arrays and structured data storage for:
  - Locations
  - Restaurants
  - Menus
  - Orders

---

## 🚦 Traffic Factor (Key Highlight)

The **Traffic Factor** is a multiplier applied to road distances to simulate real-time traffic conditions.

**Example:**
- Base travel time: `5 minutes`
- Traffic factor: `1.2`
- Adjusted time: `6 minutes`

This ensures more realistic ETA calculations and delivery charges.

---

## 🧩 Project Structure

- `Item` – Stores food item name and price  
- `Category` – Holds food items under a category  
- `Restaurant` – Contains categories and menu  
- `Location` – Represents delivery areas (graph nodes)  
- `Order` – Stores order details  
- `FoodDelivery` – Core logic:
  - Graph handling
  - Traffic adjustment
  - Order processing
  - Billing
- `main()` – Menu-driven execution

---

## 📊 Sample Outputs

- Restaurant Listing
- Menu Display
- Bill Generation
- Delivery Route with ETA
- Traffic-adjusted delivery times
- Multi-customer optimized routes

---

## 🔮 Future Enhancements

- ⏰ **Rush Hour Traffic**
  - Automatic 5% delay during peak hours (e.g., 8–10 AM / 6–8 PM).
- 📦 **High Order Volume Delay**
  - Additional delay if order count exceeds a threshold.
  
> These enhancements were not implemented due to time constraints.

---

## 🛠️ Technologies Used

- **Language:** C++
- **Concepts:** Data Structures & Algorithms
- **Paradigm:** Procedural + Modular Design
- **Compiler:** Any standard C++ compiler

---

## 📂 How to Run

```bash
g++ FoodDeliveryRoutePlanner.cpp -o planner
./planner

