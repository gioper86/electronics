import board
import busio
import adafruit_ssd1306

# Initialize I2C
i2c = busio.I2C(board.SCL, board.SDA)

# Initialize display
oled = adafruit_ssd1306.SSD1306_I2C(128, 64, i2c)

# Clear the display
oled.fill(0)
oled.show()

# Basic text display (no custom font, just a built-in 5x8 font)
# Draw horizontal line
for x in range(20, 100):
    oled.pixel(x, 10, 1)

oled.text("Hello World!", 0, 0, 1)

oled.show()

