#include <iostream>
#include <string>
using namespace std;

#define MAX_LOC 30
#define MAX_RESTAURANTS 15
#define INF 99999

class Item {
public:
    string name;
    float price;
};

class Category {
public:
    string name;
    Item items[10];
    int itemCount = 0;

    void addItem(const string& name, float price) {
        items[itemCount].name = name;
        items[itemCount].price = price;
        itemCount++;
    }

    void showItems() {
        for (int i = 0; i < itemCount; i++) {
            cout << "    " << i << " : " << items[i].name << " - Rs. " << items[i].price << endl;
        }
    }
};

class Restaurant {
public:
    string name;
    Category categories[10];
    int categoryCount = 0;

    void addCategory(const string& name) {
        categories[categoryCount].name = name;
        categoryCount++;
    }

    void showMenu() {
        cout << endl << "--- " << name << " Menu ---" << endl;
        for (int i = 0; i < categoryCount; i++) {
            cout << "  " << i << " : " << categories[i].name << endl;
            categories[i].showItems();
        }
    }
};

class Location {
public:
    string name;
};

class Order {
public:
    int restaurantID;
    int customerID;
    string itemName;
    int quantity;
    float price;
};

class FoodDelivery {
private:
    Location loc[MAX_LOC];
    int graph[MAX_LOC][MAX_LOC];
    int locCount = 0;

    int restaurantCount = 0;
    int restaurantIDs[MAX_RESTAURANTS];
    Restaurant restaurants[MAX_RESTAURANTS];

    float trafficFactor = 1.0;

    void setupMenus();
    int baseGraph[MAX_LOC][MAX_LOC];  // original distances (NO traffic)
public:
    FoodDelivery() {
        for (int i = 0; i < MAX_LOC; i++) {
            for (int j = 0; j < MAX_LOC; j++) {
                graph[i][j] = INF;
                baseGraph[i][j] = INF;
            }
        }

        setupLocations();
        setupRestaurants();
        setupMenus();
        setupRoads();
    }

    // Only F, G, E sectors
    void setupLocations() {
        string names[] = {
            "F-6","F-7","F-8","F-9","F-10","F-11",
            "G-6","G-7","G-8","G-9","G-10",
            "E-7","E-8","E-9","E-10"
        };
        int n = sizeof(names)/sizeof(names[0]);
        for (int i = 0; i < n; i++) {
            loc[locCount++].name = names[i];
        }
        loc[locCount++].name = "Aabpara"; // ID = 15
        loc[locCount++].name = "F-5";     // ID = 16
        loc[locCount++].name = "G-5";     // ID = 17
        loc[locCount++].name = "Banigala";     // ID = 18
    }

    void setupRestaurants() {
        // F sector
        addRestaurant("Cheezious",0);    // F-6
        addRestaurant("OPTP",1);         // F-7
        addRestaurant("Urban Cafe",2);   // F-8
        addRestaurant("Savour",2);       // F-8

        // G sector
        addRestaurant("Pizza Hut",6);    // G-6
        addRestaurant("KFC",7);          // G-7
        addRestaurant("Chop Chop Wok",10);// G-10

        // Aabpara & Banigala
        addRestaurant("Aabpara Bites",15);  // Aabpara
        addRestaurant("Banigala Grill",18); // Banigala

        // E sector (optional, one example)
        addRestaurant("The Dome",11);       // E-7
    }

    void addRestaurant(string name,int locID) {
        restaurants[restaurantCount].name = name;
        restaurantIDs[restaurantCount] = locID;
        restaurantCount++;
    }

