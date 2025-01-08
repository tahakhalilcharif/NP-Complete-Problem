import matplotlib.pyplot as plt
import csv

# File to read data from
input_file = "timing_results.txt"

# Data containers
dimensions = []
average_times = []

# Read data from the file
with open(input_file, "r") as file:
    reader = csv.reader(file)
    next(reader)  # Skip header
    for row in reader:
        dimension, avg_time = row[0], float(row[1])
        dimensions.append(dimension)
        average_times.append(avg_time)

# Generate the graph
plt.figure(figsize=(10, 6))
plt.plot(dimensions, average_times, marker="o", color="blue", label="Average Time")

# Adjust x-axis ticks to display every 10th dimension
plt.xticks(ticks=range(0, len(dimensions), 10), labels=dimensions[::10], rotation=45, fontsize=10)

# Graph labels and title
plt.title("Sliding Puzzle Solvability Timing", fontsize=16)
plt.xlabel("Puzzle Dimension", fontsize=12)
plt.ylabel("Average Time (ms)", fontsize=12)
plt.grid(True, linestyle="--", alpha=0.6)
plt.legend(fontsize=12)

# Save and show the graph
plt.tight_layout()
plt.savefig("timing_graph.png")  # Save as an image
plt.show()
