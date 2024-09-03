#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <stdexcept>
#include <algorithm>
#include <random>
#include <filesystem>

using namespace std;
using namespace std::chrono;

// Structure for Apartment data
struct Apartment {
    string ads_id;
    string prop_name;
    string completion_year;
    string monthly_rent;
    string location;
    string property_type;
    string rooms;
    string parking;
    string bathroom;
    string size;
    string furnished;
    string facilities;
    string additional_facilities;
    string region;
};

// Custom container class for Apartment data
class ApartmentList {
public:
    ApartmentList() : size(0), capacity(10) {
        apartments = new Apartment[capacity];
    }

    ~ApartmentList() {
        delete[] apartments;
    }

    void push_back(const Apartment& apt) {
        if (size == capacity) {
            resize();
        }
        apartments[size++] = apt;
    }

    Apartment& operator[](int index) {
        if (index < 0 || index >= size) {
            throw out_of_range("Index out of range");
        }
        return apartments[index];
    }

    const Apartment& operator[](int index) const {
        if (index < 0 || index >= size) {
            throw out_of_range("Index out of range");
        }
        return apartments[index];
    }

    int getSize() const {
        return size;
    }

    void writeToCSV(const string& filename) const {
        ofstream file(filename);

        if (!file.is_open()) {
            cerr << "Error opening file!" << endl;
            return;
        }

        // Write header
        file << "ads_id,prop_name,completion_year,monthly_rent,location,property_type,rooms,parking,bathroom,size,furnished,facilities,additional_facilities,region\n";

        // Write data
        for (int i = 0; i < size; ++i) {
            file << apartments[i].ads_id << ','
                 << apartments[i].prop_name << ','
                 << apartments[i].completion_year << ','
                 << apartments[i].monthly_rent << ','
                 << apartments[i].location << ','
                 << apartments[i].property_type << ','
                 << apartments[i].rooms << ','
                 << apartments[i].parking << ','
                 << apartments[i].bathroom << ','
                 << apartments[i].size << ','
                 << apartments[i].furnished << ','
                 << apartments[i].facilities << ','
                 << apartments[i].additional_facilities << ','
                 << apartments[i].region << '\n';
        }

        file.close();
    }

private:
    Apartment* apartments;
    int size;
    int capacity;

    void resize() {
        capacity *= 2;
        Apartment* new_apartments = new Apartment[capacity];
        for (int i = 0; i < size; ++i) {
            new_apartments[i] = apartments[i];
        }
        delete[] apartments;
        apartments = new_apartments;
    }
};

// Function prototypes
ApartmentList readCSV(const string& filename);
void quicksort(ApartmentList& data, int low, int high);
void insertionSort(ApartmentList& data, int low, int high);
int partition(ApartmentList& data, int low, int high);
void mergesort(ApartmentList& data, int left, int right);
void merge(ApartmentList& data, int left, int mid, int right);

int rentToInt(const string& rent) {
    string cleaned = "";
    for (char c : rent) {
        if (isdigit(c)) {
            cleaned += c;
        }
    }
    try {
        return stoi(cleaned);
    } catch (invalid_argument&) {
        return 0;
    }
}

int roomsToInt(const string& rooms) {
    try {
        return stoi(rooms);
    } catch (invalid_argument&) {
        return 0;
    }
}

// Function to read data from CSV file into ApartmentList
ApartmentList readCSV(const string& filename) {
    ApartmentList data;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return data;
    }

    string line;
    // Skip the header
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        Apartment row;

        getline(ss, row.ads_id, ',');
        getline(ss, row.prop_name, ',');
        getline(ss, row.completion_year, ',');
        getline(ss, row.monthly_rent, ',');
        getline(ss, row.location, ',');
        getline(ss, row.property_type, ',');
        getline(ss, row.rooms, ',');
        getline(ss, row.parking, ',');
        getline(ss, row.bathroom, ',');
        getline(ss, row.size, ',');
        getline(ss, row.furnished, ',');
        getline(ss, row.facilities, ',');
        getline(ss, row.additional_facilities, ',');
        getline(ss, row.region, ',');

        data.push_back(row);
    }

    file.close();
    return data;
}

// Function to select the pivot using the median-of-three method
int medianOfThree(ApartmentList& data, int low, int high) {
    int mid = low + (high - low) / 2;
    int rentLow = rentToInt(data[low].monthly_rent);
    int rentMid = rentToInt(data[mid].monthly_rent);
    int rentHigh = rentToInt(data[high].monthly_rent);

    if ((rentLow < rentMid && rentMid < rentHigh) || (rentHigh < rentMid && rentMid < rentLow)) {
        return mid;
    } else if ((rentMid < rentLow && rentLow < rentHigh) || (rentHigh < rentLow && rentLow < rentMid)) {
        return low;
    } else {
        return high;
    }
}

