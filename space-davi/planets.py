from skyfield.api import load
from datetime import datetime, timedelta
import board
import busio
import adafruit_ssd1306
import numpy as np
import time

# Initialize I2C
i2c = busio.I2C(board.SCL, board.SDA)

# Initialize display
oled = adafruit_ssd1306.SSD1306_I2C(128, 64, i2c)

# Clear the display
oled.fill(0)
oled.show()

planets = load('de421.bsp')
ts = load.timescale()
t = ts.now()

sun = planets['sun']
mercury = planets['mercury']
venus = planets['venus']
earth = planets['earth']
mars = planets['mars']

planet_names = ['Mercury', 'Venus', 'Earth', 'Mars']
planet_objects = [mercury, venus, earth, mars]


def get_planet_orbit(planet, include_timestamp=False, days=365):
    t0 = ts.utc(t.utc_datetime() - timedelta(days=days // 2))
    t1 = ts.utc(t.utc_datetime() + timedelta(days=days // 2))
    times = ts.linspace(t0, t1, days)
    times = times[::3]
    positions = (planet - sun).at(times).ecliptic_position().au
    return np.transpose(positions)

def get_all_current_positions():
    """
    Returns a dictionary mapping planet names to their current ecliptic positions (AU).
    """
    positions = {}
    now = ts.now()
    for planet, name in zip(planet_objects, planet_names):
        pos = (planet - sun).at(now).ecliptic_position().au
        positions[name] = pos
    return positions

def convert_orbits_to_display(orbits, width=128, height=64, margin=2):
    """
    Converts 3D orbit points to 2D coordinates for SSD1306 display,
    scaling all orbits together based on the global extent.

    Parameters:
    - orbits: dict of {planet_name: np.ndarray of shape (365, 3)}
    - width: width of the display (default: 128)
    - height: height of the display (default: 64)
    - margin: margin around the display area (default: 2)

    Returns:
    - dict of {planet_name: np.ndarray of shape (365, 2)} with (x, y) pixel coordinates
    """

    # Combine all x and y data to find global extents
    all_x = []
    all_y = []
    for orbit in orbits.values():
        all_x.extend(orbit[:, 0])
        all_y.extend(orbit[:, 1])

    all_x = np.array(all_x)
    all_y = np.array(all_y)

    # Center around global mean
    x_center = np.mean(all_x)
    y_center = np.mean(all_y)

    x_centered = all_x - x_center
    y_centered = all_y - y_center

    # Determine global max extent
    max_extent = max(np.max(np.abs(x_centered)), np.max(np.abs(y_centered)))

    # Calculate the global scaling factor
    scale = min((width - 2 * margin) / (2 * max_extent),
                 (height - 2 * margin) / (2 * max_extent))

    display_orbits = {}
    for planet, orbit in orbits.items():
        # Center around global mean
        x_centered = orbit[:, 0] - x_center
        y_centered = orbit[:, 1] - y_center

        # Apply global scale and translate to center
        x_display = (x_centered * scale + width / 2).astype(int)
        y_display = (y_centered * scale + height / 2).astype(int)

        # Clip to display bounds
        x_display = np.clip(x_display, 0, width - 1)
        y_display = np.clip(y_display, 0, height - 1)

        display_orbits[planet] = np.column_stack((x_display, y_display))

    return (display_orbits, scale, x_center, y_center)


def display_orbits(positions, range_x=0, range_y=1):
    # Clear the display
    #oled.fill(0)

    # Draw the orbits
    for x, y in positions:
        for dx in range(range_x, range_y):
            for dy in range(range_x, range_y):
                oled.pixel(x + dx, y + dy, 1)

    # for x, y in positions:
    #     oled.pixel(x, y, 1)   

    # Show the updated display

def plot_orbits():

    orbits = {}
    for planet, name in zip(planet_objects, planet_names):
        orbits[name] = get_planet_orbit(planet)
    
    scaled_obj = convert_orbits_to_display(orbits)
    coordinates = scaled_obj[0]
    scale = scaled_obj[1]
    x_center = scaled_obj[2]
    y_center = scaled_obj[3]
    
    # Clear the display
    oled.fill(0)
    oled.show()

    for planet_name in planet_names:
        coordinates_p = coordinates[planet_name]
        display_orbits(coordinates_p)
    
    current_positions = get_all_current_positions()
    current_positions["Sun"] = [0,0,0]
    display_pos = []
    width = 128
    height = 64
    for planet, pos in current_positions.items():
        x_centered = pos[0] - x_center
        y_centered = pos[1] - y_center

        # Apply global scale and translate to center
        x_display = (x_centered * scale + width / 2).astype(int)
        y_display = (y_centered * scale + height / 2).astype(int)

        display_pos.append([x_display, y_display])

    display_orbits(display_pos,-1,3)
    oled.show()

plot_orbits()