    void setupRoads() {
        // F sector horizontal
        addRoad(0,1,5); // F-6 <-> F-7
        addRoad(1,2,5); // F-7 <-> F-8
        addRoad(2,3,5); // F-8 <-> F-9
        addRoad(3,4,5); // F-9 <-> F-10
        addRoad(4,5,5); // F-10 <-> F-11

        // G sector horizontal
        addRoad(6,7,5); // G-6 <-> G-7
        addRoad(7,8,5); // G-7 <-> G-8
        addRoad(8,9,5); // G-8 <-> G-9
        addRoad(9,10,5);// G-9 <-> G-10

        // E sector horizontal
        addRoad(11,12,5); // E-7 <-> E-8
        addRoad(12,13,5); // E-8 <-> E-9
        addRoad(13,14,5); // E-9 <-> E-10

        // Vertical connections
        addRoad(0,6,6);  // F-6 <-> G-6
        addRoad(6, 15, 3);  // G-6 <-> Aabpara, assume 3 minutes distance

        addRoad(1,7,6);  // F-7 <-> G-7
        addRoad(2,8,6);  // F-8 <-> G-8
        addRoad(3,9,6);  // F-9 <-> G-9
        addRoad(1,11,6); // F-7 <-> E-7
        addRoad(2,12,6); // F-8 <-> E-8
        addRoad(3,9,6);   // F-9  <-> G-9
        addRoad(4,10,6);  // F-10 <-> G-10

        addRoad(3,13,6);  // F-9  <-> E-9
        addRoad(4,14,6);  // F-10 <-> E-10

        // F sector extension
        addRoad(16,0,5); // F-5 <-> F-6

        // G sector extension
        addRoad(17,6,5); // G-5 <-> G-6

        addRoad(16,17,5); // F-5 <-> G-5

        addRoad(17,18,8); // G-5 <-> Banigala

    }

    void addRoad(int u,int v,int w){
        baseGraph[u][v] = w;
        baseGraph[v][u] = w;

        graph[u][v] = int(w*trafficFactor);
        graph[v][u] = int(w*trafficFactor);
    }

    void applyTraffic() {
        cout << "Enter traffic multiplier (e.g., 1.2 = 20% more travel time): ";
        cin >> trafficFactor;

        while (trafficFactor < 0.5 || trafficFactor > 3.0) {
            cout << "Invalid multiplier. Enter a value between 0.5 and 3.0: ";
            cin >> trafficFactor;
        }

        for (int i = 0; i < locCount; i++) {
            for (int j = 0; j < locCount; j++) {
                if (baseGraph[i][j] != INF) {
                    graph[i][j] = int(baseGraph[i][j] * trafficFactor);
                }
            }
        }

        cout << "Traffic updated successfully!" << endl;
    }

    void showRestaurants() {
        cout << endl << "--- Restaurants ---" << endl;
        for (int i = 0; i < restaurantCount; i++) {
            cout << "  " << i << " : " << restaurants[i].name << "    (" << loc[restaurantIDs[i]].name << ")" << endl;
        }
    }

    void showCustomerAreas() {
        cout << endl << "--- Customer Areas ---" << endl;
        for (int i = 0; i < locCount; i++) {
            cout << "  " << i << " : " << loc[i].name << endl;
        }
    }

    void showMenu(int restaurantID){
        if(restaurantID<0 || restaurantID>=restaurantCount) {
            return;
        }
        restaurants[restaurantID].showMenu();
    }

    int minDistance(int dist[], bool visited[]){
        int min = INF;
        int index = -1;
        for (int i = 0; i < locCount; i++) {
            if (!visited[i] && dist[i] < min) {
                min = dist[i];
                index = i;
            }
        }
        return index;
    }

    void dijkstra(int src,int dist[],int parent[]){
        bool visited[MAX_LOC];
        for (int i = 0; i < locCount; i++) {
            dist[i] = INF;
            visited[i] = false;
            parent[i] = -1;
        }
        dist[src] = 0;
        for (int count = 0; count < locCount - 1; count++) {
            int u = minDistance(dist, visited);
            if (u == -1) {
                break;
            }
            visited[u] = true;
            for (int v = 0; v < locCount; v++) {
                if (!visited[v] && graph[u][v] != INF && dist[u] + graph[u][v] < dist[v]) {
                    dist[v] = dist[u] + graph[u][v];
                    parent[v] = u;
                }
            }
        }
    }

    void printPath(int parent[], int j) {
        if (parent[j] == -1) {
            cout << loc[j].name;
            return;
        }
        printPath(parent, parent[j]);
        cout << " -> " << loc[j].name;
    }