// Insertion sort for small subarrays
void insertionSort(ApartmentList& data, int low, int high) {
    for (int i = low + 1; i <= high; ++i) {
        Apartment key = data[i];
        int j = i - 1;
        while (j >= low && (rentToInt(data[j].monthly_rent) > rentToInt(key.monthly_rent) ||
                            (rentToInt(data[j].monthly_rent) == rentToInt(key.monthly_rent) && roomsToInt(data[j].rooms) > roomsToInt(key.rooms)))) {
            data[j + 1] = data[j];
            j--;
        }
        data[j + 1] = key;
    }
}

// Quicksort algorithm to sort Apartments based on monthly_rent and rooms
void quicksort(ApartmentList& data, int low, int high) {
    while (low < high) {
        if (high - low <= 10) { // Use insertion sort for small subarrays
            insertionSort(data, low, high);
            break;
        } else {
            int pivotIndex = medianOfThree(data, low, high);
            swap(data[pivotIndex], data[high]); // Move pivot to end
            int pivot = partition(data, low, high);
            if (pivot - low < high - pivot) {
                quicksort(data, low, pivot - 1);
                low = pivot + 1;
            } else {
                quicksort(data, pivot + 1, high);
                high = pivot - 1;
            }
        }
    }
}

// Helper function for partitioning in quicksort
int partition(ApartmentList& data, int low, int high) {
    int pivotRent = rentToInt(data[high].monthly_rent);
    int pivotRooms = roomsToInt(data[high].rooms);
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (rentToInt(data[j].monthly_rent) < pivotRent ||
            (rentToInt(data[j].monthly_rent) == pivotRent && roomsToInt(data[j].rooms) < pivotRooms)) {
            i++;
            swap(data[i], data[j]);
        }
    }
    swap(data[i + 1], data[high]);
    return (i + 1);
}

// Mergesort algorithm to sort Apartments based on monthly_rent and rooms
void mergesort(ApartmentList& data, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergesort(data, left, mid);
        mergesort(data, mid + 1, right);
        merge(data, left, mid, right);
    }
}

