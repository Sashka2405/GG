from PIL import Image
import os

def extract_number(filename):
    digits = ''.join(filter(str.isdigit, os.path.splitext(filename)[0]))
    return int(digits)

image_paths = [
    f for f in os.listdir('.') 
    if f.lower().endswith('.png') and any(ch.isdigit() for ch in f)
]

image_paths = sorted(image_paths, key=extract_number)

images = [Image.open(f).convert('RGBA') for f in image_paths]

print(*[f'{img.width, img.height}' for img in images], sep = '\n')

total_width = sum(img.width for img in images)
max_height = max(img.height for img in images)
new_image = Image.new('RGBA', (total_width, max_height), (0, 0, 0, 0))
x_offset = 0
for img in images:
    new_image.paste(img, (x_offset, 0), img)
    x_offset += img.width
new_image.save('combined.png')