    void shortestPathBetweenLocations() {
        int src, dest;

        cout << endl << "--- Shortest Path ---" << endl;
        showCustomerAreas();

        cout << "Enter source location ID: ";
        cin >> src;
        while (src < 0 || src >= locCount) {
            cout << "Invalid source ID. Please enter a valid ID (0 to "
                 << locCount - 1 << "): ";
            cin >> src;
        }

        cout << "Enter destination location ID: ";
        cin >> dest;
        while (dest < 0 || dest >= locCount) {
            cout << "Invalid destination ID. Please enter a valid ID (0 to "
                 << locCount - 1 << "): ";
            cin >> dest;
        }

        int distArr[MAX_LOC], parent[MAX_LOC];
        dijkstra(src, distArr, parent);

        if (distArr[dest] == INF) {
            cout << "No path exists" << endl;
            return;
        }

        cout << "Shortest path: ";
        printPath(parent, dest);

        int time = distArr[dest];
        if (src == dest) {
            time = 3;
        }

        cout << endl << "Time: " << time << " minutes" << endl;
    }

    float calculateBill(float amount, float discountPercent, float deliveryCharge) {
        return amount * (1 - discountPercent / 100) + deliveryCharge;
    }

    void orderFoodSingle() {
        int restID, custID;

        cout << endl << " -----------------------" << endl;
        cout << "--- Restaurant Selection ---" << endl;
        showRestaurants();

        cout << "Choose Restaurant ID: ";
        cin >> restID;
        while (restID < 0 || restID >= restaurantCount) {
            cout << "Invalid restaurant ID. Please choose a valid ID: ";
            cin >> restID;
        }

        cout << endl << " -----------------------" << endl;
        cout << "--- Customer Area Selection ---" << endl;
        showCustomerAreas();

        cout << "Choose Customer Area ID: ";
        cin >> custID;
        while (custID < 0 || custID >= locCount) {
            cout << "Invalid area ID. Please choose a valid ID: ";
            cin >> custID;
        }

        int distArr[MAX_LOC], parent[MAX_LOC];
        dijkstra(restaurantIDs[restID], distArr, parent);

        if (distArr[custID] > 60) {
            cout << "Delivery location is too far. Order cannot be placed." << endl;
            return;
        }

        cout << endl << " -----------------------" << endl;
        cout << "--- Menu ---" << endl;
        showMenu(restID);

        int totalItems = 0;
        for (int i = 0; i < restaurants[restID].categoryCount; i++) {
            totalItems += restaurants[restID].categories[i].itemCount;
        }

        int n;
        cout << "How many items to order (max " << min(totalItems, 10) << "): ";
        cin >> n;
        while (n < 1 || n > totalItems || n > 10) {
            cout << "Invalid number of items. Please enter 1 to "
                 << min(totalItems, 10) << ": ";
            cin >> n;
        }

        Order orders[10];
        float totalAmount = 0;

        for (int i = 0; i < n; i++) {
            int cat, item, qty;

            cout << endl << "Item " << i + 1 << endl;

            cout << "Category ID: ";
            cin >> cat;
            while (cat < 0 || cat >= restaurants[restID].categoryCount) {
                cout << "Invalid category. Choose a number between 0 and "
                     << restaurants[restID].categoryCount - 1 << ": ";
                cin >> cat;
            }

            cout << "Item ID: ";
            cin >> item;
            while (item < 0 ||
                   item >= restaurants[restID].categories[cat].itemCount) {
                cout << "Invalid item. Choose 0 to "
                     << restaurants[restID].categories[cat].itemCount - 1
                     << ": ";
                cin >> item;
            }

            cout << "Quantity (max 20): ";
            cin >> qty;
            while (qty < 1 || qty > 20) {
                cout << "Invalid quantity. Enter 1 to 20: ";
                cin >> qty;
            }

            bool alreadyAdded = false;
            for (int j = 0; j < i; j++) {
                if (orders[j].itemName ==
                    restaurants[restID].categories[cat].items[item].name) {

                    orders[j].quantity += qty;
                    orders[j].price +=
                        restaurants[restID].categories[cat]
                            .items[item].price * qty;

                    totalAmount +=
                        restaurants[restID].categories[cat]
                            .items[item].price * qty;

                    cout << "Item already added. Quantity updated." << endl;
                    alreadyAdded = true;
                    i--;
                    break;
                }
            }

            if (alreadyAdded) {
                continue;
            }

            orders[i].restaurantID = restID;
            orders[i].customerID = custID;
            orders[i].itemName =
                restaurants[restID].categories[cat].items[item].name;
            orders[i].quantity = qty;
            orders[i].price =
                restaurants[restID].categories[cat].items[item].price * qty;

            totalAmount += orders[i].price;
        }

        float discount = 0;
        string promo;

        cout << endl << "Promo Code (or NONE): ";
        cin >> promo;

        while (promo != "NONE" && promo != "HHA10" && promo != "FLAT200") {
            cout << "Invalid promo code. Please enter a valid coupon code or NONE: ";
            cin >> promo;
        }

        if (promo == "HHA10") {
            discount = 10;
        } else if (promo == "FLAT200") {
            discount = 200 * 100 / totalAmount;
        }

        int eta = distArr[custID];
        if (eta == 0) {
            eta = 3;
        }

        float deliveryCharge = eta * 5;
        float finalBill = calculateBill(totalAmount, discount, deliveryCharge);

        cout << endl << " -----------------------" << endl;
        cout << "--- BILL ---" << endl;

        for (int i = 0; i < n; i++) {
            cout << orders[i].itemName
                 << "   x " << orders[i].quantity
                 << "   Rs. " << orders[i].price << endl;
        }

        cout << endl;
        cout << "Subtotal        : Rs. " << totalAmount << endl;
        cout << "Discount        : " << discount << " %" << endl;
        cout << "Delivery Charge : Rs. " << deliveryCharge << endl;
        cout << "Total Payable   : Rs. " << finalBill << endl;

        cout << "Delivery Path   : ";
        printPath(parent, custID);
        cout << endl << "ETA             : " << eta << " minutes" << endl;
    }