// Helper function to merge two sorted subarrays
void merge(ApartmentList& data, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Apartment* leftArray = new Apartment[n1];
    Apartment* rightArray = new Apartment[n2];

    for (int i = 0; i < n1; i++)
        leftArray[i] = data[left + i];
    for (int j = 0; j < n2; j++)
        rightArray[j] = data[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (rentToInt(leftArray[i].monthly_rent) < rentToInt(rightArray[j].monthly_rent) ||
            (rentToInt(leftArray[i].monthly_rent) == rentToInt(rightArray[j].monthly_rent) &&
             roomsToInt(leftArray[i].rooms) < roomsToInt(rightArray[j].rooms))) {
            data[k] = leftArray[i];
            i++;
        } else {
            data[k] = rightArray[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        data[k] = leftArray[i];
        i++;
        k++;
    }

    while (j < n2) {
        data[k] = rightArray[j];
        j++;
        k++;
    }

    delete[] leftArray;
    delete[] rightArray;
}

// Function to split a string by delimiter
string* split(const string& str, char delimiter, int& token_count) {
    stringstream tokenStream(str);
    string token;
    token_count = count(str.begin(), str.end(), delimiter) + 1;
    string* tokens = new string[token_count];
    int i = 0;

    while (getline(tokenStream, token, delimiter)) {
        tokens[i++] = token;
    }

    return tokens;
}

// Function to read CSV into a dynamic array
Apartment* read_csv(const string& filename, int& rows) {
    ifstream file(filename);
    string line;
    int capacity = 10;
    rows = 0;
    Apartment* data = new Apartment[capacity];

    // Skip header
    getline(file, line);

    while (getline(file, line)) {
        if (rows == capacity) {
            capacity *= 2;
            Apartment* new_data = new Apartment[capacity];
            copy(data, data + rows, new_data);
            delete[] data;
            data = new_data;
        }
        int token_count;
        string* tokens = split(line, ',', token_count);
        if (token_count >= 14) {
            data[rows].ads_id = tokens[0];
            data[rows].prop_name = tokens[1];
            data[rows].completion_year = tokens[2];
            data[rows].monthly_rent = tokens[3];
            data[rows].location = tokens[4];
            data[rows].property_type = tokens[5];
            data[rows].rooms = tokens[6];
            data[rows].parking = tokens[7];
            data[rows].bathroom = tokens[8];
            data[rows].size = tokens[9];
            data[rows].furnished = tokens[10];
            data[rows].facilities = tokens[11];
            data[rows].additional_facilities = tokens[12];
            data[rows].region = tokens[13];
            rows++;
        }
        delete[] tokens;
    }

    file.close();
    return data;
}

// Function to write a dynamic array to a CSV file
void write_csv(const string& filename, const Apartment* data, int rows) {
    ofstream file(filename);
    
    // Write header
    file << "ads_id,prop_name,completion_year,monthly_rent,location,property_type,rooms,parking,bathroom,size,furnished,facilities,additional_facilities,region\n";

    // Write data
    for (int i = 0; i < rows; ++i) {
        file << data[i].ads_id << ','
             << data[i].prop_name << ','
             << data[i].completion_year << ','
             << data[i].monthly_rent << ','
             << data[i].location << ','
             << data[i].property_type << ','
             << data[i].rooms << ','
             << data[i].parking << ','
             << data[i].bathroom << ','
             << data[i].size << ','
             << data[i].furnished << ','
             << data[i].facilities << ','
             << data[i].additional_facilities << ','
             << data[i].region << '\n';
    }
    
    file.close();
}

// Function to clean the data
Apartment* clean_data(Apartment* data, int& rows) {
    int cleaned_rows = 0;
    int capacity = rows;
    Apartment* cleaned_data = new Apartment[capacity];

    for (int i = 0; i < rows; ++i) {
        bool has_missing = false;
        if (data[i].ads_id.empty() || data[i].prop_name.empty() || data[i].completion_year.empty() ||
            data[i].monthly_rent.empty() || data[i].location.empty() || data[i].property_type.empty() ||
            data[i].rooms.empty() || data[i].parking.empty() || data[i].bathroom.empty() ||
            data[i].size.empty() || data[i].furnished.empty() || data[i].facilities.empty() ||
            data[i].additional_facilities.empty() || data[i].region.empty()) {
            has_missing = true;
        }

        if (!has_missing) {
            cleaned_data[cleaned_rows++] = data[i];
        }
    }

    rows = cleaned_rows;
    delete[] data;
    return cleaned_data;
}

int main() {
    string filename = "mudah-apartment-kl-selangor.csv";  // CSV file name
    filesystem::path csv_file_path;

    // Search for the CSV file in the current directory
    for (const auto& entry : filesystem::directory_iterator(".")) {
        if (entry.path().filename() == filename) {
            csv_file_path = entry.path();
            break;
        }
    }

    if (csv_file_path.empty()) {
        cerr << "CSV file " << filename << " not found in the current directory." << endl;
        return 1;  // Exit the program with an error code
    }

    // Read CSV
    int rows;
    Apartment* data = read_csv(csv_file_path.string(), rows);

    // Clean data
    data = clean_data(data, rows);

    // Overwrite the original CSV file with cleaned data
    write_csv(csv_file_path.string(), data, rows);

    cout << "Data cleaning is complete and the file " << filename << " is overwritten." << endl;

    // Read cleaned data into ApartmentList
    ApartmentList apartmentList;
    for (int i = 0; i < rows; ++i) {
        apartmentList.push_back(data[i]);
    }

    delete[] data;

    if (apartmentList.getSize() == 0) {
        cout << "No data read from the CSV file. Check if the file exists and is not empty." << endl;
        return 1;
    }

    // Perform Quicksort
    ApartmentList quicksortData = apartmentList;
    auto start = high_resolution_clock::now();
    quicksort(quicksortData, 0, quicksortData.getSize() - 1);
    auto stop = high_resolution_clock::now();
    auto quicksortDuration = duration_cast<milliseconds>(stop - start);
    quicksortData.writeToCSV("quicksort_sorted.csv");

    // Perform Mergesort
    ApartmentList mergesortData = apartmentList;
    start = high_resolution_clock::now();
    mergesort(mergesortData, 0, mergesortData.getSize() - 1);
    stop = high_resolution_clock::now();
    auto mergesortDuration = duration_cast<milliseconds>(stop - start);
    mergesortData.writeToCSV("mergesort_sorted.csv");

    // Output the execution times
    cout << "Execution time for Quicksort: " << quicksortDuration.count() << " milliseconds." << endl;
    cout << "Execution time for Mergesort: " << mergesortDuration.count() << " milliseconds." << endl;

    return 0;
}
