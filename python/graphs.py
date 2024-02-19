import pandas as pd
import matplotlib.pyplot as plt

# Read the data from the .csv files
data = pd.read_csv('/data/your_file.csv')

# Extract the required columns
threads = data['Threads']
turnaround_time = data['Turnaround Time']
system_throughput = data['System Throughput']

# Create the graph
plt.plot(threads, turnaround_time, label='Average Turnaround Time')
plt.plot(threads, system_throughput, label='Average System Throughput')

# Add labels and title
plt.xlabel('Number of Threads')
plt.ylabel('Time / Throughput')
plt.title('Comparison of Number of Threads with Turnaround Time and System Throughput')

# Add legend
plt.legend()

# Show the graph
plt.show()