       void orderFoodMulti() {
        int restID, numCustomers;

        showRestaurants();
        cout << "Choose Restaurant ID for multi-delivery: ";
        cin >> restID;
        while (restID < 0 || restID >= restaurantCount) {
            cout << "Invalid restaurant ID. Choose a valid ID: ";
            cin >> restID;
        }

        cout << "Enter number of customers to deliver to (max 5): ";
        cin >> numCustomers;
        while (numCustomers < 1 || numCustomers > 5) {
            cout << "Invalid number. Enter 1 to 5: ";
            cin >> numCustomers;
        }

        int customerIDs[10];
        showCustomerAreas();
        for (int i = 0; i < numCustomers; i++) {
            cout << "Enter Customer " << i + 1 << " ID: ";
            cin >> customerIDs[i];
            while (customerIDs[i] < 0 || customerIDs[i] >= locCount) {
                cout << "Invalid area ID. Choose a valid ID: ";
                cin >> customerIDs[i];
            }
        }

        int distArr[MAX_LOC], parent[MAX_LOC];
        dijkstra(restaurantIDs[restID], distArr, parent);

        for (int i = 0; i < numCustomers; i++) {
            if (distArr[customerIDs[i]] > 60) {
                cout << "Customer " << i + 1 << " ("
                     << loc[customerIDs[i]].name
                     << ") is too far for multi-delivery." << endl;
                return;
            }
        }

        Order allOrders[10][10];
        int itemsCount[10] = {0};
        float customerTotals[10] = {0};

        for (int i = 0; i < numCustomers; i++) {
            int custID = customerIDs[i];

            cout << endl << "--- Customer " << i + 1
                 << " (" << loc[custID].name << ") ---" << endl;

            showMenu(restID);

            int totalItems = 0;
            for (int c = 0; c < restaurants[restID].categoryCount; c++) {
                totalItems += restaurants[restID].categories[c].itemCount;
            }

            int n;
            cout << "How many items to order (max "
                 << min(totalItems, 10) << "): ";
            cin >> n;
            while (n < 1 || n > totalItems || n > 10) {
                cout << "Invalid number. Enter 1 to "
                     << min(totalItems, 10) << ": ";
                cin >> n;
            }

            itemsCount[i] = 0;

            for (int j = 0; j < n; j++) {
                int cat, item, qty;

                cout << "Category ID: ";
                cin >> cat;
                while (cat < 0 ||
                       cat >= restaurants[restID].categoryCount) {
                    cout << "Invalid category. Choose 0 to "
                         << restaurants[restID].categoryCount - 1
                         << ": ";
                    cin >> cat;
                }

                cout << "Item ID: ";
                cin >> item;
                while (item < 0 ||
                       item >=
                       restaurants[restID].categories[cat].itemCount) {
                    cout << "Invalid item. Choose 0 to "
                         << restaurants[restID]
                                .categories[cat]
                                .itemCount - 1
                         << ": ";
                    cin >> item;
                }

                cout << "Quantity (max 20): ";
                cin >> qty;
                while (qty < 1 || qty > 20) {
                    cout << "Invalid quantity. Enter 1 to 20: ";
                    cin >> qty;
                }

                string selectedItem =
                    restaurants[restID]
                        .categories[cat]
                        .items[item]
                        .name;

                float itemPrice =
                    restaurants[restID]
                        .categories[cat]
                        .items[item]
                        .price;

                bool found = false;

                for (int k = 0; k < itemsCount[i]; k++) {
                    if (allOrders[i][k].itemName == selectedItem) {
                        allOrders[i][k].quantity += qty;
                        allOrders[i][k].price += itemPrice * qty;
                        found = true;
                        cout << "Item already added. Quantity updated."
                             << endl;
                        break;
                    }
                }

                if (!found) {
                    allOrders[i][itemsCount[i]].restaurantID = restID;
                    allOrders[i][itemsCount[i]].customerID = custID;
                    allOrders[i][itemsCount[i]].itemName = selectedItem;
                    allOrders[i][itemsCount[i]].quantity = qty;
                    allOrders[i][itemsCount[i]].price =
                        itemPrice * qty;
                    itemsCount[i]++;
                }

                customerTotals[i] += itemPrice * qty;
            }
        }

        bool visited[10] = {0};
        int remaining = numCustomers;
        int currentLoc = restaurantIDs[restID];
        int totalTime = 0;

        cout << endl << "--- DELIVERY ROUTE ---" << endl;
        cout << loc[currentLoc].name;

        while (remaining > 0) {
            dijkstra(currentLoc, distArr, parent);

            int nearest = -1;
            int minDist = INF;

            for (int i = 0; i < numCustomers; i++) {
                if (!visited[i] &&
                    distArr[customerIDs[i]] < minDist) {
                    nearest = i;
                    minDist = distArr[customerIDs[i]];
                }
            }

            int nextCust = customerIDs[nearest];

            int path[30];
            int len = 0;
            int temp = nextCust;

            while (temp != -1) {
                path[len++] = temp;
                temp = parent[temp];
            }

            for (int i = len - 2; i >= 0; i--) {
                cout << " -> " << loc[path[i]].name;
            }

            int segmentETA = distArr[nextCust];
            if (segmentETA == 0) {
                segmentETA = 3;
            }

            totalTime += segmentETA;
            currentLoc = nextCust;
            visited[nearest] = true;
            remaining--;
        }

        cout << endl << "--- BILLS ---" << endl;

        for (int i = 0; i < numCustomers; i++) {
            cout << "Customer " << i + 1
                 << " (" << loc[customerIDs[i]].name << ")"
                 << endl;

            for (int j = 0; j < itemsCount[i]; j++) {
                cout << allOrders[i][j].itemName
                     << "   x " << allOrders[i][j].quantity
                     << "   Rs. " << allOrders[i][j].price
                     << endl;
            }

            float discount = 0;
            string promo;

            cout << "Promo Code (or NONE): ";
            cin >> promo;
            while (promo != "NONE" &&
                   promo != "HHA10" &&
                   promo != "FLAT200") {
                cout << "Invalid promo code. Please enter a valid coupon code or NONE: ";
                cin >> promo;
            }

            if (promo == "HHA10") {
                discount = 10;
            } else if (promo == "FLAT200") {
                discount = 200 * 100 / customerTotals[i];
            }

            dijkstra(restaurantIDs[restID], distArr, parent);
            int customerETA = distArr[customerIDs[i]];
            if (customerETA == 0) {
                customerETA = 3;
            }

            float deliveryCharge = customerETA * 5;
            float finalBill =
                calculateBill(customerTotals[i],
                              discount,
                              deliveryCharge);

            cout << "Subtotal        : Rs. "
                 << customerTotals[i] << endl;
            cout << "Discount        : "
                 << discount << " %" << endl;
            cout << "Delivery Charge : Rs. "
                 << deliveryCharge << endl;
            cout << "Total Payable   : Rs. "
                 << finalBill
                 << "   ETA : "
                 << customerETA
                 << " minutes"
                 << endl;
        }

        cout << "Total route time: "
             << totalTime
             << " minutes"
             << endl;
    }

};

