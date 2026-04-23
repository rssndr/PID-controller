import matplotlib.pyplot as plt
import pandas as pd
import os

plt.style.use('ggplot')

fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(12, 10), sharex=True)

line_z, = ax1.plot([], [], label='Altitude (z)', color='blue', lw=2)
line_vz, = ax2.plot([], [], label='Vertical Velocity', color='purple', lw=2)
line_thrust, = ax3.plot([], [], label='Thrust (N)', color='orange', lw=2)

ax1.axhline(y=1.0, color='green', linestyle='--', label='Setpoint (1.0 m)')
ax1.set_ylabel('Altitude (m)')
ax1.set_title('Live Altitude PID Control')
ax1.legend()
ax1.grid(True)

ax2.axhline(y=0, color='black', lw=0.5)
ax2.set_ylabel('Velocity (m/s)')
ax2.legend()
ax2.grid(True)

ax3.set_ylabel('Thrust (N)')
ax3.set_xlabel('Time (s)')
ax3.legend()
ax3.grid(True)

plt.tight_layout()

print("Live plot started - Close window to stop")

while True:
    if os.path.exists('flight_log.csv'):
        try:
            df = pd.read_csv('flight_log.csv')
            if len(df) > 5:
                # Update data
                line_z.set_data(df['time'], df['z'])
                line_vz.set_data(df['time'], df['vz'])
                line_thrust.set_data(df['time'], df['thrust'])

                for ax in (ax1, ax2, ax3):
                    ax.relim()
                    ax.autoscale_view()

                # Wind gust highlight
                for collection in ax1.collections:
                    collection.remove()

                gust_ranges = []
                gust_active = False
                start_time = None

                for i, row in df.iterrows():
                    if row['gust_active'] == 1 and not gust_active:
                        start_time = row['time']
                        gust_active = True
                    elif row['gust_active'] == 0 and gust_active:
                        gust_ranges.append((start_time, row['time']))
                        gust_active = False

                if gust_active:  # If gust is still active at the end of the data
                    gust_ranges.append((start_time, df['time'].iloc[-1]))

                for start, end in gust_ranges:
                    ax1.axvspan(start, end, color='red', alpha=0.35, label='Wind Gust')

                # Prevent duplicate labels
                handles, labels = ax1.get_legend_handles_labels()
                by_label = dict(zip(labels, handles))
                ax1.legend(by_label.values(), by_label.keys())

        except Exception as e:
            print(f"Error: {e}")

    plt.pause(0.05)

