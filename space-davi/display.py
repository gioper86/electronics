import board
import busio
import adafruit_ssd1306
import numpy as np
from PIL import Image, ImageDraw, ImageFont

# Initialize I2C
i2c = busio.I2C(board.SCL, board.SDA)

# Initialize display
oled = adafruit_ssd1306.SSD1306_I2C(128, 64, i2c)

# Clear the display
oled.fill(0)
oled.show()
width=128
height=64

def draw_points(positions, range_x=0, range_y=1):
    for x, y in positions:
        for dx in range(range_x, range_y):
            for dy in range(range_x, range_y):
                oled.pixel(x + dx, y + dy, 1)


def convert_orbits_to_display(orbits, margin=2):
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

def convert_planets_positions_to_display(au_positions, x_center, y_center, scale):
    display_pos = []
    for planet, pos in au_positions.items():
        x_centered = pos[0] - x_center
        y_centered = pos[1] - y_center

        # Apply global scale and translate to center
        x_display = (x_centered * scale + width / 2).astype(int)
        y_display = (y_centered * scale + height / 2).astype(int)

        display_pos.append([x_display, y_display])
    return display_pos

def draw_string(s,x=0,y=0):
    image = Image.new("1", (oled.width, oled.height))
    draw = ImageDraw.Draw(image)
    font = ImageFont.load_default()
    draw.text((x, y), s, font=font, fill=255)

    oled.image(image)