void FoodDelivery::setupMenus() {

    restaurants[0].addCategory("Pizzas");
    restaurants[0].categories[0].addItem("Crown Crust Pizza", 1450);
    restaurants[0].categories[0].addItem("Stuffed Crust", 1500);
    restaurants[0].addCategory("Burgers & Sides");
    restaurants[0].categories[1].addItem("Bazinga Burger", 560);
    restaurants[0].categories[1].addItem("Loaded Fries", 450);

    restaurants[1].addCategory("Fries & Snacks");
    restaurants[1].categories[0].addItem("Crispy Fries", 400);
    restaurants[1].categories[0].addItem("Cheese Fries", 550);
    restaurants[1].addCategory("Burgers & Wraps");
    restaurants[1].categories[1].addItem("Smash Burger", 650);

    restaurants[2].addCategory("Coffee & Beverages");
    restaurants[2].categories[0].addItem("Latte", 400);
    restaurants[2].categories[0].addItem("Cappuccino", 450);
    restaurants[2].categories[0].addItem("Mocha", 500);
    restaurants[2].addCategory("Light Bites");
    restaurants[2].categories[1].addItem("Club Sandwich", 650);

    restaurants[3].addCategory("Burgers & Wraps");
    restaurants[3].categories[0].addItem("Beef Burger", 700);
    restaurants[3].categories[0].addItem("Chicken Burger", 650);
    restaurants[3].categories[0].addItem("Grilled Sandwich", 550);

    restaurants[4].addCategory("Pizzas");
    restaurants[4].categories[0].addItem("Chicken Tikka Pizza", 1800);
    restaurants[4].categories[0].addItem("Super Supreme", 2000);
    restaurants[4].addCategory("Starters & Sides");
    restaurants[4].categories[1].addItem("6 pcs Wings", 799);
    restaurants[4].categories[1].addItem("Garlic Bread", 199);

    restaurants[5].addCategory("Fried Chicken & Combos");
    restaurants[5].categories[0].addItem("Hot & Crispy Chicken", 450);
    restaurants[5].categories[0].addItem("Zinger Burger", 600);
    restaurants[5].categories[0].addItem("Mighty Zinger Combo", 1050);
    restaurants[5].addCategory("Sides & Drinks");
    restaurants[5].categories[1].addItem("Masala Fries", 360);
    restaurants[5].categories[1].addItem("Pepsi (345 ml)", 180);

    restaurants[6].addCategory("Asian");
    restaurants[6].categories[0].addItem("Chop Suey", 700);
    restaurants[6].categories[0].addItem("Chicken Fried Rice", 650);
    restaurants[6].categories[0].addItem("Kung Pao Chicken", 750);

    restaurants[7].addCategory("Fast Food");
    restaurants[7].categories[0].addItem("Beef Shawarma", 500);
    restaurants[7].categories[0].addItem("Chicken Shawarma", 450);
    restaurants[7].categories[0].addItem("French Fries", 250);

    restaurants[8].addCategory("Grill & BBQ");
    restaurants[8].categories[0].addItem("Grilled Chicken Platter", 1400);
    restaurants[8].categories[0].addItem("Beef Steak", 1700);
    restaurants[8].categories[0].addItem("Mixed Grill Platter", 1800);

    restaurants[9].addCategory("Buffet & Meals");
    restaurants[9].categories[0].addItem("All You Can Eat Buffet", 2200);
    restaurants[9].categories[0].addItem("Barbecue Platter", 1600);
    restaurants[9].categories[0].addItem("Seafood Special", 1900);
}

int main() {
    FoodDelivery fd;
    int choice;

    do {
        cout << endl << "--- RUSHER FOOD DELIVERY ROUTE PLANNER ---" << endl;
        cout << "1. Show Restaurants" << endl;
        cout << "2. Show Customer Areas" << endl;
        cout << "3. Single Order + Bill" << endl;
        cout << "4. Apply Traffic Factor" << endl;
        cout << "5. Shortest Path Between Locations" << endl;
        cout << "6. Multi-Customer Delivery" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                fd.showRestaurants();
                break;
            case 2:
                fd.showCustomerAreas();
                break;
            case 3:
                fd.orderFoodSingle();
                break;
            case 4:
                fd.applyTraffic();
                break;
            case 5:
                fd.shortestPathBetweenLocations();
                break;
            case 6:
                fd.orderFoodMulti();
                break;
            case 0:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice" << endl;
        }
    } while (choice != 0);

    return 0;
}


