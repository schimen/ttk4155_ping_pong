mport matplotlib.pyplot as plt
import matplotlib.animation as animation
from time import time
from serial import Serial

fig, ax = plt.subplots()
ax.set_ylim([0, 255])

plot_length = 200

ser = Serial('/dev/ttyS4')

ax_dict = dict()

def animate(i):
    line = ser.readline().decode()
    ser.reset_input_buffer()
    print(line, end='')
    if not any(letter.isdigit() for letter in line):
        return None
    
    if '=' in line:
        name, y_str = line.split('=')
        y = int(y_str)
    else:
        name = 'Unknown'
        y = int(line)
    
    if name not in ax_dict:
        ax_dict[name] = ([], [])

    xs, ys = ax_dict[name]
    ys.append(y)
    xs.append(time())

    while len(xs) > plot_length:
        del xs[0]
        del ys[0]

    ax.clear()
    for ax_name in ax_dict:
            ax.plot(*ax_dict[ax_name], label=ax_name)
    ax.legend()

ani = animation.FuncAnimation(fig, animate, interval=0)
plt.show()
