from skyfield.api import load
from datetime import datetime, timedelta
import board
import busio
import adafruit_ssd1306
import numpy as np
import time
import display

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

def get_all_current_positions(day):
    """
    Returns a dictionary mapping planet names to their current ecliptic positions (AU).
    """
    positions = {}
    #now = ts.now()
    for planet, name in zip(planet_objects, planet_names):
        pos = (planet - sun).at(day).ecliptic_position().au
        positions[name] = pos
    return positions

def plot_on_day(day, orbits_coordinates, x_center, y_center, scale):
    display.oled.fill(0)

    formatted_time = day.utc_strftime('%d/%m/%y')
    display.draw_string(formatted_time)

    #Render orbits
    for planet_name in planet_names:
        coordinates_p = orbits_coordinates[planet_name]
        display.draw_points(coordinates_p)
    
    #Render planets on "day"
    current_positions = get_all_current_positions(day)
    current_positions["Sun"] = [0,0,0]

    points = display.convert_planets_positions_to_display(current_positions, x_center, y_center, scale)
    display.draw_points(points,-1,3)
    display.oled.show()   

def plot_orbits():

    orbits = {}
    for planet, name in zip(planet_objects, planet_names):
        orbits[name] = get_planet_orbit(planet)
    
    scaled_obj = display.convert_orbits_to_display(orbits)
    orbits_coordinates = scaled_obj[0]
    scale = scaled_obj[1]
    x_center = scaled_obj[2]
    y_center = scaled_obj[3]
    
    num_days = 10
    t0 = ts.utc(t.utc_datetime() - timedelta(days=num_days // 2))
    t1 = ts.utc(t.utc_datetime() + timedelta(days=num_days // 2))
    days = ts.linspace(t0, t1, num_days)
    print(days.shape)

    for day in days:
        plot_on_day(day, orbits_coordinates, x_center, y_center, scale)
    
    time.sleep(2)
    
    plot_on_day(ts.now(), orbits_coordinates, x_center, y_center, scale)

plot_orbits()