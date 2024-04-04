import os
import glob
import tinify

tinify.key = ""

for fn in list(glob.glob('*.pdf')):
    print(f"Converting {fn} to PNG")
    orig_png = fn.replace('.pdf', '.orig.png')
    png = fn.replace('.pdf', '.png')
    os.system(f'convert -density 600 -alpha remove -alpha off "{fn}" "{orig_png}"')
    #-fuzz 4% -fill none -draw "color 0,0 floodfill"
    #-trim

    print(f"Optimizing {orig_png}")
    source = tinify.from_file(orig_png)
    source.to_file(png)

    # Cleanup
    #os.remove(fn)
    os.remove(orig_png)

