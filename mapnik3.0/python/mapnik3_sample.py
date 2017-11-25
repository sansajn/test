# python2.7+, taken from https://github.com/mapnik/mapnik/wiki/GettingStartedInPython

import mapnik

# create a map
m = mapnik.Map(600,300)
m.background = mapnik.Color('steelblue')

# create a style
s = mapnik.Style()
r = mapnik.Rule()
polygon_symbolizer = mapnik.PolygonSymbolizer()
polygon_symbolizer.fill = mapnik.Color('#f2eff9')
r.symbols.append(polygon_symbolizer)

line_symbolizer = mapnik.LineSymbolizer()
line_symbolizer.stroke = mapnik.Color('rgb(50%,50%,50%)')
line_symbolizer.stroke_width = 0.1
r.symbols.append(line_symbolizer)
s.rules.append(r)

# add style to map
m.append_style('My Style', s)

# create datasource
ds = mapnik.Shapefile(file='../data/TM_WORLD_BORDERS-0.3.shp')

# create a layer
layer = mapnik.Layer('world')  # a new layer called world
layer.datasource = ds
layer.styles.append('My Style')

# render map
m.layers.append(layer)
m.zoom_all()

mapnik.render_to_file(m, 'world.png', 'png')

