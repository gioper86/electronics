from skyfield.api import load
from datetime import datetime, timedelta
import numpy as np
import matplotlib.pyplot as plt


# Load planetary ephemeris
planets = load('de421.bsp')
ts = load.timescale()
t = ts.now()

sun = planets['sun']
mercury = planets['mercury']
venus = planets['venus']
earth = planets['earth']
mars = planets['mars']
jupiter = planets['jupiter barycenter']
saturn = planets['saturn barycenter']
uranus = planets['uranus barycenter']
neptune = planets['neptune barycenter']

planet_names = ['Sun','Mercury', 'Venus', 'Earth', 'Mars']
planet_objects = [sun, mercury, venus, earth, mars]
colors = ['yellow', 'gray', 'orange', 'blue', 'red']

def get_planet_orbit_xy(planet, days=100):
    t0 = ts.utc(t.utc_datetime() - timedelta(days=days // 2))
    t1 = ts.utc(t.utc_datetime() + timedelta(days=days // 2))
    times = ts.linspace(t0, t1, days)
    positions = (planet - sun).at(times).ecliptic_position().au
    x, y = positions[0], positions[1]
    return x, y

def get_planets_orbits():
    orbits_data = []
    for name, planet, color in zip(planet_names, planet_objects, colors):
        x_orbit, y_orbit = get_planet_orbit_xy(planet)
        # print(f"{name} orbit path:")
        # print(x_orbit, y_orbit)
        orbits_data.append((name, x_orbit, y_orbit))
    return orbits_data

def get_positions():
    planets_positions = []
    for name, planet, color in zip(planet_names, planet_objects, colors):
        # Orbit path
        # x_orbit, y_orbit = get_orbit_xy(planet)
        # print(f"{name} orbit path:")
        # print(x_orbit, y_orbit)

        # Current position
        pos = (planet - sun).at(t).ecliptic_position().au
        x, y = pos[0], pos[1]
        planets_positions.append((name, x, y))
    return planets_positions

def convert_to_display_coordinates(positions, width=128, height=64):
    # Define the display boundaries
    x_min, x_max = -1.5, 1.5  # AU range for x-axis
    y_min, y_max = -1.5, 1.5  # AU range for y-axis

    # Scale factors to map AU to display pixels
    x_scale = (width - 1) / (x_max - x_min)
    y_scale = (height - 1) / (y_max - y_min)

    display_positions = []

    for name, x, y in positions:
        # Convert AU to display coordinates
        x_disp = int((x - x_min) * x_scale)
        y_disp = int((y_max - y) * y_scale)  # Invert y-axis for display

        # Ensure coordinates are within display bounds
        x_disp = max(0, min(width - 1, x_disp))
        y_disp = max(0, min(height - 1, y_disp))

        display_positions.append((name, x_disp, y_disp))

    return display_positions

def plot_orbits_and_positions():
    """
    Plot only the current positions of the planets using get_positions().
    """
    # Initialize the plot
    fig, ax = plt.subplots(figsize=(8, 8))

    # Get planet data
    planet_data = get_positions()

    # Plot current positions
    for name, x, y in planet_data:
        ax.plot(x, y, 'o', label=name)
        ax.text(x + 0.02, y + 0.02, name)

    # Final formatting
    ax.legend()
    ax.set_xlim(-1.5, 1.5)
    ax.set_ylim(-1.5, 1.5)
    ax.set_aspect('equal', adjustable='box')  # Ensure equal scaling
    ax.set_title('Planetary Positions')  # Add a title
    ax.set_xlabel('X Position (AU)')  # Add X-axis label
    ax.set_ylabel('Y Position (AU)')  # Add Y-axis label
    plt.show()


