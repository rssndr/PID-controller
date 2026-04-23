#!/bin/bash

echo "=== Drone Simulation + Live Plot ==="

# Clean previous log file so plot starts empty
rm -f flight_log.csv

source venv/bin/activate

# Start the live plot in background
python3 live_plot.py &
PLOT_PID=$!

# Give the plot window time to open
echo "Waiting for plot window to open..."
sleep 5

# Start the simulation
make run

echo ""
echo "=== Simulation finished ==="
echo "Graph is still open. Press any key to close the plot window..."
read -n 1 -s

# Close the plot
kill $PLOT_PID 2>/dev/null
echo "Plot window closed."

