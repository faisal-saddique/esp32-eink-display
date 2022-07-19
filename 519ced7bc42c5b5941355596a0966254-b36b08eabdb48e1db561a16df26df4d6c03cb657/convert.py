from PIL import Image, ImageEnhance

import numpy

DESATURATED_PALETTE = [
    [0, 0, 0],
    [255, 255, 255],
    [0, 255, 0],
    [0, 0, 255],
    [255, 0, 0],
    [255, 255, 0],
    [255, 140, 0],
    [255, 255, 255]
]

SATURATED_PALETTE = [
    [57, 48, 57],
    [255, 255, 255],
    [58, 91, 70],
    [61, 59, 94],
    [156, 72, 75],
    [208, 190, 71],
    [177, 106, 73],
    [255, 255, 255]
]

def _palette_blend(saturation, dtype='uint8'):
	saturation = float(saturation)
	palette = []
	for i in range(7):
		rs, gs, bs = [c * saturation for c in SATURATED_PALETTE[i]]
		rd, gd, bd = [c * (1.0 - saturation) for c in DESATURATED_PALETTE[i]]
		if dtype == 'uint8':
			palette += [int(rs + rd), int(gs + gd), int(bs + bd)]
		if dtype == 'uint24':
			palette += [(int(rs + rd) << 16) | (int(gs + gd) << 8) | int(bs + bd)]
	if dtype == 'uint8':
		palette += [255, 255, 255]
	if dtype == 'uint24':
		palette += [0xffffff]

	return palette


image = Image.open('./carlee.png')
#image = Image.open('./test_case.png')
#image = Image.open('./sm_test.png')
image = image.convert('RGB')

width, height = image.size
print( "width = ", width, " height = ", height) 

#d = image.getdata()
#print ("source image size:", image.size)

image.show() 

saturation = 0.5
palette = _palette_blend(saturation)
palette_image = Image.new("P", (1, 1))
palette_image.putpalette(palette + [0, 0, 0] * 248)

#palimage = Image.new('P', (image.size[0], image.size[1]))
#palimage.putpalette(impression_palette)

converted_image = image.im.convert("P", True, palette_image.im)

# the reshape method will allow us to access the buffer as buf[y][x]
buf = numpy.array(converted_image, dtype=numpy.uint8).reshape(( height, width))

with open("tmp_out.c", "w") as f:

	f.write('const unsigned char gImage_4in01f[%d] PROGMEM = {\n'%(width*height/2))

	for y in range(height):
		for x in range(int(width/2)):
			pix_left_val = int( buf[y][x*2] )
			pix_right_val = int( buf[y][x*2+1] )

			combined = pix_left_val << 4 | pix_right_val
			f.write (hex(combined))
			
			if x < int(width/2):
				f.write(',')

		f.write("\n")
	
	f.write('};\n')