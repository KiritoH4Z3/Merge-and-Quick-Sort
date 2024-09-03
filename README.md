# Merge and Quick Sort

This code is designed to read apartment data from a CSV file, clean the data by removing any entries with missing information, and then sort the apartments using two different sorting algorithms: Quicksort and Mergesort. The goal is to compare the efficiency of these sorting algorithms on the given dataset.

## Sorting Algorithms Used:
- **Quicksort**: A divide-and-conquer algorithm that partitions the data into smaller segments to recursively sort them.
- **Mergesort**: Another efficient divide-and-conquer algorithm that divides the data into smaller subarrays, sorts them, and then merges them back together.

## Process:
1. **Reading Data**: The code reads apartment data from the provided CSV file, skips any entries with missing information, and cleans the dataset.
2. **Sorting**: It then sorts the cleaned data using Quicksort and Mergesort algorithms.
3. **Execution Times**: The code measures the execution times of both algorithms to compare their efficiency.

## Results:
After running the code, it was observed that Mergesort outperformed Quicksort in terms of speed for the given dataset. However, the actual performance may vary based on the computer's specifications and dataset size.

It should also be noted that 2 .csv files have been created, which is for both quicksort and mergesort

## Note:
This code is a demonstration of sorting algorithms applied to apartment data. To ensure good coding practices, future enhancements may involve dividing the code into header files for better organization and maintainability.

**Note:** The actual times may vary based on the computer's specifications and dataset size.